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
void run_profile(IntersectFunctor ifunc, bool time, unsigned int set_size, float r_min, float r_max,
                 float r_stp, unsigned int count)
{
  // set up our experiments
  // we pre-allocate everything ahead of time so that we have a larger working set
  // than what fits in cache
  std::vector< experiment > experiments;
  for (float r = r_min; r <= r_max + 0.0001; r += r_stp)
  {
    unsigned int m_size = static_cast<unsigned int>( set_size * r / ( r + 1 ) );
    unsigned int n_size = static_cast<unsigned int>( set_size / ( r + 1 ) );
    for (unsigned int i = 0; i != count; ++i)
    {
      experiment e;
      e.m.resize(m_size);
      e.n.resize(n_size);
      copy_n( std::istream_iterator<unsigned int>(std::cin), m_size, e.m.begin() );
      copy_n( std::istream_iterator<unsigned int>(std::cin), n_size, e.n.begin() );
      std::sort( e.m.begin(), e.m.end() );
      std::sort( e.n.begin(), e.n.end() );
      experiments.push_back(e);
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
    std::nth_element(profiles.begin(), profiles.end(), profiles.begin() + profiles.size() / 2 );
    std::cout << (it_e - 1)->m.size() << '\t' << (it_e - 1)->n.size() << '\t' << profiles[ profiles.size() / 2 ]
              << std::endl;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 8)
  {
    std::cerr << "usage: " << argv[0] << " <linear|binary|interp> <time|compares> "
              << "<set-size> <r-min> <r-max> <r-step> <count>" << std::endl
              << " * perform linear, baeza-binary, or baeza-interpolation set intersections" << std::endl
              << " * output statistics as time elapsed or # comparisons" << std::endl
              << " * run every combination of stepwise increments of set sizes n and m, <count> times" << std::endl
              << " * read random integers from stdin, writes statistics to stdout" << std::endl;
    return 1;
  }

  std::ios::sync_with_stdio(false);

  std::string intersect_algo = argv[1];
  bool time = (std::string( argv[2] ) == "time" );
  unsigned int set_size = lexical_cast< unsigned int >( argv[3] ),
               count =    lexical_cast< unsigned int >( argv[7] );
  float r_min = lexical_cast< float >( argv[4] ),
        r_max = lexical_cast< float >( argv[5] ),
        r_stp = lexical_cast< float >( argv[6] );

  if ( intersect_algo == "linear" )
    run_profile( li_functor() , time, set_size, r_min, r_max, r_stp, count );
  else if (intersect_algo == "binary")
    run_profile( bb_functor() , time, set_size, r_min, r_max, r_stp, count );
  else
    run_profile( bi_functor() , time, set_size, r_min, r_max, r_stp, count );
}
