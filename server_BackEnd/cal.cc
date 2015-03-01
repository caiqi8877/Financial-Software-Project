#include "cal.h"
#include "date.h"
#include "io.h"

int
SBB_calculator_interface::getPeriod(){
	if (0==period)
	{
		while (_start != _end) {
			_end.add_months(-12/frequency);
			period++;
		}
	}
	return period;
}

void 
SBB_calculator_interface::change_yield(double change)
{
  Yield_Rate = Yield_Rate + (change*1.0)/100;
}

void 
SBB_calculator_interface::set_yield(double val)
{
  Yield_Rate = val;
}

double
SBB_calculator_interface::get_yield()
{
  return Yield_Rate;
}

calculator_coupon_bearing::calculator_coupon_bearing(SBB_instrument_fields* r){
		      _start.set_from_yyyymmdd(r->SettlementDate);
		      _end.set_from_yyyymmdd(r->MaturityDate);
		      Coupon_Rate=r->CouponRate;
		      Yield_Rate=r->YieldRate;
		      frequency=r->Frequency;
		      amount=r->Amount;
		      period=0;
		      DV01=0;
}

calculator_zero_coupon::calculator_zero_coupon(SBB_instrument_fields* r){
		      _start.set_from_yyyymmdd(r->SettlementDate);
		      _end.set_from_yyyymmdd(r->MaturityDate);
		      Yield_Rate=r->YieldRate;
		      frequency=r->Frequency;
		      amount=r->Amount;
		      period=0;
		      DV01=0;
}


double
calculator_coupon_bearing::getBondPrice(){
			// calculate the price according to the formula (coupon-bearing)
			double temp = 1.0;
			for (int i=0;i<getPeriod();i++)
			{
				temp=temp*(1+Yield_Rate/100/frequency);
			}
			return (Coupon_Rate*(1-1/temp)/Yield_Rate+1/temp)*100;
}

double
calculator_zero_coupon::getBondPrice(){
			// calculate the price according to the formula (zero-coupon)
			double temp = 1.0;
			for (int i=0;i<getPeriod();i++)
			{
				temp=temp*(1+Yield_Rate/100/frequency);
			}
			return 100/temp;
}

double
calculator_coupon_bearing::getDV01(){
			if (0 == DV01)
			{
				// 1 bp change to yield up
				double temp = 1.0;
				for (int i=0;i<getPeriod();i++)
				{
					temp=temp*(1+(Yield_Rate+0.01)/100/frequency);
				}
				double bp_up = (Coupon_Rate*(1-1/temp)/(Yield_Rate+0.01)+1/temp)*100;
	
				// 1 bp change to yield down
				temp = 1.0;
				for (int i=0;i<getPeriod();i++)
				{
					temp=temp*(1+(Yield_Rate-0.01)/100/frequency);
				}		
				double bp_down = (Coupon_Rate*(1-1/temp)/(Yield_Rate-0.01)+1/temp)*100;

				// get average
				return (bp_down - bp_up)/2;
			}
			return DV01;
}

double
calculator_zero_coupon::getDV01(){
		if (0 == DV01)
		{
			// 1 bp change to yield up
			double temp = 1.0;
			for (int i=0;i<getPeriod();i++)
			{
				temp=temp*(1+(Yield_Rate+0.01)/100/frequency);
			}
			double bp_up = 100/temp; 

			// 1 bp change to yield down
			temp = 1.0;
			for (int i=0;i<getPeriod();i++)
			{
				temp=temp*(1+(Yield_Rate-0.01)/100/frequency);
			}
			double bp_down = 100/temp;

			// get average
			DV01=(bp_down - bp_up)/2;
		}
		return DV01;
}

double
calculator_zero_coupon::getRisk(){
			return getDV01()*amount/100; 
}

double
calculator_coupon_bearing::getRisk(){
			return getDV01()*amount/100;
}
