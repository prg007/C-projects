/* Implementation for Chromosome class
*/

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"
using namespace std;
//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{

    //std::random_shuffle(order_.begin(), order_.end(), generator_);
    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
    assert(is_valid());
   
    // cities is not deleted until the end of the program because it is shared between each Chromosome
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{

    if(order_.size() < 2){
        return;
    }

    unsigned rand1 = generator_() % order_.size();
    unsigned rand_raw2 = generator_() % (order_.size() - 1);
    unsigned rand2 = (rand1 > rand_raw2) ? rand_raw2 : rand_raw2 + 1;
                                        // ensures that rand2 has an equal chance of being any random number not equal to the first

    std::swap(order_[rand1], order_[rand2]);    // this function scares me

    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{

    assert(is_valid());
    assert(other->is_valid());

    auto range_begin = generator_() % order_.size();
    auto raw_range_end   = generator_() % order_.size() - 1;
    auto range_end = (raw_range_end < range_begin) ?  raw_range_end : raw_range_end + 1;

    Chromosome * child1 = create_crossover_child(this, other, range_begin, range_end);
    Chromosome * child2 = create_crossover_child(other, this, range_begin, range_end);

    assert(child1->is_valid());
    assert(child2->is_valid());



    return std::pair(child1, child2);
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
  return 10000.0 / (cities_ptr_->total_path_distance(order_) + 1.0);
                            // since the distance is always a positive number
                            // and smaller solutions are better, this should 
                            // always output a positive solution, not return 
                            // a 1 / 0 error, and have shorter paths have higher fitnesses
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
    Cities::permutation_t order_copy(order_.size());
    std::copy(order_.cbegin(), order_.cend(), order_copy.begin());
    std::sort(order_copy.begin(), order_copy.end());
    for(long unsigned int i = 0; i < order_.size(); i++){
        if(static_cast<unsigned int>(i) != order_copy[i]){
            return false;
        }
    }
    return true;
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
    return (order_.cbegin() + end) != std::find(order_.cbegin() + begin, order_.cbegin() + end, value);  // uses std:: find to check if it contains value
}
