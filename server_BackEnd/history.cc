#include <sys/errno.h>
#include <stdexcept>
#include <string>
#include "history.h"



hist_fields::hist_fields()
{
  RateType[0]=0;
  YieldRate=0;
  BenchMark[0]=0;
}

hist_file::hist_file(const char* filename)
{
  record_count = 0;
  spread = false;
  strcpy(benchmark,"null");
  _file = std::fopen(filename, "r"); // open read-only
  if (!_file){
    fprintf(stderr,"ERROR couldn't open: %s\n", filename);
    throw std::runtime_error("file open failure");
  }
}

hist_file::~hist_file()
{
  if (std::fclose(_file)) 
    fprintf(stderr,"flose failed on file %s errno: %d\n", _file, errno);
}

int
hist_file::get_record_count()
{
  // not including comments (first char is #) that is...
  // Comments interspersed in data lines is allowed
  if (0==record_count)
    {
      int line_count_minus_comments = 0;
      int comment_count = 0;
      while(fgets(_line_buf,256,_file)){
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

hist_fields*
hist_file::get_records(int& length) 
{	
  length = get_record_count();
	
  _fields_array = new hist_fields[length];

  if(NULL == _fields_array){
    fprintf(stderr,"calloc failed - errno: %d\n", errno);
    return _fields_array;
  }
  int comment_count = 0;

  _line_buf[0] = ' ';

  int non_comments_line_count_read = 0; 
  char *token;

  while(std::fgets(_line_buf,256,_file)){

    //printf("SBB CHECK file line: %s\n", _line_buf);
    if('#' == _line_buf[0]) {
      comment_count++;
      continue; // skip through comments
    }

    // the line should be:
    // SecurityID Ticker SettlementDate CouponRate MaturityDate Frequency RateType YieldRate Quality Amount

    // Date(skip)
    token = strtok(_line_buf," ");
    if(NULL == token) { 
      fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
      exit(1); 
    } 

    // RateType
    token = strtok(NULL," ");
    if(NULL == token) { 
      fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
      exit(1); 
    } 
    strcpy(_fields_array[non_comments_line_count_read].RateType, token);
    if (0==non_comments_line_count_read && 0==strcmp(_fields_array[non_comments_line_count_read].RateType,"SPREAD")) 
      spread=true;
			     
    // YieldRate
    token = strtok(NULL," ");
    if(NULL == token ) { 
      fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
      exit(1); 
    }
    _fields_array[non_comments_line_count_read].YieldRate = atof(token);

		
    // BenchMark
    token = strtok(NULL," ");
    if(NULL == token) { 
      fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
      exit(1); 
    }
    int k=0;
    while (token[k] != 0)
      {
	k++;
      }
    if (token[k-1]=='\n') token[k-1]=0;
    strcpy(_fields_array[non_comments_line_count_read].BenchMark, token);
    if (0==non_comments_line_count_read && 0!=strcmp(_fields_array[non_comments_line_count_read].BenchMark,"NULL"))
      {
	benchmark[0]=0;
	strcpy(benchmark,token);
      }

    non_comments_line_count_read++;
  }

  //printf("SBB lines read: %d \n", non_comments_line_count_read);

  return _fields_array;
}
