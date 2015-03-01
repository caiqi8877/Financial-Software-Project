Hi, 

Our team has three members and our names and N numbers are as follows:
Xinyuan Hu  N16537725
Jiawei Dong N10485340
Qi Cai      N14450379

For execution:
1. cd to the directory
2. make all
3. ./run.sh (start the server)
4. python client.py (open the client)
(all the results will be presented on stdout)

All the history files are in the ./var directory.
Opening and closing trading book files, as well as the yield curve file are in the ./input directory. The yield curve file I used is the oldest version(not the mid-term version). You can replace these files with new ones for test. But please make sure their names remain the same. 

The confidence interval I chose is 99%, so the program will pick the smallest element in the PnL vector as the VaR given the length of the PnL vector is 99.

The platform was MAC OS X, the compiler was g++

If you have any problem testing my program, please contact me through NYU Email(xh499@nyu.edu)
Thank you for taking your precious time grading my homework~