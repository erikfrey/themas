#ifndef __UTIL_H__
#define __UTIL_H__

#include "intersect.hpp"

// copy_n never made it into the standard so i guess i'll make one
template<class InputIterator, class Size, class OutputIterator>
void copy_n(InputIterator begin, Size n, OutputIterator out)
{
  while (n--)
    *out++ = *begin++;
}

// class functors to ease inlined function binding
struct li_functor
{
  template< class A, class B, class C > void operator()( A begin1, A end1, B begin2, B end2, C out )
  { themas::linear_intersect(begin1, end1, begin2, end2, out); }
  template< class A, class B, class C, class D > void operator()( A begin1, A end1, B begin2, B end2, C out, D ftor )
  { themas::linear_intersect(begin1, end1, begin2, end2, out, ftor); }
};
struct bb_functor
{
  template< class A, class B, class C > void operator()( A begin1, A end1, B begin2, B end2, C out )
  { themas::baeza_intersect< themas::binary_probe >(begin1, end1, begin2, end2, out); }
  template< class A, class B, class C, class D > void operator()( A begin1, A end1, B begin2, B end2, C out, D ftor )
  { themas::baeza_intersect< themas::binary_probe >(begin1, end1, begin2, end2, out, ftor); }
};
struct bi_functor
{
  template< class A, class B, class C > void operator()( A begin1, A end1, B begin2, B end2, C out )
  { themas::baeza_intersect< themas::interpolation_probe >(begin1, end1, begin2, end2, out); }
  template< class A, class B, class C, class D > void operator()( A begin1, A end1, B begin2, B end2, C out, D ftor )
  { themas::baeza_intersect< themas::interpolation_probe >(begin1, end1, begin2, end2, out, ftor); }
};

// low-life good-for-nothing output iterator!
struct void_output_iterator
{
  struct void_output_iterator_deref
  { template<class T> void_output_iterator_deref & operator=(const T & value) { return *this; } };
  void_output_iterator_deref dr_;
  void_output_iterator_deref & operator* () { return dr_; }
  void_output_iterator & operator++() { return *this; }
  void_output_iterator operator++(int) { return *this; }
};

// compare number of counts
class counting_comparer
{
private:
  unsigned int & count_;
public:
  counting_comparer(unsigned int & count) : count_(count) {}
  template<class T> bool operator()(const T & i, const T & j)
  { ++count_; return i < j; }
};

#endif // __UTIL_H__
