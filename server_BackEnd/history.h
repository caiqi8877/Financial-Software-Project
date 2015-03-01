#pragma once

class hist_fields
{
 public:
  hist_fields();

  char RateType[8];
  double YieldRate;
  char BenchMark[8];
  
};

class hist_file
{
 public:
  hist_file(const char* filename);
  ~hist_file();

  int get_record_count();
  
  hist_fields* get_records(int&);

  bool spread;

  char benchmark[8];

 private:
  int record_count;
  std::FILE* _file;
  char _line_buf[128];
  hist_fields* _fields_array;
};
