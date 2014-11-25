#include "SUSolver.hh"

#include "SUIncidenceMatrix.hh"

SUSolver::SUSolver()
{
  SUIncidenceMatrix m;
  ConstructFromIncidenceMatrix(m);
}

void SUSolver::FindSolutions(SolutionSet& solutionSet,
			     Solution& knownEntries,
			     unsigned nEntries)
{
  NodeSet solution(nEntries,NULL);
  NodeSet knownNodes;
  
  for (unsigned i=0;i<knownEntries.size();i++)
  {
    Node* r = fRoot->R();
    for (unsigned j=0;j<knownEntries.at(i).position;j++)
      r = r->R();

    for (unsigned j=0;j<knownEntries.at(i).value;j++)
      r = r->D();
    knownNodes.push_back(r);
  }

  Search(knownNodes,solution,solutionSet);
}

void SUSolver::Search(NodeSet& knownNodes, NodeSet& solution, SolutionSet& ss, size_t k)
{
  // Dancing Links implementation

  if (knownNodes.empty())
    return DLX<SUEntry>::Search(solution,ss,k);

  if (k == solution.size())
    return StoreSolution(solution,ss);
  
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
  Search(knownNodes,solution,ss,k+1);

  // if we have returned, then either a solution was found or there were no
  // column elements on which we could pivot.  Either way, we undo the
  // selection of r as a solution and continue.

  r = solution[k];
  c = r->C();

  j = r->L();
  while (j != r)
  {
    UncoverColumn(j->C());
    j = j->L();
  }

  UncoverColumn(c);
}
