#ifndef SUTYPEDEFS_HH
#define SUTYPEDEFS_HH

#include "DLX.hh"

struct SUEntry
{
  SUEntry() : position(std::numeric_limits<unsigned>::max()),
	      value(0) {}
  SUEntry(unsigned i) : position(i/dimension), value(i%dimension+1) {}
  SUEntry(unsigned p,short v) : position(p), value(v) {}
  
  unsigned position;
  short value;

  static unsigned dimension;

  friend bool operator < (const SUEntry& lhs,const SUEntry& rhs)
  {
    return (lhs.position != rhs.position ?
	    lhs.position < rhs.position : lhs.value < rhs.value);
  }
};

typedef DLX<SUEntry>::Solution SUSolution;
typedef DLX<SUEntry>::SolutionSet SUSolutionSet;

#endif /* SUTYPEDEFS_HH */
