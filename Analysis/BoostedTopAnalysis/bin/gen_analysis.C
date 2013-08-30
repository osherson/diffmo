#include "Analysis/BoostedTopAnalysis/interface/HadronicAnalysis.h"

#include "PhysicsTools/SelectorUtils/interface/RunLumiSelector.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()

using namespace std;


///-------------------------
/// DRIVER FUNCTION
///-------------------------

// -*- C++ -*-

// CMS includes
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"


// Root includes
#include "TROOT.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"


using namespace std;


///////////////////////////
// ///////////////////// //
// // Main Subroutine // //
// ///////////////////// //
///////////////////////////

int main (int argc, char* argv[]) 
{


  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  cout << "Hello from " << argv[0] << "!" << endl;


  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();


  cout << "Getting parameters" << endl;
  // Get the python configuration
  PythonProcessDesc builder(argv[1], argc, argv);
  boost::shared_ptr<edm::ProcessDesc> b = builder.processDesc();
  boost::shared_ptr<edm::ParameterSet> parameters = b->getProcessPSet();
  parameters->registerIt(); 


  edm::ParameterSet inputs              = parameters->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet outputs             = parameters->getParameter<edm::ParameterSet>("outputs");

  cout << "setting up TFileService" << endl;
  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
  TFileDirectory theDir = fs.mkdir( "histos" ); 
    
  cout << "Setting up chain event" << endl;
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );


  std::cout << "Making hadronic analysis object" << std::endl;

  TH1F * h_dijetMass = theDir.make<TH1F>("h_dijetMass", "Dijet Mass", 500, 0., 5000.);
  TH1F * h_genPt     = theDir.make<TH1F>("h_genPt", "GenJet p_{T}", 500, 0., 5000.);
  TH1F * h_genParton  = theDir.make<TH1F>("h_genParton",  "Parton pdgId", 50, -25,  25 );
  TH1F * h_partonFla  = theDir.make<TH1F>("h_partonFla",  "Jet Flavor",   50, -25,  25 );
  TH1F * h_numBC      = theDir.make<TH1F>("h_numBC",      "Number of BC parton",  10,  0,  10 );
  TH1F * h_numB       = theDir.make<TH1F>("h_numB",       "Number of B parton",   10,  0,  10 );
  TH1F * h_numC       = theDir.make<TH1F>("h_numC",       "Number of C parton",   10,  0,  10 );


  cout << "About to begin looping" << endl;

  int nev = 0;
  //loop through each event
  for (ev.toBegin(); ! ev.atEnd(); ++ev, ++nev) {
    edm::EventBase const & event = ev;
    if( nev > 10000 )  break;


    if ( ev.event()->size() == 0 ) continue; // skip trees with no events

    edm::Handle< std::vector<reco::GenJet> > h_genJets;
    event.getByLabel< std::vector<reco::GenJet> > ( edm::InputTag("ca8GenJets"), h_genJets );
    edm::Handle< std::vector<reco::GenParticle>  > h_genPtrs;
    event.getByLabel<  std::vector<reco::GenParticle> > ( edm::InputTag("genParticles"), h_genPtrs );
    edm::Handle< std::vector<pat::Jet>  >  h_jets;
    event.getByLabel< std::vector<pat::Jet>  > (edm::InputTag("selectedPatJetsCA8PrunedPF"), h_jets );

    std::vector<pat::Jet>::const_iterator  jetBegin = h_jets->begin(), jetEnd = h_jets->end(), ijet = jetBegin;
    for( ; ijet != jetEnd ; ++ijet )
    {
      int partonFlavour = fabs( ijet->partonFlavour() );
      int genPartonId;
      if( ijet->pt() > 50 ) {
        h_partonFla   ->  Fill( ijet->partonFlavour() );
        if( ijet->genParton() )
        {
          h_genParton   ->  Fill( ijet->genParton()->pdgId() );
          genPartonId = ijet->genParton()->pdgId();
        }
        else {
          h_genParton   ->  Fill( 0 );
          genPartonId  = 0;
        }
        //Check if it's gluon splitting
        if( (partonFlavour == 4 || partonFlavour == 5) && genPartonId == 21 )
        {
          int numB =0 , numC = 0;
          const reco::GenParticle * theParton = ijet->genParton();
          //cout<<"theParton status "<<theParton->status() << " and adress " << theParton <<endl;

          std::vector<const reco::GenParticle * >  bPartons;
          std::vector<const reco::GenParticle * >  cPartons;
          for( std::vector<reco::GenParticle>::const_iterator partonBegin = h_genPtrs->begin(), partonEnd = h_genPtrs->end(),
            iparton = partonBegin;  iparton != partonEnd; iparton++ )
            {
              if( iparton->status() == 2 && fabs(iparton->pdgId()) == 4 )
                cPartons.push_back( &(*iparton) );
              if( iparton->status() == 2 && fabs(iparton->pdgId()) == 5 )
                bPartons.push_back( &(*iparton) );
            } // genParticles loop
          //cout<<"bPartons " << bPartons.size() << " cPartons "<< cPartons.size() << endl;

          //Chech how many of b's, c's are daughters of the gluon jet
          for( size_t i=0; i < bPartons.size() ; i++ ) {
            bool stop = false;
            bool matched = false;
            const reco::Candidate * up = bPartons[i];
            //cout<<"up is "<< up <<endl;
            do {
              double dR = reco::deltaR<double>( theParton->eta(), theParton->phi(), up->eta(), up->phi() );
              //if( up == theParton ) {
              if( dR < 0.1 ) {
                matched = true;
                stop = true;
                numB ++;
              }
              //else if( up->status() == 3 || !up->mother(0) )
              else if( !up->mother(0) ) {
                    stop = true;
                    //cout<<"Stopped here?"<<endl;
                    }
                    else {
                      //cout<<"Going up"<<endl;
                      up = up->mother(0);
                    }
            }  while ( !stop );
          } // end for bPartons

          for( size_t i=0; i < cPartons.size(); i++ ) {
            bool stop = false;
            bool matched = false;
            const reco::Candidate * up = cPartons[i];
            do {
              double dR = reco::deltaR<double>( theParton->eta(), theParton->phi(), up->eta(), up->phi() );
              //if( up == theParton ) {
              if( dR < 0.1 ) {
                matched = true;
                stop = true;
                numC ++;
              }
              //else if( up->status() == 3 || !up->mother(0) )
              else if( !up->mother(0) )
                    stop = true;
                    else {
                      //cout<<"Going up"<<endl;
                      up = up->mother(0);
                    }
            }  while ( !stop );

          } // end for cPartons
          //cout<<"numB " << numB << " numC " << numC <<endl;
          h_numB    ->  Fill( numB );
          h_numC    ->  Fill( numC );
          h_numBC   ->  Fill( numB+numC );
        } // end if genPartonId 21
      }
    }  // end for ijet

    if ( h_genJets->size() >= 2 ) {
      reco::GenJet const & jet0 = h_genJets->at(0);
      reco::GenJet const & jet1 = h_genJets->at(1);

      h_genPt->Fill( jet0.pt() );
      h_genPt->Fill( jet1.pt() );
      reco::Candidate::LorentzVector p4 = jet0.p4() + jet1.p4();
      h_dijetMass->Fill( p4.mass() );
      
    }

  } // end loop over events


  return 0;
}

