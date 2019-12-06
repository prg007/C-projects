/*
 * Implementation for ClimbChromosome class
 */

#include <cassert>

#include "climb_chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
ClimbChromosome::mutate()
{
  static std::uniform_int_distribution<unsigned> dist(0, order_.size() - 1);
  auto fitness = get_fitness();

  const int idx = dist(generator_);
  const int prev = idx > 0? idx - 1 : order_.size() - 1;
  const int next = (idx + 1) % (order_.size() - 1);

  // Try swapping with prev first:
  std::swap(order_[idx], order_[prev]);
  if (fitness > get_fitness()) {  // The swap worsend the fitness, undo:
    std::swap(order_[idx], order_[prev]);
  } else {
    fitness = get_fitness();
  }

  // Now try swapping with next:
  std::swap(order_[idx], order_[next]);
  if (fitness > get_fitness()) {  // The swap worsend the fitness, undo:
    std::swap(order_[idx], order_[next]);
  }

  assert(is_valid());
}
