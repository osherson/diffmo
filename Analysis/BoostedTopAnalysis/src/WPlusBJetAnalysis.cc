#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetAnalysis.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"

using namespace std;


WPlusBJetAnalysis::WPlusBJetAnalysis( const edm::ParameterSet & iConfig,  TFileDirectory & iDir ) :
  theDir( iDir ),
  wPlusBJetType22Selection_( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection")  ),
  wPlusBJetType23Selection_( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection")  ),
  wPlusBJetType33Selection_( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection")  ),
  wJetSelector_ ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<edm::ParameterSet>("BoostedTopWJetParameters") ),
  wMassMin_     ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("wMassMin") ),
  wMassMax_     ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("wMassMax") ),
  topMassMin_   ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("topMassMin") ),
  topMassMax_   ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("topMassMax") ),
  runOnData_    ( iConfig.getParameter<bool>("runOnData") ),
  bTagAlgo_     ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<string>("bTagAlgorithm") ),
  bTagOPM_       ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("bTagOPMedium") ),
  bTagOPL_      ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<double>("bTagOPLoose") ),
  runOnTTbar_   ( iConfig.getParameter<bool>("runOnTTbar") ),
  debug_        ( iConfig.getParameter<bool>("debugOn") ),
  eventCount    (0),
  mistagFileName_   ( iConfig.getParameter<edm::ParameterSet>("WPlusBJetEventSelection").getParameter<string>("mistagFileName") )
{

  cout<< "Instantiate WPlusBJetAnalysis" << endl;
  histograms1d["nJet"]  = theDir.make<TH1F>("nJet",   "Number of Jets",     20,   0,    20);
  histograms1d["jetPt"] = theDir.make<TH1F>("jetPt",  "Jet Pt; Jet Pt (GeV/c^{2})",   200,    0,    1000 );
  histograms1d["jetEta"]  = theDir.make<TH1F>("jetEta", "Jet #eta; Jet #eta",     50,   -4.0,     4.0 );
  histograms1d["jetMass"] = theDir.make<TH1F>("jetMass",  "Jet Mass; Jet Mass (GeV/c^{2})",   200,    0,    1000 );
  histograms1d["nW"]      = theDir.make<TH1F>("nW",     "Number of W",    10,   0,    10 );
  histograms1d["nB"]      = theDir.make<TH1F>("nB",     "Number of B",    10,   0,    10 );
  histograms1d["wMass0"]   = theDir.make<TH1F>("wMass0",  "W Jet Mass",     40,   0,    200 );
  histograms1d["wMass1"]   = theDir.make<TH1F>("wMass1",  "W Jet Mass",     40,   0,    200 );
  histograms1d["nTightTopType22"]   = theDir.make<TH1F>("nTightTopType22",    "Number of Tight Top",    10,   0,  10 );
  histograms1d["nLooseTopType22"]   = theDir.make<TH1F>("nLooseTopType22",    "Number of Loose Top",    10,   0,  10 );
  histograms1d["tightTopMass0Type22"]    = theDir.make<TH1F>("tightTopMass0Type22",   "Tight Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["looseTopMass0Type22"]    = theDir.make<TH1F>("looseTopMass0Type22",   "Loose Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["tightTopMass1Type22"]    = theDir.make<TH1F>("tightTopMass1Type22",   "Tight Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["looseTopMass1Type22"]    = theDir.make<TH1F>("looseTopMass1Type22",   "Loose Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["probeWMass"]             = theDir.make<TH1F>("probeWMass",    "W Jet Mass",   40,   0,  200 );
  histograms1d["probeWMassExp"]          = theDir.make<TH1F>("probeWMassExp", "W Jet Mass",   40,   0,  200 );
  histograms1d["probeWMass1"]             = theDir.make<TH1F>("probeWMass1",    "W Jet Mass",   40,   0,  200 );
  histograms1d["probeWMass2"]             = theDir.make<TH1F>("probeWMass2",    "W Jet Mass",   40,   0,  200 );
  histograms1d["probeTopMass"]           = theDir.make<TH1F>("probeTopMass",  "Top Mass",   100,    0,  500 );
  histograms1d["probeTopMass1"]           = theDir.make<TH1F>("probeTopMass1",  "Top Mass",   100,    0,  500 );
  histograms1d["sideBandWMass0"]         = theDir.make<TH1F>("sideBandWMass0", "W Jet Mass",   40,   0,  200 );
  histograms1d["sideBandTopMass0"]       = theDir.make<TH1F>("sideBandTopMass0", "Top Mass",   100,    0,  500 );
  histograms1d["sideBandWMass1"]         = theDir.make<TH1F>("sideBandWMass1", "W Jet Mass",   40,   0,  200 );
  histograms1d["sideBandTopMass1"]       = theDir.make<TH1F>("sideBandTopMass1", "Top Mass",   100,    0,  500 );
  histograms1d["sideBandWMass2"]         = theDir.make<TH1F>("sideBandWMass2", "W Jet Mass",   40,   0,  200 );
  histograms1d["sideBandWMass3"]         = theDir.make<TH1F>("sideBandWMass3", "W Jet Mass",   40,   0,  200 );
  histograms1d["sideBandTopMass2"]       = theDir.make<TH1F>("sideBandTopMass2", "Top Mass",   100,    0,  500 );
  histograms1d["sideBandTopMass3"]       = theDir.make<TH1F>("sideBandTopMass3", "Top Mass",   100,    0,  500 );

  histograms1d["tightTopMass0Type23"]    = theDir.make<TH1F>("tightTopMass0Type23",   "Tight Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["tightTopMass1Type23"]    = theDir.make<TH1F>("tightTopMass1Type23",   "Tight Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );

  histograms1d["type3TopMass0Type23"]    = theDir.make<TH1F>("type3TopMass0Type23",   "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["type3TopMass1Type23"]    = theDir.make<TH1F>("type3TopMass1Type23",   "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["minPairMass0Type23"]     = theDir.make<TH1F>("minPairMass0Type23",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass1Type23"]     = theDir.make<TH1F>("minPairMass1Type23",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["probePairMassType23"]    = theDir.make<TH1F>("probePairMassType23",   "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["probeTopMassType23"]     = theDir.make<TH1F>("probeTopMassType23",    "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );

  histograms1d["type3TopMass0Type33"]    = theDir.make<TH1F>("type3TopMass0Type33",   "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["type3TopMass1Type33"]    = theDir.make<TH1F>("type3TopMass1Type33",   "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );
  histograms1d["minPairMass0Type33"]     = theDir.make<TH1F>("minPairMass0Type33",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass1Type33"]     = theDir.make<TH1F>("minPairMass1Type33",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["probePairMassType33"]    = theDir.make<TH1F>("probePairMassType33",   "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["probeTopMassType33"]     = theDir.make<TH1F>("probeTopMassType33",    "Type 3 Top Mass; Mass (GeV/c^{2})",   100,  0,  500 );

  histograms1d["minPairMassType33_sel"]     = theDir.make<TH1F>("minPairMassType33_sel",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMassType33_pred"]     = theDir.make<TH1F>("minPairMassType33_pred",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass0Type33_sel"]     = theDir.make<TH1F>("minPairMass0Type33_sel",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass0Type33_pred"]     = theDir.make<TH1F>("minPairMass0Type33_pred",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass1Type33_sel"]     = theDir.make<TH1F>("minPairMass1Type33_sel",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );
  histograms1d["minPairMass1Type33_pred"]     = theDir.make<TH1F>("minPairMass1Type33_pred",    "Min Pair Inv Mass; Mass (GeV/c^{2})",  100,  0,  500 );


  histograms1d["leadJetPt"]       = theDir.make<TH1F>("leadJetPt",      "Leading Jet Pt",           200,  0,    1000 );
  histograms1d["leadJetEta"]      = theDir.make<TH1F>("leadJetEta",     "Leading Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["secJetPt"]       = theDir.make<TH1F>("secJetPt",      "Second Jet Pt",           200,  0,    1000 );
  histograms1d["secJetEta"]      = theDir.make<TH1F>("secJetEta",     "Second Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["thirdJetPt"]       = theDir.make<TH1F>("thirdJetPt",      "Third Jet Pt",           200,  0,    1000 );
  histograms1d["thirdJetEta"]      = theDir.make<TH1F>("thirdJetEta",     "Third Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["fourthJetPt"]       = theDir.make<TH1F>("fourthJetPt",      "Fourth Jet Pt",           200,  0,    1000 );
  histograms1d["fourthJetEta"]      = theDir.make<TH1F>("fourthJetEta",     "Fourth Jet #eta",         50,   -4.0, 4.0 );

  histograms1d["minPairDr0Type33"]       = theDir.make<TH1F>("minPairDr0Type33",      "Min Pair #Delta R",        50,   0.0, 6.0 );
  histograms1d["minPairDPhi0Type33"]     = theDir.make<TH1F>("minPairDPhi0Type33",    "Min Pair #Delta Phi",      50,   0,    3. );
  histograms1d["minPairDr1Type33"]       = theDir.make<TH1F>("minPairDr1Type33",      "Min Pair #Delta R",        50,   0.0, 6.0 );
  histograms1d["minPairDPhi1Type33"]     = theDir.make<TH1F>("minPairDPhi1Type33",    "Min Pair #Delta Phi",      50,   0,    3. );

  histograms1d["wbJetDr0Type22"]         = theDir.make<TH1F>("wbJetDr0Type22",        "W and B Jet #Delta R",     50,   0.0, 6.0 );
  histograms1d["wbJetDPhi0Type22"]       = theDir.make<TH1F>("wbJetDPhi0Type22",      "W and B Jet #Delta Phi",   50,   0.,   3. );
  histograms1d["wbJetDr1Type22"]         = theDir.make<TH1F>("wbJetDr1Type22",        "W and B Jet #Delta R",     50,   0.0, 6.0 );
  histograms1d["wbJetDPhi1Type22"]       = theDir.make<TH1F>("wbJetDPhi1Type22",      "W and B Jet #Delta Phi",   50,   0.,   3. );

  histograms1d["waJetDr0Type22"]         = theDir.make<TH1F>("waJetDr0Type22",        "W and A Jet #Delta R",     50,   0.0, 6.0 );
  histograms1d["waJetDPhi0Type22"]       = theDir.make<TH1F>("waJetDPhi0Type22",      "W and A Jet #Delta Phi",   50,   0.,   3. );
  histograms1d["waJetDr1Type22"]         = theDir.make<TH1F>("waJetDr1Type22",        "W and A Jet #Delta R",     50,   0.0, 6.0 );
  histograms1d["waJetDPhi1Type22"]       = theDir.make<TH1F>("waJetDPhi1Type22",      "W and A Jet #Delta Phi",   50,   0.,   3. );


  histograms1d["topPairDPhiType22"]     = theDir.make<TH1F>("topPairDPhiType22",    "Top Pair #Delta Phi",      50,   0.,   4. );
  histograms1d["topPairDrType22"]       = theDir.make<TH1F>("topPairDrType22",      "Top Pair #Delta R",        50,   0.0,  6.0 );
  histograms1d["pairBJetDPhi0"]    = theDir.make<TH1F>("pairBJetDPhi0",   "Min Pair and B Jet #Delta Phi",  50, 0.0, 3. );
  histograms1d["pairBJetDr0"]      = theDir.make<TH1F>("pairBJetDr0",     "Min Pair and B Jet #Delta R",  50,   0.0,  6 );
  histograms1d["pairBJetDPhi1"]    = theDir.make<TH1F>("pairBJetDPhi1",   "Min Pair and B Jet #Delta Phi",  50, 0.0, 3. );
  histograms1d["pairBJetDr1"]      = theDir.make<TH1F>("pairBJetDr1",     "Min Pair and B Jet #Delta R",  50,   0.0,  6 );

  histograms1d["wJetPt"]          = theDir.make<TH1F>("wJetPt",         "W Jet Pt",                 200,  0,    1000 );
  histograms1d["bJetPt"]          = theDir.make<TH1F>("bJetPt",         "b Jet Pt",                 200,  0,    1000 );
  histograms1d["topPt0Type22"]    = theDir.make<TH1F>("topPt0Type22",         "Top Pt",                   200,  0,    1000 );
  histograms1d["topPt1Type22"]    = theDir.make<TH1F>("topPt1Type22",         "Top Pt",                   200,  0,    1000 );
  histograms1d["ttPtType22"]      = theDir.make<TH1F>("ttPtType22",     "t#bar{t} Pt",              200,  0,    1000 );
  histograms1d["htType22"]        = theDir.make<TH1F>("htType22",               "HT",     400,    0,    2000 );
  histograms1d["ttMassType22"]    = theDir.make<TH1F>("ttMassType22",   "t#bar{t} Inv Mass Type22",   200,  0,  2000 );
  histograms1d["ttMassType23"]    = theDir.make<TH1F>("ttMassType23",   "t#bar{t} Inv Mass Type23",   200,  0,  2000 );
  histograms1d["ttMassType33"]    = theDir.make<TH1F>("ttMassType33",   "t#bar{t} Inv Mass Type33",   200,  0,  2000 );
  histograms2d["topPt0VsttMassType22"]  = theDir.make<TH2F>("topPt0VsttMassType22",  "ttMass Vs Top Pt0", 200,  0,  1000,   200,  0,  1000 );
  histograms2d["topPt1VsttMassType22"]  = theDir.make<TH2F>("topPt1VsttMassType22",   "ttMass Vs Top Pt1",  200,  0,  1000,   200,  0,  1000 );
  histograms2d["ttPtVsttMassType22"]    = theDir.make<TH2F>("ttPtVsttMassType22",     "ttPt Vs ttMass",   200,    0,  1000,   200,  0,  1000 );
  histograms2d["topPt0VsPt1Type22"]     = theDir.make<TH2F>("topPt0VsPt1Type22",      "Top Pt0 Vs Pt1",   200,    0,  1000,   200,  0,  1000 );
  histograms2d["ttDrVsttMassType22"]    = theDir.make<TH2F>("ttDrVsttMassType22",     "tt #Delta R vs ttMass",    50,   0.0,  6.0,  200, 0, 1000 );
  histograms2d["htVsttMassType22"]      = theDir.make<TH2F>("htVsttMassType22",       "ht Vs ttMass",   200,  0,  1000, 200,  0,  1000 );

  histograms1d["ttMassType22_pred"]    = theDir.make<TH1F>("ttMassType22_pred",   "t#bar{t} Inv Mass Type22",   200,  0,  2000 );
  histograms1d["ttMassType23_pred"]    = theDir.make<TH1F>("ttMassType23_pred",   "t#bar{t} Inv Mass Type23",   200,  0,  2000 );
  histograms1d["ttMassType33_pred"]    = theDir.make<TH1F>("ttMassType33_pred",   "t#bar{t} Inv Mass Type33",   200,  0,  2000 );

  histograms2d["minMass0vsMinMass1Type33"]    = theDir.make<TH2F>("minMass0vsMinMass1Type33",   "Min Mass",    100,  0,  500 , 100,  0,  500 );
  histograms2d["topMass0vsTopMass1Type33"]    = theDir.make<TH2F>("topMass0vsTopMass1Type33",   "Top Mass",    100,  0,  500 , 100,  0,  500 );
  histograms2d["topMass0vsTopMass1Type23"]    = theDir.make<TH2F>("topMass0vsTopMass1Type23",   "Top Mass",    100,  0,  500 , 100,  0,  500 );
  histograms2d["topMass0vsTopMass1Type22"]    = theDir.make<TH2F>("topMass0vsTopMass1Type22",   "Top Mass",    100,  0,  500 , 100,  0,  500 );
  histograms2d["nJets0vsNJets1"]              = theDir.make<TH2F>("nJets0vsNJets1",             "N Jets",       10,  0,  10,  10, 0, 10 );
  histograms2d["nB0vsNB1"]                    = theDir.make<TH2F>("nB0vsNB1",                   "N BJets",      10,  0,  10,  10, 0, 10 );


  histograms2d["jetMassVsPt"]     = theDir.make<TH2F>("jetMassVsPt",    "Jet Mass Vs Pt; Jet Pt (GeV/c^{2}); Jet Mass (GeV/c^{2})",    200,  0,    1000,   100,    0,    500 );

  //For b tag rate parameterization
  histograms1d["jetTotal"]     = theDir.make<TH1F>("jetTotal", "jetTotal",    200,    0,    1000 );
  histograms1d["jetTotalE"]    = theDir.make<TH1F>("jetTotalE", "jetTotal",    200,    0,    1000 );
  histograms1d["jetTotalO"]    = theDir.make<TH1F>("jetTotalO", "jetTotal",     200,    0,    1000 );
  histograms1d["bTag_M"]         = theDir.make<TH1F>("bTag_M", "B Tag Rates, Medium",    200,    0,    1000 );
  histograms1d["bTag_L"]         = theDir.make<TH1F>("bTag_L", "B Tag Rates, Loose",    200,    0,    1000 );
  histograms1d["bTag_ME"]        = theDir.make<TH1F>("bTag_ME", "B Tag Rates, Medium",  200,    0,    1000 );
  histograms1d["bTag_MO"]        = theDir.make<TH1F>("bTag_MO", "B Tag Rates, Medium",  200,    0,    1000 );
  histograms1d["bTag_LE"]        = theDir.make<TH1F>("bTag_LE", "B Tag Rates, Loose",  200,    0,    1000 );
  histograms1d["bTag_LO"]        = theDir.make<TH1F>("bTag_LO", "B Tag Rates, Loose",  200,    0,    1000 );
  //For W mistag parameterization
  histograms1d["wTag"]          = theDir.make<TH1F>("wTag",       "wJet",         200,    0,    1000 );
  //MC truth histograms
  histograms1d["ttMass_truth"]  = theDir.make<TH1F>("ttMass_truth",   "t#bar{t} Inv Mass",   200,  0,  2000 );
  histograms1d["ttMassType22_truth"]  = theDir.make<TH1F>("ttMassType22_truth",   "t#bar{t} Inv Mass Type22",   200,  0,  2000 );
  histograms1d["ttMassType23_truth"]  = theDir.make<TH1F>("ttMassType23_truth",   "t#bar{t} Inv Mass Type23",   200,  0,  2000 );
  histograms1d["ttMassType33_truth"]  = theDir.make<TH1F>("ttMassType33_truth",   "t#bar{t} Inv Mass Type33",   200,  0,  2000 );
  histograms1d["nJets_MC"]        = theDir.make<TH1F>("nJets_MC",       "n Jets MC",          20,   0,    20 );
  histograms1d["leadJetPt_MC"]       = theDir.make<TH1F>("leadJetPt_MC",      "Leading Jet Pt",           200,  0,    1000 );
  histograms1d["leadJetEta_MC"]      = theDir.make<TH1F>("leadJetEta_MC",     "Leading Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["secJetPt_MC"]       = theDir.make<TH1F>("secJetPt_MC",      "Second Jet Pt",           200,  0,    1000 );
  histograms1d["secJetEta_MC"]      = theDir.make<TH1F>("secJetEta_MC",     "Second Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["thirdJetPt_MC"]       = theDir.make<TH1F>("thirdJetPt_MC",      "Third Jet Pt",           200,  0,    1000 );
  histograms1d["thirdJetEta_MC"]      = theDir.make<TH1F>("thirdJetEta_MC",     "Third Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["fourthJetPt_MC"]       = theDir.make<TH1F>("fourthJetPt_MC",      "Fourth Jet Pt",           200,  0,    1000 );
  histograms1d["fourthJetEta_MC"]      = theDir.make<TH1F>("fourthJetEta_MC",     "Fourth Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["fifthJetPt_MC"]       = theDir.make<TH1F>("fifthJetPt_MC",      "Fifth Jet Pt",           200,  0,    1000 );
  histograms1d["fifthJetEta_MC"]      = theDir.make<TH1F>("fifthJetEta_MC",     "Fifth Jet #eta",         50,   -4.0, 4.0 );
  histograms1d["sixthJetPt_MC"]       = theDir.make<TH1F>("sixthJetPt_MC",      "Sixth Jet Pt",           200,  0,    1000 );
  histograms1d["sixthJetEta_MC"]      = theDir.make<TH1F>("sixthJetEta_MC",     "Sixth Jet #eta",         50,   -4.0, 4.0 );

  mistagFile_   = TFile::Open( mistagFileName_.c_str() );
  wMistag_      = (TH1F*)mistagFile_   -> Get("wMistag");
  bMistag_      = (TH1F*)mistagFile_   -> Get("bMistag");
  TDirectory * dir = theDir.getBareDirectory();

  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "Module requires the RandomNumberGeneratorService\n";
  }

  CLHEP::HepRandomEngine& engine = rng->getEngine();
  flatDistribution_ = new CLHEP::RandFlat(engine, 0., 1.);

}

void WPlusBJetAnalysis::analyze( const edm::EventBase & iEvent )
{
  if( debug_ )    cout<<"Start Analyzer"<<endl;
  //cout<<"Point 1"<<endl;
  //If run on ttbar MC, only analyze all hadronic events
  if( runOnTTbar_ ) {
    edm::Handle<TtGenEvent>     ttGen;
    iEvent.getByLabel( edm::InputTag("genEvt") , ttGen );
    if( !ttGen.isValid() )    throw cms::Exception("ProductNotFound") <<endl;
    //cout<<"TT Events"<<endl;
    if( !ttGen->isFullHadronic() )  {
      //cout<<"Not Full Hadronic Events"<<endl;
      return;
    }
  }  // end if runOnTTbar_

  double ttTrueMass = TTMass( iEvent );
  histograms1d["ttMass_truth"]        ->  Fill( ttTrueMass );

  if( debug_ )    cout<<"Start Type22 selection"<<endl;
  pat::strbitset retType22 = wPlusBJetType22Selection_.getBitTemplate();
  bool passType22 = wPlusBJetType22Selection_( iEvent, retType22 );

  if( ttTrueMass > 700 )  {
    std::vector<edm::Ptr<pat::Jet> >  const &  pfJets = wPlusBJetType22Selection_.pfJets();
    histograms1d["nJets_MC"]          ->  Fill( pfJets.size() );
    std::vector<std::string>  prefix;
    prefix.push_back( "lead" );  prefix.push_back("sec"); prefix.push_back("third"); 
    prefix.push_back("fourth");  prefix.push_back("fifth"); prefix.push_back("sixth");
    size_t nMax = pfJets.size() > 6 ? 6 : pfJets.size() ;
    for( size_t i=0; i < nMax; i++ ) {
      std::string ptTemp = prefix.at(i) + "JetPt_MC";
      std::string etaTemp = prefix.at(i) + "JetEta_MC";
      histograms1d[ ptTemp.c_str() ]        ->  Fill( pfJets.at(i)->pt() );
      histograms1d[ etaTemp.c_str() ]       ->  Fill( pfJets.at(i)->eta() );
    }
  } // end if tt > 700

  if( debug_ )    cout<<"Start Fourth Jet Pt"<<endl;
  //baseline selection
  if( retType22[string("Fourth Jet Pt")] ) {
    std::vector<edm::Ptr<pat::Jet> >  const &  pfJets = wPlusBJetType22Selection_.pfJets();
    std::vector<edm::Ptr<pat::Jet> >  const &  tWJets = wPlusBJetType22Selection_.wJet0();
    std::vector<edm::Ptr<pat::Jet> >  const &  oWJets = wPlusBJetType22Selection_.wJet1();
    std::vector<edm::Ptr<pat::Jet> >  const &  tbJets = wPlusBJetType22Selection_.bJet0();
    std::vector<edm::Ptr<pat::Jet> >  const &  obJets = wPlusBJetType22Selection_.bJet1();
    std::vector<edm::Ptr<pat::Jet> >  const &  tMinDrPair = wPlusBJetType22Selection_.minDrPair0();
    std::vector<edm::Ptr<pat::Jet> >  const &  oMinDrPair = wPlusBJetType22Selection_.minDrPair1();
    std::vector<edm::Ptr<pat::Jet> >  const &  allJets0 = wPlusBJetType22Selection_.allJets0();
    std::vector<edm::Ptr<pat::Jet> >  const &  allJets1 = wPlusBJetType22Selection_.allJets1();
    edm::Ptr<pat::Jet>   const  & taJet = wPlusBJetType22Selection_.aJet0();
    edm::Ptr<pat::Jet>   const  & oaJet = wPlusBJetType22Selection_.aJet1();

    histograms2d["nJets0vsNJets1"]      ->  Fill( allJets0.size(), allJets1.size() );
    histograms1d["nJet"]      ->  Fill( pfJets.size() );

    double ht=0.0;
    for( size_t i=0; i<pfJets.size(); i++ ) {
      ht += pfJets.at(i)->pt() ;
      histograms1d["jetPt"]     ->  Fill( pfJets.at(i)->pt() );
      histograms1d["jetEta"]    ->  Fill( pfJets.at(i)->eta() );
      histograms1d["jetMass"]   ->  Fill( pfJets.at(i)->mass() );
      pat::strbitset wRet = wJetSelector_.getBitTemplate();
      bool pass = wJetSelector_( *(pfJets.at(i)) , wRet );
      bool passMass = (pfJets.at(i)->mass() > wMassMin_ && pfJets.at(i)->mass() < wMassMax_ ) ;
      if( pass && passMass )  {
        histograms1d["wTag"]    ->  Fill( pfJets.at(i)->pt() );
      }
      if( pfJets.at(i)->pt() > 200 )  {
        histograms2d["jetMassVsPt"]     ->  Fill( pfJets.at(i)->pt() , pfJets.at(i)->mass() );
      }
    }
    histograms1d["leadJetPt"]       ->  Fill( pfJets.at(0)->pt() );
    histograms1d["leadJetEta"]      ->  Fill( pfJets.at(0)->eta() );
    histograms1d["secJetPt"]        ->  Fill( pfJets.at(1)->pt() );
    histograms1d["secJetEta"]       ->  Fill( pfJets.at(1)->eta() );
    histograms1d["thirdJetPt"]       ->  Fill( pfJets.at(2)->pt() );
    histograms1d["thirdJetEta"]      ->  Fill( pfJets.at(2)->eta() );
    histograms1d["fourthJetPt"]        ->  Fill( pfJets.at(3)->pt() );
    histograms1d["fourthJetEta"]       ->  Fill( pfJets.at(3)->eta() );
    histograms1d["htType22"]           ->  Fill( ht );

    int numWJets = tWJets.size() + oWJets.size();
    int numBJets = tbJets.size() + obJets.size();

    histograms1d["nW"]      ->  Fill( numWJets );
    histograms1d["nB"]      ->  Fill( numBJets );
    histograms2d["nB0vsNB1"]      ->  Fill( tbJets.size(),  obJets.size() );
    if( tWJets.size() >= 1 ) {
      histograms1d["wMass0"]      ->  Fill( tWJets.at(0)->mass() );
      histograms1d["wJetPt"]      ->  Fill( tWJets.at(0)->pt() );
    }
    if( oWJets.size() >= 1 ) {
      histograms1d["wMass1"]      ->  Fill( oWJets.at(0)->mass() );
      histograms1d["wJetPt"]      ->  Fill( oWJets.at(0)->pt() );
    }
    if( tbJets.size() >=1 ) 
      histograms1d["bJetPt"]      ->  Fill( tbJets.at(0)->pt() );
    if( obJets.size() >=1 )
      histograms1d["bJetPt"]      ->  Fill( obJets.at(0)->pt() );

    if( wPlusBJetType22Selection_.Type == Type22 ) {
      int numTightTop=0, numLooseTop=0;
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType22Selection_.p4_top0();
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType22Selection_.p4_top1();

      if( retType22[string("has Top0")] ) {
        if ( wPlusBJetType22Selection_.hasTightTop0() ) {
          numTightTop++;
          double deltaR = reco::deltaR<double>( tWJets.at(0)->eta(), tWJets.at(0)->phi(), tbJets.at(0)->eta(), tbJets.at(0)->phi() );
          double deltaPhi = fabs( reco::deltaPhi<double>( tWJets.at(0)->phi(), tbJets.at(0)->phi() ) );
          histograms1d["wbJetDr0Type22"]            ->  Fill( deltaR );
          histograms1d["wbJetDPhi0Type22"]          ->  Fill( deltaPhi );
          histograms1d["tightTopMass0Type22"]       ->  Fill( p4_top0.mass() );
        }
        else {
          numLooseTop++;
          double deltaR = reco::deltaR<double>( tWJets.at(0)->eta(), tWJets.at(0)->phi(), taJet->eta(), taJet->phi() );
          double deltaPhi = fabs( reco::deltaPhi<double>( tWJets.at(0)->phi(), taJet->phi() ) );
          histograms1d["waJetDr0Type22"]            ->  Fill( deltaR );
          histograms1d["waJetDPhi0Type22"]          ->  Fill( deltaPhi );
          histograms1d["looseTopMass0Type22"]       ->  Fill( p4_top0.mass() );
        }
      } // has Top0
      if( retType22[string("has Top1")]   )   {
        if( wPlusBJetType22Selection_.hasTightTop1()  ) {
          numTightTop++;
          double deltaR = reco::deltaR<double>( oWJets.at(0)->eta(), oWJets.at(0)->phi(), obJets.at(0)->eta(), obJets.at(0)->phi() );
          double deltaPhi = fabs( reco::deltaPhi<double>( oWJets.at(0)->phi(), obJets.at(0)->phi() ) );
          histograms1d["wbJetDr1Type22"]            ->  Fill( deltaR );
          histograms1d["wbJetDPhi1Type22"]          ->  Fill( deltaPhi );
          histograms1d["tightTopMass1Type22"]       ->  Fill( p4_top1.mass() );
        }
        else  {
          numLooseTop++;
          double deltaR = reco::deltaR<double>( oWJets.at(0)->eta(), oWJets.at(0)->phi(), oaJet->eta(), oaJet->phi() );
          double deltaPhi = fabs( reco::deltaPhi<double>( oWJets.at(0)->phi(), oaJet->phi() ) );
          histograms1d["waJetDr1Type22"]            ->  Fill( deltaR );
          histograms1d["waJetDPhi1Type22"]          ->  Fill( deltaPhi );
          histograms1d["looseTopMass1Type22"]       ->  Fill( p4_top1.mass() );
        }
      } // has Top1
      histograms1d["nTightTopType22"]   ->  Fill( numTightTop );
      histograms1d["nLooseTopType22"]   ->  Fill( numLooseTop );

      if( passType22 ) {
        histograms1d["ttMassType22"]    ->  Fill( (p4_top0+p4_top1).mass() );
        histograms1d["ttPtType22"]      ->  Fill( (p4_top0+p4_top1).pt() );
        histograms2d["ttPtVsttMassType22"]    ->  Fill( (p4_top0+p4_top1).pt(), (p4_top0+p4_top1).mass() );
        histograms2d["htVsttMassType22"]      ->  Fill( ht, (p4_top0+p4_top1).mass() );
        histograms1d["ttMassType22_truth"]    ->  Fill( ttTrueMass );
        if( runOnData_ )  cout<<"Woohoo, Type2+Type2, Event id, "<<iEvent.id()<<endl;
        double deltaR = reco::deltaR<double>( p4_top0.eta(), p4_top0.phi(), p4_top1.eta(), p4_top1.phi() );
        double deltaPhi = fabs( reco::deltaPhi<double>( p4_top0.phi(), p4_top1.phi() ) );
        histograms1d["topPairDPhiType22"]     ->  Fill( deltaPhi );
        histograms1d["topPairDrType22"]       ->  Fill( deltaR );
        histograms2d["ttDrVsttMassType22"]    ->  Fill( deltaR, (p4_top0+p4_top1).mass() );

        double topPt0, topPt1;
        topPt0 = p4_top0.pt() > p4_top1.pt() ? p4_top0.pt() : p4_top1.pt();
        topPt1 = p4_top0.pt() < p4_top1.pt() ? p4_top0.pt() : p4_top1.pt();
        histograms1d["topPt0Type22"]      ->  Fill( topPt0 );
        histograms1d["topPt1Type22"]      ->  Fill( topPt1 );
        histograms2d["topPt0VsttMassType22"]    ->  Fill( topPt0, (p4_top0+p4_top1).mass() );
        histograms2d["topPt1VsttMassType22"]    ->  Fill( topPt1, (p4_top0+p4_top1).mass() );
        histograms2d["topPt0VsPt1Type22"]       ->  Fill( topPt0, topPt1 );
      }
    } // end if ret hasTwoTops

    if( debug_ )    cout<<"Start Tag Tight and Probe"<<endl;
    //Tight tag one hemisphere and probe the other
    if( wPlusBJetType22Selection_.hasTightTop0() && retType22[string("TopMassCut0")] )  {
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType22Selection_.p4_top1();
      
      //cout<<"aaa"<<endl;
      for(size_t i=0; i<oWJets.size(); i++ )  {
        //cout<<"bbb"<<endl;
        histograms1d["probeWMass"]    ->  Fill( oWJets.at(i)->mass() );
        if( obJets.size() >= 1 )  {
          histograms1d["probeWMass1"]   ->  Fill( oWJets.at(i)->mass() );
        }
      }
      //cout<<"ccc"<<endl;
      if( retType22[string("has Top1")] ) { 
        //cout<<"111"<<endl;
        histograms1d["probeTopMass"]  ->  Fill( p4_top1.mass() );
        if( obJets.size() >= 1 )  {
          //cout<<"222"<<endl;
          histograms1d["probeTopMass1"] ->  Fill( p4_top1.mass() );
        }
      }
      //cout<<"ddd"<<endl;
    }  
    if( wPlusBJetType22Selection_.hasTightTop1() && retType22[string("TopMassCut1")] )  {
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType22Selection_.p4_top0();
      for(size_t i=0; i<tWJets.size(); i++ )  {
        histograms1d["probeWMass"]    ->  Fill( tWJets.at(i)->mass() );
        if( tbJets.size() >= 1 )  {
          histograms1d["probeWMass1"]     ->  Fill(  tWJets.at(i)->mass() );
        }
      }
      if( retType22[string("has Top0")] ) {
        histograms1d["probeTopMass"]  ->  Fill( p4_top0.mass() );
        if( tbJets.size() >= 1 )  {
          histograms1d["probeTopMass1"] ->  Fill(  p4_top0.mass() );
        }
      }
    }  // end if

    //Tag one hemisphere without b tagging, but take the b tagging parameterization
    if( retType22[string("TopMassCut0")] )  {
      const pat::Jet * wJet0 = &(*tWJets.at(0));
      for( size_t i=0; i<allJets0.size(); i++ ) {
        const pat::Jet * aJet = &(*allJets0.at(i));
        if( !sameJet( wJet0, aJet ) ) {
          int bBin;
          double weight;
          bBin = bMistag_   -> FindBin( aJet->pt() );
          weight = bMistag_ -> GetBinContent( bBin );
          for(size_t j=0; j<oWJets.size(); j++ )  {
            histograms1d["probeWMassExp"]   ->  Fill( oWJets.at(j)->mass(), weight );
          }
        } // sameJet
        else ; //cout<<"Same Jet"<<endl;
      } // end if allJet0 size
    }  // end if TopMassCut0
    if( retType22[string("TopMassCut1")] )  {
      const pat::Jet * wJet1 = &(*oWJets.at(0));
      for( size_t i=0; i<allJets1.size(); i++ ) {
        const pat::Jet * aJet = &(*allJets1.at(i));
        if( !sameJet( wJet1, aJet ) ) {
          int bBin;
          double weight;
          bBin = bMistag_   -> FindBin( aJet->pt() );
          weight = bMistag_ -> GetBinContent( bBin );
          for(size_t j=0; j<tWJets.size(); j++ )  {
            histograms1d["probeWMassExp"]   ->  Fill( tWJets.at(j)->mass(), weight );
          }
        } // sameJet
        else ; //cout<<"Same Jet"<<endl;
      } // end if allJet0 size
    }  // end if TopMassCut0

    if( debug_ )      cout<<"Start De-select top"<<endl;
    //De-select top events and get the sideband distributions
    if( retType22[string("has Top0")] )  {
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType22Selection_.p4_top1();
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType22Selection_.p4_top0();
      if( p4_top0.mass() < topMassMin_ )  {
        for( size_t i=0; i<oWJets.size(); i++ )  {
          histograms1d["sideBandWMass0"]   ->  Fill( oWJets.at(i)->mass() );
          if( wPlusBJetType22Selection_.hasTightTop0() )  {
            histograms1d["sideBandWMass1"]  ->  Fill( oWJets.at(i)->mass() );
          }
        }
        if( retType22[string("has Top1")] ) {
          histograms1d["sideBandTopMass0"] ->  Fill( p4_top1.mass() );
          if( wPlusBJetType22Selection_.hasTightTop0() )  {
            histograms1d["sideBandTopMass1"] ->  Fill( p4_top1.mass() );
          }
        }
      }  // top0 mass
      if( p4_top0.mass() > topMassMax_ )  {
        for( size_t i=0; i<oWJets.size(); i++ )  {  
          histograms1d["sideBandWMass2"]   ->  Fill( oWJets.at(i)->mass() );
          if( wPlusBJetType22Selection_.hasTightTop0() )  {
            histograms1d["sideBandWMass3"]  ->  Fill( oWJets.at(i)->mass() );
          }
        }
        if( retType22[string("has Top1")] ) {
          histograms1d["sideBandTopMass2"] ->  Fill( p4_top1.mass() );
          if( wPlusBJetType22Selection_.hasTightTop0() )  {
            histograms1d["sideBandTopMass3"] ->  Fill( p4_top1.mass() );
          }
        }
      }  // top0 mass
    } // end if tight top0
    else if( retType22[string("has Top1")] ) {
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType22Selection_.p4_top1();
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType22Selection_.p4_top0();
      if( p4_top1.mass() < topMassMin_ ) {
        for(size_t i=0; i<tWJets.size(); i++ )  {
          histograms1d["sideBandWMass0"]   ->  Fill( tWJets.at(i)->mass() );
          if(  wPlusBJetType22Selection_.hasTightTop1() ) {
            histograms1d["sideBandWMass1"]  ->  Fill( tWJets.at(i)->mass() );
          }
        }
        if( retType22[string("has Top0")] ) {
          histograms1d["sideBandTopMass0"] ->  Fill( p4_top0.mass() );
          if( wPlusBJetType22Selection_.hasTightTop1() ) {
            histograms1d["sideBandTopMass1"] ->  Fill( p4_top0.mass() );
          }
        }
      }  // end if top1 mass
      if( p4_top1.mass() > topMassMax_ ) {
        for(size_t i=0; i<tWJets.size(); i++ )  {
          histograms1d["sideBandWMass2"]   ->  Fill( tWJets.at(i)->mass() );
          if(  wPlusBJetType22Selection_.hasTightTop1() ) {
            histograms1d["sideBandWMass3"]  ->  Fill( tWJets.at(i)->mass() );
          }
        }
        if( retType22[string("has Top0")] ) {
          histograms1d["sideBandTopMass2"] ->  Fill( p4_top0.mass() );
          if( wPlusBJetType22Selection_.hasTightTop1() ) {
            histograms1d["sideBandTopMass3"] ->  Fill( p4_top0.mass() );
          }
        }
      }  // end if top1 mass
    } // end if tight top1

    if( debug_ )    cout<<"Start lift W Mass"<<endl;
    //Lift the W mass cut on both side, and require b tagging and top mass cut, check the W mass bump
    if( retType22[string("hasTwoTops")] && tbJets.size() >=1 && obJets.size() >=1 ) {
      for(size_t i=0; i<tWJets.size(); i++ )  {
        histograms1d["probeWMass2"]         ->  Fill( tWJets.at(i)->mass() );
      }
      for(size_t i=0; i<oWJets.size(); i++ )  {
        histograms1d["probeWMass2"]         ->  Fill( oWJets.at(i)->mass() );
      }
    }

    if( debug_ )   cout<<"Start Type23"<<endl;
    if( wPlusBJetType22Selection_.Type == Type23 ) {
      pat::strbitset retType23 = wPlusBJetType23Selection_.getBitTemplate();
      bool passType23 = wPlusBJetType23Selection_( iEvent, retType23 );
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType23Selection_.p4_top0();
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType23Selection_.p4_top1();
      if( retType23[string("has Top2")] ) {
        if( wPlusBJetType23Selection_.hasTightTop0() )  { 
          histograms1d["tightTopMass0Type23"]   ->  Fill( p4_top0.mass() );
        }
        else  {
          histograms1d["tightTopMass1Type23"]   ->  Fill( p4_top1.mass() );
        }
        // Type2 hemisphere tag, probe type3 hemisphere
        if( retType23[string("Top2MassCut")] && retType23[string("hasMinPair")] ) {
          if( wPlusBJetType23Selection_.hasTightTop0() ) {
            double minPairMass = (oMinDrPair.at(0)->p4()+oMinDrPair.at(1)->p4()).mass();
            histograms1d["probePairMassType23"]     ->  Fill( minPairMass );
          }
          else  {
            double minPairMass = (tMinDrPair.at(0)->p4()+tMinDrPair.at(1)->p4()).mass();
            histograms1d["probePairMassType23"]     ->  Fill( minPairMass );
          }
        }  // Top2MassCut
      }  // end if has Top2
      if( retType23[string("hasMinPair")] ) {
        if( tMinDrPair.size() == 2 ) {
          double minPairMass = (tMinDrPair.at(0)->p4()+tMinDrPair.at(1)->p4()).mass();
          histograms1d["minPairMass0Type23"]      ->  Fill( minPairMass );
          if( retType23[string("minPairMassCut")] ) {
            histograms1d["type3TopMass0Type23"]   ->  Fill( p4_top0.mass() );
          }
        } // end tMinDrPair
        else {
          double minPairMass = (oMinDrPair.at(0)->p4()+oMinDrPair.at(1)->p4()).mass();
          histograms1d["minPairMass1Type23"]      ->  Fill( minPairMass );
          if( retType23[string("minPairMassCut")] ) {
            histograms1d["type3TopMass1Type23"]   ->  Fill( p4_top1.mass() );
          }
        } // end else
      }  // hasMinPair
      if( passType23 ) {
        histograms1d["ttMassType23"]    ->  Fill( (p4_top0+p4_top1).mass() );
        histograms1d["ttMassType23_truth"]      ->  Fill( ttTrueMass );
        if(runOnData_)    cout<<"Woohoo, Type2+Type3, Event id, run "<<iEvent.id()<<endl;
        double deltaR = reco::deltaR<double>( p4_top0.eta(), p4_top0.phi(), p4_top1.eta(), p4_top1.phi() );
        double deltaPhi = fabs( reco::deltaPhi<double>( p4_top0.phi(), p4_top1.phi() ) );
        //histograms1d["topPairDPhi"]     ->  Fill( deltaPhi );
        //histograms1d["topPairDr"]       ->  Fill( deltaR );        
      } // end passType23
    }  // end if >= 1 WJet and !>= 2 WJet

    if( debug_ )   cout<<"Start Type33"<<endl;
    if( wPlusBJetType22Selection_.Type == Type33  ) {
      pat::strbitset  retType33 = wPlusBJetType33Selection_.getBitTemplate();
      bool passType33 = wPlusBJetType33Selection_( iEvent, retType33 );
      reco::Candidate::LorentzVector const p4_top0 = wPlusBJetType33Selection_.p4_top0();
      reco::Candidate::LorentzVector const p4_top1 = wPlusBJetType33Selection_.p4_top1();

      if( retType33[string("hasMinPair0")] && retType33[string("hasMinPair1")] )  {
        reco::Candidate::LorentzVector  p4_first0 = tMinDrPair.at(0)->p4();
        reco::Candidate::LorentzVector  p4_second0  = tMinDrPair.at(1)->p4();
        reco::Candidate::LorentzVector  p4_first1 = oMinDrPair.at(0)->p4();
        reco::Candidate::LorentzVector  p4_second1  = oMinDrPair.at(1)->p4();
        double dR0 = reco::deltaR<double>( p4_first0.eta(), p4_first0.phi(), p4_second0.eta(), p4_second0.phi() );
        double dR1 = reco::deltaR<double>( p4_first1.eta(), p4_first1.phi(), p4_second1.eta(), p4_second1.phi() );
        double dPhi0 = fabs( reco::deltaPhi<double> ( p4_first0.phi(), p4_second0.phi() ) );
        double dPhi1 = fabs( reco::deltaPhi<double> ( p4_first1.phi(), p4_second1.phi() ) );
        histograms1d["minPairDr0Type33"]    ->  Fill( dR0 );
        histograms1d["minPairDPhi0Type33"]  ->  Fill( dPhi0 );
        histograms1d["minPairDr1Type33"]    ->  Fill( dR1 );
        histograms1d["minPairDPhi1Type33"]  ->  Fill( dPhi1 );
        histograms1d["minPairMass0Type33"]  ->  Fill( (p4_first0+p4_second0).mass() );
        histograms1d["minPairMass1Type33"]  ->  Fill( (p4_first1+p4_second1).mass() );
        if( retType33[string("minPairMassCut0")] ) {
          histograms1d["type3TopMass0Type33"]   ->  Fill( p4_top0.mass() );
        }
        if( retType33[string("minPairMassCut1")] ) {
          histograms1d["type3TopMass1Type33"]   ->  Fill( p4_top1.mass() );
        }
        if( retType33[string("topMass0")] ) {
          histograms1d["probePairMassType33"]   ->  Fill( (p4_first1+p4_second1).mass() );
          histograms1d["probeTopMassType33"]    ->  Fill( p4_top1.mass() );
        }
        if( retType33[string("topMass1")] ) {
          histograms1d["probePairMassType33"]   ->  Fill( (p4_first0+p4_second0).mass() );
          histograms1d["probeTopMassType33"]    ->  Fill( p4_top1.mass() );
        }
        if( passType33 )  {
          histograms1d["ttMassType33"]    ->  Fill( (p4_top0+p4_top1).mass() );
          histograms1d["ttMassType33_truth"]      ->  Fill( ttTrueMass );
          if(runOnData_)    {
            cout<<"Woohoo, Type3+Type3, Event id, "<<iEvent.id()<<endl;
            cout<<"px,py,pz,E,mass "<<endl;
            cout<<"bJet0, "<<tbJets.at(0)->px()<<" "<<tbJets.at(0)->py()<<" "<<tbJets.at(0)->pz()<<" "<<tbJets.at(0)->energy()<<" "<<tbJets.at(0)->mass()<<endl;
            cout<<"bJet1, "<<obJets.at(0)->px()<<" "<<obJets.at(0)->py()<<" "<<obJets.at(0)->pz()<<" "<<obJets.at(0)->energy()<<" "<<obJets.at(0)->mass()<<endl;
            reco::Candidate::LorentzVector  w0 = p4_first0 + p4_second0 ;
            reco::Candidate::LorentzVector  w1 = p4_first1 + p4_second1 ;
            cout<<"w0, "<<w0.px()<<" "<<w0.py()<<" "<<w0.pz()<<" "<<w0.energy()<<" "<<w0.mass()<<endl;
            cout<<"w1, "<<w1.px()<<" "<<w1.py()<<" "<<w1.pz()<<" "<<w1.energy()<<" "<<w1.mass()<<endl;
            cout<<"top0, "<<p4_top0.px()<<" "<<p4_top0.py()<<" "<<p4_top0.pz()<<" "<<p4_top0.energy()<<" "<<p4_top0.mass()<<endl;
            cout<<"top1, "<<p4_top1.px()<<" "<<p4_top1.py()<<" "<<p4_top1.pz()<<" "<<p4_top1.energy()<<" "<<p4_top1.mass()<<endl;
          }
          double deltaR = reco::deltaR<double>( p4_top0.eta(), p4_top0.phi(), p4_top1.eta(), p4_top1.phi() );
          double deltaPhi = fabs( reco::deltaPhi<double>( p4_top0.phi(), p4_top1.phi() ) ) ;
          //histograms1d["topPairDPhi"]     ->  Fill( deltaPhi );
          //histograms1d["topPairDr"]       ->  Fill( deltaR );
        }
      }  // hasMinPair0 && hasMinPair1

      bool hasHF = hasHeavyFlavor( iEvent );

      if( retType33[string("nJets >= 6")] ) { //&& !hasHF ) 
        long eventId = iEvent.id().event();
        eventCount++;
        //Check b tagging rates in multijets events
        //Get parameterization from both odd and even events
        for( size_t i=0; i<pfJets.size(); i++ ) {
          if( pfJets.at(i)->bDiscriminator( bTagAlgo_ ) > bTagOPM_ ) {
            histograms1d["bTag_M"]        ->  Fill( pfJets.at(i)->pt() );
            if( 0 == eventCount%2 )  
              histograms1d["bTag_ME"]     ->  Fill( pfJets.at(i)->pt() );
            else
              histograms1d["bTag_MO"]     ->  Fill( pfJets.at(i)->pt() );
          }
          if( pfJets.at(i)->bDiscriminator( bTagAlgo_ ) > bTagOPL_ ) {
            histograms1d["bTag_L"]        ->  Fill( pfJets.at(i)->pt() );
            if( 0 == eventCount%2 )
              histograms1d["bTag_LE"]     ->  Fill( pfJets.at(i)->pt() );
            else
              histograms1d["bTag_LO"]     ->  Fill( pfJets.at(i)->pt() );
          }
          histograms1d["jetTotal"]      ->  Fill( pfJets.at(i)->pt() );
          if( 0 == eventCount%2 )  
            histograms1d["jetTotalE"]   ->  Fill( pfJets.at(i)->pt() );
          else  
            histograms1d["jetTotalO"]   ->  Fill( pfJets.at(i)->pt() );
        } // end for i

        //Apply the parameterization on even events
        if( 0 != eventCount%2 )  {
          //cout<<"Even event"<<endl;
          double x = flatDistribution_->fire();
          //cout<<"Random number is "<<x<<endl;
          //for random tagger exercise
          std::vector<edm::Ptr<pat::Jet> >  const &  tMinDrPair = wPlusBJetType33Selection_.minDrPair0();
          std::vector<edm::Ptr<pat::Jet> >  const &  oMinDrPair = wPlusBJetType33Selection_.minDrPair1();
          std::vector<edm::Ptr<pat::Jet> >  const &  tbJets = wPlusBJetType33Selection_.bJet0();
          std::vector<edm::Ptr<pat::Jet> >  const &  obJets = wPlusBJetType33Selection_.bJet1();

          bool towards = false;
          if( x < 0.5 )  towards = true;
          //selection only
          if( retType33[string("hasMinPair0")] )  {
            reco::Candidate::LorentzVector  p4_first0 = tMinDrPair.at(0)->p4();
            reco::Candidate::LorentzVector  p4_second0  = tMinDrPair.at(1)->p4();
            histograms1d["minPairMass0Type33_sel"]     ->  Fill( (p4_first0+p4_second0).mass() );
            if( towards )
              histograms1d["minPairMassType33_sel"]     ->  Fill( (p4_first0+p4_second0).mass() );
          }  // hasMinPair0
          //Do combinatorics
          std::vector<Type3>      const & type3Tops0  = wPlusBJetType22Selection_.type3Tops0();
          //cout<<"nJets >= 6 allJets0 size "<<allJets0.size()<<endl;
          //cout<<"type3Tops0 size "<<type3Tops0.size()<<endl;
          //cout<<"BJets0 are "<<tbJets.size()<<endl;

          for( size_t i=0; i<type3Tops0.size(); i++ ) {
            double weight = type3Tops0.at(i).weight ;
            //cout<<"Weight "<<weight<<endl;
            double minPairMass = type3Tops0.at(i).minMass();
            histograms1d["minPairMass0Type33_pred"]    ->  Fill( minPairMass, weight );
            if( towards )
              histograms1d["minPairMassType33_pred"]    ->  Fill( minPairMass, weight );
          }  // end for
          if( retType33[string("hasMinPair1")] ) {
            reco::Candidate::LorentzVector  p4_first1 = oMinDrPair.at(0)->p4();
            reco::Candidate::LorentzVector  p4_second1  = oMinDrPair.at(1)->p4();
            histograms1d["minPairMass1Type33_sel"]     ->  Fill( (p4_first1+p4_second1).mass() );
            if( !towards )
              histograms1d["minPairMassType33_sel"]     ->  Fill( (p4_first1+p4_second1).mass() );
          } // hasMinPair1
          //Do combinatorics
          std::vector<Type3>      const & type3Tops1  = wPlusBJetType22Selection_.type3Tops1();
          //cout<<"nJets >= 6 allJets1 size "<<allJets1.size()<<endl;
          //cout<<"type3Tops1 size "<<type3Tops1.size()<<endl;
          //cout<<"BJets1 are "<<obJets.size()<<endl;

          for( size_t i=0; i<type3Tops1.size(); i++ ) {
            double weight = type3Tops1.at(i).weight ;
            //cout<<"Weight "<<weight<<endl;
            double minPairMass = type3Tops1.at(i).minMass();
            histograms1d["minPairMass1Type33_pred"]    ->  Fill( minPairMass, weight );
            if( !towards )
              histograms1d["minPairMassType33_pred"]    ->  Fill( minPairMass, weight );
          }  // end for
        } // eventId%2 == 0
      } // end nJets >= 6
    }  // end if !>= 1 WJet

    //combinatoric background estimation afterwards
    std::vector<Type2L>     const & looseTops0  = wPlusBJetType22Selection_.looseTops0();
    std::vector<Type2T>     const & tightTops0  = wPlusBJetType22Selection_.tightTops0();
    std::vector<Type3>      const & type3Tops0  = wPlusBJetType22Selection_.type3Tops0();  
    std::vector<Type2L>     const & looseTops1  = wPlusBJetType22Selection_.looseTops1();
    std::vector<Type2T>     const & tightTops1  = wPlusBJetType22Selection_.tightTops1();
    std::vector<Type3>      const & type3Tops1  = wPlusBJetType22Selection_.type3Tops1();

    //cout<<"bein here"<<endl;
    //for type2+type2
    for( size_t i=0; i<tightTops0.size(); i++ ) {
      for( size_t j=0; j<tightTops1.size(); j++ ) {
        FourVector top0 = tightTops0.at(i).top();
        FourVector top1 = tightTops1.at(j).top(); 
        histograms2d["topMass0vsTopMass1Type22"]    ->  Fill( top0.mass(), top1.mass() );
        if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
          double weight = tightTops0.at(i).weight * tightTops1.at(j).weight;
          histograms1d["ttMassType22_pred"]   ->  Fill( (top0+top1).mass(), weight );
        }
      } // end j
      for( size_t j=0; j<looseTops1.size(); j++ ) {
        const FourVector top0 = tightTops0.at(i).top();
        const FourVector top1 = looseTops1.at(j).top();
        histograms2d["topMass0vsTopMass1Type22"]    ->  Fill( top0.mass(), top1.mass() );
        if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
          double weight = tightTops0.at(i).weight * looseTops1.at(j).weight;
          histograms1d["ttMassType22_pred"]   ->  Fill( (top0+top1).mass(), weight );
        }
      }  // end j
    } // end i

    //cout<<"Point1"<<endl;

    for( size_t i=0; i<looseTops0.size(); i++ ) {
      for( size_t j=0; j<tightTops1.size(); j++ ) {
        const FourVector top0 = looseTops0.at(i).top();
        const FourVector top1 = tightTops1.at(j).top();
        histograms2d["topMass0vsTopMass1Type22"]    ->  Fill( top0.mass(), top1.mass() );
        if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
          double weight = looseTops0.at(i).weight * tightTops1.at(j).weight;
          histograms1d["ttMassType22_pred"]   ->  Fill( (top0+top1).mass(), weight );
        }
      } // end j
    } // end i

    //cout<<"Point2"<<endl;
    // for type2+type3
    for( size_t i=0; i<tightTops0.size(); i++ ) {
      for( size_t j=0; j<type3Tops1.size(); j++ ) {
        const FourVector top0 = tightTops0.at(i).top();
        const FourVector top1 = type3Tops1.at(j).top();
        if( type3Tops1.at(j).minMass() > wMassMin_ && type3Tops1.at(j).minMass() < wMassMax_ ) {
          histograms2d["topMass0vsTopMass1Type23"]    ->  Fill( top0.mass(), top1.mass() );
          if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
            double weight = tightTops0.at(i).weight * type3Tops1.at(j).weight;
            histograms1d["ttMassType23_pred"]   ->  Fill( (top0+top1).mass(), weight );
          }
        }
      } // end j
    } // end i

    //cout<<"Point3"<<endl;
    for( size_t i=0; i<type3Tops0.size(); i++ ) {
      for( size_t j=0; j<tightTops1.size(); j++ ) {
        const FourVector top0 = type3Tops0.at(i).top();
        const FourVector top1 = tightTops1.at(j).top();
        if( type3Tops0.at(i).minMass() > wMassMin_ && type3Tops0.at(i).minMass() < wMassMax_ ) {
          histograms2d["topMass0vsTopMass1Type23"]    ->  Fill( top0.mass(), top1.mass() );
          if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
            double weight = type3Tops0.at(i).weight * tightTops1.at(j).weight;
            histograms1d["ttMassType23_pred"]   ->  Fill( (top0+top1).mass(), weight );
          }
        }
      } // end j
    } // end i
    //cout<<"Point4"<<endl;

    // for type3+type3
    for( size_t i=0; i<type3Tops0.size(); i++ ) {
      for( size_t j=0; j<type3Tops1.size(); j++ ) {
        const FourVector top0 = type3Tops0.at(i).top();
        const FourVector top1 = type3Tops1.at(j).top();
        double minPairMass0 = type3Tops0.at(i).minMass();
        double minPairMass1 = type3Tops1.at(j).minMass();
        histograms2d["minMass0vsMinMass1Type33"]    ->  Fill( minPairMass0, minPairMass1 );
        if( minPairMass0 > wMassMin_ && minPairMass0 < wMassMax_ &&
            minPairMass1 > wMassMin_ && minPairMass1 < wMassMax_ )  {

          histograms2d["topMass0vsTopMass1Type33"]      ->  Fill( top0.mass(), top1.mass() );
          if( top0.mass() > topMassMin_ && top0.mass() < topMassMax_ && top1.mass() > topMassMin_ && top1.mass() < topMassMax_ ) {
            double weight = type3Tops0.at(i).weight * type3Tops1.at(j).weight;
            histograms1d["ttMassType33_pred"]   ->  Fill( (top0+top1).mass(), weight );
          }
        } // end minPairMass
      } // end j
    } // end i
    //cout<<"end"<<endl;
  } // end retType22 fourth Jet Pt, baseline selection
}

bool WPlusBJetAnalysis::hasHeavyFlavor( const edm::EventBase& iEvent ) 
{
  //Get genParticles
  edm::Handle< vector<reco::GenParticle> >  h_genParticles;
  iEvent.getByLabel( edm::InputTag("genParticles") , h_genParticles );
  if( !h_genParticles.isValid() )   return false;
  bool hasHF = false;
  for( vector<reco::GenParticle>::const_iterator igen = h_genParticles->begin(); igen != h_genParticles->end(); igen++ )
  {
    int pdgId = fabs( igen->pdgId() );
    if( pdgId == 4 || pdgId == 5 )  hasHF = true;
    if( hasHF )   { //cout<<"Has HF"<<endl;
                    return true; }
  } // end for igen

  return false;
}

double WPlusBJetAnalysis::TTMass( const edm::EventBase& iEvent )
{
  //Get genParticles
  edm::Handle< vector<reco::GenParticle> >  h_genParticles;
  iEvent.getByLabel( edm::InputTag("genParticles") , h_genParticles );
  if( !h_genParticles.isValid() )   return 0.0;
  vector<const reco::GenParticle *>     tops;
  for( vector<reco::GenParticle>::const_iterator igen = h_genParticles->begin(); igen != h_genParticles->end(); igen++ )
  {
    int pdgId = fabs( igen->pdgId() );
    if( igen->status() == 3 && pdgId == 6 )
      tops.push_back( &(*igen) );      
  }  // end for
  if( tops.size() != 2 )  return 1;
  else {
    double mass = (tops.at(0)->p4() + tops.at(1)->p4() ).mass();
    return mass;
  }
}

bool WPlusBJetAnalysis::sameJet( const pat::Jet * jet1, const pat::Jet * jet2 )
{
  double dR = reco::deltaR<double>( jet1->eta(), jet1->phi(), jet2->eta(), jet2->phi() );
  if( dR < 10e-6 )  return true;
  else    return false;
}
