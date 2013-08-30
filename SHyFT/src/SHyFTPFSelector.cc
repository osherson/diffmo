#include "Analysis/SHyFT/interface/SHyFTPFSelector.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"


#include <iostream>

using namespace std;

SHyFTPFSelector::SHyFTPFSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  muonTag_           (params.getParameter<edm::InputTag>("muonSrc") ),
  electronTag_       (params.getParameter<edm::InputTag>("electronSrc") ),
  jetTag_            (params.getParameter<edm::InputTag>("jetSrc") ),
  metTag_            (params.getParameter<edm::InputTag>("metSrc") ),  
  pvTag_             (params.getParameter<edm::InputTag>("pvSrc") ),
  trigTag_           (params.getParameter<edm::InputTag>("trigSrc") ),
  muonIdPFTight_     (params.getParameter<edm::ParameterSet>("muonIdPFTight")),
  muonIdPFLoose_     (params.getParameter<edm::ParameterSet>("muonIdPFLoose")),
  electronIdPFTight_ (params.getParameter<edm::ParameterSet>("electronIdPFTight")),
  electronIdPFLoose_ (params.getParameter<edm::ParameterSet>("electronIdPFLoose")), 
  minJets_           (params.getParameter<int> ("minJets") ),
  muPtMin_           (params.getParameter<double>("muPtMin")), 
  muEtaMax_          (params.getParameter<double>("muEtaMax")), 
  eleEtMin_          (params.getParameter<double>("eleEtMin")), 
  eleEtaMax_         (params.getParameter<double>("eleEtaMax")), 
  muPtMinLoose_      (params.getParameter<double>("muPtMinLoose")), 
  muEtaMaxLoose_     (params.getParameter<double>("muEtaMaxLoose")), 
  eleEtMinLoose_     (params.getParameter<double>("eleEtMinLoose")), 
  eleEtaMaxLoose_    (params.getParameter<double>("eleEtaMaxLoose")), 
  jetPtMin_          (params.getParameter<double>("jetPtMin")), 
  jetEtaMax_         (params.getParameter<double>("jetEtaMax")), 
  jetScale_          (params.getParameter<double>("jetScale")),
  jetUncertainty_    (params.getParameter<double>("jetUncertainty")),
  jetSmear_          (params.getParameter<double>("jetSmear")),
  metMin_            (params.getParameter<double>("metMin")),
  metMax_            (params.getParameter<double>("metMax")),
  unclMetScale_      (params.getParameter<double>("unclMetScale")),
  useData_           (params.getParameter<bool>("useData")),
  useL1Corr_         (params.getParameter<bool>("useL1Corr")),
  jecPayload_        (params.getParameter<std::string>("jecPayload"))
{

  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Lepton"    );
  push_back( "== 1 Tight Lepton"    );
  push_back( "0 other lepton"    );
  push_back( "MET Cut Min"    );
  push_back( "MET Cut Max"    );
  push_back( ">=1 Jets"       );
  push_back( ">=2 Jets"       );
  push_back( ">=3 Jets"       );
  push_back( ">=4 Jets"       );
  push_back( ">=5 Jets"       );


  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 Lepton"    );
  set( "== 1 Tight Lepton"    );
  set( "0 other lepton"    );
  set( "MET Cut Min"    );
  set( "MET Cut Max"    ); 
  set( ">=1 Jets", minJets_ >= 1);
  set( ">=2 Jets", minJets_ >= 2);
  set( ">=3 Jets", minJets_ >= 3);
  set( ">=4 Jets", minJets_ >= 4);
  set( ">=5 Jets", minJets_ >= 5); 


  inclusiveIndex_ = index_type(&bits_, std::string("Inclusive"      ));
  triggerIndex_   = index_type(&bits_, std::string("Trigger"        ));
  lep1Index_      = index_type(&bits_, std::string(">= 1 Lepton"    ));
  lep2Index_      = index_type(&bits_, std::string("== 1 Tight Lepton"    ));
  lep3Index_      = index_type(&bits_, std::string("0 other lepton"    ));
  metLowIndex_    = index_type(&bits_, std::string("MET Cut Min"));
  metHighIndex_   = index_type(&bits_, std::string("MET Cut Max"));


  jet1Index_ = index_type(&bits_, std::string(">=1 Jets"));
  jet2Index_ = index_type(&bits_, std::string(">=2 Jets"));
  jet3Index_ = index_type(&bits_, std::string(">=3 Jets"));
  jet4Index_ = index_type(&bits_, std::string(">=4 Jets"));
  jet5Index_ = index_type(&bits_, std::string(">=5 Jets")); 

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );
	

  retInternal_ = getBitTemplate();

  string L1Tag   = "Jec10V1_L1Offset_AK5PF.txt";
  string L3Tag   = "Jec10V1_L3Absolute_AK5PF.txt";
  string L2Tag   = "Jec10V1_L2Relative_AK5PF.txt";
  string L2L3Tag = "Jec10V1_L2L3Residual_AK5PF.txt"; 
  JetCorrectorParameters L1JetPar(L1Tag);
  JetCorrectorParameters L3JetPar(L3Tag);
  JetCorrectorParameters L2JetPar(L2Tag);
  JetCorrectorParameters L2L3JetPar(L2L3Tag);
  vector<JetCorrectorParameters> vPar;
  if ( useL1Corr_ )
    vPar.push_back(L1JetPar);
  vPar.push_back(L2JetPar);
  vPar.push_back(L3JetPar);
  if ( useData_ ) {
    vPar.push_back(L2L3JetPar);
  }

  jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );

  jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecPayload_));

}

bool SHyFTPFSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  // std::cout << "" << std::endl;

  ret.set(false);

  selectedJets_.clear();

  allMuons_.clear();
  selectedTightMuons_.clear();
  selectedLooseMuons_.clear();

  allElectrons_.clear();
  selectedTightElectrons_.clear();
  selectedLooseMuons_.clear();

  selectedMETs_.clear();

  passCut( ret, inclusiveIndex_);


  bool passTrig = false;
  if (!ignoreCut(triggerIndex_) ) {
    edm::Handle<pat::TriggerEvent> triggerEvent;
    pat::TriggerEvent const * trig ;      
    if( trig_ != "" ){
      event.getByLabel(trigTag_, triggerEvent);
      trig = &*triggerEvent;
 
      pat::TriggerPath const * path = trig->path(trig_);
         
      if ( path != 0 && path->wasAccept() ) {
	passTrig = true;    
      }  
    } 
  }
  

  if ( ignoreCut(triggerIndex_) || 
       passTrig ) {
    passCut(ret, triggerIndex_);


    edm::Handle< vector< pat::Electron > > electronHandle;
    event.getByLabel (electronTag_, electronHandle);
  
    edm::Handle< vector< pat::Muon > > muonHandle;
    event.getByLabel (muonTag_, muonHandle);

    edm::Handle< vector<reco::Vertex> > pvHandle;
    event.getByLabel (pvTag_, pvHandle);

    edm::Handle< vector< pat::Jet > > jetHandle;

    edm::Handle< vector< pat::MET > > metHandle;
    event.getByLabel (metTag_, metHandle);

    reco::Candidate::LorentzVector metP4 = metHandle->at(0).p4();


    //--------------------------     
    // Muon Selection
    //--------------------------     
  
    for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(), muonEnd = muonHandle->end(), imuon = muonBegin;
	  imuon != muonEnd; ++imuon ) {
      allMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
      if ( !imuon->isGlobalMuon() ) continue;
	
      // Tight cuts
      bool passTight = imuon->isTrackerMuon() && imuon->isGlobalMuon() && muonIdPFTight_(*imuon);
      if (  imuon->pt() > muPtMin_ && fabs(imuon->eta()) < muEtaMax_ && passTight ) {
	selectedTightMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
      }
      else {
	// Loose cuts
	bool passLoose = imuon->isGlobalMuon() && muonIdPFLoose_(*imuon) ;
	if ( imuon->pt() > muPtMinLoose_ && fabs(imuon->eta()) < muEtaMaxLoose_ && passLoose ) {
	  selectedLooseMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
	}
      }
    }
	 
    //--------------------------     
    // Electron Selection
    //--------------------------     
  
    for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(), electronEnd = electronHandle->end(), ielectron = electronBegin;
	  ielectron != electronEnd; ++ielectron ) {
      allElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, ielectron - electronBegin ) ) );
	
      // Tight cuts
      bool passTight = electronIdPFTight_(*ielectron);
      if (  ielectron->pt() > eleEtMin_ && fabs(ielectron->eta()) < eleEtaMax_ && passTight ) {
	selectedTightElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, ielectron - electronBegin ) ) );
      } else {

	// Loose cuts
	bool passLoose = electronIdPFLoose_(*ielectron);
	if ( ielectron->pt() > eleEtMinLoose_ && fabs(ielectron->eta()) < eleEtaMaxLoose_ && passLoose ){
	  selectedLooseElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, ielectron - electronBegin ) ) );
	}
      }
    }

    //Loop over jets and remove tight leptons within

    event.getByLabel (jetTag_, jetHandle);

    for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
	    jetEnd = jetHandle->end(), ijet = jetBegin;
	  ijet != jetEnd; ++ijet ) {

      // get a copy of the uncorrected p4
      reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);

      // Remove the 4-vectors from any tight leptons within the jet
      for ( SHyFTPFSelector::const_iterator imu = selectedTightMuons_.begin(),
	      imuEnd = selectedTightMuons_.end(); imu != imuEnd; ++imu ) {
	if ( reco::deltaR<reco::Candidate,reco::Candidate>( *ijet, *imu ) < 0.5 ) 
	  uncorrJet -= imu->p4();
      }

      for ( SHyFTPFSelector::const_iterator iele = selectedTightElectrons_.begin(),
	      ieleEnd = selectedTightElectrons_.end(); iele != ieleEnd; ++iele ) {
	if ( reco::deltaR<reco::Candidate,reco::Candidate>( *ijet, *iele ) < 0.5 ) 
	  uncorrJet -= iele->p4();
      }
      
      // Then get the correction (L1+L2+L3 [+L2L3 for data])
      jec_->setJetEta( uncorrJet.eta() );
      jec_->setJetPt ( uncorrJet.pt() );
      jec_->setJetE  ( uncorrJet.energy() );
      jec_->setNPV   ( pvHandle->size() );
      double corr = jec_->getCorrection();

      // Here will be the working variable for all the jet energy effects
      reco::Candidate::LorentzVector scaledJetP4 = uncorrJet * corr;


      // -------
      // Jet energy scale variation
      //    - Also computes a piece of MET uncertainty due to this effect
      // -------
      if ( fabs(jetScale_) > 0.0 ) {	     
	// First subtract the uncorrected px and py from MET
	metP4.SetPx( metP4.Px() + uncorrJet.px() );
	metP4.SetPy( metP4.Py() + uncorrJet.py() );


	// Now get the uncertainties
	jecUnc_->setJetEta( scaledJetP4.eta() );
	jecUnc_->setJetPt( scaledJetP4.pt() );
	double unc = fabs(jecUnc_->getUncertainty( bool(jetScale_ > 0) ));

	// Add the "flat" flavor dependent corrections in quadrature
	unc = sqrt( unc*unc + jetUncertainty_*jetUncertainty_);
	     
	// Scale up or down by jetScale_
	double ijetscale = (1 + jetScale_ * unc);
	scaledJetP4 *= ijetscale;

	// Correct the MET back again for this effect
	metP4.SetPx( metP4.Px() - uncorrJet.px() * ijetscale);
	metP4.SetPy( metP4.Py() - uncorrJet.py() * ijetscale);	     

      }

      // -------
      // Jet energy resolution variation
      //    - Also computes a piece of MET uncertainty due to this effect
      // -------
      if ( fabs(jetSmear_) > 0.0 && ijet->genJet() != 0 && ijet->genJet()->pt() > 15.0 ) {
	// First subtract the uncorrected px and py from MET
	metP4.SetPx( metP4.Px() + uncorrJet.px() );
	metP4.SetPy( metP4.Py() + uncorrJet.py() );
	// Next smear the jets
	double scale = jetSmear_;
	double recopt = ijet->pt();
	double genpt = ijet->genJet()->pt();
	double deltapt = (recopt-genpt)*scale;
	double ptscale = std::max((double)0.0,(recopt+deltapt)/recopt);
	scaledJetP4 *= ptscale;
	// Correct the MET back again for this effect
	metP4.SetPx( metP4.Px() - uncorrJet.px() * ptscale);
	metP4.SetPy( metP4.Py() - uncorrJet.py() * ptscale);
      }


      reco::ShallowClonePtrCandidate scaledJet ( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Jet>( jetHandle, ijet - jetBegin ),
										 ijet->charge(),
										 scaledJetP4 ) );
      if ( scaledJet.pt() > jetPtMin_ && fabs(scaledJet.eta()) < jetEtaMax_ ) {             
	selectedJets_.push_back( scaledJet );
      }
    }// end loop over jets
	   
    // -------
    // Unclustered MET resolution
    // -------
    if ( unclMetScale_ > 0.0 ) {

      // Subtract off the (uncorrected) jets
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator jetBegin = selectedJets_.begin(),
	      jetEnd = selectedJets_.end(), ijet = jetBegin;
	    ijet != jetEnd; ++ijet ) {
	pat::Jet const & jet = dynamic_cast<pat::Jet const &>( *(ijet->masterClonePtr().get() ));
	metP4.SetPx( metP4.px() + jet.correctedP4(0).px() );
	metP4.SetPy( metP4.py() + jet.correctedP4(0).py() );
      }

      // Subtract off muons
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator muBegin = allMuons_.begin(),
	      muEnd = allMuons_.end(), imu = muBegin;
	    imu != muEnd; ++imu ) {
	metP4.SetPx( metP4.px() + imu->px() );
	metP4.SetPy( metP4.py() + imu->py() );	       
      }

      // Subtract off electrons
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator eleBegin = allElectrons_.begin(),
	      eleEnd = allElectrons_.end(), iele = eleBegin;
	    iele != eleEnd; ++iele ) {
	metP4.SetPx( metP4.px() + iele->px() );
	metP4.SetPy( metP4.py() + iele->py() );	       
      }

      // met_x and met_y are now unclustered energy
      // apply the 10% on the unclustered energy. "factor" is either 0.9 or 1.1, for MET_minus or MET_plus, resp.
      metP4.SetPx( metP4.px() * unclMetScale_ );
      metP4.SetPy( metP4.py() * unclMetScale_ );


      // Add back the jets
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator jetBegin = selectedJets_.begin(),
	      jetEnd = selectedJets_.end(), ijet = jetBegin;
	    ijet != jetEnd; ++ijet ) {
	pat::Jet const & jet = dynamic_cast<pat::Jet const &>( *(ijet->masterClonePtr().get() ));
	metP4.SetPx( metP4.px() - jet.correctedP4(0).px() );
	metP4.SetPy( metP4.py() - jet.correctedP4(0).py() );
      }

      // Add back the muons
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator muBegin = allMuons_.begin(),
	      muEnd = allMuons_.end(), imu = muBegin;
	    imu != muEnd; ++imu ) {
	metP4.SetPx( metP4.px() - imu->px() );
	metP4.SetPy( metP4.py() - imu->py() );	       
      }


      // Add back the electrons
      for ( std::vector<reco::ShallowClonePtrCandidate>::const_iterator eleBegin = allElectrons_.begin(),
	      eleEnd = allElectrons_.end(), iele = eleBegin;
	    iele != eleEnd; ++iele ) {
	metP4.SetPx( metP4.px() - iele->px() );
	metP4.SetPy( metP4.py() - iele->py() );	       
      }


    }

     
    // Set the MET
    met_ = reco::ShallowClonePtrCandidate( edm::Ptr<pat::MET>( metHandle, 0),
					   metHandle->at(0).charge(),
					   metP4 );



    int nleptons = selectedTightMuons_.size() + selectedTightElectrons_.size();
    // Check >= 1 Tight lepton
    if ( ignoreCut(lep1Index_) || 
	 ( nleptons > 0 ) ){
      passCut( ret, lep1Index_);

      // Check == 1 Tight lepton
      if ( ignoreCut(lep2Index_) || 
	   ( nleptons == 1 ) ){
	passCut( ret, lep2Index_);
              
	// Check == 1 Lepton (i.e. dilepton veto)
	bool oneLepton = 
	  ( selectedTightMuons_.size() + selectedTightElectrons_.size() +
	    selectedLooseMuons_.size() + selectedLooseElectrons_.size() <= 1
	    );
      
	if ( ignoreCut(lep3Index_) || 
	     oneLepton
	     ) {
	  passCut(ret, lep3Index_);

  
	  bool metCutMin = met_.pt() > metMin_;
	  bool metCutMax = met_.pt() < metMax_;
	  //cout << "metCutMin = " << metCutMin << ",metCutMax = " << metCutMax << endl;
	  //cout << "minCut = " << metMin_ << ", maxCut = " << metMax_ << endl;
	  if ( ignoreCut(metLowIndex_) || (metCutMin)) {
	    passCut( ret, metLowIndex_ );
	  
	    if ( ignoreCut(metHighIndex_) || (metCutMax)) {
	      passCut( ret, metHighIndex_ );

	      if ( ignoreCut(jet1Index_) ||
		   static_cast<int>(selectedJets_.size()) >=  1 ){
		passCut(ret,jet1Index_);  
	      } // end if >=1 tight jets

	      if ( ignoreCut(jet2Index_) ||
		   static_cast<int>(selectedJets_.size()) >=  2 ){
		passCut(ret,jet2Index_);  
	      } // end if >=2 tight jets

	      if ( ignoreCut(jet3Index_) ||
		   static_cast<int>(selectedJets_.size()) >=  3 ){
		passCut(ret,jet3Index_);  
	      } // end if >=3 tight jets

	      if ( ignoreCut(jet4Index_) ||
		   static_cast<int>(selectedJets_.size()) >=  4 ){
		passCut(ret,jet4Index_);  
	      } // end if >=4 tight jets

	      if ( ignoreCut(jet5Index_) ||
		   static_cast<int>(selectedJets_.size()) >=  5 ){
		passCut(ret,jet5Index_);  
	      }

	    } // end if met cut max

	  }// end if met cut low
	  
	} // end if == 1 lepton

      } // end if == 1 tight lepton

    } // end if >= 1 tight lepton
    
  } // end if trigger


  setIgnored(ret);
  return (bool)ret;
}
