#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <map>
#include <queue>
#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;
#include "huffman.h"

//--- Definition of buildDecodingTree()
void Huffman::buildDecodingTree(ifstream & codeIn)
{
  char ch, temp; // a character
  string code = ""; // its code
  for (;;)
  {
  codeIn.get(ch);
  codeIn.get(temp);
  if ( codeIn.eof() ) return;
  while(temp != '\n') {
    codeIn.get(temp);
    if(temp != '\n') code += temp;
  }
  insert(ch, code);
  code = "";
  }
}

//--- Definition of insert()
void Huffman::insert(char ch, string code)
{
  Huffman::HuffNodePointer p = myRoot; // pointer to move down the tree
  for(int i = 0; i < code.length(); i++)
  {
    switch (code[i])
    {
      case '0' : // descend left
        if (p->left == 0) // create node along path
          p->left = new Huffman::HuffNode('*');
        p = p->left;
        break;
      case '1' : // descend right
        if (p->right == 0) // create node along path
          p->right = new Huffman::HuffNode('*');
        p = p->right;
        break;
      default:
        cerr << "*** Illegal character in code ***\n";
        exit(1);
    }
  }
  p->data = ch;
}

//--- Definition of decode()
void Huffman::decode(ifstream & messageIn)
{
  char bit, prev_bit; // next message bit
  Huffman::HuffNodePointer p; // pointer to trace path in decoding tree
  std::ofstream output_file("decoded/decoded_output");
  bit = bit & 0x00;
  p = myRoot;
  for(;;) {
    messageIn.get(bit);
    if ( messageIn.eof() ) return;
    for(int i = 0; i < 8; i++) {
      if ( p->left == 0 && p->right == 0) {
        output_file << p->data;
        p = myRoot;
      }
      if((bit & 0x80) == 0x80) {
        p = p->right;
      } else {
        p = p->left;
      }
      bit <<= 1;
    }
  }
}
//--- Definition of printTree()
void Huffman::printTree(ostream & out, Huffman::HuffNodePointer root, int indent)
{
  if (root != 0) {
    printTree(out, root->right, indent + 8);
    out << setw(indent) << " " << root->data << endl;
    printTree(out, root->left, indent + 8);
  }
}

// Methods for encoding

// Building Frequencies from text
vector< pair<char, int> > Huffman::buildFrequencies(ifstream & input) {
  map<char, int> freq_map;
  char ch;
  vector < pair<char, int> > freq_vec;
  cout << "building Frequencies\n";
  for(;;) {
    input >> ch;
    if ( input.eof() ) break;
    if (freq_map.find(ch) == freq_map.end()) {
      freq_map.insert(make_pair(ch, 1));
    } else {
      freq_map[ch] += 1;
    }
  }
  cout << "Done!! Making pairs...\n";
  for(map<char, int>::iterator it = freq_map.begin(); it != freq_map.end(); ++it) {
    freq_vec.push_back(make_pair(it->first, it->second));
  }
  return freq_vec;
}

// Build the lookup table
map<char, string> Huffman::buildLookupTable(Huffman::HuffNodePointer htree) {
  map<char, string> code_table;
  deque< pair<Huffman::HuffNodePointer, string> > huffqueue;
  cout << "Building Lookup table...\n";
  huffqueue.push_back(make_pair(htree, ""));
  while (!huffqueue.empty()) {
    Huffman::HuffNodePointer node, left, right;
    string code;
    node = huffqueue.front().first;
    code = huffqueue.front().second;
    huffqueue.pop_front();
    left = node->left;
    right = node->right;
    if (left) {
      string temp_code = code;
      huffqueue.push_back(make_pair(left, code.append("0")));
      huffqueue.push_back(make_pair(right, temp_code.append("1")));
    } else {
      code_table.insert(make_pair(node->data, code));
    }
  }
  cout << "built the table!!! \n";
  return code_table;
}

void Huffman::writeLookupTable(map<char, string> lookup) {
  std::ofstream code_file("keys/code.key");
  for(map<char, string>::iterator it = lookup.begin(); it != lookup.end(); ++it) {
    code_file << it->first << " " << it->second << "\n";
  }
}

//Building an encoding tree with the character & frequency pair
Huffman::HuffNodePointer Huffman::buildEncodingTree(vector< pair<char, int> > freq_vec) {

    priority_queue<Huffman::HuffNode , vector<Huffman::HuffNodePointer >, Huffman::Compare > enc_tree;
    for (vector< pair<char, int> >::iterator it = freq_vec.begin(); it != freq_vec.end(); ++it) {
        Huffman::HuffNodePointer leaf = new Huffman::HuffNode(it->first, it->second);
        enc_tree.push(leaf);
    }

    Huffman::HuffNodePointer root = NULL;
    while (enc_tree.size() > 1) {
        Huffman::HuffNodePointer l, r;
        l = enc_tree.top();
        enc_tree.pop();
        r = enc_tree.top();
        enc_tree.pop();
        root = new Huffman::HuffNode(0, l->freq + r->freq, l, r);
        enc_tree.push(root);
    }

    return root;
}

//encodes the input using code lookup table
string Huffman::encode(ifstream & input_file, map<char, string> lookup) {

  string encoded_res;
  char ch;

  for (;;) {
    input_file >> ch;
    if ( input_file.eof() ) break;
    string code = lookup[ch];
    encoded_res.append(code);
  }

  return encoded_res;
}

void Huffman::write(string encoded) {
  std::ofstream output_file("encoded/encoded_input.bin", ios::out | ios::binary);
  unsigned char temp;
  int count = 0;
  while(count < encoded.length()) {
    temp = temp & 0x00;
    for(int i=0; i<8 ; i++) {
      if(count >= encoded.length()) {
        break;
      }
      if(encoded[count] == '1') {
        temp ^=0x01;
      }
      if(i != 7) {
        temp <<= 1;
      }
      count++;
    }
    output_file << temp;
  }
}
