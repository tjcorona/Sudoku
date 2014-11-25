#include "SUProfiler.hh"
#include <iostream>

unsigned SUSolvedEntriesProfiler::fSolvedEntryRanges[5][2] = {{50,81},
							      {36,49},
							      {32,35},
							      {28,31},
							      {22,27}};

unsigned SULowerBoundsProfiler::fLowerBounds[5] = {5,4,3,2,0};

unsigned SUSolvedEntriesProfiler::EvaluatePuzzle(const SUSolution& puzzle)
{
  unsigned rating;

  for (rating = 0;rating<5;rating++)
    if (puzzle.size()>fSolvedEntryRanges[rating][0])
      break;

  return rating+1;
}

unsigned SULowerBoundsProfiler::EvaluatePuzzle(const SUSolution& puzzle)
{
  std::vector<unsigned> lowerBoundByColumn(SUEntry::dimension,0);
  std::vector<unsigned> lowerBoundByRow(SUEntry::dimension,0);

  for (unsigned i=0;i<puzzle.size();i++)
  {
    unsigned row = puzzle[i].position/SUEntry::dimension;
    unsigned column = puzzle[i].position%SUEntry::dimension;
    
    lowerBoundByColumn[column]++;
    lowerBoundByRow[row]++;
  }

  unsigned min = SUEntry::dimension;

  for (unsigned i=0;i<SUEntry::dimension;i++)
  {    
    if (lowerBoundByColumn[i]<min)
      min = lowerBoundByColumn[i];
    if (lowerBoundByRow[i]<min)
      min = lowerBoundByRow[i];
  }

  unsigned rating;

  for (rating = 0;rating<5;rating++)
    if (min>=fLowerBounds[rating])
      break;

  return rating+1;
}
