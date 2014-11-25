#include "Histogram.hh"

#include <iostream>
#include <iomanip>

void PrintHistogram(std::string title,HistogramData& entries)
{
  short space = (80 - title.size())/2;
  std::cout<<std::endl;
  for (short i=0;i<space;i++)
    std::cout<<" ";
  std::cout<<title<<std::endl;

    short labelSize = 0;

  unsigned max = 0;
  unsigned total = 0;
  
  for (unsigned i=0;i<entries.size();i++)
  {
    short lbl = entries[i].first.size() + 5;
    unsigned len = entries[i].second;

    if (lbl > labelSize)
      labelSize = lbl;

    if (len > max)
      max = len;

    total += len;
  }

  short displaySize = 80 - 1 - labelSize;
  std::vector<short> displayEntries;
  
  for (unsigned i=0;i<entries.size();i++)
  {
    short space = labelSize - (entries[i].first.size() + 5);
    std::cout<<entries[i].first<<": ";
    for (short j=0;j<space;j++)
      std::cout<<" ";
    std::cout<<std::setw(2)<<((int)(100*((double)(entries[i].second))/total))<<"% ";
    short dsp = displaySize*((double)(entries[i].second))/max;
    for (short j=0;j<dsp;j++)
      std::cout<<"]";
    std::cout<<std::endl;
  }
}
