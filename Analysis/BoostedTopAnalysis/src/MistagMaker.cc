#include "Analysis/BoostedTopAnalysis/interface/MistagMaker.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


MistagMaker::MistagMaker( edm::ParameterSet const & params, TFileDirectory & iDir ) :
  dijetSelector_         (params.getParameter<edm::ParameterSet>("jetIDParams"), 
                          params.getParameter<edm::ParameterSet>("pfJetIDParams"),
                          params.getParameter<edm::ParameterSet>("dijetSelectorParams")),
  theDir(iDir),
  boostedTopWTagFunctor_  (params.getParameter<edm::ParameterSet>("boostedTopWTagParams") ),
  wMassMin_               (params.getParameter<double>("wMassMin") ),
  wMassMax_               (params.getParameter<double>("wMassMax") ),
  ptMin_               (params.getParameter<double>("jetPtMin") )
{
  histograms1d["wTagPt"]      =   theDir.make<TH1F>("wTagPt",   "W Jet Pt",   400,    0,    2000 );
  histograms1d["probePt"]     =   theDir.make<TH1F>("probePt",  "W Jet Pt",   400,    0,    2000 );
  histograms1d["nJets"]       =   theDir.make<TH1F>("nJets",    "N Jets",     20,     0,    20  );
  histograms1d["topProbePt"]  =   theDir.make<TH1F>("topProbePt", "Top Probe Pt", 400,  0,  2000 );
  histograms1d["topTagPt"]    =   theDir.make<TH1F>("topTagPt", "Top Tag Pt", 400,    0,    2000 );

  histograms1d["PassNminusOne_ProbePt"]  =   theDir.make<TH1F>("PassNminusOne_ProbePt", "PassNminusOne_ProbePt", 400,  0,  2000 );
  histograms1d["PassNminusOne_TagPt"]    =   theDir.make<TH1F>("PassNminusOne_TagPt", "PassNminusOne_TagPt", 400,  0,  2000 );

  histograms1d["wTagPt"]        ->  Sumw2();
  histograms1d["probePt"]       ->  Sumw2();
  histograms1d["topProbePt"]    ->  Sumw2();
  histograms1d["topTagPt"]      ->  Sumw2();

  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "Module requires the RandomNumberGeneratorService\n";
  }

  CLHEP::HepRandomEngine& engine = rng->getEngine();
  flatDistribution_ = new CLHEP::RandFlat(engine, 0., 1.);


}

void MistagMaker::analyze( const edm::EventBase & iEvent )
{

  double weight = 1.0;
  edm::Handle<GenEventInfoProduct>    genEvt;
  iEvent.getByLabel( edm::InputTag("generator"),  genEvt );
  if( genEvt.isValid() )  {
    weight = genEvt->weight() ;
  }

  pat::strbitset jetRet = dijetSelector_.getBitTemplate();
  bool passDijet = dijetSelector_( iEvent, jetRet );

  std::vector<edm::Ptr<pat::Jet> > const & pretaggedJets = dijetSelector_.pfJets()  ;
  histograms1d["nJets"]       ->      Fill( pretaggedJets.size() );
  if( pretaggedJets.size() == 2 ) {
    bool dijet = pretaggedJets[0]->pt() > ptMin_  && pretaggedJets[1]->pt() > ptMin_;
    if( dijet ) {
      double x = flatDistribution_->fire();
      int index = x < 0.5 ? 0 : 1;
      const pat::Jet & probe( *pretaggedJets[index] );

      pat::strbitset wRet = boostedTopWTagFunctor_.getBitTemplate();
      bool pass = boostedTopWTagFunctor_ ( probe, wRet );
      bool passMass = probe.mass() > wMassMin_ && probe.mass() < wMassMax_ ;
      histograms1d["probePt"]     ->  Fill( probe.pt() , weight );
      if( pass && passMass )
        histograms1d["wTagPt"]    ->  Fill( probe.pt(), weight );
    }
  }

  //CATopTag mistag
  edm::Handle<std::vector<pat::Jet> >     caTopJetsHandle ;
  iEvent.getByLabel( edm::InputTag("goodPatJetsCATopTagPF"),  caTopJetsHandle );
  std::vector<const pat::Jet * >      caTopJets;

  for( std::vector<pat::Jet>::const_iterator ijet=caTopJetsHandle->begin(), jetEnd=caTopJetsHandle->end();
  ijet != jetEnd; ijet++ )
  {
    if( ijet->pt() > 350 && fabs( ijet->eta() ) < 2.4 ) {
      caTopJets.push_back( &(*ijet) ) ;
    }
  }
	
  if( caTopJets.size() == 2 ) {
    pat::Jet const & jet0 = *(caTopJets.at(0)) ;
    pat::Jet const & jet1 = *(caTopJets.at(1)) ;
	
	double minMass0 = 0.0,  minMass1 = 0.0 ;
    std::vector<const reco::Candidate *>  subjets0 = jet0.getJetConstituentsQuick();
    std::vector<const reco::Candidate *>  subjets1 = jet1.getJetConstituentsQuick();

    if( subjets0.size() >= 3 )  {
      math::XYZTLorentzVector pairwiseMass01;
      math::XYZTLorentzVector pairwiseMass02;
      math::XYZTLorentzVector pairwiseMass12;

      pairwiseMass01 = subjets0.at(0)->p4() + subjets0.at(1)->p4();
      pairwiseMass02 = subjets0.at(0)->p4() + subjets0.at(2)->p4();
      pairwiseMass12 = subjets0.at(1)->p4() + subjets0.at(2)->p4();
      minMass0 = std::min( pairwiseMass01.mass(), pairwiseMass02.mass() );
      minMass0 = std::min( minMass0, pairwiseMass12.mass() );
    }

    if( subjets1.size() >= 3 )  {
      math::XYZTLorentzVector pairwiseMass01;
      math::XYZTLorentzVector pairwiseMass02;
      math::XYZTLorentzVector pairwiseMass12;

      pairwiseMass01 = subjets1.at(0)->p4() + subjets1.at(1)->p4();
      pairwiseMass02 = subjets1.at(0)->p4() + subjets1.at(2)->p4();
      pairwiseMass12 = subjets1.at(1)->p4() + subjets1.at(2)->p4();
      minMass1 = std::min( pairwiseMass01.mass(), pairwiseMass02.mass() );
      minMass1 = std::min( minMass1, pairwiseMass12.mass() );
    }

    bool tagged0 = minMass0 > 50 && jet0.mass() > 140 && jet0.mass() < 250 ;
    bool tagged1 = minMass1 > 50 && jet1.mass() > 140 && jet1.mass() < 250 ;
    
	bool m0 = minMass0 < 50 && jet0.mass() > 140 && jet0.mass() < 250 ;
	bool m1 = minMass0 < 50 && jet0.mass() > 140 && jet0.mass() < 250 ;

    double x = flatDistribution_->fire();
    if( x < 0.5 ) {
      if( !tagged0 )  {
        histograms1d["topProbePt"]      ->    Fill( jet1.pt() );
        if( tagged1 )   histograms1d["topTagPt"]    ->    Fill( jet1.pt() );
      }
	  if ( m0 ) {
		histograms1d["PassNminusOne_ProbePt"]      ->    Fill( jet1.pt() );
		if( tagged1 )   histograms1d["PassNminusOne_TagPt"]    ->    Fill( jet1.pt() );
	  }
    } else {
      if( !tagged1 )  {
        histograms1d["topProbePt"]    ->      Fill( jet0.pt() );
        if( tagged0 )   histograms1d["topTagPt"]    ->    Fill( jet0.pt() );
      }
	  if ( m1 ) {
        histograms1d["PassNminusOne_ProbePt"]      ->    Fill( jet0.pt() );
        if( tagged1 )   histograms1d["PassNminusOne_TagPt"]    ->    Fill( jet0.pt() );
      }
    }
  }

}

