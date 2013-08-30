#include "Analysis/JetAnalysis/interface/Dijet22Analyzer.h"


#include <vector>
#include <iterator>

Dijet22Analyzer::Dijet22Analyzer(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  dijet22Selector_( iConfig.getParameter<edm::ParameterSet>("simpleSubstructureSelector") ),
  theDir(iDir),
  dir2( theDir.mkdir("Kin") ),
  dir3( theDir.mkdir("muCut1") ),
  dir4( theDir.mkdir("muCut2") ),
  dir5( theDir.mkdir("mCut1") ),
  dir6( theDir.mkdir("mCut2") )
{

  std::vector<TFileDirectory*> tempDirs;
  tempDirs.push_back( &dir2 );
  tempDirs.push_back( &dir3 );
  tempDirs.push_back( &dir4 );
  tempDirs.push_back( &dir5 );
  tempDirs.push_back( &dir6 );

  for ( std::vector<TFileDirectory*>::const_iterator idir = tempDirs.begin(),
	  dirEnd = tempDirs.end(); idir != dirEnd; ++idir ) {
    TFileDirectory & dir = **idir;
    
    dir.make<TH1F>("jet0Pt",   "Jet 0 Pt",         100, 0, 5000);
    dir.make<TH1F>("jet0Eta",  "Jet 0 Rapidity",   100, -3.0, 3.0 );
    dir.make<TH1F>("jet0Phi",  "Jet 0 Phi",        100, -TMath::Pi(), TMath::Pi());
    dir.make<TH1F>("jet0Mass", "Jet 0 Mass",       100, 0, 1000);
    dir.make<TH1F>("jet0Mu",   "Jet 0 Mass Drop",  100, 0, 1.0 );
    dir.make<TH1F>("jet1Pt",   "Jet 1 Pt",         100, 0, 5000);
    dir.make<TH1F>("jet1Eta",  "Jet 1 Rapidity",   100, -3.0, 3.0 );
    dir.make<TH1F>("jet1Phi",  "Jet 1 Phi",        100, -TMath::Pi(), TMath::Pi());
    dir.make<TH1F>("jet1Mass", "Jet 1 Mass",       100, 0, 1000);
    dir.make<TH1F>("jet1Mu",   "Jet 1 Mass Drop",  100, 0, 1.0 );
    dir.make<TH1F>("dijetMass","Dijet Mass", 700, 0, 7000 );
    dir.make<TH1F>("deltaPhi", "Dijet #Delta #phi", 100, 0, TMath::Pi() );
  }

}




///////////////////
/// The event loop
//////////////////
void Dijet22Analyzer::analyze(const edm::EventBase& event)
{

  pat::strbitset ret = dijet22Selector_.getBitTemplate();
  dijet22Selector_( event, ret );
  
  // Now fill the histograms hierarchically,
  // to watch the magic unfold!
  if ( ret[dijet22Selector_.indexKin_] ) {
    fillHistos( dir2 );
    if ( ret[dijet22Selector_.indexMu1_] ) {    
      fillHistos( dir3 );
      if ( ret[dijet22Selector_.indexMu2_] ) {
	fillHistos( dir4 );
	if ( ret[dijet22Selector_.indexM1_] ) {
	  fillHistos( dir5 );
	  if ( ret[dijet22Selector_.indexM2_] ) {
	    fillHistos( dir6 );
	  }
	}
      }
    }
  }
}


void Dijet22Analyzer::endJob()
{
  dijet22Selector_.print(std::cout);
}


void Dijet22Analyzer::fillHistos( TFileDirectory & dir )
{
  dir.getObject<TH1>( "jet0Pt"   )->Fill(  dijet22Selector_.jetP4(0).Pt()        );
  dir.getObject<TH1>( "jet0Eta"  )->Fill(  dijet22Selector_.jetP4(0).Rapidity()  );
  dir.getObject<TH1>( "jet0Phi"  )->Fill(  dijet22Selector_.jetP4(0).Phi()       );
  dir.getObject<TH1>( "jet0Mass" )->Fill(  dijet22Selector_.jetP4(0).M()         );
  dir.getObject<TH1>( "jet0Mu"   )->Fill(  dijet22Selector_.jetMu(0)             );
  dir.getObject<TH1>( "jet1Pt"   )->Fill(  dijet22Selector_.jetP4(1).Pt()        );
  dir.getObject<TH1>( "jet1Eta"  )->Fill(  dijet22Selector_.jetP4(1).Rapidity()  );
  dir.getObject<TH1>( "jet1Phi"  )->Fill(  dijet22Selector_.jetP4(1).Phi()       );
  dir.getObject<TH1>( "jet1Mass" )->Fill(  dijet22Selector_.jetP4(1).M()         );
  dir.getObject<TH1>( "jet1Mu"   )->Fill(  dijet22Selector_.jetMu(1)             );
  dir.getObject<TH1>( "dijetMass")->Fill(  dijet22Selector_.jjCand().M()         );
  dir.getObject<TH1>( "deltaPhi" )->Fill(  reco::deltaPhi<double>(dijet22Selector_.jetP4(0).phi(),
								  dijet22Selector_.jetP4(1).phi() )); 
}
