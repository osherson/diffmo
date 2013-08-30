#include "TROOT.h"
#include <iostream>
#include <cmath>
#include "TMath.h"

#include "Analysis/TTBSMTuplesAnalysis/interface/PredictedDistribution.h"

using namespace std;

/*----------------------------------------------*/
/* CONSTRUCTORS                                 */
/*----------------------------------------------*/

//=============sets 1D histograms with fixed bins
PredictedDistribution::PredictedDistribution(TH1D const * mtx, const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup) : 
  _fNDataHisto(nbinsx + 2),
  h1_r(mtx), 
  h1_p(0), h1_o(0), h1_tgb(0),
  _errsSet(false)
{
  h1_o = new TH1D(name, title, nbinsx, xlow, xup);
  h1_o->Sumw2();
  char name2[20]; sprintf(name2,"%s_pred",name);
  h1_p = new TH1D(name2, title, nbinsx, xlow, xup);
  h1_p->Sumw2();
  char name3[20]; sprintf(name3,"%s_tgb",name);
  h1_tgb = new TH1D(name3, title, nbinsx, xlow, xup);
  h1_tgb->Sumw2();
  char name4[20]; sprintf(name4, "%s_weights", name);
  h_weights = new TH2D(name4, name4, nbinsx, xlow, xup, mtx->GetNbinsX(), mtx->GetXaxis()->GetXbins()->GetArray()  );
  h_weights->Sumw2();
}
   
//=============sets histograms with bin array
PredictedDistribution::PredictedDistribution(TH1D const * mtx, const char* name, const char* title, Int_t nbinsx, const Double_t* xbins) : 
  _fNDataHisto(nbinsx + 2),
  h1_r(mtx),
  h1_p(0), h1_o(0), h1_tgb(0)
{
  h1_o = new TH1D(name, title, nbinsx, xbins);
  h1_o->Sumw2();
  char name2[20]; sprintf(name2,"%s_pred",name);
  h1_p = new TH1D(name2, title, nbinsx, xbins);
  h1_p->Sumw2();
  char name3[20]; sprintf(name3,"%s_tgb",name);
  h1_tgb = new TH1D(name3, title, nbinsx, xbins);
  h1_tgb->Sumw2();
  char name4[20]; sprintf(name4, "%s_weights", name);
  h_weights = new TH2D(name4, name4, nbinsx, xbins, mtx->GetNbinsX(), mtx->GetXaxis()->GetXbins()->GetArray() );
}

/*----------------------------------------------*/
/*  DESTRUCTORS                                 */
/*----------------------------------------------*/
PredictedDistribution::~PredictedDistribution() {
  if ( h1_p != 0 ) {delete h1_p; h1_p=0;}
  if ( h1_o != 0 ) {delete h1_o; h1_o=0;}
  if ( h1_tgb !=0) {delete h1_tgb; h1_tgb=0;}
  if ( h_weights !=0) { delete h_weights; h_weights = 0;}
}


//=============fill all per-jet histograms
// This method is broken for Sal's new matrix with phi as separate histogram
void PredictedDistribution::Accumulate(Float_t X, Float_t et, Bool_t tagged, Float_t weightFactor ) {
  //Fill returns the bin, but returns -1 for both over/underflow, so you must do two steps
  
  // Remember to include the weight factor, in case we are looping over
  // the jets multiple times
  
  Int_t bin = h1_r->GetXaxis()->FindBin( et );
  Float_t rate   = h1_r->GetBinContent(bin);
  //  Float_t err    = h1_r->GetBinError(bin);

  //  Int_t binx= h1_o->FindBin(X);

//   cout << "---------" << endl;
//   cout << "filling: " << endl;
//   cout << "bin = " << bin << ", binx = " << binx << ", weightFactor = " << weightFactor << ", rate = " << rate << ", err = " << err << endl;

   //fill observed
  if ( weightFactor > 0.0 ) {
    h1_o->Fill(X,tagged ? weightFactor : 0.);
    //fill predicted
    h1_p->Fill(X,rate * weightFactor ); 
    // always fill taggable
    h1_tgb->Fill(X, weightFactor);
    // always fill weights
    h_weights->Fill(X, et, weightFactor);
  }
  
}

//=============call these three functions at the end of the job
void PredictedDistribution::SetCalculatedErrors() {
  if (! _errsSet) {
    if ( h1_p == 0 ) {
      std::cout << "We have no tags in PredictedDistribution, we're bailing out" << std::endl;
      return;
    }

//     h_weights->Draw("colz");

//     cout << "======== Setting Calculated Errors =========" << endl;

//     cout << "nbinsx = " << h_weights->GetNbinsX() << endl;
//     cout << "nbinsy = " << h_weights->GetNbinsY() << endl;

    for (int i=0; i<=h_weights->GetNbinsX(); ++i) {

      Double_t a1 = 0.;
      Double_t a2 = 0.;
      Double_t den = 0.0;

//       cout << "------ bin " << i << " --------" << endl;
      for (int j = 0; j <= h_weights->GetNbinsY(); ++j) {


	a1 += h1_r->GetBinContent(j) * h_weights->GetBinError(i,j);
	a2 += h1_r->GetBinError(j)   * h_weights->GetBinContent(i,j);
	den+= h1_r->GetBinContent(j) * h_weights->GetBinContent(i,j);


// 	cout << "i = " << i << ", j = " << j << endl;
// 	cout << "   rate = " << h1_r->GetBinContent(j) << endl;
// 	cout << "  drate = " << h1_r->GetBinError(j) << endl;
// 	cout << "    wij = " << h_weights->GetBinContent(i,j) << endl;
// 	cout << "   dwij = " << h_weights->GetBinError(i,j) << endl;
// 	cout << "    a1i = " << h1_r->GetBinContent(j) * h_weights->GetBinError(i,j) << endl;
// 	cout << "    a2i = " << h1_r->GetBinError(j) * h_weights->GetBinContent(i,j) << endl;
// 	cout << "    den = " << h1_r->GetBinContent(j) * h_weights->GetBinContent(i,j) << endl;

      }

      Double_t x =den;
      Double_t dx=0.0;
      if ( x > 0.0 ) {
	dx = sqrt(a1*a1+a2*a2)/ den;
      }
      
      
//       cout << " ---- error[" << i << "] = " << dx << endl;
      h1_p->SetBinError(i,dx * h1_p->GetBinContent(i) ); 
//       cout << "Setting bin : " << i << " = " << h1_p->GetBinContent(i) << " +- " << h1_p->GetBinError(i) << ", x = " << x << endl;

    }

    _errsSet=true;
  }
}

void PredictedDistribution::DivideObserved(TH1D *h) {
  DivideDists(h1_o, h, true);
}
void PredictedDistribution::DividePredicted(TH1D *h) {
  DivideDists(h1_p, h, false);
}
void PredictedDistribution::DivideDists(TH1D *h1d_1, TH1D *h1d_2, bool useBinomial ) {  
  if (h1d_1==0 || h1d_2==0) {
    std::cout << "PredictedDistribution::DivideDists has all empty hists!" << std::endl;
    return ;
  }
  else {
    if (h1d_1->GetSize() != h1d_2->GetSize()) {
      std::cout << "PredictedDistribution::DivideDists has hists of different sizes!" << std::endl;
      return ;
    }
  }

  Int_t nent  = h1d_1->GetEntries();
  Int_t Nbin  = h1d_1->GetSize();

  for ( Int_t i = 0; i < Nbin; i++ ){
    Float_t val1 = h1d_1->GetBinContent( i );
    Float_t val2 = h1d_2->GetBinContent( i );
    Float_t err1 = h1d_1->GetBinError( i );
    Float_t err2 = h1d_2->GetBinError( i );

    Float_t val, err;
    if ( val2 != 0 ){
      if (!useBinomial) {
	val = val1 / val2;
	//	if (i==0) std::cout << "Using homegrown errors for " << h1->GetName() << std::endl;
	err = std::sqrt( err1*err1 + err2*err2*val1*val1/(val2*val2) ) / val2;
      } else { // calculate binomial err a la the other holloway
	BinomialEff rate;
	rate.Np = val1;
	rate.Nt = val2;
	  
	if ( (rate.Np < 0) || (rate.Nt < 0) || (rate.Nt < rate.Np) ) {
	  std::cout << "\n INVALID INPUT FOR ERROR CALCULATION. I QUIT! " 
	       << rate.Np << " " << rate.Nt << std::endl;
	  std::sqrt(-1.);
	}
	  
	val = rate.eff();
	// Treatment of case with low statistics per bin
	// In the rare case efficiency is 1, take error from lower bound
	// error from program.
	if (rate.Nt == rate.Np) {
	  err = rate.elo();
	} // If number of tags is less than 10 take error from higher bound
	  // from program
	else if (rate.Np < 10) { 
	  err = rate.eup();
	}
	// Otherwise, just use equation:
	else {
	  err = std::sqrt( val*(1-val) / (rate.Nt - 1));
	}
      }
    } else { //denom was zero, just bail
      val=0;
      err=0;
    } 

    h1d_1->SetBinContent(i, val );
    h1d_1->SetBinError(i, err );

  } //loop
  
  // Since SetBinContent increments the number of entries, we want to
  // reset them to what they were before
  h1d_1->SetEntries( nent );
}
