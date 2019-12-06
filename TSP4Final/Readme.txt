I, "Prasun" worked alone on this project. In order to execute the program simply do the following in the VM terminal:
>>make
>>./tsp challenge.tsv 1000 0.3 4
On running tsp with a population size of 1000 , mutation rate of 0.3 and number of threads = 4, I got a minimum distance of 5490. 

Num_iter = default(8,000,000)
pop_size = 1000
Mutation Rate = 0.3
Number of Threads = 4
Average Shortest Distance with 4 threads = Mid - High 5000's
Time Taken with 4 threads = 25-30 s 

Shortest Distance with a single thread = mid 6000's
Time taken = 60-70 s

Run make clean before using make.
Once again, running valgrind showed no memory errors.
I have replaced the original shortest.tsv with my shortest.tsv
Another file called "best.tsv" has been added that represents my shortest tour
-pthread has been added to Makefile