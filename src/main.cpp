/*
  author: talmai.oliveira (talmai@talmai.com.br)
  file: main.cpp  
  A simple program that simulates 5 MM1 queues at an airport. The first queue
  feeds 3 other queues that work in parallel. These feed a final queue.
  Simulation runs until error below threshold and, at the end,
  a performance evaluation report is shown, and logs are generated
  for plotting and analysis.
*/

using namespace std;

#include <customer.hpp>
#include <mm1_queue.hpp>

void pause()
{
 std::string sInputString;
 // Wait for input from stdin (the keyboard)
 cout << "Continue (y)?";
 std::cin >> sInputString;
}

void test_random_number_generator()
{
    Random_Number_Generator r;
    r.set_rate(10.0);

    cout << "Recommended Seed Numbers [100.000 interval]" << endl;
    r.set_seed(1);
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    for (int i = 0; i < 100000; i++) r.next();
    cout << r.next() << endl;
    
    r.test();

    pause();
}

int main(int argc, char* argv[])
{
   int next = 0;
//   test_random_number_generator();

   for (int cur_lambda = 3; cur_lambda < 34; cur_lambda++)
   {
      
   MM1_Queue checkin;
   checkin.set_file_names("01_log.txt", "01_wait.txt", "01_service.txt");
   checkin.set_lambda(cur_lambda);
   checkin.set_mu(53);
   checkin.initialize();
   checkin.set_seed(1, 7.51391e+008);

   MM1_Queue security_gate01;
   security_gate01.set_file_names("02_log.txt", "02_wait.txt", "02_service.txt");
   security_gate01.set_lambda(cur_lambda/3);
   security_gate01.set_mu(20);
   security_gate01.autogenerate_new_arrivals(false);
   security_gate01.initialize();
   security_gate01.set_seed(5.648e+008, 1.42815e+009);

   MM1_Queue security_gate02;
   security_gate02.set_file_names("03_log.txt", "03_wait.txt", "03_service.txt");
   security_gate02.set_lambda(cur_lambda/3);
   security_gate02.set_mu(20);
   security_gate02.autogenerate_new_arrivals(false);
   security_gate02.initialize();
   security_gate02.set_seed(1.4853e+009, 1.77402e+009);

   MM1_Queue security_gate03;
   security_gate03.set_file_names("04_log.txt", "04_wait.txt", "04_service.txt");
   security_gate03.set_lambda(cur_lambda/3);
   security_gate03.set_mu(20);
   security_gate03.autogenerate_new_arrivals(false);
   security_gate03.initialize();
   security_gate03.set_seed(4.49834e+008, 2.35495e+008);

   MM1_Queue id_xray;
   id_xray.set_file_names("05_log.txt", "05_wait.txt", "05_service.txt");
   id_xray.set_lambda(cur_lambda);
   id_xray.set_mu(80);
   id_xray.autogenerate_new_arrivals(false);
   id_xray.initialize();
   id_xray.set_seed(6.92328e+008, 1.15022e+008);

//   for (; 
//       !checkin.is_within_confidence_interval() ||
//       !security_gate01.is_within_confidence_interval() ||
//       !security_gate02.is_within_confidence_interval() ||
//       !security_gate03.is_within_confidence_interval()
//       ;)
//   {
   for (; 
       !checkin.is_within_error_range(0.002)||
       !security_gate01.is_within_error_range(0.002) ||
       !security_gate02.is_within_error_range(0.002) ||
       !security_gate03.is_within_error_range(0.002)
       ;)
   {
       Customer cust = checkin.process_next_event();
       Customer cust2 = security_gate01.process_next_event();
       Customer cust3 = security_gate02.process_next_event();
       Customer cust4 = security_gate03.process_next_event();
       id_xray.process_next_event();
       if (cust.get_type() == Customer::COMPLETED())
       {
          switch(next)
          {
            case 0:
                 security_gate01.add_external_arrival();
                 break;
            case 1:
                 security_gate02.add_external_arrival();
                 break;
            case 2:
                 security_gate03.add_external_arrival();
                 break;
          }
          next++;
          if (next%3==0) next = 0;
       }
       if (cust2.get_type() == Customer::COMPLETED() || cust3.get_type() == Customer::COMPLETED() || cust4.get_type() == Customer::COMPLETED())
       {
          id_xray.add_external_arrival();
       }
   }

   cout << "Necessary simulation time: " << checkin.get_current_time() << endl;

   checkin.output();
   
   cout << "*********" << endl;

   security_gate01.output();

   cout << "*********" << endl;

   security_gate02.output();

   cout << "*********" << endl;

   security_gate03.output();

   cout << "*********" << endl;

   id_xray.output();
   
//   pause();

     checkin.plot_results_output();
   }

   return(0);
}

