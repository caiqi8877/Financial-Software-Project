#include "yieldCurve.h"
#include "io.h"
#include "date.h"

yield_curve::yield_curve(const char* f)
{
	tradingBookCollection* yield_curve_input=new tradingBookCollection(f);
	SBB_instrument_fields* records = yield_curve_input->get_records(curve_count);
	if (curve_count<=0)
		printf("no benchmarks in the yield_curve input file");
	else 
	{
		my_benchmarks = new benchmark[curve_count];
		for (int i=0;i<curve_count;i++)
		{
			my_benchmarks[i].yieldRate=records[i].YieldRate;
			my_benchmarks[i].maturity=get_months(records[i].SettlementDate,records[i].MaturityDate);
		}
	}
	sort_benchmarks();
	yield_curve_input->free_records();
	delete yield_curve_input;
}

int
yield_curve::get_months(int s,int e)
{
	SBB_date start,end;
	start.set_from_yyyymmdd(s);
	end.set_from_yyyymmdd(e);
	int m=0;
	while (start!=end)
	{
		end.add_months(-1);
		m++;
	}
	return m;
}

double
yield_curve::getNearestYield(int m)
{
	return getNearestYield(m,0,curve_count-1,0);	
}

double
yield_curve::getNearestYield(int m,int a,int b,int flag)
{
	if (0==flag)
	{
		if (my_benchmarks[a].maturity>=m) return my_benchmarks[a].yieldRate;
		if (my_benchmarks[b].maturity<=m) return my_benchmarks[b].yieldRate;
	}
	if (1==b-a) {if (((my_benchmarks[a].maturity+my_benchmarks[b].maturity)/2)>=m) return my_benchmarks[a].yieldRate; else return my_benchmarks[b].yieldRate;}
	if (my_benchmarks[(a+b)/2].maturity==m) return my_benchmarks[(a+b)/2].yieldRate;
	if (my_benchmarks[(a+b)/2].maturity>m) return getNearestYield(m,a,(a+b)/2,1); else return getNearestYield(m,(a+b)/2,b,1);
}

void 
yield_curve::sort_benchmarks()
{
	for (int i=curve_count-1;i>=0;i--)
		for (int j=0;j<i;j++)
		{
			if (my_benchmarks[j].maturity > my_benchmarks[j+1].maturity)
			{
				swap(my_benchmarks[j].maturity,my_benchmarks[j+1].maturity);
				swap(my_benchmarks[j].yieldRate,my_benchmarks[j+1].yieldRate);	
			} 
		}
}

void 
yield_curve::swap(int& a,int& b)
{
  if(a!=b)
    { 
	a=a+b;
	b=a-b;
	a=a-b;
    }
}

void
yield_curve::swap(double& a,double& b)
{
	if(a!=b)	
	{
		a=a+b;
		b=a-b;
		a=a-b;
	}
}











