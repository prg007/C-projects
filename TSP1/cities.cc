#include "cities.hh"
#include <iterator>
#include <random>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <time.h>
#include <random>

using namespace std;

//The below function >> has been declared as a free function. It reads the x and y co-ordinates of
// different cities from the file and puts them in a city object
istream& operator>> (std::istream& is, Cities& city)// implemented as a free function
{
	int n1; // x-coordinate
	int n2; //y-coordinate
	while (is>>n1) //Read X Co-ordinate	until no x-coordinates left to read, 
	{
		Cities::coord_t CurrentCoordinate;
		is>>n2;// Read Y Co-ordinate
		CurrentCoordinate.first = n1;
		CurrentCoordinate.second = n2;
		city.GetCityList().push_back(CurrentCoordinate);//Append the co-ordinates of that city to our vector object

	}
	return is;
}

//The below function << has been declared as a free function. It reads a list of all the city co-ordinate pairs,
//from a city object and provides them to an output stream for writing 

ostream& operator<< (std::ostream& os, Cities& city)
{
	
	for (auto city_coordinate:city.GetCityList()) // Get City List returns a vector object of type coord_t. Look
	{											  // inside the cities.hh header file.
		os <<city_coordinate.first<<" ";
		os<<city_coordinate.second<<"\n";
	}
	return os;
}

//Gives us the total distance traversed for a given ordering of cities. Uses STL function hypot
double Cities::total_path_distance(const permutation_t& ordering) const
{
	
	int x1,x2,y1,y2;
	double dist = 0.0;
	for (long unsigned int j = 0; j< ordering.size()-1; j++)// We need to keep iterations restricted to size - 2,  
	{														//as we are calculating distance by grabbing the next one is sequence
		x1 = ListOfCities[(ordering[j])].first;
		y1 = ListOfCities[(ordering[j])].second;
		x2 = ListOfCities[(ordering[j+1])].first;
		y2 = ListOfCities[(ordering[j+1])].second;
		dist = dist + hypot(x2-x1, y2-y1);
	}

	//Finally compute the distance between the first city in the permutation to the last one 
	dist = dist + hypot(ListOfCities[ordering[0]].first - ListOfCities[ordering[ordering.size()-1]].first,ListOfCities[ordering[0]].second - ListOfCities[ordering[ordering.size()-1]].second);
	return dist; // done so return 
}

//Does what the question asks us to do, creates a new city object that matches up with the ordering 
//and returns that particular object,  
Cities Cities::reorder(const permutation_t& ordering) const
{
	Cities NewCityObj;
	long unsigned j = 0;
	while (j < ordering.size())
	{
		NewCityObj.GetCityList().push_back(ListOfCities[ordering[j]]);
		j++;
	}
	return NewCityObj;
}

//Gives a random permutation of ordering of the cities from ) to len-1
Cities::permutation_t Cities::random_permutation(unsigned len)
{
	Cities::permutation_t NewOrdering;//Initially fill up the permutation with numbers 0 to the length(len)
	for(unsigned int i = 0; i < len; i++)
    {
        NewOrdering.push_back(i);
    }

    //After filling up that permutation,shuffle that permutation. Makes use of C++ STL algorithm shuffle
    //The technique used for shuffling has been borrowed from the below mentioned address
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();//This is a time based seed
	shuffle(NewOrdering.begin(),NewOrdering.end(), default_random_engine(seed));
    return NewOrdering;
    //borrowed from http://www.cplusplus.com/reference/random/uniform_int_distribution/operator()/
}


