#include "Analysis/BoostedTopAnalysis/interface/SubjetHelper.h"

void pat::subjetHelper( pat::Jet const &jet,
			double & y,
			double & mu,
			double & dR ) {

  int numOfDaughters 	= jet.numberOfDaughters();

  if ( numOfDaughters < 2 ) {
    y = mu = dR = -1.0;
    return;
  }
  double mfat		= jet.mass();
  double m0		= jet.daughter(0) -> mass();
  double m1		= jet.daughter(1) -> mass();
  double pt0		= jet.daughter(0) -> pt();
  double pt1		= jet.daughter(1) -> pt();
  if ( m1 > m0 ) {
    double temp = m1;
    m1 = m0;
    m0 = temp;
    temp = pt1;
    pt1 = pt0;
    pt0 = temp;
  }

  dR = reco::deltaR<double>( jet.daughter(0) ->eta(),
			     jet.daughter(0) ->phi(),
			     jet.daughter(1) ->eta(),
			     jet.daughter(1) ->phi()  );
  y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (mfat*mfat);
  mu = m0 / mfat ;

}

void pat::subjetHelper( math::XYZTLorentzVector const & p1, math::XYZTLorentzVector const & p2,
			double & y, double & mu, double & dR , double & mfat )  {

  math::XYZTLorentzVector p = p1 + p2;
  mfat = p.M();
  double m0 = p1.M();
  double m1 = p2.M();
  double pt0 = p1.pt();
  double pt1 = p2.pt();
  if ( m1 > m0 ) {
    double temp = m1;
    m1 = m0;
    m0 = temp;
    temp = pt1;
    pt1 = pt0;
    pt0 = temp;
  }

  dR = reco::deltaR<double>( p1.eta(),
                             p1.phi(),
                             p2.eta(),
                             p2.phi()  );
  y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (mfat*mfat);
  mu = m0 / mfat ;

  return ;
}



void pat::subjetHelper( math::PtEtaPhiMLorentzVector const & p1, math::PtEtaPhiMLorentzVector const & p2,
			double & y, double & mu, double & dR , double mfat )  {

  math::PtEtaPhiMLorentzVector p = p1 + p2;
  double m0 = p1.M();
  double m1 = p2.M();
  double pt0 = p1.pt();
  double pt1 = p2.pt();
  if ( m1 > m0 ) {
    double temp = m1;
    m1 = m0;
    m0 = temp;
    temp = pt1;
    pt1 = pt0;
    pt0 = temp;
  }

  dR = reco::deltaR<double>( p1.eta(),
                             p1.phi(),
                             p2.eta(),
                             p2.phi()  );
  y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (mfat*mfat);
  mu = m0 / mfat ;

  return ;
}

void pat::subjetHelper( reco::Candidate const & jet1, reco::Candidate const & jet2,
                        double & y, double & mu, double & dR , double & mfat )  {

  pat::subjetHelper( jet1.p4(), jet2.p4(), y, mu, dR , mfat);
  return;
}

