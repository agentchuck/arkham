#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

#include "parser.hpp"
#include "world.hpp"

int timelimit;
int memorylimit;
int cores;
int rngTest;
vector< string > phrases;
string inputfile;


uint32_t numberFromSeed(uint32_t seed) {
  // The random number associated with a seed consists of bits 30..16 of that seed
  return (seed >> 16) & 0x7fff;
}

uint32_t iterateRNG(uint32_t seed) {
  uint64_t temp = uint64_t(seed) * uint64_t(1103515245);
  temp += 12345;
  return temp & 0xffffffff;
}

void testRNG(uint32_t seed, size_t count) {
  cerr << "First " << count << " elements of seed: " << seed << endl;
  for(size_t i = 0; i < count; i++) {
    cout << numberFromSeed(seed) << endl;
    seed = iterateRNG(seed);
  }
}

int main(int argc, char **argv)
{
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("filename,f", po::value< string >(&inputfile), "File containing JSON encoded input")
      ("timelimit,t", po::value< int >(&timelimit)->default_value(60),
          "Time limit, in seconds, to produce output")
      ("memory,m", po::value< int >(&memorylimit)->default_value(1000),
          "Memory limit, in megabytes, to produce output")
      ("rng,r", po::value< int >(&rngTest)->default_value(-1),
          "RNG test seed")
      ("cores,c", po::value< int >(&memorylimit)->default_value(1),
          "Available cores for computation")
      ("phrase,p", po::value< vector<string> >(&phrases), "Phrase of power, as quoted string")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
      cout << desc << "\n";
      return 1;
  }

  if (rngTest >= 0) {
    testRNG(rngTest, 100);
    return 0;
  }

  if (vm.count("filename") == 1) {
      cout << "Input file: " << vm["filename"].as<string>() << ".\n";
  } else {
      cout << "Input file required. Use -f <filename>\n";
      cout << desc << "\n";
      return -1;
  }

  cout << "Time limit: " << timelimit << " seconds.\n";
  cout << "Memory limit: " << memorylimit << " MB.\n";

  if (phrases.size()) {
    cout << "Phrases of power: " << phrases.size() << endl;
    for (size_t word = 0; word < phrases.size(); word++) {
      cout << phrases[word] << endl;
    }
  }

  /*
  readFile(inputfile.c_str());

  Board myBoard(10,10);
  myBoard.c[5][5] = true;
  myBoard.print();

  Board myOtherBoard(myBoard);
  myOtherBoard.c[6][6] = true;
  myOtherBoard.print();

  myBoard.print();
  */

  World wld;
  wld.import(inputfile.c_str());

  return 0;
}
