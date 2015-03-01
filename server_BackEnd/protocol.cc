#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>
#include <vector>
#include <algorithm>
#include "protocol.h"
#include "io.h"
#include "cal.h"
#include "date.h"
#include "ratings.h"
#include "history.h"

using namespace std;


parser::parser(const char* filename)
{
  tradingBookCollection* mybook = new tradingBookCollection(filename);
  int count=0;
  SBB_instrument_fields* records = mybook->get_records(count);
  SBB_calculator_interface* cal;
  for (int i=0;i<count;i++)
    {
      if (0 == strcmp(records[i].SecurityID,"T2")) 
	{
	  cal=construct_calculator(records[i]);
	  T2_DV01=cal->getDV01();
	  delete cal;
	  cal = NULL;
	  break;
	}
    }
}

message
parser::parse(char* msg)
{
  message temp;
  temp.category = new char[8];
  temp.category[0] = 0;
  temp.num=0;

  char* token;
  //category
  token = strtok(msg," ");
  if (NULL == token) 
    {
      fprintf(stderr,"message parsing failed\n");
      exit(1);
    }
  strcpy(temp.category,token);
  //num
  token = strtok(NULL," ");
  if (NULL == token) 
    {
      fprintf(stderr,"message parsing failed\n");
      exit(1);
    }
  temp.num = atoi(token);

  return temp;
}


char*
parser::handle(message msg)
{
  if (0 == strcmp(msg.category,"hedge")) {delete[] msg.category; return hedge(msg.num);}
  if (0 == strcmp(msg.category,"valup")) {delete[] msg.category; return totalValChange(msg.num);}
  if (0 == strcmp(msg.category,"valdw")) {delete[] msg.category; return totalValChange(-1*msg.num);}
  if (0 == strcmp(msg.category,"lgddf")) {delete[] msg.category; return lgdChange();}
  if (0 == strcmp(msg.category,"wtime")) {delete[] msg.category; return getTime();}
  if (0 == strcmp(msg.category,"vardf")) {delete[] msg.category; return varChange(msg.num);}
  fprintf(stderr,"no such command\n");
  exit(1);
}

char*
parser::getTime()
{
  double real_time,user_time,sys_time=0;
  timer->end_clock(real_time,user_time,sys_time);
  char* temp = new char[64];
  temp[0]=0;
  strcpy(temp,"wtime");

  char* rtime=new char[16];
  char* utime=new char[16];
  char* stime=new char[16];
  rtime[0]=0;
  utime[0]=0;
  stime[0]=0;
  sprintf(rtime,"%.3f",real_time);
  sprintf(utime,"%.3f",user_time);
  sprintf(stime,"%.3f",sys_time);

  strcat(strcat(strcat(strcat(strcat(strcat(temp," "),rtime)," "),utime)," "),stime);
  delete[] rtime;
  delete[] utime;
  delete[] stime;
  return temp;
}

char*
parser::hedge(int bucket)
{
  int high=0;
  int low=0;
  switch (bucket)
    {
    case 2:
      high=24;
      low=0;
      break;

    case 5:
      high=60;
      low=24;
      break;

    case 10:
      high=120;
      low=60;
      break;

    case 30:
      high=360;
      low=120;
      break;

    default:
      fprintf(stderr,"no such bucket\n");
      exit(1);
    }
  double risk_sum=0;
  int months=0;
  SBB_calculator_interface* myCal;
  for (int i=0;i<closing_records_count;i++)
    {
      myCal = construct_calculator(closing_records[i]);
      months = getMonths(closing_records[i].SettlementDate,closing_records[i].MaturityDate);
      if ((low < months) && ((high >= months) || (high==360))) 
	risk_sum = risk_sum + myCal->getRisk();
      delete myCal;
      myCal = NULL;
    }
  char* temp= new char[24];
  char* number = new char[16];
  temp[0]=0;
  number[0]=0;

  strcpy(temp,"hedge");
  sprintf(number,"%.3f",risk_sum/T2_DV01*100);
  strcat(strcat(temp," "),number);

  delete[] number;
  return temp;
}

char*
parser::totalValChange(int change)
{
  SBB_calculator_interface* cal;
  double sum=0;
  for (int i=0;i<closing_records_count;i++)
    {
      cal = construct_calculator(closing_records[i]);
      cal->change_yield(change);
      sum = sum + cal->getBondPrice() / 100 * closing_records[i].Amount;
      delete cal;
      cal = NULL;
    }
  
  char* temp = new char[24];
  char* number = new char[16];
  temp[0]=0;
  number[0]=0;
  
  if (change>0) 
    strcpy(temp,"valup");
  else 
    strcpy(temp,"valdw");
  
  sprintf(number,"%.3f",sum);
  strcat(strcat(temp," "),number);

  delete[] number;
  return temp;
}

char*
parser::varChange(int confident)
{
  double open_var,close_var=0;
  char filename[16];
  filename[0]=0;
  strcpy(filename,"./var/");
  strcat(filename,closing_records[0].SecurityID);
  strcat(filename,".txt");
  hist_file* test = new hist_file(filename);
  hist_fields* test_records;
  int vector_length=0;
  test_records = test->get_records(vector_length);
  vector_length--;
  vector<double> sum_PnL_vector(vector_length,0);
  for (int i=0;i<closing_records_count;i++)
    {
      filename[0]=0;
      strcpy(filename,"./var/");
      strcat(filename,closing_records[i].SecurityID);
      strcat(filename,".txt");
      if (0 == strcmp(closing_records[i].RateType,"YIELD"))
	{
	  hist_file* my_hist = new hist_file(filename);
	  hist_fields* hist_records;
	  int count = 0;
	  hist_records = my_hist->get_records(count);
	  double yield_changes[count-1];
	  for (int j=0;j<count-1;j++)
	    yield_changes[j] = hist_records[j+1].YieldRate - hist_records[j].YieldRate;
	  calculate_PnL_vector(yield_changes,count-1,construct_calculator(closing_records[i]),closing_records[i].Amount);
	  for (int j=0; j<vector_length; j++)
	    {
	      sum_PnL_vector[j] = sum_PnL_vector[j] + yield_changes[j];
	    }
	}
      else 
	{
	  hist_file* my_hist = new hist_file(filename);
	  hist_fields* hist_records;
	  int count = 0;
	  hist_records = my_hist->get_records(count);
	  double yield_changes[count-1];
	  for (int j=0;j<count-1;j++)
	    yield_changes[j] = hist_records[j+1].YieldRate - hist_records[j].YieldRate;
	  calculate_PnL_vector_spread(yield_changes,count-1,construct_calculator(closing_records[i]),closing_records[i].Amount,my_hist->benchmark);
	  for (int j=0; j<vector_length; j++)
	    {
	      sum_PnL_vector[j] = sum_PnL_vector[j] + yield_changes[j];
	    }
	}
    }
  sort(sum_PnL_vector.begin(),sum_PnL_vector.end());
  int index = (100-confident)*1.0/100*vector_length;
  close_var = sum_PnL_vector[index];
  




  filename[0]=0;
  strcpy(filename,"./var/");
  strcat(filename,opening_records[0].SecurityID);
  strcat(filename,".txt");
  delete test_records;
  delete test;
  test_records = NULL;
  test = NULL;
  test = new hist_file(filename);
  vector_length=0;
  test_records = test->get_records(vector_length);
  vector_length--;
  vector<double> sum_PnL_vector_open(vector_length,0);
  for (int i=0;i<opening_records_count;i++)
    {
      filename[0]=0;
      strcpy(filename,"./var/");
      strcat(filename,opening_records[i].SecurityID);
      strcat(filename,".txt");
      if (0 == strcmp(opening_records[i].RateType,"YIELD"))
	{
	  hist_file* my_hist = new hist_file(filename);
	  hist_fields* hist_records;
	  int count = 0;
	  hist_records = my_hist->get_records(count);
	  double yield_changes[count-1];
	  for (int j=0;j<count-1;j++)
	    {
	      yield_changes[j] = hist_records[j+1].YieldRate - hist_records[j].YieldRate;
	    }
	  calculate_PnL_vector(yield_changes,count-1,construct_calculator(closing_records[i]),opening_records[i].Amount);
	  for (int j=0; j<vector_length; j++)
	    {
	      sum_PnL_vector_open[j] = sum_PnL_vector_open[j] + yield_changes[j];
	    }
	}
      else 
	{
	  hist_file* my_hist = new hist_file(filename);
	  hist_fields* hist_records;
	  int count = 0;
	  hist_records = my_hist->get_records(count);
	  double yield_changes[count-1];
	  for (int j=0;j<count-1;j++)
	    yield_changes[j] = hist_records[j+1].YieldRate - hist_records[j].YieldRate;
	  calculate_PnL_vector_spread(yield_changes,count-1,construct_calculator(closing_records[i]),opening_records[i].Amount,my_hist->benchmark);
	  for (int j=0; j<vector_length; j++)
	    {
	      sum_PnL_vector_open[j] = sum_PnL_vector_open[j] + yield_changes[j];
	    }
	}
    }


  sort(sum_PnL_vector_open.begin(),sum_PnL_vector_open.end());
  index = (100-confident)*1.0/100*vector_length;
  open_var = sum_PnL_vector_open[index];


  char* temp = new char[24];
  char* number = new char[16];
  temp[0]=0;
  number[0]=0;

  strcpy(temp,"vardf");

  sprintf(number,"%.3f",close_var - open_var);
  strcat(strcat(temp," "),number);

  delete[] number;
  return temp;  
}

char* 
parser::lgdChange()
{
  SBB_bond_ratings myBondRating;
  double opening_lgd_sum,closing_lgd_sum = 0;
  for (int i=0;i<opening_records_count;i++)
    opening_lgd_sum = opening_lgd_sum + myBondRating.LGD_given_SnP_Fitch(opening_records[i].Quality) * opening_records[i].Amount;
  for (int i=0;i<closing_records_count;i++)
    closing_lgd_sum = closing_lgd_sum + myBondRating.LGD_given_SnP_Fitch(closing_records[i].Quality) * closing_records[i].Amount;
  char* temp = new char[24];
  char* number = new char[16];
  temp[0]=0;
  number[0]=0;

  strcpy(temp,"lgddf");
  
  sprintf(number,"%.3f",closing_lgd_sum - opening_lgd_sum);
  strcat(strcat(temp," "),number);

  delete[] number;
  return temp;
}

SBB_calculator_interface*
parser::construct_calculator(SBB_instrument_fields record)
{
  if (0 == record.CouponRate) 
    return (new calculator_zero_coupon(&record));
  else 
    return (new calculator_coupon_bearing(&record));
}

int
parser::getMonths(int start,int end)
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

void 
parser::calculate_PnL_vector(double* change_array, int length, SBB_calculator_interface* my_cal, int amount)
{
  double origin_price = my_cal->getBondPrice();
  double origin_yield = my_cal->get_yield();
  for (int i=0;i<length;i++)
    {
      my_cal->change_yield(change_array[i]*100);
      change_array[i] = (my_cal->getBondPrice()-origin_price) * amount / 100;
      my_cal->set_yield(origin_yield);
    }
}

void 
parser::calculate_PnL_vector_spread(double* change_array, int length, SBB_calculator_interface* my_cal, int amount, char* benchmark)
{
  char filename[16];
  filename[0]=0;
  strcpy(filename,"./var/");
  strcat(filename,benchmark);
  strcat(filename,".txt");
  hist_file* my_hist = new hist_file(filename);
  hist_fields* bench_records;
  int bench_records_length = 0;
  bench_records = my_hist->get_records(bench_records_length);
  for (int i=0;i<length;i++)
    {
      change_array[i] = change_array[i] + (bench_records[i+1].YieldRate - bench_records[i].YieldRate)*100;
    }
  double origin_price = my_cal->getBondPrice();
  double origin_yield = my_cal->get_yield();
  for (int i=0;i<length;i++)
    {
      my_cal->set_yield(origin_yield);
      my_cal->change_yield(change_array[i]);
      change_array[i] = (my_cal->getBondPrice()-origin_price) * amount / 100;
    }
  
}
