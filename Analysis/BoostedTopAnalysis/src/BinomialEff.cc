#include "Analysis/BoostedTopAnalysis/interface/BinomialEff.h"

#include <cmath>

double BinomialEff::eup() const
{
  double upper;
  int imax = (int) std::floor(Np);
  double CL = 0.1585;

  if ( Np == Nt ) {
    
    upper = 1.0;

  } else {

    double d = 1.0;
    double p = 1.0;
    double s = 0.0;
    double diff = 999.0;
    double eps = std::sqrt( (Np*(Nt-Np))/(Nt*Nt*Nt) )/10.0;

    for (int j=0; diff > eps ; ++j ) {

      double pold = p;
      ((s-CL) > 0.0) ? d = std::abs(d/2.0) : d = -1.0*std::abs(d/2.0); 

      p += d;
      double r = std::log(p/(1.0-p));
      double term = Nt*std::log(1.0-p);

      s = std::exp(term);

      for (int i=0; i!=imax ; ++i) {
        double temp = (Nt - float(i))/float(i+1);
        term = term + r + std::log(temp);
	s += std::exp(term);
      }

      if ( pold != 0.0 ) diff = std::abs(pold-p)/std::abs(pold);
    }

    upper = p;

  } // --- endif
  
  return (upper - eff());

} // --- eup()

// ===========================
// --- ELO
// ===========================

double BinomialEff::elo() const
{
  double lower;
  int imax = (int) std::floor(Np)-1;
  double CL = 0.8415;

  if ( Np == 0.0 ) {

    lower = 1.0;

  } else {

    double d = 1.0;
    double p = 1.0;
    double s = 0.0;
    double diff = 999.0;
    double eps = std::sqrt( (Np*(Nt-Np))/(Nt*Nt*Nt) )/10.0;

    for (int j=0; diff > eps ; ++j ) {

      double pold = p;
      ((s-CL) > 0.0) ? d = std::abs(d/2.0) : d = -1.0*std::abs(d/2.0); 

      p += d;
      double r = std::log(p/(1.0-p));
      double term = Nt*std::log(1.0-p);

      s = std::exp(term);

      for (int i=0; i!=imax ; ++i) {
        double temp = (Nt - float(i))/float(i+1);
        term = term + r + std::log(temp);
	s += std::exp(term);
      }

      if ( pold != 0.0 ) diff = std::abs(pold-p)/std::abs(pold);
    }

    lower = p;

  } // --- endif
  
  return (eff() - lower);

} // --- elo()

// ===========================
