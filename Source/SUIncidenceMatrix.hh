#ifndef SUINCIDENCEMATRIX_HH
#define SUINCIDENCEMATRIX_HH

#include "SUTypeDefs.hh"
#include "DLX.hh"

class SUIncidenceMatrix : public IncidenceMatrix
{
public:
  virtual ~SUIncidenceMatrix() {}

  SUIncidenceMatrix() {}

  bool operator()(unsigned i,unsigned j) const;

  unsigned ColumnRank() const { return 4*pow(SUEntry::dimension,2); }
  unsigned RowRank() const { return pow(SUEntry::dimension,3); }

private:
  unsigned Box(unsigned i,unsigned j) const;
};


#endif /* SUINCIDENCEMATRIX_HH */
