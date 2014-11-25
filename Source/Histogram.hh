#ifndef SUHISTOGRAM_HH
#define SUHISTOGRAM_HH

#include <string>
#include <utility>
#include <vector>

typedef std::vector<std::pair<std::string,unsigned> > HistogramData;

void PrintHistogram(std::string,HistogramData&);

#endif /* SUHISTOGRAM_HH */
