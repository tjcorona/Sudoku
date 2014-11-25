#include "SUIncidenceMatrix.hh"

unsigned SUIncidenceMatrix::Box(unsigned i,unsigned j) const
{
  unsigned sqrt_dim = sqrt(SUEntry::dimension);
  
  return (i/sqrt_dim)*sqrt_dim + (j/sqrt_dim);
}

bool SUIncidenceMatrix::operator()(unsigned i,unsigned j) const
{
  // Returns the value of the binary matrix form at (i,j) (index notation).

  // matrix row: per symbol, per grid location
  // matrix column: conditions:
  //   per row: symbol in row
  //   per column: symbol in column
  //   per box: symbol in box
  //
  // For Sudoku:
  // there are <dim> symbols
  // there are <dim*dim> grid locations
  // there are <dim> rows
  // there are <dim> columns
  // there are <dim> boxes

  // so: first <dim*dim> columns represent the position 
  // next <dim*dim> columns represent the row
  // next <dim*dim> columns represent the column
  // final <dim*dim> columns represent the box
  
  unsigned dim = SUEntry::dimension;
  unsigned dim2 = dim*dim;

  unsigned value = i%dim;
  unsigned position = (i/dim)%dim2;
  
  unsigned row = position/dim;
  unsigned column = position%dim;
  unsigned box = Box(row,column);

  if (j < dim2)
  {
    // we are querying the matrix columns corresponding to the position
    // condition:
    return (position == j);
  }
  
  j -= dim2;
  
  if (j < dim2)
  {
    // we are querying the matrix columns corresponding to the row condition:
    return (row*SUEntry::dimension + value == j);
  }
  
  j -= dim2;
  
  if (j < dim2)
  {
    // we are querying the matrix columns corresponding to the column condition:
    return (column*SUEntry::dimension + value == j);
  }
  
  j -= dim2;

  // we are querying the matrix columns corresponding to the box condition:
  return (box*SUEntry::dimension + value == j);
}
