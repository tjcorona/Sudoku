#include <getopt.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <sstream>

#include "SUTypeDefs.hh"
#include "SUDisplay.hh"
#include "SUSolver.hh"

int main(int argc, char** argv)
{
  std::string usage =
    "\n"
    "Usage: SudokuSolver <options>\n"
    "\n"
    "This program solves Sudoku puzzles.\n"
    "\n"
    "\tAvailable options:\n"
    "\t -h, --help              (shows this message and exits)\n"
    "\t -s, --puzzle-size       (set the size of the puzzle)\n"
    "\t -i, --input-format      (set the input format: file, keyboard)\n"
    ;

  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"puzzle-size", required_argument, 0, 's'},
    {"input-format", required_argument, 0, 'i'},
  };

  static const char *optString = "hs:i:";

  std::string inputFormat = "keyboard";

  while(1) {
    char optId = getopt_long(argc, argv,optString, longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
    case('h'): // help
      std::cout<<usage<<std::endl;
      return 0;
    case('s'):
      SUEntry::dimension = atoi(optarg);
      break;
    case('i'):
      inputFormat = optarg;
      break;
    default: // unrecognized option
      std::cout<<usage<<std::endl;
      return 1;
    }
  }

  if (pow(sqrt(SUEntry::dimension),2) != SUEntry::dimension)
  {
    std::cout<<"Size must be a square number!"<<std::endl;
    return 1;
  }

  if (inputFormat != "file" && inputFormat != "keyboard")
  {
    std::cout<<"Unrecognized input option: "<<inputFormat<<"."<<std::endl;
    return 1;
  }

  SUSolver solver;
  SUSolution knownEntries;
  SUSolutionSet solutionSet;

  if (inputFormat == "keyboard")
  {
    std::cout<<"Input position/value pairs:"<<std::endl;
    std::cout<<"> ";

    std::string s;
    int position, value;
    while (std::getline( std::cin, s ) && !s.empty())
    {
      std::stringstream ss(s);
      ss >> position >> value;
      if (!ss)
	std::cout << "Invalid pair. Try again."<<std::endl;
      else
	knownEntries.push_back(SUEntry(position,value));
      std::cout << "> ";
    }
  }
  else
  {
    std::cout<<"To do: implement board configuration from file."<<std::endl;
  }
  
  std::cout<<"Initial board configuration: "<<std::endl;
  PrintPuzzle(SUEntry::dimension,knownEntries);
  std::cout<<std::endl;

  solver.FindSolutions(solutionSet,knownEntries,SUEntry::dimension*SUEntry::dimension);
  
  std::cout<<"There are "<<solutionSet.size()<<" solutions."<<std::endl;

  for (unsigned i=0;i<solutionSet.size();i++)
  {
    PrintPuzzle(SUEntry::dimension,solutionSet.at(i));
  }
  
  std::cout<<std::endl;

  return 0;
}
