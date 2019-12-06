/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include <iostream>
#include <vector>
#include "chromosome.hh"
#include "deme.hh"
#include <random>
#include <algorithm>

using namespace std;
// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
  mut_rate_ = mut_rate;//mut_rate was also pre-defined

  for (unsigned index = 0; index < pop_size ; index++) // builds a deme with a population of pop_size individuals
  {
  	auto Individual = new Chromosome(cities_ptr); //Creates a population vector of type Chromosome by the variable name pop_
  	pop_.push_back(Individual);
  }
  //Creates a population vector of 

}

// properly removes each chromosome from the vector and dealocates them
void cleanup(vector<Chromosome *> v){
    for(Chromosome * c : v){
        delete c;
    }
    v.clear();
}

// Clean up as necessary
Deme::~Deme()
{
    cleanup(pop_); //Deallocates the memory consumed by each individual
}
  	

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
 vector<Chromosome*> newGen;
  for (unsigned int index = 0;index < (pop_.size())/2; index++)
  {
    uniform_real_distribution<double> fn(0.0, 1.0);
    double random1 = fn(generator_); //Implicit Type Conversion, Generating a random number between 0 and 1
    double random2 = fn(generator_);

    auto FirstParent = select_parent(); //Picking two parents at random using the Roulette wheel strategy.
    auto SecondParent = select_parent(); 


    auto fp = FirstParent; //Creating an alias of First Parent. This will come in handy when we don't want the 
                          //mutated value of our parent. Read README.txt to understand what is going on.

    if (random1 < mut_rate_)
    {
        FirstParent->mutate();
        if(FirstParent->get_fitness()<fp->get_fitness())//Allow FirstParent to mutate only when its mutated value is higher
           FirstParent = fp;
      
    }

    auto sp = SecondParent;//Creating an alias of Second Parent. This will come in handy when we don't want the 
                          //mutated value of our parent. Read README.txt to understand what is going on.



    if(random2 < mut_rate_)
    {
        SecondParent->mutate();
        if(SecondParent->get_fitness()<sp->get_fitness())//Allow SecondParent to mutate only when its mutated value is higher
           SecondParent = sp;
      
    }
    
   
    auto Children = FirstParent->recombine(SecondParent); //Creating a new Generation of Children by 
    newGen.push_back(Children.first);                     // recombining the parents  using recombine function from chromosome.cc 
    newGen.push_back(Children.second);                    // and adding them to the vector newGen
    

  }
    

  
   cleanup(pop_); //Delete every parent in the old generation. We want this to be replaced by a new generation of children
  pop_ = newGen; //Finally set the new generation of children to be our parents in the next generation
  newGen.clear();
}


// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  auto CurrentBestIndividual = pop_[0];//Initially setting the first individual to be the most fit
  double CurrentBestFitScore = CurrentBestIndividual->get_fitness();
  
  for (unsigned int index = 0; index < pop_.size();index++)//This loop is pretty straightforward, finds the maximum fitness by checking fitness 
  {                                                        //of every element in the vector
  	if(pop_[index]->get_fitness()>CurrentBestFitScore)    //Compares the currrent best fitness with the fitness of this element
  	{
  		CurrentBestFitScore = pop_[index]->get_fitness();
  		CurrentBestIndividual = pop_[index];
  	}
  }

  return CurrentBestIndividual;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.

//Implements the Roulette Wheel Selection
Chromosome* Deme::select_parent()
{
  auto sum = 0.0; //Used to calculate the sum of all fitness values in pop_ vector 
  
for (unsigned int index = 0;index < pop_.size(); index++)
  {
    sum = sum + pop_[index]->get_fitness();
  }

vector<Chromosome*> newobj;  //New Vector which will consist of only those values greater than average
double avg = sum/pop_.size(); //Calculate the average of all fitness values in the original vector. We are only gonna choose elements bigger than our average fitness. 

copy_if(pop_.begin(), pop_.end(), back_inserter(newobj), //This function is complicated, what it basically says it cherrypick only those
    [=](Chromosome * chm){return chm->get_fitness() > avg;});//values from the original vector whose fitness values are greater than avg and put it in the new vector newobj


//We are now gonna implement Roulette Wheel strategy on our new vector newobj as described in the link given in Moodle HW
auto sum1 = 0.0;
for (unsigned int index = 0;index < newobj.size(); index++)//Step 1 of Roulette Wheel, Calculate the sum of all fitness values in our new vector "newobj"
  {
    sum1 = sum + newobj[index]->get_fitness();
  }

  uniform_real_distribution<double> fn(0.0, sum1); //Step 2 of Roulette Wheel, Generate a random number between 0 and calculated vector sum
  auto RandomNumber = fn(generator_); 

  double PartialSum = 0;
  for (unsigned int index = 0; index < newobj.size(); index++)
  {
  	PartialSum = PartialSum + newobj[index]->get_fitness();//Step3 of Roulette Wheel, Starting from the top of the population, 
                                                            //keep adding the finesses to the partial sum 
  	if(PartialSum > RandomNumber)//Step 4 of Roulette Wheel
  	{
  		return newobj[index];
  	}
  }
  return newobj[newobj.size()-1];
  //Only reaches here if Random Number is in the last Partial Sum and hence we should return the last element.
  

}
