#ifndef Analysis_BoostedTopAnalysis_SubjetHelper_h
#define Analysis_BoostedTopAnalysis_SubjetHelper_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Math/interface/LorentzVector.h"

namespace pat {

  void subjetHelper( pat::Jet const &jet,
		     double & y,
		     double & mu,
		     double & dR );

  void subjetHelper( reco::Candidate const & jet1,  reco::Candidate const & jet2,
  			double & y, double & mu, double & dR , double & m_fat );

  void subjetHelper( math::XYZTLorentzVector const & p1, math::XYZTLorentzVector const & p2,
                        double & y, double & mu, double & dR , double & m_fat );

  void subjetHelper( math::PtEtaPhiMLorentzVector const & p1, math::PtEtaPhiMLorentzVector const & p2,
                        double & y, double & mu, double & dR , double  m_fat );
}
#endif
