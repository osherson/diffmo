#include "Analysis/SHyFT/interface/combinatorics.h"



int main (int argc, char ** argv)
{
  std::vector<shyft::helper::EffInfo> s;
  s.push_back( shyft::helper::EffInfo(0, 0.50, 0) );
  s.push_back( shyft::helper::EffInfo(1, 0.10, 1) );
  s.push_back( shyft::helper::EffInfo(2, 0.05, 2) );
  s.push_back( shyft::helper::EffInfo(3, 0.50, 0) );


  shyft::helper::EffInfo unity( -1, 1.0, 0);

  std::sort( s.begin(), s.end() );

  std::cout << "Looking for these efficiencies: " << std::endl;
  copy(s.begin(), s.end(), std::ostream_iterator<shyft::helper::EffInfo>(std::cout, " "));

  std::cout << std::endl;
  double sum = 0.0;

  // Loop over all the possible "N_jets"
  for ( std::size_t k = 0; k <= s.size(); ++k ) {
    std::cout << " ------ k = " << k << std::endl;
    // Loop over all combinations
    double ksum = 0.0;
    do
      {
	// Dump to the screen
	copy(s.begin(), s.end(), std::ostream_iterator<shyft::helper::EffInfo>(std::cout, " "));
	// Get the "tag" bit, from "begin" to "k"
	shyft::helper::EffInfo ni = accumulate(s.begin(), s.begin() + k, unity, std::multiplies<shyft::helper::EffInfo>());
	// Get the "untag" bit, from "k" to "end"
	shyft::helper::EffInfo nj = accumulate(s.begin()+k, s.end(), unity, shyft::helper::oneminusmultiplies<shyft::helper::EffInfo>());
	// The product is the total probability to tag exact k jets.
	sum += ni.eff * nj.eff;	
	ksum += ni.eff * nj.eff;
	std::cout << "    tagprob = " << ni.eff << ", untagprob = " << nj.eff << ", prod = " << ni.eff*nj.eff << std::endl;
      }
    while(shyft::helper::next_combination(s.begin(),s.begin() + k, s.end()));
    std::cout << " Sum for k = " << k << " = " << ksum << std::endl;
  }
  // Print out the total
  std::cout << "Total = " << sum << std::endl;
}

