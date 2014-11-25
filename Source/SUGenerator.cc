#include "SUGenerator.hh"

#include <iostream>

#include "SUIncidenceMatrix.hh"
#include "Random.hh"
#include "SUDisplay.hh"
#include "SUSolver.hh"

SUGenerator::SUGenerator()
{
  SUIncidenceMatrix m;
  ConstructFromIncidenceMatrix(m);
}

void SUGenerator::GeneratePuzzle(SUSolution& puzzle,unsigned nEntries)
{
  GenerateSolvedPuzzle(puzzle,nEntries);

  Random::GetInstance().Shuffle(puzzle);

  SUSolver solver;

  SUSolutionSet solutionSet;

  SUEntry lastEntry;

  unsigned nRemovalAttempts = Random::GetInstance().Uniform(SUEntry::dimension)+1;
  // unsigned nRemovalAttempts = SUEntry::dimension;

  while (nRemovalAttempts)
  {
    solutionSet.clear();
    lastEntry = puzzle.back();
    puzzle.pop_back();
    solver.FindSolutions(solutionSet,puzzle,nEntries);

  if (solutionSet.size()>1)
    {
      puzzle.push_back(puzzle.front());
      puzzle.front() = lastEntry;
      nRemovalAttempts--;
    }
  }
}

void SUGenerator::GenerateSeedEntries(SUSolution& seedEntries)
{
  std::vector<std::vector<short> > values(sqrt(SUEntry::dimension));

  for (unsigned i=0;i<SUEntry::dimension;i++)
    for (unsigned j=0;j<sqrt(SUEntry::dimension);j++)
      values[j].push_back(i+1);

  bool allowed = false;

  while(!allowed)
  {
    allowed = true;

    // shuffle the entries
    for (unsigned i=0;i<sqrt(SUEntry::dimension);i++)
      Random::GetInstance().Shuffle(values[i]);

    for (unsigned i=0;i<SUEntry::dimension;i++)
    {
      for (unsigned j=0;j<sqrt(SUEntry::dimension);j++)
      {
    	for (unsigned k=0;k<j;k++)
    	{
    	  if (values[j][i] == values[k][i])
    	    allowed = false;
    	  if (!allowed) break;
    	}
    	if (!allowed) break;
      }
      if (!allowed) break;
    }
  }

  for (unsigned i=0;i<sqrt(SUEntry::dimension);i++)
  {
    for (unsigned j=0;j<SUEntry::dimension;j++)
    {
      seedEntries.push_back(SUEntry(i*SUEntry::dimension*sqrt(SUEntry::dimension)+j,values[i][j]));
    }
  }
}

void SUGenerator::GenerateSolvedPuzzle(SUSolution& puzzle,unsigned nEntries)
{
  NodeSet solution(nEntries,NULL);
  SUSolutionSet ss;

  while (ss.empty())
  {
    SUSolution seedEntries;
    GenerateSeedEntries(seedEntries);
    
    NodeSet knownNodes;

    for (unsigned i=0;i<seedEntries.size();i++)
    {
      Node* r = fRoot->R();
      for (unsigned j=0;j<seedEntries.at(i).position;j++)
	r = r->R();

      for (unsigned j=0;j<seedEntries.at(i).value;j++)
	r = r->D();
      knownNodes.push_back(r);
    }

    Populate(knownNodes,solution,ss);
  }

  puzzle = ss[0];
}

void SUGenerator::Populate(NodeSet& knownNodes, NodeSet& solution, SolutionSet& solutionSet, size_t k)
{
  if (!solutionSet.empty())
    return;
    
  if (knownNodes.empty())
    return Populate(solution,solutionSet,k);

  if (k == solution.size())
  {
    // std::cout<<"K REACHED "<<k<<std::endl;
    return StoreSolution(solution,solutionSet);
  }
  
  // Choose a column object
  Header* c = knownNodes.back()->C();

  // Cover column c
  CoverColumn(c);

  Node* r = knownNodes.back();
  knownNodes.pop_back();
  
  // set r as a solution
  solution[k] = r;

  // loop over row elements of r
  Node* j = r->R();
  while (j != r)
  {
    // cover all columns that share a row element with r
    CoverColumn(j->C());
    j = j->R();
  }

  // now that the linked list is updated to contain only the remaining
  // available entries, we try another move
  Populate(knownNodes,solution,solutionSet,k+1);

  // if we have returned, then either a solution was found or there were no
  // column elements on which we could pivot.  Either way, we undo the
  // selection of r as a solution and continue.

  j = r->L();
  while (j != r)
  {
    UncoverColumn(j->C());
    j = j->L();
  }

  UncoverColumn(c);
}

void SUGenerator::Populate(NodeSet& solution, SolutionSet& solutionSet, size_t k)
{
  // Dancing Links implementation

  if (k == solution.size())
    return StoreSolution(solution,solutionSet);
  
  if (!solutionSet.empty())
    return;
    
  // Choose a column object
  Header* c = ChooseColumn();

  if (c->S() == 0)
    return;
  
  // Cover column c
  CoverColumn(c);

  // loop over the column elments of c
  Node* r = c->D();
  while (r != c)
  {
    // try r as a solution
    solution[k] = r;

    // loop over row elements of r
    Node* j = r->R();
    while (j != r)
    {
      // cover all columns that share a row element with r
      CoverColumn(j->C());
      j = j->R();
    }

    // now that the linked list is updated to contain only the remaining
    // available entries, we try another move
    Populate(solution,solutionSet,k+1);
    
    // if we have returned, then either a solution was found or there were no
    // column elements on which we could pivot.  Either way, we undo the
    // selection of r as a solution and continue.

    j = r->L();
    while (j != r)
    {
      UncoverColumn(j->C());
      j = j->L();
    }

    r = r->D();
  }

  UncoverColumn(c);
}
