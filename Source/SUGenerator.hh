#ifndef SUGENERATOR_HH
#define SUGENERATOR_HH

#include "DLX.hh"
#include "SUTypeDefs.hh"

class SUGenerator : public DLX<SUEntry>
{
public:
  SUGenerator();

  virtual ~SUGenerator() {}

  void GeneratePuzzle(SUSolution&,unsigned);

protected:
  void GenerateSeedEntries(SUSolution&);
  void GenerateSolvedPuzzle(SUSolution&,unsigned);
  
  void Populate(NodeSet&,NodeSet&,SolutionSet&,size_t k=0);
  void Populate(NodeSet&,SolutionSet&,size_t k=0);
};

#endif /* SUDLX_HH */
