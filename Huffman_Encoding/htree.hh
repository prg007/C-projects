/*
 * HTree: a class to represent a tree node with two elements: a key
 * and a value.
 */

#pragma once

#include <cassert>
#include <memory>
#include <list>

class HTree {
 public:
  using tree_ptr_t = std::shared_ptr<const HTree>;
  enum class Direction { LEFT, RIGHT };
  using path_t = std::list<Direction>;

  // Initialize with a key and a value:
  HTree(int key, uint64_t value, tree_ptr_t left = nullptr, tree_ptr_t right = nullptr)
    : key_(key), value_(value), left_(left), right_(right)
  { }

  ~HTree() = default;
  HTree(const HTree&) = default;
  HTree(HTree&&) = default;
  HTree& operator=(const HTree&) = delete;
  HTree& operator=(HTree&&) = delete;

  int get_key() const { return key_; }  // Return key in current node
  uint64_t get_value() const { return value_; }      // Return value in current node

  // Return the child of this node indicated by dir
  tree_ptr_t get_child(Direction dir) const
  {
    return (dir == Direction::LEFT)? left_ : right_;
  }

  // Crashes (with an assert) if key not contained in this tree
  path_t path_to(int key) const
  {
    assert(find(key));

    path_t ret;
    if (key_ != key) {
      if (left_ && left_->find(key)) {
        ret = left_->path_to(key);
        ret.push_front(Direction::LEFT);
      } else {              // key must be in right_
        ret = right_->path_to(key);
        ret.push_front(Direction::RIGHT);
      }
    }
    return ret;
  }

 private:
  int key_;
  uint64_t value_;
  tree_ptr_t left_, right_;

  // Return true IFF the given key is contained somewhere in my tree
  bool find(int key) const
  {
    return key_ == key
       || (left_ && left_->find(key))
       || (right_ && right_->find(key));
  }

};
