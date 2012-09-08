/*
  author: talmai.oliveira (talmai@talmai.com.br)
  file: customer.hpp
  This class is a wrapper to help simplify the handling of the
  data generated from the simulation. 
*/

#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

/* Header files needed for this program */
#include <iostream>
#include <fstream>

class Customer {
	int id_;
	char type_;
	double arrival_;
	double service_;
	double departure_;

public:
	Customer() : id_(-1), type_(Customer::UNDEFINED()), arrival_(-1), service_(-1), departure_(-1){};

	/* pseudo-constants */
	static char ARRIVAL() 				{ return 0x00; }; /* 0000 0000 */
	static char DEPARTURE() 			{ return 0x10; }; /* 0001 0000 */
	static char UNDEFINED() 			{ return 0x20; }; /* 0010 0000 */
	static char COMPLETED() 			{ return 0x30; }; /* 0011 0000 */

	/* getters and setters */
	void set_id(int i) { id_ = i; };
	int get_id() const { return id_; };
	void set_type(char c) { type_ = c; };
	char get_type() const { return type_; };
	void set_arrival(double d) { arrival_ = d; };
	double get_arrival() const { return arrival_; };
	void set_service(double d) { service_ = d; };
	double get_service() const { return service_; };
	void set_departure(double d) { departure_ = d; };
	double get_departure() const { return departure_; };
	
	void log(std::ofstream &log)
	{
         log << get_id() << " [A]" << get_arrival() << " [S]" << get_service() << " [D]" << get_departure() << endl;
	};
	
	double get_wait_time()
    {
           return service_ - arrival_;
    };

	double get_service_time()
    {
           return departure_ - service_;
    };

	double get_response_time()
    {
           return departure_ - arrival_;
    };
};

#endif
