#include <iostream>
#include "huffman.hh"
#include "bitio.hh"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
void DecodingCurrentFile(char*);

//As the question dictates, this allows us to take more than a single command-line argument(arg_c checks the number of arguments) 
//so that the encoder compresses all the filenames given as input, and the decoder decompresses all of the inputs 
int main(int argument_count, char** arguments)
{
	if (argument_count < 2) //We need both the input and output 
	{
		cerr<<"Not enough arguments , need input and output"<<endl;
	}

//This while loop allows us to decompress multiple files at a time. It uses 
	int j = 1;
	while (j <argument_count)
	{
		DecodingCurrentFile(arguments[j]);
		++j;// Move to next file 
	}
	return 0;
}

//Helper function used to decompress each individual file. This is called from the while loop in the main function
//so that we can process each individual file 
void DecodingCurrentFile(char* name)
{
	auto File_name = name;
	string extension = ".plaintext";
	ifstream InputFile(name); // Creating an input stream, input stream will read the text from the given file
	ofstream OutputFile(File_name + extension, ios::out); //Creating an output stream which will write the text onto the file ending with .plaintext

	if(!InputFile.is_open()) // Mandatory chech to see that a bad or non-existent file hasn't been asked to decompress
	{
		cout<<"No file exists";
		return;
	}

	BitIO bit_obj(nullptr, &InputFile);
	Huffman huff_obj;
	//Note that ostream is passed as nullptr because we are using input_bit. Our input_bit function relies on load bit
	//which uses the ostream variable buffer_ , it keeps reading individual bits and passes it onto the huffman obj, If the huffman object detetects a pattern
	//and encounters a leaf node, then it prints that leaf.
	//Keeps decoding until HEOF is reached, HEOF is a constant described in the header file of Huffman denoting the end of file
	bool bits = bit_obj.input_bit();
	int check_node = huff_obj.decode(bits);
	while (check_node != Huffman::HEOF)
	{
		if (check_node != -1) // If not an internal node, then print this leaf(character)
		{
			OutputFile<<char(check_node);
		}
		bits = bit_obj.input_bit(); //Get the next bit, load it onto the huffman tree
		check_node = huff_obj.decode(bits);// and repeat the same pattern until you hit a leaf in the huffman tree
	}

}