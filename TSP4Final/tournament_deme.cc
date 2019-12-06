/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "cities.hh"
#include "climb_chromosome.hh"
#include "tournament_deme.hh"

#include <cassert>
#include <list>

//////////////////////////////////////////////////////////////////////////////
// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
// Uses tournament selection.
Chromosome* TournamentDeme::select_parent()
{
  static const unsigned TOURNAMENT_SIZE = 2;
  assert(TOURNAMENT_SIZE <= pop_.size());
  assert(TOURNAMENT_SIZE % 2 == 0);

  std::list<Chromosome*> pool;
  std::uniform_int_distribution<int> dist(0, pop_.size() - 1);

  for (auto i = TOURNAMENT_SIZE; i; --i) {
    pool.push_front(pop_[dist(generator_)]);
  }

  while (pool.size() > 1) {
    const auto p1 = pool.front(); pool.pop_front();
    const auto p2 = pool.front(); pool.pop_front();
    if (p1->get_fitness() > p2->get_fitness()) {
      pool.push_back(p1);
    } else {
      pool.push_back(p2);
    }
  }

  assert(pool.size() == 1);
  return pool.front();
}
