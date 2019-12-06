/*
 * HForest: a class to represent a collection of HTrees, each with an
 * associated value (inverse priority).
 * Implemented using the STL's make_heap.
 */

#include <algorithm>
#include "hforest.hh"

//////////////////////////////////////////////////////////////////////////////
// Comparator function for std::*_heap
static bool
compare_trees(HForest::tree_t t1, HForest::tree_t t2)
{
  return t1->get_value() > t2->get_value();
}

//////////////////////////////////////////////////////////////////////////////
// Add a single tree to the forest:
void
HForest::add_tree(tree_t tree)
{
  trees_.push_back(tree);
  std::push_heap(trees_.begin(), trees_.end(), compare_trees);
}

//////////////////////////////////////////////////////////////////////////////
// Return the tree with the highest frequency count (and remove it from forest)
HForest::tree_t
HForest::pop_top()
{
  if (trees_.empty()) {
    return nullptr;
  }
  std::pop_heap(trees_.begin(), trees_.end(), compare_trees);
  auto ret = trees_.back();
  trees_.pop_back();
  return ret;
}

