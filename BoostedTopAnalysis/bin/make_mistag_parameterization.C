// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 


#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

// Root includes
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include <string> 

using namespace std;

// Make a mistag parameterization in et and eta.
// Procedure:
// 1) Loop over odd events (leave even for cross checks).
// 2) Require exactly 2 jets.
// 3) If jet1 is anti-tagged:
//    a) Fill denominator with et,eta of jet 2
//    b) If jet2 is tagged, fill numerator with et, eta of jet 2.
// 4) If jet2 is anti-tagged:
//    a) Fill denominator with et,eta of jet 1
//    b) If jet1 is tagged, fill numerator with et, eta of jet 1.
// 5) Sum up contributions.
// 6) Pass numerator and denominator to the combiner function,
//    which will combine all the separate jet bins, and divide them,
//    taking into account the statistical uncertainty. 


// make a selector for this selection
class MistagMakerSelector : public Selector<edm::EventBase> {
public:
  MistagMakerSelector( boost::shared_ptr<CATopTagFunctor> const & tagSel,
		       std::string const & jetSrc ) :
    tagSel_ (tagSel), jetSrc_(jetSrc)
  {
    push_back("Inclusive");
    push_back(">= 1-jet");
    push_back("2-jets");
    push_back("anti plus X");
    push_back("anti plus tag");
    set("Inclusive");
    set(">= 1-jet");
    set("2-jets");
    set("anti plus X");
    set("anti plus tag");
  }


  bool operator() ( edm::EventBase const & event,  pat::strbitset & ret ) {
    
    passCut(ret, "Inclusive");

    tagged0_ = tagged1_ = false;

    // get the jets
    event.getByLabel( edm::InputTag( jetSrc_) , h_jet );

    vector<pat::Jet> const & jets = *h_jet;

    if ( jets.size() >= 1 || ignoreCut(">= 1-jet") ) {
      passCut( ret, ">= 1-jet");


      // require dijet events
      if ( jets.size() >= 2 || ignoreCut("2-jets") ) {
	passCut( ret, "2-jets");

	pat::Jet const & jet0 = jets[0];
	pat::Jet const & jet1 = jets[1];

	pat::strbitset jetRet = tagSel_->getBitTemplate();
	jetRet.set(false);

	// get the tag quantities
	tagged0_ = (*tagSel_)( jet0, jetRet );
	tagged1_ = (*tagSel_)( jet1, jetRet );
	
	if ( !tagged0_ || !tagged1_ || ignoreCut("anti plus X") ) {
	  passCut( ret, "anti plus X");	  
	  if ( (!tagged0_ && tagged1_) ||
	       (tagged0_ && !tagged1_) ||
	       ignoreCut("anti plus tag") ) {
	    passCut( ret, "anti plus tag");
	  } // anti-tag plus tag

	} // anti-tag plus X
	
      } // at least 2 jets
      
    } // at least 1 jet
    
    return (bool)ret;
  }


  vector<pat::Jet> const & jets() const { return *h_jet; }

  bool tagged0() const { return tagged0_;} // is jet0 tagged?
  bool tagged1() const { return tagged1_;} // is jet1 tagged?

private:
  boost::shared_ptr<CATopTagFunctor> tagSel_;
  std::string                        jetSrc_;
  edm::Handle<vector<pat::Jet> >     h_jet;
  bool                               tagged0_;
  bool                               tagged1_;
};


int main (int argc, char* argv[]) 
{
   

  ////////////////////////////////
  // ////////////////////////// //
  // // Command Line Options // //
  // ////////////////////////// //
  ////////////////////////////////

  // Tell people what this analysis code does and setup default options.
  optutl::CommandLineParser parser ("Make mistag parameterization");

  ////////////////////////////////////////////////
  // Change any defaults or add any new command //
  //      line options you would like here.     //
  ////////////////////////////////////////////////
  parser.stringValue ("outputFile") = "mistag_param"; // .root added automatically
  parser.addOption   ("textFile", optutl::CommandLineParser::kString,
		      "Text File",
		      "mistag_param_out.txt" );
  parser.addOption   ("jetSrc", optutl::CommandLineParser::kString,
		      "Jet Source",
		      "selectedLayer1JetsTopTagPF");
  parser.addOption   ("processAll", optutl::CommandLineParser::kBool,
		      "Process all or odd only",
		      true );


  // Parse the command line arguments
  parser.parseArguments (argc, argv);

  // output text file
  ofstream textFile( parser.stringValue("textFile").c_str(), ios::app );


  // Get the name of the tag infos.
  // By default, these are PF jets
  string tagName = "CATopPFJet";

  // If we're taking Calo jets, switch the tag name
  if ( parser.stringValue("jetSrc").find("Calo") != string::npos ) {
    tagName = "CATopTagCaloJet";
  }

  //////////////////////////////////
  // //////////////////////////// //
  // // Create Event Container // //
  // //////////////////////////// //
  //////////////////////////////////

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::EventContainer ev (parser);

  ////////////////////////////////////////
  // ////////////////////////////////// //
  // //         Begin Run            // //
  // // (e.g., book histograms, etc) // //
  // ////////////////////////////////// //
  ////////////////////////////////////////

  // bins for the histograms
  Double_t PtBins[] = {
    0,
    250,
    275,
    300,
    325,
    350,
    375,
    400,
    450,
    500,
    550,
    600,
    650,
    700,
    750,
    800,
    900,
    1000,
    1200,
    1600,
    2000
  };

  // book histograms
  static const int nPtBins = sizeof( PtBins ) / sizeof( Double_t );
  // Double_t ptMax = PtBins[nPtBins - 1];

  // create the TFileService instance and a map for the histograms
  boost::shared_ptr<fwlite::TFileService> fs( new fwlite::TFileService("mistag_parameterization.root") );
  std::map<std::string, TH1*> hist;

  hist["numerator"] =     fs->make<TH1D>("numerator",     "Fake Tag Parameterization Numerator",   nPtBins-1, PtBins );
  hist["denominator"] =   fs->make<TH1D>("denominator",   "Fake Tag Parameterization Denominator", nPtBins-1, PtBins );
  hist["numerator2d"] =   fs->make<TH2D>("numerator2d",   "Fake Tag Parameterization Numerator",   nPtBins-1, PtBins, 20, -3.0, 3.0 );
  hist["denominator2d"] = fs->make<TH2D>("denominator2d", "Fake Tag Parameterization Denominator", nPtBins-1, PtBins, 20, -3.0, 3.0 );

  hist["numerator"]->Sumw2();
  hist["denominator"]->Sumw2();
  hist["numerator2d"]->Sumw2();
  hist["denominator2d"]->Sumw2();

  // make selector for selecting top tagged jets
  boost::shared_ptr<CATopTagFunctor> catopTagFunctor; 
  
  // PF by default
  if ( parser.stringValue("jetSrc").find("PF") != string::npos ) {
    catopTagFunctor =  
      boost::shared_ptr<CATopTagFunctor>(new CATopTagFunctor( CATopTagFunctor::PF,
							      CATopTagFunctor::TIGHT,
							      tagName ) );
  } else {
    catopTagFunctor =  
      boost::shared_ptr<CATopTagFunctor>(new CATopTagFunctor( CATopTagFunctor::CALO,
							      CATopTagFunctor::TIGHT,
							      tagName ) );
  }

  // make selector for mistag maker
  MistagMakerSelector mistagMakerSelector( catopTagFunctor, parser.stringValue("jetSrc") );
  // get the bit template for caching results
  pat::strbitset ret = mistagMakerSelector.getBitTemplate();					    
  

  //////////////////////
  // //////////////// //
  // // Event Loop // //
  // //////////////// //
  //////////////////////


  // Loop over events
  for( ev.toBegin();
       ! ev.atEnd();
       ++ev) {
    

    unsigned int eventNumber = ev.id().event();

    if ( eventNumber % 2 == 0 && !parser.boolValue("processAll") ) continue; // Leave even events for closure tests

    // clear the cache
    ret.set(false);
    // make the selection
    mistagMakerSelector( ev, ret );

    // make sure there are two jets
    if ( ret[string("2-jets")] ) {
      

      // If jet 0 is antitagged, fill denominator
      if ( !mistagMakerSelector.tagged0() ) {
	hist["denominator"]  ->Fill( mistagMakerSelector.jets().at(1).pt());
	hist["denominator2d"]->Fill( mistagMakerSelector.jets().at(1).pt(), 
					mistagMakerSelector.jets().at(1).rapidity());
	// if jet 1 is also tagged, fill numerator
	if ( mistagMakerSelector.tagged1() ) {
	  hist["numerator"]  ->Fill( mistagMakerSelector.jets().at(1).pt());
	  hist["numerator2d"]->Fill( mistagMakerSelector.jets().at(1).pt(), 
					mistagMakerSelector.jets().at(1).rapidity() );
	}
      } 
      // If jet 1 is antitagged, fill denominator
      if ( !mistagMakerSelector.tagged1() ) {
	hist["denominator"]  ->Fill( mistagMakerSelector.jets().at(0).pt());
	hist["denominator2d"]->Fill( mistagMakerSelector.jets().at(0).pt(), 
					mistagMakerSelector.jets().at(0).rapidity());
	// if jet 0 is also tagged, fill denominator
	if ( mistagMakerSelector.tagged0() ) {
	  hist["numerator"  ]->Fill( mistagMakerSelector.jets().at(0).pt());
	  hist["numerator2d"]->Fill( mistagMakerSelector.jets().at(0).pt(), 
					mistagMakerSelector.jets().at(0).rapidity());
	}
      } 

    }

   
  }

  textFile << parser.stringValue("outputFile") << endl;
  mistagMakerSelector.print( textFile );

  cout << parser.stringValue("outputFile") << endl;
  mistagMakerSelector.print( cout );
  
  cout << "Exiting" << endl;
}
