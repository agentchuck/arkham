#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

#include "parser.hpp"
#include "world.hpp"

int timelimit;
int memorylimit;
int cores;
int rngTest;
int runSeed = -1;
int interactive = -1;
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
      ("interactive,i", po::value< int >(&interactive), "Run interactive")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
      cout << desc << "\n";
      return 1;
  }

  if (rngTest >= 0) {

#if 0
  for (int cnt = 0; cnt < wld.sourcelength; cnt++) {
    size_t nextUnit = wld.nextUnit();
    cout << "Next piece: " << nextUnit << endl;
    wld.units[nextUnit].print();

    wld.seed = iterateRNG(wld.seed);
  }
    CC testCC;
    pii loc;
    cout << "Test CC\n";

    for (int x = 0; x < 10; x++) {
      for (int y = 0; y < 10; y++) {
        loc.first = x;
        loc.second = y;
        testCC.fromOddR(loc);
        cout << "Oddr: " << loc.first << ":" << loc.second << endl;
        cout << "Cube: " << testCC.x << ":" << testCC.y << ":" << testCC.z << endl << endl;
        pii loc2 = testCC.toOddR();
        if (loc != loc2) {
          cout << "Error! loc2: " << loc2.first << ":" << loc2.second << endl;
          return -1;
        }
      }
    }
#endif
#if 0
    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        loc.first = x;
        loc.second = y;
        pii piv = {3,3};

        cout << "In: " << loc.first << ":" << loc.second << endl;
        pii rotd = rotateAround(piv, loc, true);
        cout << " CW Out: " << rotd.first << ":" << rotd.second << endl;
        rotd = rotateAround(piv, loc, false);
        cout << " CcW Out: " << rotd.first << ":" << rotd.second << endl << endl;
      }
    }
#endif


    //testRNG(rngTest, 100);
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
  wld.initialSeed = runSeed;
  wld.seed = runSeed;

  cout << "Running seed: " << runSeed << endl;
  cout << "Source length: " << wld.sourcelength << endl;
  cout << "Total units: " << wld.units.size() << endl;

#if 0

  for (int cnt = 0; cnt < wld.units.size(); cnt++) {
    cout << 0 << " vs " << cnt << endl;
    if (wld.units[0] == wld.units[cnt]) {
      cout << " Match\n";
    } else {
      cout << " Not match\n";
    }
  }
  cout << endl;
  Unit rotunit(wld.units[9]);
  for (int cnt = 0; cnt < 8; cnt++) {
    if (wld.units[9] == rotunit) {
      cout << "Match\n\n";
    } else {
      cout << "Not match\n\n";
    }
    rotunit.rotate(true);
  }

  cout << "Board with no units\n";
  wld.board.print();
  for (int cnt = 0; cnt < wld.units.size(); cnt++) {
    cout << "Board with unit " << cnt << endl;;
    wld.board.au = &wld.units[cnt];
    wld.board.print();
  }
#endif


  if (interactive == 1) {
    int inch = -1;
    string runCommands;
    do {

      if (wld.board.au == nullptr) {
        if (!wld.actNextUnit()) {
          // Source is empty.
          break;
        }
        if (!wld.board.val(wld.activeUnit)) {
          // Board is full.
          break;
        }
      }
      fstream outputfile("board.txt", ios_base::out);
      wld.board.print(outputfile);
      outputfile.close();
      inch = getchar();
      if (inch > 0) {
        act_cmd cmd = charToCmd(inch);
        if (cmd != idle) {
          runCommands.push_back(inch);
        }
        wld.board.doAct(cmd);
      }
    } while (inch > 0);

    jsonHeader(std::cerr);
    jsonOutput(true, wld.id, wld.initialSeed, runCommands, std::cerr);
    jsonFooter(std::cerr);
    //cerr << "Ran command string: " << runCommands << endl;
  } else {
    wld.runAll();
  }

  return 0;
}
