#pragma once

#include <iostream>
#include "cal.h"
#include "yieldCurve.h"

#define SBB_LINE_BUFFER_LENGTH 256
#define SBB_INSTRUMENT_ID_LENGTH 32
#define SBB_INSTRUMENT_TICKER_LENGTH 32
#define SBB_INSTRUMENT_QUALITY_LENGTH 32

class SBB_calculator_interface;

// wide open class for loading data per instrument
class SBB_instrument_fields {
	public:
		SBB_instrument_fields();

		//show() maybe need to be adjusted
		void show();

		char* mf();

		char SecurityID[SBB_INSTRUMENT_ID_LENGTH];
		char Ticker[SBB_INSTRUMENT_TICKER_LENGTH];
		int SettlementDate;
		double CouponRate;
		int MaturityDate;
		char RateType[8];
		char Quality[SBB_INSTRUMENT_QUALITY_LENGTH];
		int Amount;
		short Frequency;
		double YieldRate;
};

// simple class for interfacing with data file that contains instrument descriptive data 
// (and yield)
class tradingBookCollection {

	// Resource (file handle) acquisition is initialization version - "RAII"
	// destructor closes file...
	public:
		tradingBookCollection(const char* filename);
		~tradingBookCollection();

		void import_curve(const char*);
		
		int getMonths(int,int);
		//
		// once his object is constructed (and the file is open), returns number of items in it
		//
		int get_record_count();
	
		//
		// returns a heap allocated array of file contents
		//
		SBB_instrument_fields* get_records(int& length);
			
		//
		// As a convenience to the caller memory is allocated inside this class
		// Call this to free it
		//
		void free_records();		
	
		int get_total_position();				//get the total position of the trading book
	     
	
	private:
		int record_count;
		std::FILE* _file;
		char _line_buf[SBB_LINE_BUFFER_LENGTH];
		SBB_instrument_fields *_fields_array;
		yield_curve* my_curve;			

		SBB_calculator_interface* construct_calculator(SBB_instrument_fields*);	 
		//
		// prevent copying
		// 
		tradingBookCollection (const tradingBookCollection &);
		
		//
		// prevent assignment
		//
		tradingBookCollection & operator= (const tradingBookCollection &);

		//
		// disable the default constructor
		// force construction with filename param only...
		//
		tradingBookCollection();
};
