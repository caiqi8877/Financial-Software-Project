#pragma once

class benchmark
{
	public:
		double yieldRate;
		int maturity;
		
};

class yield_curve
{
	public:
		yield_curve(const char*);
		double getNearestYield(int);

	private:
		benchmark* my_benchmarks;
		void sort_benchmarks();
		void swap(int&,int&);
		void swap(double&,double&);
		int get_months(int,int);
		int curve_count;
		double getNearestYield(int,int,int,int);
		 
};
