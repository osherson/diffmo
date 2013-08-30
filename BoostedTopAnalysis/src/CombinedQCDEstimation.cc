#include "Analysis/BoostedTopAnalysis/interface/CombinedQCDEstimation.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "DataFormats/PatCandidates/interface/MET.h"

using namespace std;

CombinedQCDEstimation::CombinedQCDEstimation( const edm::ParameterSet & iConfig,  TFileDirectory & iDir ) :
	theDir( iDir ),
	type22Selection_v1_     ( iConfig.getParameter<edm::ParameterSet>("Type22Selection") ),
	bTagOP_                 ( iConfig.getParameter<edm::ParameterSet>("Type22Selection").getParameter<double>("bTagOP") ),
	bTagAlgo_               ( iConfig.getParameter<edm::ParameterSet>("Type22Selection").getParameter<string>("bTagAlgo") ),
	jetPt0_					( iConfig.getParameter<edm::ParameterSet>("Type22Selection").getParameter<double>("jetPt0") ),
	jetPt1_					( iConfig.getParameter<edm::ParameterSet>("Type22Selection").getParameter<double>("jetPt1") ),
	jetEta_					( iConfig.getParameter<edm::ParameterSet>("Type22Selection").getParameter<double>("jetEta") ),
	wMassMin_               ( iConfig.getParameter<double>("wMassMin") ),
	wMassMax_               ( iConfig.getParameter<double>("wMassMax") ),
	topMassMin_             ( iConfig.getParameter<double>("topMassMin") ),
	topMassMax_             ( iConfig.getParameter<double>("topMassMax") ),
	mistagFileName_         ( iConfig.getParameter<string>("mistagFile") ),
	prob                    ( iConfig.getParameter<double>("Probability") ),
	runOnData_              ( iConfig.getParameter<bool>("runOnData") ),
	type11Selection_v1_     ( iConfig.getParameter<edm::ParameterSet>("Type11Selection") ),
	caTopJetPtMin_          ( iConfig.getParameter<edm::ParameterSet>("Type11Selection").getParameter<double>("caTopJetPtMin") ),
	caTopJetEtaCut_         ( iConfig.getParameter<edm::ParameterSet>("Type11Selection").getParameter<double>("caTopJetEtaCut") ),
	caTopJetMassMin_        ( iConfig.getParameter<double>("caTopJetMassMin") ),
	caTopJetMassMax_        ( iConfig.getParameter<double>("caTopJetMassMax") ),
	caTopMinMassMin_        ( iConfig.getParameter<double>("caTopMinMassMin") ),
	caTopMistagFileName_    ( iConfig.getParameter<string>("caTopMistagFileName") )
{
  std::cout << "Instantiated CombinedQCDEstimation" << std::endl;
	
  // Type11 histograms
  theDir.make<TH1F>("ttMassType11_measured",   "measured t#bar{t} Inv Mass Type11",   500,  0,  5000 );
 
  theDir.make<TH1F>("testPredict11_Sample_MinusPlus_PlusPlus",					"testPredict11_Sample_MinusPlus_PlusPlus",					500,  0,  5000 );
  theDir.make<TH1F>("testPredict11_Sample_MinusPlus_PlusPlus_reducedWeight",	"testPredict11_Sample_MinusPlus_PlusPlus_reducedWeight",	500,  0,  5000 );
  theDir.make<TH1F>("testPredict11_Sample_MinusPlus",							"testPredict11_Sample_MinusPlus",							500,  0,  5000 );
  theDir.make<TH1F>("testPredict11_Sample_MinusPlus_reducedWeight",				"testPredict11_Sample_MinusPlus",							500,  0,  5000 );
  theDir.make<TH1F>("testPredict11_Sample_MinusMinus",							"testPredict11_Sample_MinusMinus",							500,  0,  5000 );
  theDir.make<TH1F>("testPredict11_Sample_All",									"testPredict11_Sample_All",									500,  0,  5000 );
	
  // Type12 histograms	
  theDir.make<TH1F>("ttMassType12_measured",   "measured t#bar{t} Inv Mass Type12",   500,  0,  5000 );
  theDir.make<TH1F>("ttMassType12_predicted",  "predicted t#bar{t} Inv Mass Type12",  500,  0,  5000 );
  theDir.make<TH1F>("ttMassType12_predicted_errorSquared", "sum of squared errors t#bar{t} Inv Mass Type12",   500,  0,  5000 );

  // Type22 histograms
  theDir.make<TH1F>("ttMassType22_measured",   "t#bar{t} Inv Mass Type22",   500,  0,  5000 );
  theDir.make<TH1F>("ttMassType22_predicted",  "t#bar{t} Inv Mass Type22",   500,  0,  5000 );
  theDir.make<TH1F>("ttMassType22_predicted_errorSquared", "sum of squared errors t#bar{t} Inv Mass Type22",   500,  0,  5000 );
	
  theDir.make<TH1F>("topMassPred",    "Top Mass",									100,  0,  500 );

  // counting histograms
  theDir.make<TH1F>("Nevents_22_tighttight_tightloose",   "Nevents_22_tighttight_tightloose",   2,  0.5,  2.5 );
  theDir.make<TH1F>("Nevents_12_tight_loose",   "Nevents_12_tight_loose",   2,  0.5,  2.5 );
  theDir.make<TH1F>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg",   "Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg",   6,  0.5,  6.5 );
  theDir.make<TH1F>("Nevents_Type22_Case123",   "Nevents_Type22_Case123",   3,  0.5,  3.5 );
  theDir.make<TH1F>("Nevents_PassCuts",   "Nevents_PassCuts",   10,  0.5,  10.5 );
  theDir.make<TH1F>("Nevents_succesiveCuts_type12bkg",   "Nevents_succesiveCuts_type12bkg",   7,  0.5,  7.5 );
	
  theDir.make<TH1F>("Nevents_analyzed",   "Nevents_analyzed",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_preselected",   "Nevents_preselected",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_hasTaggedTopJet0",   "Nevents_hasTaggedTopJet0",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTaggedTopJet1",   "Nevents_hasTaggedTopJet1",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasZeroTopTags",   "Nevents_hasZeroTopTags",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneTopTag",   "Nevents_hasOneTopTag",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoTopTags",   "Nevents_hasTwoTopTags",   1,  0.5,  1.5 );

  theDir.make<TH1F>("Nevents_hasWTag0",   "Nevents_hasWTag0",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasWTag1",   "Nevents_hasWTag1",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasZeroWTags",   "Nevents_hasZeroWTags",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneWTag",   "Nevents_hasOneWTag",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoWTags",   "Nevents_hasTwoWTags",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_hasBTag0",   "Nevents_hasBTag0",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasBTag1",   "Nevents_hasBTag1",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasZeroBTags",   "Nevents_hasZeroBTags",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneBTag",   "Nevents_hasOneBTag",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoBTags",   "Nevents_hasTwoBTags",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_hasTightTop0",   "Nevents_hasTightTop0",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTightTop1",   "Nevents_hasTightTop1",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasZeroTightTops",   "Nevents_hasZeroTightTops",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneTightTop",   "Nevents_hasOneTightTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoTightTops",   "Nevents_hasTwoTightTops",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_hasLooseTop0",   "Nevents_hasLooseTop0",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasLooseTop1",   "Nevents_hasLooseTop1",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasZeroLooseTops",   "Nevents_hasZeroLooseTops",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneLooseTop",   "Nevents_hasOneLooseTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoLooseTops",   "Nevents_hasTwoLooseTops",   1,  0.5,  1.5 );

  theDir.make<TH1F>("Nevents_hasOneLooseOneTightTop",   "Nevents_hasOneLooseOneTightTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hemiWithWandB",   "Nevents_hemiWithWandB",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hemiWithWandUntagged",   "Nevents_hemiWithWandUntagged",   1,  0.5,  1.5 );

  theDir.make<TH1F>("Nevents_TopTagAndTightTop",   "Nevents_TopTagAndTightTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_TopTagAndLooseTop",   "Nevents_TopTagAndLooseTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_TopTagAndWandB",   "Nevents_TopTagAndWandB",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoWTags_hasOneBTag",   "Nevents_hasTwoWTags_hasOneBTag",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasTwoWTags_hasTwoBTags",   "Nevents_hasTwoWTags_hasTwoBTags",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_TightTopAndLooseTop",   "Nevents_TightTopAndLooseTop",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasOneTopJet_noNonLeadingBjet",   "Nevents_hasOneTopJet_noNonLeadingBjet",   1,  0.5,  1.5 );

  theDir.make<TH1F>("Nevents_type11sig",   "Nevents_type11sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_type12sig",   "Nevents_type12sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_type22sig",   "Nevents_type22sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_type11bkg",   "Nevents_type11bkg",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_type12bkg",   "Nevents_type12bkg",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_type22bkg",   "Nevents_type22bkg",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_cascade11sig",   "Nevents_cascade11sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_cascade12sig",   "Nevents_cascade12sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_cascade22sig",   "Nevents_cascade22sig",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_cascade11bkg",   "Nevents_cascade11bkg",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_cascade12bkg",   "Nevents_cascade12bkg",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_cascade22bkg",   "Nevents_cascade22bkg",   1,  0.5,  1.5 );
	
  theDir.make<TH1F>("Nevents_hemiTop_hemiBnoW",   "Nevents_hemiTop_hemiBnoW",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasBTag",   "Nevents_hasBTag",   1,  0.5,  1.5 );
  theDir.make<TH1F>("Nevents_hasBTag_noWTag",   "Nevents_hasBTag_noWTag",   1,  0.5,  1.5 );
  
  theDir.make<TH1F>("Nevents_type11sig_cutflow",   "Nevents_type11sig_cutflow",   20,  0.5,  20.5 );
  theDir.make<TH1F>("Nevents_type12sig_cutflow",   "Nevents_type12sig_cutflow",   20,  0.5,  20.5 );
  theDir.make<TH1F>("Nevents_type22sig_cutflow",   "Nevents_type22sig_cutflow",   20,  0.5,  20.5 );
	
  theDir.make<TH1F>("Nevents_type11bkg_cutflow",   "Nevents_type11bkg_cutflow",   20,  0.5,  20.5 );
  theDir.make<TH1F>("Nevents_type12bkg_cutflow",   "Nevents_type12bkg_cutflow",   20,  0.5,  20.5 );
  theDir.make<TH1F>("Nevents_type22bkg_cutflow",   "Nevents_type22bkg_cutflow",   20,  0.5,  20.5 );
	
	// pretagged histograms
  theDir.make<TH1F>("CATop_mass",				"CATop_mass",				100,	0,  400 );
  theDir.make<TH1F>("CATop_minmass",			"CATop_minmass",			100,	0,  200 );
  theDir.make<TH1F>("CATop_nsubjets",			"CATop_nsubjets",			5,		0,  5 );
  theDir.make<TH1F>("CATop_pt",					"CATop_pt",					100,	0,  4000 );
  theDir.make<TH1F>("CATop_eta",				"CATop_eta",				100,	-3,  3 );
  //theDir.make<TH2F>("CATop_mass_minmass",		"CATop_mass_minmass",		400,	0,  400,  400,	0,  200 );

  theDir.make<TH1F>("CATop_mass_masscut",		"CATop_mass_masscut",		100,	0,  200 );
  theDir.make<TH1F>("CATop_minmass_masscut",	"CATop_minmass_masscut",	100,	0,  200 );
  theDir.make<TH1F>("CATop_nsubjets_masscut",   "CATop_nsubjets_masscut",   5,		0,  5 );
  theDir.make<TH1F>("CATop_pt_masscut",			"CATop_pt_masscut",			100,	0,  4000 );
  theDir.make<TH1F>("CATop_eta_masscut",		"CATop_eta_masscut",		100,	-3,  3 );
  
  theDir.make<TH1F>("CATop_mass_minmasscut",		"CATop_mass_minmasscut",		100,	0,  200 );
  theDir.make<TH1F>("CATop_minmass_minmasscut",		"CATop_minmass_minmasscut",		100,	0,  200 );
  theDir.make<TH1F>("CATop_nsubjets_minmasscut",   	"CATop_nsubjets_minmasscut",   	5,		0,  5 );
  theDir.make<TH1F>("CATop_pt_minmasscut",			"CATop_pt_minmasscut",			100,	0,  4000 );
  theDir.make<TH1F>("CATop_eta_minmasscut",			"CATop_eta_minmasscut",			100,	-3,  3 );
	
  theDir.make<TH1F>("CATop_mass_masscut_nsubjetscut",		"CATop_mass_masscut_nsubjetscut",		100,	0,  200 );
  theDir.make<TH1F>("CATop_minmass_masscut_nsubjetscut",	"CATop_minmass_masscut_nsubjetscut",	100,	0,  200 );
  theDir.make<TH1F>("CATop_nsubjets_masscut_nsubjetscut",   "CATop_nsubjets_masscut_nsubjetscut",   5,		0,  5 );
  theDir.make<TH1F>("CATop_pt_masscut_nsubjetscut",			"CATop_pt_masscut_nsubjetscut",			100,	0,  4000 );
  theDir.make<TH1F>("CATop_eta_masscut_nsubjetscut",		"CATop_eta_masscut_nsubjetscut",		100,	-3,  3 );


  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_mass",		"CATopUntagged_atLeast1Tag_mass",		100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_minmass",	"CATopUntagged_atLeast1Tag_minmass",	100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_nsubjets",   "CATopUntagged_atLeast1Tag_nsubjets",   5,		0,  5 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_pt",			"CATopUntagged_atLeast1Tag_pt",			100,	0,  4000 );
  theDir.make<TH1F>("ttMassType11_atLeast1Tag",		"ttMassType11_atLeast1Tag", 500,  0,  5000);

  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_mass",		"CATopUntagged_atLeast1Tag_masscut_mass",		100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_minmass",	"CATopUntagged_atLeast1Tag_masscut_minmass",	100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_nsubjets",   "CATopUntagged_atLeast1Tag_masscut_nsubjets",   5,		0,  5 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_pt",			"CATopUntagged_atLeast1Tag_masscut_pt",			100,	0,  4000 );
  theDir.make<TH1F>("ttMassType11_atLeast1Tag_masscut",		"ttMassType11_atLeast1Tag_masscut", 500,  0,  5000);

 theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_mass",       "CATopUntagged_atLeast1Tag_masscut_nsubjetscut_mass",       100,    0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_minmass",    "CATopUntagged_atLeast1Tag_masscut_nsubjetscut_minmass",    100,    0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_nsubjets",   "CATopUntagged_atLeast1Tag_masscut_nsubjetscut_nsubjets",   5,      0,  5 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_pt",         "CATopUntagged_atLeast1Tag_masscut_nsubjetscut_pt",         100,    0,  4000 );
  theDir.make<TH1F>("ttMassType11_atLeast1Tag_masscut_nsubjetscut",     "ttMassType11_atLeast1Tag_masscut_nsubjetscut", 500,  0,  5000);


  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_minmasscut_mass",		"CATopUntagged_atLeast1Tag_minmasscut_mass",		100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_minmasscut_minmass",	"CATopUntagged_atLeast1Tag_minmasscut_minmass",	100,	0,  200 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_minmasscut_nsubjets",   "CATopUntagged_atLeast1Tag_minmasscut_nsubjets",   5,		0,  5 );
  theDir.make<TH1F>("CATopUntagged_atLeast1Tag_minmasscut_pt",			"CATopUntagged_atLeast1Tag_minmasscut_pt",			100,	0,  4000 );
  theDir.make<TH1F>("ttMassType11_atLeast1Tag_minmasscut",		"ttMassType11_atLeast1Tag_minmasscut", 500,  0,  5000);


  theDir.make<TH1F>("PrunedJet_mass",				"PrunedJet_mass",		100,	0,  400 );
  theDir.make<TH1F>("PrunedJet_mu",					"PrunedJet_mu",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_dR",					"PrunedJet_dR",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_y",					"PrunedJet_y",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_nsubjets",			"PrunedJet_nsubjets",	5,		0,  5 );
  theDir.make<TH1F>("PrunedJet_pt",					"PrunedJet_pt",			100,	0,  4000 );
  theDir.make<TH1F>("PrunedJet_eta",				"PrunedJet_eta",		100,	-3,  3 );
  //theDir.make<TH2F>("PrunedJet_mass_mu",			"PrunedJet_mass_mu",	400,	0,  400,  400,	0,  1 );
  //theDir.make<TH2F>("PrunedJet_pt_mu",				"PrunedJet_pt_mu",		400,	0,  4000,  400,	0,  1 );

  theDir.make<TH1F>("PrunedJet_mass_masscut",				"PrunedJet_mass_masscut",		100,	0,  400 );
  theDir.make<TH1F>("PrunedJet_mu_masscut",					"PrunedJet_mu_masscut",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_nsubjets_masscut",			"PrunedJet_nsubjets_masscut",	5,		0,  5 );
  theDir.make<TH1F>("PrunedJet_pt_masscut",					"PrunedJet_pt_masscut",			100,	0,  4000 );
  theDir.make<TH1F>("PrunedJet_eta_masscut",				"PrunedJet_eta_masscut",		100,	-3,  3 );
  theDir.make<TH1F>("PrunedJet_dR_masscut",					"PrunedJet_dR_masscut",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_y_masscut",					"PrunedJet_y_masscut",			100,	0,  1 );
	
  theDir.make<TH1F>("PrunedJet_mass_mucut",				"PrunedJet_mass_mucut",		100,	0,  400 );
  theDir.make<TH1F>("PrunedJet_mu_mucut",				"PrunedJet_mu_mucut",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_nsubjets_mucut",		"PrunedJet_nsubjets_mucut",	5,		0,  5 );
  theDir.make<TH1F>("PrunedJet_pt_mucut",				"PrunedJet_pt_mucut",			100,	0,  4000 );
  theDir.make<TH1F>("PrunedJet_eta_mucut",				"PrunedJet_eta_mucut",		100,	-3,  3 );
  theDir.make<TH1F>("PrunedJet_dR_mucut",					"PrunedJet_dR_mucut",			100,	0,  1 );
  theDir.make<TH1F>("PrunedJet_y_mucut",					"PrunedJet_y_mucut",			100,	0,  1 );


	
  theDir.make<TH1F>("PrunedJet_mu_taggedType1",	"PrunedJet_mu_taggedType1",		100,	0,  1 );
  theDir.make<TH1F>("CATop_minmass_taggedType2","CATop_minmass_taggedType2",	100,	0,  200 );
	
  std::cout << "Instantiated histograms" << std::endl;

  // Input histograms
  mistagFile_   =  TFile::Open( mistagFileName_.c_str() );
  wMistag_      =  (TH1F*)mistagFile_       ->  Get("wMistag");
  caTopMistagFile_   =  TFile::Open( caTopMistagFileName_.c_str() );
  topMistag_      =  (TH1F*)caTopMistagFile_       ->  Get("MISTAG_RATE");
  std::cout << "Input histograms" << std::endl;

  //use the PredictedDistrubution class to get correct error
  ttMassPred11  =  new PredictedDistribution( (TH1D*)topMistag_ , "ttMassPred11", "t#bar{t} Inv Mass",  500,  0,  5000 );
  ttMassPred12  =  new PredictedDistribution( (TH1D*)wMistag_ , "ttMassPred12", "t#bar{t} Inv Mass",  500,  0,  5000 );
  ttMassPred22  =  new PredictedDistribution( (TH1D*)wMistag_ , "ttMassPred22", "t#bar{t} Inv Mass",  500,  0,  5000 );
  std::cout << "PredictedDistrubution" << std::endl;
	
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "Module requires the RandomNumberGeneratorService\n";
  }
	
  CLHEP::HepRandomEngine& engine = rng->getEngine();
  flatDistribution_ = new CLHEP::RandFlat(engine, 0., 1.);
}

void CombinedQCDEstimation::analyze( const edm::EventBase & iEvent )
{	
  bool verbose_ = false;
  int run = iEvent.id().run();
  int event = iEvent.id().event();
  int lumi = iEvent.id().luminosityBlock(); 
  if (verbose_)cout<<"\nAnalyze event "<<iEvent.id()<<endl;

  theDir.getObject<TH1>("Nevents_PassCuts")->Fill(1);
  theDir.getObject<TH1>("Nevents_analyzed")->Fill(1);
 
  //////////////////////////////////
  // Generator weights
	
  double evtWeight = 1.0;
	
  edm::Handle<GenEventInfoProduct>    genEvt;
  iEvent.getByLabel( edm::InputTag("generator"),  genEvt );
  if( genEvt.isValid() )  {
	evtWeight = genEvt->weight() ;
  }
  if (verbose_)cout<<"generator evtWeight = "<<evtWeight<<endl;

  //////////////////////////////////
  // Make jet hemispheres
	
  pat::strbitset   retType11 = type11Selection_v1_.getBitTemplate();
  bool passType11 = type11Selection_v1_( iEvent, retType11 );
  std::vector<edm::Ptr<pat::Jet> >  const &  caTopJets_ = type11Selection_v1_.caTopJets();
  std::vector<edm::Ptr<pat::Jet> >  const &  ca8Jets_ = type11Selection_v1_.ca8Jets();
	
  pat::strbitset   retType22 = type22Selection_v1_.getBitTemplate();
  bool passType22 = type22Selection_v1_( iEvent, retType22 );
  std::vector<edm::Ptr<pat::Jet> >  const &  pfJets_ = type22Selection_v1_.pfJets();
	
  wJetSelector_  = &(type22Selection_v1_.wJetSelector() );

  if (verbose_)cout<<"Selector results: passType11 = "<<passType11<<"  passType22 = "<<passType22<<endl;
  if (verbose_&&passType11) cout<<"  ca8Jets_.size() = "<<ca8Jets_.size()<<endl;
  if (verbose_&&passType11) cout<<"  caTopJets_.size() = "<<caTopJets_.size()<<endl;
  if (verbose_&&passType22) cout<<"  pfJets_.size() = "<<pfJets_.size()<<endl;
	
  //if (passType11 && ca8Jets_.size() >1 ) verbose_=true;

//check
if (passType11)
{
	int countMassiveJets=0;
	for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=ca8Jets_.begin(), jetEnd=ca8Jets_.end(), ica8=jetBegin ;
       ica8!=jetEnd; ica8++ )
    {
      pat::Jet const & ca8Jet = **ica8;
	  if (ca8Jet.mass()>200) countMassiveJets++; 
    }
	if (countMassiveJets > 4 )
	{
		cout<<"WTF ! THIS EVENT HAS MORE THAN 4 MASSIVE JETS : "<<iEvent.id()<<endl; 
		verbose_ = true;
	}

}


  if (verbose_&&passType11)
  {
cout<<"Print ca8Jets info - JEC"<<endl;
	int count=0;
    for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=ca8Jets_.begin(), jetEnd=ca8Jets_.end(), ica8=jetBegin ;
       ica8!=jetEnd; ica8++ )
    {
      pat::Jet const & ca8Jet = **ica8;
      //cout<<"  eta "<<ca8Jet.eta()<<" phi "<<ca8Jet.phi()<<
	  
		cout<<"Jet "<<count<<endl;
		cout<<" corrected pt "<<ca8Jet.pt()<<endl;
		cout<<" uncorrected pt "<<ca8Jet.correctedJet("Uncorrected").pt()<<endl;
		cout<<" L2Relative corrected pt "<<ca8Jet.correctedJet("L2Relative").pt()<<endl;
		cout<<" L3Absolute corrected pt "<<ca8Jet.correctedJet("L3Absolute").pt()<<endl;

      //cout<<" CHEF "<<ca8Jet.chargedHadronEnergyFraction()<<" CHE "<<ca8Jet.chargedHadronEnergy();
      //cout<<" NHEF "<<ca8Jet.neutralHadronEnergyFraction()<<" NHE "<<ca8Jet.neutralHadronEnergy();
      //cout<<" CEMEF "<<ca8Jet.chargedEmEnergyFraction()<<" NEMEF"<<ca8Jet.neutralEmEnergyFraction();
      //cout<<" photonEF "<<ca8Jet.photonEnergyFraction()<<" muonEF"<<ca8Jet.muonEnergyFraction();
      //cout<<" nConst "<<ca8Jet.nConstituents()<<endl;
    count++;
	}


    cout<<"Print ca8Jets info"<<endl;
	for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=ca8Jets_.begin(), jetEnd=ca8Jets_.end(), ica8=jetBegin ;
	   ica8!=jetEnd; ica8++ ) 
    {
      pat::Jet const & ca8Jet = **ica8;
      cout<<"  eta "<<ca8Jet.eta()<<" phi "<<ca8Jet.phi()<<" pt "<<ca8Jet.pt()<<" mass "<<ca8Jet.mass()<<" energy "<<ca8Jet.energy();
	  cout<<" uncorr pt "<<ca8Jet.correctedJet("Uncorrected").pt()<<" uncorr mass "<<ca8Jet.correctedJet("Uncorrected").mass();
	  cout<<" CHEF "<<ca8Jet.chargedHadronEnergyFraction()<<" CHE "<<ca8Jet.chargedHadronEnergy();
	  cout<<" NHEF "<<ca8Jet.neutralHadronEnergyFraction()<<" NHE "<<ca8Jet.neutralHadronEnergy();
	  cout<<" CEMEF "<<ca8Jet.chargedEmEnergyFraction()<<" NEMEF"<<ca8Jet.neutralEmEnergyFraction();
	  //cout<<" photonEF "<<ca8Jet.photonEnergyFraction()<<" muonEF"<<ca8Jet.muonEnergyFraction();
	  cout<<" nConst "<<ca8Jet.nConstituents()<<endl;
    }
	cout<<"Print caTopJet info"<<endl;
	for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=caTopJets_.begin(), jetEnd=caTopJets_.end(), icatop=jetBegin ;
	   icatop!=jetEnd; icatop++ ) 
	{
	  pat::Jet const & caTopJet = **icatop;
			
	  cout<<"  eta "<<caTopJet.eta()<<" phi "<<caTopJet.phi()<<" pt "<<caTopJet.pt()<<" mass "<<caTopJet.mass()<<"  energy "<<caTopJet.energy()<<endl;
	}
  }
  if (verbose_&&passType22)
  {
    cout<<"Print prunedJet info"<<endl;
    for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=pfJets_.begin(), jetEnd=pfJets_.end(), ijet=jetBegin ;
       ijet!=jetEnd; ijet++ ) 
    {
      pat::Jet const & jet = **ijet;
      bool  wtagged = false;
      bool  btagged = false;
	  bool  cajet_btagged = false;
	  pat::strbitset iret = wJetSelector_->getBitTemplate();
	  wtagged = wJetSelector_->operator()( jet, iret );
	  bool passWMass = (jet.mass() > wMassMin_ ) && (jet.mass() < wMassMax_ );
	  btagged = (jet.bDiscriminator( bTagAlgo_ ) > bTagOP_ );
	  cout<<"  eta "<<jet.eta()<<" phi "<<jet.phi()<<" pt "<<jet.pt()<<" mass "<<jet.mass()
	      <<" wtagged? "<<wtagged<<" btagged? "<<btagged
	      <<" energy "<<jet.energy()<<" px "<<jet.px()<<" py "<<jet.py()<<" jet.pz() "<<jet.pz()<<endl;			
	}
  }
	
  // Define two hemispheres in deltaphi. hemisphere0 is centered on the leading unpruned CA8 jet. hemisphere1 is opposite. Group caTop and pruned jets into these hemispheres.
  if (verbose_) cout<<"Group jets into hemispheres"<<endl;
  if (verbose_) cout<<"passType11  "<<passType11<<endl;
  if (verbose_) cout<<"passType22  "<<passType22<<endl;
  if (verbose_) cout<<"ca8Jets_.size() "<<ca8Jets_.size()<<endl;
  if( passType11 && passType22 && ca8Jets_.size()>=2 )  
  {	
  	if (verbose_) cout<<"pass passType11 && passType22 && ca8Jets_.size()>=2  "<<endl;
    theDir.getObject<TH1>("Nevents_PassCuts")->Fill(2);
	
    //Put jets in the proper hemisphere
    pat::Jet const & leadJet = *ca8Jets_.at(0);
    std::vector<edm::Ptr<pat::Jet> >  hemisphere0_ca8, hemisphere1_ca8;
	std::vector<edm::Ptr<pat::Jet> >  hemisphere0, hemisphere1;
	std::vector<edm::Ptr<pat::Jet> >  hemisphere0_catop, hemisphere1_catop;
	std::vector<edm::Ptr<pat::Jet> >  topTags0, topTags1;
	std::vector<edm::Ptr<pat::Jet> >  wTags0,   wTags1;
	std::vector<edm::Ptr<pat::Jet> >  bTags0,   bTags1;
	std::vector<edm::Ptr<pat::Jet> >  noTags0,  noTags1;
	pat::Jet const * aJet0=NULL;
	pat::Jet const * aJet1=NULL;
		
	for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=ca8Jets_.begin(), jetEnd=ca8Jets_.end(), ica8=jetBegin ;
	   ica8!=jetEnd; ica8++ ) 
	{
	  pat::Jet const & ca8Jet = **ica8;
	  if (verbose_) cout<<" ca8Jet eta "<<ca8Jet.eta()<<" phi "<<ca8Jet.phi()<<" pt "<<ca8Jet.pt()<<" mass "<<ca8Jet.mass()<<endl;
			
	  //Group the ca8 jets into hemispheres
	  double deltaPhi_leadJet_ca8Jet = fabs( reco::deltaPhi<double>( leadJet.phi(), ca8Jet.phi() ) );
	  if( deltaPhi_leadJet_ca8Jet < TMath::Pi()/2 ) 
	  {
	    hemisphere0_ca8.push_back( *ica8 );	
		if (verbose_) cout<<"   -> hemisphere0_ca8. deltaPhi = "<< deltaPhi_leadJet_ca8Jet <<endl;
				
		// Match caTop to ca8 jets and group into hemispheres
		for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=caTopJets_.begin(), jetEnd=caTopJets_.end(), icatop=jetBegin ;
		   icatop!=jetEnd; icatop++ ) 
		{
		  pat::Jet const & caTopJet = **icatop;
		  double deltaR_ca8Jet_caTopJet = deltaR( ca8Jet.eta(), ca8Jet.phi(), caTopJet.eta(), caTopJet.phi() );
					
		  //match caTop jet to ca8 jet
		  if( deltaR_ca8Jet_caTopJet < 0.1 ) 
		  {
		    if (verbose_) cout<<"   -> found matching caTopJet eta "
					<< caTopJet.eta()<<" phi "<<caTopJet.phi()<<" pt "<<caTopJet.pt()<<" mass "<<caTopJet.mass()
					<<" deltaR "<<deltaR_ca8Jet_caTopJet<<endl;
			hemisphere0_catop.push_back( *icatop );	
			if (verbose_) cout<<"     -> hemisphere0_catop. "<<endl;
		  }
		}
	      // Match pruned jets to ca8 jets and group into hemispheres
		for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=pfJets_.begin(), jetEnd=pfJets_.end(), ijet=jetBegin ;
		   ijet!=jetEnd; ijet++ ) 
		{
		  pat::Jet const & prunedJet = **ijet;				
		  bool  wtagged = false;
		  bool  btagged = false;
		  pat::strbitset iret = wJetSelector_->getBitTemplate();
		  wtagged = wJetSelector_->operator()( prunedJet, iret );
		  bool passWMass = (prunedJet.mass() > wMassMin_ ) && (prunedJet.mass() < wMassMax_);
		  btagged = (prunedJet.bDiscriminator( bTagAlgo_ ) > bTagOP_ );
		  double deltaR_ca8Jet_prunedJet = deltaR( ca8Jet.eta(), ca8Jet.phi(), prunedJet.eta(), prunedJet.phi() );
					
		  //match pruned jet to catop jet
		  if( deltaR_ca8Jet_prunedJet < 0.1 ) 
		  {	
			if (verbose_) cout<<"   -> found matching prunedJet eta "
					<< prunedJet.eta()<<" phi "<<prunedJet.phi()<<" pt "<<prunedJet.pt()<<" mass "<<prunedJet.mass()
					<<" deltaR "<<deltaR_ca8Jet_prunedJet<<endl;
						
			hemisphere0.push_back( *ijet );
						
			if( wtagged && passWMass  && prunedJet.pt() > jetPt0_ ) 
			  wTags0.push_back( *ijet );
			else if ( btagged )
			  bTags0.push_back( *ijet );
			else
			  noTags0.push_back( *ijet );												
		  }
		}//pruned jet loop
	  }//deltaphi requirement to define hemisphere0
	  else
	  {
	    hemisphere1_ca8.push_back( *ica8 );	
		if (verbose_) cout<<"   -> hemisphere1_ca8. deltaPhi = "<< deltaPhi_leadJet_ca8Jet<<endl;
				
		// Match caTop to ca8 jets and group into hemispheres
		for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=caTopJets_.begin(), jetEnd=caTopJets_.end(), icatop=jetBegin ;
		   icatop!=jetEnd; icatop++ ) 
		{
		  pat::Jet const & caTopJet = **icatop;
		  double deltaR_ca8Jet_caTopJet = deltaR( ca8Jet.eta(), ca8Jet.phi(), caTopJet.eta(), caTopJet.phi() );
					
		  //match caTop jet to ca8 jet
		  if( deltaR_ca8Jet_caTopJet < 0.1 ) 
		  {
		    if (verbose_) cout<<"   -> found matching caTopJet eta "
					<< caTopJet.eta()<<" phi "<<caTopJet.phi()<<" pt "<<caTopJet.pt()<<" mass "<<caTopJet.mass()
					<<" deltaR "<<deltaR_ca8Jet_caTopJet<<endl;
			hemisphere1_catop.push_back( *icatop );	
			if (verbose_) cout<<"     -> hemisphere1_catop. "<<endl;
		  }
		}//caTop jet loop
	      
		// Match pruned jets to ca8 jets and group into hemispheres
		for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=pfJets_.begin(), jetEnd=pfJets_.end(), ijet=jetBegin ;
		   ijet!=jetEnd; ijet++ ) 
		{
		  pat::Jet const & prunedJet = **ijet;				
		  bool  wtagged = false;
		  bool  btagged = false;
		  pat::strbitset iret = wJetSelector_->getBitTemplate();
		  wtagged = wJetSelector_->operator()( prunedJet, iret );
		  bool passWMass = (prunedJet.mass() > wMassMin_ ) && (prunedJet.mass() < wMassMax_ );
		  btagged = (prunedJet.bDiscriminator( bTagAlgo_ ) > bTagOP_ );
					
		  double deltaR_ca8Jet_prunedJet = deltaR( ca8Jet.eta(), ca8Jet.phi(), prunedJet.eta(), prunedJet.phi() );
					
		  //match pruned jet to catop jet
		  if( deltaR_ca8Jet_prunedJet < 0.1 ) 
		  {	
			if (verbose_) cout<<"   -> found matching prunedJet eta "
					<< prunedJet.eta()<<" phi "<<prunedJet.phi()<<" pt "<<prunedJet.pt()<<" mass "<<prunedJet.mass()<<" deltaR "
					<< deltaR_ca8Jet_prunedJet<<endl;
						
			hemisphere1.push_back( *ijet );
						
			if( wtagged && passWMass && prunedJet.pt() > jetPt0_ ) 
			  wTags1.push_back( *ijet );
			else if ( btagged )
			  bTags1.push_back( *ijet );
			else
			  noTags1.push_back( *ijet );												
		  }
		}//pruned jet loop
	  }//define hemisphere1
	}//ca8 jet loop
      
	if (verbose_) cout<<"  hemisphere0_ca8.size() "<<hemisphere0_ca8.size()<<endl;
	if (verbose_) cout<<"  hemisphere1_ca8.size() "<<hemisphere1_ca8.size()<<endl;
	if (verbose_) cout<<"  hemisphere0_catop.size() "<<hemisphere0_catop.size()<<endl;
	if (verbose_) cout<<"  hemisphere1_catop.size() "<<hemisphere1_catop.size()<<endl;
	if (verbose_) cout<<"  hemisphere0.size() "<<hemisphere0.size()<<endl;
	if (verbose_) cout<<"  hemisphere1.size() "<<hemisphere1.size()<<endl;
		
	if( wTags0.size() >= 1 )  {
	  if (verbose_) cout<<"Find untagged jet clostest to Wjet0"<<endl;
	  double minDr = 999999. ;
	  for(size_t i=0; i<noTags0.size(); i++ ) {
	    double dR = reco::deltaR<double>( wTags0.at(0)->eta(), wTags0.at(0)->phi(),
					    noTags0.at(i)->eta(), noTags0.at(i)->phi() );
	    if( dR < minDr )  {
	      aJet0 = &(*noTags0.at(i));
	      minDr = dR;
	    }
	  }//noTags0 jet loop
	  if (verbose_ && noTags0.size()>0 ) 
	    cout<<"  wJet0 eta "<<wTags0.at(0)->eta()<<" wJet0 phi "<<wTags0.at(0)->phi()
	      <<" closest jet eta "<<aJet0->eta()<<" phi "<<aJet0->phi()<<endl;
	}//if wTags0 not empty
		
	if( wTags1.size() >= 1 )  {
	  if (verbose_) cout<<"Find untagged jet clostest to Wjet1"<<endl;
	  double minDr = 999999. ;
	  for( size_t i=0; i<noTags1.size(); i++ )  {
	    double dR = reco::deltaR<double>( wTags1.at(0)->eta(), wTags1.at(0)->phi(),
					    noTags1.at(i)->eta(), noTags1.at(i)->phi() );
	    if( dR < minDr )  {
	      aJet1 = &(*noTags1.at(i));
	      minDr = dR;
	    }
	  }//noTags1 jet loop
	  if (verbose_ && noTags1.size()>0 ) 
	    cout<<"  wJet1 eta "<<wTags1.at(0)->eta()<<" wJet1 phi "<<wTags1.at(0)->phi()
	      <<" closest jet eta "<<aJet1->eta()<<" phi "<<aJet1->phi()<<endl;
	}//if wTags1 not empty
	
	if (verbose_) cout<<"about to start preselection "<<endl;
    ////////////////////////////
	// Preselection
	bool preselected_event=false;
	if ( hemisphere0_ca8.size()>0 && hemisphere1_ca8.size()>0 )
	{
	  pat::Jet const & ca8_0 = *hemisphere0_ca8.at(0);
	  pat::Jet const & ca8_1 = *hemisphere1_ca8.at(0);
	  if ( ca8_0.pt() > jetPt0_ && ca8_1.pt() > jetPt0_ && fabs(ca8_0.eta()) < jetEta_ && fabs(ca8_1.eta()) < jetEta_ )
	  {
	    preselected_event = true;
		if (verbose_) 
		  cout<<"Event passes preselection (pt>"<<jetPt0_<<" && |eta|<"<<jetEta_<<").  pt0 "<<ca8_0.pt()
		    <<" pt1 "<<ca8_1.pt()<<" eta0 "<<ca8_0.eta()<<" eta1 "<<ca8_1.eta()<<endl;
	  }
	  else 
	  {
	    if (verbose_) 
		  cout<<"Event fails preselection! (pt<"<<jetPt0_<<" || |eta|>"<<jetEta_<<").  pt0 "<<ca8_0.pt()
		    <<" pt1 "<<ca8_1.pt()<<" eta0 "<<ca8_0.eta()<<" eta1 "<<ca8_1.eta()<<endl;
	  }
	}//if both hemispheres are not empty
		
	if (preselected_event)
	{			
	  //////////////////////////////////
	  // Setup Type 1
	  if (verbose_) cout<<"Setup Type 1"<<endl;

	  reco::Candidate::LorentzVector p4_catop_jet0;
	  reco::Candidate::LorentzVector p4_catop_jet1;
	  double j0_minmass=-99;
	  double j1_minmass=-99;
	  double j0_nsubjets=-99;
	  double j1_nsubjets=-99;
			
	  if ( hemisphere0_catop.size()>0 )
	  {			
	    pat::Jet const & catop0 = *hemisphere0_catop.at(0);
		p4_catop_jet0 = catop0.p4();
		std::vector<const reco::Candidate *>  catop0_subjets = catop0.getJetConstituentsQuick();
				
		if (verbose_) 
		  cout<<"  catop0  pt "<<catop0.pt()<<" eta "<<catop0.eta()<<" phi "
		    <<catop0.phi()<<"  Nsubjets "<<catop0_subjets.size() <<endl;
				
		if ( catop0_subjets.size() >=3)
		{	
		  int subjetLoopCount=0;
		  math::XYZTLorentzVector pairwiseMass01;
		  math::XYZTLorentzVector pairwiseMass02;
		  math::XYZTLorentzVector pairwiseMass12;
		  math::XYZTLorentzVector pairwiseMassAll;
					
		  for (std::vector<const reco::Candidate *>::iterator subjetIt = catop0_subjets.begin(); subjetIt != catop0_subjets.end(); subjetIt++)
		  {					
			reco::Candidate const * subjetCand =  (*subjetIt);
			reco::PFJet const * pfSubjet = dynamic_cast<reco::PFJet const *>(subjetCand);  
						
			if (verbose_) cout<<"jet 1 subjet "<<subjetLoopCount<<" mass = "<<pfSubjet->mass();			
			if (subjetLoopCount==0 || subjetLoopCount==1) pairwiseMass01 += pfSubjet->p4();
			if (subjetLoopCount==0 || subjetLoopCount==2) pairwiseMass02 += pfSubjet->p4();
			if (subjetLoopCount==1 || subjetLoopCount==2) pairwiseMass12 += pfSubjet->p4();
			subjetLoopCount++;
			  pairwiseMassAll+= pfSubjet->p4();
		  }
					
		  if (verbose_) cout<<"mass of all subjets jet0 = "<<pairwiseMassAll.mass()<<endl;
		  double min2 = std::min(pairwiseMass01.mass(), pairwiseMass02.mass() );
		  j0_minmass = std::min(min2, pairwiseMass12.mass() );
		  j0_nsubjets = subjetLoopCount;
		}//end if jet0 nsubjets>=3
              
		if (verbose_){  
		  cout<<"Check subjets sorting "<<endl;
		  if( catop0_subjets.size() == 4 )  {
		    cout<<catop0_subjets.at(0)->pt()<<"\t"<<catop0_subjets.at(1)->pt()<<"\t"<<catop0_subjets.at(2)->pt()<<"\t"
			  <<catop0_subjets.at(3)->pt()<<endl;
		  }
		  if( catop0_subjets.size() == 3 ) {
		    cout<<catop0_subjets.at(0)->pt()<<"\t"<<catop0_subjets.at(1)->pt()<<"\t"<<catop0_subjets.at(2)->pt()<<endl;
		  }
		}
	  }//end hemi0 not empty
			
	  if ( hemisphere1_catop.size()>0 )
	    {
	      pat::Jet const & catop1 = *hemisphere1_catop.at(0);
	      p4_catop_jet1 = catop1.p4();
	      std::vector<const reco::Candidate *>  catop1_subjets = catop1.getJetConstituentsQuick();
				
	      if (verbose_) 
		cout<<"  catop1  pt "<<catop1.pt()<<" eta "<<catop1.eta()<<" phi "
		    <<catop1.phi()<<"  Nsubjets "<<catop1_subjets.size() <<endl;

	      if ( catop1_subjets.size() >=3)
		{	
		  int subjetLoopCount=0;
		  math::XYZTLorentzVector pairwiseMass01;
		  math::XYZTLorentzVector pairwiseMass02;
		  math::XYZTLorentzVector pairwiseMass12;
		  math::XYZTLorentzVector pairwiseMassAll;
					
		  for (std::vector<const reco::Candidate *>::iterator subjetIt = catop1_subjets.begin(); subjetIt != catop1_subjets.end(); subjetIt++)
		    {					
		      reco::Candidate const * subjetCand =  (*subjetIt);
		      reco::PFJet const * pfSubjet = dynamic_cast<reco::PFJet const *>(subjetCand);  
			if (verbose_) cout<<"jet 1 subjet "<<subjetLoopCount<<" mass = "<<pfSubjet->mass();			
		      if (subjetLoopCount==0 || subjetLoopCount==1) pairwiseMass01 += pfSubjet->p4();
		      if (subjetLoopCount==0 || subjetLoopCount==2) pairwiseMass02 += pfSubjet->p4();
		      if (subjetLoopCount==1 || subjetLoopCount==2) pairwiseMass12 += pfSubjet->p4();
		      subjetLoopCount++;
			  pairwiseMassAll+= pfSubjet->p4();
		    }
		   if (verbose_) cout<<"mass of all subjets jet 1"<<pairwiseMassAll.mass()<<endl;
		  double min2 = std::min(pairwiseMass01.mass(), pairwiseMass02.mass() );
		  j1_minmass = std::min(min2, pairwiseMass12.mass() );
		  j1_nsubjets = subjetLoopCount;
					
		}//end if jet1 nsubjets>=3
	    }//end hemi1 notempty
			
	  

	  if (verbose_) 
	    cout<<"    p4_catop_jet0.mass() "<<p4_catop_jet0.mass()<<"  j0_minmass "<<j0_minmass<<"  j0_nsubjets "<<j0_nsubjets<<endl;
	  if (verbose_) 
	    cout<<"    p4_catop_jet1.mass() "<<p4_catop_jet1.mass()<<"  j1_minmass "<<j1_minmass<<"  j1_nsubjets "<<j0_nsubjets<<endl;
		
	  bool hasTaggedTopJet0=false;
	  bool hasTaggedTopJet1=false;
	  if ( p4_catop_jet0.mass() > caTopJetMassMin_ && 
	       p4_catop_jet0.mass() < caTopJetMassMax_ && 
	       j0_minmass > caTopMinMassMin_ && 
	       j0_nsubjets>2 && 
	       p4_catop_jet0.pt() > caTopJetPtMin_ && 
	       fabs(p4_catop_jet0.eta()) < caTopJetEtaCut_ ){
	    hasTaggedTopJet0=true;
	  }
	  if ( p4_catop_jet1.mass() > caTopJetMassMin_ &&
	       p4_catop_jet1.mass() < caTopJetMassMax_ && 
	       j1_minmass > caTopMinMassMin_ && 
	       j1_nsubjets>2 && 
	       p4_catop_jet1.pt() > caTopJetPtMin_ && 
	       fabs(p4_catop_jet1.eta()) < caTopJetEtaCut_ ){
	    hasTaggedTopJet1=true;
	  }

	  if (verbose_ && hasTaggedTopJet0 ) cout<<" hasTaggedTopJet0"<<endl;
	  if (verbose_ && hasTaggedTopJet1 ) cout<<" hasTaggedTopJet1"<<endl;	

	  //-----------------
	  // Type 1 commissioning plots

	  theDir.getObject<TH1>("CATop_mass")		->Fill( p4_catop_jet0.mass(), evtWeight);
	  theDir.getObject<TH1>("CATop_mass")		->Fill( p4_catop_jet1.mass(), evtWeight);
	  theDir.getObject<TH1>("CATop_minmass")	->Fill( j0_minmass, evtWeight);
	  theDir.getObject<TH1>("CATop_minmass")	->Fill( j1_minmass, evtWeight);
	  theDir.getObject<TH1>("CATop_nsubjets")	->Fill( j0_nsubjets, evtWeight);
	  theDir.getObject<TH1>("CATop_nsubjets")	->Fill( j1_nsubjets, evtWeight);
	  theDir.getObject<TH1>("CATop_pt")			->Fill( p4_catop_jet0.pt(), evtWeight);
	  theDir.getObject<TH1>("CATop_pt")			->Fill( p4_catop_jet1.pt(), evtWeight);
	  theDir.getObject<TH1>("CATop_eta")		->Fill( p4_catop_jet0.eta(), evtWeight);
	  theDir.getObject<TH1>("CATop_eta")		->Fill( p4_catop_jet1.eta(), evtWeight);
	  //theDir.getObject<TH2>("CATop_mass_minmass") ->Fill( p4_catop_jet0.mass(), j0_minmass, evtWeight);
	  //theDir.getObject<TH2>("CATop_mass_minmass") ->Fill( p4_catop_jet1.mass(), j1_minmass, evtWeight);
		
		
		
	  if ( p4_catop_jet0.mass() > caTopJetMassMin_ && p4_catop_jet0.mass() < caTopJetMassMax_ ) 
	  {		
			theDir.getObject<TH1>("CATop_mass_masscut")		->Fill( p4_catop_jet0.mass(), evtWeight);
			theDir.getObject<TH1>("CATop_minmass_masscut")	->Fill( j0_minmass, evtWeight);
			theDir.getObject<TH1>("CATop_nsubjets_masscut")	->Fill( j0_nsubjets, evtWeight);
			theDir.getObject<TH1>("CATop_pt_masscut")		->Fill( p4_catop_jet0.pt(), evtWeight);
			theDir.getObject<TH1>("CATop_eta_masscut")		->Fill( p4_catop_jet0.eta(), evtWeight);
	  }
	  if ( p4_catop_jet1.mass() > caTopJetMassMin_ && p4_catop_jet1.mass() < caTopJetMassMax_ ) 
	  {		
			theDir.getObject<TH1>("CATop_mass_masscut")		->Fill( p4_catop_jet1.mass(), evtWeight);
			theDir.getObject<TH1>("CATop_minmass_masscut")	->Fill( j1_minmass, evtWeight);
			theDir.getObject<TH1>("CATop_nsubjets_masscut")	->Fill( j1_nsubjets, evtWeight);
			theDir.getObject<TH1>("CATop_pt_masscut")		->Fill( p4_catop_jet1.pt(), evtWeight);
			theDir.getObject<TH1>("CATop_eta_masscut")		->Fill( p4_catop_jet1.eta(), evtWeight);
	  }
	  if ( j0_minmass > caTopMinMassMin_  ) 
	  {		
			theDir.getObject<TH1>("CATop_mass_minmasscut")		->Fill( p4_catop_jet0.mass(), evtWeight);
			theDir.getObject<TH1>("CATop_minmass_minmasscut")	->Fill( j0_minmass, evtWeight);
			theDir.getObject<TH1>("CATop_nsubjets_minmasscut")	->Fill( j0_nsubjets, evtWeight);
			theDir.getObject<TH1>("CATop_pt_minmasscut")		->Fill( p4_catop_jet0.pt(), evtWeight);
			theDir.getObject<TH1>("CATop_eta_minmasscut")		->Fill( p4_catop_jet0.eta(), evtWeight);
	  }
	  if (  j1_minmass > caTopMinMassMin_  ) 
	  {		
			theDir.getObject<TH1>("CATop_mass_minmasscut")		->Fill( p4_catop_jet1.mass(), evtWeight);
			theDir.getObject<TH1>("CATop_minmass_minmasscut")	->Fill( j1_minmass, evtWeight);
			theDir.getObject<TH1>("CATop_nsubjets_minmasscut")	->Fill( j1_nsubjets, evtWeight);
			theDir.getObject<TH1>("CATop_pt_minmasscut")		->Fill( p4_catop_jet1.pt(), evtWeight);
			theDir.getObject<TH1>("CATop_eta_minmasscut")		->Fill( p4_catop_jet1.eta(), evtWeight);
	  }
	  if ( p4_catop_jet0.mass() > caTopJetMassMin_ && p4_catop_jet0.mass() < caTopJetMassMax_ && j0_nsubjets>=3)
      {
            theDir.getObject<TH1>("CATop_mass_masscut_nsubjetscut")     ->Fill( p4_catop_jet0.mass(), evtWeight);
            theDir.getObject<TH1>("CATop_minmass_masscut_nsubjetscut")  ->Fill( j0_minmass, evtWeight);
            theDir.getObject<TH1>("CATop_nsubjets_masscut_nsubjetscut") ->Fill( j0_nsubjets, evtWeight);
            theDir.getObject<TH1>("CATop_pt_masscut_nsubjetscut")       ->Fill( p4_catop_jet0.pt(), evtWeight);
            theDir.getObject<TH1>("CATop_eta_masscut_nsubjetscut")      ->Fill( p4_catop_jet0.eta(), evtWeight);
      }
      if ( p4_catop_jet1.mass() > caTopJetMassMin_ && p4_catop_jet1.mass() < caTopJetMassMax_ && j1_nsubjets>=3)
      {
            theDir.getObject<TH1>("CATop_mass_masscut_nsubjetscut")     ->Fill( p4_catop_jet1.mass(), evtWeight);
            theDir.getObject<TH1>("CATop_minmass_masscut_nsubjetscut")  ->Fill( j1_minmass, evtWeight);
            theDir.getObject<TH1>("CATop_nsubjets_masscut_nsubjetscut") ->Fill( j1_nsubjets, evtWeight);
            theDir.getObject<TH1>("CATop_pt_masscut_nsubjetscut")       ->Fill( p4_catop_jet1.pt(), evtWeight);
            theDir.getObject<TH1>("CATop_eta_masscut_nsubjetscut")      ->Fill( p4_catop_jet1.eta(), evtWeight);
      }


	  
		

	  //if ( hasTaggedTopJet0 ) theDir.getObject<TH1>("CATop_pt")	->Fill( p4_catop_jet0.pt(), evtWeight);
	  
		//-----------------
		// Type 2 commissioning plots
		if ( hemisphere0.size()>0 )
		{	
			double y = -1;
			double mu = -1;
			double dR = -1; 
			pat::Jet const & jet0 = *hemisphere0.at(0);
			int numOfDaughters    = jet0.numberOfDaughters();
			if (verbose_) std::cout << "numOfDaughters " << numOfDaughters << std::endl;
			double mfat       = jet0.mass();
			double pt = jet0.pt();
			double eta = jet0.eta();
			if ( numOfDaughters >= 2 ) {
				double mfat       = jet0.mass();
				double m0     = jet0.daughter(0) -> mass();
				double m1     = jet0.daughter(1) -> mass();
				double pt0        = jet0.daughter(0) -> pt();
				double pt1        = jet0.daughter(1) -> pt();
				if ( m1 > m0 ) {
					double temp = m1;
					m1 = m0;
					m0 = temp;
					temp = pt1;
					pt1 = pt0;
					pt0 = temp;
				}
				
				dR = reco::deltaR<double>( jet0.daughter(0) ->eta(),
										  jet0.daughter(0) ->phi(),
										  jet0.daughter(1) ->eta(),
										  jet0.daughter(1) ->phi()  );
				y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (mfat*mfat);
				mu = m0 / mfat ;
				if (verbose_) cout<<"y = "<<y <<" mu = "<<mu<< " dR "<<dR <<endl;
				
			}
			
			theDir.getObject<TH1>("PrunedJet_mass") ->Fill(mfat,evtWeight);
			theDir.getObject<TH1>("PrunedJet_pt") ->Fill(pt,evtWeight);
			theDir.getObject<TH1>("PrunedJet_eta") ->Fill(eta,evtWeight);
			theDir.getObject<TH1>("PrunedJet_nsubjets") ->Fill(numOfDaughters,evtWeight);		
			theDir.getObject<TH1>("PrunedJet_mu") ->Fill(mu,evtWeight);
			//theDir.getObject<TH2>("PrunedJet_mass_mu") ->Fill(mfat,mu,evtWeight);
			//theDir.getObject<TH2>("PrunedJet_pt_mu") ->Fill(jet0.pt(),mu,evtWeight);

			
			if ( mfat >60 && mfat<100 ) 
			{
				theDir.getObject<TH1>("PrunedJet_mass_masscut") ->Fill(mfat,evtWeight);
				theDir.getObject<TH1>("PrunedJet_pt_masscut") ->Fill(pt,evtWeight);
				theDir.getObject<TH1>("PrunedJet_eta_masscut") ->Fill(eta,evtWeight);
				theDir.getObject<TH1>("PrunedJet_nsubjets_masscut") ->Fill(numOfDaughters,evtWeight);		
				theDir.getObject<TH1>("PrunedJet_mu_masscut") ->Fill(mu,evtWeight);
				theDir.getObject<TH1>("PrunedJet_dR_masscut") ->Fill(dR,evtWeight);
				theDir.getObject<TH1>("PrunedJet_y_masscut") ->Fill(y,evtWeight);
			}
			if ( mu <0.4)
			{
				theDir.getObject<TH1>("PrunedJet_mass_mucut") ->Fill(mfat,evtWeight);
				theDir.getObject<TH1>("PrunedJet_pt_mucut") ->Fill(pt,evtWeight);
				theDir.getObject<TH1>("PrunedJet_eta_mucut") ->Fill(eta,evtWeight);
				theDir.getObject<TH1>("PrunedJet_nsubjets_mucut") ->Fill(numOfDaughters,evtWeight);		
				theDir.getObject<TH1>("PrunedJet_mu_mucut") ->Fill(mu,evtWeight);
				theDir.getObject<TH1>("PrunedJet_dR_mucut") ->Fill(dR,evtWeight);
				theDir.getObject<TH1>("PrunedJet_y_mucut") ->Fill(y,evtWeight);
			}
			
			
		}
		if ( hemisphere1.size()>0 )
		{	
			double y = -1;
			double mu = -1;
			double dR = -1; 
			pat::Jet const & jet1 = *hemisphere1.at(0);
			double pt = jet1.pt();
			double eta = jet1.eta();
			int numOfDaughters    = jet1.numberOfDaughters();
			if (verbose_) std::cout << "numOfDaughters " << numOfDaughters << std::endl;
			double mfat       = jet1.mass();
			
			if ( numOfDaughters >= 2 ) {
				double mfat       = jet1.mass();
				double m0     = jet1.daughter(0) -> mass();
				double m1     = jet1.daughter(1) -> mass();
				double pt0        = jet1.daughter(0) -> pt();
				double pt1        = jet1.daughter(1) -> pt();
				if ( m1 > m0 ) {
					double temp = m1;
					m1 = m0;
					m0 = temp;
					temp = pt1;
					pt1 = pt0;
					pt0 = temp;
				}
				
				dR = reco::deltaR<double>( jet1.daughter(0) ->eta(),
										  jet1.daughter(0) ->phi(),
										  jet1.daughter(1) ->eta(),
										  jet1.daughter(1) ->phi()  );
				y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (mfat*mfat);
				mu = m0 / mfat ;
				if (verbose_) cout<<"y = "<<y <<" mu = "<<mu<< " dR "<<dR <<endl;
				
			}
			
			theDir.getObject<TH1>("PrunedJet_mass") ->Fill(mfat,evtWeight);
			theDir.getObject<TH1>("PrunedJet_pt") ->Fill(pt,evtWeight);
			theDir.getObject<TH1>("PrunedJet_eta") ->Fill(eta,evtWeight);
			theDir.getObject<TH1>("PrunedJet_nsubjets") ->Fill(numOfDaughters,evtWeight);		
			theDir.getObject<TH1>("PrunedJet_mu") ->Fill(mu,evtWeight);
			theDir.getObject<TH1>("PrunedJet_dR") ->Fill(dR,evtWeight);
			theDir.getObject<TH1>("PrunedJet_y") ->Fill(y,evtWeight);
			//theDir.getObject<TH2>("PrunedJet_mass_mu") ->Fill(mfat,mu,evtWeight);
			//theDir.getObject<TH2>("PrunedJet_pt_mu") ->Fill(jet1.pt(),mu,evtWeight);
			
			if ( mfat >60 && mfat<100 ) 
			{
				theDir.getObject<TH1>("PrunedJet_mass_masscut") ->Fill(mfat,evtWeight);
				theDir.getObject<TH1>("PrunedJet_pt_masscut") ->Fill(pt,evtWeight);
				theDir.getObject<TH1>("PrunedJet_eta_masscut") ->Fill(eta,evtWeight);
				theDir.getObject<TH1>("PrunedJet_nsubjets_masscut") ->Fill(numOfDaughters,evtWeight);		
				theDir.getObject<TH1>("PrunedJet_mu_masscut") ->Fill(mu,evtWeight);
				theDir.getObject<TH1>("PrunedJet_dR_masscut") ->Fill(dR,evtWeight);
				theDir.getObject<TH1>("PrunedJet_y_masscut") ->Fill(y,evtWeight);
			}
			if ( mu <0.4)
			{
				theDir.getObject<TH1>("PrunedJet_mass_mucut") ->Fill(mfat,evtWeight);
				theDir.getObject<TH1>("PrunedJet_pt_mucut") ->Fill(pt,evtWeight);
				theDir.getObject<TH1>("PrunedJet_eta_mucut") ->Fill(eta,evtWeight);
				theDir.getObject<TH1>("PrunedJet_nsubjets_mucut") ->Fill(numOfDaughters,evtWeight);		
				theDir.getObject<TH1>("PrunedJet_mu_mucut") ->Fill(mu,evtWeight);
				theDir.getObject<TH1>("PrunedJet_dR_mucut") ->Fill(dR,evtWeight);
				theDir.getObject<TH1>("PrunedJet_y_mucut") ->Fill(y,evtWeight);
			}
			
		}
		
		
	  //-----------------
	  // Find non -leading btags		
	  if (verbose_) cout<<"Check if each hemisphere has non-leading b-jet"<<endl;
	  bool hasNonLeadingBjet0 = false;
	  bool hasNonLeadingBjet1 = false;
			
	  for(size_t i=0; i<hemisphere0_ca8.size(); i++ ) {
	    pat::Jet const & ca8 = *hemisphere0_ca8.at(i);
	    if (verbose_) 
	      cout<<"  hemi0 ca8  jet "<<i<<"  eta "<<ca8.eta()<<" phi "<<ca8.phi()<<" pt "
		  <<ca8.pt()<<" bDiscrim "<<ca8.bDiscriminator( bTagAlgo_ )<<" op "<< bTagOP_ <<endl;
				
	    if ( ca8.bDiscriminator( bTagAlgo_ ) > bTagOP_ )
	      {
		if (verbose_) cout<<"   -> b-tagged"<<endl;
		if (i>0) { hasNonLeadingBjet0 = true; if (verbose_) cout<<"    -> non-leading b-tag"<<endl;}
	      }
	  }
	  for(size_t i=0; i<hemisphere1_ca8.size(); i++ ) {
	    pat::Jet const & ca8 = *hemisphere1_ca8.at(i);
	    if (verbose_) 
	      cout<<"  hemi1 ca8 jet "<<i<<"  eta "<<ca8.eta()<<" phi "<<ca8.phi()<<" pt "
		  <<ca8.pt()<<" bDiscrim "<<ca8.bDiscriminator( bTagAlgo_ )<<" op "<< bTagOP_ <<endl;
				
	    if ( ca8.bDiscriminator( bTagAlgo_ ) > bTagOP_ )
	      {
		if (verbose_) cout<<"   -> b-tagged"<<endl;
		if (i>0) { hasNonLeadingBjet1 = true; if (verbose_) cout<<"    -> non-leading b-tag"<<endl;}
	      }
	  }
			
	  //////////////////////////////////
	  // Setup Type 2		
	  if (verbose_) cout<<"Setup Type 2"<<endl;
	  bool hasLooseTop0 = false, hasTightTop0 = false;
	  bool hasLooseTop1 = false, hasTightTop1 = false;
	  bool hasTwoWTags = (wTags0.size() >=1 ) && (wTags1.size() >=1 );
	  bool hasBTag0 = (bTags0.size() >=1 );
	  bool hasBTag1 = (bTags1.size() >= 1 );
	  bool hasWTag0 = (wTags0.size() >= 1 );
	  bool hasWTag1 = (wTags1.size() >= 1 );
	  bool hasOneWTag = (hasWTag0 && !hasWTag1 ) || (hasWTag1 && !hasWTag0) ;
	  bool hasWTag = hasWTag0 || hasWTag1;
	  bool hasBTag = hasBTag0 || hasBTag1;
			
	  reco::Candidate::LorentzVector p4_top0;
	  reco::Candidate::LorentzVector p4_top1;
			
			
	  if( hasBTag0 && hasWTag0 )  {
	    p4_top0 = wTags0.at(0)->p4() + bTags0.at(0)->p4() ;
	    if (verbose_) cout<<"  hemi 0 has W and b tag.   mass(W,b) = "<<p4_top0.mass()<<endl;
				
	    if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )
	      hasTightTop0 = true;
	  } else if( aJet0 && hasWTag0 ) {
	    p4_top0 = wTags0.at(0)->p4() + aJet0->p4() ;
	    if (verbose_) cout<<"  hemi 0 has W tag and a non-b jet.   mass(W,jet) = "<<p4_top0.mass()<<endl;
	    if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )
	      hasLooseTop0 = true;
	  }
	  if( hasBTag1 && hasWTag1 )  {
	    p4_top1 = wTags1.at(0)->p4() + bTags1.at(0)->p4();
	    if (verbose_) cout<<"  hemi 1 has W and b tag.   mass(W,b) = "<<p4_top1.mass()<<endl;
	    if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )
	      hasTightTop1 = true;
	  } else if ( aJet1 && hasWTag1 ) {
	    p4_top1 = wTags1.at(0)->p4() + aJet1->p4();
	    if (verbose_) cout<<"  hemi 1 has W tag and a non-b jet.   mass(W,jet) = "<<p4_top1.mass()<<endl;
	    if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )
	      hasLooseTop1 = true;
	  }
						
	  if (verbose_) 
	    {			
	      cout<<"Setup Summary "<<endl;
	      cout<<"           hasWTag0 "<<hasWTag0<<endl;
	      cout<<"           hasBTag0 "<<hasBTag0<<endl;
	      cout<<"       hasTightTop0 "<<hasTightTop0<<endl;
	      cout<<"       hasLooseTop0 "<<hasLooseTop0<<endl;	
	      cout<<"   hasTaggedTopJet0 "<<hasTaggedTopJet0<<endl;
	      cout<<" hasNonLeadingBjet0 "<<hasNonLeadingBjet0<<endl;
	      cout<<"           hasWTag1 "<<hasWTag1<<endl;
	      cout<<"           hasBTag1 "<<hasBTag1<<endl;
	      cout<<"       hasTightTop1 "<<hasTightTop1<<endl;
	      cout<<"       hasLooseTop1 "<<hasLooseTop1<<endl;	
	      cout<<"   hasTaggedTopJet1 "<<hasTaggedTopJet1<<endl;
	      cout<<" hasNonLeadingBjet1 "<<hasNonLeadingBjet1<<endl;
	    }	
			
			
	  ///////////////////////////////////////////////////////////////////////////////////////////////
	  // Do some counting to measure efficiencies
			
	  theDir.getObject<TH1>("Nevents_preselected")->Fill(1,evtWeight);
			
	  bool hasZeroTopTags = (!hasTaggedTopJet0 && !hasTaggedTopJet1);
	  bool hasOneTopTag = ((hasTaggedTopJet0 && !hasTaggedTopJet1) || (hasTaggedTopJet1 && !hasTaggedTopJet0));
	  bool hasTwoTopTags = (hasTaggedTopJet0 && hasTaggedTopJet1);
			
	  bool hasZeroBTags = (!hasBTag0 && !hasBTag1);
	  bool hasOneBTag = ((hasBTag0 && !hasBTag1) || (hasBTag1 && !hasBTag0));
	  bool hasTwoBTags = (hasBTag0 && hasBTag1);
	  //cout<<"hasBTag0 "<<hasBTag0<<" hasBTag1 "<<hasBTag1<<" hasZeroBTags "<<hasZeroBTags<<" hasOneBTag "<<hasOneBTag<<" hasTwoBTags "<<hasTwoBTags<<endl;
			 
	  bool hasZeroWTags = (!hasWTag0 && !hasWTag1);
			
	  bool hasZeroLooseTops = (!hasLooseTop0 && !hasLooseTop1);
	  bool hasOneLooseTop = ((hasLooseTop0 && !hasLooseTop1) || (hasLooseTop1 && !hasLooseTop0));
	  bool hasTwoLooseTops = (hasLooseTop0 && hasLooseTop1);
			
	  bool hasZeroTightTops = (!hasTightTop0 && !hasTightTop1);
	  bool hasOneTightTop = ((hasTightTop0 && !hasTightTop1) || (hasTightTop1 && !hasTightTop0));
	  bool hasTwoTightTops = (hasTightTop0 && hasTightTop1);
			
	  bool hasOneLooseOneTightTop = ((hasTightTop0 && hasLooseTop1) || (hasTightTop1 && hasLooseTop0));
			
	  bool hasUntaggedJet0 = (noTags0.size() >=1);
	  bool hasUntaggedJet1 = (noTags0.size() >=1);
			
	  // Count top tags
	  if (hasTaggedTopJet0) theDir.getObject<TH1>("Nevents_hasTaggedTopJet0")->Fill(1,evtWeight);
	  if (hasTaggedTopJet1) theDir.getObject<TH1>("Nevents_hasTaggedTopJet1")->Fill(1,evtWeight);
	  if (hasZeroTopTags) theDir.getObject<TH1>("Nevents_hasZeroTopTags")->Fill(1,evtWeight);
	  if (hasOneTopTag) theDir.getObject<TH1>("Nevents_hasOneTopTag")->Fill(1,evtWeight);
	  if (hasTwoTopTags) theDir.getObject<TH1>("Nevents_hasTwoTopTags")->Fill(1,evtWeight);
			
	  // Count W tags
	  if (hasWTag0) theDir.getObject<TH1>("Nevents_hasWTag0")->Fill(1,evtWeight);
	  if (hasWTag1) theDir.getObject<TH1>("Nevents_hasWTag1")->Fill(1,evtWeight);
	  if (hasZeroWTags) theDir.getObject<TH1>("Nevents_hasZeroWTags")->Fill(1,evtWeight);
	  if (hasOneWTag) theDir.getObject<TH1>("Nevents_hasOneWTag")->Fill(1,evtWeight);
	  if (hasTwoWTags) theDir.getObject<TH1>("Nevents_hasTwoWTags")->Fill(1,evtWeight);
			
	  // Count b tags
	  if (hasBTag0) theDir.getObject<TH1>("Nevents_hasBTag0")->Fill(1,evtWeight);
	  if (hasBTag1) theDir.getObject<TH1>("Nevents_hasBTag1")->Fill(1,evtWeight);
	  if (hasZeroBTags) theDir.getObject<TH1>("Nevents_hasZeroBTags")->Fill(1,evtWeight);
	  if (hasOneBTag) theDir.getObject<TH1>("Nevents_hasOneBTag")->Fill(1,evtWeight);
	  if (hasTwoBTags) theDir.getObject<TH1>("Nevents_hasTwoBTags")->Fill(1,evtWeight);
			
	  // Count Tight Tops
	  if (hasTightTop0) theDir.getObject<TH1>("Nevents_hasTightTop0")->Fill(1,evtWeight);
	  if (hasTightTop1) theDir.getObject<TH1>("Nevents_hasTightTop1")->Fill(1,evtWeight);
	  if (hasZeroTightTops) theDir.getObject<TH1>("Nevents_hasZeroTightTops")->Fill(1,evtWeight);
	  if (hasOneTightTop) theDir.getObject<TH1>("Nevents_hasOneTightTop")->Fill(1,evtWeight);
	  if (hasTwoTightTops) theDir.getObject<TH1>("Nevents_hasTwoTightTops")->Fill(1,evtWeight);
			
	  // Count Loose Tops
	  if (hasLooseTop0) theDir.getObject<TH1>("Nevents_hasLooseTop0")->Fill(1,evtWeight);
	  if (hasLooseTop1) theDir.getObject<TH1>("Nevents_hasLooseTop1")->Fill(1,evtWeight);
	  if (hasZeroLooseTops) theDir.getObject<TH1>("Nevents_hasZeroLooseTops")->Fill(1,evtWeight);
	  if (hasOneLooseTop) theDir.getObject<TH1>("Nevents_hasOneLooseTop")->Fill(1,evtWeight);
	  if (hasTwoLooseTops) theDir.getObject<TH1>("Nevents_hasTwoLooseTops")->Fill(1,evtWeight);
			
	  // Count other stuff
	  if (hasOneLooseOneTightTop) theDir.getObject<TH1>("Nevents_hasOneLooseOneTightTop")->Fill(1,evtWeight);
	  if ( ( hasWTag0 && hasBTag0 ) || ( hasWTag1 && hasBTag1 ) ) theDir.getObject<TH1>("Nevents_hemiWithWandB")->Fill(1,evtWeight);
	  if ( ( hasWTag0 && hasUntaggedJet0 ) || ( hasWTag1 && hasUntaggedJet1 ) ) theDir.getObject<TH1>("Nevents_hemiWithWandUntagged")->Fill(1,evtWeight);
	  if ( (hasTaggedTopJet0 && hasTightTop1) || (hasTaggedTopJet1 && hasTightTop0) ) theDir.getObject<TH1>("Nevents_TopTagAndTightTop")->Fill(1,evtWeight);
	  if ( (hasTaggedTopJet0 && hasLooseTop1) || (hasTaggedTopJet1 && hasLooseTop0) ) theDir.getObject<TH1>("Nevents_TopTagAndLooseTop")->Fill(1,evtWeight);
	  if ( (hasTaggedTopJet0 && hasWTag1 && hasBTag1) || (hasTaggedTopJet1 && hasWTag0 && hasBTag0) ) theDir.getObject<TH1>("Nevents_TopTagAndWandB")->Fill(1,evtWeight);
	  if ( hasTwoWTags && hasOneBTag  ) theDir.getObject<TH1>("Nevents_hasTwoWTags_hasOneBTag")->Fill(1,evtWeight);
	  if ( hasTwoWTags && hasTwoBTags  ) theDir.getObject<TH1>("Nevents_hasTwoWTags_hasTwoBTags")->Fill(1,evtWeight);
	  if ( (hasTightTop0 && hasLooseTop1) || (hasLooseTop0 && hasTightTop1))  theDir.getObject<TH1>("Nevents_TightTopAndLooseTop")->Fill (1.5, 1);
	  if ( (hasTaggedTopJet0 && !hasTaggedTopJet1 && !hasNonLeadingBjet1) || (hasTaggedTopJet1 && !hasTaggedTopJet0 && !hasNonLeadingBjet0)  ) theDir.getObject<TH1>("Nevents_hasOneTopJet_noNonLeadingBjet")->Fill(1,evtWeight);
	  if ( (hasTaggedTopJet0  && !hasWTag1 && hasBTag1 ) || (hasTaggedTopJet1  && !hasWTag0 && hasBTag0 ) ) theDir.getObject<TH1>("Nevents_hemiTop_hemiBnoW")->Fill (1.5, 1);    
	  if ( hasBTag ) theDir.getObject<TH1>("Nevents_hasBTag")->Fill (1.5, 1); 
	  if ( hasBTag && !hasWTag0 && !hasWTag1 ) theDir.getObject<TH1>("Nevents_hasBTag_noWTag")->Fill (1.5, 1); 
			

	
			
			
		

	  ///////////////////////////////////////////////////////////////////////////////////////////////
	  // Measure pass events
	  if (verbose_) cout<<"Measure pass events"<<endl;

	  bool type11_passevent=false;
	  bool type12_passevent=false;
	  bool type22_passevent=false;
			
	  // Use unpruned, regular CA8 jets for Njets, pT, and eta cuts. 
	
	 //"N-1" plots
	  if ( preselected_event && (hasTaggedTopJet0 || hasTaggedTopJet1) )
      {
        double delta_phi_catop = fabs( reco::deltaPhi<double>( p4_catop_jet0.phi(), p4_catop_jet1.phi() ) );
        if (delta_phi_catop >2.1)
        {
          double ttMass = (p4_catop_jet0+p4_catop_jet1).mass();

          theDir.getObject<TH1>("ttMassType11_atLeast1Tag")->Fill (ttMass, evtWeight); 


          if (hasTaggedTopJet0)
		  {
		    theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_pt")->Fill (p4_catop_jet1.pt() , evtWeight); 
		    theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_mass")->Fill ( p4_catop_jet1.mass() , evtWeight); 
		    theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmass")->Fill (j1_minmass , evtWeight); 
		    theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_nsubjets")->Fill (j1_nsubjets , evtWeight); 
		  
		  	if ( p4_catop_jet1.mass() >140 && p4_catop_jet1.mass() <250 )
			{
		  	    theDir.getObject<TH1>("ttMassType11_atLeast1Tag_masscut")->Fill (ttMass, evtWeight);
				theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_pt")->Fill (p4_catop_jet1.pt() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_mass")->Fill ( p4_catop_jet1.mass() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_minmass")->Fill (j1_minmass , evtWeight);        
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjets")->Fill (j1_nsubjets , evtWeight);
			}
			if ( p4_catop_jet1.mass() >140 && p4_catop_jet1.mass() <250 && j1_nsubjets>=3 )
			{
		  	    theDir.getObject<TH1>("ttMassType11_atLeast1Tag_masscut_nsubjetscut")->Fill (ttMass, evtWeight);
				theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_pt")->Fill (p4_catop_jet1.pt() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_mass")->Fill ( p4_catop_jet1.mass() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_minmass")->Fill (j1_minmass , evtWeight);        
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_nsubjets")->Fill (j1_nsubjets , evtWeight);
			}
		 
		  	if (  j1_minmass >50 )
			{
		    	theDir.getObject<TH1>("ttMassType11_atLeast1Tag_minmasscut")->Fill (ttMass, evtWeight);
				theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_pt")->Fill (p4_catop_jet1.pt() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_mass")->Fill ( p4_catop_jet1.mass() , evtWeight);
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_minmass")->Fill (j1_minmass , evtWeight);        
            	theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_nsubjets")->Fill (j1_nsubjets , evtWeight);
			}
		  }
		if (hasTaggedTopJet1)
          {   
            theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_pt")->Fill (p4_catop_jet0.pt() , evtWeight);
            theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_mass")->Fill ( p4_catop_jet0.mass() , evtWeight);
            theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmass")->Fill (j0_minmass , evtWeight);        
            theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_nsubjets")->Fill (j0_nsubjets , evtWeight);
          
            if ( p4_catop_jet0.mass() >140 && p4_catop_jet0.mass() <250 )
            {
		    	theDir.getObject<TH1>("ttMassType11_atLeast1Tag_masscut")->Fill (ttMass, evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_pt")->Fill (p4_catop_jet0.pt() , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_mass")->Fill ( p4_catop_jet0.mass() , evtWeight);    
            theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_minmass")->Fill (j0_minmass , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjets")->Fill (j0_nsubjets , evtWeight);
            }
 			if ( p4_catop_jet0.mass() >140 && p4_catop_jet0.mass() <250 && j0_nsubjets>=3 )
            {
                theDir.getObject<TH1>("ttMassType11_atLeast1Tag_masscut_nsubjetscut")->Fill (ttMass, evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_pt")->Fill (p4_catop_jet0.pt() , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_mass")->Fill ( p4_catop_jet0.mass() , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_minmass")->Fill (j0_minmass , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_masscut_nsubjetscut_nsubjets")->Fill (j0_nsubjets , evtWeight);
            }


            if (  j0_minmass >50 )
            {
		    	theDir.getObject<TH1>("ttMassType11_atLeast1Tag_minmasscut")->Fill (ttMass, evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_pt")->Fill (p4_catop_jet0.pt() , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_mass")->Fill ( p4_catop_jet0.mass() , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_minmass")->Fill (j0_minmass , evtWeight);
                theDir.getObject<TH1>("CATopUntagged_atLeast1Tag_minmasscut_nsubjets")->Fill (j0_nsubjets , evtWeight);
            }
          } 


        }
      }

	  // Type 1+1 signal
	  if ( preselected_event && hasTaggedTopJet0 && hasTaggedTopJet1 )
	  {
	    double delta_phi_catop = fabs( reco::deltaPhi<double>( p4_catop_jet0.phi(), p4_catop_jet1.phi() ) );
	    if (delta_phi_catop >2.1)
        {
					
		  type11_passevent = true;
		  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(1,evtWeight);
		  theDir.getObject<TH1>("Nevents_type11sig")->Fill(1,evtWeight);
					
		  double ttMass = (p4_catop_jet0+p4_catop_jet1).mass();
		  theDir.getObject<TH1>("ttMassType11_measured")->Fill (ttMass, evtWeight);
					
		  if(runOnData_||verbose_) 
		    {
			if (ttMass>2700) cout<<"WOW"<<endl;
		      cout<<" Yipee!, Type1+Type1, Event id, "<<iEvent.id()<<"  ttmass "<<ttMass<<endl;
		      cout<<" summary:"<<endl;
		      cout<<"  j0_pt "<<p4_catop_jet0.pt()<<endl;
		      cout<<"  j1_pt "<<p4_catop_jet1.pt()<<endl;
		      cout<<"  j0_mass "<<p4_catop_jet0.mass()<<endl;
		      cout<<"  j1_mass "<<p4_catop_jet1.mass()<<endl;
		      cout<<"  j0_nsubjets "<<j0_nsubjets<<endl;
		      cout<<"  j1_nsubjets "<<j1_nsubjets<<endl;
		      cout<<"  j0_minmass "<<j0_minmass<<endl;
		      cout<<"  j1_minmass "<<j1_minmass<<endl;
		    }
		  //This is our signal, return
		  return;
		}
	  }
			
	  // Type 1+2 signal
	  if ( preselected_event && !type11_passevent )
	    {
	      //some test histograms
	      theDir.getObject<TH1>("Nevents_cascade12sig")->Fill(1,evtWeight);
	      if ( (hasTaggedTopJet0 && hasTightTop1) || (hasTaggedTopJet1 && hasTightTop0) ) theDir.getObject<TH1>("Nevents_12_tight_loose")->Fill (1, evtWeight);
	      if ( (hasTaggedTopJet0 && hasLooseTop1) || (hasTaggedTopJet1 && hasLooseTop0) ) theDir.getObject<TH1>("Nevents_12_tight_loose")->Fill (2, evtWeight);
				
	      //fill pass histograms
	      //if ( (hasTaggedTopJet0 && ( hasTightTop1 || hasLooseTop1)) || (hasTaggedTopJet1 && ( hasTightTop0 || hasLooseTop0)) )
				
	      if ( hasOneWTag )
		{
		  if ( (hasTaggedTopJet0 && hasTightTop1) || (hasTaggedTopJet1 && hasTightTop0) )
		    {
		      type12_passevent = true;
		      theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(2,evtWeight);
		      theDir.getObject<TH1>("Nevents_type12sig")->Fill(1,evtWeight);

		      double ttMass =0;
		      if (hasTaggedTopJet0) ttMass = (p4_catop_jet0+p4_top1).mass() ; 
		      if (hasTaggedTopJet1) ttMass = (p4_catop_jet1+p4_top0).mass() ; 
		      theDir.getObject<TH1>("ttMassType12_measured")->Fill (ttMass, evtWeight);
						

		      if(runOnData_||verbose_) 
			{
			  cout<<" WoopWoop!, Type1+Type2, Event id, "<<iEvent.id()<<endl;
			  cout<<" summary:"<<endl;
			  cout<<"  catop j0_mass "<<p4_catop_jet0.mass()<<endl;
			  cout<<"  catop j1_mass "<<p4_catop_jet1.mass()<<endl;
			  cout<<"  catop j0_nsubjets "<<j0_nsubjets<<endl;
			  cout<<"  catop j1_nsubjets "<<j1_nsubjets<<endl;
			  cout<<"  catop j0_minmass "<<j0_minmass<<endl;
			  cout<<"  catop j1_minmass "<<j1_minmass<<endl;
							
			  cout<<"  p4_catop_jet0.mass() "<<p4_catop_jet0.mass()<<endl;
			  cout<<"  p4_catop_jet1.mass() "<<p4_catop_jet1.mass()<<endl;
			  cout<<"  p4_top0.mass() "<<p4_top0.mass()<<endl;
			  cout<<"  p4_top1.mass() "<<p4_top1.mass()<<endl;
			  cout<<"  ttMass "<<ttMass<<endl;
			}
		      //This is our signal, return
		      return;
		    }
		}
	    }
			
	  // Type 22	
	  if ( preselected_event && !type11_passevent && !type12_passevent )
	    {
	      //some test histograms
	      theDir.getObject<TH1>("Nevents_cascade22sig")->Fill(1,evtWeight);
	      if(hasTightTop0 && hasTightTop1) theDir.getObject<TH1>("Nevents_22_tighttight_tightloose")->Fill (0.5, 1);
	      if((hasTightTop0 && hasLooseTop1) || (hasLooseTop0 && hasTightTop1))  theDir.getObject<TH1>("Nevents_22_tighttight_tightloose")->Fill (1.5, 1);
				
	      //fill pass histogram
	      if( (hasTightTop0 && hasTightTop1) || (hasTightTop0 && hasLooseTop1) || (hasLooseTop0 && hasTightTop1) )  
		{
		  type22_passevent=true;
		  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(3,evtWeight);
		  theDir.getObject<TH1>("Nevents_type22sig")->Fill(1,evtWeight);
					
		  double ttMass = (p4_top0+p4_top1).mass() ;
		  theDir.getObject<TH1>("ttMassType22_measured")      ->  Fill( ttMass, evtWeight );

		  if(runOnData_||verbose_) 
		    {
		      cout<<" Woohoo, Type2+Type2, Event id, "<<iEvent.id()<<endl;
		      cout<<"  p4_top0.mass() "<<p4_top0.mass()<<endl;
		      cout<<"  p4_top1.mass() "<<p4_top1.mass()<<endl;
		      cout<<"  ttMass "<<ttMass<<endl;
		      cout<<"  hasTaggedTopJet0 "<<hasTaggedTopJet0<<" hasTaggedTopJet1 "<<hasTaggedTopJet1<<" hasTightTop0 "<< hasTightTop0 <<" hasTightTop1 "<<hasTightTop1<<" hasLooseTop0 "<<hasLooseTop0<<" hasLooseTop1 "<<hasLooseTop1<<endl;
		      cout<<"  hasBTag0 "<<hasBTag0<<" hasBTag1 "<<hasBTag1<<" hasWTag0 "<<hasWTag0<<" hasWTag1 "<<hasWTag1<<endl; 
		    }
		  //This is our signal, return
		  return;
		}
	    }
			
			
	  ///////////////////////////////////////////////////////////////////////////////////////////////
	  // Background estimation
	  if (verbose_) cout<<"Background estimation"<<endl;

	  bool type11_bkgd_prediction_event=false;
	  bool type12_bkgd_prediction_event=false;
		
		
		
	  // Type 1+1 Background estimation starts here
	 
	 // OLD METHOD IS WRONG:
	 /* if ( preselected_event && !type11_passevent && !type12_passevent && !type22_passevent )
	    {
	      theDir.getObject<TH1>("Nevents_cascade11bkg")->Fill(1,evtWeight);
	      double delta_phi_catop = fabs( reco::deltaPhi<double>( p4_catop_jet0.phi(), p4_catop_jet1.phi() ) );
	      if (delta_phi_catop >2.1)
                {
		  int bin0 = topMistag_->FindBin( p4_catop_jet0.pt() );
		  int bin1 = topMistag_->FindBin( p4_catop_jet1.pt() );
		  double mistagProb_jet0 = topMistag_->GetBinContent(bin0);
		  double mistagProb_jet1 = topMistag_->GetBinContent(bin1);
		  double mistagError_jet0 = topMistag_->GetBinError(bin0);
		  double mistagError_jet1 = topMistag_->GetBinError(bin1);
				
		  if ( (hasTaggedTopJet0 && !hasTaggedTopJet1 && !hasNonLeadingBjet1) || (hasTaggedTopJet1 && !hasTaggedTopJet0 && !hasNonLeadingBjet0)  ) 
		    {		
	
		      double ttMass = (p4_catop_jet0+p4_catop_jet1).mass();
		      double weight = 0;
		      double error_squared = 0;
		      double pt=0;
						
		      if (hasTaggedTopJet0)
			{	
			  weight = mistagProb_jet1;
			  error_squared = mistagError_jet1;
			  pt = p4_catop_jet1.pt();
			}
		      if (hasTaggedTopJet1)
			{	
			  weight = mistagProb_jet0;
			  error_squared = mistagError_jet0;
			  pt = p4_catop_jet0.pt();
			}
		      if ( pt > caTopJetPtMin_)
			{
			  if (verbose_) cout<<" Type 11 background estimation event"<<endl;
			  if (verbose_) cout<<"   hasTaggedTopJet0 "<<hasTaggedTopJet0<<endl;
			  if (verbose_) cout<<"   hasTaggedTopJet1 "<<hasTaggedTopJet1<<endl;
			  if (verbose_) cout<<"   hasNonLeadingBjet0 "<<hasNonLeadingBjet0<<endl;
			  if (verbose_) cout<<"   hasNonLeadingBjet1 "<<hasNonLeadingBjet1<<endl;
			  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(4,evtWeight);
			  theDir.getObject<TH1>("Nevents_type11bkg")->Fill(1,evtWeight);
			  type11_bkgd_prediction_event=true;
							
			  theDir.getObject<TH1>("ttMassType11_predicted")->Fill (ttMass, weight);
			  theDir.getObject<TH1>("ttMassType11_predicted_errorSquared")->Fill (ttMass, error_squared);
			  ttMassPred11 -> Accumulate( ttMass, pt, 1,  evtWeight );

			}
		    }
		}
	    }//end 11bkg
		*/
		
		// Background estimation methods:
		// method A: test using the plus-anything sample (1 tagged, don't care about other tag)
		// method B: test using the plus-minus sample ( 1 tagged, 1 not tagged)
		// method C: test using the entire sample, ignoring if they are tagged.
		// method D: test using the minus- minus sample (0 tagged) 
		
		if ( preselected_event )
	    {
			double delta_phi_catop = fabs( reco::deltaPhi<double>( p4_catop_jet0.phi(), p4_catop_jet1.phi() ) );
			if (delta_phi_catop >2.1)
			{
				if ( p4_catop_jet0.pt() > caTopJetPtMin_ && p4_catop_jet1.pt() > caTopJetPtMin_ )
				{
					int bin0 = topMistag_->FindBin( p4_catop_jet0.pt() );
					int bin1 = topMistag_->FindBin( p4_catop_jet1.pt() );
					double mistagProb_jet0 = topMistag_->GetBinContent(bin0);
					double mistagProb_jet1 = topMistag_->GetBinContent(bin1);
					double mistagError_jet0 = topMistag_->GetBinError(bin0);
					double mistagError_jet1 = topMistag_->GetBinError(bin1);
					
					double ttMass = (p4_catop_jet0+p4_catop_jet1).mass();
					
					// Method A - PREFERED METHOD
					double x = flatDistribution_->fire();
					if( x < 0.5 ) {
						if (hasTaggedTopJet0) {
							double weight = mistagProb_jet1;
							double pt = p4_catop_jet1.pt();
							theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_PlusPlus")->Fill (ttMass, weight );
							theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_PlusPlus_reducedWeight")->Fill (ttMass, weight*(1-weight) );
							
							ttMassPred11 -> Accumulate( ttMass, pt, 1,  evtWeight );

						}
					}
					else {
						if (hasTaggedTopJet1) {
							double weight = mistagProb_jet0;
							double pt = p4_catop_jet0.pt();
							theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_PlusPlus")->Fill (ttMass, weight);
							theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_PlusPlus_reducedWeight")->Fill (ttMass, weight*(1-weight) );
							
							ttMassPred11 -> Accumulate( ttMass, pt, 1,  evtWeight );

						}
					}
					
					// Method B
					if (hasTaggedTopJet0 && !hasTaggedTopJet1) {
						double weight = mistagProb_jet1;
						theDir.getObject<TH1>("testPredict11_Sample_MinusPlus")->Fill (ttMass, weight);
						theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_reducedWeight")->Fill (ttMass, weight*(1-weight) );
					}
					if (hasTaggedTopJet1 && !hasTaggedTopJet0) {
						double weight = mistagProb_jet0;
						theDir.getObject<TH1>("testPredict11_Sample_MinusPlus")->Fill (ttMass, weight);
						theDir.getObject<TH1>("testPredict11_Sample_MinusPlus_reducedWeight")->Fill (ttMass, weight*(1-weight) );
					}
					
					// Method C
					if ( !hasTaggedTopJet0 && !hasTaggedTopJet1 ) 
					{		
						if ( p4_catop_jet0.pt() > caTopJetPtMin_ && p4_catop_jet1.pt() > caTopJetPtMin_ )
						{
							double weight = mistagProb_jet0*mistagProb_jet1;
							theDir.getObject<TH1>("testPredict11_Sample_MinusMinus")->Fill (ttMass, weight);
						}
					}
					
					// Method D
					double weight = mistagProb_jet0*mistagProb_jet1;
					theDir.getObject<TH1>("testPredict11_Sample_All")->Fill (ttMass, weight);
	
				}
			}
	    }
		
		
	  // Type 1+2 Background estimation starts here
	  //  Events with 1 top-tagged jet, 0 W-tagged jets, 1 b-tagged jet. 
	  //  Jets in the hemisphere opposite the top jet which, when combined with the b-jet, have a pairwise mass in the top mass window, are used as probes to estimate the background
			
	  theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(1,evtWeight);
	  if( preselected_event && !type11_passevent && !type12_passevent && !type22_passevent && !type11_bkgd_prediction_event )  
	    {
	      theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(2,evtWeight);
	      theDir.getObject<TH1>("Nevents_cascade12bkg")->Fill(1,evtWeight);
	      if( hasBTag && !hasWTag0 && !hasWTag1 ) 
		{
		  theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(3,evtWeight);
		  if( hasTaggedTopJet0  && !hasWTag1 && hasBTag1 )  
		    { 						
		      theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(4,evtWeight);
		      if (verbose_) cout<<" Type 12 background estimation event"<<endl;
		      if (verbose_) cout<<"   hasTaggedTopJet0 "<<hasTaggedTopJet0<<endl;
		      if (verbose_) cout<<"   hasTaggedTopJet1 "<<hasTaggedTopJet1<<endl;
		      if (verbose_) cout<<"   hasWTag0 "<<hasWTag0<<endl;
		      if (verbose_) cout<<"   hasWTag1 "<<hasWTag1<<endl;
		      if (verbose_) cout<<"   hasBTag0 "<<hasBTag0<<endl;
		      if (verbose_) cout<<"   hasBTag1 "<<hasBTag1<<endl;
						
		      bool passTopMass1 = false;
		      p4_top1.SetPxPyPzE(0,0,0,0);
						
		      if (noTags1.size() !=0) theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(5,evtWeight);
						
		      for( size_t i=0; i<noTags1.size(); i++ ) 
			{
			  double pt = noTags1.at(i)->pt();
			  int bin = wMistag_       ->  FindBin( pt );
			  double weight = wMistag_ ->  GetBinContent( bin );  //dummy value, depend on pt
			  double mistagError = wMistag_ ->GetBinError( bin );
							
			  p4_top1 = noTags1.at(i)->p4() + bTags1.at(0)->p4();
			  if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )  
			    {
			      theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(6,evtWeight);
			      if (pt > jetPt0_)
				{
				  theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(7,evtWeight);
				  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(5,evtWeight);
				  theDir.getObject<TH1>("Nevents_type12bkg")->Fill(1,evtWeight);
				  type12_bkgd_prediction_event=true;
									
				  passTopMass1 = true;
				  double ttMass = (p4_catop_jet0+p4_top1).mass() ;
									
				  theDir.getObject<TH1>("ttMassType12_predicted")      ->  Fill( ttMass, weight*evtWeight );
				  theDir.getObject<TH1>("ttMassType12_predicted_errorSquared")->Fill (ttMass, mistagError*mistagError);
				  ttMassPred12 -> Accumulate( ttMass, pt, 1,  evtWeight );
				}
			    }
							
			}  // end i  
		    } // jet0_toptagged 
		  else if ( hasTaggedTopJet1 && !hasWTag0 && hasBTag0) 
		    {
		      theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(4,evtWeight);
		      if (verbose_) cout<<" Type 12 background estimation event"<<endl;
		      if (verbose_) cout<<"   hasTaggedTopJet0 "<<hasTaggedTopJet0<<endl;
		      if (verbose_) cout<<"   hasTaggedTopJet1 "<<hasTaggedTopJet1<<endl;
		      if (verbose_) cout<<"   hasWTag0 "<<hasWTag0<<endl;
		      if (verbose_) cout<<"   hasWTag1 "<<hasWTag1<<endl;
		      if (verbose_) cout<<"   hasBTag0 "<<hasBTag0<<endl;
		      if (verbose_) cout<<"   hasBTag1 "<<hasBTag1<<endl;
						
		      bool passTopMass0 = false;
		      p4_top0.SetPxPyPzE(0,0,0,0);
						
		      if (noTags1.size() !=0) theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(5,evtWeight);
						
		      for( size_t i=0; i<noTags0.size(); i++ )  
			{
			  double pt = noTags0.at(i)->pt();
			  int   bin   =   wMistag_      ->  FindBin( pt );
			  double weight = wMistag_      ->  GetBinContent( bin );  //dummy value, depend on pt
			  double mistagError = wMistag_ ->GetBinError( bin );

			  p4_top0 = noTags0.at(i)->p4() + bTags0.at(0)->p4();
			  if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )  
			    {
			      theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(6,evtWeight);
			      if (pt > jetPt0_)
				{
				  theDir.getObject<TH1>("Nevents_succesiveCuts_type12bkg")->Fill(7,evtWeight);
				  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(5,evtWeight);
				  theDir.getObject<TH1>("Nevents_type12bkg")->Fill(1,evtWeight);
				  type12_bkgd_prediction_event=true;
									
				  passTopMass0 = true;
				  double ttMass = (p4_top0+p4_catop_jet1).mass() ;
				  theDir.getObject<TH1>("ttMassType12_predicted")      ->  Fill( ttMass, weight*evtWeight );
				  theDir.getObject<TH1>("ttMassType12_predicted_errorSquared")->Fill (ttMass, mistagError*mistagError);
				  ttMassPred12 -> Accumulate( ttMass, pt, 1,  evtWeight );
				}
			    }
			}  // end i
		    } //has top jet
		} // end if has btag
	    }//end if preselected
			

	  // Type 2+2 Background estimation starts here
	  if( preselected_event && !type11_passevent && !type12_passevent && !type22_passevent && !type11_bkgd_prediction_event && !type12_bkgd_prediction_event )  
	    {
	      theDir.getObject<TH1>("Nevents_cascade22bkg")->Fill(1,evtWeight);
	      if( hasOneWTag && hasBTag ) {
		if( hasWTag0 )  { 
		  //cout<<"case 1"<<endl;
		  bool passTopMass1 = false;
		  p4_top1.SetPxPyPzE(0,0,0,0);
		  if( hasTightTop0 )  { 
		    //cout<<"case 10"<<endl;
		    if (verbose_) cout<<"hasTightTop0. loop over hemi1 jets for background estimation"<<endl;
							
		    for( size_t i=0; i<noTags1.size(); i++ )  {
								
								
		      double pt = noTags1.at(i)->pt();
		      int bin = wMistag_       ->  FindBin( pt );
		      double weight = wMistag_ ->  GetBinContent( bin );  //dummy value, depend on pt
		      double mistagError = wMistag_ ->GetBinError( bin );
		      if (verbose_) cout<<" jet "<<i<<" pt "<<noTags1.at(i)->pt()<<"  weight "<<weight<<endl;

		      if( hasBTag1 )  {
			cout<<"   -> hasBTag1"<<endl;
			p4_top1 = noTags1.at(i)->p4() + bTags1.at(0)->p4();
			theDir.getObject<TH1>("topMassPred")     ->  Fill( p4_top1.mass(), weight*evtWeight );
			if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )  {
			  cout<<"   -> passes top mass cut "<<p4_top1.mass() <<endl;

			  if (pt > jetPt0_) {
											
			    if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			    if (verbose_) cout<<"   hasTightTop0. hemi 1 has b-tag and p4_top1.mass() "<<p4_top1.mass()<<endl;
											
			    passTopMass1 = true;
			    theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			    theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);
			    theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(1,evtWeight);
											
			    double ttMass = (p4_top0+p4_top1).mass() ;
			    theDir.getObject<TH1>("ttMassType22_predicted")      ->  Fill( ttMass, weight*evtWeight );
			    theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
											
			    ttMassPred22      ->    Accumulate( ttMass, pt, 1,  evtWeight );
			  }
			}
		      }
		      else {  
			if (verbose_) cout<<"   -> no hemi1 btag"<<endl;
			if (verbose_) cout<<"      Loop over other jets and find the closest jet"<<endl;

			double minDr = 9999.0;
			pat::Jet const * nearestJet=NULL;
			for( size_t j=0; j<noTags1.size(); j++ )  {
			  if( i==j )   continue;
			  double dR = reco::deltaR<double>( noTags1.at(i)->eta(), noTags1.at(i)->phi(),
							    noTags1.at(j)->eta(), noTags1.at(j)->phi() );
			  if( dR < minDr )  {
			    minDr = dR ;
			    nearestJet = &(*noTags1.at(j));
			  }
			 if (verbose_)  cout<<"        Jet "<<j<<" dR "<<dR<<" minDr "<<minDr<<endl;

			} //end j
			if( nearestJet )  {
			  //cout<<"case 11"<<endl;
			 if (verbose_)  cout<<"          Found a nearestJet pt = "<<nearestJet->pt()<<" weight "<<weight<<endl;

			  p4_top1 = noTags1.at(i)->p4() + nearestJet->p4();
			  int  bin1  = wMistag_      ->  FindBin( nearestJet->pt() );
			  double weight1 =  wMistag_ -> GetBinContent( bin1 );
			  double mistagError1 = wMistag_ ->GetBinError( bin1 );

			  weight *= (1-weight1);
										
			  if (verbose_) cout<<"            weight1 = "<<weight1<<" weight "<<weight<<endl;

			  if (verbose_) cout<<"            1-weight1 = "<<1-weight1<<" weight "<<weight<<endl;

										
			  theDir.getObject<TH1>("topMassPred")   ->  Fill( p4_top1.mass(), weight*evtWeight );
			  if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )  {
			    if (verbose_) cout<<"               passes top mass cut "<<p4_top1.mass()<<endl;
											
			    if (pt > jetPt0_) {
												
			      if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			      if (verbose_) cout<<"   hasTightTop0. hemi 1 has 0 b-tags and p4_top1.mass() "<<p4_top1.mass()<<endl;
												
			      passTopMass1 = true;
			      theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			      theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(2,evtWeight);
			      theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);

			      double ttMass = (p4_top0+p4_top1).mass() ;
			      theDir.getObject<TH1>("ttMassType22_predicted")    ->  Fill( ttMass, weight*evtWeight );
			      theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
												
			      ttMassPred22        ->      Accumulate( ttMass, pt, 1,  (1-weight1)*evtWeight );
			    }
			  }
			}
		      }  // end else
		    }  // end i
		  } //hasTightTop
		  else if( hasLooseTop0 )  {
		    //cout<<"case 12"<<endl;
		    //cout<<bTags1.size()<<endl;
		    for( size_t i=0; i<noTags1.size(); i++ )  {
		      double pt = noTags1.at(i)->pt();
		      int bin     = wMistag_      ->  FindBin( pt );
		      double weight = wMistag_    ->  GetBinContent( bin ) ; //dummy value
		      double mistagError = wMistag_ ->GetBinError( bin );

		      p4_top1 = noTags1.at(i)->p4() + bTags1.at(0)->p4();

		      theDir.getObject<TH1>("topMassPred")     ->   Fill( p4_top1.mass(), weight*evtWeight );
		      if( p4_top1.mass() > topMassMin_ && p4_top1.mass() < topMassMax_ )  {
									
			if (pt > jetPt0_) {
										
			  if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			  if (verbose_) cout<<"   hasLooseTop0. hemi 1 has 0 b-tags and p4_top1.mass() "<<p4_top1.mass()<<endl;
										
			  passTopMass1 = true;
			  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			  theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(3,evtWeight);
			  theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);

			  double ttMass = (p4_top0+p4_top1).mass() ;
			  theDir.getObject<TH1>("ttMassType22_predicted")      ->  Fill( ttMass, weight*evtWeight );
			  theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
										
			  ttMassPred22         ->      Accumulate( ttMass, pt, 1,  evtWeight );
			}
		      }
		    }
		  }        
		} // hasWTag0 
		else  {
		  //cout<<"case 2"<<endl;
		  bool passTopMass0 = false;
		  p4_top0.SetPxPyPzE(0,0,0,0);
		  if( hasTightTop1 )  {
		    //cout<<"case 20"<<endl;
		    for( size_t i=0; i<noTags0.size(); i++ )  {
		      double pt = noTags0.at(i)->pt();
		      int   bin   =   wMistag_      ->  FindBin( pt );
		      double weight = wMistag_      ->  GetBinContent( bin );  //dummy value, depend on pt
		      double mistagError = wMistag_ ->GetBinError( bin );

		      if( hasBTag0 )  {
			p4_top0 = noTags0.at(i)->p4() + bTags0.at(0)->p4();
			theDir.getObject<TH1>("topMassPred")     ->  Fill( p4_top0.mass(), weight*evtWeight );
			if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )  {
										
			  if (pt > jetPt0_) {
											
			    if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			    if (verbose_) cout<<"   hasTightTop1. hemi 0 has b-tag and p4_top0.mass() "<<p4_top0.mass()<<endl;
											
											
			    passTopMass0 = true;
			    theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			    theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(1,evtWeight);
			    theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);

			    double ttMass = (p4_top0+p4_top1).mass() ;
			    theDir.getObject<TH1>("ttMassType22_predicted")      ->  Fill( ttMass, weight*evtWeight );
			    theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
											
			    ttMassPred22       ->      Accumulate( ttMass, pt, 1,  evtWeight );
			  }
			}
		      }
		      else {
			double minDr = 9999.0;
			pat::Jet const * nearestJet=NULL;
			for( size_t j=0; j<noTags0.size(); j++ )  {
			  if( i==j )   continue;
			  double dR = reco::deltaR<double>( noTags0.at(i)->eta(), noTags0.at(i)->phi(),
							    noTags0.at(j)->eta(), noTags0.at(j)->phi() );
			  if( dR < minDr )  {
			    minDr = dR ;
			    nearestJet = &(*noTags0.at(j));
			  }
			} //end j
			if( nearestJet )  {
			  //cout<<"case 22"<<endl;
			  p4_top0 = noTags0.at(i)->p4() + nearestJet->p4();
			  int   bin1  =  wMistag_       ->  FindBin( nearestJet->pt() );
			  double weight1  = wMistag_    ->  GetBinContent( bin1 );
			  double mistagError1 = wMistag_ ->GetBinError( bin1 );

			  weight *= (1-weight1);
			  theDir.getObject<TH1>("topMassPred")   ->  Fill( p4_top0.mass(), weight*evtWeight );
			  if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )  {
											
			    if (pt > jetPt0_) {
												
			      if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			      if (verbose_) cout<<"   hasTightTop1. hemi 0 has 0 b-tags and p4_top0.mass() "<<p4_top0.mass()<<endl;
												
			      passTopMass0 = true;
			      theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			      theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(2,evtWeight);
			      theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);

			      double ttMass = (p4_top0+p4_top1).mass() ;
			      theDir.getObject<TH1>("ttMassType22_predicted")    ->  Fill( ttMass, weight*evtWeight );
			      theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
												
			      ttMassPred22      ->      Accumulate( ttMass, pt, 1,  (1-weight1)*evtWeight );
			    }
			  }
			}
		      }  // end else
		    }  // end i
		  } //hasTightTop
		  else if( hasLooseTop1 )  {
		    //cout<<"case 23"<<endl;
		    for( size_t i=0; i<noTags0.size(); i++ )  {
		      double pt = noTags0.at(i)->pt();
		      int   bin =   wMistag_    ->  FindBin( pt );
		      double weight = wMistag_  ->  GetBinContent( bin );  //dummy value
		      double mistagError = wMistag_ ->GetBinError( bin );

		      p4_top0 = noTags0.at(i)->p4() + bTags0.at(0)->p4();
		      theDir.getObject<TH1>("topMassPred")     ->   Fill( p4_top0.mass(), weight*evtWeight );

		      if( p4_top0.mass() > topMassMin_ && p4_top0.mass() < topMassMax_ )  {
									
			if (pt > jetPt0_) {
			  if (verbose_) cout<<" Type 22 background estimation event "<<endl;
			  if (verbose_) cout<<"   hasLooseTop1. p4_top0.mass() "<<p4_top0.mass()<<endl;
										
			  passTopMass0 = true;
			  theDir.getObject<TH1>("Nevents_11sig_12sig_22sig_11bkg_12bkg_22bkg")->Fill(6,evtWeight);
			  theDir.getObject<TH1>("Nevents_Type22_Case123")->Fill(3,evtWeight);
			  theDir.getObject<TH1>("Nevents_type22bkg")->Fill(1,evtWeight);

			  double ttMass = (p4_top0+p4_top1).mass() ;
			  theDir.getObject<TH1>("ttMassType22_predicted")      ->  Fill( ttMass, weight*evtWeight );
			  theDir.getObject<TH1>("ttMassType22_predicted_errorSquared")      ->  Fill( ttMass, mistagError*mistagError );
										
			  ttMassPred22      ->      Accumulate( ttMass, pt, 1, evtWeight );
			}
		      }
		    }
		  }
		} // else 
	      } // end type 2+2 background estimation
	    }//end cascading cuts
			
	
			

		 
			
			
	}//end if preselected
    }//end if passType11 && passType22
}//end analyze



