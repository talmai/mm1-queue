/*
  author: talmai.oliveira (talmai@talmai.com.br)
  file: mm1_queue.cpp
  A MM1 queue object that, given lambda and mu, is capable
  of simulating a queue and evaluating results. This object
  can also be "plugged" in sequentially, creating queues
  that "feed" each other.
*/

using namespace std;

#include <mm1_queue.hpp>

/*
  Used to update the evaluation values. Both the mean queue length as well 
  as the mean number of customer are calculated based on time averages. Every
  time the queue is processed for a new event, we check if it is busy and 
  if there is anyone in the waiting buffer. If there is, we update the
   accumulation of the queue length and number of customers. The actual
   mean queue length is later obtained by dividing the accumulation by the
   current time. And the actual mean number of customers, by adding the 
   accumulation of the mean queue length with the accumulation of the mean
   number of customers and dividing it by the current time:
          
	double get_mean_queue_length() const { return mean_queue_length_/current_time_; };
	double get_mean_number_customers() const { return (mean_queue_length_ + mean_number_customers_)/current_time_; };   
*/
void MM1_Queue::update_evaluation_values()
{
   mean_queue_length_ += waiting_buffer_.size()*(current_time_ - mql_upd_time_);
   mql_upd_time_ = current_time_;
   mean_number_customers_ += (busy_ ? (current_time_ - mnc_upd_time_):0);
   mnc_upd_time_ = current_time_;
}

/*
  Used to create a new customer arrival event
*/
Customer MM1_Queue::create_new_customer()
{
    Customer cust;
    cust.set_id(arrivals_++);
    cust.set_type(Customer::ARRIVAL());
    return cust;
}

/*
  Used to create files for log of queue activity
*/
void MM1_Queue::set_file_names(string log, string wait, string service)
{ 
#ifdef LOG_ENABLED
    log_outFile.open(log.c_str(), ios::out);
    log_outFile.precision(6);
#endif
    mwt_outFile.open(wait.c_str(), ios::out);
    mwt_outFile.precision(6);
    mst_outFile.open(service.c_str(), ios::out);
    mst_outFile.precision(6);
}

/*
  Sets the seed of the random variables
*/
void MM1_Queue::set_seed(double arr, double ser)
{
     rnd_arrival.set_seed(arr);
     rnd_service.set_seed(ser);
}

/*
  Creates instances of the Random Variate Generator and
  also the first arrival event...
*/
void MM1_Queue::initialize()
{
    if (!initialized_)
    {
      expected_server_utilization_ = lambda_/mu_;
      expected_idle_prob_ = 1 - expected_server_utilization_;
      expected_queue_length_ = (expected_server_utilization_*expected_server_utilization_)/(1-expected_server_utilization_);
      expected_number_customers_ = expected_server_utilization_/(1-expected_server_utilization_);
      expected_waiting_time_ = expected_server_utilization_/(mu_-lambda_);
      expected_response_time_ = 1/(mu_-lambda_);

      rnd_arrival.set_rate(lambda_);
      rnd_service.set_rate(mu_);

      if (generate_new_arrivals_)
      {
         // Scheduling first arrival:
         Customer cust = create_new_customer();
         next_arrival_ = rnd_arrival.exponential();
      }
      else
      {
          has_external_arrivals_ = 0;
      }
      
     // We use next_departure_ to either process a departure event
     // or a arrival event...
     next_departure_ = next_arrival_ + 1; // means no scheduled event...
     initialized_ = true;
    }
}

void MM1_Queue::add_external_arrival() 
{
     double interval = rnd_arrival.exponential();
     next_arrival_ += interval; // scheduling the next arrival
     if (interval > max_arrival_interval_) max_arrival_interval_ = interval;
     if (interval < min_arrival_interval_) min_arrival_interval_ = interval;
#ifdef LOG_ENABLED
     log_outFile << "E " << current_time_ << ":" << next_arrival_ << endl;
#endif
     has_external_arrivals_++; 
};

/*
  Looks for the next event by time 
*/
Customer MM1_Queue::get_next_customer()
{
    Customer next_customer;
    double next_time = 100000; /* bug: if all events greater than this number?... */
	for (int i=0; i < waiting_buffer_.size(); i++)
	{
        if (waiting_buffer_[i].get_arrival() < next_time)
        {
           next_time = waiting_buffer_[i].get_arrival();
           next_customer = waiting_buffer_[i];
        }
    }
    vector<Customer>::iterator startIterator;
    for( startIterator = waiting_buffer_.begin(); startIterator != waiting_buffer_.end(); startIterator++ )
    {
         if (((Customer)*startIterator).get_id() == next_customer.get_id())
         {
            waiting_buffer_.erase( startIterator );
            break;
         }
    }
    return next_customer;
}

Customer MM1_Queue::process_next_event()
{
    double interval;
    if (next_arrival_ < next_departure_)
    {
     // process arrival...
     current_time_ = next_arrival_; // advance simulation time
     update_evaluation_values();
     Customer cust;
     if (has_external_arrivals_ > 0)
     {
         cust = create_new_customer();
         // if externally generated arrivals, then the next_arrival_ has
         // already been set!
         if (generate_new_arrivals_)
         {
             interval = rnd_arrival.exponential();
             next_arrival_ += interval; // scheduling the next arrival
             if (interval > max_arrival_interval_) max_arrival_interval_ = interval;
             if (interval < min_arrival_interval_) min_arrival_interval_ = interval;
         }
         else
         {
             has_external_arrivals_--;
         }
     }
     else
     {
         return cust;
     }
     if (!busy_)
     {
        no_waiting_arrivals_++;
        busy_ = true;
        interval = rnd_service.exponential();
        next_departure_ = current_time_ + interval; // scheduling the end of service
        if (interval > max_service_duration_) max_service_duration_ = interval;
        if (interval < min_service_duration_) min_service_duration_ = interval;
        cust.set_arrival(current_time_);//{ arrival time of the served customer }
        cust.set_service(next_departure_);
#ifdef LOG_ENABLED
        log_outFile << "S "; cust.log(log_outFile);
#endif
        serving_ = cust;
     }
     else
     {
         // server is busy, queue...
         cust.set_arrival(current_time_);
         waiting_buffer_.push_back(cust);
#ifdef LOG_ENABLED
        log_outFile << "Q "; cust.log(log_outFile);
#endif
     }
    }
    else
    {   
      // process departures..
      current_time_ = next_departure_; // advance simulation time
      update_evaluation_values();
      services_++;
      serving_.set_departure(current_time_);
      serving_.set_type(Customer::COMPLETED());
      mwt_outFile << fixed << serving_.get_wait_time() << endl;        
      mst_outFile << fixed << serving_.get_service_time() << endl;
#ifdef LOG_ENABLED
      log_outFile << "D "; serving_.log(log_outFile);
#endif
      mean_response_time_ += current_time_ - serving_.get_arrival();
      if (serving_.get_service() > max_response_time_) max_response_time_ = serving_.get_service();
      busy_ = false;
      next_departure_ = next_arrival_ + 1; // means no scheduled event...
      // check waiting queue for customers...
      if (waiting_buffer_.size() > 0)
      {
        busy_ = true;
        interval = rnd_service.exponential();
        next_departure_ = current_time_ + interval; // scheduling the end of service
        if (interval > max_service_duration_) max_service_duration_ = interval;
        if (interval < min_service_duration_) min_service_duration_ = interval;
        serving_ = get_next_customer();
        serving_.set_service(current_time_);
#ifdef LOG_ENABLED
        log_outFile << "S "; serving_.log(log_outFile);
#endif
        mean_waiting_time_ += serving_.get_wait_time();
        if (serving_.get_wait_time() > max_waiting_time_) max_waiting_time_ = serving_.get_wait_time();
     }
    }
  return serving_;
}
