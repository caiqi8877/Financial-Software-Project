#include <string>
#include <sys/errno.h>
#include <unistd.h>
#include <stdexcept>
#include "io.h"
#include "date.h"
#include "ratings.h"

void 
SBB_instrument_fields::show()
{
			printf("ID: %s SettleDate: %d Coupon: %.2f MatDate: %d Frequency: %d Yield: %.3f\n",
				SecurityID,
				SettlementDate,
				CouponRate,
				MaturityDate,
				Frequency,
				YieldRate);
}

SBB_instrument_fields::SBB_instrument_fields(){
			SecurityID[0]=0;
			Ticker[0]=0;
			RateType[0]=0;
			Quality[0]=0;
			Amount=0;
			SettlementDate=0;
			CouponRate=0.0;
			MaturityDate=0;
			Frequency=0;
			YieldRate=0.0;
} 

char*
SBB_instrument_fields::mf()
{
			char* id= new char[64];
			char* date = new char[32];
			char* rate = new char[32];
			id[0]=0;
			date[0]=0;
			rate[0]=0;
			
			sprintf(rate,"%.3f",CouponRate);
			sprintf(date,"%d",MaturityDate);
			
			strcat(strcat(strcat(strcat(strcat(id,Ticker),"_"),rate),"_"),date);
			
			delete[] date;
			delete[] rate;
			
			return id;
}

tradingBookCollection::tradingBookCollection(const char * filename)
{
	record_count = 0;
	_file = std::fopen(filename, "r"); // open read-only
	if (!_file){
		fprintf(stderr,"ERROR couldn't open: %s\n", filename);
		throw std::runtime_error("file open failure");
	}
}

tradingBookCollection::~tradingBookCollection() 
{
	if (std::fclose(_file)) {
		fprintf(stderr,"flose failed on file %s errno: %d\n", 
			_file, errno);
	}
}

void 
tradingBookCollection::import_curve(const char* f)
{
	my_curve = new yield_curve(f);
}

int
tradingBookCollection::get_record_count()
{
	// not including comments (first char is #) that is...
	// Comments interspersed in data lines is allowed
	if (0==record_count)
	{
		int line_count_minus_comments = 0;
		int comment_count = 0;
		while(fgets(_line_buf,SBB_LINE_BUFFER_LENGTH,_file)){
			if('#' == _line_buf[0]) {
				comment_count++;
				continue; // skip through comments
			}
			++line_count_minus_comments;
		}
		//printf("comment count in file: %d\n", comment_count);
	
		rewind(_file);
		//fseek(_file, 0L, SEEK_SET);
		record_count =  line_count_minus_comments;
	}
	return record_count;
}

void
tradingBookCollection::free_records()
{
	delete[] _fields_array;
}

int
tradingBookCollection::get_total_position()
{
	int sum=0;
	SBB_instrument_fields *temp = _fields_array;
	for (int i=0;i<get_record_count();i++)
	{	
		sum=sum+temp->Amount;
		temp++;
	} 
	return sum;
}

SBB_instrument_fields*
tradingBookCollection::get_records(int& length) 
{	
	length = get_record_count();
	
	_fields_array = new SBB_instrument_fields[length];

	if(NULL == _fields_array){
		fprintf(stderr,"calloc failed - errno: %d\n", errno);
		return _fields_array;
	}
	int comment_count = 0;

	_line_buf[0] = ' ';

	int non_comments_line_count_read = 0; 
	char *token;

	while(std::fgets(_line_buf,SBB_LINE_BUFFER_LENGTH,_file)){

		//printf("SBB CHECK file line: %s\n", _line_buf);
		if('#' == _line_buf[0]) {
		  comment_count++;
		  continue; // skip through comments
		}

		// the line should be:
		// SecurityID Ticker SettlementDate CouponRate MaturityDate Frequency RateType YieldRate Quality Amount

		// SecurityID
		token = strtok(_line_buf," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("SecurityID: %s\n", token);
		strcpy(_fields_array[non_comments_line_count_read].SecurityID, token);

		// Ticker
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("Ticker: %s\n", token);
		strcpy(_fields_array[non_comments_line_count_read].Ticker, token);
	
		// SettlementDate
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("SettlementDate: %s\n", token); 
		_fields_array[non_comments_line_count_read].SettlementDate = atoi(token);

		// CouponRate
		token = strtok(NULL," ");
		if(NULL == token ) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("CouponRate: %s\n", token);
		_fields_array[non_comments_line_count_read].CouponRate = atof(token);

		// MaturityDate
		token = strtok(NULL," ");
		//printf("MaturityDate: %s\n", token);
		_fields_array[non_comments_line_count_read].MaturityDate = atoi(token);				

		// Frequency
		token = strtok(NULL," ");
		//printf("Frequency: %s\n", token);
		_fields_array[non_comments_line_count_read].Frequency = atoi(token);				

		// RateType
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("RateType: %s\n", token);
		strcpy(_fields_array[non_comments_line_count_read].RateType, token);

		// Yield (later we will load in spread and a string token of an OTR. Also will have a separate file

		token = strtok(NULL," ");
		if(NULL == token ) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		if (0==strcmp(_fields_array[non_comments_line_count_read].RateType,"YIELD"))	
		{	
			//printf("YieldRate is:  %s\n", token);
			_fields_array[non_comments_line_count_read].YieldRate = atof(token);
		}
		else 
		{	
			_fields_array[non_comments_line_count_read].YieldRate = atof(token)*0.01 + my_curve->getNearestYield(getMonths(_fields_array[non_comments_line_count_read].SettlementDate,_fields_array[non_comments_line_count_read].MaturityDate));
		}
		
		// Quality
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("Quality: %s\n", token);
		strcpy(_fields_array[non_comments_line_count_read].Quality, token);
		
		
		// Amount
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("Amount: %s\n", token);
		_fields_array[non_comments_line_count_read].Amount=atoi(token);

		non_comments_line_count_read++;
	}

	//printf("SBB lines read: %d \n", non_comments_line_count_read);

	return _fields_array;
}

int
tradingBookCollection::getMonths(int start,int end)
{
	SBB_date s,e;
	s.set_from_yyyymmdd(start);
	e.set_from_yyyymmdd(end);
	int count=0;
	while (s!=e)
	{
		count++;
		e.add_months(-1);
	}
	return count;
}

SBB_calculator_interface*
tradingBookCollection::construct_calculator(SBB_instrument_fields* record)
{
	if (0==record->CouponRate) return (new calculator_zero_coupon(record));
		else return (new calculator_coupon_bearing(record));
}
