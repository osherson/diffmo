#include "Analysis/BoostedTopAnalysis/interface/SemileptonicSelection.h"
#include "TLorentzVector.h"
#include <iostream>

//using namespace std;
using std::vector;
using std::cout;
using std::map;
using std::endl;
using std::string;

SemileptonicSelection::SemileptonicSelection( edm::ParameterSet const & params ) :
  wPlusJets_(params.getParameter<edm::ParameterSet>("WPlusJetsParams")),
  taggedMuons_(),
  taggedJets_(),
  taggedMETs_(),
  wJet(),
  closestJet(),
  dRMin(TMath::Pi()/3),
  nJetsA(0),
  nJetsB(0),
  nJetsC(0),
  jetSrc(params.getParameter<edm::InputTag>("jetSrc")),
  ptRelMin(params.getParameter<double>("ptRelMin")),
  dRMinCut(params.getParameter<double>("dRMin")),
  anyLeadJetPt(params.getParameter<double>("anyLeadJetPt")),
  oppLeadJetPt(params.getParameter<double>("oppLeadJetPt")),
  htLepCut(params.getParameter<double>("htLepCut")),
  bDiscriminator(params.getParameter<std::string>("bDiscriminator")),
  bDiscriminatorCut(params.getParameter<double>("bDiscriminatorCut"))
{
  // std::cout << "Instantiated SemileptonicSelection" << std::endl;
  // make the bitset
  push_back("Inclusive");
  push_back("Trigger");
  push_back("Lepton");
  push_back("Lepton + >=1 Jets");
  push_back("Lepton + >=2 Jets");
  push_back("Any leadJetPt");
  push_back("Lepton has close jet");
  push_back("Hemispheric");
  push_back("Opposite leadJetPt");
  push_back("Relative Pt and Min Delta R");
  push_back("HTLep");
  push_back("SemilepBtag");
  // all on by default
  set("Inclusive");
  set("Trigger");
  set("Lepton");
  set("Lepton + >=1 Jets");
  set("Lepton + >=2 Jets");
  set("Any leadJetPt");
  set("Lepton has close jet");
  set("Hemispheric");
  set("Opposite leadJetPt");
  set("Relative Pt and Min Delta R");
  set("HTLep", htLepCut);
  set("SemilepBtag", bDiscriminatorCut);

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<vector<string> >("cutsToIgnore") );

  // initialize bitsets for later
  retSemi = wPlusJets_.getBitTemplate();
  // retHad  = boostedTopWTagFunctor_.getBitTemplate();
  retInt = getBitTemplate();

  if ( wPlusJets_.ignoreCut(string("== 1 Lepton")) ||
       wPlusJets_.ignoreCut(string(">=2 Jets"))) {
    std::cout << "In SemileptonicSelection, we require 1 lepton and >= 2 jets, please!" << std::endl;
    wPlusJets_.set(string("== 1 Lepton"), true);
    wPlusJets_.set(string(">=2 Jets"), true);
  }

  retInternal_ = getBitTemplate();
}

bool SemileptonicSelection::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  static int nev = 0;
  nev++;
  // fail everything by default
  ret.set(false);
  retSemi.set(false);
  // retHad.set(false);
  // tagged objects are const references, so they're handled by the
  // selector that owns them

  taggedMuons_.clear();
  taggedJets_.clear();

  passCut(ret,"Inclusive");
  if(ignoreCut("Trigger")) 
    wPlusJets_.set("Trigger", false);

  /*bool passedSemi = */wPlusJets_(event, retSemi);
  // ignoreCut isn't needed here, but it doesn't hurt to be pedantic
  if( ignoreCut("Trigger") || retSemi[string("Trigger")]) passCut(ret, "Trigger");

  // Check to see if we have at least one lepton. If so,
  // this is the first stage of the selection
  if ( retSemi[string("== 1 Lepton")] || ignoreCut("Lepton") ) 
    {
      passCut(ret, string("Lepton"));
      candidate_collection const & taggedMuons     = wPlusJets_.selectedMuons();
      candidate            const & taggedMETs      = wPlusJets_.selectedMET();

      // Check to see if we have at least one jet. This is the
      // second stage of the selection. This allows the computation
      // of the muon 2d cut
      if ( retSemi[string(">=1 Jets")] || ignoreCut("Lepton + >=1 Jets") ) {
	passCut(ret, string("Lepton + >=1 Jets") );
	candidate_collection const & taggedJets      = wPlusJets_.selectedJets();
	wJet = taggedJets.end();

	// calculate the muon 2d cut
	dRMin = 999.0;
	closestJet = taggedJets.end();
	bool foundClose = false;
	bool passAnyLeadJetPtCut = false;
	for ( vector<reco::ShallowClonePtrCandidate>::const_iterator ijet = taggedJets.begin(),
		taggedJets_Begin = taggedJets.begin(), taggedJets_End = taggedJets.end();
	      ijet != taggedJets_End; ++ijet ) 
	  {
	    double dR = reco::deltaR<double>(ijet->eta(), ijet->phi(),
					     taggedMuons[0].eta(), taggedMuons[0].phi() );
	    if ( dR < dRMin ) 
	      {
		dRMin = dR;
		closestJet = ijet;
		foundClose = true;
	      }
	    passAnyLeadJetPtCut |= ijet->pt() > anyLeadJetPt;
	  }
	

		
	// Check if there are at least 2 jets. If so, this is the
	// third stage of the selection
	if( (retSemi[string(">=2 Jets")] || ignoreCut("Lepton + >=2 Jets")) )
	  {
	    passCut(ret, string("Lepton + >=2 Jets"));

	  
	    if ( passAnyLeadJetPtCut || ignoreCut("Any leadJetPt") ) {
	      passCut(ret, string("Any leadJetPt") );
	    
	      // Check the muon 2d cut
	      if ( foundClose || ignoreCut("Lepton has close jet"))  
		{
		  passCut(ret, "Lepton has close jet");
		  TLorentzVector muP ( taggedMuons[0].px(),
				       taggedMuons[0].py(),
				       taggedMuons[0].pz(),
				       taggedMuons[0].energy() );
		
		  TLorentzVector bjetP ( closestJet->px(),
					 closestJet->py(),
					 closestJet->pz(),
					 closestJet->energy() );
		
		
		  double ptRel = TMath::Abs( muP.Perp( bjetP.Vect() ) );
		  bool dRandPtCut = !(ptRel < ptRelMin && dRMin < dRMinCut);
		
		  // Separate out the jets into "Mercedes" regions. 
		  // Region A: Close to the muon (dphi < pi/3)
		  // Region B: Away from the muon (dphi > 2pi/3)
		  // Region C: Sideband regions (2pi/3 > dphi > pi/3)
		  bool oppLeadJetCut = false;
		  double leadPt = -1.0; 
		  for ( vector<reco::ShallowClonePtrCandidate>::const_iterator ijet = taggedJets.begin(),
			  taggedJets_Begin = taggedJets.begin(), taggedJets_End = taggedJets.end();
			ijet != taggedJets_End; ++ijet ) 
		    {
		      if( fabs(reco::deltaPhi<double>(ijet->phi(), taggedMuons[0].phi()) < TMath::Pi()/3 ))
			nJetsA++;
		      else if (fabs(reco::deltaPhi<double>(ijet->phi(), taggedMuons[0].phi())) > 2*TMath::Pi()/3 )
			{
			  oppLeadJetCut |= (ijet->pt() > oppLeadJetPt );
			  if ( ijet->pt() > leadPt ) {
			    leadPt = ijet->pt();
			    wJet = ijet;
			  }
			  nJetsB++;
			}
		      else 
			nJetsC++;
		    }
		
		   
		  if ( (nJetsB >= 0 && wJet != taggedJets.end()) || ignoreCut("Hemispheric") ) {
		    passCut( ret, "Hemispheric");

		    // Require a hard jet in the Mercedes "away" region (region B)
		    if( oppLeadJetCut || ignoreCut("Opposite leadJetPt") )
		      {
			passCut(ret, "Opposite leadJetPt");
		      

			if(dRandPtCut ||  ignoreCut("Relative Pt and Min Delta R") )
			  {
			    passCut(ret, "Relative Pt and Min Delta R");
			  
			    double htLep = taggedMuons[0].pt() + taggedMETs.pt();
			    if ( htLep >= cut("HTLep", double()) || ignoreCut("HTLep") ) {
			      passCut(ret, "HTLep");

			      pat::Jet const * semilepPatJet = dynamic_cast<pat::Jet const *>(closestJet->masterClonePtr().get());
			      if ( (semilepPatJet && 
				    semilepPatJet->bDiscriminator(bDiscriminator) > cut("SemilepBtag", double()) ) 
				   || ignoreCut("SemilepBtag") ) {
				passCut(ret, "SemilepBtag");
			      }
			    }
			  }//if(!oppLeadJetCut)
		      }
		  }// end if >= 2 jets
		} // lepton has pt cut
	    }// any jets with pt > anyLeadJetPt
	  } // end if >= 2 jets
      }// if have >= 1 jets
    } //if have >= 1 muon
  
  return (bool)ret;
}

