/*Reference of concept: https://github.com/ningke/huffman-codes*/
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#ifndef HUFFMAN
#define HUFFMAN

class Huffman{
 private:
  class HuffNode {
    public:
      char data;
      int freq;
      HuffNode * left, * right;

      HuffNode(char data, int freq=0, HuffNode *left = NULL, HuffNode *right = NULL) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->freq = freq;
      }

      ~HuffNode() {
        delete left, delete right;
      }
  };

  // Compare two tree nodes
  class Compare {
    public:
    bool operator()(HuffNode *a, HuffNode *b) {
      return a->freq > b->freq;
    }
  };

  public:

  typedef HuffNode * HuffNodePointer;

  Huffman();

  void buildDecodingTree(ifstream & codeIn);

  vector< pair<char, int> > buildFrequencies(ifstream & input);

  HuffNodePointer buildEncodingTree(vector< pair<char, int> > freq_vec);

  map<char, string> buildLookupTable(HuffNodePointer htree);

  void writeLookupTable(map<char, string> lookup);

  string encode(ifstream & input_file, map<char, string> lookup);

  void write(string encoded);

  void insert(char ch, string code);

  void decode(ifstream & messageIn);

  void printTree(ostream & out, HuffNodePointer root, int indent);

  void displayDecodingTree(ostream & out);

  private:
  HuffNodePointer myRoot;
};

inline Huffman::Huffman()
{
  myRoot = new HuffNode('*');
}

inline void Huffman::displayDecodingTree(ostream & out)
{
  printTree(out, myRoot, 0);
}

#endif