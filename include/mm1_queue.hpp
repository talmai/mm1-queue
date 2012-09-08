/*
  author: talmai.oliveira (talmai@talmai.com.br)
  file: mm1_queue.hpp
  A MM1 queue object that, given lambda and mu, is capable
  of simulating a queue and evaluating results. This object
  can also be "plugged" in sequentially, creating queues
  that "feed" each other.
*/

#ifndef MM1_QUEUE_HPP
#define MM1_QUEUE_HPP

/* Header files needed for this program */
#include <iostream>
#include <vector>
#include <string>
#include <customer.hpp>
#include <random_number_generator.hpp>
#include <fstream>

/* Enable or disable the creation of the log file with detailed output */
#undef LOG_ENABLED

class MM1_Queue {
/*
  These variables are used to evaluate the performance of 
  the queue during or at the end of the simulation
*/
  int arrivals_; // total number of arrivals
  int services_; // total number of served customers
  double no_waiting_arrivals_; // used for idle calculation
  double min_arrival_interval_;
  double max_arrival_interval_;
  double min_service_duration_;
  double max_service_duration_;  
  double max_waiting_time_;
  double max_response_time_;
  double mean_queue_length_;
  double expected_queue_length_;
  double mean_number_customers_;
  double expected_number_customers_;
  double mean_waiting_time_;
  double expected_waiting_time_;
  double mean_response_time_;
  double expected_response_time_;
  double expected_idle_prob_;
  double expected_server_utilization_;

/*
  These variables are used for the simulation of the queue
*/  
  double next_arrival_;
  double next_departure_;
  bool busy_;
  double mql_upd_time_; // last update time of the mean_queue_length_
  double mnc_upd_time_; // last update time of the mean_number_customers_
  vector<Customer> waiting_buffer_;
  Random_Number_Generator rnd_arrival;
  Random_Number_Generator rnd_service;
  double current_time_;
  bool generate_new_arrivals_;
  int has_external_arrivals_;
  double lambda_;
  double mu_;
  bool initialized_;
  Customer serving_;

/*
  Log files
*/
#ifdef LOG_ENABLED
  ofstream log_outFile;
#endif
  ofstream mwt_outFile;
  ofstream mst_outFile;
    
/*
  Private methods
*/
Customer create_new_customer();
void update_evaluation_values();

public:
	MM1_Queue() : min_arrival_interval_(100000), max_arrival_interval_(0), 
                  arrivals_(-1), services_(-1), no_waiting_arrivals_(false), 
                  min_service_duration_(100000), max_service_duration_(0),
                  mean_queue_length_(0), mean_number_customers_(0), 
                  mean_waiting_time_(0), mean_response_time_(0), 
                  max_waiting_time_(0), max_response_time_(0),
                  next_arrival_(0), next_departure_(0), busy_(false), mql_upd_time_(0),
                  mnc_upd_time_(0), current_time_(0), generate_new_arrivals_(true),
                  has_external_arrivals_(1), lambda_(0), mu_(0), initialized_(false),
                  expected_queue_length_(0), expected_number_customers_(0), expected_waiting_time_(0),
                  expected_response_time_(0), expected_idle_prob_(0), expected_server_utilization_(0) {};

    /* Getters and Setters */
	void set_lambda(double d) { lambda_ = d; };
	double get_lambda() const { return lambda_; };
	void set_mu(double d) { mu_ = d; };
	double get_mu() const { return mu_; };
	double get_current_time() const { return current_time_; };
    void autogenerate_new_arrivals(bool b) { generate_new_arrivals_ = b; };
    int get_current_queue_size() const { return waiting_buffer_.size(); };
    bool is_busy() const { return busy_; };
    int get_number_arrivals() const { return arrivals_; };
    int get_number_services() const { return services_; };

	/* mean values */
	double get_mean_waiting_time() const { return mean_waiting_time_/services_; };
	double get_mean_response_time() const { return mean_response_time_/services_; };
	double get_mean_queue_length() const { return mean_queue_length_/current_time_; };
	double get_mean_number_customers() const { return (mean_queue_length_ + mean_number_customers_)/current_time_; };
	double get_server_utilization() const { return mean_number_customers_/current_time_; };
	double get_idle_prob() const { return no_waiting_arrivals_/arrivals_; };

	/* expected values */
	double get_expected_server_utilization() const { return expected_server_utilization_; };
	double get_expected_idle_prob() const { return expected_idle_prob_; };
	double get_expected_queue_length() const { return expected_queue_length_; };
	double get_expected_number_customers() const { return expected_number_customers_; };
	double get_expected_waiting_time() const { return expected_waiting_time_; };
	double get_expected_response_time() const { return expected_response_time_; };
	
	/* this next method returns TRUE if the simulation is
	   within the desired confidence interval */
	bool is_within_confidence_interval() const { 
         // 90%  confidence interval ->  z = 1.645
         // mean_var = { sum[ (xi)^2 - 2xixm + (xm)^2 ] } / n
         double var = ( pow(mean_response_time_, 2) 
                      - (2 * mean_response_time_ * expected_response_time_) 
                      + pow(expected_response_time_, 2) ) / services_;
         return get_mean_response_time() < expected_response_time_+(1.645*var) 
                &&
                get_mean_response_time() > expected_response_time_-(1.645*var)
                &&
                services_ > 30;
    };

	/* this next method returns TRUE if the simulation is
	   within the desired error range */
	bool is_within_error_range(double err) const { 
         return abs(get_mean_response_time() - expected_response_time_) < err;
    };

	/* definition of remaining public functions */
	void initialize();
    void set_file_names(string queue, string wait, string service);
	Customer process_next_event();
    Customer get_next_customer();
    void add_external_arrival();
    void set_seed(double arr, double serv);
    
    void output() const
    {
       cout << "Arrivals: " << arrivals_ << " Services: " << services_ << endl;
       cout << "Within 90\% CI? " << (is_within_confidence_interval()?"true":"false") << endl;
       cout << "Mean Response Time: " << get_mean_response_time() << " [" << get_expected_response_time() << "] " << endl;
       cout << "Mean Waiting Time: " << get_mean_waiting_time() << " [" << get_expected_waiting_time() << "] " << endl;
       cout << "Mean Queue Length: " << get_mean_queue_length() << " [" << get_expected_queue_length() << "] " << endl;
       cout << "Mean Number of Customers: " << get_mean_number_customers() << " [" << get_expected_number_customers() << "] " << endl;
       cout << "Server Utilization: " << get_server_utilization() << " [" << get_expected_server_utilization() << "] " << endl;
       cout << "Server Idle: " << get_idle_prob() << " [" << get_expected_idle_prob() << "] " << endl;
    }
    
    void plot_results_output() const
    {
       /* response time comparison */
       ofstream theory_rt;
       theory_rt.open("theory_rt.txt", ios::out|ios::app);
       theory_rt.precision(6);

       ofstream practice_rt;
       practice_rt.open("practice_rt.txt", ios::out|ios::app);
       practice_rt.precision(6);

       practice_rt << get_mean_response_time() << endl;
       theory_rt << get_expected_response_time() << endl;

       /* waiting time comparison */
       ofstream theory_wt;
       theory_wt.open("theory_wt.txt", ios::out|ios::app);
       theory_wt.precision(6);

       ofstream practice_wt;
       practice_wt.open("practice_wt.txt", ios::out|ios::app);
       practice_wt.precision(6);

       practice_wt << get_mean_waiting_time() << endl;
       theory_wt << get_expected_waiting_time() << endl;

       /* expected queue length comparison */
       ofstream theory_ql;
       theory_ql.open("theory_ql.txt", ios::out|ios::app);
       theory_ql.precision(6);

       ofstream practice_ql;
       practice_ql.open("practice_ql.txt", ios::out|ios::app);
       practice_ql.precision(6);

       practice_ql << get_mean_queue_length() << endl;
       theory_ql << get_expected_queue_length() << endl;

       /* mean number of customers comparison */
       ofstream theory_en;
       theory_en.open("theory_en.txt", ios::out|ios::app);
       theory_en.precision(6);

       ofstream practice_en;
       practice_en.open("practice_en.txt", ios::out|ios::app);
       practice_en.precision(6);

       practice_en << get_mean_number_customers() << endl;
       theory_en << get_expected_number_customers() << endl;

       /* utilization comparison */
       ofstream theory_u;
       theory_u.open("theory_u.txt", ios::out|ios::app);
       theory_u.precision(6);

       ofstream practice_u;
       practice_u.open("practice_u.txt", ios::out|ios::app);
       practice_u.precision(6);

       practice_u << get_server_utilization() << endl;
       theory_u << get_expected_server_utilization() << endl;

       /* idle comparison */
       ofstream theory_i;
       theory_i.open("theory_i.txt", ios::out|ios::app);
       theory_i.precision(6);

       ofstream practice_i;
       practice_i.open("practice_i.txt", ios::out|ios::app);
       practice_i.precision(6);

       practice_i << get_idle_prob() << endl;
       theory_i << get_expected_idle_prob() << endl;

    }
    
};

#endif
