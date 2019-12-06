#include "cities.hh"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;

int main(int argument_count, char** argv)
{
	Cities city_obj; //Creating city_obj
	const int ITERATIONS = 1000000; 
	if(argument_count != 2)// Ensures that we have the appropiate number of files i.e. not too many or too few files
	{
		cerr<<"Need exactly one command line argument \n"; //Display an error message 
		return -1; 
	}

	//Gets the file name ,turns it into a stream and loads it onto the city object  
	auto InputFile = argv[1];
	auto fin = ifstream(InputFile);
	fin>>city_obj;
 
	auto best_so_far = city_obj.random_permutation(city_obj.GetCityList().size());// Initially we set the shortest length found to a random permutation
																				  // There are 2^(size -1) different possible options for generating a random permutation.
																				
	for (int i = 0; i<ITERATIONS; i++)
	{
		// generate new permutation at every iteration
		auto current = city_obj.random_permutation(city_obj.GetCityList().size());

		// Updates the shortest distance, if a better permutation is found.
		if (city_obj.total_path_distance(current)< city_obj.total_path_distance(best_so_far))
		{
			best_so_far = current;
			cout<<i<<"\t"<<city_obj.total_path_distance(best_so_far)<<"\n";
		}
	}

	city_obj = city_obj.reorder(best_so_far);// Set the permutation that gives us the minimum path, using the reorder function defined in cities.cc file
	
	//Finally create an output stream to a new file and write that city object to that particulat file("shortest.tsv")
	ofstream OutputFIle("shortest.tsv");
	OutputFIle << city_obj;
	OutputFIle.close();
	return 0;
}