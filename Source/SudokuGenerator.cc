#include <getopt.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>

#include "Random.hh"
#include "SUTypeDefs.hh"
#include "SUDisplay.hh"
#include "SUSolver.hh"
#include "SUGenerator.hh"
#include "SUProfiler.hh"

int main(int argc, char** argv)
{
  std::string usage =
    "\n"
    "Usage: SudokuGenerator <options>\n"
    "\n"
    "This program generates Sudoku puzzles.\n"
    "\n"
    "\tAvailable options:\n"
    "\t -h, --help              (shows this message and exits)\n"
    "\t -s, --puzzle-size       (set the size of the puzzle)\n"
    "\t -d, --difficulty        (set the difficulty from 1 to 5)\n"
    "\t -l, --show-solution     (bool; show the solution)\n"
    ;

  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"puzzle-size", required_argument, 0, 's'},
    {"difficulty", no_argument, 0, 'd'},
    {"show-solution", no_argument, 0, 'l'},
  };

  static const char *optString = "hs:d:l";

  unsigned difficulty = 3;
  bool showSolution = false;

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
    case('d'):
      difficulty = atoi(optarg);
      break;
    case('l'):
       showSolution = true;
      break;
    default: // unrecognized option
      std::cout<<usage<<std::endl;
      return 1;
    }
  }

  if (SUEntry::dimension < 2 || SUEntry::dimension > 36)
  {
    std::cout<<"Bad puzzle size input!"<<std::endl;
    return 1;
  }
  
  if (pow(sqrt(SUEntry::dimension),2) != SUEntry::dimension)
  {
    std::cout<<"Size must be a square number!"<<std::endl;
    return 1;
  }

  if (difficulty == 0 || difficulty > 5)
  {
    std::cout<<"Bad difficulty input!"<<std::endl;
    return 1;
  }

  SUSolution puzzle;
  SUGenerator generator;

  if (SUEntry::dimension == 9)
  {
    SUSolvedEntriesProfiler solvedEntriesProfiler;
    SULowerBoundsProfiler lowerBoundsProfiler;

    double rank = 0.;

    while (((int)rank) != difficulty)
    {
      generator.GeneratePuzzle(puzzle,SUEntry::dimension*SUEntry::dimension);

      rank = ((2./3.)*solvedEntriesProfiler.EvaluatePuzzle(puzzle) +
	      (1./3.)*lowerBoundsProfiler.EvaluatePuzzle(puzzle));
    }
  }
  else
    generator.GeneratePuzzle(puzzle,SUEntry::dimension*SUEntry::dimension);

  PrintPuzzle(SUEntry::dimension,puzzle);
  
  if (showSolution)
  {
    SUSolver solver;
    SUSolutionSet solutionSet;

    solver.FindSolutions(solutionSet,puzzle,SUEntry::dimension*SUEntry::dimension);

    PrintPuzzle(SUEntry::dimension,solutionSet[0]);
  }
  
  std::cout<<std::endl;

  return 0;
}
