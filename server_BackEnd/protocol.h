#pragma once
#include "io.h"
#include "cal.h"
#include "clock.h"

extern SBB_instrument_fields* opening_records;
extern SBB_instrument_fields* closing_records; 
extern int opening_records_count;
extern int closing_records_count;
extern SBB_clock* timer;

typedef struct message
{
  char* category;
  int num;
};

class parser
{
 public:
  parser(const char*);
  message parse(char*); 
  char* handle(message);

 private:
  char* getTime();
  char* lgdChange();
  char* varChange(int);
  char* hedge(int);
  char* totalValChange(int);
  double T2_DV01;
  int getMonths(int,int);
  void calculate_PnL_vector(double*,int,SBB_calculator_interface*,int);
  void calculate_PnL_vector_spread(double*,int,SBB_calculator_interface*,int,char*);
  SBB_calculator_interface* construct_calculator(SBB_instrument_fields);
};

