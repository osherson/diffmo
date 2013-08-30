#include "Analysis/SHyFT/interface/SHyFT.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <sstream>
#include "TRandom.h"
#include "Analysis/SHyFT/interface/combinatorics.h"
// #include "/uscmst1/prod/sw/cms/slc5_ia32_gcc434/external/lhapdf/5.6.0/full/include/LHAPDF/LHAPDF.h"
#include "/uscmst1/prod/sw/cms/slc5_ia32_gcc434/external/lhapdf/5.6.0-cms2/share/lhapdf/PDFsets/../../../include/LHAPDF/LHAPDF.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
// namespace LHAPDF {
      
//       void initPDF(int nset);
//       void initPDFSet(int nset, const std::string& filename, int member=0);
//       int numberPDF(int nset);
//       void usePDFMember(int nset, int member);
//       double xfx(int nset, double x, double Q, int fl);
//       double getXmin(int nset, int member);
//       double getXmax(int nset, int member);
//       double getQ2min(int nset, int member);
//       double getQ2max(int nset, int member);
//       void extrapolate(bool extrapolate=true);
// }


using namespace std;



SHyFT::SHyFT(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  edm::BasicAnalyzer(iConfig,iDir),
  wPlusJets(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis")),
  theDir(iDir),
 
  subdirMU_plus ( theDir.mkdir("mu_plus") ),
  subdirMU_minus ( theDir.mkdir("mu_minus") ),

  subdirEB_plus ( theDir.mkdir("eleEB_plus") ),
  subdirEE_plus ( theDir.mkdir("eleEE_plus") ),
  subdirEB_minus ( theDir.mkdir("eleEB_minus") ),
  subdirEE_minus ( theDir.mkdir("eleEE_minus") ),
 
  muPlusJets_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("muPlusJets")),
  ePlusJets_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("ePlusJets")),
  //useHFcat will no longer do anything
  useHFcat_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("heavyFlavour")),
  nJetsCut_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<int>("minJets")),  
  sampleNameInput(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<std::string>("sampleName")),
  doMC_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("doMC") ),
  identifier_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<std::string>("identifier")),
  reweightPDF_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("reweightPDF")),
  reweightBTagEff_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("reweightBTagEff")),
  pdfInputTag_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<edm::InputTag>("pdfSrc")),
  pdfToUse_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<std::string>("pdfToUse")),
  pdfEigenToUse_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<int>("pdfEigenToUse")),
  pdfVariation_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<int>("pdfVariation")),
  btaggerString_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<std::string>("btaggerString")),
  bcEffScale_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("bcEffScale")),
  lfEffScale_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("lfEffScale")),
  allDiscrCut_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("allDiscriminantCut")),
  bDiscrCut_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("bDiscriminantCut")),
  cDiscrCut_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("cDiscriminantCut")),
  lDiscrCut_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<double>("lDiscriminantCut")),
  useCustomPayload_ (iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("useCustomPayload")),
  customTagRootFile_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<string>("customPayload")),
  simpleSFCalc_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("simpleSFCalc")),
  weightSFCalc_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<bool>("weightSFCalc")),
  jetAlgo_(iConfig.getParameter<edm::ParameterSet>("shyftAnalysis").getParameter<string>("jetAlgo"))
{
  
   // TFileDirectory *elePlusMinusDir[4]={&subdirEB_plus,&subdirEB_minus,&subdirEE_plus,&subdirEE_minus};
   // TFileDirectory *muPlusMinusDir[4]={&subdirMU_plus,&subdirMU_minus};

   // Let's create another array just for the purpose of creating all these histograms
   // Whether we fill them is another story
   // How about whatever we create and fill separately for +/- charge we also fill for the combined charges 
  TFileDirectory *forBookingDir[7]={&subdirEB_plus,&subdirEB_minus,&subdirEE_plus,&subdirEE_minus,&theDir,&subdirMU_plus,&subdirMU_minus};
  

   if ( simpleSFCalc_) 
      gRandom->SetSeed( 960622508 );
  

   if ( useCustomPayload_ ) {
      customBtagFile_ = boost::shared_ptr<TFile>( new TFile(customTagRootFile_.c_str(), "READ") );
   }

   //cout <<"let book the histo " << endl;
   //book all the histograms for leptons
   theDir.make<TH1F>("lepPt",       "Lepton p_{T} (GeV/c) ",   100,    0, 200);
   theDir.make<TH1F>("lepEta",      "Lepton eta",               60, -3.0, 3.0);
   theDir.make<TH1F>("lepPhi",      "Lepton Phi",               50, -3.2, 3.2);
   theDir.make<TH1F>("lepD0",       "Lepton D0",                60, -0.2, 0.2);
   theDir.make<TH1F>("lepTrackIso", "Lepton Track Iso",         30,    0,  30);
   theDir.make<TH1F>("lepECalIso",  "Lepton ECal Iso",          30,    0,  30);
   theDir.make<TH1F>("lepHCalIso",  "Lepton HCal Iso",          30,    0,  30);
   theDir.make<TH1F>("lepRelIso",   "Lepton Rel Iso",           30,    0,  30);
   theDir.make<TH1F>("lepJetdR",    "dR b/w closest jet and lepton", 60, 0, 3.0);
     
   // book histograms that are specific to muons
   if(muPlusJets_){
     theDir.make<TH1F>("muNhits",    "Muon N Hits",            35,    0,  35);
     theDir.make<TH1F>("muChi2",     "Muon Chi2",              20,    0,   5);
     theDir.make<TH1F>("muHCalVeto", "Muon hCalVeto",          30,    0,  30);
     theDir.make<TH1F>("muECalVeto", "Muon eCalVeto",          30,    0,  30);
   }

   // book histograms that are specific for electrons
   if(ePlusJets_) {
      theDir.make<TH1F>("eDelEta_EE",  "#Delta #eta in EE", 36, -0.04, 0.04);
      theDir.make<TH1F>("eDelEta_EB",  "#Delta #eta in EB", 36, -0.04, 0.04);
      theDir.make<TH1F>("eDelPhi_EE",  "#Delta #phi in EE", 50, -0.25, 0.25);
      theDir.make<TH1F>("eDelPhi_EB",  "#Delta #phi in EB", 50, -0.25, 0.25);
      theDir.make<TH1F>("eSigihih_EE", "#sigma_{i#eta i#eta} in EE", 50, 0, 0.05);
      theDir.make<TH1F>("eSigihih_EB", "#sigma_{i#eta i#eta} in EB", 20, 0, 0.02);
      theDir.make<TH1F>("eHoE_EE", "HoE in EE", 30, 0, 0.15);
      theDir.make<TH1F>("eHoE_EB", "HoE in EB", 30, 0, 0.15);  
   }
  
   theDir.make<TH1F>("metPt", "Missing p_{T} (GeV/c)", 100, 0, 200 );
   theDir.make<TH2F>("massVsPt", "Mass vs pt", 25, 0, 250, 25, 0, 500);

  std::vector<std::string> secvtxEnd;
  std::vector<std::string> twoTagsEnd;
  std::vector<std::string> allTagsEnd;

  //will use to split by flavor paths and jet flavor per tagged jet
  if(doMC_) {
    twoTagsEnd.push_back("2t_b");  
    twoTagsEnd.push_back("2t_c"); 
    twoTagsEnd.push_back("2t_q");
    twoTagsEnd.push_back("2t_x");
    twoTagsEnd.push_back("2t");
    
    secvtxEnd.push_back("1t_b");  
    secvtxEnd.push_back("1t_c");  
    secvtxEnd.push_back("1t_q");
    secvtxEnd.push_back("1t_x");  
    secvtxEnd.push_back("1t");
    secvtxEnd.push_back("2t_b");  
    secvtxEnd.push_back("2t_c"); 
    secvtxEnd.push_back("2t_q");
    secvtxEnd.push_back("2t_x");
    secvtxEnd.push_back("2t");

    allTagsEnd.push_back("0t_b");  
    allTagsEnd.push_back("0t_c");  
    allTagsEnd.push_back("0t_q");
    allTagsEnd.push_back("0t_x");  
    allTagsEnd.push_back("0t");
    allTagsEnd.push_back("1t_b");  
    allTagsEnd.push_back("1t_c");  
    allTagsEnd.push_back("1t_q");
    allTagsEnd.push_back("1t_x");  
    allTagsEnd.push_back("1t");
    allTagsEnd.push_back("2t_b");  
    allTagsEnd.push_back("2t_c"); 
    allTagsEnd.push_back("2t_q");
    allTagsEnd.push_back("2t_x");
    allTagsEnd.push_back("2t");
  }
  else {
    twoTagsEnd.push_back("2t");
    secvtxEnd.push_back("1t"); secvtxEnd.push_back("2t");
    allTagsEnd.push_back("0t"); allTagsEnd.push_back("1t"); allTagsEnd.push_back("2t");
  }

   theDir.make<TH1F>("nJets",    "N Jets, pt>30, eta<2.4",  15,    0,   15);
   for(unsigned int i=1; i<5; ++i) {
      string jtNum = Form("%d",i);
      theDir.make<TH1F>(("jet"+jtNum+"Pt").c_str(),   ("jet "+jtNum+" leading jet pt").c_str(),     150,    0,  300);
      theDir.make<TH1F>(("jet"+jtNum+"Eta").c_str(),  ("jet "+jtNum+" leading jet eta").c_str(),     60, -3.0,  3.0);
      theDir.make<TH1F>(("jet"+jtNum+"Phi").c_str(),  ("jet "+jtNum+" leading jet phi").c_str(),     60, -3.5,  3.5);
      theDir.make<TH1F>(("jet"+jtNum+"Mass").c_str(), ("jet "+jtNum+" leading jet mass").c_str(),    50,    0,  150);
      if(doMC_) {
	theDir.make<TH2F>(("jet"+jtNum+"PtTrueRes").c_str(),("jet "+jtNum+" leading jet pt / gen pt").c_str(), 25, -5.0, 5.0, 50, 0, 3);
	theDir.make<TH2F>(("jet"+jtNum+"PtTrueResBJets").c_str(),("jet "+jtNum+" leading bjet pt / gen pt").c_str(), 25, -5.0, 5.0, 50, 0, 3);
      }
   }

   if(doMC_) {
     theDir.make<TH1F>("bmass",    "B Sec Vtx Mass",          40,    0,   10);
     theDir.make<TH1F>("cmass",    "C Sec Vtx Mass",          40,    0,   10);
     theDir.make<TH1F>("lfmass",   "LF Sec Vtx Mass",         40,    0,   10);
     theDir.make<TH1F>("flavorHistory", "Flavor History",     12,    0,   12);
      if ( reweightPDF_ )
	theDir.make<TH1F>("pdfWeight", "PDF Weight", 50, 0., 2.0);
   }
   theDir.make<TH1F>("discriminator", "BTag Discriminator", 30,    2,    8);
   theDir.make<TH1F>("nVertices",     "num sec Vertices",    5,    0,    5);
   theDir.make<TH1F>("nTags",     "number of Tags",          3,    0,    3);
  
   theDir.make<TH1F>("m3", "M3 pretag", 60, 0, 600);
   
   //Histogram booking without splitting into flavor paths but into tags
   //-------------------------------------------------------------------
   for ( int itag = 0; itag <= 2; ++itag ) {
      std::string m3_temp = "m3_" + boost::lexical_cast<std::string>(itag) + "t";
      theDir.make<TH1F>(m3_temp.c_str(), "M3", 60, 0, 600);
   }
  
   TString histTitle;  
   // Loop over 7 (sub)directories in file: EB+,EB-,EE+,EE-,theDir, mu+,mu-
   // If eplusjets, book histos for the first 5
   // If muplusjets, book histos for the last 3
   int imax=0;
   int imin=0;
   if ( ePlusJets_){ imin=0; imax=4; }
   if (muPlusJets_){ imin=4; imax=6; }
   for (int idir=imin; idir<=imax; idir++){
     for(unsigned int i=0; i<6; ++i) {
       string jtNum = Form("_%dj",i);
       string jt; 
       if(i==0 || i==1) jt = Form("%d Jet", i);
       else if(i>1)     jt = Form("%d Jets", i);
       
       for ( int itag = 0; itag <= 2; ++itag ) {
         
         if(i==0 && itag>0) continue;
         if(i==1 && itag==2) continue;

	 histTitle = "HT (#sum Jet Et + lep Pt + MET), "+jt;
	 forBookingDir[idir]->make<TH1F>( (sampleNameInput+"_hT"+jtNum + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), histTitle, 120, 0, 1200);
         
	 histTitle = "W Trans Mass, "+jt;
	 forBookingDir[idir]->make<TH1F>( (sampleNameInput+"_wMT"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), histTitle, 120,0,300);

	 forBookingDir[idir]->make<TH1F>( (sampleNameInput+"_lepPt"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "lepton p_{T}", 100, 0, 200);

	 forBookingDir[idir]->make<TH2F>( (sampleNameInput+"_lepIsoVsEta"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "Lepton isolation vs Eta",
                               12, 0., 2.4,
                               20, 0., 1.0
               );

	 forBookingDir[idir]->make<TH2F>( (sampleNameInput+"_lepIsoVsHt"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "Lepton isolation vs H_{T}",
                               20., 0., 400,
                               20, 0., 1.0
               );

	 forBookingDir[idir]->make<TH2F>( (sampleNameInput+"_lepIsoVsMET"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "Lepton isolation vs MET",
                               120., 0., 300,
                               20, 0., 1.0
               );

	 forBookingDir[idir]->make<TH2F>( (sampleNameInput+"_lepIsoVswMT"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "Lepton isolation vs wMT",
                               20., 0., 200,
                               20, 0., 1.0
               );

	 forBookingDir[idir]->make<TH2F>( (sampleNameInput+"_lepIsoVsD0"+jtNum+ + "_" + boost::lexical_cast<std::string>(itag) + "t").c_str(), "Lepton isolation vs D0",
                               100., 0., 0.2,
                               20, 0., 1.0
               );

       } // End loop over itags (0,1,2)

       // For histograms where we want _b _c _q _x endings too for MC, we'll do separately for now
       // secvtx mass
       for(unsigned int l=0;l<secvtxEnd.size();++l) {
	 if (i==0) break; //we need at least 1 jet
	 forBookingDir[idir]->make<TH1F>((sampleNameInput+"_secvtxMass"+jtNum+"_"+secvtxEnd[l]).c_str(), "secvtxmass", 40,    0.,   10.);
	 forBookingDir[idir]->make<TH2F>((sampleNameInput+"_secvtxMass"+jtNum+"_"+secvtxEnd[l]+"_vs_iso").c_str(), "lepton isolation vs. secvtxmass", 40,    0,   10,
					20, 0., 1.0 );
	 if(i==1 && l==4) break; 
	 else if( (!doMC_) && i==1 && l==0) break;  
       }//l
       // Jet et
       for(unsigned int l=0;l<allTagsEnd.size();++l) {
         if (i==0) break; //we need at least 1 jet        
	 forBookingDir[idir]->make<TH1F>((sampleNameInput+"_jetEt"+jtNum+"_"+allTagsEnd[l]).c_str(), "#sum jet pt", 100, 0, 1000);
	 if (i==1 && l==9) break;                  // cut off 1-jet bin after 1-tags
	 //else if( (!doMC_) && i==1 && l==0) break; // we use 1j 1t histogram later, so better to keep it
	 else if( (!doMC_) && i==1 && l==1) break;
       }//l
       //untag diJet mass
       for(unsigned int l=0;l<twoTagsEnd.size();++l) {
	 if (i<4) break; // we need at least 4 jets
	 forBookingDir[idir]->make<TH1F>((sampleNameInput+"_dijetMass"+jtNum+"_"+twoTagsEnd[l]).c_str(), "dijet mass", 50, 0, 500);
       }//l
       // lepton eta
       for(unsigned int l=0;l<allTagsEnd.size();++l) {
	 forBookingDir[idir]->make<TH1F>((sampleNameInput+"_lepEta"+jtNum+"_"+allTagsEnd[l]).c_str(), "Lepton eta", 30, 0, 3.0);
	   if ( i==0 && l == 4 ) break;   // cut off 0-jet bin after 0-tags
	   else if(i==1 && l == 9) break; // cut off 1-jet bin after 1-tags
	   else if( (!doMC_) && i==0 && l==0) break; // Data says "You had me at "Hello" ".
	   else if( (!doMC_) && i==1 && l==1) break; // 
       }//l
       // MET
       for(unsigned int l=0;l<allTagsEnd.size();++l) {
	 histTitle = "Missing E_{T},  "+jt;
	 forBookingDir[idir]->make<TH1F>((sampleNameInput+"_MET"+jtNum+"_"+allTagsEnd[l]).c_str(),histTitle, 120, 0, 300);
	   if ( i==0 && l == 4 ) break;   // cut off 0-jet bin after 0-tags
	   else if(i==1 && l == 9) break; // cut off 1-jet bin after 1-tags
	   else if( (!doMC_) && i==0 && l==0) break; // Data says "You had me at "Hello" ".
	   else if( (!doMC_) && i==1 && l==1) break; // 
       }//l

     } //End loop over njets
   }// End loop over idir   

   allNumTags_ = 0;
   allNumJets_ = 0;
   
   if ( reweightPDF_ ) {
      std::cout << "Initializing pdfs, identifier = " << identifier_ << std::endl;
      // For the first one, MAKE ABSOLUTELY SURE it is the one used to generate
      // your sample. 
      std::cout << "PDF to use = " << pdfToUse_ << std::endl;
      LHAPDF::initPDFSet(pdfToUse_);
      std::cout << "Done initializing pdfs" << std::endl;
   }


   // for closure test
   nExpectedTaggedJets_ = 0.;
   nObservedTaggedJets_ = 0.;
   nExpectedTaggedEvents_ = 0.;
   nObservedTaggedEvents_ = 0.;
}

// fill the plots for the electrons
bool SHyFT::analyze_electrons(const std::vector<reco::ShallowClonePtrCandidate>& electrons)
{
   bool   isEE(0), isEB(0);
   string Ex;
   if ( electrons.size() == 0 )  return false;
   const pat::Electron * electron_ = dynamic_cast<const pat::Electron*>(electrons[0].masterClonePtr().get());
   if ( electron_ == 0 )
      throw cms::Exception("InvalidElectronPointer") << "Electron pointer is invalid you schmuck." << std::endl;
   isEE = electron_->isEE();
   isEB = electron_->isEB();
   if(isEE)      Ex = "EE";   
   else if(isEB) Ex = "EB"; 
   
   double ePt_      = electron_ ->pt();
   double eEta_     = electron_ ->eta();
   double ePhi_     = electron_ ->phi();
   double eD0_      = electron_ ->dB();
   double trackIso_ = electron_ ->dr03TkSumPt();
   double eCalIso_  = electron_ ->dr03EcalRecHitSumEt();
   double hCalIso_  = electron_ ->dr03HcalTowerSumEt();
   double relIso_   = ( trackIso_ + eCalIso_ + hCalIso_ )/ePt_ ;

   double deta_     =  electron_ ->deltaEtaSuperClusterTrackAtVtx();
   double dphi_     =  electron_ ->deltaPhiSuperClusterTrackAtVtx();
   double sihih_    =  electron_ ->sigmaIetaIeta();
   double hoe_      =  electron_ ->hadronicOverEm();
   
   theDir.getObject<TH1>( "lepPt"      )->Fill( ePt_        , globalWeight_);
   theDir.getObject<TH1>( "lepEta"     )->Fill( eEta_       , globalWeight_);
   theDir.getObject<TH1>( "lepPhi"     )->Fill( ePhi_       , globalWeight_);
   theDir.getObject<TH1>( "lepD0"      )->Fill( eD0_        , globalWeight_);
   theDir.getObject<TH1>( "lepTrackIso")->Fill( trackIso_   , globalWeight_);
   theDir.getObject<TH1>( "lepECalIso" )->Fill( eCalIso_    , globalWeight_);
   theDir.getObject<TH1>( "lepHCalIso" )->Fill( hCalIso_    , globalWeight_);
   theDir.getObject<TH1>( "lepRelIso"  )->Fill( relIso_     , globalWeight_);
     
   theDir.getObject<TH1>( "eDelEta_" + Ex) ->Fill( deta_     , globalWeight_); 
   theDir.getObject<TH1>( "eDelPhi_" + Ex) ->Fill( dphi_     , globalWeight_);
   theDir.getObject<TH1>( "eSigihih_"+ Ex) ->Fill( sihih_    , globalWeight_);
   theDir.getObject<TH1>( "eHoE_"    + Ex) ->Fill( hoe_      , globalWeight_);
 
   return true;
}

// fill the plots for the muons
bool SHyFT::analyze_muons(const std::vector<reco::ShallowClonePtrCandidate>& muons)
{
   const pat::Muon * globalMuon = NULL;
   for ( ShallowCloneCollection::const_iterator muonBegin = muons.begin(),
            muonEnd = muons.end(), imuon = muonBegin;
         imuon != muonEnd; ++imuon ) {
      if ( imuon->isGlobalMuon() ) {
         globalMuon = dynamic_cast<const pat::Muon *>(imuon->masterClonePtr().get());
         break;
      }
   }

   if ( globalMuon == NULL ) {  cout<<"No Global Muon is found"<<endl; return false; }
   double muPt_       = globalMuon->pt();
   double muEta_      = globalMuon->eta();
   double muPhi_      = globalMuon->phi();
   double nhits_      = static_cast<int>( globalMuon->numberOfValidHits() );
   double muD0_       = globalMuon->dB();
   double norm_chi2_  = globalMuon->normChi2();
   double muHCalVeto_ = globalMuon->isolationR03().hadVetoEt;
   double muECalVeto_ = globalMuon->isolationR03().emVetoEt;
   double trackIso_   = globalMuon->trackIso();
   double eCalIso_    = globalMuon->ecalIso();
   double hCalIso_    = globalMuon->hcalIso();
   double relIso_     = ( trackIso_ + eCalIso_ + hCalIso_ )/muPt_ ;

   theDir.getObject<TH1>( "lepPt"      )->Fill( muPt_        , globalWeight_);
   theDir.getObject<TH1>( "lepEta"     )->Fill( muEta_       , globalWeight_);
   theDir.getObject<TH1>( "lepPhi"     )->Fill( muPhi_       , globalWeight_);
   theDir.getObject<TH1>( "lepD0"      )->Fill( muD0_        , globalWeight_);
   theDir.getObject<TH1>( "lepTrackIso")->Fill( trackIso_    , globalWeight_);
   theDir.getObject<TH1>( "lepECalIso" )->Fill( eCalIso_     , globalWeight_);
   theDir.getObject<TH1>( "lepHCalIso" )->Fill( hCalIso_     , globalWeight_);
   theDir.getObject<TH1>( "lepRelIso"  )->Fill( relIso_      , globalWeight_);

   theDir.getObject<TH1>( "muNhits"   )->Fill( nhits_       , globalWeight_);
   theDir.getObject<TH1>( "muChi2"    )->Fill( norm_chi2_   , globalWeight_);
   theDir.getObject<TH1>( "muHCalVeto")->Fill( muHCalVeto_  , globalWeight_);
   theDir.getObject<TH1>( "muECalVeto")->Fill( muECalVeto_  , globalWeight_);
  
   return true;
}


// fill the plots for the jets
bool SHyFT::make_templates(const std::vector<reco::ShallowClonePtrCandidate>& jets,
                           const reco::ShallowClonePtrCandidate             & met,
                           const std::vector<reco::ShallowClonePtrCandidate>& muons,
                           const std::vector<reco::ShallowClonePtrCandidate>& electrons)
{
  TFileDirectory *subDir = 0;//Dummy sub directory to be replaced either by EE,EB,or MU; plus or minus 
  allNumTags_ = 0;
  allNumJets_ = (int) jets.size();

  //--------Old stuff of b-tagging reweighting scheme------------
  TH1 * bEff = 0;
  TH1 * cEff = 0;
  TH1 * pEff = 0;
  if ( useCustomPayload_ ) {
    std::string bstr = jetAlgo_ + "BEff";
    std::string cstr = jetAlgo_ + "CEff";
    std::string pstr = jetAlgo_ + "LFEff";

    bEff = (TH1*)customBtagFile_->Get( bstr.c_str() );
    cEff = (TH1*)customBtagFile_->Get( cstr.c_str() );
    pEff = (TH1*)customBtagFile_->Get( pstr.c_str() );        
  }
  //-----------end old stuff-------------
  // std::cout << "Filling global weight in make_templates : " << globalWeight_ << std::endl;
  reco::Candidate::LorentzVector nu_p4 = met.p4();
  reco::Candidate::LorentzVector lep_p4 = ( muPlusJets_  ? muons[0].p4() : electrons[0].p4() );
  double wPt = lep_p4.Pt() + nu_p4.Pt();
  double wPx = lep_p4.Px() + nu_p4.Px();
  double wPy = lep_p4.Py() + nu_p4.Py();
  double wMT = TMath::Sqrt(wPt*wPt-wPx*wPx-wPy*wPy);
  double hT = lep_p4.pt() + nu_p4.Et();
    

  double hcalIso(-99.), ecalIso(-99.), trkIso(-99.), lepPt(-99.), relIso(-99.), lepD0(-99), lepEta(-99.), lepPhi(-99.);
  bool   isEE(0), isEB(0);
  if(muPlusJets_){
    pat::Muon const * patMuon = dynamic_cast<pat::Muon const *>(&* muons[0].masterClonePtr());
    if ( patMuon == 0 )
      throw cms::Exception("InvalidMuonPointer") << "Muon pointer is invalid you schmuck." << std::endl;
    if(     patMuon->charge() == 1)  subDir = &subdirMU_plus;
    else if(patMuon->charge() == -1) subDir = &subdirMU_minus;
    if(subDir == 0 )
      throw cms::Exception("InvalidPointertoSubDirectories") << "sub directory pointer is invalid." << std::endl;

    hcalIso = patMuon->hcalIso();
    ecalIso = patMuon->ecalIso();
    trkIso  = patMuon->trackIso();
    lepPt      = patMuon->pt() ;
    lepD0      = fabs(patMuon->dB());  
    relIso  = (ecalIso + hcalIso + trkIso) / lepPt;
    lepEta  = patMuon->eta();
    lepPhi  = patMuon->phi();
  }
  else if(ePlusJets_){
    pat::Electron const * patElectron = dynamic_cast<pat::Electron const *>(&* electrons[0].masterClonePtr());
    if ( patElectron == 0 )
      throw cms::Exception("InvalidElectronPointer") << "Electron pointer is invalid you schmuck." << std::endl;
    //split into EE and EB and charge directories
    isEE = patElectron->isEE();
    isEB = patElectron->isEB();    
    if(     isEE && patElectron->charge() == 1)  subDir = &subdirEE_plus;
    else if(isEB && patElectron->charge() == 1)  subDir = &subdirEB_plus; 
    else if(isEE && patElectron->charge() == -1) subDir = &subdirEE_minus;
    else if(isEB && patElectron->charge() == -1) subDir = &subdirEB_minus; 
    if(subDir == 0 )
       throw cms::Exception("InvalidPointertoSubDirectories") << "sub directory pointer is invalid." << std::endl;
    
    hcalIso = patElectron->dr03HcalTowerSumEt();
    ecalIso = patElectron->dr03EcalRecHitSumEt();
    trkIso  = patElectron->dr03TkSumPt();
    lepPt      = patElectron->pt() ;
    lepD0      = fabs(patElectron->dB());
    relIso = (ecalIso + hcalIso + trkIso) / lepPt;
    lepEta  = patElectron->eta();
    lepPhi  = patElectron->phi();
  }
  unsigned int maxJets = jets.size();
  unsigned int ibjet = 0;
  if ( (int)maxJets >= nJetsCut_ ) {
    if ( maxJets > 4 ) maxJets = 4;
    for ( unsigned int i=0; i<maxJets; ++i) {
      theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(i+1) + "Pt") ->Fill( jets[i].pt()  , globalWeight_);
      theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(i+1) + "Eta")->Fill( jets[i].eta() , globalWeight_);
      theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(i+1) + "Phi")->Fill( jets[i].phi() , globalWeight_);
      theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(i+1) + "Mass")->Fill( jets[i].mass() , globalWeight_);
      pat::Jet const * patJet = dynamic_cast<pat::Jet const *>( &* jets[i].masterClonePtr()  );
      if ( doMC_ && patJet != 0 && patJet->genJet() != 0 ) {
	static_cast<TH2*>(theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(i+1) + "PtTrueRes")) ->Fill( jets[i].eta(), jets[i].pt() / patJet->genJet()->pt() , globalWeight_ );
	if ( abs(patJet->partonFlavour()) == 5 ) {
	  ++ibjet;
	  static_cast<TH2*>(theDir.getObject<TH1>( "jet" + boost::lexical_cast<std::string>(ibjet) + "PtTrueResBJets")) ->Fill( jets[i].eta(), jets[i].pt() / patJet->genJet()->pt(), globalWeight_  );
	}
      }//genJet
    }//ijet
  }//no of jets 
  
  //SecVtxMass and b-tagging related quantities
  //-------------------------------------------
  int numBottom=0,numCharm=0,numLight=0;
  int numTags=0, numJets=0;
  // A bit of explanation:
  // The vertex mass is only defined (obviously) for jets with a vertex. So,
  // if we are counting tags from MC, there's no problem. However, if we are reweighting the jets
  // based on the tag efficiencies, there's an issue, because if we set this to "-1", then 
  // there's no way to achieve a correct event counting since "Integral" won't count the underflow
  // bins. Furthermore, the shape is meaningless because lots of jets with mass=-1 will be
  // included in the weighting of the distribution.
  // Thus, we set this to -1 for the straight counting, and 0 for the weighted version.
  // At the end we use the shape from counting but the rates from weighting. 
  double vertexMass = -1.0;
  if ( reweightBTagEff_ )
    vertexMass = 0.0;

  std::vector<double> vertexMasses;

  // Fill the M3 for >=3 jets
  // --------------
  reco::Candidate::LorentzVector p4_m3(0,0,0,0);
  double M3 = 0.0;
  double highestPt = 0.0;
  if ( jets.size() >= 3 ) {

    // std::vector<TVector3> JetEnergy;
    std::vector<TLorentzVector> jets_p4;
    for (unsigned i=0; i< jets.size(); ++i) {
      // JetEnergy.push_back( TVector3( jets[i].px(), jets[i].py(), jets[i].pz() ) );
      jets_p4.push_back( TLorentzVector( jets[i].px(), jets[i].py(), jets[i].pz(), jets[i].energy() ) );
    }
    
    for (unsigned int j=0;j<jets.size() - 2;++j) {
      for (unsigned int k=j+1;k<jets.size() - 1;++k) {
	for (unsigned int l = k+1; l<jets.size(); ++l) {
	  TLorentzVector threeJets = jets_p4[j] + jets_p4[k] + jets_p4[l];
	  if (highestPt < threeJets.Perp()) {
	    M3 = threeJets.M();
	    highestPt=threeJets.Perp();
	  }
	}
      }
    }
    theDir.getObject<TH1>( "m3")->Fill( M3, globalWeight_ );
  }
 
  //std::cout << "M3 = " << M3 << std::endl;

  // bool foundWeird = false;
  // --------------
  // Loop over the jets. Find the flavor of the *highest pt jet* that passes
  // the discriminator cuts. 
  //    Check the parton flavor if MC
  //    Plot pt versus mass 
  //    If there is a SecondaryVertexTagInfo:
  //        plot number of vertices
  //        if there are >= 1 vertex:
  //               fill discriminator
  //               if MC, fill the tag eff.
  //               if discriminator passes cuts (i.e. is tagged):
  //                     fill secondary vertex mass
  //                     if MC, fill template
  // ------------------------------------------------------------------------------------
  std::vector<int> taggedIndex;
  bool firstTag = true;
  double jetEt = 0;
  for ( ShallowCloneCollection::const_iterator jetBegin = jets.begin(),
	  jetEnd = jets.end(), jetIter = jetBegin;
	jetIter != jetEnd; ++jetIter)
    {
      const pat::Jet* jet = dynamic_cast<const pat::Jet *>(jetIter->masterClonePtr().get());
      // We first get the flavor of the jet so we can fill look at btag efficiency.
      int jetFlavor = std::abs( jet->partonFlavour() );
      double jetPt  = std::abs( jet->pt() );
      jetEt += jet->et();
      hT += jet->et();
      
      static_cast<TH2*>(theDir.getObject<TH1>( "massVsPt") )->Fill( jetPt, jet->mass(), globalWeight_ );

      //dR b/w jet and lepton
      double dR = reco::deltaR( lepEta, lepPhi, jetIter->eta(), jetIter->phi() );
      theDir.getObject<TH1>( "lepJetdR")->Fill( dR, globalWeight_ );
      
      //Here we determine what kind of flavor we have in this jet	
      if( doMC_ ) {
	switch (jetFlavor)
	  {
	  case 5:
	    // bottom
	    ++numBottom;
	    break;
	  case 4:
	    // charm
	    ++numCharm;
	    break;
	  default:
	    // light flavour
	    ++numLight;
	  }
      }

      // Get the secondary vertex tag info
      reco::SecondaryVertexTagInfo const * svTagInfos
	= jet->tagInfoSecondaryVertex("secondaryVertex");
      if ( svTagInfos == 0 ) { 
	continue;
      }
      theDir.getObject<TH1>( "nVertices")-> Fill( svTagInfos->nVertices(), globalWeight_ );
      
      // Check to make sure we have a vertex
      if ( svTagInfos->nVertices() <= 0 ) {
	continue;
      }
      
      // This discriminator is only filled when we have a secondary vertex
      // tag info and a vertex in it
      theDir.getObject<TH1>( "discriminator")-> Fill ( jet->bDiscriminator(btaggerString_), globalWeight_ );
     
      //std::cout << "Jet " << jetIter - jetBegin << ", pt = " << jet->pt() << std::endl;
      // typedef std::pair<std::string,float> sfpair;
      // typedef std::vector<sfpair> sfpair_coll;
      // sfpair_coll const & discs = jet->getPairDiscri();
      // for ( sfpair_coll::const_iterator ipairBegin = discs.begin(),
      // 	      ipairEnd = discs.end(), ipair = ipairBegin;
      // 	    ipair != ipairEnd; ++ipair ) {
      // 	std::cout << " disc : " << ipair->first << " = " << ipair->second << std::endl;
      // }


      // Check to see if the actual jet is tagged
      double discCut = allDiscrCut_;
      if ( doMC_ ) {
	if ( bDiscrCut_ > 0.0 && jetFlavor == 5 ){ 
	  discCut = bDiscrCut_;
	}
	if ( cDiscrCut_ > 0.0 && jetFlavor == 4 ) { 
	  discCut = cDiscrCut_;
	}
	if ( lDiscrCut_ > 0.0 && jetFlavor!=5 && jetFlavor != 4 ) {
	  discCut = lDiscrCut_;	
	}
      }

      // If desired to use the simple SF inclusion, throw a random variable between 1 and 0,
      // if it is less than the SF, keep going, else, throw the jet away
      bool keepGoing = true;
      if ( simpleSFCalc_ ) {
	double irand = gRandom->Uniform();
	double max = 0;
	if ( jetFlavor == 5 || jetFlavor == 4 )
	  max = bcEffScale_;
	else
	  max = lfEffScale_;
	if ( irand < max ) keepGoing=true;
	else keepGoing=false;
      } 
      
      // copy ( jet->userDataNames().begin(), jet->userDataNames().end(), std::ostream_iterator<std::string>(std::cout, "\n"));

      // if ( jet->hasUserData("secvtxMass") )
	vertexMasses.push_back( jet->userFloat("secvtxMass") );
      // else {
      // 	std::cout << "Yuck... you forgot to add the userData." << std::endl;
      // 	vertexMasses.push_back( svTagInfos->secondaryVertex(0).p4().mass() );
      // }
      
      
      if( jet->bDiscriminator(btaggerString_) < discCut || !keepGoing ) continue;
      ++allNumTags_;
      taggedIndex.push_back(jetIter-jetBegin);
    
      // Take the template info from the first tag (ordered by jet pt)      
      if ( firstTag ) {
	
	// if ( jet->hasUserData("secvtxMass") )
	  vertexMass = jet->userFloat("secvtxMass");
	// else {
	//   std::cout << "Yuck... you forgot to add the userData." << std::endl;
	//   vertexMass = svTagInfos->secondaryVertex(0).p4().mass();
	// }
	
        firstTag = false;
      }// end if first tag
    }// end loop over jets
  

  //Here we determine what kind of flavor we have in this jet	
  string whichtag = "";
  int massFlavor = 0;
  if( doMC_ ) {

    // Restrict to >= 2 tags
    if(numBottom>2) numBottom=2;
    if(numCharm>2 ) numCharm =2;
    if(numLight>2 ) numLight =2;

    if      ( numBottom ) massFlavor = 5; 
    else if ( numCharm  ) massFlavor = 4;
    else                  massFlavor = 0;

    switch (massFlavor)
      {
      case 5:
	// bottom
	theDir.getObject<TH1>( "bmass")->Fill(vertexMass, globalWeight_);
	break;
      case 4:
	// charm
	theDir.getObject<TH1>( "cmass")->Fill(vertexMass, globalWeight_);
	break;
      default:
	// light flavour
	theDir.getObject<TH1>( "lfmass")->Fill(vertexMass, globalWeight_);
      }

    if      (numBottom)              whichtag = "_b";
    else if (numCharm)               whichtag = "_c";
    else if (numLight)               whichtag = "_q";
    else                             whichtag = "_x";


    // if (1 == numTags) {
    //   // single tag
    //   if      (numBottom)              whichtag = "_b";
    //   else if (numCharm)               whichtag = "_c";
    //   else if (numLight)               whichtag = "_q";
    //   else                             whichtag = "_x";
    // }
    // else {
    //   // double tags
    //   if      (2 == numBottom)         whichtag = "_bb";
    //   else if (2 == numCharm)          whichtag = "_cc";
    //   else if (2 == numLight)          whichtag = "_qq";
    //   else if (numBottom && numCharm)  whichtag = "_bc";
    //   else if (numBottom && numLight)  whichtag = "_bq";
    //   else if (numCharm  && numLight)  whichtag = "_cq";
    //   else                             whichtag = "_xx";
    // } // if two tags
  }//doMC_
  
  // For now, we only care if we have 2 tags...any more are treated the same - maybe we should look at 3 tags?
  numTags = std::min( allNumTags_, 2 );
  numJets = std::min( allNumJets_, 5 );
  
//--------------------------------
  double M2Min = -99.; 
  //At least 2 tags
  if(numJets>3 && numTags == 2){

     //for(std::vector<int>::iterator ti=taggedIndex.begin(); ti!=taggedIndex.end(); ti++)
     //  std::cout << "tag index " << *ti << " ";
     //std::cout << std::endl << std::endl;
     
     //copy the jet collection since originally its a constant vector
     std::vector<reco::ShallowClonePtrCandidate>::iterator iter1,iter2;
     std::vector<reco::ShallowClonePtrCandidate> orderedJets;
     orderedJets.resize(jets.size());
     copy(jets.begin(),jets.end(),orderedJets.begin());
        
     //for(iter1=orderedJets.begin(); iter1!=orderedJets.end(); iter1++)
     //   std::cout << iter1->pt() << "   ";     
     //std::cout << std::endl << std::endl;

     //suffle them so that the btag jets appears at position 0 and 1
     rotate(orderedJets.begin(), orderedJets.begin()+taggedIndex[0], orderedJets.begin()+taggedIndex[0]+1);
     rotate(orderedJets.begin()+1, orderedJets.begin()+taggedIndex[1], orderedJets.begin()+taggedIndex[1]+1);

     int untag_counter = 0; double diff(1000.), M2(0.); 
     math::XYZTLorentzVector  jet3_p4,  jet4_p4; 
     std::vector<math::XYZTLorentzVector> untagJets_p4;
     for (unsigned i=0; i< orderedJets.size(); ++i) {
        untag_counter++;
        if(untag_counter  > 2) //since first two places are reserved for tagged jets
           untagJets_p4.push_back( math::XYZTLorentzVector( orderedJets[i].px(), orderedJets[i].py(), orderedJets[i].pz(), orderedJets[i].energy() ) );
     }
  
     for (unsigned int j=0;j<untagJets_p4.size() - 1;++j) {
        for (unsigned int k=j+1;k<untagJets_p4.size();++k) {
           math::XYZTLorentzVector twoJets = untagJets_p4[j] + untagJets_p4[k];
           M2 = twoJets.M();           
           if(diff > abs(M2 - 80.4) ){ //pick the combination closest to W mass
              diff = abs(M2 - 80.4);
              M2Min = M2;
           }
        }
     }
  }//if >=4jets and >=2 tags
  //std::cout << "M2 = " <<M2Min <<std::endl;

  if(numJets > 3){
       string m3Name = Form("m3_%dt", numTags);
       theDir.getObject<TH1>( m3Name )->Fill(M3, globalWeight_);
  }
    
  theDir.getObject<TH1>( "nTags")->Fill(numTags, globalWeight_);


  if ( !reweightBTagEff_ )  {

     //Fill variables with only 2 tags
     //-------------------------------
     if( numJets> 3 && numTags == 2){
        //std::cout << "dijetMass of untagged jets = " << M2Min << std::endl;
        string diJetMassName  = sampleNameInput + Form("_dijetMass_%dj_%dt", numJets, numTags); 
	subDir->getObject<TH1>(diJetMassName)-> Fill (M2Min, globalWeight_);
	theDir.getObject<TH1>(diJetMassName)-> Fill (M2Min, globalWeight_);
	if( doMC_ ){
	  subDir->getObject<TH1>(diJetMassName + whichtag  )-> Fill (M2Min, globalWeight_);
	  theDir.getObject<TH1>(diJetMassName + whichtag  )-> Fill (M2Min, globalWeight_);
	}
     } // end if 
     
     //Fill variables with only 1,2 tags
     //---------------------------------
     if( numTags > 0 ) {
       string massName  = sampleNameInput + Form("_secvtxMass_%dj_%dt", numJets, numTags);   
       subDir->getObject<TH1>(massName             )-> Fill (vertexMass, globalWeight_);
       theDir.getObject<TH1>(massName             )-> Fill (vertexMass, globalWeight_);
       static_cast<TH2*>(subDir->getObject<TH1>( massName +"_vs_iso"            ))-> Fill (vertexMass, relIso, globalWeight_);
       static_cast<TH2*>(theDir.getObject<TH1>( massName +"_vs_iso"            ))-> Fill (vertexMass, relIso, globalWeight_);
       if( doMC_ ) {
	 subDir->getObject<TH1>(massName + whichtag  )-> Fill (vertexMass, globalWeight_);
	 theDir.getObject<TH1>(massName + whichtag  )-> Fill (vertexMass, globalWeight_);
	 static_cast<TH2*>(subDir->getObject<TH1>( massName + whichtag + "_vs_iso"  ))-> Fill (vertexMass, relIso, globalWeight_);
	 static_cast<TH2*>(theDir.getObject<TH1>( massName + whichtag + "_vs_iso"  ))-> Fill (vertexMass, relIso, globalWeight_);
       } // end if doMC
     } // end if numTags > 0    

   
    //Fill variables with all tags (0,1,2)
    //------------------------------------
    //string lepEtaName  = sampleHistName_ + Form("_lepEta_%dj_%dt", numJets, numTags);
    //subDir->getObject<TH1>( lepEtaName )-> Fill (fabs(lepEta), globalWeight_);	
    //theDir.getObject<TH1>( lepEtaName )-> Fill (fabs(lepEta), globalWeight_);	
    if ( doMC_ ){
      subDir->getObject<TH1>( sampleNameInput + Form("_lepEta_%dj_%dt",numJets, numTags) + whichtag  )->Fill( fabs(lepEta), globalWeight_);
      theDir.getObject<TH1>( sampleNameInput + Form("_lepEta_%dj_%dt",numJets, numTags) + whichtag  )->Fill( fabs(lepEta), globalWeight_);
      subDir->getObject<TH1>( sampleNameInput + Form("_MET_%dj_%dt",   numJets, numTags) + whichtag  )->Fill( met.pt(),    globalWeight_ );
      theDir.getObject<TH1>( sampleNameInput + Form("_MET_%dj_%dt",   numJets, numTags) + whichtag  )->Fill( met.pt(),    globalWeight_ );
      if(numJets!=0){
	subDir->getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, numTags) + whichtag )-> Fill (jetEt, globalWeight_);
	theDir.getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, numTags) + whichtag )-> Fill (jetEt, globalWeight_);
      }
    }

    subDir->getObject<TH1>(sampleNameInput + Form("_lepEta_%dj_%dt", numJets, numTags))->Fill( fabs(lepEta),  globalWeight_ );
    subDir->getObject<TH1>(sampleNameInput + Form("_hT_%dj_%dt",     numJets, numTags))->Fill( hT,            globalWeight_ );
    subDir->getObject<TH1>(sampleNameInput + Form("_wMT_%dj_%dt",    numJets, numTags))->Fill( wMT,           globalWeight_ );
    subDir->getObject<TH1>(sampleNameInput + Form("_MET_%dj_%dt",    numJets, numTags))->Fill( met.pt(),      globalWeight_ );
    subDir->getObject<TH1>(sampleNameInput + Form("_lepPt_%dj_%dt",  numJets, numTags))->Fill( fabs(lepPt),  globalWeight_ );
       
    theDir.getObject<TH1>(sampleNameInput + Form("_lepEta_%dj_%dt", numJets, numTags))->Fill( fabs(lepEta),  globalWeight_ );
    theDir.getObject<TH1>(sampleNameInput + Form("_hT_%dj_%dt",     numJets, numTags))->Fill( hT,            globalWeight_ );
    theDir.getObject<TH1>(sampleNameInput + Form("_wMT_%dj_%dt",    numJets, numTags))->Fill( wMT,           globalWeight_ );
    theDir.getObject<TH1>(sampleNameInput + Form("_MET_%dj_%dt",    numJets, numTags))->Fill( met.pt(),      globalWeight_ );
    theDir.getObject<TH1>(sampleNameInput + Form("_lepPt_%dj_%dt",  numJets, numTags))->Fill( fabs(lepPt),   globalWeight_ );
    
    if(numJets!=0) {
      subDir->getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, numTags))-> Fill (jetEt, globalWeight_ );
      theDir.getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, numTags))-> Fill (jetEt, globalWeight_ );
    }

    static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsEta_%dj_%dt", numJets, numTags)))->Fill( fabs(lepEta), relIso, globalWeight_ );
    static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsHt_%dj_%dt",  numJets, numTags)))->Fill( hT, relIso, globalWeight_ );
    static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVswMT_%dj_%dt", numJets, numTags)))->Fill( wMT, relIso, globalWeight_ );
    static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsMET_%dj_%dt", numJets, numTags)))->Fill( met.pt(), relIso, globalWeight_ );
    static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsD0_%dj_%dt",  numJets, numTags)))->Fill( lepD0, relIso, globalWeight_ );

    static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsEta_%dj_%dt", numJets, numTags)))->Fill( fabs(lepEta), relIso, globalWeight_ );
    static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsHt_%dj_%dt",  numJets, numTags)))->Fill( hT, relIso, globalWeight_ );
    static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVswMT_%dj_%dt", numJets, numTags)))->Fill( wMT, relIso, globalWeight_ );
    static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsMET_%dj_%dt", numJets, numTags)))->Fill( met.pt(), relIso, globalWeight_ );
    static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsD0_%dj_%dt",  numJets, numTags)))->Fill( lepD0, relIso, globalWeight_ );
  }// end if not reweighting b-tag eff
  
  else {  // reweighting b-tag eff:
     
     //std::cout << "Reweighting b-tag eff" << std::endl;
    // Here will be the efficiencies for the jets
    std::vector<shyft::helper::EffInfo> effs;
    // "unity"
    shyft::helper::EffInfo unity( -1, 1.0, 0);

    // Loop over jets, create the efficiency vector to be used in
    // the combinatoric calculations. 
    // std::cout << "Looping over jets" << std::endl;
    for ( ShallowCloneCollection::const_iterator jetBegin = jets.begin(),
	    jetEnd = jets.end(), jetIter = jetBegin;
	  jetIter != jetEnd; ++jetIter)
      {
         //std::cout << "Processing jet " << jetIter - jetBegin << std::endl;
	const pat::Jet* jet = dynamic_cast<const pat::Jet *>(jetIter->masterClonePtr().get());
	// We first get the flavor of the jet so we can fill look at btag efficiency.
	int jetFlavor = std::abs( jet->partonFlavour() );
	double jetPt  = std::abs( jet->pt() );
	//double jetEta = std::abs( jet->eta() );


	if ( weightSFCalc_ ) {
	  double isf = 1.0;

	  // Check to see if the actual jet is tagged
	  double discCut = allDiscrCut_;
	  if ( bDiscrCut_ > 0.0 && jetFlavor == 5 ){ 
	    discCut = bDiscrCut_;
	  }
	  if ( cDiscrCut_ > 0.0 && jetFlavor == 4 ) { 
	    discCut = cDiscrCut_;
	  }
	  if ( lDiscrCut_ > 0.0 && jetFlavor!=5 && jetFlavor != 4 ) {
	    discCut = lDiscrCut_;	
	  }
	  
	  
	  if ( jetFlavor == 5 || jetFlavor == 4 ) {
	    isf = bcEffScale_;
	  } else { 
	    isf = lfEffScale_;
	  }

	  bool tagged = jet->bDiscriminator(btaggerString_) >= discCut;
	  if (!tagged )
	    isf = 0.0;

	  effs.push_back( shyft::helper::EffInfo(jetIter - jetBegin, 
						 isf,
						 jetFlavor
						 ) );
	}
	else {//old weighting scheme from the ttbar eff plot
	  int ibin = bEff->GetXaxis()->FindBin( jetPt );
	  // int jbin = bEff->GetYaxis()->FindBin( jetEta );
	  if ( ibin >= bEff->GetNbinsX() ) ibin = bEff->GetNbinsX() - 1;
	  if ( jetFlavor == 5 ) {
	    effs.push_back( shyft::helper::EffInfo(jetIter - jetBegin, 
						   bEff->GetBinContent( ibin ) * bcEffScale_,
						   jetFlavor
						   ) );
	  } else if (jetFlavor == 4 ) {
	    effs.push_back( shyft::helper::EffInfo(jetIter - jetBegin, 
						   cEff->GetBinContent( ibin ) * bcEffScale_,
						   jetFlavor
						   ) );
	  } else {
	    effs.push_back( shyft::helper::EffInfo(jetIter - jetBegin, 
						   pEff->GetBinContent( ibin ) * lfEffScale_,
						   jetFlavor
						   ) );
	  }
	}//old weighting scheme
      }//jet iterator
    
    double totalSum = 0.0;
    for ( unsigned int inumTags = 0; inumTags <= jets.size(); ++inumTags ) {
      // std::cout << "inumTags = " << inumTags << std::endl;
      double sum = 0.0;
      do {

	// std::cout << "Combination : " << std::endl;
	// copy(effs.begin(), effs.end(), std::ostream_iterator<shyft::helper::EffInfo>(std::cout, " "));
	// std::cout << std::endl;

	// Get the "tag" bit, from "begin" to "inumTags"
	shyft::helper::EffInfo ni = accumulate(effs.begin(), effs.begin() + inumTags, unity, std::multiplies<shyft::helper::EffInfo>());
	// Get the "untag" bit, from "inumTags" to "end"
	shyft::helper::EffInfo nj = accumulate(effs.begin()+inumTags, effs.end(), unity, shyft::helper::oneminusmultiplies<shyft::helper::EffInfo>());
	// The product is the total probability to tag exactly inumTags jets.
	double iprob = ni.eff * nj.eff;
	sum += iprob;	

	int kknumTags = std::min( (int) inumTags, 2 );
	if(numJets > 3){
	  string m3Name = Form("m3_%dt", kknumTags);
	  theDir.getObject<TH1>( m3Name )->Fill(M3, globalWeight_ * iprob);
	}

	//Fill variables with only 2 tags
	//-------------------------------
	if( numJets> 3 && kknumTags == 2){
	  //std::cout << "dijetMass of untagged jets = " << M2Min << std::endl;
	  string diJetMassName  = sampleNameInput + Form("_dijetMass_%dj_%dt", numJets, kknumTags); 
	  subDir->getObject<TH1>(diJetMassName)-> Fill (M2Min, globalWeight_ * iprob);
	  theDir.getObject<TH1>(diJetMassName)-> Fill (M2Min, globalWeight_ * iprob);
	  if( doMC_ ){
	    subDir->getObject<TH1>(diJetMassName + whichtag  )-> Fill (M2Min, globalWeight_ * iprob);
	    theDir.getObject<TH1>(diJetMassName + whichtag  )-> Fill (M2Min, globalWeight_ * iprob);
	  }
	} // end if 

	//Fill variables with only 1,2 tags
	//---------------------------------
	if( kknumTags > 0 ) {
	  string massName  = sampleNameInput + Form("_secvtxMass_%dj_%dt", numJets, kknumTags);      
	  subDir->getObject<TH1>( massName                       )-> Fill (vertexMass, globalWeight_ * iprob);
	  theDir.getObject<TH1>( massName                       )-> Fill (vertexMass, globalWeight_ * iprob);
	  static_cast<TH2*>(subDir->getObject<TH1>( massName +"_vs_iso"            ))-> Fill (vertexMass, relIso, globalWeight_ * iprob);
	  static_cast<TH2*>(theDir.getObject<TH1>( massName +"_vs_iso"            ))-> Fill (vertexMass, relIso, globalWeight_ * iprob);
	  if( doMC_ ) {	
	    subDir->getObject<TH1>(massName + whichtag              )-> Fill (vertexMass, globalWeight_ * iprob);	
	    theDir.getObject<TH1>(massName + whichtag              )-> Fill (vertexMass, globalWeight_ * iprob);	
	    static_cast<TH2*>(subDir->getObject<TH1>( massName + whichtag + "_vs_iso"  ))-> Fill (vertexMass, relIso, globalWeight_ * iprob);
	    static_cast<TH2*>(theDir.getObject<TH1>( massName + whichtag + "_vs_iso"  ))-> Fill (vertexMass, relIso, globalWeight_ * iprob);
	  } // end if doMC
	} // end if kknumTags > 0
	
	//Fill variables with all tags (0,1,2)     
	if ( doMC_  ){
	  subDir->getObject<TH1>( sampleNameInput + Form("_lepEta_%dj_%dt",numJets, kknumTags) + whichtag  )->Fill( fabs(lepEta), globalWeight_ * iprob);
	  theDir.getObject<TH1>( sampleNameInput + Form("_lepEta_%dj_%dt",numJets, kknumTags) + whichtag  )->Fill( fabs(lepEta), globalWeight_ * iprob);
	  subDir->getObject<TH1>( sampleNameInput + Form("_MET_%dj_%dt",   numJets, kknumTags) + whichtag  )->Fill( met.pt(),    globalWeight_ * iprob );
	  theDir.getObject<TH1>( sampleNameInput + Form("_MET_%dj_%dt",   numJets, kknumTags) + whichtag  )->Fill( met.pt(),    globalWeight_ * iprob );
	  if(numJets!=0){
	    subDir->getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, kknumTags) + whichtag )-> Fill (jetEt, globalWeight_ * iprob);
	    theDir.getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, kknumTags) + whichtag )-> Fill (jetEt, globalWeight_ * iprob);
	  }
	}
	
	subDir->getObject<TH1>(sampleNameInput + Form("_lepEta_%dj_%dt", numJets, kknumTags))->Fill( fabs(lepEta),  globalWeight_ * iprob );
	subDir->getObject<TH1>(sampleNameInput + Form("_hT_%dj_%dt",     numJets, kknumTags))->Fill( hT,            globalWeight_ * iprob );
	subDir->getObject<TH1>(sampleNameInput + Form("_wMT_%dj_%dt",    numJets, kknumTags))->Fill( wMT,           globalWeight_ * iprob );
	subDir->getObject<TH1>(sampleNameInput + Form("_MET_%dj_%dt",    numJets, kknumTags))->Fill( met.pt(),      globalWeight_ * iprob );
	subDir->getObject<TH1>(sampleNameInput + Form("_lepPt_%dj_%dt",  numJets, kknumTags))->Fill( fabs(lepPt),  globalWeight_ * iprob );
	
	theDir.getObject<TH1>(sampleNameInput + Form("_lepEta_%dj_%dt", numJets, kknumTags))->Fill( fabs(lepEta),  globalWeight_ * iprob );
	theDir.getObject<TH1>(sampleNameInput + Form("_hT_%dj_%dt",     numJets, kknumTags))->Fill( hT,            globalWeight_ * iprob );
	theDir.getObject<TH1>(sampleNameInput + Form("_wMT_%dj_%dt",    numJets, kknumTags))->Fill( wMT,           globalWeight_ * iprob );
	theDir.getObject<TH1>(sampleNameInput + Form("_MET_%dj_%dt",    numJets, kknumTags))->Fill( met.pt(),      globalWeight_ * iprob );
	theDir.getObject<TH1>(sampleNameInput + Form("_lepPt_%dj_%dt",  numJets, kknumTags))->Fill( fabs(lepPt),   globalWeight_ * iprob );
	
	if(numJets!=0) {
	  subDir->getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, kknumTags))-> Fill (jetEt, globalWeight_ );
	  theDir.getObject<TH1>( sampleNameInput + Form("_jetEt_%dj_%dt", numJets, kknumTags))-> Fill (jetEt, globalWeight_ );
	}
	
	static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsEta_%dj_%dt", numJets, kknumTags)))->Fill( fabs(lepEta), relIso, globalWeight_ * iprob );
	static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsHt_%dj_%dt",  numJets, kknumTags)))->Fill( hT, relIso, globalWeight_ * iprob );
	static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVswMT_%dj_%dt", numJets, kknumTags)))->Fill( wMT, relIso, globalWeight_ * iprob );
	static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsMET_%dj_%dt", numJets, kknumTags)))->Fill( met.pt(), relIso, globalWeight_ * iprob );
	static_cast<TH2*>(subDir->getObject<TH1>(sampleNameInput + Form("_lepIsoVsD0_%dj_%dt",  numJets, kknumTags)))->Fill( lepD0, relIso, globalWeight_ * iprob );
	
	static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsEta_%dj_%dt", numJets, kknumTags)))->Fill( fabs(lepEta), relIso, globalWeight_ * iprob );
	static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsHt_%dj_%dt",  numJets, kknumTags)))->Fill( hT, relIso, globalWeight_ * iprob );
	static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVswMT_%dj_%dt", numJets, kknumTags)))->Fill( wMT, relIso, globalWeight_ * iprob );
	static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsMET_%dj_%dt", numJets, kknumTags)))->Fill( met.pt(), relIso, globalWeight_ * iprob );
	static_cast<TH2*>(theDir.getObject<TH1>(sampleNameInput + Form("_lepIsoVsD0_%dj_%dt",  numJets, kknumTags)))->Fill( lepD0, relIso, globalWeight_ * iprob );
	
     
      } while (shyft::helper::next_combination(effs.begin(),effs.begin() + inumTags, effs.end())); 
      // std::cout << "Combination probability = " << sum << std::endl;
      totalSum += sum;
    } // end loop over number of tags
    // std::cout << "Total probabilty = " << totalSum << std::endl;
  } // end if reweighting b-tag eff
  return true;
}

bool SHyFT::analyze_met(const reco::ShallowClonePtrCandidate & met)
{
  theDir.getObject<TH1>( "metPt")->Fill( met.pt(), globalWeight_ );
  return true;
}

///////////////////
/// The event loop
//////////////////
void SHyFT::analyze(const edm::EventBase& iEvent)
{
  globalWeight_ = 1.0;

  pat::strbitset ret = wPlusJets.getBitTemplate();
  
  wPlusJets(iEvent, ret);
  std::vector<reco::ShallowClonePtrCandidate> const & electrons = wPlusJets.selectedElectrons();
  std::vector<reco::ShallowClonePtrCandidate> const & muons     = wPlusJets.selectedMuons();
  std::vector<reco::ShallowClonePtrCandidate> const & jets      = wPlusJets.cleanedJets();
  reco::ShallowClonePtrCandidate const & met = wPlusJets.selectedMET();
  
  string bit_;

  bit_ = "Trigger" ;
  bool passTrigger = ret[ bit_ ];
  // bit_ = "== 1 Lepton";
  // bit_  = "Z Veto";
  // bool passOneLepton = ret[ bit_ ];
  // bit_ = ">=1 Jets";
  // bool jet1 = ret[bit_];
  // bit_ = ">=2 Jets";
  // bool jet2 = ret[bit_];
  // bit_ = ">=3 Jets";
  // bool jet3 = ret[bit_];
  // bit_ = ">=4 Jets";
  // bool jet4 = ret[bit_];
  // bit_ = ">=5 Jets";
  // bool jet5 = ret[bit_];
  bit_ = "Cosmic Veto";
  bool passPre = ret[bit_];
  //cout << passPre << endl;
  // bool anyJets = jet1 || jet2 || jet3 || jet4 || jet5;
  
  // if not passed trigger, next event
  if ( !passTrigger )  return;
  
  if (doMC_ && reweightPDF_) {
    weightPDF(iEvent);
  }
  
  if(useHFcat_) {
    edm::Handle< unsigned int > heavyFlavorCategory;
    iEvent.getByLabel ( edm::InputTag("flavorHistoryFilter"),heavyFlavorCategory);
    assert ( heavyFlavorCategory.isValid() );
    if ( useHFcat_ ) theDir.getObject<TH1>( "flavorHistory")-> Fill ( *heavyFlavorCategory, globalWeight_ );
  }


  if (passPre) 
    {
      theDir.getObject<TH1>( "nJets")->Fill( jets.size(), globalWeight_ );
      //if(useHFcat_ && sampleHistName_ == sampleNameInput) return;
      make_templates(jets, met, muons, electrons);
      analyze_met( met );
      if ( muPlusJets_ ) analyze_muons(muons);
      if ( ePlusJets_ ) analyze_electrons(electrons);          

      if ( !doMC_) {
	summary_.push_back( SHyFTSummary(iEvent.id().run(),
					 iEvent.id().luminosityBlock(),
					 iEvent.id().event(),
					 allNumJets_,
					 allNumTags_
					 ) );
      }    
    }

}
  



void SHyFT::endJob()
{

  std::cout << "----------------------------------------------------------------------------------------" << std::endl;
  std::cout << "      So long, and thanks for all the fish..." << std::endl;
  std::cout << "                 -- " << identifier_ << std::endl;
  std::cout << "----------------------------------------------------------------------------------------" << std::endl;
  wPlusJets.print(std::cout);
  wPlusJets.printSelectors(std::cout);
  if ( !doMC_ ) {
    sort(summary_.begin(), summary_.end());
    std::cout << "** Start " << identifier_ << " **" << std::endl;
    copy(summary_.begin(), summary_.end(), std::ostream_iterator<SHyFTSummary>(std::cout, "\n"));  
    std::cout << "** End **" << std::endl;
  }
  std::cout << "  Btagging closure test: " << std::endl;
  std::cout << "  N_exp_>=1     : " << nExpectedTaggedEvents_ << std::endl;
  std::cout << "  N_obs_>=1     : " << nObservedTaggedEvents_ << std::endl;
  std::cout << "  N_obs jettags : " << nObservedTaggedJets_ << std::endl;
}


void SHyFT::weightPDF(  edm::EventBase const & iEvent) 
{
  // 
  // NOTA BENE!!!!
  //
  //     The values "pdf1" and "pdf2" below are *wrong* for madgraph or alpgen samples.
  //     They must be taken from the "zeroth" PDF in the PDF set, assuming that is set
    
  double iWeightSum = 0.0;
  edm::Handle<GenEventInfoProduct> pdfstuff;
  iEvent.getByLabel(pdfInputTag_, pdfstuff);
       
  float Q = pdfstuff->pdf()->scalePDF;
  int id1 = pdfstuff->pdf()->id.first;
  double x1 = pdfstuff->pdf()->x.first;
  int id2 = pdfstuff->pdf()->id.second;
  double x2 = pdfstuff->pdf()->x.second;

  // BROKEN for Madgraph productions:
  // double pdf1 = pdfstuff->pdf()->xPDF.first;
  // double pdf2 = pdfstuff->pdf()->xPDF.second;  

  LHAPDF::usePDFMember(1,0);
  double xpdf1 = LHAPDF::xfx(1, x1, Q, id1);
  double xpdf2 = LHAPDF::xfx(1, x2, Q, id2);
  double w0 = xpdf1 * xpdf2;

  for(int i=1; i <=44; ++i){
    LHAPDF::usePDFMember(1,i);
    double xpdf1_new = LHAPDF::xfx(1, x1, Q, id1);
    double xpdf2_new = LHAPDF::xfx(1, x2, Q, id2);
    double weight = xpdf1_new * xpdf2_new / w0;
    iWeightSum += weight*weight;
  }


  iWeightSum = TMath::Sqrt(iWeightSum) ;

  // char buff[1000];
  // sprintf(buff, "Q = %6.2f, id1 = %4d, id2 = %4d, x1 = %6.2f, x2 = %6.2f, pdf1 = %6.2f, pdf2 = %6.2f",
  // 	  Q, id1, id2, x1, x2, pdf1, pdf2
  // 	  );
  // std::cout << buff << std::endl;
  // sprintf(buff, "  New :  pdf1 = %6.2f, pdf2 = %6.2f, prod=%6.2f",
  // 	  newpdf1, newpdf2, prod
  // 	  );
  // std::cout << buff << std::endl;
  



    
  globalWeight_ *= iWeightSum ;
  // std::cout << "Global weight = " << globalWeight_ << std::endl;
  theDir.getObject<TH1>( "pdfWeight")->Fill( globalWeight_ );

}

