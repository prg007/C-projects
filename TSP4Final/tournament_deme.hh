/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.
 * TournamentDeme uses tournament selection.
 */

#pragma once

#include "chromosome.hh"
#include "cities.hh"
#include "deme.hh"

#include <random>

class TournamentDeme : public Deme {
 public:
  // Generate a Deme of the specified size with all-random chromosomes.
  // Also receives a mutation rate in the range [0-1].
  TournamentDeme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
    : Deme(cities_ptr, pop_size, mut_rate)
  {}

  virtual ~TournamentDeme() = default;

 protected:
  // Randomly select a chromosome in the population based on fitness and
  // return a pointer to that chromosome.
  virtual Chromosome* select_parent() override;
};
