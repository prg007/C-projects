#include "huffman.hh"
#include "hforest.hh"
#include "htree.hh"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

Huffman::Huffman()  //Initialize the frequency table of size 257 with value 0 
{
	freqtable_ = std::vector<int>(257, 0);
}

Huffman::~Huffman() // Destructor
{
}

Huffman::bits_t Huffman::encode(int symbol)
{
	hufftree_ = createNewhuffman_tree(); //Finding that particular symbol or character in the tree


    bits_t Sequence; // is of type vector boolean
    ++freqtable_[symbol]; //Increment the frequency count of that character/symbol by 1
    auto path = hufftree_ -> path_to(symbol); //We know path_to returns a list of Directions from HW7 

    for(auto dir : path)
    {
        if(dir == HTree::Direction::LEFT)
        {
            Sequence.push_back(false);
        }
        else
        {
            Sequence.push_back(true);
        }
    }

  
    return Sequence; // Returns a vector of booleans as stated in the question
}

int Huffman::decode(bool bit){


    if(hufftree_ == nullptr) // First Bit in Sequence, build the Huffman Tree from scratch
    {
        hufftree_ = Huffman::createNewhuffman_tree();
    }

    if(bit)
    { // If bit is set to 1
        hufftree_ = hufftree_ -> get_child(HTree::Direction::RIGHT);
    }
    else //If bit is set to 0
    {
        hufftree_ = hufftree_ -> get_child(HTree::Direction::LEFT);
    }

    auto newkey_ = hufftree_ -> get_key();

    //Checking if it is an intermediate node
    // return its symbol if not
    if(newkey_ > -1) 
    {
        ++freqtable_[newkey_];// update the frequency of the character
        hufftree_ = createNewhuffman_tree(); // resetting the Huffman Tree to nullptr
        return newkey_;
    }

    return -1;

}

HTree::tree_ptr_t Huffman::createNewhuffman_tree()
{
    
    HForest forest; 
    //Creating a new HTREE node for every possible character and EOF. 
    //character/symbol will be the key, and its count from the frequency table as its value 

    for(long unsigned int i = 0 ; i < freqtable_.size(); i++)
    {
        forest.add_tree(HTree::tree_ptr_t(new HTree(i, freqtable_[i])));
    }

    //Doing what the question states, iterating until the forest has a single tree in it 
    while(forest.size() > 1)
    {

        //Finding the top two HTrees in the forest with the lowest node
        auto left = forest.pop_top(); //Indicating the left node which we will merge
        auto right = forest.pop_top(); //Indicating the right node which we will merge

        //Creating a new internal node in the tree with its children being left and right, Representing an internal symbol with the value -1
        forest.add_tree(HTree::tree_ptr_t(new HTree(-1,(left->get_value() + right->get_value()), left, right)));
    }    

    assert(forest.size() == 1);
    return forest.pop_top(); // This is the Huffman Tree that we need
}
