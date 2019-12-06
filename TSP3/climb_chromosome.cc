#include "climb_chromosome.hh"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <random>
using namespace std;

void ClimbChromosome::mutate()
{
	ClimbChromosome* SecondChromosome = this->clone();
	ClimbChromosome* ThirdChromosome = this->clone();

	//Chooses a random point p in the chromosome (Question2)
	uniform_int_distribution<int> dist(0, order_.size() - 1);
    unsigned RandomPoint = dist(generator_); // Step2: Picks a random point 

	double CurrentFitness = get_fitness(); //Step 1: evaluating the fitness of current chromosome
	auto CurrentOrder = this->order_;

	if (RandomPoint != 0) //Step3 of Question
	{
		swap(SecondChromosome->order_[RandomPoint], SecondChromosome->order_[RandomPoint-1]);
	}
	else
	{
		swap(SecondChromosome->order_[RandomPoint], SecondChromosome->order_[SecondChromosome->order_.size()-1]);
	}

	double SecondFitness = SecondChromosome->get_fitness(); //Evaluating the Fitness of Second Chromosome

	swap(ThirdChromosome->order_[(RandomPoint + 1) % (ThirdChromosome->order_.size()-1)], ThirdChromosome->order_[RandomPoint]); 
	//Step4: Takes Care of both cases in Point 4 using the modulo % operator.	
	double ThirdFitness = ThirdChromosome->get_fitness();

	//Rest of the code implements Step5
	if(CurrentFitness>SecondFitness)
	{
		if(ThirdFitness>CurrentFitness)// Third has the Highest Fitness
		{
			this->order_ = ThirdChromosome->order_; //Sets the current ordering to the ordering of Chromosome 3
			delete SecondChromosome;     //Don't want memory leaks
			delete ThirdChromosome;
		}	
		if(CurrentFitness>ThirdFitness) // First has the Highest Fitness
		{
			delete SecondChromosome;   //Don't want memory leaks
			delete ThirdChromosome;			
		}
		
	}
	else if(SecondFitness>ThirdFitness) //2>3 and 2>1 , so 2 is highest
	{
		this->order_ = SecondChromosome->order_;
		delete SecondChromosome;	//Don't want memory leaks
		delete ThirdChromosome;
	}
	else //2>1 and 3>2 , so 3 is highest
	{
		this->order_ = ThirdChromosome->order_;
		delete SecondChromosome; //Don't want memory leaks
		delete ThirdChromosome;
	}

}