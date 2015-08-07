#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;


int timelimit;
int memorylimit;
vector< string > phrases;

int main(int argc, char **argv)
{
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("filename,f", po::value< string >(), "File containing JSON encoded input")
      ("timelimit,t", po::value< int >(&timelimit)->default_value(60),
          "Time limit, in seconds, to produce output")
      ("memory,m", po::value< int >(&memorylimit)->default_value(1000),
          "Memory limit, in megabytes, to produce output")
      ("phrase,p", po::value< vector<string> >(&phrases), "Phrase of power, as quoted string")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
      cout << desc << "\n";
      return 1;
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
  return 0;
}
