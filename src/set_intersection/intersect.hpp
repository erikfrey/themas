#ifndef __INTERSECT_HPP__
#define __INTERSECT_HPP__

#include "probe.hpp"

namespace themas {

/*
 * like stl's set_intersect
 */
template<class InputIterator, class OutputIterator>
void linear_intersect(InputIterator begin1, InputIterator end1,
                      InputIterator begin2, InputIterator end2,
                      OutputIterator out)
{
  if ( (end2 - begin2) > (end1 - begin1) )
  {
    // why in the world would i do this?
    // hmmmmmmm.......... !
    std::swap(begin1, begin2);
    std::swap(end1, end2);
  }
  while (begin1 != end1 && begin2 != end2)
  {
    if (*begin1 < *begin2)
      ++begin1;
    else if (*begin2 < *begin1)
      ++begin2;
    else
    {
      *out++ = *begin1;
      ++begin1;
      ++begin2;
    }
  }
}

/*
 * this time with a comparator!
 */
template<class InputIterator, class OutputIterator, class Comparator >
void linear_intersect(InputIterator begin1, InputIterator end1,
                      InputIterator begin2, InputIterator end2,
                      OutputIterator out, Comparator cmp)
{
  if ( (end2 - begin2) > (end1 - begin1) )
  {
    // why in the world would i do this?
    // hmmmmmmm.......... !
    std::swap(begin1, begin2);
    std::swap(end1, end2);
  }
  while (begin1 != end1 && begin2 != end2)
  {
    if (cmp( *begin1, *begin2 ) )
      ++begin1;
    else if ( cmp(*begin2, *begin1) )
      ++begin2;
    else
    {
      *out++ = *begin1;
      ++begin1;
      ++begin2;
    }
  }
}

/*
 * baeza_intersect
 */
template< template <class, class> class Probe,
  class RandomAccessIterator, class OutputIterator>
void baeza_intersect(RandomAccessIterator begin1, RandomAccessIterator end1,
                     RandomAccessIterator begin2, RandomAccessIterator end2,
                     OutputIterator out)
{
  RandomAccessIterator probe1, probe2;

  if ( (end1 - begin1) < ( end2 - begin2 ) )
  {
    if ( begin1 == end1 )
      return;
    probe1 = begin1 + ( ( end1 - begin1 ) >> 1 );
    probe2 = lower_bound< Probe >( begin2, end2, *probe1 );
    baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out); // intersect left
    if (! (probe2 == end2 || *probe1 < *probe2 ))
      *out++ = *probe2++;
    baeza_intersect< Probe >(++probe1, end1, probe2, end2, out); // intersect right
  }
  else
  {
    if ( begin2 == end2 )
      return;
    probe2 = begin2 + ( ( end2 - begin2 ) >> 1 );
    probe1 = lower_bound< Probe >( begin1, end1, *probe2 );
    baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out); // intersect left
    if (! (probe1 == end1 || *probe2 < *probe1 ))
      *out++ = *probe1++;
    baeza_intersect< Probe >(probe1, end1, ++probe2, end2, out); // intersect right
  }
}

/*
 * with a comparator
 */
template< template <class, class> class Probe,
  class RandomAccessIterator, class OutputIterator, class Comparator >
void baeza_intersect(RandomAccessIterator begin1, RandomAccessIterator end1,
                     RandomAccessIterator begin2, RandomAccessIterator end2,
                     OutputIterator out, Comparator cmp)
{
  RandomAccessIterator probe1, probe2;

  if ( (end1 - begin1) < ( end2 - begin2 ) )
  {
    if ( begin1 == end1 )
      return;
    probe1 = begin1 + ( ( end1 - begin1 ) >> 1 );
    probe2 = lower_bound< Probe >( begin2, end2, *probe1, cmp );
    baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out, cmp); // intersect left
    if (! (probe2 == end2 || cmp( *probe1, *probe2 ) ))
      *out++ = *probe2++;
    baeza_intersect< Probe >(++probe1, end1, probe2, end2, out, cmp); // intersect right
  }
  else
  {
    if ( begin2 == end2 )
      return;
    probe2 = begin2 + ( ( end2 - begin2 ) >> 1 );
    probe1 = lower_bound< Probe >( begin1, end1, *probe2, cmp );
    baeza_intersect< Probe >(begin1, probe1, begin2, probe2, out, cmp); // intersect left
    if (! (probe1 == end1 || cmp( *probe2, *probe1 ) ))
      *out++ = *probe1++;
    baeza_intersect< Probe >(probe1, end1, ++probe2, end2, out, cmp); // intersect right
  }
}

} // themas

#endif // __INTERSECT_HPP__
