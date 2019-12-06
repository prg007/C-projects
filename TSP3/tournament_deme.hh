#pragma once
#include "deme.hh"
#include "climb_chromosome.hh"
#include "cities.hh"
#include <random>
#include <iostream>
#include <vector>
using namespace std;

class TournamentDeme : public Deme
{
public: 
	TournamentDeme(const Cities* cities_ptr, unsigned pop_size, double mut_rate);
    ~TournamentDeme()
    {
    	//Do nothing
    }
protected:
	virtual Chromosome* select_parent() override;
};
