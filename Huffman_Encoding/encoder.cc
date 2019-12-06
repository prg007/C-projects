#include <iostream>
#include "huffman.hh"
#include "bitio.hh"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void EncodingCurrentFile(char*); // function declaration to encode individual file

//As the question dictates, this allows us to take more than a single command-line argument(arg_c checks the number of arguments) 
//so that the encoder compresses all the filenames given as input, and the decoder decompresses all of the inputs 
int main(int argument_count, char** arguments)
{
	if (argument_count < 2) //We need both the input and output 
	{
		cerr<<"Not enough arguments , need input and output"<<endl;
	}

//This while loop allows us to compress multiple files at a time. It uses 
	int j = 1;
	while (j <argument_count)
	{
		EncodingCurrentFile(arguments[j]);
		++j;// Move to next file 
	}
	return 0;
}

//Helper function used to compress each individual file. This is called from the while loop in the main function
//so that we can process each individual file 

void EncodingCurrentFile(char* name)
{
	auto File_name = name;
	string extension = ".comp";
	ifstream InputFile(name); // Creating an input stream, input stream will read the text from the given file
	ofstream OutputFile(File_name + extension, ios::out); //Creating an output stream which will write the text onto the file ending with .comp

	if(!InputFile.is_open()) // Mandatory chech to see that a bad or non-existent file hasn't been asked to decompress
	{
		cout<<"No file exists";
		return;
	}

	BitIO bit_obj(&OutputFile,nullptr);
	Huffman huff_obj;
	char x;
	vector<bool> array; //Creating a boolean vector that will hold the path in the Huffman of that particular character.

	//Note that istream is passed as nullptr because we are using output_bit. Our output_bit function relies on write bit
	//helper function which uses the ostream variable os, the while loop reads individual character, encodes a particular
	//character into a vector of booleans and then writes those booleans into the compressed file "OutputFile"
	while(InputFile.get(x))
	{
		array = huff_obj.encode(x);
		long unsigned int i;
		for (i = 0;i < array.size();i++) //Utility of boolean vector : Using it to produce the path to the output file 
		{
			bit_obj.output_bit(array[i]);//Produces an individual bit to the output file 
		}
	}

	for (auto x : huff_obj.encode(Huffman::HEOF)) //Make sure that you encode the character HEOF as well or your decoder would never terminate
	{											//as HEOF indicates end of file
		bit_obj.output_bit(x);
	}

}



