#include <iostream>
#include <vector>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "intersect.hpp"
#include "util.h"

using namespace boost;

struct experiment
{
  std::vector<unsigned int> m;
  std::vector<unsigned int> n;
};

// the meat of the profiler
template< class IntersectFunctor >
void run_profile(IntersectFunctor ifunc, bool time, unsigned int m_min, unsigned int m_max, unsigned int m_stp,
                            unsigned int n_min, unsigned int n_max, unsigned int n_stp, unsigned int count)
{
  // set up our experiments
  // we pre-allocate everything ahead of time so that we have a larger working set
  // than what fits in cache
  std::vector< experiment > experiments;
  for (unsigned int i = m_min; i <= m_max; i += m_stp)
  {
    for (unsigned int j = n_min; j <= n_max; j += n_stp )
    {
      if ( i > j )
        continue;
      for ( unsigned int k = 0; k != count; ++k )
      {
        experiment e;
        e.m.resize(i);
        e.n.resize(j);
        copy_n( std::istream_iterator<unsigned int>(std::cin), i, e.m.begin() );
        copy_n( std::istream_iterator<unsigned int>(std::cin), j, e.n.begin() );
        std::sort( e.m.begin(), e.m.end() );
        std::sort( e.n.begin(), e.n.end() );
        experiments.push_back(e);
      }
    }
  }

  // okay, now run through our experiments <count> at a time
  boost::posix_time::ptime start, stop;
  void_output_iterator it_void;

  for (std::vector< experiment >::iterator it_e = experiments.begin(); it_e != experiments.end(); )
  {
    std::vector< unsigned int > profiles;
    for (unsigned int i = 0; i != count; ++i)
    {
      experiment & e = *it_e++;
      if (time)
      {
        start = boost::posix_time::microsec_clock::local_time(); // start timing!
        ifunc( e.m.begin(), e.m.end(), e.n.begin(), e.n.end(), it_void );
        stop = boost::posix_time::microsec_clock::local_time(); // stop timing!
        profiles.push_back( (stop - start).total_microseconds() );
      }
      else
      {
        unsigned int cmps = 0;
        counting_comparer cc(cmps);
        ifunc( e.m.begin(), e.m.end(), e.n.begin(), e.n.end(), it_void, cc );
        profiles.push_back( cmps );
      }
    }
    std::sort(profiles.begin(), profiles.end());
//    std::nth_element(profiles.begin(), profiles.end(), profiles.begin() + profiles.size() / 2 );
    std::cout << (it_e - 1)->m.size() << '\t' << (it_e - 1)->n.size() << '\t';
    for (unsigned int i = 0; i != count; ++i)
      std::cout << profiles[i] << ' ';
    std::cout << std::endl;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 10)
  {
    std::cerr << "usage: " << argv[0] << " <linear|binary|interp> <time|compares> "
              << "<m-min> <m-max> <m-step> <n-min> <n-max> <n-step> <count>" << std::endl
              << " * perform linear, baeza-binary, or baeza-interpolation set intersections" << std::endl
              << " * output statistics as time elapsed or # comparisons" << std::endl
              << " * run every combination of stepwise increments of set sizes n and m, <count> times" << std::endl
              << " * read random integers from stdin, writes statistics to stdout" << std::endl;
    return 1;
  }

  std::ios::sync_with_stdio(false);

  std::string intersect_algo = argv[1];
  bool time = (std::string( argv[2] ) == "time" );
  unsigned int m_min = lexical_cast< unsigned int >( argv[3] ),
               m_max = lexical_cast< unsigned int >( argv[4] ),
               m_stp = lexical_cast< unsigned int >( argv[5] ),
               n_min = lexical_cast< unsigned int >( argv[6] ),
               n_max = lexical_cast< unsigned int >( argv[7] ),
               n_stp = lexical_cast< unsigned int >( argv[8] ),
               count = lexical_cast< unsigned int >( argv[9] );

  if ( intersect_algo == "linear" )
    run_profile( li_functor() , time, m_min, m_max, m_stp, n_min, n_max, n_stp, count );
  else if (intersect_algo == "binary")
    run_profile( bb_functor() , time, m_min, m_max, m_stp, n_min, n_max, n_stp, count );
  else
    run_profile( bi_functor() , time, m_min, m_max, m_stp, n_min, n_max, n_stp, count );
}
