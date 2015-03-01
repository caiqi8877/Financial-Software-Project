#pragma once

#include "date.h"
#include "io.h"

class SBB_instrument_fields;

class SBB_calculator_interface{
	public:	virtual double getBondPrice() {};

		virtual double getDV01() {};

		virtual double getRisk() {};

		void change_yield(double);
		
		void set_yield(double);

		double get_yield();

	protected:
	        int getPeriod();
	        SBB_date _start, _end;
	        double Coupon_Rate, Yield_Rate, DV01;
	        short frequency;
	        int period;
		int amount;
};

class calculator_coupon_bearing:public SBB_calculator_interface{
	public:
		calculator_coupon_bearing(SBB_instrument_fields*);

		double getBondPrice();
		
		double getDV01();

		double getRisk();
	
};

class calculator_zero_coupon:public SBB_calculator_interface{
	public:
		calculator_zero_coupon(SBB_instrument_fields*);

		double getBondPrice();

		double getDV01();

		double getRisk();
	
};
