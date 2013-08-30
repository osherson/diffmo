#include "Analysis/BoostedTopAnalysis/interface/SemileptonicAnalysis.h"
#include "TLorentzVector.h"

using namespace std;


SemileptonicAnalysis::SemileptonicAnalysis(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  semileptonicSelection_(iConfig.getParameter<edm::ParameterSet>("semileptonicSelection")),
  boostedTopWTagFunctor_(iConfig.getParameter<edm::ParameterSet>("hadronicWParams")),
  theDir(iDir)
{


  histograms1d["nJetsSemi"] = theDir.make<TH1F>( "nJetsSemi", "Number of Semilep Jets; #Jets", 5, 0, 5 );
  histograms1d["semiLepTopMass"] = theDir.make<TH1F>("semiLepTopMass","Semileptonic top mass",100,60.0,350);
  histograms1d["zPrimeMass"] = theDir.make<TH1F>("zPrimeMass","Z' invariant mass",60,0.0,3000);
  histograms1d["hadronicTopMass"] = theDir.make<TH1F>("hadronicTopMass","Hadronic top mass",100,60.0,350);
  histograms1d["metDPhiMin"] = theDir.make<TH1F>("metDPhiMin","MET #Delta R to closest Jet;Delta R",50,0,5.0);
  histograms2d["semiMassVsGenPt"] = theDir.make<TH2F>("semiMassVsGenPt", "Semileptonic Side Mass versus GenJet p_{T}", 50, 0, 1500, 50, 0, 350); 

  histograms1d["lepJetPt_1"] = theDir.make<TH1F>( "lepJetPt_1", "Jet p_{T}, >= 1 Jet;Jet p_{T} (GeV/c)", 50, 0, 1000);
  histograms1d["lepJetPt_2"] = theDir.make<TH1F>( "lepJetPt_2", "Jet p_{T}, >= 2 Jet, p_{T}^{HAD} > 150;Jet p_{T} (GeV/c)", 50, 0, 1000);
  histograms1d["lepJetPt_3"] = theDir.make<TH1F>( "lepJetPt_3", "Jet p_{T}, 2D Cut;Jet p_{T} (GeV/c)", 50, 0, 1000);
  histograms1d["lepJetPt_4"] = theDir.make<TH1F>( "lepJetPt_4", "Jet p_{T}, H_{T}^{lep};Jet p_{T} (GeV/c)", 50, 0, 1000);

  histograms1d["lepJetMass_1"] = theDir.make<TH1F>( "lepJetMass_1", "Semileptonic Jet Mass, >= 1 Jet;Jet Mass (GeV/c^{2}", 50, 0, 250 );
  histograms1d["lepJetMass_2"] = theDir.make<TH1F>( "lepJetMass_2", "Semileptonic Jet Mass, >= 2 Jet, p_{T}^{HAD} > 150;Jet Mass (GeV/c^{2}", 50, 0, 250 );
  histograms1d["lepJetMass_3"] = theDir.make<TH1F>( "lepJetMass_3", "Semileptonic Jet Mass, 2D Cut;Jet Mass (GeV/c^{2}", 50, 0, 250 );
  histograms1d["lepJetMass_4"] = theDir.make<TH1F>( "lepJetMass_4", "Semileptonic Jet Mass, H_{T}^{lep};Jet Mass (GeV/c^{2}", 50, 0, 250 );

  histograms1d["muPt_1"] = theDir.make<TH1F>( "muPt_1", "Muon p_{T}, >= 1 Jet;Muon p_{T} (GeV/c)", 50, 0, 250 );
  histograms1d["muPt_2"] = theDir.make<TH1F>( "muPt_2", "Muon p_{T}, >= 2 Jets, p_{T}^{HAD} > 150;Muon p_{T} (GeV/c)", 50, 0, 250 );
  histograms1d["muPt_3"] = theDir.make<TH1F>( "muPt_3", "Muon p_{T}, 2D Cut;Muon p_{T} (GeV/c)", 50, 0, 250 );
  histograms1d["muPt_4"] = theDir.make<TH1F>( "muPt_4", "Muon p_{T}, H_{T}^{lep};Muon p_{T} (GeV/c)", 50, 0, 250 );

  histograms1d["muPtRel_1"] = theDir.make<TH1F>( "muPtRel_1", "Muon p_{T}^{REL}, >= 1 Jet;Muon p_{T} (GeV/c)", 50, 0, 50 );
  histograms1d["muPtRel_2"] = theDir.make<TH1F>( "muPtRel_2", "Muon p_{T}^{REL}, >= 2 Jets, p_{T}^{HAD} > 150;Muon p_{T} (GeV/c)", 50, 0, 50 );
  histograms1d["muPtRel_3"] = theDir.make<TH1F>( "muPtRel_3", "Muon p_{T}^{REL}, 2D Cut;Muon p_{T} (GeV/c)", 50, 0, 50 );
  histograms1d["muPtRel_4"] = theDir.make<TH1F>( "muPtRel_4", "Muon p_{T}^{REL}, H_{T}^{lep};Muon p_{T} (GeV/c)", 50, 0, 50 );

  histograms1d["muDRMin_1"] = theDir.make<TH1F>( "muDRMin_1", "Muon #Delta R_{min}, >= 1 Jet;Muon p_{T} (GeV/c)", 50, 0, 2 );
  histograms1d["muDRMin_2"] = theDir.make<TH1F>( "muDRMin_2", "Muon #Delta R_{min}, >= 2 Jets, p_{T}^{HAD} > 150;Muon p_{T} (GeV/c)", 50, 0, 2 );
  histograms1d["muDRMin_3"] = theDir.make<TH1F>( "muDRMin_3", "Muon #Delta R_{min}, 2D Cut;Muon p_{T} (GeV/c)", 50, 0, 2 );
  histograms1d["muDRMin_4"] = theDir.make<TH1F>( "muDRMin_4", "Muon #Delta R_{min}, H_{T}^{lep};Muon p_{T} (GeV/c)", 50, 0, 2 );

  histograms2d["mu2D_1"] = theDir.make<TH2F>( "mu2D_1", "Muon 2D Cut, >= 1 Jet;#Delta R_{min};p_{T}^{REL} (GeV/c)", 50, 0, 5.0, 40, 0, 200.0 );
  histograms2d["mu2D_2"] = theDir.make<TH2F>( "mu2D_2", "Muon 2D Cut, >= 2 Jets, p_{T}^{HAD} > 150;#Delta R_{min};p_{T}^{REL} (GeV/c)", 50, 0, 5.0, 40, 0, 200.0 );
  histograms2d["mu2D_3"] = theDir.make<TH2F>( "mu2D_3", "Muon 2D Cut, 2D Cut;#Delta R_{min};p_{T}^{REL} (GeV/c)", 50, 0, 5.0, 40, 0, 200.0 );
  histograms2d["mu2D_4"] = theDir.make<TH2F>( "mu2D_4", "Muon 2D Cut, H_{T}^{lep};#Delta R_{min};p_{T}^{REL} (GeV/c)", 50, 0, 5.0, 40, 0, 200.0 );

  histograms1d["muHt_1"] = theDir.make<TH1F>( "muHt_1", "Muon H_{T}, >= 1 Jet;#Delta R_{min};Muon H_{T} (GeV/c)", 50, 0, 500 );
  histograms1d["muHt_2"] = theDir.make<TH1F>( "muHt_2", "Muon H_{T}, >= 2 Jets, p_{T}^{HAD} > 150;Muon H_{T} (GeV/c)", 50, 0, 500 );
  histograms1d["muHt_3"] = theDir.make<TH1F>( "muHt_3", "Muon H_{T}, 2D Cut;Muon H_{T} (GeV/c)", 50, 0, 500 );
  histograms1d["muHt_4"] = theDir.make<TH1F>( "muHt_4", "Muon H_{T}, H_{T}^{lep};Muon H_{T} (GeV/c)", 50, 0, 500 );

  histograms1d["had_w_deltaPhi"] = theDir.make<TH1F>( "had_w_deltaPhi", "#Delta #phi", 50, 0, TMath::Pi());
  histograms1d["had_w_m"] = theDir.make<TH1F>( "had_w_m", "m", 50, 0, 250 );
  histograms1d["had_w_pt"] = theDir.make<TH1F>( "had_w_pt", "pt", 150, 0, 1500 );
  histograms1d["had_w_m0"] = theDir.make<TH1F>( "had_w_m0", "m0", 50, 0, 200);
  histograms1d["had_w_m1"] = theDir.make<TH1F>( "had_w_m1", "m1", 50, 0, 200);
  histograms1d["had_w_pt0"] = theDir.make<TH1F>( "had_w_pt0", "pt0", 50, 0, 200);
  histograms1d["had_w_pt1"] = theDir.make<TH1F>( "had_w_pt1", "pt1", 50, 0, 200);
  histograms1d["had_w_deltaR"] = theDir.make<TH1F>( "had_w_deltaR", "#Delta R", 50, 0, 1.0 );
  histograms1d["had_w_y"]= theDir.make<TH1F>( "had_w_y", "Subjet Asymmetry", 50, 0, 1 );
  histograms1d["had_w_mu"] = theDir.make<TH1F>( "had_w_mu", "m_{0} / m", 50, 0, 1 );
  histograms2d["hadMassVsGenPt"] = theDir.make<TH2F>("hadMassVsGenPt", "Hadronic Side Mass versus GenJet p_{T}", 50, 0, 1500, 50, 0, 350); 

  histograms1d["semilepbtag_had_w_deltaPhi"] = theDir.make<TH1F>( "semilepbtag_had_w_deltaPhi", "#Delta #phi", 50, 0, TMath::Pi());
  histograms1d["semilepbtag_had_w_m"] = theDir.make<TH1F>( "semilepbtag_had_w_m", "m", 50, 0, 250 );
  histograms1d["semilepbtag_had_w_pt"] = theDir.make<TH1F>( "semilepbtag_had_w_pt", "pt", 150, 0, 1500 );
  histograms1d["semilepbtag_had_w_m0"] = theDir.make<TH1F>( "semilepbtag_had_w_m0", "m0", 50, 0, 200);
  histograms1d["semilepbtag_had_w_m1"] = theDir.make<TH1F>( "semilepbtag_had_w_m1", "m1", 50, 0, 200);
  histograms1d["semilepbtag_had_w_pt0"] = theDir.make<TH1F>( "semilepbtag_had_w_pt0", "pt0", 50, 0, 200);
  histograms1d["semilepbtag_had_w_pt1"] = theDir.make<TH1F>( "semilepbtag_had_w_pt1", "pt1", 50, 0, 200);
  histograms1d["semilepbtag_had_w_deltaR"] = theDir.make<TH1F>( "semilepbtag_had_w_deltaR", "#Delta R", 50, 0, 1.0 );
  histograms1d["semilepbtag_had_w_y"]= theDir.make<TH1F>( "semilepbtag_had_w_y", "Subjet Asymmetry", 50, 0, 1 );
  histograms1d["semilepbtag_had_w_mu"] = theDir.make<TH1F>( "semilepbtag_had_w_mu", "m_{0} / m", 50, 0, 1 );

  histograms1d["nJetsA"] = theDir.make<TH1F>("nJetsA", "Number of jets in region A",5,0,5); 
  histograms1d["nJetsB"] = theDir.make<TH1F>("nJetsB", "Number of jets in region B",5,0,5);
  histograms1d["nJetsC"] = theDir.make<TH1F>("nJetsC", "Number of jets in region C",5,0,5);

  histograms1d["diffPtb1b2"] = theDir.make<TH1F>("diffPtb1b2", "p_{T} b1-b2", 50, -300,300);
  histograms1d["diffEb1b2"] = theDir.make<TH1F>("diffEb1b2", "E b1-b2", 50, -300,300);

  histograms2d["Mb1VsMb2"] = theDir.make<TH2F> ("Mb1VsMb2", "Mass of b1 vs mass of b2",50,0,300,50,0,300);
  histograms2d["Ptb1VsPtb2"] = theDir.make<TH2F> ("Ptb1VsPtb2", "p_{T} of b1 vs p_{T} of b2",50,0,1500,50,0,1500);


}
static double calculateMETz(int type, TLorentzVector& MET, TLorentzVector& lepton)
{
  double M_W  = 80.4;
  double M_mu =  0.10566;
  double emu = lepton.Energy();
  double pxmu = lepton.Px();
  double pymu = lepton.Py();
  double pzmu = lepton.Pz();
  double pxnu = MET.Px();
  double pynu = MET.Py();
  double pznu = 0.;

		
  double a = M_W*M_W - M_mu*M_mu + 2.0*pxmu*pxnu + 2.0*pymu*pynu;
  double A = 4.0*(emu*emu - pzmu*pzmu);
  double B = -4.0*a*pzmu;
  double C = 4.0*emu*emu*(pxnu*pxnu + pynu*pynu) - a*a;

  double tmproot = B*B - 4.0*A*C;

  if (tmproot<0) {
     pznu = - B/(2*A); // take real part of complex roots
  }
  else {
    double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
    double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
    if (type == 0 ) {
      // two real roots, pick the one closest to pz of muon
      if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2;}
      else pznu = tmpsol1;
      // if pznu is > 300 pick the most central root
      if ( pznu > 300. ) {
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) pznu = tmpsol1;
	else pznu = tmpsol2;
      }
    }
    if (type == 1 ) {
      // two real roots, pick the one closest to pz of muon
      if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2;}
      else pznu = tmpsol1;
    }
    if (type == 2 ) {
      // pick the most central root.
      if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) pznu = tmpsol1;
      else pznu = tmpsol2;
    }
    if (type == 3 ) {
      // pick the largest value of the cosine
      TVector3 p3w, p3mu;
      p3w.SetXYZ(pxmu+pxnu, pymu+pynu, pzmu+ tmpsol1);
      p3mu.SetXYZ(pxmu, pymu, pzmu );
				
      double sinthcm1 = 2.*(p3mu.Perp(p3w))/M_W;
      p3w.SetXYZ(pxmu+pxnu, pymu+pynu, pzmu+ tmpsol2);
      double sinthcm2 = 2.*(p3mu.Perp(p3w))/M_W;

      double costhcm1 = TMath::Sqrt(1. - sinthcm1*sinthcm1);
      double costhcm2 = TMath::Sqrt(1. - sinthcm2*sinthcm2);

      if ( costhcm1 > costhcm2 ) pznu = tmpsol1;
      else pznu = tmpsol2;
    }
  }
  return pznu;
}

static TLorentzVector castLorentzVector(reco::Candidate::LorentzVector vec)
{
  return TLorentzVector(vec.px(),vec.py(),vec.pz(),vec.energy());
}

void SemileptonicAnalysis::analyze(const edm::EventBase& iEvent)
{

  pat::strbitset semilepRet = semileptonicSelection_.getBitTemplate();
  semileptonicSelection_(iEvent, semilepRet);
  string bitString;
  //SemileptonicSelection::candidate_collection::const_iterator wJet = semileptonicSelection_.getWJet();
  SemileptonicSelection::candidate_collection::const_iterator closestJet = semileptonicSelection_.getClosestJet();
  SemileptonicSelection::candidate const & met = semileptonicSelection_.taggedMETs();
  SemileptonicSelection::candidate_collection const & taggedMuons = semileptonicSelection_.taggedMuons();
  SemileptonicSelection::candidate_collection const & taggedJets  = semileptonicSelection_.taggedJets();
  pat::strbitset wPlusJetsRet (semileptonicSelection_.getWPlusJetsBitSet());

  if(taggedMuons.size() > 0)
    {
      TLorentzVector muP ( taggedMuons[0].px(),
			   taggedMuons[0].py(),
			   taggedMuons[0].pz(),
			   taggedMuons[0].energy() );
      if(semilepRet[string("Lepton")])
	{

	  if(semilepRet[string("Lepton has close jet")])
	    {
	      pat::strbitset retHad = boostedTopWTagFunctor_.getBitTemplate();
	      retHad.set(false);

	      TLorentzVector MET ( met.px(), 
				   met.py(), 
				   met.pz(), //filling with zero, should have an estimate of this to get it right
				   met.energy() );
	      TLorentzVector muP ( taggedMuons[0].px(),
				   taggedMuons[0].py(),
				   taggedMuons[0].pz(),
				   taggedMuons[0].energy() );
	      TLorentzVector bjetP ( closestJet->px(),
				     closestJet->py(),
				     closestJet->pz(),
				     closestJet->energy() );
	      MET.SetPz(calculateMETz(0,MET,muP));
	      double ptRel = TMath::Abs(muP.Perp(bjetP.Vect()));
	      double dRMin = reco::deltaR<double>( muP.Eta(), muP.Phi(),
						   bjetP.Eta(), bjetP.Phi() );
	      histograms1d["nJetsSemi"]->Fill(taggedJets.size());
	      histograms1d["muHt_1"]->Fill(taggedMuons[0].pt() + met.et());
	      histograms2d["mu2D_1"]->Fill( dRMin, ptRel );
	      histograms1d["muPtRel_1"]->Fill( ptRel );
	      histograms1d["muDRMin_1"]->Fill( dRMin );
	      histograms1d["muPt_1"]->Fill( muP.Perp() );
	      histograms1d["lepJetPt_1"]->Fill( bjetP.Perp() );
	      histograms1d["lepJetMass_1"]->Fill( bjetP.M() );

	      if(semilepRet[string("Opposite leadJetPt")])
		{

		  histograms1d["muHt_2"]->Fill(taggedMuons[0].pt() + met.et());
		  histograms2d["mu2D_2"]->Fill( dRMin, ptRel );
		  histograms1d["muPtRel_2"]->Fill( ptRel );
		  histograms1d["muDRMin_2"]->Fill( dRMin );
		  histograms1d["muPt_2"]->Fill( muP.Perp() );
		  histograms1d["lepJetPt_2"]->Fill( bjetP.Perp() );
		  histograms1d["lepJetMass_2"]->Fill( bjetP.M() );


		if ( semilepRet[string("Relative Pt and Min Delta R")] ) {

		  histograms1d["muHt_3"]->Fill(taggedMuons[0].pt() + met.et());
		  histograms2d["mu2D_3"]->Fill( dRMin, ptRel );
		  histograms1d["muPtRel_3"]->Fill( ptRel );
		  histograms1d["muDRMin_3"]->Fill( dRMin );
		  histograms1d["muPt_3"]->Fill( muP.Perp() );
		  histograms1d["lepJetPt_3"]->Fill( bjetP.Perp() );
		  histograms1d["lepJetMass_3"]->Fill( bjetP.M() );


		    if( semilepRet[string("HTLep")] ) {
		      
		      TLorentzVector zPrime(0,0,0,0);
		      histograms1d["muHt_4"]->Fill(taggedMuons[0].pt() + met.et());
		      histograms2d["mu2D_4"]->Fill( dRMin, ptRel );
		      histograms1d["muPtRel_4"]->Fill( ptRel );
		      histograms1d["muDRMin_4"]->Fill( dRMin );
		      histograms1d["muPt_4"]->Fill( muP.Perp() );
		      histograms1d["lepJetPt_4"]->Fill( bjetP.Perp() );
		      histograms1d["lepJetMass_4"]->Fill( bjetP.M() );

		      /*
		      TLorentzVector MET ( met.px(), 
					   met.py(), 
					   met.pz(), //filling with zero, should have an estimate of this to get it right
					   met.energy() );
		      */
		      double metDPhiMin = reco::deltaPhi<double>( closestJet->phi(), met.phi() );
		      //semileptonic histograms
		      histograms1d["metDPhiMin"]->Fill(metDPhiMin);
		      histograms1d["semiLepTopMass"]->Fill((muP + bjetP + MET).M());

		      pat::Jet const * closestJetPat = dynamic_cast<pat::Jet const *>(closestJet->masterClonePtr().get());
		      if (closestJetPat != NULL && closestJetPat->genJet() != 0 )
			histograms2d["semiMassVsGenPt"]->Fill( closestJetPat->genJet()->pt(), closestJet->mass() );
		      // else
		      // 	edm::LogWarning("AnomalousTopology") << "No GenJet!" << std::endl;
		      //double yMax=0.0, muMax=0.0, dRMax=0.0;
		      //pat::Jet const * jet = dynamic_cast<pat::Jet const *>(wJet->masterClonePtr().get());
		      //if (jet == NULL) cout << "ERROR, crashing!\n";
		      //pat::subjetHelper(*jet, yMax, muMax, dRMax);
		      //hadronic histograms
		      SemileptonicSelection::candidate_collection::const_iterator wJet = semileptonicSelection_.getWJet();
		      if ( wJet != semileptonicSelection_.taggedJets().end() ) {
			pat::Jet const * wJetPat = dynamic_cast<pat::Jet const *>(wJet->masterClonePtr().get());
			if ( wJetPat != 0 ) {
			  zPrime = MET + bjetP + muP + castLorentzVector(wJet->p4());// + TLorentzVector(wJet->px(),wJet->py(),wJet->pz(),wJet->energy());
			  SemileptonicSelection::candidate_collection::const_iterator hadWJet = taggedJets.end();
			  double leadPt = -1.0, deltaPhi = -1.0;
			  SemileptonicSelection::candidate_collection::const_iterator bJet = taggedJets.end();
			  double leadBJetPt = -1.0;
			  for(SemileptonicSelection::candidate_collection::const_iterator ijet = taggedJets.begin(),
				jetEnd = taggedJets.end(), jetBegin = taggedJets.begin(); ijet != jetEnd; ++ijet)
			    {
			      deltaPhi = fabs(reco::deltaPhi<double>(ijet->phi(), taggedMuons[0].phi()));
			      if( deltaPhi > 2*TMath::Pi()/3 )
				{
				  if(ijet != wJet )
				    {
				      pat::Jet const * bJetPat = dynamic_cast<pat::Jet const *>(ijet->masterClonePtr().get());
				      if(bJetPat == NULL) continue;
				      if(bJetPat->bDiscriminator("trackCountingHighEffBJetTags") > 3.3)
					{
					  if(ijet->pt() > leadBJetPt)
					    {
					      leadBJetPt = ijet->pt();
					      bJet = ijet;
					    }
					}
				    }
				}
			    }
			  if(bJet != taggedJets.end() )
			    {
			      zPrime += castLorentzVector(bJet->p4());
			      cout <<" Z' Hypothesis mass: "<<zPrime.M()<<endl;
			      histograms1d["zPrimeMass"]->Fill(zPrime.M());

			      boostedTopWTagFunctor_( *wJetPat, retHad);
			      double y = 0.0, mu = 0.0, dR = 0.0;
			      pat::subjetHelper( *wJetPat, y, mu, dR );
			      histograms1d["diffPtb1b2"]->Fill(closestJet->pt() - wJet->pt() );
			      histograms1d["diffEb1b2"]->Fill(closestJet->energy() - wJet->energy() );
			      histograms2d["Mb1VsMb2"]->Fill(closestJet->mass(), wJet->mass() );
			      histograms2d["Ptb1VsPtb2"]->Fill(closestJet->pt(), wJet->pt() );
			      histograms1d["had_w_m"]->Fill( wJet->mass() );
			      histograms1d["had_w_pt"]->Fill( wJet->pt() );
			      if ( wJet->daughter(0) != 0 ) {
				histograms1d["had_w_m0"]->Fill( wJet->daughter(0)->mass() );
				histograms1d["had_w_pt0"]->Fill( wJet->daughter(0)->pt() );
			      } 
			      else {
				// edm::LogWarning("AnomalousTopology") << "W0 has no daughters" << std::endl;
				histograms1d["had_w_m0"]->Fill( -1.0 );
				histograms1d["had_w_pt0"]->Fill( -1.0 );
			      }

			      if ( wJet->daughter(1) != 0 ) {
				histograms1d["had_w_m1"]->Fill( wJet->daughter(1)->mass() );
				histograms1d["had_w_pt1"]->Fill( wJet->daughter(1)->pt() );
			      } else {
				// edm::LogWarning("AnomalousTopology") << "W1 has no daughters" << std::endl;
				histograms1d["had_w_m1"]->Fill( -1.0 );
				histograms1d["had_w_pt1"]->Fill( -1.0 );
			      }

			      histograms1d["had_w_mu"]->Fill( mu );
			      histograms1d["had_w_y"]->Fill( y );
			      histograms1d["had_w_deltaR"]->Fill( dR );	

			      if ( wJetPat->genJet() != 0 ) 
				histograms2d["hadMassVsGenPt"]->Fill(  wJetPat->genJet()->pt(), wJet->mass() );
			      // else
			      //   edm::LogWarning("AnomalousTopology") << "GenJet is zero for W candidates!" << std::endl;

			      if( semilepRet[string("SemilepBtag")] ) {

				histograms1d["semilepbtag_had_w_m"]->Fill( wJet->mass() );
				histograms1d["semilepbtag_had_w_pt"]->Fill( wJet->pt() );
				if ( wJet->daughter(0) != 0 ) {
				  histograms1d["semilepbtag_had_w_m0"]->Fill( wJet->daughter(0)->mass() );
				  histograms1d["semilepbtag_had_w_pt0"]->Fill( wJet->daughter(0)->pt() );
				} else {
				  // edm::LogWarning("AnomalousTopology") << "W0 has no daughters" << std::endl;
				  histograms1d["semilepbtag_had_w_m0"]->Fill( -1.0 );
				  histograms1d["semilepbtag_had_w_pt0"]->Fill( -1.0 );
				}
			    
				if ( wJet->daughter(1) != 0 ) {
				  histograms1d["semilepbtag_had_w_m1"]->Fill( wJet->daughter(1)->mass() );
				  histograms1d["semilepbtag_had_w_pt1"]->Fill( wJet->daughter(1)->pt() );
				} else {
				  // edm::LogWarning("AnomalousTopology") << "W1 has no daughters" << std::endl;
				  histograms1d["semilepbtag_had_w_m1"]->Fill( -1.0 );
				  histograms1d["semilepbtag_had_w_pt1"]->Fill( -1.0 );
				}
			    
				histograms1d["semilepbtag_had_w_mu"]->Fill( mu );
				histograms1d["semilepbtag_had_w_y"]->Fill( y );
				histograms1d["semilepbtag_had_w_deltaR"]->Fill( dR );	
			    
			      } // end if semileptonic btag  
			    }// found hadronic bjet
			
			} else {
			  edm::LogWarning("BadCast") << "This isn't a pat jet!" << std::endl;
			} // end if pat jet
		      }// end if valid wjet
		    }// end if hemispheric event
		}// end if opposite leading jet pt
		}// end if 2d cut
	    }// end if lepton has close jet
	}// end if lepton passes
    }// end if there are any muons
}// end of function

