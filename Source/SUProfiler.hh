#ifndef SUPROFILER_HH
#define SUPROFILER_HH

#include "SUTypeDefs.hh"

class SUProfiler
{
public:
  virtual ~SUProfiler() {}

  virtual unsigned EvaluatePuzzle(const SUSolution&) = 0;
};

class SUSolvedEntriesProfiler : public SUProfiler
{
public:
  unsigned EvaluatePuzzle(const SUSolution&);

private:
  static unsigned fSolvedEntryRanges[5][2];
};

class SULowerBoundsProfiler : public SUProfiler
{
public:
  unsigned EvaluatePuzzle(const SUSolution&);

private:
  static unsigned fLowerBounds[5];
};

#endif /* SUPROFILER_HH */
