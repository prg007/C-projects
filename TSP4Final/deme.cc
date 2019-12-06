/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "cities.hh"
#include "climb_chromosome.hh"
#include "deme.hh"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

//////////////////////////////////////////////////////////////////////////////
// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
 : pop_(pop_size), mut_rate_(mut_rate), generator_(rand())
{
  // Create random Chromosomes and put into population vector
  for (auto& cp : pop_) {
    cp = new ClimbChromosome(cities_ptr);
  }
}

// Clean up as necessary
Deme::~Deme()
{
  for (auto cp : pop_) {
    delete cp;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  auto newpop = pop_;
  assert(pop_.size() % 2 == 0 && "Even population size required!");

  for (unsigned i = 0; i < pop_.size(); ) {
    auto p1 = select_parent();
    auto p2 = select_parent();

    static std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(generator_) <= mut_rate_) {
      p1->mutate();
    }
    if (dist(generator_) <= mut_rate_) {
      p2->mutate();
    }

    auto children = p1->recombine(p2);
    newpop[i++] = children.first;
    newpop[i++] = children.second;
  }

  for (auto cp : pop_) {
    delete cp;
  }
  std::swap(pop_, newpop);
}

//////////////////////////////////////////////////////////////////////////////
// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  assert(!pop_.empty());
  return *std::max_element(pop_.cbegin(), pop_.cend(), [](auto cp1, auto cp2){
      return cp1->get_fitness() < cp2->get_fitness(); });
}

//////////////////////////////////////////////////////////////////////////////
// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
// Uses roulette-wheel selection
Chromosome* Deme::select_parent()
{
  // Figure out what the total sum of fitness in pop_ is:
  const double total_fitness = std::accumulate(pop_.cbegin(), pop_.cend(), 0.,
      [](double sum, auto cp){ return sum + cp->get_fitness(); });

  // Pick a random fitness sum value to stop the wheel at:
  static std::uniform_real_distribution<> dist(0.0, 1);
  const double threshold = total_fitness * dist(generator_);

  // Now, find the first chromosome whose partial sum of fitness exceeds threshold:
  double fit_sum = 0.; // Total fitness so far

  const auto it = std::find_if(pop_.cbegin(), pop_.cend(), [&](auto cp) {
        fit_sum += cp->get_fitness();
        return fit_sum >= threshold;
   });
  assert(it != pop_.cend());

  return *it;
}


