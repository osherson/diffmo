#include "Analysis/JetAnalysis/interface/SimpleSubstructureSelector.h"

/// --------------------------------------
///   Constructor
///      - Inputs jet source, pt and rapidity cuts.
///      - Inputs muCut and mCut, sets up Selector
/// --------------------------------------
SimpleSubstructureSelector::SimpleSubstructureSelector( edm::ParameterSet const & parameters ) :
  src_            ( parameters.getParameter<edm::InputTag>("src") ),
  rhoSrc_         ( parameters.getParameter<edm::InputTag>("rhoSrc") ),
  pvSrc_          ( parameters.getParameter<edm::InputTag>("pvSrc") ),
  ptCut_          ( parameters.getParameter<double>("ptCut") ), 
  rapidityCut_    ( parameters.getParameter<double>("rapidityCut") ),
  jetScale_       ( parameters.getParameter<double>("jetScale") ),
  jetSmear_       ( parameters.getParameter<double>("jetSmear") ),
  jetUncertainty_ ( parameters.getParameter<double>("jetUncertainty") ),
  jecPayloads_    ( parameters.getParameter<std::vector<std::string> >("jecPayloads") ),
  jecUncPayload_  ( parameters.getParameter<std::string>("jecUncPayload"))
{
  
  push_back("Inclusive");
  push_back("Kin");
  push_back("mu1Cut", parameters.getParameter<double>("muCut"));
  push_back("mu2Cut", parameters.getParameter<double>("muCut"));
  push_back("m1Cut",  parameters.getParameter<double>("mCut"));
  push_back("m2Cut",  parameters.getParameter<double>("mCut"));
      
  set("Inclusive");
  set("Kin");
  set("mu1Cut");
  set("mu2Cut");
  set("m1Cut");
  set("m2Cut");

  indexInclusive_ = index_type(&bits_, "Inclusive" );
  indexKin_       = index_type(&bits_, "Kin"       );
  indexMu1_       = index_type(&bits_, "mu1Cut"    );
  indexMu2_       = index_type(&bits_, "mu2Cut"    );
  indexM1_        = index_type(&bits_, "m1Cut"     );
  indexM2_        = index_type(&bits_, "m2Cut"     );

    
  if ( parameters.exists("cutsToIgnore") )
    setIgnoredCuts( parameters.getParameter<std::vector<std::string> >("cutsToIgnore") );
	
  retInternal_ = getBitTemplate();


  // Get the factorized jet corrector. 
  // The payloads contain N elements, 
  // the Nth is the uncertainty, and the first N-1 elements are the
  // actual correction levels. 
  std::vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator ipayload = jecPayloads_.begin(),
	  ipayloadEnd = jecPayloads_.end(); ipayload != ipayloadEnd; ++ipayload ) {
    std::cout << "Adding payload " << *ipayload << std::endl;
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }

  jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );

  jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecUncPayload_));




}


/// --------------------------------------
///   operator()
///        Performs full selection.
/// 
/// This will make cuts in phases for
///   analysis, starting:
///    - Inclusive (all)
///    - Kinematics (pt > ptMin, |y| < yMax)
///    - Subjet mass symmetry for 1st jet (mu0 < muCut)
///    - Subjet mass symmetry for 2nd jet (mu1 < muCut)
///    - Mass cut for 1st jet (m > mCut)
///    - Mass cut for 2nd jet (m > mCut)
/// --------------------------------------
bool SimpleSubstructureSelector::operator()( edm::EventBase const & iEvent, pat::strbitset & ret){

  // Initialize the Selector strbitset for returning
  ret.set(false);
  passCut( ret, indexInclusive_ );
  // Initialize the cache of the jet stuff in this event
  jetP4_.clear();
  jetMu_.clear();
  // Initialize the cache of the dijet invariant mass
  jjCand_ = reco::Candidate::LorentzVector();

  iEvent.getByLabel( src_, jetsHandle_ );

  edm::Handle<double> rhoHandle;
  iEvent.getByLabel( rhoSrc_, rhoHandle );

  edm::Handle<std::vector<reco::Vertex> > pvHandle;
  iEvent.getByLabel( pvSrc_, pvHandle );


  bool passKin = false;
  if ( jetsHandle_->size() >= 2 ) {

    for ( std::vector<pat::Jet>::const_iterator jetBegin = jetsHandle_->begin(),
	    jetEnd = jetsHandle_->end(), ijet = jetBegin;
	  ijet != jetEnd; ++ijet ) {

      // get a copy of the uncorrected p4
      reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);

      // Then get the correction (L1+L2+L3 [+L2L3 for data])
      jec_->setJetEta( uncorrJet.eta() );
      jec_->setJetPt ( uncorrJet.pt() );
      jec_->setJetE  ( uncorrJet.energy() );
      jec_->setJetA  ( ijet->jetArea() );
      jec_->setRho   ( *(rhoHandle.product()) );
      jec_->setNPV   ( pvHandle->size() );
      double corr = jec_->getCorrection();


      // Here will be the working variable for all the jet energy effects
      reco::Candidate::LorentzVector scaledJetP4 = uncorrJet * corr;


      // -------
      // Jet energy scale variation
      // -------
      if ( fabs(jetScale_) > 0.0 ) {	     


	// Now get the uncertainties
	jecUnc_->setJetEta( scaledJetP4.eta() );
	jecUnc_->setJetPt( scaledJetP4.pt() );
	double unc = fabs(jecUnc_->getUncertainty( bool(jetScale_ > 0) ));

	// Add the "flat" flavor dependent corrections in quadrature
	unc = sqrt( unc*unc + jetUncertainty_*jetUncertainty_);
	     
	// Scale up or down by jetScale_
	double ijetscale = (1 + jetScale_ * unc);
	scaledJetP4 *= ijetscale;
      }// end jet energy scale up and down

      // -------
      // Jet energy resolution variation
      // -------
      if ( fabs(jetSmear_) > 0.0 && ijet->genJet() != 0 && ijet->genJet()->pt() > 15.0 ) {
	double scale = jetSmear_;
	double recopt = ijet->pt();
	double genpt = ijet->genJet()->pt();
	double deltapt = (recopt-genpt)*scale;
	double ptscale = std::max((double)0.0,(recopt+deltapt)/recopt);
	scaledJetP4 *= ptscale;
      } // end jet energy resolution smearing
      
      
	// Now check out the subjets. in this case
	// we required exactly two. 
      reco::Candidate const * da0 = ijet->daughter(0);
      reco::Candidate const * da1 = ijet->daughter(1);

      if ( da0 == 0 || da1 == 0 ) {
	throw cms::Exception("InvalidInput") << " jet has uninitialized daughters" << std::endl;
      }


      // Look at the highest mass subjet
      if ( da1->mass() > da0->mass() ) {
	reco::Candidate const * temp = da0;
	da0 = da1;
	da1 = temp;
      }
	
      // The "mass drop" is defined as mu = m0 / mjet
      double mu = da0->mass() / scaledJetP4.mass();

      jetP4_.push_back( scaledJetP4 );
      jetMu_.push_back( mu );
 
    
    } // end loop over jets
      
      // Now perform kinematic selections

    jjCand_ = jetP4_[0] + jetP4_[1];
      
    passKin = ( jetP4_[0].pt() > ptCut_ && fabs(jetP4_[0].Rapidity()) < rapidityCut_ &&
		jetP4_[1].pt() > ptCut_ && fabs(jetP4_[1].Rapidity()) < rapidityCut_ );

  }

  // Now do the selection
  if ( passKin || ignoreCut(indexKin_) ) {
    passCut( ret, indexKin_ );
    if ( ignoreCut(indexMu1_) || jetMu_[0] < cut(indexMu1_, double() ) ) {
      passCut( ret, indexMu1_);
      if ( ignoreCut(indexMu2_) || jetMu_[1] < cut(indexMu2_, double() ) ) {
	passCut( ret, indexMu2_);
	if ( ignoreCut(indexM1_) || jetP4_[0].mass() > cut(indexM1_, double() ) ) {
	  passCut( ret, indexM1_);
	  if ( ignoreCut(indexM2_) || jetP4_[1].mass() > cut(indexM2_, double() ) ) {
	    passCut( ret, indexM2_);
	  }// end m2 cut
	}// end m1 cut
      }// end mu2 cut
    }// end mu1 cut
  }// end kinematic cuts

  setIgnored(ret);    
  return (bool)ret;
}
