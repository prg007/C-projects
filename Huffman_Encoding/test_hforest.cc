/*
 * test_hforest: A simple unit test for the HForest data structure.
 */

#include "hforest.hh"
#include <iostream>
#include <cassert>

HForest create_test_forest()
{
  HForest forest;

  forest.add_tree(HTree::tree_ptr_t(new HTree(1, 10)));
  forest.add_tree(HTree::tree_ptr_t(new HTree(2, 20)));
  forest.add_tree(HTree::tree_ptr_t(new HTree(3, 30)));
  forest.add_tree(HTree::tree_ptr_t(new HTree(4, 40)));
  forest.add_tree(HTree::tree_ptr_t(new HTree(5, 5)));
  forest.add_tree(HTree::tree_ptr_t(new HTree(1, 6)));

  return forest;
}

void test_forest()
{
  auto forest = create_test_forest();
  assert(forest.size() == 6);
  assert(forest.pop_top()->get_value() == 5);
  assert(forest.pop_top()->get_value() == 6);
  assert(forest.pop_top()->get_value() == 10);
  assert(forest.pop_top()->get_value() == 20);
  assert(forest.pop_top()->get_value() == 30);
  assert(forest.pop_top()->get_value() == 40);
  assert(forest.size() == 0);
  assert(forest.pop_top() == nullptr);
}

int main()
{
  test_forest();
  return 0;
}

