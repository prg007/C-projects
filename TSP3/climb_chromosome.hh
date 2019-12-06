#pragma once
#include "chromosome.hh"
#include <random>
#include <iostream>
#include <cassert>
using namespace std;

class ClimbChromosome : public Chromosome
{
public:
	void mutate() override; //Override ensures that function signatures have to be same when overriding
	
	virtual ClimbChromosome* clone() const override // const ensures nothing changes, use of overrife has already been explained
	{
		return new ClimbChromosome(*this); 
	}

	~ClimbChromosome() = default; //No significant change from our original Chromosome class

	ClimbChromosome(const Cities* cities) : Chromosome(cities) //You need to feed the values to the class you are inheriting from
	{
		// Do nothing 
	}
};
