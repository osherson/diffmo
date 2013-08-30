#ifndef Analysis_SHyFT_interface_combinatorics_h
#define Analysis_SHyFT_interface_combinatorics_h

#include <vector>
#include <iostream>
//#include <algorithms>
#include <numeric>
#include <functional>
#include <algorithm>

namespace shyft {

  namespace helper {

//
// next_combination
// 
// create combinations according to the algorithm here from Thomas Draper:
//    http://marknelson.us/attachments/next-permutation/NextCombination.tgz
//
// This will modify the sequence pointed to in steps,
// and at the last step, the sequence will be back to the original configuration.
template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
  if ((first == last) || (first == k) || (last == k))
    return false;
  Iterator itr1 = first;
  Iterator itr2 = last;
  ++itr1;
  if (last == itr1)
    return false;
  itr1 = last;
  --itr1;
  itr1 = k;
  --itr2;
  while (first != itr1)
    {
      if (*--itr1 < *itr2)
	{
	  Iterator j = k;
	  while (!(*itr1 < *j)) ++j;
	  std::iter_swap(itr1,j);
	  ++itr1;
	  ++j;
	  itr2 = k;
	  std::rotate(itr1,j,last);
	  while (last != j)
            {
	      ++j;
	      ++itr2;
            }
	  std::rotate(k,itr2,last);
	  return true;
	}
    }
  std::rotate(first,k,last);
  return false;
}


// Helper function to compute prod_i (1-a_i)
template <class T> struct oneminusmultiplies : std::binary_function <T,T,T> {
  T operator() (const T& x, const T& y) const
  {return x.oneminusmultiplies(y);}
};

// The next_combination class has issues with multiple entries.
// It doesn't expect that any distinction will be made for the
// "left" part of the vector and the "right", but we will use the
// "left" part to indicate the "tagged" terms, and the "right" part
// to indicate the "untagged" terms. So this way, we just take
// accumulate(begin,k, multiplies) * accumulate(k,end, oneminusmultiplies)
// and that's the combinatoric. 
// The "EffInfo" deals with this by giving a unique index to sort from
// to generate the combinations, and adds the actual value as "eff".
// There's also an optional flavor "f" for later use. 
struct EffInfo {

  EffInfo( int i, double e, int f) :
    index(i), eff(e), flavor(f)
  {
  }

  // Comparisons on EffInfo use index
  bool operator<( EffInfo const & right ) const {
    return index < right.index;
  }

  bool operator>( EffInfo const & right ) const {
    return index > right.index;
  }

  bool operator==( EffInfo const & right ) const {
    return index == right.index;
  }

  // Mathematical operatios on EffInfo use eff
  EffInfo operator*( EffInfo const & right ) const {
    EffInfo toRet( *this );
    toRet.eff *= right.eff;
    return toRet;
  }


  // To implement the "oneminusmultiplies" functionality
  EffInfo oneminusmultiplies( EffInfo const & r) const {
    EffInfo toRet(*this);
    toRet.eff *= (1.0 - r.eff);
    return toRet;
  }


  // Screen dumps
  friend std::ostream & operator<<( std::ostream & out, EffInfo const & e ) {
    out << e.eff;
    return out;
  }
  
  int index;
  double eff;
  int flavor;
};

  }

}

#endif
