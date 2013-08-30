#ifndef Analysis_BoostedTopAnalysis_interface_WPlusBJetTypes_h
#define Analysis_BoostedTopAnalysis_interface_WPlusBJetTypes_h

#include "Math/GenVector/PxPyPzM4D.h"

using namespace std;

typedef reco::Candidate::LorentzVector FourVector;

struct Type2L {
public :
  Type2L( FourVector wJet_, FourVector aJet_, double w_) : wJet(wJet_), aJet(aJet_), weight(w_) {}
  FourVector    wJet;
  FourVector    aJet;
  double        weight;
  FourVector    top() const { return wJet+aJet; }
};

class Type2T {
public :
  Type2T( FourVector wJet_, FourVector bJet_, double w_) : wJet(wJet_), bJet(bJet_), weight(w_) {}
  FourVector    wJet;
  FourVector    bJet;
  double        weight;
  FourVector    top() const { return wJet+bJet; }
};

class Type3 {
public :
  Type3( FourVector bJet_, FourVector minDrPair0_, FourVector minDrPair1_, double w_ ) :
    bJet(bJet_), minDrPair0(minDrPair0_), minDrPair1(minDrPair1_), weight(w_) {}
  FourVector    bJet;
  FourVector    minDrPair0;
  FourVector    minDrPair1;
  double        weight;
  FourVector    top() const { return minDrPair0+minDrPair1+bJet; }
  double        minMass() const { return (minDrPair0+minDrPair1).mass(); }
};


enum ttTypes { Undef, Type22, Type23, Type33 };

#endif
