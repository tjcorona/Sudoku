#include <getopt.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <sstream>

#include "Random.hh"
#include "SUTypeDefs.hh"
#include "SUDisplay.hh"
#include "SUSolver.hh"
#include "SUGenerator.hh"
#include "SUProfiler.hh"
#include "Histogram.hh"

int main(int argc, char** argv)
{
  std::string usage =
    "\n"
    "Usage: SudokuProfiler <options>\n"
    "\n"
    "This program profiles the Sudoku puzzle generator for 9x9 puzzles.\n"
    "\n"
    "\tAvailable options:\n"
    "\t -h, --help              (shows this message and exits)\n"
    "\t -n, --generated-number  (set the number of generated puzzles)\n"
    "\t -s, --simple-profile    (bool; display simple profile)\n"
    "\t -f, --full-profile      (bool; display full profile)\n"
    ;

  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"generated-number", required_argument, 0, 'n'},
    {"simple-profile", no_argument, 0, 's'},
    {"full-profile", no_argument, 0, 'f'},
  };

  static const char *optString = "hn:sf";

  unsigned generatedNumber = 1000;
  bool simpleProfile = false;
  bool fullProfile = false;
  
  while(1) {
    char optId = getopt_long(argc, argv,optString, longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
    case('h'): // help
      std::cout<<usage<<std::endl;
      return 0;
    case('n'):
      generatedNumber = atoi(optarg);
      break;
    case('s'):
      simpleProfile = true;
      break;
    case('f'):
      fullProfile = true;
      break;
    default: // unrecognized option
      std::cout<<usage<<std::endl;
      return 1;
    }
  }

  if (!simpleProfile && !fullProfile)
  {
    std::cout<<"No profiler selected!"<<std::endl;
    std::cout<<usage<<std::endl;
    return 1;
  }
  
  SUEntry::dimension = 9;

  HistogramData hist;

  hist.push_back(std::make_pair("Extremely Easy",0));
  hist.push_back(std::make_pair("Easy",0));
  hist.push_back(std::make_pair("Medium",0));
  hist.push_back(std::make_pair("Difficult",0));
  hist.push_back(std::make_pair("Evil",0));
  
  HistogramData hist2;
  for (unsigned i=0;i<81;i++)
  {
    std::stringstream s;
    s << i;
    hist2.push_back(std::make_pair(s.str(),0));
  }
  
  SUSolvedEntriesProfiler solvedEntriesProfiler;
  SULowerBoundsProfiler lowerBoundsProfiler;

  for (unsigned i=0;i<generatedNumber;i++)
  {
    SUSolution puzzle;
    SUGenerator generator;

    generator.GeneratePuzzle(puzzle,SUEntry::dimension*SUEntry::dimension);
  
    unsigned difficulty=((2./3.)*solvedEntriesProfiler.EvaluatePuzzle(puzzle)+
			 (1./3.)*lowerBoundsProfiler.EvaluatePuzzle(puzzle))-1;

    hist[difficulty].second++;

    hist2[puzzle.size()].second++;
  }
  
  if (simpleProfile)
  {
    std::cout<<std::endl;
    PrintHistogram("Difficulty Histogram",hist);
    std::cout<<std::endl;
  }
  
  if (fullProfile)
  {
    std::cout<<std::endl;
    PrintHistogram("Histogram of Filled Entries",hist2);
    std::cout<<std::endl;
  }
  
  return 0;
}
