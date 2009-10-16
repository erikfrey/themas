#ifndef __PROBE_HPP__
#define __PROBE_HPP__

namespace themas {

/**
 * interpolation probe: pick the next element to probe by making a jump informed by the difference
 * between the low value, the high value, and the value you are searching
 */
template< class RandomAccessIterator, class T >
struct interpolation_probe
{
  RandomAccessIterator operator()(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
  {
    float a = *begin, b = *(end - 1), x = value;
    return begin + (end - begin - 1) * std::min(1.0f, std::max(0.0f, (x - a) / (b - a) ));
  }
};

/**
 * binary probe: pick the next element by choosing the halfway point between low and high
 */
template< class RandomAccessIterator, class T >
struct binary_probe
{
  RandomAccessIterator operator()(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
  {
    return begin + ( (end - begin) >> 1);
  }
};

/**
 * lower_bound: like stl's lower_bound but with different kinds of probing
 * note the appearance of the rare template parameter template!
 */
template< template <class, class> class Probe, class RandomAccessIterator, class T >
RandomAccessIterator lower_bound(RandomAccessIterator begin, RandomAccessIterator end, const T & value)
{
  RandomAccessIterator pit;
  Probe< RandomAccessIterator, T > pfunc; // probe-functor (wants to get func'd up)

  while ( begin < end )
  {
    pit = pfunc(begin, end, value);
    if ( *pit < value )
      begin = pit + 1;
    else
      end = pit;
  }
  return begin;
}

/*
 * this time with a comparator!
 */
template< template <class, class> class Probe, class RandomAccessIterator, class T, class Comparator >
RandomAccessIterator lower_bound(RandomAccessIterator begin, RandomAccessIterator end, const T & value, Comparator cmp)
{
  RandomAccessIterator pit;
  Probe< RandomAccessIterator, T > pfunc;

  while ( begin < end )
  {
    pit = pfunc(begin, end, value);
    if ( cmp(*pit, value) )
      begin = pit + 1;
    else
      end = pit;
  }
  return begin;
}

} // themas

#endif // __PROBE_HPP__
