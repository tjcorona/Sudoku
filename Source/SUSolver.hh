#ifndef SUSOLVER_HH
#define SUSOLVER_HH

#include "DLX.hh"
#include "SUTypeDefs.hh"

class SUSolver : public DLX<SUEntry>
{
public:
  SUSolver();

  virtual ~SUSolver() {}

  void FindSolutions(SUSolutionSet&,SUSolution&,unsigned);

protected:
  void Search(DLX<SUEntry>::NodeSet&,DLX<SUEntry>::NodeSet&,SUSolutionSet&,size_t k=0);
};

#endif /* SUSOLVER_HH */
