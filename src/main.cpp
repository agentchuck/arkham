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
int runSeed = -1;
vector< string > phrases;
string inputfile;


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
      ("seed,s", po::value< int >(&runSeed), "Run a specific seed")
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

  World wld;
  wld.import(inputfile.c_str());

  // For now, just run the first seed unless another is specified.
  if (runSeed == -1) {
    runSeed = wld.seeds[0];
  }
  wld.seed = runSeed;

  cout << "Running seed: " << runSeed << endl;
  cout << "Source length: " << wld.sourcelength << endl;
  cout << "Total units: " << wld.units.size() << endl;

  for (int cnt = 0; cnt < wld.sourcelength; cnt++) {
    size_t nextUnit = wld.nextUnit();
    cout << "Next piece: " << nextUnit << endl;
    wld.units[nextUnit].print();

    wld.seed = iterateRNG(wld.seed);
  }

  return 0;
}
