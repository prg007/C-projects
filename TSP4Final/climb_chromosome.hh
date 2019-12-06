/*
 * Declarations for ClimbChromosome class to be used in a (GA) to
 * approximate the travelling-salesperson problem (TSP).
 * ClimbChromosome is a Chromosome (see chromosome.hh) that overrides
 * the mutation operator with local hill climbing.
 */

#pragma once

#include "cities.hh"
#include "chromosome.hh"

class ClimbChromosome : public Chromosome {
 protected:
  // Disable public copying of objects for polymorphism:
  ClimbChromosome(const ClimbChromosome&) = default;
  ClimbChromosome(ClimbChromosome&&) = default;
  ClimbChromosome& operator=(const ClimbChromosome&) = default;
  ClimbChromosome& operator=(ClimbChromosome&&) = default;

 public:
  // Creation method for new Chromsomoe. Saves a copy of the cities and
  // generates a completely random permutation from a list of cities.
  ClimbChromosome(const Cities* cities) : Chromosome(cities) {}

  virtual ~ClimbChromosome() = default;

  // Polymorphic creation method from an existing ClimbChromosome.
  // This method allocates memory for the newly created ClimbChromosome.
  // It is the caller's responsibility to free this memory.
  virtual Chromosome* clone() const override
  {
    return new ClimbChromosome(*this);
  }

  // Perform a single mutation on this ClimbChromosome
  virtual void mutate() override;
};
