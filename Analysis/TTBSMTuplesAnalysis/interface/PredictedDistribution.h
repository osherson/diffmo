#ifndef PredictedDistribution_h
#define PredictedDistribution_h

/*****************************************************
 * Class: PredictedDistribution
 * Manages the TH6 to get errors out conveniently.
 * Now has the option of making 1D or 2D predictograms 
 * based on which constructor you used.
 *****************************************************/

#include "BinomialEff.h"

#include <TH2D.h>
#include <TH1D.h>

#include <iostream>
#include <vector>
#include <map>


class PredictedDistribution {
 private:		    
  //forbid shallow cloning
  PredictedDistribution( const PredictedDistribution & doNotCopy);
  const PredictedDistribution& operator=( const PredictedDistribution & doNotAssign);
  
 public:      
  /*** CONSTRUCTORS                                 ******/
  //default, everything empty, must allocate new dists, not implemented yet
  PredictedDistribution() : _fNDataHisto(0),h_weights(0), h1_r(0), h1_p(0), h1_o(0), h1_tgb(0)
  { std::cout << "Do not use me." << std::endl; };
  
  //sets d with fixed bins or vary bins
  // 1D version
  PredictedDistribution(TH1D const * mtx, const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup);
  PredictedDistribution(TH1D const * mtx, const char* name, const char* title, Int_t nbinsx, const Double_t* xbins);
  
  /*** DSTRUCTOR                                    ******/
  virtual ~PredictedDistribution();
  
  /*** INLINES                                      ******/
  TH1D const * GetRateMatrix() const {return h1_r;}
  TH1D * GetPredictedHist() const {return h1_p;}
  TH1D * GetObservedHist() const {return h1_o;}
  TH1D * GetTaggableHist() const {return h1_tgb;}

   /*** ADD A JET                                    ******/
  void Accumulate(Float_t X, Float_t et, Bool_t tagged, Float_t weightFactor=1.f);

  
   /*** SET ERRORS                                   ******/
  void SetCalculatedErrors();

  // Normalize obs and pred histos to underlying distribution
  void Normalize() {SetCalculatedErrors(); DivideObserved(h1_tgb); DividePredicted(h1_tgb);}
  void DivideObserved(TH1D *h); // Uses binomial errs
  void DividePredicted(TH1D *h); // Uses propagation of homecooked errs
  static void DivideDists( TH1D * h1, TH1D * h2, bool useBinomial );
  
 protected:

  const  Int_t _fNDataHisto; 	                   //number of bins in distribution with overflo & underflo

  
  TH2D * h_weights;

  TH1D const  *h1_r;                                   //the tag rate matrix, divided, with errors inside  		
  TH1D   *h1_p, * h1_o, * h1_tgb ;                //the desired 1-d distributions: predict and observe	

  bool _errsSet;
};

#endif
