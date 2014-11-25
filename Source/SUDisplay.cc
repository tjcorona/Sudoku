#include "SUDisplay.hh"

#include <iostream>

void PrintPuzzle(unsigned int n,const SUSolution& s)
{
  SUSolution solution = s;
  std::sort(solution.rbegin(),solution.rend());

  for (unsigned int i=0;i<n;i++)
  {
    std::cout<<std::endl;
    for (unsigned int j=0;j<n;j++)
      std::cout<<" ---";
    std::cout<<std::endl<<"|";
    for (unsigned int j=0;j<n;j++)
    {
      std::cout<<" ";
      if (!solution.empty())
      {
	if (i == solution.back().position / n &&
	    j == solution.back().position % n)
	{
	  std::cout<<solution.back().value;
	  solution.pop_back();
	}
	else
	  std::cout<<" ";
      }
      else
	std::cout<<" ";
      std::cout<<" |";
    }
  }
  std::cout<<std::endl<<" ---";
  for (unsigned int j=0;j<n-1;j++)
    std::cout<<" ---";
  std::cout<<std::endl;
}

void PrintPuzzle(unsigned int n)
{
  SUSolution null;
  return PrintPuzzle(n,null);
}

