I, "Prasun" worked alone on this project. In order to execute the program simply do the following in the VM terminal:
>>make
>>./tsp challenge.tsv 3000 0.1

Notice that I am using a population size of 3000 and a mutation rate of 0.1. I found that using smaller mutation rates of 0.1 and 0.2 produced much better results. I got shortest paths in the range of 7000-10000 with an average of high 7000's(approx). The minimum that I got was in the low 7000's. Maybe using even bigger population size would produce better results but it takes a long time to run. 

For my "local.tsv" file which simply modifies the mutate and clone method(Question2 of assignment), I got distances in the range of 17000- mid 18000's. A "comparison.gif" has been added that compares “baseline-ga.tsv”, “local.tsv”, and “tournament.tsv”. "tournament.tsv" has the best results as mentioned in the previous paragraph.

Note: You might see some code in comments when going through the files. Ignore them as they were simply used to test and print to see whether my code was working correctly as intended. Also, please be a little patient during the execution of ./tsp file. It takes a long time for 100000 iterations to execute.

A shortest.gif has also been added that plots out the shortest.tsv (shortest path)

Running valgrind showed no memory leaks.
Be very patient when running valgrind on a population size of 3000. You can run it for a smaller number of iterations(say 10000 or 20000) if you don't want to wait for upto a 100000 iterations.

Once again everything has been commented out extensively. Run make clean before using make.