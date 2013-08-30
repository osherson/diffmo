#ifndef Binomial_Eff_h
#define Binomial_Eff_h


// =========================================================================
// --- converted from .f to .cc file Nov-01 (DAG)
// --- originally from Lee Holloway
// --- iteration modified to improve robustness regardless
// ---   of input numbers (DAG)
// =========================================================================
// ---	WHAT IS THE CORRECT ERROR IN AN EFFICIENCY CALCULATION?
// =========================================================================
// 
//			Lee Holloway		Pisa,  June 15, 1989    
//
//	When calculating the efficiency, p, of a trigger one
//	takes    p = Ngood/Ntot   where Ntot is the total number of
//	events, and Ngood is the number of successful events. The usual
//	formula for the error in the efficiency is:
//	      delta p = Sqrt[ p(1-p)/Ntot ]
//	This formula is incorrect, as can be easily seen by considering
//	the case of, say, two successfull hits in two attempts.  The
//	formula gives zero for the error which is clearly wrong.
//	
//	The correct way to do it is rather complicated and involves
//	solving a problem in summed binomial distributions.  Interested
//	parties can read an article by 
//	F. James and M. Roos, Nucl. Phys. B172, 475 (1980)
//	who discuss a similar problem, i.e., the error on the ratio of
//	two small numbers. They acknowledge an article by the famous
//	statistician Pearson: 
//	C.J. Clopper and E.S. Pearson, Biometrika 26, 404 (1934)
//
//	I have written a routine which will give you the upper and
//	and lower limits corresponding to a 68.3% confidence interval.
//	If you want another condidence level, change the values of b(1)
//	and b(2) in subroutine CONFIDENCE.  For example for a 95% confidence
//	interval, use b(1)=0.975 and b(2)=0.025 .
//
// =========================================================================
class BinomialEff {
 public: 
  double Np; //! Number of obs quantity
  double Nt; //! Total number
  double eff() const {return (Np/Nt);}
  double eup() const;
  double elo() const;
};

#endif
