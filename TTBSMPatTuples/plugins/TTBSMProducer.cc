// -*- C++ -*-
//
// Package:    TTBSMProducer
// Class:      TTBSMProducer
// 
/**\class TTBSMProducer TTBSMProducer.cc Analysis/TTBSMProducer/src/TTBSMProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Salvatore Rappoccio"
//         Created:  Mon Jan 17 21:44:07 CST 2011
// $Id: TTBSMProducer.cc,v 1.13 2012/02/03 17:11:54 guofan Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/SubjetHelper.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>

#include "Nsubjettiness.hh"
#include "Njettiness.hh"

using namespace fastjet;
using namespace std;

namespace LHAPDF {
      void initPDFSet(int nset, const std::string& filename, int member=0);
      int numberPDF(int nset);
      void usePDFMember(int nset, int member);
      double xfx(int nset, double x, double Q, int fl);
      double getXmin(int nset, int member);
      double getXmax(int nset, int member);
      double getQ2min(int nset, int member);
      double getQ2max(int nset, int member);
      void extrapolate(bool extrapolate=true);
}

//
// class declaration
//

class TTBSMProducer : public edm::EDFilter {
   public:
      explicit TTBSMProducer(const edm::ParameterSet&);
      float smear_factor_lookup(float eta, float error);
     ~TTBSMProducer();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
  edm::InputTag             wTagSrc_; 
  edm::InputTag             topTagSrc_;
  edm::InputTag             trigSrc_;
  edm::InputTag   rhoSrc_;          /// mean pt per unit area
  edm::InputTag   pvSrc_;           /// primary vertex
  edm::InputTag   genJetsSrc_;
  std::vector<std::string>  trigs_;
  std::string               topTagName_;
  CATopTagFunctor           topTagFunctor_;
  BoostedTopWTagFunctor     wTagFunctor_;
  bool                      readTrig_;
  double                    scale_;
  double                    smear_;
  double                    etaSmear_;


  std::vector<std::string>  jecPayloads_; /// files for JEC payloads
  std::string               pdfSet_; /// lhapdf string

  boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
  boost::shared_ptr<FactorizedJetCorrector> jec_;



};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
TTBSMProducer::TTBSMProducer(const edm::ParameterSet& iConfig) :
  wTagSrc_      (iConfig.getParameter<edm::InputTag>("wTagSrc") ),
  topTagSrc_    (iConfig.getParameter<edm::InputTag>("topTagSrc") ),
  trigSrc_      (iConfig.getParameter<edm::InputTag>("trigSrc") ),
  rhoSrc_       ( iConfig.getParameter<edm::InputTag>("rhoSrc")),
  pvSrc_        ( iConfig.getParameter<edm::InputTag>("pvSrc")),
  trigs_        (iConfig.getParameter<std::vector<std::string> > ("trigs") ),
  topTagName_   (iConfig.getParameter<edm::ParameterSet>("topTagParams").getParameter<std::string>("tagName") ),
  topTagFunctor_(iConfig.getParameter<edm::ParameterSet>("topTagParams")),
  wTagFunctor_  (iConfig.getParameter<edm::ParameterSet>("wTagParams")),
  readTrig_     (iConfig.getParameter<bool>("readTrig")),
  scale_        (iConfig.getParameter<double>( "jetScale" ) ),
  smear_        (iConfig.getParameter<double>( "jetPtSmear") ),
  etaSmear_     (iConfig.getParameter<double>( "jetEtaSmear") ),
  jecPayloads_  (iConfig.getParameter<std::vector<std::string> >  ("jecPayloads")),
  pdfSet_       (iConfig.getParameter<std::string> ("pdfSet") ),
  genJetsSrc_   (iConfig.getParameter<edm::InputTag>("genJetsSrc"))
{

  // Get the factorized jet corrector. 
  // The payloads contain N elements, 
  // the Nth is the uncertainty, and the first N-1 elements are the
  // actual correction levels. 
  std::vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator ipayload = jecPayloads_.begin(),
	  ipayloadEnd = jecPayloads_.end(); ipayload != ipayloadEnd - 1; ++ipayload ) {
    std::cout << "Adding payload " << *ipayload << std::endl;
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }

  jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );

  jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecPayloads_.back()));

  std::cout << "here" << std::endl;

  //register your products
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("wTagP4");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagP4");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("ca8JetP4"); 
  produces<std::vector<double> > ("topTagBDisc");
  produces<std::vector<int> >    ("topTagPartonFlavour");
  produces<std::vector<double> > ("wTagBDisc");
  produces<std::vector<int> >    ("wTagPartonFlavour");
  produces<std::vector<double> > ("wTagMu");
  produces<std::vector<int> >    ("wTagPass");
  produces<std::vector<double> > ("topTagMinMass");
  produces<std::vector<double> > ("topTagTopMass");
  produces<std::vector<double> > ("topTagNSubjets");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagSubjet1P4");
  produces<std::vector<int> > ("topTagSubjetJetID");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagSubjet2P4");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagSubjet3P4");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagSubjet4P4");
  produces<std::vector<double> > ("topTagSubjetBDisc");
  produces<std::vector<int> >    ("topTagPass");
  produces<std::vector<int> >    ("prescales");
  produces<std::vector<int> >    ("trigIndex");
  produces<std::vector<int> >    ("myTrigIndex");
  produces<std::vector<std::string> > ("trigNames");
  produces<double> ("rho");
  produces<double> ("weight");
  produces<unsigned int>    ("npv");
  produces<int>    ("npvTrue");
  produces<double>   ("mttgen");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("pfMET");

  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("wTagP4Hemis0");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("wTagP4Hemis1");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagP4Hemis0");
  produces<std::vector<reco::Candidate::PolarLorentzVector> > ("topTagP4Hemis1");
  produces<std::vector<double> > ("wTagBDiscHemis0");
  produces<std::vector<int> >    ("wTagPartonFlavour0");
  produces<std::vector<double> > ("wTagMuHemis0");
  produces<std::vector<double> > ("topTagMinMassHemis0");
  produces<std::vector<int> >    ("topTagPartonFlavour0");
  produces<std::vector<double> > ("topTagTopMassHemis0");
  produces<std::vector<double> > ("topTagNSubjetsHemis0");
  produces<std::vector<int> >    ("topTagPassHemis0");
  produces<std::vector<double> > ("wTagBDiscHemis1");
  produces<std::vector<int> >    ("wTagPartonFlavour1");
  produces<std::vector<double> > ("wTagMuHemis1");
  produces<std::vector<double> > ("topTagMinMassHemis1");
  produces<std::vector<int> >    ("topTagPartonFlavour1");
  produces<std::vector<double> > ("topTagTopMassHemis1");
  produces<std::vector<double> > ("topTagNSubjetsHemis1");
  produces<std::vector<int> >    ("topTagPassHemis1");
  produces<int>                  ("jet3Hemis0");
  produces<int>                  ("jet3Hemis1");
  produces<std::vector<double> > ("pdfWeights");

  produces<std::vector<double> > ("tau1");
  produces<std::vector<double> > ("tau2");
  produces<std::vector<double> > ("tau3");
  produces<std::vector<double> > ("tau4");
  produces<std::vector<double> > ("onepasstau1");
  produces<std::vector<double> > ("onepasstau2");
  produces<std::vector<double> > ("onepasstau3");
  produces<std::vector<double> > ("onepasstau4");

  produces<std::vector<double> > ("topTagCHE");
  produces<std::vector<double> > ("topTagNE");
  produces<std::vector<int> > ("topTagNumPF");
  produces<std::vector<int> > ("topTagNumCH");
  produces<std::vector<int> > ("topTagNumNE");


  if ( pdfSet_ != "" )
    LHAPDF::initPDFSet(1, pdfSet_.c_str());
}


TTBSMProducer::~TTBSMProducer()
{
}


//
// member functions
//

// ------------ method called to produce the data  ------------
bool
TTBSMProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  
  edm::Handle< std::vector<reco::Vertex> > h_pv;
  iEvent.getByLabel( pvSrc_, h_pv );

  typedef std::vector<reco::Candidate::PolarLorentzVector> p4_vector;
  typedef reco::Candidate::PolarLorentzVector LorentzV;

  std::auto_ptr<p4_vector> wTagP4( new p4_vector() );
  std::auto_ptr<p4_vector> topTagP4( new p4_vector() );
  std::auto_ptr<p4_vector> ca8JetP4( new p4_vector() );
  std::auto_ptr<std::vector<double> > wTagBDisc ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagBDisc ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > wTagMu ( new std::vector<double>() );
  std::auto_ptr<std::vector<int> >    wTagPass ( new std::vector<int>() );
  std::auto_ptr<std::vector<double> > topTagMinMass ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagTopMass ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagNSubjets ( new std::vector<double>() );
  std::auto_ptr<p4_vector> topTagSubjet1P4( new p4_vector() );
  std::auto_ptr<std::vector<int> > topTagSubjetJetID( new std::vector<int>() );
  std::auto_ptr<p4_vector> topTagSubjet2P4( new p4_vector() );
  std::auto_ptr<p4_vector> topTagSubjet3P4( new p4_vector() );
  std::auto_ptr<p4_vector> topTagSubjet4P4( new p4_vector() );
  std::auto_ptr<std::vector<double> > topTagSubjetBDisc( new std::vector<double>() );
  std::auto_ptr<std::vector<int> >    topTagPass ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >    prescales ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >    trigIndex ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >    myTrigIndex ( new std::vector<int>() );
  std::auto_ptr<std::vector<std::string> >    trigNames ( new std::vector<std::string>() );
  std::auto_ptr<double>               rho( new double(-1.0) );
  std::auto_ptr<double>               weight( new double(1.0) );
  std::auto_ptr<double>               mttgen( new double(1.0) );
  std::auto_ptr<p4_vector>    pfMET( new p4_vector() );
  std::auto_ptr<std::vector<double> > topTagCHE( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagNE( new std::vector<double>() );
  std::auto_ptr<std::vector<int> > topTagNumPF( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > topTagNumCH( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > topTagNumNE( new std::vector<int>() );
  

  //The duplicate quantities by hemisphere
  std::auto_ptr<p4_vector> topTagP4Hemis0 ( new p4_vector() );
  std::auto_ptr<p4_vector> topTagP4Hemis1 ( new p4_vector() );
  std::auto_ptr<p4_vector> wTagP4Hemis0 ( new p4_vector() );
  std::auto_ptr<p4_vector> wTagP4Hemis1 ( new p4_vector() );
  std::auto_ptr<int> jet3Hemis0   ( new int(-1) );
  std::auto_ptr<int> jet3Hemis1   ( new int(-1) );
  std::auto_ptr<std::vector<double> > wTagBDiscHemis0( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > wTagBDiscHemis1( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > wTagMuHemis0( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > wTagMuHemis1( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagMinMassHemis0 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagMinMassHemis1 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagTopMassHemis0 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagTopMassHemis1 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagNSubjetsHemis0( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > topTagNSubjetsHemis1( new std::vector<double>() );
  std::auto_ptr<std::vector<int> > topTagPassHemis0  ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > topTagPassHemis1  ( new std::vector<int>() );
  
  std::auto_ptr<unsigned int> npv( new unsigned int() );
  std::auto_ptr<int> npvTrue( new int() );
  std::auto_ptr<std::vector<double> > pdf_weights( new std::vector<double>() );

  std::auto_ptr<std::vector<int> > topTagPartonFlavour ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > wTagPartonFlavour ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > topTagPartonFlavour0 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > topTagPartonFlavour1 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > wTagPartonFlavour0 ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > wTagPartonFlavour1 ( new std::vector<int>() );

  std::auto_ptr<std::vector<double> > tau1 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tau2 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tau3 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tau4 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > onepasstau1 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > onepasstau2 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > onepasstau3 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > onepasstau4 ( new std::vector<double>() );

  Nsubjettiness Nsub1(1, Njettiness::AxesMode::kt_axes, 1.0, 0.8);
  Nsubjettiness Nsub2(2, Njettiness::AxesMode::kt_axes, 1.0, 0.8);
  Nsubjettiness Nsub3(3, Njettiness::AxesMode::kt_axes, 1.0, 0.8);
  Nsubjettiness Nsub4(4, Njettiness::AxesMode::kt_axes, 1.0, 0.8);

  Nsubjettiness Nsubonepass1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
  Nsubjettiness Nsubonepass2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
  Nsubjettiness Nsubonepass3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
  Nsubjettiness Nsubonepass4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);

  // double Rtrim = 0.2;
  // double ptfrac = 0.05;
  // fastjet::Filter trimmer_CA2_PFRAC5(fastjet::JetDefinition(fastjet::cambridge_algorithm, Rtrim), fastjet::SelectorPtFractionMin(ptfrac) );
      
  // Number of reconstructed PV's
  *npv = h_pv->size();

  // Pileup reweighting in the MC
  *npvTrue = -1;
  if ( ! iEvent.isRealData() ) {
    edm::InputTag PileupSrc_ ("addPileupInfo");
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent.getByLabel(PileupSrc_, PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    // (then, for example, you can do)
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      if(BX == 0) { 
	*npvTrue = PVI->getTrueNumInteractions();
	break;
      }
    }
  }

  edm::Handle<std::vector<reco::GenParticle> >   h_gen;
  iEvent.getByLabel( "prunedGenParticles", h_gen );
  if(h_gen.isValid())  {
    std::vector<const reco::GenParticle *>  genTop;
    for ( std::vector<reco::GenParticle>::const_iterator  igen = h_gen->begin(),
          genEnd = h_gen->end(); igen != genEnd; ++igen ) {
        if (fabs(igen->pdgId()) == 6)
          genTop.push_back( &(*igen) );
    }
    if (genTop.size() == 2) {
        *mttgen = (genTop[0]->p4() + genTop[1]->p4()).M();
    }
  }



  edm::Handle<std::vector<pat::Jet> > h_wTag;
  edm::Handle<std::vector<pat::Jet> > h_topTag;
  edm::Handle<std::vector<pat::Jet> > h_ca8Jets;
  //edm::Handle<trigger::TriggerEvent>      h_trig;
  edm::Handle<std::vector<pat::MET> > h_met;
  edm::Handle<double> h_rho;

  iEvent.getByLabel( rhoSrc_, h_rho );
  double rhoVal = *h_rho;

  edm::Handle<std::vector<reco::GenJet> > h_genJets;

  if (!iEvent.isRealData()) iEvent.getByLabel( genJetsSrc_, h_genJets);



  iEvent.getByLabel( wTagSrc_, h_wTag );
  iEvent.getByLabel( topTagSrc_, h_topTag );
  iEvent.getByLabel( "patMETsPFlow", h_met );
  iEvent.getByLabel( "goodPatJetsCA8PF", h_ca8Jets);

  pat::strbitset wTagRet = wTagFunctor_.getBitTemplate();
  pat::strbitset topTagRet = topTagFunctor_.getBitTemplate();
  for ( std::vector<pat::MET>::const_iterator metBegin = h_met->begin(),
          metEnd = h_met->end(), imet = metBegin; imet != metEnd; ++imet )
  {
    pfMET->push_back(imet->polarP4());

  }

  for ( std::vector<pat::Jet>::const_iterator jetBegin = h_wTag->begin(), jetEnd = h_wTag->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) {


    // -------
    // Jet energy corrections
    // -------
    reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);
    jec_->setJetEta( uncorrJet.eta() );
    jec_->setJetPt ( uncorrJet.pt() );
    jec_->setJetE  ( uncorrJet.energy() );
    jec_->setJetA  ( ijet->jetArea() );
    jec_->setRho   ( rhoVal );
    jec_->setNPV   ( h_pv->size() );
    double corr = jec_->getCorrection();


    reco::GenJet theMatchingGenJet;


    float eta1 = uncorrJet.eta();
    float phi1 = uncorrJet.phi();
    float eta2, phi2, deltaR;


    //Find matching genJet for systematic smearing

    if (!iEvent.isRealData()){
    for ( std::vector<reco::GenJet>::const_iterator genJBegin = h_genJets->begin(),
	   genJEnd = h_genJets->end(), igenjet = genJBegin; igenjet != genJEnd; ++igenjet) {

	   eta2 = igenjet->eta();
          phi2 = igenjet->phi();


  	  deltaR = sqrt( (eta2-eta1)*(eta2-eta1) + (phi2-phi1)*(phi2-phi1) );




          if (deltaR < 0.1) {
		  theMatchingGenJet = (*igenjet);
	   }
    }
    }







    // -------
    // Jet energy scale variation
    // -------
    if ( fabs(scale_) > 0.0001 ) {
      // Now get the uncertainties
      jecUnc_->setJetEta( uncorrJet.eta() );
      jecUnc_->setJetPt( uncorrJet.pt() * corr );
      double unc = fabs(jecUnc_->getUncertainty( bool(scale_ > 0) ));

      // Add the "flat" flavor dependent corrections in quadrature
      unc = sqrt( unc*unc + scale_*scale_);
      double sign = 1.0;
      if ( scale_ < 0.0 ) sign = -1.0;
	     
      // Scale up or down by jetScale_
      double ijetscale = (1 + sign * unc);
      corr *= ijetscale;
    }    

    // -------
    // Jet angular resolution smearing
    // -------
    double etaScale_ = 1.0;
    if( fabs(etaSmear_) > 0.0 && theMatchingGenJet.pt() > 15.0 ) {
      double recoeta = ijet->eta();
      double geneta = theMatchingGenJet.eta();
      double deltaeta = (recoeta-geneta)*etaSmear_ ;
      etaScale_ = std::max((double)0.0,(recoeta+deltaeta)/recoeta);
    }


    double phiScale_ = 1.0;
    if( fabs(etaSmear_) > 0.0 && theMatchingGenJet.pt() > 15.0 ) {
      double recophi = ijet->phi();
      double genphi = theMatchingGenJet.phi();
      double deltaphi = (recophi-genphi)*etaSmear_ ;
      phiScale_ = std::max((double)0.0,(recophi+deltaphi)/recophi);
    }


    // -------
    // Jet energy resolution smearing
    // -------
    double ptSmear_ = 1.0;
    if( fabs(smear_) > -1.0 && theMatchingGenJet.pt() > 15.0 )  {
      double recopt = ijet->pt();
      double genpt = theMatchingGenJet.pt();
      float smear_factor = smear_factor_lookup(theMatchingGenJet.eta(), smear_);
      double deltapt = (recopt-genpt)*smear_factor;
      ptSmear_ = std::max((double)0.0, (recopt+deltapt)/recopt);
    }

    reco::Candidate::PolarLorentzVector corrJet (uncorrJet.pt(), uncorrJet.eta(), uncorrJet.phi(), uncorrJet.mass());
    corrJet *=  (corr * ptSmear_);
    corrJet.SetEta( corrJet.eta() * etaScale_ );
    corrJet.SetPhi( corrJet.phi() * phiScale_ );


    wTagP4->push_back( corrJet );
    reco::Candidate::PolarLorentzVector corrDa0 = ijet->daughter(0)->polarP4() * (corr * ptSmear_);
    reco::Candidate::PolarLorentzVector corrDa1 = ijet->daughter(1)->polarP4() * (corr * ptSmear_);

    corrDa0.SetEta( corrDa0.eta() * etaScale_ );
    corrDa1.SetEta( corrDa1.eta() * etaScale_ );

    corrDa0.SetPhi( corrDa0.phi() * phiScale_ );
    corrDa1.SetPhi( corrDa1.phi() * phiScale_ );


    bool passedWTag = wTagFunctor_(*ijet, wTagRet);
    double y = -1.0, mu = -1.0, dR = -1.0;
    pat::subjetHelper( corrDa0, corrDa1, y, mu, dR, corrJet.mass() );
    wTagMu->push_back( mu );
    wTagPass->push_back( passedWTag );
    wTagBDisc->push_back( ijet->bDiscriminator("combinedSecondaryVertexBJetTags") );
    wTagPartonFlavour->push_back( ijet->partonFlavour() );
  }
  
  for ( std::vector<pat::Jet>::const_iterator jetBegin = h_ca8Jets->begin(), jetEnd = h_ca8Jets->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) {
	  
	  
	  std::vector<fastjet::PseudoJet> FJparticles;
	  
	  

      for (unsigned i = 0; i < ijet->numberOfDaughters() ; i++){

	        const reco::PFCandidate* this_constituent = dynamic_cast<const reco::PFCandidate*>(ijet->daughter(i));

	        FJparticles.push_back( fastjet::PseudoJet( this_constituent->px(),
						   	   this_constituent->py(),
							   this_constituent->pz(),
							   this_constituent->energy() ) );


      }

	  fastjet::PseudoJet combJet = fastjet::join(FJparticles);
	  tau1->push_back(Nsub1.result(combJet));
	  tau2->push_back(Nsub2.result(combJet));
	  tau3->push_back(Nsub3.result(combJet));
	  tau4->push_back(Nsub4.result(combJet));
    onepasstau1->push_back(Nsubonepass1.result(combJet));
    onepasstau2->push_back(Nsubonepass2.result(combJet));
    onepasstau3->push_back(Nsubonepass3.result(combJet));
    onepasstau4->push_back(Nsubonepass4.result(combJet));


	  
    	  reco::Candidate::LorentzVector uncorrCA8jet = ijet->correctedP4(1);
          reco::Candidate::PolarLorentzVector corrCA8jet (uncorrCA8jet.pt(), uncorrCA8jet.eta(), uncorrCA8jet.phi(), uncorrCA8jet.mass());
      
	  
	  ca8JetP4->push_back(corrCA8jet);	  
	           
    //fastjet::PseudoJet trimmed_jet_CA2_PFRAC5 = trimmer_CA2_PFRAC5(combJet);

	  
  }
	  
	  
  
 int jetIndex = -1; 
  

  for ( std::vector<pat::Jet>::const_iterator jetBegin = h_topTag->begin(),
	  jetEnd = h_topTag->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) {



	jetIndex++;

    // -------
    // Jet energy corrections
    // -------
    reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);
    jec_->setJetEta( uncorrJet.eta() );
    jec_->setJetPt ( uncorrJet.pt() );
    jec_->setJetE  ( uncorrJet.energy() );
    jec_->setJetA  ( ijet->jetArea() );
    jec_->setRho   ( rhoVal );
    jec_->setNPV   ( h_pv->size() );
    double corr = jec_->getCorrection();
    double nSubjets = ijet->numberOfDaughters();

   


reco::GenJet theMatchingGenJet;

    float eta1 = uncorrJet.eta();
    float phi1 = uncorrJet.phi();
    float eta2, phi2, deltaR;

   if (!iEvent.isRealData()){
  //Find matching genJet for systematic smearing
   for ( std::vector<reco::GenJet>::const_iterator genJBegin = h_genJets->begin(),
	   genJEnd = h_genJets->end(), igenjet = genJBegin; igenjet != genJEnd; ++igenjet) {

	  eta2 = igenjet->eta();
          phi2 = igenjet->phi();


  	  deltaR = sqrt( (eta2-eta1)*(eta2-eta1) + (phi2-phi1)*(phi2-phi1) );




          if (deltaR < 0.1) {
		theMatchingGenJet = (*igenjet);
	  }

   }

}








    // -------
    // Jet energy scale variation
    // -------
    if ( fabs(scale_) > 0.0001 ) {
      // Now get the uncertainties
      jecUnc_->setJetEta( uncorrJet.eta() );
      jecUnc_->setJetPt( uncorrJet.pt() * corr );
      double unc = fabs(jecUnc_->getUncertainty( bool(scale_ > 0) ));

      // Add the "flat" flavor dependent corrections in quadrature
      unc = sqrt( unc*unc + scale_*scale_);
      double sign = 1.0;
      if ( scale_ < 0.0 ) sign = -1.0;
	     
      // Scale up or down by jetScale_
      double ijetscale = (1 + sign * unc);
      corr *= ijetscale;
    }    

    // -------
    // Jet angular resolution smearing
    // -------
    double etaScale_ = 1.0;
    if( fabs(etaSmear_) > 0.0 && theMatchingGenJet.pt() > 15.0 ) {
      double recoeta = ijet->eta();
      double geneta = theMatchingGenJet.eta();
      double deltaeta = (recoeta-geneta)*etaSmear_ ;
      etaScale_ = std::max((double)0.0,(recoeta+deltaeta)/recoeta);
    }


    double phiScale_ = 1.0;
    if( fabs(etaSmear_) > 0.0 && theMatchingGenJet.pt() > 15.0 ) {
      double recophi = ijet->phi();
      double genphi = theMatchingGenJet.phi();
      double deltaphi = (recophi-genphi)*etaSmear_ ;
      phiScale_ = std::max((double)0.0,(recophi+deltaphi)/recophi);
    }



    // -------
    // Jet energy resolution smearing
    // -------
    double ptSmear_ = 1.0;
    if( fabs(smear_) > -1.0 && theMatchingGenJet.pt() > 15.0 )  {
      double recopt = ijet->pt();
      double genpt = theMatchingGenJet.pt();
      float smear_factor = smear_factor_lookup(theMatchingGenJet.eta(), smear_);
      double deltapt = (recopt-genpt)*smear_factor;
      ptSmear_ = std::max((double)0.0, (recopt+deltapt)/recopt);
    }

    reco::Candidate::PolarLorentzVector corrJet (uncorrJet.pt(), uncorrJet.eta(), uncorrJet.phi(), uncorrJet.mass());
    corrJet *=  (corr * ptSmear_);
    corrJet.SetEta( corrJet.eta() * etaScale_ );
    corrJet.SetPhi( corrJet.phi() * phiScale_ );

    const reco::CATopJetTagInfo * catopTag = 
      dynamic_cast<reco::CATopJetTagInfo const *>(ijet->tagInfo(topTagName_));
    bool passedTopTag = topTagFunctor_( *ijet, topTagRet );

    topTagP4->push_back( corrJet );
    topTagPass->push_back( passedTopTag );
    topTagMinMass->push_back( catopTag->properties().minMass * (corr * ptSmear_) );
    topTagTopMass->push_back( catopTag->properties().topMass * (corr * ptSmear_) );
    topTagNSubjets->push_back( ijet->numberOfDaughters() );
    topTagBDisc->push_back( ijet->bDiscriminator("combinedSecondaryVertexBJetTags") );
    topTagPartonFlavour->push_back( ijet->partonFlavour() );
   
	const pat::Jet* topTagSubjets[4] = { NULL, NULL, NULL, NULL };


	topTagSubjets[0] = dynamic_cast<const pat::Jet*>((ijet->daughter(0)));
	if (nSubjets > 1) topTagSubjets[1] = dynamic_cast<const pat::Jet*>((ijet->daughter(1)));
	if (nSubjets > 2) topTagSubjets[2] = dynamic_cast<const pat::Jet*>((ijet->daughter(2)));
	if (nSubjets > 3) topTagSubjets[3] = dynamic_cast<const pat::Jet*>((ijet->daughter(3)));
 
for (int i = 0; i < nSubjets; i++ ) {
    
	const pat::Jet* this_subjet = dynamic_cast<const pat::Jet*>((ijet->daughter(i)));

	
	reco::Candidate::LorentzVector uncorrSubjet = this_subjet->correctedP4(0);
	reco::Candidate::PolarLorentzVector corrSubjet (uncorrSubjet.pt(), uncorrSubjet.eta(), uncorrSubjet.phi(), uncorrSubjet.mass());

	corrSubjet *= (corr*ptSmear_);

	double bDisc = this_subjet->bDiscriminator("combinedSecondaryVertexBJetTags");

	topTagSubjetBDisc->push_back(bDisc);

	topTagSubjet1P4->push_back(corrSubjet);

	topTagSubjetJetID->push_back(jetIndex);

}


 
    int numPF = 0;
    int numCH = 0;
    int numNE = 0;
    float chE = 0.0;
    float neutE = 0.0;
/*
    for (unsigned isub = 0; isub < ijet->numberOfDaughters(); isub++){

	const pat::Jet* this_sub = dynamic_cast<const pat::Jet*>((ijet->daughter(isub)));	
    	chE += this_sub->chargedHadronEnergy();
	chE += this_sub->chargedEmEnergy();
	neutE += this_sub->neutralHadronEnergy();
	neutE += this_sub->neutralEmEnergy();
	numPF += this_sub->nConstituents();
	numCH += this_sub->chargedMultiplicity();
	numNE += this_sub->neutralMultiplicity();	




    }
*/
    topTagCHE->push_back(chE);// ijet->chargedHadronEnergy() + ijet->chargedEmEnergy() );
    topTagNE->push_back(neutE);// ijet->neutralHadronEnergy() + ijet->neutralEmEnergy() );
    topTagNumPF->push_back(numPF);// ijet->numberOfDaughters() );
    topTagNumCH->push_back(numCH);
    topTagNumNE->push_back(numNE);

  }

  // For real data, get trigger path
  if ( readTrig_ ) {
/*    iEvent.getByLabel( trigSrc_,h_trig );
    for ( std::vector<std::string>::const_iterator itrigBegin = trigs_.begin(),
	    itrigEnd = trigs_.end(), itrig = itrigBegin;
	  itrig != itrigEnd; ++itrig ) {
      if ( h_trig->wasRun() && h_trig->wasAccept() && h_trig->paths() > 0 ) {
	int indexPath = h_trig->indexPath( *itrig );
	if ( indexPath > 0 ) {
	  pat::TriggerPath const * path = h_trig->path( *itrig );
	  if ( path != 0 && path->wasRun() && path->wasAccept() ) {
	    trigIndex->push_back( path->index() );
	    myTrigIndex->push_back( static_cast<int>(itrig - itrigBegin) );
	    prescales->push_back( path->prescale() );
	    trigNames->push_back( path->name() );
	  }
	}
      }
    }*/
  }

  //Make hemisphere
  if( topTagP4->size() > 0 ) {
    LorentzV  leadJet = topTagP4->at(0);
    for( size_t i=0; i<topTagP4->size(); i++ ) {
      double dPhi = fabs( reco::deltaPhi<double>( leadJet.phi(), topTagP4->at(i).phi() ) );
      if( dPhi < TMath::Pi()/2 )  {
        topTagP4Hemis0->push_back( topTagP4->at(i) );
        topTagMinMassHemis0->push_back( topTagMinMass->at(i) );
        topTagTopMassHemis0->push_back( topTagTopMass->at(i) );
        topTagNSubjetsHemis0->push_back( topTagNSubjets->at(i) );
        topTagPassHemis0->push_back( topTagPass->at(i) );
        topTagPartonFlavour0->push_back( topTagPartonFlavour->at(i) );
      }
      else  {
        topTagP4Hemis1->push_back( topTagP4->at(i) );
        topTagMinMassHemis1->push_back( topTagMinMass->at(i) );
        topTagTopMassHemis1->push_back( topTagTopMass->at(i) );
        topTagNSubjetsHemis1->push_back( topTagNSubjets->at(i) );
        topTagPassHemis1->push_back( topTagPass->at(i) );
        topTagPartonFlavour1->push_back( topTagPartonFlavour->at(i) );
      }
    }
    for( size_t i=0; i<wTagP4->size(); i++ )  {
      double dPhi = fabs( reco::deltaPhi<double>( leadJet.phi(), wTagP4->at(i).phi() )  );
      if( dPhi < TMath::Pi()/2 )  {
        wTagP4Hemis0->push_back( wTagP4->at(i) );
        wTagBDiscHemis0->push_back( wTagBDisc->at(i) );
        wTagMuHemis0->push_back( wTagMu->at(i) );
        wTagPartonFlavour0->push_back( wTagPartonFlavour->at(i) );
      }
      else  {
        wTagP4Hemis1->push_back( wTagP4->at(i) );
        wTagBDiscHemis1->push_back( wTagBDisc->at(i) );
        wTagMuHemis1->push_back( wTagMu->at(i) );
        wTagPartonFlavour1->push_back( wTagPartonFlavour->at(i) );
      }
    }
  }

  if( wTagP4Hemis0->size() > 0 )  {
    LorentzV   leadJetHemis0 = wTagP4Hemis0->at(0) ;
    double minDr = 99999. ;
    for( size_t i=1 ; i<wTagP4Hemis0->size() ; i++ )  {
      double deltaR = reco::deltaR<double>( leadJetHemis0.eta(), leadJetHemis0.phi(),
                                            wTagP4Hemis0->at(i).eta(), wTagP4Hemis0->at(i).phi() );
      if( deltaR < minDr )  {
        *jet3Hemis0 = i;
        minDr = deltaR;
      }
    }
  }
  if( wTagP4Hemis1->size() > 0 )  {
    LorentzV   leadJetHemis1 = wTagP4Hemis1->at(0) ;
    double minDr = 99999. ;
    for( size_t i=1 ; i<wTagP4Hemis1->size() ; i++ )  {
      double deltaR = reco::deltaR<double>( leadJetHemis1.eta(), leadJetHemis1.phi(),
                                            wTagP4Hemis1->at(i).eta(), wTagP4Hemis1->at(i).phi() );
      if( deltaR < minDr )  {
        *jet3Hemis1 = i;
        minDr = deltaR;
      }
    }
  }


  *rho = rhoVal ;


  edm::Handle<GenEventInfoProduct>    genEvt;
  iEvent.getByLabel( edm::InputTag("generator"),  genEvt );
  if( genEvt.isValid() )  {
    *weight = genEvt->weight() ;
  }

  if ( ! iEvent.isRealData() && pdfSet_ != "" ) {

    edm::Handle<GenEventInfoProduct> pdfstuff;
    if (iEvent.getByLabel("generator", pdfstuff)) {


      LHAPDF::usePDFMember(1,0);

      float q = pdfstuff->pdf()->scalePDF;
 
      int id1 = pdfstuff->pdf()->id.first;
      double x1 = pdfstuff->pdf()->x.first;
      // double pdf1 = pdfstuff->pdf()->xPDF.first;
      
      int id2 = pdfstuff->pdf()->id.second;
      double x2 = pdfstuff->pdf()->x.second;
      // double pdf2 = pdfstuff->pdf()->xPDF.second; 

      double xpdf1 = LHAPDF::xfx(1, x1, q, id1);
      double xpdf2 = LHAPDF::xfx(1, x2, q, id2);
      double w0 = xpdf1 * xpdf2;
      for(int i=1; i <=44; ++i){
	LHAPDF::usePDFMember(1,i);
	double xpdf1_new = LHAPDF::xfx(1, x1, q, id1);
	double xpdf2_new = LHAPDF::xfx(1, x2, q, id2);
	double weight = xpdf1_new * xpdf2_new / w0;
	pdf_weights->push_back(weight);
      }
    }
  }





 
 





  iEvent.put(wTagP4        ,"wTagP4");
  iEvent.put(topTagP4      ,"topTagP4");  
  iEvent.put(ca8JetP4      ,"ca8JetP4");
  iEvent.put(wTagBDisc     ,"wTagBDisc");
  iEvent.put(topTagBDisc   ,"topTagBDisc");
  iEvent.put(wTagMu        ,"wTagMu");
  iEvent.put(wTagPass      ,"wTagPass");
  iEvent.put(topTagMinMass ,"topTagMinMass");
  iEvent.put(topTagTopMass ,"topTagTopMass");
  iEvent.put(topTagNSubjets,"topTagNSubjets");
  iEvent.put(topTagSubjet1P4, "topTagSubjet1P4");
  iEvent.put(topTagSubjet2P4, "topTagSubjet2P4");
  iEvent.put(topTagSubjet3P4, "topTagSubjet3P4");
  iEvent.put(topTagSubjet4P4, "topTagSubjet4P4");
  iEvent.put(topTagSubjetBDisc, "topTagSubjetBDisc");
  iEvent.put(topTagSubjetJetID, "topTagSubjetJetID");
  iEvent.put(topTagPass    ,"topTagPass");    
  iEvent.put(prescales     ,"prescales");
  iEvent.put(trigIndex     ,"trigIndex");
  iEvent.put(myTrigIndex   ,"myTrigIndex");
  iEvent.put(trigNames     ,"trigNames");
  iEvent.put(wTagP4Hemis0        ,"wTagP4Hemis0");
  iEvent.put(topTagP4Hemis0      ,"topTagP4Hemis0");
  iEvent.put(wTagBDiscHemis0     ,"wTagBDiscHemis0");
  iEvent.put(wTagMuHemis0        ,"wTagMuHemis0");
  iEvent.put(topTagMinMassHemis0 ,"topTagMinMassHemis0");
  iEvent.put(topTagTopMassHemis0 ,"topTagTopMassHemis0");
  iEvent.put(topTagNSubjetsHemis0,"topTagNSubjetsHemis0");
  iEvent.put(topTagPassHemis0    ,"topTagPassHemis0");
  iEvent.put(jet3Hemis0,          "jet3Hemis0" );
  iEvent.put(wTagP4Hemis1        ,"wTagP4Hemis1");
  iEvent.put(topTagP4Hemis1      ,"topTagP4Hemis1");
  iEvent.put(wTagBDiscHemis1     ,"wTagBDiscHemis1");
  iEvent.put(wTagMuHemis1        ,"wTagMuHemis1");
  iEvent.put(topTagMinMassHemis1 ,"topTagMinMassHemis1");
  iEvent.put(topTagTopMassHemis1 ,"topTagTopMassHemis1");
  iEvent.put(topTagNSubjetsHemis1,"topTagNSubjetsHemis1");
  iEvent.put(topTagPassHemis1    ,"topTagPassHemis1");
  iEvent.put(jet3Hemis1,          "jet3Hemis1"  );
  iEvent.put( rho,                "rho" );
  iEvent.put( weight,             "weight");
  iEvent.put( npv,     "npv");
  iEvent.put( npvTrue, "npvTrue");
  iEvent.put( pdf_weights, "pdfWeights");
  iEvent.put( mttgen, "mttgen" );
  iEvent.put( pfMET, "pfMET" );
  iEvent.put(tau1, "tau1");
  iEvent.put(tau2, "tau2");
  iEvent.put(tau3, "tau3");
  iEvent.put(tau4, "tau4");
  iEvent.put(onepasstau1, "onepasstau1");
  iEvent.put(onepasstau2, "onepasstau2");
  iEvent.put(onepasstau3, "onepasstau3");
  iEvent.put(onepasstau4, "onepasstau4");
  iEvent.put(topTagCHE, "topTagCHE");
  iEvent.put(topTagNE, "topTagNE");
  iEvent.put(topTagNumPF, "topTagNumPF");
  iEvent.put(topTagNumCH, "topTagNumCH");
  iEvent.put(topTagNumNE, "topTagNumNE");

  iEvent.put( topTagPartonFlavour, "topTagPartonFlavour");
  iEvent.put( topTagPartonFlavour0, "topTagPartonFlavour0");
  iEvent.put( topTagPartonFlavour1, "topTagPartonFlavour1");
  iEvent.put( wTagPartonFlavour, "wTagPartonFlavour");
  iEvent.put( wTagPartonFlavour0, "wTagPartonFlavour0");
  iEvent.put( wTagPartonFlavour1, "wTagPartonFlavour1");

  return true;
}

float TTBSMProducer::smear_factor_lookup(float eta, float error)
{
	float abseta = abs(eta);
	float smear_nominal, smear_up, smear_down;
	float smear_factor;

	if (abseta <= 0.5) {
		smear_nominal = 0.052;
		smear_up = 0.115;
		smear_down = -0.01;
	}
	else if (abseta <= 1.1){
		smear_nominal = 0.057;
		smear_up = 0.114;
		smear_down = 0.001;
	}
	else if (abseta <= 1.7){
		smear_nominal = 0.096;
		smear_up = 0.161;
		smear_down = 0.032;
	}
	else if (abseta <= 2.3){
		smear_nominal = 0.134;
		smear_up = 0.228;
		smear_down = 0.042;
	}
	else if (abseta < 5.0){
		smear_nominal = 0.288;
		smear_up = 0.488;
		smear_down = 0.089;
	}
	
	if (error < 0.05) smear_factor = smear_down;
	else if (error < 0.15) smear_factor = smear_nominal;
	else if (error < 0.25) smear_factor = smear_up;	




	return smear_factor;	
}

// ------------ method called once each job just before starting event loop  ------------
void 
TTBSMProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTBSMProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TTBSMProducer);
