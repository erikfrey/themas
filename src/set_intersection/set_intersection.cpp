#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>

#include <ctime>
#include <boost/random/mersenne_twister.hpp>

#include "probe.hpp"
#include "intersect.hpp"

using namespace themas;

struct Cmper
{
  int & i_;
  Cmper(int & i) : i_(i) {}
  bool operator()(const int & lhs, const int & rhs)
  {
    ++i_;
    return lhs < rhs;
  }
};

int main(int argc, char * argv[])
{
  std::set<int> nums1, nums2;
  std::vector<int> result1, result2, result3;

  boost::mt19937 rng(time(NULL));

  for ( unsigned int i = rng() % 16384; i != 0; --i )
    nums1.insert(rng());
  for ( unsigned int i = rng() % 16384; i != 0; --i )
    nums2.insert(rng());
  for ( unsigned int i = rng() % 16384; i != 0; --i )
  {
    unsigned int j = rng();
    nums1.insert(j);
    nums2.insert(j);
  }
  std::vector<int> v1(nums1.begin(), nums1.end()), v2(nums2.begin(), nums2.end());

  int i = 0, j = 0, k = 0;
  Cmper ci(i), cj(j), ck(k);

  linear_intersect(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result1), ci);
  baeza_intersect < binary_probe > (v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result2), cj);
  baeza_intersect < interpolation_probe > (v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result2), ck);

  std::cout << i << " " << j << " " << k << std::endl;;
}
