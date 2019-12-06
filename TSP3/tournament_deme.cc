#include "tournament_deme.hh"
#include <cassert>
#include <numeric>
#include <list>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;

TournamentDeme::TournamentDeme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
: Deme( cities_ptr, pop_size, mut_rate)
{

}


Chromosome* TournamentDeme::select_parent()
{

vector<pair<Chromosome*,Chromosome*> > elements;//The vector on which we will implement Tournament Selection

//The idea is to choose P parents at Random.
//The idea is that the number of chosen parents are always in between 1/16 - 1/32 of the total population
 

 static int LowerIndex = pop_.size()/32;
 static int HigherIndex = pop_.size()/16;
 int i = 1;

 //The following while loop ensures that P is ALWAYS a power of 2 in between 1/16 - 1/32 of the original
 //population

 while(true)
 {
 	if (i>=LowerIndex and i<=HigherIndex)
 		break;
 	i = i*2;
 }
 static unsigned int P = max(i,8); //Makes sure that the number of chosen parents are atleast 8 


vector<unsigned int> v;//We always want to choose unique parents. This vector ensures that the IndexNumber
					   //or in other words the parent chosen is always unique

pair<Chromosome*, Chromosome*> Candidates; //Posseses 2 parents which will me made to fight. Groups of 2 
										   //parents are added to the elements vector on which Tournament Sort
										   //is implemented

for (unsigned j = 0;j <P/2; j++)
{
	uniform_int_distribution<unsigned> dist(0, pop_.size()-1);
	unsigned int ParentOneIndex = dist(generator_);//Choose a RANDOM Parent
	while ((std::count(v.begin(), v.end(), ParentOneIndex))) //This loop makes sure that this parent hasn't
	{														 //already been chosen.
		ParentOneIndex = dist(generator_);
	}
	v.push_back(ParentOneIndex);							//Add this parent to the list of unique parents already chosen
	

	unsigned int ParentTwoIndex = dist(generator_);			//Same stuff as in the last loop
	while ((std::count(v.begin(), v.end(), ParentTwoIndex)))
	{
		ParentTwoIndex = dist(generator_);
	}
	v.push_back(ParentTwoIndex);

	auto RandomParent1 = pop_[ParentOneIndex];	  //Group Parent1 and Parent2 together using the Candidates pair declared
	auto RandomParent2 = pop_[ParentTwoIndex];	  //above the loop
	Candidates.first = RandomParent1;
	Candidates.second = RandomParent2;
	elements.push_back(Candidates);				  //Add this group of 2 elements in the resulting vector on which we will implement
												  //Tournament Selection
}



vector<pair<Chromosome*,Chromosome*> > Newelements; //This is a new vector that is always half the size of elements vector and 
													//contains the winners from the previous iteration of tournament sort

pair<Chromosome*, Chromosome*> NewCandidates;



	while(true) //Keep runnning as long as there are more than 2 elements 
	{
		
		if (elements.size() == 1)//If only a single pair exists, pick the better parent and exit the loop
		{
			
			if (elements[0].first->get_fitness()>= elements[0].second->get_fitness()) 
			{
				
	            return elements[0].first;
	        }
	        else 
	        {
	        	
	            return elements[0].second;
	        }
		}

		
		for (unsigned k = 0; k < elements.size()-1; k = k + 2)
		{
			if (elements[k].first->get_fitness()>= elements[k].second->get_fitness()) //Pick the winner from current pair
			{																		  //make it the first element of the pair
																					  // which will be used in the next iteration
	            NewCandidates.first = elements[k].first;
	        }
	        else 
	        {
	        	
	            NewCandidates.first = elements[k].second;
	        }
	        

			if (elements[k+1].first->get_fitness()>= elements[k+1].second->get_fitness())//Pick the winner from current pair
			{																			 //make it the second element of the pair
																						 // which will be used in the next iteration
	            NewCandidates.second = elements[k+1].first;
	        }
	        else 
	        {
	        	
	            NewCandidates.second = elements[k+1].second;
	        }
	        Newelements.push_back(NewCandidates);

		}
		elements.swap(Newelements); //Finally swap the two vectors so that elements vector contains only the winners from this round.

		Newelements.clear(); //Clear out the current vector as this will be reused in the next iteration



	}


elements.clear();//Redundant statement. Elements is a vector so no chance of memory leak

}