
I, "Prasun", worked alone on this project. I have declared 2 free functions istream& operator>> and ostream& operator<<. Other private variables(vectors) and functions(such as accessors) have been added in the header file to be used in the implementation. The technique used for shuffling in random_permutation has been borrowed from the following source- "http://www.cplusplus.com/reference/random/uniform_int_distribution/operator()/". Every function inside this cities.hh and cities.cc files have been explained in great length so reading and understanding what is going on inside these functions should be pretty straightforward. I am making use of STL algorithms such as shuffle for use in random_permutation.

speed.tsv, speed.gif, shortest.tsv , shortest.gif have all been provided with the submission. 
A close look at the shortest.gif shows that our random iteration method tried its best to find out the shortest path; however there are way too many crossing(intersections) between cities in the shortest.gif file. Even after running multiple simulations, there were still a lot of points where the paths intersected. Hopefully, we will be able to improve on this in our future assignments. 

Note: I have reset the axis range in my speed.gif file to capture the whole of x-axis and y-axis. ( X-axis being a million iterations and Y-axis being the shortest path found)

Note: I have a Makefile to make things easier to run on the terminal. Simply execute the commands in a way the question asks them to do. 