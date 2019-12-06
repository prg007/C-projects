I, "Prasun", worked with Cruz Barnum on this project. Cruz was responsible for implementing the chromosome.cc file and I was responsible for implementing the deme.cc file, debugging and optimizing our shortest path. In order to execute the program simply do the following in the VM terminal:
>>make
>>./tsp challenge.tsv 80 0.2  (This runs the ga search, to run randomize only comment 				out line 135 and comment line 136)

On running tsp with a population of 80 and mutation rate of 0.2, we frequently got shortest paths below 10000. I want to discuss two key strategies that I have implemented in my deme.cc file to improve our shortest paths that might be difficult to understand just by looking at the code.

1> Inside the compute_next_generation() function in deme class, the parents are supposed to mutate when the mutation rate is less than the random value produced by the random number generator. I made a small modification wherein I chose the mutated parent only when the mutated parent had a higher fitness that the original unmutated parent. 

2> Inside the select_parent() function in deme.cc file, I implemented a slightly modified version of Roulette wheel. I realized that in order to get "fitter" children, I needed to choose fitter parents. The way to do this is to scan through the original vector of chrosomes and storing only those chromosomes to a new "Vector of Chromosomes" whose fitness values were greater than the average. Then implement the complete Roulette wheel strategy in this new upgraded "Vector of chromosomes". This resulted in a significant improvement in our shortest path and brought it down in the ballpark of 8000-10000
and in some cases below 8000.

Note: You might see some code in comments when going through the files. Ignore them as they were simply used to test and print to see whether our code was working correctly as intended. Also, please be a little patient during the execution of ./tsp file. It takes a long time for 100000 iterations to execute.

Note2: Comparison.gif, ga.tsv, randomized.tsv,shortest.tsv,shortest.gif have also been uploaded to Moodle.

Once again, everything has been commented out extensively. Reading and understanding them should be pretty straightforward.
Running valgrind showed no memory leaks either.


