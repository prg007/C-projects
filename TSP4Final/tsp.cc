/*
 * Main file for Traveling-Salesperson solver.
 * Read a cities file in TSV format into a Cities object, then run
 * of several solver algorithms on it and output the best result.
 */

#include "cities.hh"
#include "tournament_deme.hh"

#include <atomic>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <numeric>
#include <thread>
#include <mutex>

//////////////////////////////////////////////////////////////////////////////
// Check whether a specific ordering reduces the total path distance in cities
// from a previous best distance. If so, print out the iteration number and
// new distance, update the best distance, and return true.
// Returns false if no improvement was found.
bool
is_improved(const Cities& cities,
            const Cities::permutation_t& ordering,
            double& best_dist,
            uint64_t iter)
{
  const auto dist = cities.total_path_distance(ordering);
  if (dist < best_dist) {
    std::cout << iter << "\t" << dist << std::endl;
    best_dist = dist;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches niter randomized operdinges on the given cities
// The best cities permutation is returned.
Cities::permutation_t
randomized_search(const Cities& cities, uint64_t niter)
{
  auto best_ordering = Cities::permutation_t(cities.size());
  auto best_dist = 1e100;

  for (uint64_t i = 0; i < niter; ++i) {
    auto ordering = random_permutation(cities.size());
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  }

  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////
// threaded_randomized_search is a multi-threaded wrapper of exhaustive_search
Cities::permutation_t
threaded_randomized_search(const Cities& cities,
                           uint64_t niter,
                           unsigned nthread = 1)
{
  auto best_ordering = Cities::permutation_t(cities.size());
  auto best_mutex = std::mutex();

  auto run_one_thread = [&]() {
    auto my_best = randomized_search(cities, niter / nthread);
    if (cities.total_path_distance(my_best) < cities.total_path_distance(best_ordering)) {
      auto guard = std::lock_guard(best_mutex);
      // Repeat check, maybe something changed:
      if (cities.total_path_distance(my_best) < cities.total_path_distance(best_ordering)) {
        best_ordering = my_best;
      }
    }
  };

  std::vector<std::thread> threads;
  for (unsigned i = 0; i < nthread; ++i) {
    threads.push_back(std::thread(run_one_thread));
  }

  for (auto& t : threads) {
    t.join();
  }

  return best_ordering;

}

//////////////////////////////////////////////////////////////////////////////
// granular_exhaustive_search is a multi-threaded wrapper of exhaustive_search
// Unliked threaded_randomized search, you can set the granularity of each
// search step in this function.
Cities::permutation_t
granular_randomized_search(const Cities& cities,
                           uint64_t niter,
                           unsigned nthread = 1,
                           unsigned granularity = 100)
{
  auto best_ordering = Cities::permutation_t(cities.size());
  auto best_mutex = std::mutex();
  std::atomic<unsigned> evaluated = 0;

  auto run_one_thread = [&]() {
    while (evaluated < niter) {
      auto my_best = randomized_search(cities, granularity);
      evaluated += granularity;
      if (cities.total_path_distance(my_best) < cities.total_path_distance(best_ordering)) {
        auto guard = std::lock_guard(best_mutex);
        // Repeat check, maybe something changed:
        if (cities.total_path_distance(my_best) < cities.total_path_distance(best_ordering)) {
          best_ordering = my_best;
        }
      }
    }
  };

  std::vector<std::thread> threads;
  for (unsigned i = 0; i < nthread; ++i) {
    threads.push_back(std::thread(run_one_thread));
  }

  for (auto& t : threads) {
    t.join();
  }

  return best_ordering;

}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches every single permutation of the cities.
// The best cities permutation is returned.
Cities::permutation_t
exhaustive_search(const Cities& cities)
{
  auto ordering = Cities::permutation_t(cities.size());
  std::iota(ordering.begin(), ordering.end(), 0);
  auto best_ordering = ordering;
  auto best_dist = 1e100;
  uint64_t i = 0;

  do {
    i++;
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  } while (std::next_permutation(ordering.begin(), ordering.end()));

  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////
// ga_search uses a genetic algorithm to solve the traveling salesperson
// problem for a given list of cities.
// This function then creates a randomly generated population of permutations
// for traveling to those cities.
// The function also requires a population size and mutation rate.
// indicates how aggressively the population's individuals mutate.  The
// function then repeatedly evolves the population to generate increasingly
// better (i.e. shorter total distances) city permutations.
// The best cities permutation is returned.

Cities::permutation_t
ga_search(const Cities& cities,
          unsigned iters,
          unsigned pop_size,
          double mutation_rate,
          unsigned nthread = 1)
{
  auto best_dist = 1e100 + nthread; // Eliminate silly warning
  auto best_ordering = random_permutation(cities.size());
  auto bmutex = std::mutex();


  auto run_this_thread = [&]() 
  {
    TournamentDeme deme(&cities, pop_size, mutation_rate);

    // Evolve the population to make it fitter and keep track of
    // the shortest distance generated
    for (long i = 1; i <= (iters/pop_size)/nthread; ++i) 
    {

      deme.compute_next_generation();    // generate next generation

      // Find best individual in this population

      const auto ordering = deme.get_best()->get_ordering();
      
      if (cities.total_path_distance(ordering)<cities.total_path_distance(best_ordering)) 
      {    
        
        auto guard = std::lock_guard(bmutex); //Lock this current thread over here
        
        if (cities.total_path_distance(ordering)<cities.total_path_distance(best_ordering)) 
        {
          best_dist = cities.total_path_distance(ordering);
          best_ordering = ordering;
          std::cout << i*pop_size << "\t" << best_dist << std::endl;
        }
      }
    }
  };

  std::vector<std::thread> VectorOfThreads;

  for (unsigned i= 0; i<nthread; i++) 
  {
    VectorOfThreads.push_back(std::thread(run_this_thread));
  }

  for (auto& t: VectorOfThreads) 
  {
    t.join();
  }

  std::cout<<"Best distance found "<<best_dist;
  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  if (argc < 4) {
    std::cerr << "Required arguments: filename for cities, population size, and mutation rate\n";
    return -1;
  }

  const auto cities = Cities(argv[1]);
  const auto pop_size = atoi(argv[2]);
  const auto mut_rate = atof(argv[3]);
  const auto nthread = (argc > 4)? atoi(argv[4]) : 1;
  const auto granularity = (argc > 5)? atoi(argv[5]) : 100;

  constexpr unsigned NUM_ITER = 8'000'000;


//  const auto best_ordering = exhaustive_search(cities);
//  const auto best_ordering = randomized_search(cities, NUM_ITER);
 //const auto best_ordering = threaded_randomized_search(cities, NUM_ITER, nthread);
//  const auto best_ordering = granular_randomized_search(cities, NUM_ITER, nthread, granularity);
 const auto best_ordering = ga_search(cities, NUM_ITER, pop_size, mut_rate, nthread);

  auto out = std::ofstream("shortest.tsv");
  if (!out.is_open()) {
    std::cerr << "Can't open output file to record shortest path!\n";
    return -2;
  }

  out << cities.reorder(best_ordering);

  return pop_size - mut_rate + nthread - granularity; // eliminate silly warning
}

