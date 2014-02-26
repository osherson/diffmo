#include <cstdlib>
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include "TChain.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveLabel.h"
#include <vector>	
#include <algorithm>

#include "names.C"

using namespace std;
using namespace names;

float ptMap(float pt);
float bMap(float b);
float tauMap(float t);


int makeTemplates(){



	TString labels[names::NUM_PROCS];
	labels[names::DATA] = "data";
	labels[names::QCD]  = "qcd";
	labels[names::TT7] = "ttbar7";
	labels[names::TT7_MIS] = "ttbar7_mis";
	labels[names::TT10] = "ttbar10";
	labels[names::TT10_MIS] = "ttbar10_mis";
	labels[names::ZPN10] = "ZpN10";
	labels[names::ZPN15] = "ZpN15";
	labels[names::ZPN20] = "ZpN20";
	labels[names::ZPN30] = "ZpN30";
	labels[names::ZPN10_SCALEUP] = "ZpN10_jes_up";
	labels[names::ZPN15_SCALEUP] = "ZpN15_jes_up";
	labels[names::ZPN20_SCALEUP] = "ZpN20_jes_up";
	labels[names::ZPN30_SCALEUP] = "ZpN30_jes_up";
	labels[names::ZPN10_SCALEDN] = "ZpN10_jes_dn";
	labels[names::ZPN15_SCALEDN] = "ZpN15_jes_dn";
	labels[names::ZPN20_SCALEDN] = "ZpN20_jes_dn";
	labels[names::ZPN30_SCALEDN] = "ZpN30_jes_dn";
	labels[names::ZPN10_JERUP] = "ZpN10_jer_up";
	labels[names::ZPN15_JERUP] = "ZpN15_jer_up";
	labels[names::ZPN20_JERUP] = "ZpN20_jer_up";
	labels[names::ZPN30_JERUP] = "ZpN30_jer_up";
	labels[names::ZPN10_JERDN] = "ZpN10_jer_dn";
	labels[names::ZPN15_JERDN] = "ZpN15_jer_dn";
	labels[names::ZPN20_JERDN] = "ZpN20_jer_dn";
	labels[names::ZPN30_JERDN] = "ZpN30_jer_dn";

	ofstream txtfile("bins.txt");	
	TFile *outFile = new TFile("bins.root", "RECREATE");


	int nBins = 50;
	int min = 50;
	int max = 5000;
	int nTagCats = 4;

	TString tagLabels[nTagCats];
	tagLabels[0] = "btag0";
	tagLabels[1] = "btag1";
	tagLabels[2] = "btag2";
	tagLabels[3] = "all";	


	TH1F *histos[names::NUM_PROCS][nTagCats];
	for (int h = 0; h < names::NUM_PROCS; h++){
		histos[h][0] = new TH1F(labels[h]+tagLabels[0], labels[h]+tagLabels[0], nBins, min, max);
		histos[h][0]->Sumw2();
		histos[h][1] = new TH1F(labels[h]+tagLabels[1], labels[h]+tagLabels[1], nBins, min, max);
		histos[h][1]->Sumw2();
		histos[h][2] = new TH1F(labels[h]+tagLabels[2], labels[h]+tagLabels[2], nBins, min, max);
		histos[h][2]->Sumw2();
		histos[h][3] = new TH1F(labels[h]+tagLabels[3], labels[h]+tagLabels[3], nBins, min, max);
		histos[h][3]->Sumw2();
	}

	
	
	

	TFile *mistagFile = new TFile("Feb18_mistag.root", "READ");
	TH3D *mistagH = (TH3D *) mistagFile->Get("MISTAG_RATE_SUB_TTBAR_Inclusive");

	TH2F *binFillHist = new TH2F("binFill", "binFill", 50, 0, 50, 2*29*4*9, 0, 2*29*4*9);

	int mistagBins[3];
	mistagBins[0] = mistagH->GetNbinsX();
	mistagBins[1] = mistagH->GetNbinsY();
	mistagBins[2] = mistagH->GetNbinsZ();

	cout << nBins << " " << mistagBins[0] << " " << mistagBins[1] << " " << mistagBins[2] << endl;
	int arrayForErrors[nTagCats][nBins][mistagBins[0]][mistagBins[1]][mistagBins[2]];


	for (int x = 0; x < nBins; x++){
		for (int y = 0; y < mistagBins[0]; y++){
			for (int z = 0; z < mistagBins[1]; z++){
				for (int t = 0; t < mistagBins[2]; t++){
					arrayForErrors[0][x][y][z][t] = 0;
					arrayForErrors[1][x][y][z][t] = 0;
					arrayForErrors[2][x][y][z][t] = 0;
					arrayForErrors[3][x][y][z][t] = 0;
				}
			}
		}
	}


	TString files[names::NUM_PROCS];
	files[names::DATA] = "Feb12_all.root";
	files[names::QCD] = "Feb12_all.root";
	files[names::TT7] = "Jan13_ttjets7_ptw.root";
	files[names::TT10] = "Jan13_ttjets10_ptw.root";
	files[names::TT7_MIS] = files[names::TT7];
	files[names::TT10_MIS] = files[names::TT10];
	files[names::ZPN20] = "Feb12_Zp20_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPN10] = "Feb12_Zp10_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPN15] = "Feb12_Zp15_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPN30] = "Feb12_Zp30_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPN20_SCALEUP] = "Feb12_Zp20_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPN10_SCALEUP] = "Feb12_Zp10_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPN15_SCALEUP] = "Feb12_Zp15_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPN30_SCALEUP] = "Feb12_Zp30_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPN20_SCALEDN] = "Feb12_Zp20_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPN10_SCALEDN] = "Feb12_Zp10_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPN15_SCALEDN] = "Feb12_Zp15_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPN30_SCALEDN] = "Feb12_Zp30_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPN20_JERUP] = "Feb12_Zp20_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPN10_JERUP] = "Feb12_Zp10_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPN15_JERUP] = "Feb12_Zp15_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPN30_JERUP] = "Feb12_Zp30_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPN20_JERDN] = "Feb12_Zp20_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPN10_JERDN] = "Feb12_Zp10_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPN15_JERDN] = "Feb12_Zp15_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPN30_JERDN] = "Feb12_Zp30_sec0_ttpair_PtSmearDown_TriggernoWeight.root";

	for (int proc = 0; proc < names::NUM_PROCS; proc++){

	TChain *currentChain = new TChain("treeVars");
	currentChain->Add(files[proc]); 
	int nEvents = currentChain->GetEntries();


	float mttMass, mttMassPred, mistagWt, jet1Pt, jet2Pt, jet1tau32, jet2tau32, jet1SubjetMaxBDisc, jet2SubjetMaxBDisc, jetPtForMistag;
	float cutflow, index, ptReweight;
	













	currentChain->SetBranchAddress("mttMass", &mttMass);
	currentChain->SetBranchAddress("mttMassPred", &mttMassPred);
	currentChain->SetBranchAddress("cutflow", &cutflow);
	currentChain->SetBranchAddress("index", &index);
	currentChain->SetBranchAddress("mistagWt", &mistagWt);
	currentChain->SetBranchAddress("jet1Pt", &jet1Pt);
	currentChain->SetBranchAddress("jet2Pt", &jet2Pt);
	currentChain->SetBranchAddress("jet1tau32", &jet1tau32);
	currentChain->SetBranchAddress("jet2tau32", &jet2tau32);
	currentChain->SetBranchAddress("jet1SubjetMaxBDisc", &jet1SubjetMaxBDisc);
	currentChain->SetBranchAddress("jet2SubjetMaxBDisc", &jet2SubjetMaxBDisc);
	currentChain->SetBranchAddress("jetPtForMistag", &jetPtForMistag);
	currentChain->SetBranchAddress("ptReweight", &ptReweight);	

	cout << "Processing sample " << labels[proc] << " with   " << nEvents << " events" << endl;


	int QCDcount = 0;
	for (int i = 0; i < nEvents; i++){

		currentChain->GetEntry(i);

		bool btag0 = jet1SubjetMaxBDisc < 0.679 && jet2SubjetMaxBDisc < 0.679 && jet1tau32 < 0.7 && jet2tau32 < 0.7;
		bool btag1 = (jet1SubjetMaxBDisc > 0.679 || jet2SubjetMaxBDisc > 0.679) && (jet1SubjetMaxBDisc < 0.679 || jet2SubjetMaxBDisc <  0.679) && jet1tau32 < 0.7 && jet2tau32 < 0.7;
		bool btag2 = jet1SubjetMaxBDisc > 0.679 && jet2SubjetMaxBDisc > 0.679;


		
		float bScore, tauScore = -999.9;
		if (proc <= names::TT10 && cutflow == 4 && index == 0) {

			if (proc == names::TT10 || proc == names::TT7) {

				histos[proc][3]->Fill(mttMass, ptReweight);
				if (btag0) histos[proc][0]->Fill(mttMass, ptReweight);
				if (btag1) histos[proc][1]->Fill(mttMass, ptReweight);
				if (btag2) histos[proc][2]->Fill(mttMass, ptReweight);
			}
			else {
				histos[proc][3]->Fill(mttMass);
				if (btag0) histos[proc][0]->Fill(mttMass);
				if (btag1) histos[proc][1]->Fill(mttMass);
				if (btag2) histos[proc][2]->Fill(mttMass);
			}
		}
		else if (proc > names::TT10 && index == 1) {

			histos[proc][3]->Fill(mttMassPred, mistagWt);
			if (btag0) histos[proc][0]->Fill(mttMassPred, mistagWt);
			if (btag1) histos[proc][1]->Fill(mttMassPred, mistagWt);
			if (btag2) histos[proc][2]->Fill(mttMassPred, mistagWt);
			if (jet1Pt == jetPtForMistag) {
				bScore = jet1SubjetMaxBDisc;
				tauScore = jet1tau32;
			}
			if (jet2Pt == jetPtForMistag) {
				bScore = jet2SubjetMaxBDisc;
				tauScore = jet2tau32;
			}
			if (proc == names::QCD) {
				int mtt_bin = histos[names::QCD][3]->FindBin(mttMassPred) - 1;
				if (mtt_bin == 50) mtt_bin = 49;
				binFillHist->Fill(mtt_bin, mistagH->FindBin(ptMap(jetPtForMistag), bMap(bScore), tauMap(tauScore)));	
				arrayForErrors[3][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag0) arrayForErrors[0][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag1) arrayForErrors[1][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag2) arrayForErrors[2][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
		
			}
		}

	
	}	

	delete currentChain;



	}


	cout << "Done with Filling Histograms" << endl;







	//Computing QCD errors

	for (int tag = 0; tag < nTagCats; tag++){

	txtfile << "=====================================" << tag << "============================" << endl;



	for (int bin0 = 0; bin0 < nBins; bin0++){


		float a1 = 0.0;
		float a2 = 0.0;
		float den = 0.0;



		for (int bin1 = 0; bin1 < mistagBins[0]; bin1++){

			for (int bin2 = 0; bin2 < mistagBins[1]; bin2++){ 

				for (int bin3 = 0; bin3 < mistagBins[2]; bin3++){

					a1 += mistagH->GetBinContent(bin1, bin2, bin3) * sqrt(arrayForErrors[tag][bin0][bin1][bin2][bin3]);
					a2 += mistagH->GetBinError(bin1, bin2, bin3) * arrayForErrors[tag][bin0][bin1][bin2][bin3];
					den += mistagH->GetBinContent(bin1, bin2, bin3) * arrayForErrors[tag][bin0][bin1][bin2][bin3]; 
					
					 txtfile << bin0 << " " << bin1 << " " << bin2 << " " << bin3 << " " << arrayForErrors[tag][bin0][bin1][bin2][bin3] << endl;

				}
			}
		}





		//Do calcluation
		double x = den;
      		double dx=0.0;
      		if ( x > 0.0 ) dx = sqrt(a1*a1+a2*a2)/ den;

		histos[names::QCD][tag]->SetBinError( bin0, dx * histos[names::QCD][tag]->GetBinContent(bin0));
		cout << bin0 << "  " << dx << endl;
	}

	}


	//make plot


	float lumi = 19700.;
	float subSF = 0.95*0.95;

	for (int tag = 0; tag < nTagCats; tag++){

	TCanvas *c1 = new TCanvas();

	histos[names::QCD][tag]->SetMarkerSize(0);

	histos[names::TT7][tag]->Scale( 234. * lumi * subSF * 0.074 / 3082812. );
	histos[names::TT10][tag]->Scale( 234. * lumi * subSF * 0.014 / 1249111. );
	histos[names::TT7_MIS][tag]->Scale( 234. * lumi * subSF * 0.074 / 3082812. );
	histos[names::TT10_MIS][tag]->Scale( 234. * lumi * subSF * 0.014 / 1249111. );


	histos[names::ZPN10][tag]->Scale( 1. * lumi * subSF  / 101697. ); 
	histos[names::ZPN15][tag]->Scale( 1. * lumi * subSF  /  94371. ); 
	histos[names::ZPN20][tag]->Scale( 1. * lumi * subSF  /  90778. ); 
	histos[names::ZPN30][tag]->Scale( 1. * lumi * subSF  /  91209. ); 
	histos[names::ZPN10_SCALEUP][tag]->Scale( 1. * lumi * subSF  / 101697. ); 
	histos[names::ZPN15_SCALEUP][tag]->Scale( 1. * lumi * subSF  /  94371. ); 
	histos[names::ZPN20_SCALEUP][tag]->Scale( 1. * lumi * subSF  /  90778. ); 
	histos[names::ZPN30_SCALEUP][tag]->Scale( 1. * lumi * subSF  /  91209. ); 
	histos[names::ZPN10_SCALEDN][tag]->Scale( 1. * lumi * subSF  / 101697. ); 
	histos[names::ZPN15_SCALEDN][tag]->Scale( 1. * lumi * subSF  /  94371. ); 
	histos[names::ZPN20_SCALEDN][tag]->Scale( 1. * lumi * subSF  /  90778. ); 
	histos[names::ZPN30_SCALEDN][tag]->Scale( 1. * lumi * subSF  /  91209. ); 
	histos[names::ZPN10_JERUP][tag]->Scale( 1. * lumi * subSF  / 101697. ); 
	histos[names::ZPN15_JERUP][tag]->Scale( 1. * lumi * subSF  /  94371. ); 
	histos[names::ZPN20_JERUP][tag]->Scale( 1. * lumi * subSF  /  90778. ); 
	histos[names::ZPN30_JERUP][tag]->Scale( 1. * lumi * subSF  /  91209. ); 
	histos[names::ZPN10_JERDN][tag]->Scale( 1. * lumi * subSF  / 101697. ); 
	histos[names::ZPN15_JERDN][tag]->Scale( 1. * lumi * subSF  /  94371. ); 
	histos[names::ZPN20_JERDN][tag]->Scale( 1. * lumi * subSF  /  90778. ); 
	histos[names::ZPN30_JERDN][tag]->Scale( 1. * lumi * subSF  /  91209. ); 



	histos[names::TT7][tag]->Add(histos[names::TT10][tag]);

	histos[names::TT7][tag]->SetFillColor(kRed);
	histos[names::QCD][tag]->SetFillColor(kYellow);
	histos[names::QCD][tag]->Add(histos[names::TT7_MIS][tag], -1);
	histos[names::QCD][tag]->Add(histos[names::TT10_MIS][tag], -1);
	
	histos[names::DATA][tag]->SetMaximum(1.4 * histos[names::DATA][tag]->GetMaximum() );
	histos[names::DATA][tag]->Draw("E");	
 
	THStack *stack = new THStack();
	stack->Add(histos[names::TT7][tag]);
	stack->Add(histos[names::QCD][tag]);
	stack->Draw("hist same");
	histos[names::DATA][tag]->SetMarkerStyle(21);
	histos[names::DATA][tag]->Draw("E same");


	TH1F *totalH = new TH1F("totalH", "totalH", nBins, min, max);	
    	totalH = (TH1F *) histos[names::QCD][tag]->Clone("totalH");
	totalH->Add(histos[names::TT7][tag]);
	totalH->SetFillStyle(3004);
    	totalH->SetFillColor(kBlack);
    	totalH->Draw("same E2");

	double error = -999.9;
	histos[names::QCD][tag]->IntegralAndError(0, nBins+1, error);


	cout << tag << " B-TAG CATEGORY" << endl;
	cout << "==================================" << endl;
	cout << "QCD       :" << histos[names::QCD][tag]->Integral() << "  +/-  " << error << endl;
	cout << "ttbar     :" << histos[names::TT7][tag]->Integral() << endl;
	cout << "Total BG  :" << histos[names::QCD][tag]->Integral() + histos[names::TT7][tag]->Integral() << endl;
	cout << "Data      :" << histos[names::DATA][tag]->Integral() << endl;


	c1->SaveAs("errors"+tagLabels[tag]+".pdf");
	c1->SetLogy(1);
	c1->SaveAs("errors"+tagLabels[tag]+"_log.pdf");
	}


	outFile->cd();
	//binFillHist->Write();
	for (int tag = 0; tag < 3; tag++){



		TH1F *upQCDH = (TH1F *) histos[names::QCD][tag]->Clone("upQCDH");
		TH1F *dnQCDH = (TH1F *) histos[names::QCD][tag]->Clone("dnQCDH");

		for (int Nbin = 0; Nbin < histos[names::QCD][tag]->GetNbinsX(); Nbin++){

			upQCDH->SetBinContent( Nbin, histos[names::QCD][tag]->GetBinContent(Nbin) + histos[names::QCD][tag]->GetBinError(Nbin) ); 
			dnQCDH->SetBinContent( Nbin, histos[names::QCD][tag]->GetBinContent(Nbin) - histos[names::QCD][tag]->GetBinError(Nbin) ); 
		}


		upQCDH->Write( Form("%dbtag__qcd__misErr__plus", tag) );
		dnQCDH->Write( Form("%dbtag__qcd__misErr__minus", tag) );

		histos[names::QCD][tag]->Write( Form("%dbtag__qcd", tag) );
		histos[names::TT7][tag]->Write( Form("%dbtag__ttbar", tag) );
		histos[names::DATA][tag]->Write( Form("%dbtag__DATA", tag) );
		histos[names::ZPN10][tag]->Write( Form("%dbtag__Zprime1000", tag));
		histos[names::ZPN15][tag]->Write( Form("%dbtag__Zprime1500", tag) );
		histos[names::ZPN20][tag]->Write( Form("%dbtag__Zprime2000", tag) );
		histos[names::ZPN30][tag]->Write( Form("%dbtag__Zprime3000", tag) );
		histos[names::ZPN10_SCALEUP][tag]->Write( Form("%dbtag__Zprime1000__jes__plus", tag));
		histos[names::ZPN15_SCALEUP][tag]->Write( Form("%dbtag__Zprime1500__jes__plus", tag) );
		histos[names::ZPN20_SCALEUP][tag]->Write( Form("%dbtag__Zprime2000__jes__plus", tag) );
		histos[names::ZPN30_SCALEUP][tag]->Write( Form("%dbtag__Zprime3000__jes__plus", tag) );
		histos[names::ZPN10_SCALEDN][tag]->Write( Form("%dbtag__Zprime1000__jes__minus", tag));
		histos[names::ZPN15_SCALEDN][tag]->Write( Form("%dbtag__Zprime1500__jes__minus", tag) );
		histos[names::ZPN20_SCALEDN][tag]->Write( Form("%dbtag__Zprime2000__jes__minus", tag) );
		histos[names::ZPN30_SCALEDN][tag]->Write( Form("%dbtag__Zprime3000__jes__minus", tag) );
		histos[names::ZPN10_JERUP][tag]->Write( Form("%dbtag__Zprime1000__jer__plus", tag));
		histos[names::ZPN15_JERUP][tag]->Write( Form("%dbtag__Zprime1500__jer__plus", tag) );
		histos[names::ZPN20_JERUP][tag]->Write( Form("%dbtag__Zprime2000__jer__plus", tag) );
		histos[names::ZPN30_JERUP][tag]->Write( Form("%dbtag__Zprime3000__jer__plus", tag) );
		histos[names::ZPN10_JERDN][tag]->Write( Form("%dbtag__Zprime1000__jer__minus", tag));
		histos[names::ZPN15_JERDN][tag]->Write( Form("%dbtag__Zprime1500__jer__minus", tag) );
		histos[names::ZPN20_JERDN][tag]->Write( Form("%dbtag__Zprime2000__jer__minus", tag) );
		histos[names::ZPN30_JERDN][tag]->Write( Form("%dbtag__Zprime3000__jer__minus", tag) );
	}	

	outFile->Close();

	return 0;


}


	
float ptMap(float pt){


	float out = -999.;

	if (pt < 405) out = 0.1;
	else if (pt < 410) out = 1.1;
	else if (pt < 415) out = 2.1;
	else if (pt < 420) out = 3.1;
	else if (pt < 425) out = 4.1;
	else if (pt < 430) out = 5.1;
	else if (pt < 435) out = 6.1;
	else if (pt < 440) out = 7.1;
	else if (pt < 445) out = 8.1;
	else if (pt < 450) out = 9.1;
	else if (pt < 460) out = 10.1;
	else if (pt < 470) out = 11.1;
	else if (pt < 480) out = 12.1;
	else if (pt < 490) out = 13.1;
	else if (pt < 500) out = 14.1;
	else if (pt < 525) out = 15.1;
	else if (pt < 550) out = 16.1;
	else if (pt < 575) out = 17.1;
	else if (pt < 600) out = 18.1;
	else if (pt < 650) out = 19.1;
	else if (pt < 700) out = 20.1;
	else if (pt < 750) out = 21.1;
	else if (pt < 800) out = 22.1;
	else if (pt < 900) out = 23.1;
	else if (pt < 1000) out = 24.1;
	else if (pt < 1100) out = 25.1;
	else if (pt < 1250) out = 26.1;
	else if (pt < 1500) out = 27.1;
	else out = 28.1;

	return out;

}

float bMap(float b){

	float out = -999.;

	if (b < 0.0) out = 0.1;
	else if (b < 0.244) out = 1.1;
	else if (b < 0.679) out = 2.1;
	else if (b < 10.00) out = 3.1;

	return out;

}


float tauMap(float t){

	float out = -999.;

	if (t < 0.4) out = 0.1;
	else if (t < 0.5) out = 1.1;
	else if (t < 0.6) out = 2.1;
	else if (t < 0.7) out = 3.1;
	else if (t < 0.8) out = 4.1;
	else if (t < 0.9) out = 5.1;
	else if (t < 1.0) out = 6.1;
	else if (t < 1.2) out = 7.1;
	else out = 8.1;

	return out;



} 



	


	

