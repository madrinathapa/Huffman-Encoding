#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;
#include "huffman.h"
#include "huffman.cpp"

int main()
{
  char filename[32];
  char option;
  clock_t start, finish;
  Huffman h;

  cout << "[E]ncode or [D]ecode?\n";
  cin >> option;

  if (option == 'E') {

    cout << "Enter the name of input file: ";
    cin >> filename;
    start = clock();
    ifstream inputstream(filename);
    ifstream input2(filename);
    inputstream.unsetf(ios_base::skipws);
    input2.unsetf(ios_base::skipws);
    vector< pair<char, int> > huffman_freq = h.buildFrequencies(inputstream);
    Huffman::HuffNodePointer htree = h.buildEncodingTree(huffman_freq);
    map<char, string> lookup = h.buildLookupTable(htree);
    h.writeLookupTable(lookup);
    string t = h.encode(input2, lookup);
    h.write(t);
    finish = clock();
    cout << "\nExecution time: +/- " << ((double)(finish - start))/CLOCKS_PER_SEC << "s\n";

  } else if (option == 'D') {

    cout << "Enter name of code file: ";
    cin >> filename;
    ifstream codestream(filename);
    codestream.unsetf(ios_base::skipws);
    cout << "\nName of message file: ";
    cin >> filename;
    ifstream message(filename, ios::binary);
    message.unsetf(ios_base::skipws);
    start = clock();
    if (!codestream.is_open()) {
      cout << "Cannot open code file.\n";
      exit(1);
    }
    h.buildDecodingTree(codestream);
    cout << "Here is the Huffman decoding tree:\n";
    h.displayDecodingTree(cout);
    cout << endl;
    if (!message.is_open()) {
      cout << "Cannot open message file.\n";
      exit(1);
    }
    h.decode(message);
    finish = clock();
    cout << "\nExecution time: +/- " << ((double)(finish - start))/CLOCKS_PER_SEC << "s\n";
    
  } else {
    cout << "Invalid Option!!";
  }
}