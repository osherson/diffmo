#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooExtendPdf.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "RooNLLVar.h"
#include "RooArgList.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooFormulaVar.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "TRandom.h"
#include <iostream>
#include <sstream>
#include "RooAbsData.h"
#include "RooAddition.h"
#include "RooMinuit.h"
#include "RooRandom.h"

#include <algorithm>

using namespace RooFit ;
using namespace std;

bool verbose;

class Sample {
public:
  Sample(string name, double lum, double genXsec, double nEvts, double kFactor, double guess, string sampName="");
  Sample(string name, string sampName="");
  Sample();
  ~Sample();

  // Maybe these should be rLum.getVal()?
  string const &     name() { return name_;     }
  double              lum() { return lum_;      }
  double          genXsec() { return genXsec_;  }
  double            nEvts() { return nEvts_;    }
  double          kFactor() { return kFactor_;  }
  double            guess() { return guess_;    }
  string const & sampName() { return sampName_; }
  
  RooRealVar   *     rLum() { return rLum_;     }
  RooRealVar   * rGenXsec() { return rGenXsec_; }
  RooRealVar   *   rNevts() { return rNevts_;   }
  RooRealVar   * rKfactor() { return rKfactor_; }
  RooRealVar   *   rGuess() { return rGuess_;   }
  RooRealVar   *   fitVar() { return fitVar_;   }

  RooFormulaVar   *  form() { return form_;     }

  vector<RooRealVar *> rExtraParms() { return rExtraParms_; }
  
  void setLum( double lum )         { lum_     = lum;     }
  void setGenXsec( double genXsec ) { genXsec_ = genXsec; }
  void setNevts( double nEvts )     { nEvts_   = nEvts;  }
  void setKfactor( double kFactor ) { kFactor_ = kFactor; }
  void setGuess( double guess )     { guess_   = guess;   }

  void setFormula( RooFormulaVar * form) {
    if( form_ ) delete form_;
     form_ = form;
     if(verbose) {
      cout << name_ << "Formula Set, dumping contents " << endl;
      form_->printMultiline(cout,kFALSE);
    }
  }
  
  void resetFitVar() {
    fitVar_->setVal(guess_);
  }
  
  // Alternatively, we could have (double parm, string name) and use the name
  void push_back( double parm ) {
    stringstream tmpString;
    tmpString.str("");
    unsigned int i = extraParms_.size();
    tmpString << name_ << "_parm" << i;

    extraParms_.push_back( parm );
    rExtraParms_.push_back( new RooRealVar( tmpString.str().c_str(), tmpString.str().c_str(), parm ) );
  }
  
private:
  string       name_;
  double       lum_;
  double       genXsec_;
  double       nEvts_;
  double       kFactor_;
  double       guess_;
  string       sampName_; //! We only need this so we can load the same histograms but have everything else with different names
  RooRealVar * rLum_;
  RooRealVar * rGenXsec_;
  RooRealVar * rNevts_;
  RooRealVar * rKfactor_;
  RooRealVar * rGuess_;
  //RooRealVar * nSamp_; This is the equivalent of nMC for all templates, but we do not have this anywhere yet

  RooRealVar    * fitVar_;
  RooFormulaVar * form_;
  
  vector<double >        extraParms_;
  vector<RooRealVar * > rExtraParms_;
  
};

Sample::Sample() {}

Sample::Sample(string name, double lum, double genXsec, double nEvts, double kFactor, double guess, string sampName):
  name_(name), lum_(lum), genXsec_(genXsec), nEvts_(nEvts), kFactor_(kFactor), guess_(guess), sampName_(sampName) {
  if(verbose) cout << "Creating Sample " << name_ << endl;
  rLum_     = new RooRealVar((name_+"_lum").c_str(),     (name_+"_lum").c_str(),     lum_     );
  rGenXsec_ = new RooRealVar((name_+"_genXsec").c_str(), (name_+"_genXsec").c_str(), genXsec_ );
  rNevts_   = new RooRealVar((name_+"_nEvts").c_str(),   (name_+"_nEvts").c_str(),   nEvts_   );
  rKfactor_ = new RooRealVar((name_+"_kFactor").c_str(), (name_+"_kFactor").c_str(), kFactor_ );
  rGuess_   = new RooRealVar((name_+"_guess").c_str(),   (name_+"_guess").c_str(),   guess_   );
  fitVar_   = new RooRealVar((name_+"_fitVar").c_str(),  (name_+"_fitVar").c_str(),  guess_, 0.0001, 10000  );

  form_ = new RooFormulaVar((name_+"_sampForm").c_str(), (name_+"_sampForm").c_str(),
                            "@0*@1*@2/@3*@4", RooArgSet( *fitVar_, *rLum_, *rGenXsec_, *rNevts_, *rKfactor_ ) );

  if(verbose) {
    cout << "Created Sample " << name_ << endl << "Dumping Formula " << endl;
    form_->printMetaArgs(cout);
    form_->printMultiline(cout,kFALSE);
  }
}

Sample::Sample(string name, string sampName):
  name_(name), sampName_(sampName) {
  fitVar_   = new RooRealVar((name_+"_fitVar").c_str(),  (name_+"_fitVar").c_str(),  10, 0.0001, 10000 );
}

Sample::~Sample() {
  //right now this is broken with the copy constructor...
  /* if ( rLum_     ) delete rLum_;
  if ( rGenXsec_ ) delete rGenXsec_;
  if ( rNevts_   ) delete rNevts_;
  if ( rKfactor_ ) delete rKfactor_;
  if ( form_     ) delete form_;

  for ( unsigned int i=0; i<rExtraParms_.size(); ++i) {
    if ( rExtraParms_[i] ) delete rExtraParms_[i];
  }
  */
}
  
class Template {
public:
  Template(TH1F histo, string name, RooRealVar& binningVar, Sample theSample);
  ~Template();

  TH1F *           hist() { return &hist_;          }
  string const &   name() { return name_;           }
  double            nmc() { return nmc_;            }
  double           norm() { return norm_->getVal(); }
  RooDataHist *   hdata() { return rdh_;            }
  RooExtendPdf *   epdf() { return rep_;            }
  
private:
  TH1F            hist_;       //< Input histogram for fit PDFs
  string          name_;       //< Name of this template
  double          nmc_;        //< Actual number of input MC events
  RooRealVar    * rNmc_;       //< Actual number of input MC events
  RooDataHist   * rdh_;        //< DataHist for fit
  RooHistPdf    * rhp_;        //< HistPdf for fit
  RooFormulaVar * norm_;       //< Current normalization at any point in fit
  RooExtendPdf  * rep_;        //< Extended pdf for fit...what we're actually using
  RooRealVar      binningVar_; //< What we're binning in (Ht or secvtxmass)
};

Template::Template(TH1F hist, string name, RooRealVar& binningVar, Sample theSample):
  hist_(hist),name_(name),binningVar_(binningVar) {
  //  nmc_     = hist_.GetEntries();
  nmc_     = hist_.Integral(); // must be used for secvtxmass plots...note there are no overflow/underflow
  rNmc_    = new RooRealVar((name_+"_nmc").c_str(), (name_+"_nmc").c_str(), nmc_);
  rdh_     = new RooDataHist((name_+"_datahist").c_str(),(name_+"_datahist").c_str(), binningVar_, &hist_);
  rhp_     = new RooHistPdf((name_+"_pdf").c_str(),(name_+"_pdf").c_str(), binningVar_, *rdh_);
  norm_    = new RooFormulaVar((name_+"_norm").c_str(), (name_+"_norm").c_str(),"@0*@1",
                               RooArgSet(*(theSample.form()), *rNmc_));
  rep_     = new RooExtendPdf((name_+"_epdf").c_str(), (name_+"_epdf").c_str(), *rhp_, *norm_);
  if ( verbose ) {
    cout << "Number of MC events of " << name_.c_str() << " is " << nmc_ << endl
         << "Value of current normalization is " << norm_->getVal() << endl;
  }
}

Template::~Template() {
  if ( rdh_     ) delete rdh_;
  if ( rhp_     ) delete rhp_;
  if ( rep_     ) delete rep_;
  if ( norm_    ) delete norm_;
}

class JetTagBin {
public:
  JetTagBin(TFile & file, string jtBinName, vector<Sample> theSamples);
  ~JetTagBin();
  
  string const &        jtBinName() const { return jtBinName_;   }
  RooRealVar const *     jtBinVar() const { return jtBinVar_;    }
  RooAbsData *             jtData()       { return jtData_;      }
  RooNLLVar *                 nll()       { return jtNll_;       } 
  vector<Template*> & jtTemplates()       { return jtTemplates_; }
  
  void generateData();
  void inputData( TFile * file );
  void makeNLLVar();
  void makeNLLVar(RooAbsData * abs_data);
  void plotOn( RooPlot * frame );

  double nevt() {
    double sum = 0;
    for (unsigned int i=0; i<jtTemplates_.size(); ++i) {
      sum+=jtTemplates_[i]->norm();
    }
    return sum;
  }
  
private:
  string              jtBinName_;      //< Bin Name
  TH1F              * jtDataHist_;     //< Jet-tag bin template for PDF
  RooRealVar        * jtBinVar_;       //< Jet-tag bin bin variable
  RooAbsData        * jtData_;         //< Jet-tag bin for data to fit to
  RooAbsPdf         * jtPdf_;          //< The fitting pdf for this jet-tag bin
  RooNLLVar         * jtNll_;          //< Likelihood variable
  vector<Template*>   jtTemplates_;    //< Vector of templates representing the various species (ttbar,W+Jets, etc)
};

JetTagBin::JetTagBin(TFile & file, string jtBinName, vector<Sample> theSamples):
  jtBinName_(jtBinName)
{
  jtBinVar_= new RooRealVar(jtBinName.c_str(), jtBinName.c_str(), 50, 600);
  int nbins_def = 0;
  for(unsigned int i=0;i<theSamples.size();++i) {
    string name = theSamples[i].name()+jtBinName;
    string histname = "";
    if ( theSamples[i].sampName()=="" ) histname = name;
    else histname = theSamples[i].sampName()+jtBinName;
    if ( verbose ) cout << "Opening histogram " << histname << endl;
    TH1F * tempHisto = (TH1F*)file.Get(histname.c_str());
    TH1F * tempHisto2;
    if(tempHisto==0) {
      cout << "Template " << name << " does not exist" << endl;
      assert(0); // maybe we don't assert but rather break/continue so we can easily work with different jtbin names
    }
    else {
      tempHisto2 = (TH1F*) tempHisto->Clone();
      //      delete tempHisto;
    }

    // We set the binning, assuming all samples have the same binning
    int nbins = tempHisto2->GetNbinsX();
    if(i==0) {
      nbins_def = nbins;
      int xlo   = tempHisto2->GetBinLowEdge(1);
      int xhi   = tempHisto2->GetBinLowEdge(nbins + 1);
      RooUniformBinning jtBinning(xlo,xhi,nbins_def);
      jtBinVar_->setBinning(jtBinning);
    }
    else {
      if (nbins!=nbins_def) {
        cout << "something here " << endl;
        assert(0);
      }
    }
    
    Template * temp = new Template(*tempHisto2, name, *jtBinVar_, theSamples[i]);
    jtTemplates_.push_back( temp );
    delete tempHisto2;

  }
  if ( verbose ) cout << "Making combined pdf" << endl;
  RooArgList TemplateList;
  for(unsigned int i=0;i<jtTemplates_.size();++i) {
    TemplateList.add(*(jtTemplates_[i]->epdf()));
  }
  jtPdf_ = new RooAddPdf((jtBinName_+"_pdf").c_str(),(jtBinName_+"_pdf").c_str(),TemplateList);
  jtData_ = 0;
  jtNll_ = 0;
}

void JetTagBin::generateData()
{
  if ( jtData_ ) delete jtData_;
  int nToGen = (int) nevt()+.5;
  int nToGenPoisson = RooRandom::randomGenerator()->Poisson(nToGen) ;
  jtData_ = jtPdf_->generateBinned( RooArgList(*jtBinVar_), nToGenPoisson, Verbose(1));
  if ( verbose ) cout << "Generated data histogram with " << jtData_->numEntries() << " bins and " 
                      << jtData_->sumEntries() << " events" << endl
                      << "Using seed: " << RooRandom::randomGenerator()->GetSeed() << endl;
}

void JetTagBin::inputData( TFile * file ) 
{
  TH1F * tempDataHist = (TH1F*)file->Get(("Data_"+jtBinName_).c_str());
  if ( verbose ) cout << "Loaded Data_" << jtBinName_ << " from the file." << endl;
  jtDataHist_ = (TH1F*) tempDataHist->Clone();
  //include check on binning with respect to other histograms
  if ( jtData_ ) delete jtData_;
  jtData_ = new RooDataHist(("Data_"+jtBinName_).c_str(),("Data_"+jtBinName_).c_str(),*jtBinVar_,Import(*jtDataHist_));
}

void JetTagBin::makeNLLVar()
{
  if ( verbose ) cout << "Creating nllVar" << endl;
  if ( jtNll_ ) delete jtNll_;
  jtNll_ = new RooNLLVar((jtBinName_+"_nll").c_str(), (jtBinName_+"_nll").c_str(), *jtPdf_, *jtData_, Extended(kTRUE));
}

void JetTagBin::makeNLLVar( RooAbsData * abs_data )
{
  if ( verbose ) cout << "Creating nllVar" << endl;
  if ( jtNll_ ) delete jtNll_;
  jtNll_ = new RooNLLVar((jtBinName_+"_nll").c_str(), (jtBinName_+"_nll").c_str(), *jtPdf_, *abs_data, Extended(kTRUE));
}

void JetTagBin::plotOn( RooPlot * frame )
{
  if ( verbose ) cout << "Plotting data and combined pdf" << endl;
  jtData_->plotOn(frame);
  jtPdf_ ->plotOn(frame);
  vector<Template*>::iterator i = jtTemplates_.begin(), iend = jtTemplates_.end();
  int color=1;
  for ( ; i != iend ; ++i ) {
    color++;
    if (color == 5) color++;  // only dead yellow is good yellow
    if ( verbose ) cout << "Plotting component " << (*i)->epdf()->GetName()
                        << " with color " << color << endl;
    jtPdf_->plotOn(frame,Components((*i)->epdf()->GetName()),LineColor(color));
  }                                                               
}

JetTagBin::~JetTagBin() {
  if ( jtDataHist_ ) delete jtDataHist_;
  if ( jtBinVar_   ) delete jtBinVar_;
  if ( jtData_     ) delete jtData_;
  if ( jtPdf_      ) delete jtPdf_;
  if ( jtNll_      ) delete jtNll_;
}

class SHyFT {
public:

  SHyFT( string const & fileName, vector<Sample> theSamples );
  SHyFT() { file_ = 0; }
  ~SHyFT() { if ( file_ ) file_->Close(); }

  void init();
  
  void generateData(int seed); 
  void makeNLLVars();
  void makeNLLVars(vector< JetTagBin * > bins_ref);
  void fit(bool verbose);
  void print();
  void plot();
  map<string,TH1F *> histos() { return histos_; }

  vector< JetTagBin * > bins() { return bins_; }

  TFile * file() { return file_; }
  
protected:
  vector< JetTagBin * > bins_;
  vector< RooNLLVar * > nlls_;
  TFile * file_;
  vector<Sample> samples_;
  map<string,TH1F *> histos_;
  int nPEs_;

};

SHyFT::SHyFT( string const & fileName, vector<Sample> theSamples ) :
  file_( new TFile(fileName.c_str(),"r") ),
  samples_(theSamples),
  nPEs_(0)
{
  stringstream tmpString;
  
  for(int n_jets = 1; n_jets <=5; ++n_jets) {
    for(int n_tags = 1; n_tags <=2; ++n_tags) { // will run only once for now, but must run later until 3
      if(n_tags > n_jets) continue;
      tmpString.str("");
      // eventually we will have a loop over all jets with 0T as a fixed string for ht and mt
      // then we loop over jets and tags  like this for secvtxmass
      //tmpString << "_hT_" << n_jets << "j"; //_" << n_tags << "t";
      tmpString << "_" << n_jets << "j_" << n_tags << "t";
      bins_.push_back(new JetTagBin(*file_, tmpString.str(), samples_ ) );
    }
  }
  //Some Basic Plots here
  for(unsigned int i = 0; i<theSamples.size(); ++i) {
    double guess = theSamples[i].guess();

    histos_[(samples_[i].name()+"_fitVar")] = new TH1F((samples_[i].name()+"_fitVar").c_str(),
                                                       (samples_[i].name()+"_fitVar").c_str(), 100, guess*0.6, guess*1.67);

    histos_[(samples_[i].name()+"_fitErr")] = new TH1F((samples_[i].name()+"_fitErr").c_str(),
                                                       (samples_[i].name()+"_fitErr").c_str(), 100, 0.0, guess*0.15);

    histos_[(samples_[i].name()+"_fitPull")] = new TH1F((samples_[i].name()+"_fitPull").c_str(),
                                                        (samples_[i].name()+"_fitPull").c_str(), 100, -5.0, 5.0);
  }
}

void SHyFT::init() {
  for(unsigned int i=0; i < samples_.size(); ++i) {
    samples_[i].resetFitVar();
  }
  nPEs_++;
}

void SHyFT::generateData(int seed)
{
  if ( seed>0 && nPEs_==1 ) RooRandom::randomGenerator()->SetSeed(seed);
  if ( verbose ) cout << "Generating Pseudo Data with seed: " << seed << endl;
  for(unsigned int i=0;i<bins_.size();++i) {
    bins_[i]->generateData();
  }
}

void SHyFT::makeNLLVars()
{
  if ( verbose ) cout << "Making NLL variables" << endl;
  for(unsigned int i=0;i<bins_.size();++i) {
    bins_[i]->makeNLLVar();
  }
}

void SHyFT::makeNLLVars(vector< JetTagBin * > bins_ref)
{
  if ( verbose ) cout << "Making NLL variables" << endl;
  for(unsigned int i=0;i<bins_.size();++i) {
    bins_[i]->makeNLLVar( bins_ref[i]->jtData_ );
  }
}

void SHyFT::fit(bool verbose)
{
  if ( verbose ) cout << "Fitting" << endl;
  RooArgSet nllset;
  for(unsigned int i=0;i<bins_.size();++i) {
    nllset.add(*bins_[i]->nll());
  }
  
  RooAddition nllsum("nllsum","nllsum",nllset);
  RooMinuit m(nllsum);

  if ( verbose ) m.setVerbose(kTRUE);
  else {
    m.setVerbose(kFALSE);
    m.setPrintLevel(-1);
  }
  m.migrad();
  m.hesse();
  //  m.minos();
  if ( verbose ) {
    RooFitResult * f = m.save();
    f->Print("v");
  }
}

void SHyFT::print()
{
  cout << "Printing" << endl;
  for(unsigned int i=0;i<bins_.size();++i) {
    bins_[i]->nll()->Print();
  }
}

void SHyFT::plot()
{
  //if ( 0 ) { //We don't want to have this for every event
  if ( verbose && nPEs_<10 ) { //We don't want to have this for every event
    cout << "Plotting" << endl;
    stringstream tempstring;
    tempstring.str("");
    tempstring << nPEs_;
    for ( vector< JetTagBin * >::iterator ibin = bins_.begin(),
            binsBegin = bins_.begin(), binsEnd = bins_.end(); ibin != binsEnd; ++ibin ) {
      
      cout << "Plotting bin " << ibin - binsBegin+1 << endl;
      JetTagBin & bin = **ibin;
      TCanvas * c1    = new TCanvas(("c"+bin.jtBinName()).c_str(), "Combined Fit");
      RooPlot * frame = bin.jtBinVar()->frame();
      bin.plotOn( frame );
      frame->Draw();
      c1->SaveAs(("fitter"+bin.jtBinName()+"_"+tempstring.str()+".png").c_str());
      delete c1;
    }
  }
  for(unsigned int i=0; i<samples_.size(); ++i) {
    double value = samples_[i].fitVar()->getVal();
    double error = samples_[i].fitVar()->getError();
    if ( verbose ) cout << samples_[i].name() << "_fitVar = " << value << " +- " << error << endl;
    double pull = 0;
    if ( error > 0.00001 ) pull = ( value - samples_[i].guess() ) / error;
    histos_[samples_[i].name()+"_fitVar"] ->Fill(value);
    histos_[samples_[i].name()+"_fitErr"] ->Fill(error);
    histos_[samples_[i].name()+"_fitPull"]->Fill(pull);
  }   
}

void Fitter(string fileName, int maxPEs, int seed=0, string fileName_ref="")
{
  //  RooRandom::randomGenerator()->Reset();
  verbose = false;
  if(maxPEs<=10) verbose = true;

  Sample ttbar = Sample("Top", 10, 90, 221131, 1, 1);
  /*ttbar.setGuess( 1000 );
  ttbar.push_back(21781);
  RooFormulaVar * form_ttbar = new RooFormulaVar("form_ttbar","form_ttbar",
                                                 "@0/@1", RooArgSet( *ttbar.fitVar(), *ttbar.rExtraParms()[0]) );
  ttbar.setFormula( form_ttbar);
  */
  Sample wjets = Sample("Wqq", 10, 17810, 1204434, 1, 1);
  /*wjets.push_back(25965);
  wjets.setGuess( 7000 );
  RooFormulaVar * form_wjets = new RooFormulaVar("form_wjets","form_wjets",
                                                 "@0/@1", RooArgSet( *wjets.fitVar(), *wjets.rExtraParms()[0]) );
  wjets.setFormula( form_wjets);
  */
  //Sample wbb = Sample("Wbb", 10, 17810, 1204434, 1, 1);
  /*wjets.push_back(25965);
  wjets.setGuess( 7000 );
  RooFormulaVar * form_wjets = new RooFormulaVar("form_wjets","form_wjets",
                                                 "@0/@1", RooArgSet( *wjets.fitVar(), *wjets.rExtraParms()[0]) );
  wjets.setFormula( form_wjets);
  */
  //Sample wcc = Sample("Wbb", 10, 17810, 1204434, 1, 1);
  /*wjets.push_back(25965);
  wjets.setGuess( 7000 );
  RooFormulaVar * form_wjets = new RooFormulaVar("form_wjets","form_wjets",
                                                 "@0/@1", RooArgSet( *wjets.fitVar(), *wjets.rExtraParms()[0]) );
  wjets.setFormula( form_wjets);
  */
  vector<Sample> theSamples;
  theSamples.push_back(ttbar);
  theSamples.push_back(wjets);

  //For references
  vector<Sample> theSamples_ref;
  SHyFT * shyft_ref=0;
  bool have_ref = false;
  if ( fileName_ref != "" ) have_ref = true;

  if ( have_ref ) {
    Sample ttbar_ref(  "Top_ref", 10,    90,  221131, 1.5, 1,   "Top");
    ttbar_ref.push_back(21781);
    Sample wjets_ref("Wjets_ref", 10, 17810, 1204434,   1, 1, "Wjets");
    wjets_ref.push_back(25965);
    theSamples_ref.push_back(ttbar_ref);
    theSamples_ref.push_back(wjets_ref);
    shyft_ref = new SHyFT(fileName_ref, theSamples_ref);
  }
  SHyFT shyft(fileName, theSamples);
  for(int numPEs = 0; numPEs < maxPEs; ++numPEs) {
    shyft.init();
    if ( !have_ref ) {
      shyft.generateData(seed);
      shyft.makeNLLVars();
    } 
    else {
      shyft_ref->init();
      shyft_ref->generateData(seed);
      shyft.makeNLLVars(shyft_ref->bins());
    }
    if ( verbose ) shyft.print();
    shyft.fit(verbose);
    shyft.plot();
    if ( numPEs % 100 == 0 ) cout << "NEVENT IS " << numPEs << endl;
  }
  TCanvas * c1 = new TCanvas("plots", "plots");
  for(unsigned int i = 0; i<theSamples.size(); ++i) {
    stringstream nPEs;
    nPEs.str("");
    nPEs << maxPEs;
    shyft.histos()[theSamples[i].name()+"_fitVar"]->Draw();
    c1->SaveAs((theSamples[i].name()+"_fitVar_"+nPEs.str()+".png").c_str());
    shyft.histos()[theSamples[i].name()+"_fitErr"]->Draw();
    c1->SaveAs((theSamples[i].name()+"_fitErr_"+nPEs.str()+".png").c_str());
    shyft.histos()[theSamples[i].name()+"_fitPull"]->Draw();
    c1->SaveAs((theSamples[i].name()+"_fitPull_"+nPEs.str()+".png").c_str());
  }
  delete c1;
  
}

