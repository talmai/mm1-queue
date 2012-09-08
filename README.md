Airport Queues Simulation

*** Quick description ***

    The purpose of this project is to simulate the queues that an airline customer will have to face in order to be 
    able to board a plane. We assume that there is only one air line company in the airport. Furthermore, we
    assume that passengers' interarrival times are independent and identically distributed (IID) random variables
    with an exponential distribution with mean 1 / lambda. The service times are also assumed to be IID and exponentially
    distributed random variables with mean 1 / mu.

    Initially the passenger will have to pass through the check-in gate (M/M/1) where they will have to present
    a copy of their flight confirmation at the reception desk. Once inside the terminal building, due to security
    measures, all passengers must pass through security check. Passengers are split up into three different queues
    (M/M/1), where they will be throughly searched and checked. There is an airport security offcer that directs the
    passengers to each queue, guaranteeing that each queue gets the same amount of passengers. Finally, once inside
    the airport lounge, passengers can head towards their plane, where a quick check of identification documents
    and baggage x-ray (M/M/1) will be followed by boarding of the aircraft.

*** To compile: ***
           
    Compile all files at the same time. I am using Dev-C++ 4.9.9.2, so I only needed to add to the
    "project" all the files and configure the "INCLUDE" directory to see the "*.hpp" files, and the
    "COMPILE" parameters to see the "*.cpp" files.
    
    The main file is "main.cpp". All files are required to compile. The "*.hpp" files define and declare
    the objects. The "mm1_queue.cpp" declares additional methods.           

*** To create a queue: *** 
              
    MM1_Queue checkin;
> this creates a new instance of the object
    checkin.set_file_names("01_log.txt", "01_wait.txt", "01_service.txt");
> set_file_names is used to set the name of the files for posterior analysis (log)
    checkin.set_lambda(33);
    checkin.set_mu(53);
> sets lambda and mu which is used by the 2 internal random number generator objects
    checkin.initialize();
> necessary to calculate theoretical results (E[n], E[r], ...), and generate arrivals
    checkin.set_seed(1, 7.51391e+008);
> sets seeds for both internal random number generators

*** Stopping Criteria: *** 
         
    for (; !checkin.is_within_error_range(0.001);)
> this means that program will keep running until difference between expected E[r] and actual E[r] is less than given range

*** Handle events within queue: *** 
       
    Customer cust = checkin.process_next_event();
> this will process departure or arrival within queue. IF there is departure, then customer will be returned.

*** See output: *** 
    
    checkin.output();
> this will print to screen the results from queue    
