#ifndef DLX_HH
#define DLX_HH

#include <cmath>
#include <vector>

class IncidenceMatrix
{
public:
  virtual ~IncidenceMatrix() {}

  virtual bool operator()(unsigned,unsigned) const = 0;

  virtual unsigned RowRank() const = 0;
  virtual unsigned ColumnRank() const = 0;
};

template <typename EntryType>
class DLX
{
public:
  typedef std::vector<EntryType> Solution;
  typedef std::vector<Solution> SolutionSet;

  DLX() : fRoot(NULL) {}
  virtual ~DLX();

  void ConstructFromIncidenceMatrix(const IncidenceMatrix&);
  void FindSolutions(SolutionSet&,unsigned);

protected:

  class Header;

  class Node
  {
  public:
    Node(EntryType entry) : fLeft(NULL),
			    fRight(NULL),
			    fUp(NULL),
			    fDown(NULL),
			    fHeader(NULL),
			    fEntry(entry) {}
    virtual ~Node() {}
    
    void L(Node* n) { fLeft = n; }
    void R(Node* n) { fRight = n; }
    void U(Node* n) { fUp = n; }
    void D(Node* n) { fDown = n; }
    void C(Header* h) { fHeader = h; }

    Node* L() { return fLeft; }
    Node* R() { return fRight; }
    Node* U() { return fUp; }
    Node* D() { return fDown; }
    Header* C() { return fHeader; }
    EntryType& Entry() { return fEntry; }

  private:
    Node* fLeft;
    Node* fRight;
    Node* fUp;
    Node* fDown;
    Header* fHeader;
    EntryType fEntry;
  };

  class Header : public Node
  {
  public:
    Header(EntryType entry) : Node(entry),
			      fSize(0) { this->C(this); }
    virtual ~Header() {}
    
    size_t S() { return fSize; }

    void Inc() { fSize++; }
    void Dec() { fSize--; }

  private:
    size_t fSize;
  };

  typedef std::vector<Node*> NodeSet;
  void Search(NodeSet&,SolutionSet&,size_t k=0);

  // selection criteria
  virtual Header* ChooseColumn();
  
  // Extract Entries in the NodeSet, and add them to the SolutionSet
  void StoreSolution(NodeSet& solution, SolutionSet& ss);

  // make a selection
  void CoverColumn(Header* c);

  // undo a selection
  void UncoverColumn(Header* c);

  Header* fRoot;
};

template <typename EntryType>
DLX<EntryType>::~DLX()
{
  Header* c = (Header*)(fRoot->R());
  while (c != fRoot)
  {
    Node* n = c->D();
    while (n != c)
    {
      Node* next_n = n->D();
      delete n;
      n = next_n;
    }
    Header* next_c = (Header*)(c->R());
    delete c;
    c = next_c;
  }
  delete fRoot;
}

template <typename EntryType>
void DLX<EntryType>::ConstructFromIncidenceMatrix(const IncidenceMatrix& m)
{
  // Construct the toroidal map

  // construct the root header
  fRoot = new Header(EntryType());

  // construct the column headers
  Node* last = fRoot;
  for (unsigned i=0;i<m.ColumnRank();i++)
  {
    Header* c = new Header(EntryType());
    last->R(c);
    c->L(last);
    c->R(fRoot);
    fRoot->L(c);
    fRoot->Inc();

    last = c;
  }

  // fill the columns, one row at a time
  Header* c = fRoot;
  Header** headers = new Header*[m.ColumnRank()];
  Node** previousInColumn = new Node*[m.ColumnRank()];
  for (unsigned i=0;i<m.ColumnRank();i++)
  {
    previousInColumn[i] = headers[i] = c = (Header*)(c->R());
  }

  for (unsigned i=0;i<m.RowRank();i++)
  {
    Node* firstInRow = NULL;
    Node* previousInRow = NULL;

    for (unsigned j=0;j<m.ColumnRank();j++)
    {
      if (m(i,j))
      {
	Node* n = new Node(EntryType(i));
	n->C(headers[j]);
	headers[j]->Inc();

	if (!firstInRow)
	  firstInRow = previousInRow = n;

	n->U(previousInColumn[j]);
	previousInColumn[j]->D(n);
	n->D(headers[j]);
	headers[j]->U(n);

	n->L(previousInRow);
	previousInRow->R(n);
	n->R(firstInRow);
	firstInRow->L(n);

	previousInColumn[j] = n;
	previousInRow = n;
      }
    }
  }

  delete [] headers;
  delete [] previousInColumn;
}

template <typename EntryType>
typename DLX<EntryType>::Header* DLX<EntryType>::ChooseColumn()
{
  // Choose the column object with the fewest pivots
  Header* c = fRoot;
  Header* j = (Header*)(c->R());
  while (j != fRoot)
  {
    if (j->S() < c->S())
      c = j;
    j = (Header*)(j->R());
  }
  return c;
}

template <typename EntryType>
void DLX<EntryType>::StoreSolution(NodeSet& solution, SolutionSet& solutionSet)
{
  // Extract Entries in the NodeSet, and add them to the SolutionSet
  Solution s;
  for (typename NodeSet::iterator it=solution.begin();it!=solution.end();++it)
    if (*it)
      s.push_back((*it)->Entry());
  solutionSet.push_back(s);
}

template <typename EntryType>
void DLX<EntryType>::CoverColumn(typename DLX<EntryType>::Header* c)
{
  // Cover column c
  c->R()->L(c->L());
  c->L()->R(c->R());

  Node* i = c->D();
  while (i != c)
  {
    Node* j = i->R();
    while (j != i)
    {
      j->D()->U(j->U());
      j->U()->D(j->D());
      j->C()->Dec();

      j = j->R();
    }

    i = i->D();
  }
  fRoot->Dec();
}

template <typename EntryType>
void DLX<EntryType>::UncoverColumn(typename DLX<EntryType>::Header* c)
{
  // Uncover column c
  Node* i = c->U();
  while (i != c)
  {
    Node* j = i->L();
    while (j != i)
    {
      j->C()->Inc();
      j->D()->U(j);
      j->U()->D(j);

      j = j->L();
    }

    i = i->U();
  }

  c->R()->L(c);
  c->L()->R(c);

  fRoot->Inc();
}

template <typename EntryType>
void DLX<EntryType>::Search(typename DLX<EntryType>::NodeSet& solution,
			    typename DLX<EntryType>::SolutionSet& solutionSet,
			    size_t k)
{
  // Dancing Links implementation
  
  if (k == solution.size())
    return StoreSolution(solution,solutionSet);
  
  // Choose a column object
  Header* c = ChooseColumn();
    
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
    Search(solution,solutionSet,k+1);
    
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

template <typename EntryType>
void DLX<EntryType>::FindSolutions(SolutionSet& solutionSet,
				   unsigned nEntries)
{
  // DLX backtracking search, returning solutions that have <nEntries> entries
  std::vector<Node*> solution(nEntries,NULL);
  Search(solution,solutionSet);
}

#endif /* SUDLX_HH */
