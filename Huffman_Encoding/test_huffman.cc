/*
 * test_huffman: test the Huffman class
 */

#include "huffman.hh"
#include <iostream>
#include <cassert>
#include <climits>

//////////////////////////////////////////////////////////////////////////////
// Ensure the encoder actually modifies the coding' length as expected
void test_encode()
{
  Huffman huff;

  auto bits = huff.encode('a');
  assert(bits.size() > CHAR_BIT);

  bits = huff.encode('b');
  assert(bits.size() > CHAR_BIT);

  bits = huff.encode('a');
  assert(bits.size() < CHAR_BIT);

  bits = huff.encode('b');
  assert(bits.size() < CHAR_BIT);

  bits = huff.encode('b');
  assert(bits.size() == huff.encode('a').size());

  bits = huff.encode('b');
  bits = huff.encode('b');
  bits = huff.encode('b');
  assert(bits.size() < huff.encode('a').size());
}

//////////////////////////////////////////////////////////////////////////////
// A utility function that takes a collection of bits and a Huffman tree, and
// figures out which symbol these bits encode.
int decode_symbol(Huffman& huff, const Huffman::bits_t& bits)
{
  int symbol = -1;

  for (auto b : bits) {
    symbol = huff.decode(b);
  }
  assert(symbol >= 0);
  return symbol;
}

//////////////////////////////////////////////////////////////////////////////
// Ensure decoding returns the same symbol that was encoded, in different
// scenarios.
void test_decode()
{
  Huffman encoder, decoder;

  auto bits = encoder.encode('a');
  assert(decode_symbol(decoder, bits) == 'a');
  bits = encoder.encode('a');
  assert(decode_symbol(decoder, bits) == 'a');
  bits = encoder.encode('b');
  assert(decode_symbol(decoder, bits) == 'b');
  bits = encoder.encode('a');
  assert(decode_symbol(decoder, bits) == 'a');
  bits = encoder.encode(Huffman::HEOF);
  assert(decode_symbol(decoder, bits) == Huffman::HEOF);
}

//////////////////////////////////////////////////////////////////////////////
int main()
{
  test_encode();
  test_decode();
  return 0;
}

