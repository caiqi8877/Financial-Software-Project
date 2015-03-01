#pragma once

#define SBB_FIRST_VALID_YEAR 1900
#define SBB_ERROR -1

enum MonthName {
	NULL_MONTHNAME = 0,
	JANUARY,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

class SBB_date {
	public:
		SBB_date();

		// copy constructor
		SBB_date(const SBB_date& rhs);
		
		// assignment
		SBB_date& operator=(const SBB_date& rhs);

		// equality
		bool operator==(const SBB_date& rhs);

		// inequality
		bool operator!=(const SBB_date& rhs);

		void set_from_yyyymmdd(long yyyymmdd);

		long get_yyyymmdd();

		void show();

		void add_months(int number); // -6 passed to go backwards a semi-annual period
		int is_valid();

	private:
		char _month;
		char _day;
		short _year;
};
