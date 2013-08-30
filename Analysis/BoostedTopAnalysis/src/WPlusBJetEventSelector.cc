#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetEventSelector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <utility>


WPlusBJetEventSelector::WPlusBJetEventSelector ( edm::ParameterSet const & params ) :
  pfJetIdParams_         (params.getParameter<edm::ParameterSet>("pfJetIDParams") ),
  pfJetSel_              (new PFJetIDSelectionFunctor(pfJetIdParams_)),
  jetTag_	(params.getParameter<edm::InputTag>("jetSrc")  ),
  wJetSelector_ (params.getParameter<edm::ParameterSet>("BoostedTopWJetParameters") ),
  jetPtMin_	(params.getParameter<double>("jetPtMin") ),
  jetEtaMax_	(params.getParameter<double>("jetEtaMax") ),
  bTagAlgo_	(params.getParameter<string>("bTagAlgorithm") ),
  bTagOP_	(params.getParameter<double>("bTagOP")  ),
  mistagFileName_  (params.getParameter<string>("mistagFileName") )
{
  //make the bitset
  push_back("Inclusive");
  push_back("Jet Preselection"  );
  push_back(">= 1 WJet");
  push_back(">= 1 bJet");

  //turn on
  set("Inclusive");
  set("Jet Preselection" );
  set(">= 1 WJet");
  set(">= 1 bJet");

  mistagFile_   = TFile::Open( mistagFileName_.c_str() );
  wMistag_      = (TH1F*)mistagFile_   -> Get("wMistag");
  bMistag_      = (TH1F*)mistagFile_   -> Get("bMistag");

  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "Module requires the RandomNumberGeneratorService\n";
  }

  CLHEP::HepRandomEngine& engine = rng->getEngine();
  flatDistribution_ = new CLHEP::RandFlat(engine, 0., 1.);

}

bool WPlusBJetEventSelector::operator() (edm::EventBase const & t, reco::Candidate::LorentzVector const & v, pat::strbitset & ret, bool towards)
{
  ret.set(false);
  wJets_.clear();
  bJets_.clear();
  pfJets_.clear();
  allJets_.clear();
  minDrPair_.clear();
  looseTops_.clear();
  tightTops_.clear();
  type3Tops_.clear();
  aJetFound_ = false;

  edm::Handle<vector<pat::Jet>  >   jetHandle;
  t.getByLabel( jetTag_, jetHandle );

  //Get the towards Lorentz vector
  vtowards = (towards) ? v : (-1)*v ;
  //Contain non-tagged jets inside the cone
  std::vector<edm::Ptr<pat::Jet> >  nonTags;

  pat::strbitset retPFJet = pfJetSel_->getBitTemplate();
  //Apply pfJets ID
  for( vector<pat::Jet>::const_iterator jetBegin=jetHandle->begin(), jetEnd=jetHandle->end(), ijet=jetBegin ;
    ijet!=jetEnd; ijet++ )
  {
    if( ijet->pt() > jetPtMin_ && fabs( ijet->eta() ) < jetEtaMax_ )  {
      retPFJet.set(false);
      bool passJetID = (*pfJetSel_)( *ijet, retPFJet );
      if( passJetID )
        pfJets_.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
    } // end if jetPt, jetEta
  }

  //Search for W, b jets
  for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=pfJets_.begin(), jetEnd=pfJets_.end(), ijet=jetBegin ;
    ijet!=jetEnd; ijet++ )
  {
    pat::Jet const & jet = **ijet;
    //Only consider jets in the towards hemisphere
    double dPhi_ = fabs( reco::deltaPhi<double>( vtowards.phi(), jet.phi()  ) );
    if( dPhi_ < TMath::Pi()/2 ) {
      allJets_.push_back( *ijet );
      pat::strbitset iret = wJetSelector_.getBitTemplate();
      if( wJetSelector_( jet, iret )  ) {
        wJets_.push_back( *ijet  );
      } // end if wjet selector
      // not W jet, check b tag
      else {
        //toy tagger
        //double randomTag = flatDistribution_->fire();
        if( jet.bDiscriminator( bTagAlgo_ ) > bTagOP_ )
        //if( randomTag < 1./3. ) 
          bJets_.push_back( *ijet );
        else  // put inside the nonTags container
          nonTags.push_back( *ijet  );
      }  // end else
    } // end if deltaR
  }  // end for pat jets

  //Since only the bJet, wJet on the top be used, put the rest back in the nonTags
  for( size_t i=1; i<wJets_.size(); i++ )
    nonTags.push_back( wJets_.at(i) );
  for( size_t i=1; i<bJets_.size(); i++ )
    nonTags.push_back( bJets_.at(i) );

  //If W is found, search for the nearest jet
  if( hasWJets() ) {
    double minDeltaR = 99999. ;
    for( size_t i=0; i<nonTags.size(); i++ ) {
      double deltaR_ = reco::deltaR<double>( wJets_.at(0)->eta(), wJets_.at(0)->phi(), 
      						nonTags.at(i)->eta(), nonTags.at(i)->phi() );
      if( deltaR_ < minDeltaR ) {
        minDeltaR = deltaR_;
	aJetFound_ = true;
	aJet_ = nonTags.at(i);
      }  //end if < minDeltaR 
    }  // end i
  }  // end if hasWJets

  //Found the min DeltaR pair of jets
  double minDeltaR = 9999. ;
  for( size_t i=0; i<nonTags.size(); i++ ) {
    for( size_t j=i+1; j<nonTags.size(); j++ ) {
      double deltaR_ = reco::deltaR<double>( nonTags.at(i)->eta(), nonTags.at(i)->phi(),
      						nonTags.at(j)->eta(), nonTags.at(j)->phi() );
      if( deltaR_ < minDeltaR ) {
        minDrPair_.clear();
        minDeltaR = deltaR_ ;
	minDrPair_.push_back( nonTags.at(i)  );
	minDrPair_.push_back( nonTags.at(j)  );
      }
    }  // end for j
  }  // end for i

  fillLooseTops();
  fillTightTops();
  fillType3Tops();

  passCut( ret, "Inclusive" );

  if( ignoreCut( "Jet Preselection" ) || pfJets_.size() >= 2 )  {
    passCut( ret, "Jet Preselection" );
    if( ignoreCut(">= 1 WJet") || hasWJets() )  {
      passCut( ret, ">= 1 WJet" );

      if( ignoreCut(">= 1 bJet") || hasBJets() ) {
        passCut( ret, ">= 1 bJet" );
      }  // end >= 1 bjet
    }  // end >= 1 wjet
  } // pass jet preselection

  return (bool)ret;
}


void WPlusBJetEventSelector::fillLooseTops()
{
  //cout<<"Begin WPlusBJetEventSelector::fillLooseTops"<<endl;
  for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=allJets_.begin(), jetEnd=allJets_.end(),
    ijet=jetBegin; ijet != jetEnd; ijet++ )
  {
    pat::Jet const & jet0 = **ijet;
    //ijet as a W jet
    if( jet0.pt() > 200 )   {
      pat::Jet const * jet1 = NULL;
      double weight = 1.0, wRate;
      int wBin;
      wBin = wMistag_         ->  FindBin( jet0.pt() );
      wRate = wMistag_        ->  GetBinContent( wBin );
      weight = wRate;
      double minDeltaR = 9999.0;
      //All the other jets are not b-tagged, find the closest jet to W jet
      for( vector<edm::Ptr<pat::Jet> >::const_iterator ijet1 = jetBegin; ijet1 != jetEnd; ijet1++ ) {
        if( ijet1 == ijet )   continue;
        int bBin;
        double bRate;
        bBin  = bMistag_    ->  FindBin( (**ijet1).pt() );
        bRate = bMistag_    ->  GetBinContent( bBin );
        weight *= (1.0-bRate);
        double dR = reco::deltaR<double>( jet0.eta(), jet0.phi(), (**ijet1).eta(), (**ijet1).phi() );
        if( dR < minDeltaR )  {
          minDeltaR = dR;
          jet1 = &(**ijet1);
        }  // if dR 
      }  // end for ijet1

      for( vector<edm::Ptr<pat::Jet> >::const_iterator ijet2 = jetBegin; ijet2 != ijet; ijet2++ ) {
        int theBin;
        double theWeight;
        theBin = wMistag_       ->  FindBin( (**ijet2).pt() );
        theWeight = wMistag_    ->  GetBinContent( theBin );
        weight *= (1.0-theWeight);
      }  // end ijet2

      if( jet1 )  {
        Type2L  fakeTop (jet0.p4(), jet1->p4(), weight);
        looseTops_.push_back( fakeTop );
      } // end if jet1
    } // end jet0.pt()
  }  //for ijet
  //cout<<"End WPlusBJetEventSelector::fillLooseTops"<<endl;
}

void WPlusBJetEventSelector::fillTightTops()
{
  //cout<<"Begin WPlusBJetEventSelector::fillTightTops"<<endl;
  for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=allJets_.begin(), jetEnd=allJets_.end(),
    ijet=jetBegin; ijet != jetEnd; ijet++ )
  {
    for( vector<edm::Ptr<pat::Jet> >::const_iterator ijet1 = jetBegin; ijet1 != jetEnd; ijet1++ ) {
      if( ijet1 == ijet )   continue;
      pat::Jet const & jet0 = **ijet;
      pat::Jet const & jet1 = **ijet1;
      int wBin, bBin;
      double wRate, bRate, weight;
      //jet0 as W jet, jet1 as b jet
      if( jet0.pt() > 200 ) {
        wBin  = wMistag_    ->  FindBin( jet0.pt() );
        bBin  = bMistag_    ->  FindBin( jet1.pt() );
        wRate = wMistag_    ->  GetBinContent( wBin );
        bRate = bMistag_    ->  GetBinContent( bBin );
        weight  = wRate*bRate;
        for( vector<edm::Ptr<pat::Jet> >::const_iterator nowtag = jetBegin; nowtag != ijet; nowtag++ ) {
          int theBin;
          double theWeight;
          theBin = wMistag_       ->  FindBin( (**nowtag).pt() );
          theWeight = wMistag_    ->  GetBinContent( theBin );
          weight *= (1.0-theWeight);
        } //end nowtag
        for( vector<edm::Ptr<pat::Jet> >::const_iterator nobtag = jetBegin; nobtag != ijet1; nobtag++ ) {
          int theBin;
          double theWeight;
          theBin    = bMistag_    ->  FindBin( (**nobtag).pt() );
          theWeight = bMistag_    ->  GetBinContent( theBin );
          weight *= (1.0-theWeight);
        }  // end nobtag
        Type2T  fakeTop (jet0.p4(), jet1.p4(), weight);
        tightTops_.push_back( fakeTop );
      }
    }  // for ijet1
  }  //for ijet
  //cout<<"End WPlusBJetEventSelector::fillTightTops"<<endl;
}

void WPlusBJetEventSelector::fillType3Tops()
{
  //cout<<"Begin WPlusBJetEventSelector::fillType3Tops"<<endl;
  //cout<<"allJets size "<<allJets_.size()<<endl;
  for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=allJets_.begin(), jetEnd=allJets_.end(),
    ijet=jetBegin; ijet != jetEnd; ijet++ )
  {
    pat::Jet const & jet0 = **ijet;
    pat::Jet const * jet1 = NULL;
    pat::Jet const * jet2 = NULL;
    //jet0 as b jet,
    int bBin;
    double bRate, weight;
    bBin  = bMistag_    ->  FindBin( jet0.pt() );
    bRate = bMistag_    ->  GetBinContent( bBin );
    weight = bRate;
    //jet1, jet2 as minPair jets
    double minDeltaR = 9999.0;
    for( vector<edm::Ptr<pat::Jet> >::const_iterator ijet1 = jetBegin; ijet1 != jetEnd; ijet1++ ) {
      for( vector<edm::Ptr<pat::Jet> >::const_iterator ijet2 = ijet1+1; ijet2 != jetEnd; ijet2++ ) {
        if( ijet1 == ijet || ijet2 == ijet )  continue;
        double dR = reco::deltaR<double>( (**ijet1).eta(), (**ijet1).phi(), (**ijet2).eta(), (**ijet2).phi() );
        if( dR < minDeltaR ) {
          jet1 = &(**ijet1);
          jet2 = &(**ijet2);
          minDeltaR = dR;
        } // end if dR
      }  // for ijet2
    }  // for ijet1

    //Just follow our selection, where we only take the btag jet with greatest pt
    for( vector<edm::Ptr<pat::Jet> >::const_iterator nobtag=allJets_.begin(); nobtag != ijet ; nobtag ++ )  {
      int theBin;
      double theWeight;
      theBin = bMistag_   ->  FindBin( (**nobtag).pt() );
      theWeight = bMistag_->  GetBinContent( theBin );
      weight *= (1.0-theWeight);
    }  // end nobtag
    if( jet1 && jet2 ) {
      Type3  fakeTop (jet0.p4(), jet1->p4(), jet2->p4(), weight);
      type3Tops_.push_back( fakeTop );
    }
  }  //for ijet
  //cout<<"End WPlusBJetEventSelector::fillType3Tops"<<endl;
}
