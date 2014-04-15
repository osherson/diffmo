#include <cstdlib>
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TF1.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include "TChain.h"
#include "TLatex.h"
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



int makeTemplates(bool ZPNflag, bool ZPWflag, bool RSGflag){



	TString labels[names::NUM_PROCS];
	labels[names::DATA] = "data";
	labels[names::QCD]  = "qcd";
	labels[names::TT7] = "ttbar7";
	labels[names::TTLOW] = "ttbarLOW";
	labels[names::TT7_SCALEUP] = "ttbar7_jes_up";
	labels[names::TT7_SCALEDN] = "ttbar7_jes_dn";
	labels[names::TT7_Q2UP] = "ttbar7_q2_up";
	labels[names::TT7_Q2DN] = "ttbar7_q2_dn";
	labels[names::TT7_MIS] = "ttbar7_mis";
	labels[names::TT10] = "ttbar10";
	labels[names::TT10_SCALEUP] = "ttbar10_jes_up";
	labels[names::TT10_SCALEDN] = "ttbar10_jes_dn";
	labels[names::TT10_Q2UP] = "ttbar10_q2_up";
	labels[names::TT10_Q2DN] = "ttbar10_q2_dn";
	labels[names::TT10_MIS] = "ttbar10_mis";
	labels[names::ZPN10_MIS] = "ZpN10_mis";
	labels[names::ZPN15_MIS] = "ZpN15_mis";
	labels[names::ZPN20_MIS] = "ZpN20_mis";
	labels[names::ZPN30_MIS] = "ZpN30_mis";
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
	labels[names::ZPW10] = "ZpW10";
	labels[names::ZPW15] = "ZpW15";
	labels[names::ZPW20] = "ZpW20";
	labels[names::ZPW30] = "ZpW30";
	labels[names::ZPW10_SCALEUP] = "ZpW10_jes_up";
	labels[names::ZPW15_SCALEUP] = "ZpW15_jes_up";
	labels[names::ZPW20_SCALEUP] = "ZpW20_jes_up";
	labels[names::ZPW30_SCALEUP] = "ZpW30_jes_up";
	labels[names::ZPW10_SCALEDN] = "ZpW10_jes_dn";
	labels[names::ZPW15_SCALEDN] = "ZpW15_jes_dn";
	labels[names::ZPW20_SCALEDN] = "ZpW20_jes_dn";
	labels[names::ZPW30_SCALEDN] = "ZpW30_jes_dn";
	labels[names::ZPW10_JERUP] = "ZpW10_jer_up";
	labels[names::ZPW15_JERUP] = "ZpW15_jer_up";
	labels[names::ZPW20_JERUP] = "ZpW20_jer_up";
	labels[names::ZPW30_JERUP] = "ZpW30_jer_up";
	labels[names::ZPW10_JERDN] = "ZpW10_jer_dn";
	labels[names::ZPW15_JERDN] = "ZpW15_jer_dn";
	labels[names::ZPW20_JERDN] = "ZpW20_jer_dn";
	labels[names::ZPW30_JERDN] = "ZpW30_jer_dn";
	labels[names::RSG10] = "RSG10";
	labels[names::RSG15] = "RSG15";
	labels[names::RSG20] = "RSG20";
	labels[names::RSG30] = "RSG30";
	labels[names::RSG10_SCALEUP] = "RSG10_jes_up";
	labels[names::RSG15_SCALEUP] = "RSG15_jes_up";
	labels[names::RSG20_SCALEUP] = "RSG20_jes_up";
	labels[names::RSG30_SCALEUP] = "RSG30_jes_up";
	labels[names::RSG10_SCALEDN] = "RSG10_jes_dn";
	labels[names::RSG15_SCALEDN] = "RSG15_jes_dn";
	labels[names::RSG20_SCALEDN] = "RSG20_jes_dn";
	labels[names::RSG30_SCALEDN] = "RSG30_jes_dn";
	labels[names::RSG10_JERUP] = "RSG10_jer_up";
	labels[names::RSG15_JERUP] = "RSG15_jer_up";
	labels[names::RSG20_JERUP] = "RSG20_jer_up";
	labels[names::RSG30_JERUP] = "RSG30_jer_up";
	labels[names::RSG10_JERDN] = "RSG10_jer_dn";
	labels[names::RSG15_JERDN] = "RSG15_jer_dn";
	labels[names::RSG20_JERDN] = "RSG20_jer_dn";
	labels[names::RSG30_JERDN] = "RSG30_jer_dn";

	ofstream txtfile("bins.txt");	
	TFile *outFile = new TFile("bins.root", "RECREATE");


	int nBins = 50;
	int min = 50;
	int max = 5000;
	int nTagCats = 7;

	TString tagLabels[nTagCats];
	tagLabels[0] = "btag0";
	tagLabels[1] = "btag1";
	tagLabels[2] = "btag2";
	tagLabels[3] = "btag0_hiDY";	
	tagLabels[4] = "btag1_hiDY";	
	tagLabels[5] = "btag2_hiDY";	
	tagLabels[6] = "all";	


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
		histos[h][4] = new TH1F(labels[h]+tagLabels[4], labels[h]+tagLabels[4], nBins, min, max);
		histos[h][4]->Sumw2();
		histos[h][5] = new TH1F(labels[h]+tagLabels[5], labels[h]+tagLabels[5], nBins, min, max);
		histos[h][5]->Sumw2();
		histos[h][6] = new TH1F(labels[h]+tagLabels[6], labels[h]+tagLabels[6], nBins, min, max);
		histos[h][6]->Sumw2();
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
					arrayForErrors[4][x][y][z][t] = 0;
					arrayForErrors[5][x][y][z][t] = 0;
					arrayForErrors[6][x][y][z][t] = 0;
				}
			}
		}
	}


	TString files[names::NUM_PROCS];
	files[names::DATA] = "FINAL/Mar25_data.root";
	files[names::QCD] = files[names::DATA];
	files[names::TT7] = "FINAL/Mar25_ttjets7.root";
	files[names::TTLOW] = "Aug19_ttjetsFULL_sec0_ttpair__TriggernoWeight.root";
	files[names::TT10] = "FINAL/Mar25_ttjets10.root";
	files[names::TT7_SCALEUP] = "Mar4_ttjets7_ScaleUp_ptw.root";
	files[names::TT10_SCALEUP] = "Mar4_ttjets10_ScaleUp_ptw.root";
	files[names::TT7_Q2UP] = "FINAL/ttjets7_scaleup_ptw.root";
	files[names::TT10_Q2UP] = "FINAL/ttjets10_scaleup_ptw.root";
	files[names::TT7_SCALEDN] = "Mar4_ttjets7_ScaleDown_ptw.root";
	files[names::TT10_SCALEDN] = "Mar4_ttjets10_ScaleDown_ptw.root";
	files[names::TT7_Q2DN] = "FINAL/ttjets7_scaledown_ptw.root";
	files[names::TT10_Q2DN] = "FINAL/ttjets10_scaledown_ptw.root";
	files[names::TT7_MIS] = files[names::TT7];
	files[names::TT10_MIS] = files[names::TT10];
	files[names::ZPN20_MIS] = "Feb12_Zp20_mistag.root";
	files[names::ZPN10_MIS] = "Feb12_Zp10_mistag.root";
	files[names::ZPN15_MIS] = "Feb12_Zp15_mistag.root";
	files[names::ZPN30_MIS] = "Feb12_Zp30_mistag.root";
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
	files[names::ZPW20] = "Feb12_ZpW20_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPW10] = "Feb12_ZpW10_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPW15] = "Feb12_ZpW15_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPW30] = "Feb12_ZpW30_sec0_ttpair__TriggernoWeight.root";
	files[names::ZPW20_SCALEUP] = "Feb12_ZpW20_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPW10_SCALEUP] = "Feb12_ZpW10_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPW15_SCALEUP] = "Feb12_ZpW15_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPW30_SCALEUP] = "Feb12_ZpW30_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::ZPW20_SCALEDN] = "Feb12_ZpW20_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPW10_SCALEDN] = "Feb12_ZpW10_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPW15_SCALEDN] = "Feb12_ZpW15_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPW30_SCALEDN] = "Feb12_ZpW30_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::ZPW20_JERUP] = "Feb12_ZpW20_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPW10_JERUP] = "Feb12_ZpW10_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPW15_JERUP] = "Feb12_ZpW15_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPW30_JERUP] = "Feb12_ZpW30_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::ZPW20_JERDN] = "Feb12_ZpW20_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPW10_JERDN] = "Feb12_ZpW10_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPW15_JERDN] = "Feb12_ZpW15_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::ZPW30_JERDN] = "Feb12_ZpW30_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::RSG20] = "Feb12_RSG20_sec0_ttpair__TriggernoWeight.root";
	files[names::RSG10] = "Feb12_RSG10_sec0_ttpair__TriggernoWeight.root";
	files[names::RSG15] = "Feb12_RSG15_sec0_ttpair__TriggernoWeight.root";
	files[names::RSG30] = "Feb12_RSG30_sec0_ttpair__TriggernoWeight.root";
	files[names::RSG20_SCALEUP] = "Feb12_RSG20_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::RSG10_SCALEUP] = "Feb12_RSG10_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::RSG15_SCALEUP] = "Feb12_RSG15_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::RSG30_SCALEUP] = "Feb12_RSG30_sec0_ttpair_ScaleUp_TriggernoWeight.root";
	files[names::RSG20_SCALEDN] = "Feb12_RSG20_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::RSG10_SCALEDN] = "Feb12_RSG10_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::RSG15_SCALEDN] = "Feb12_RSG15_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::RSG30_SCALEDN] = "Feb12_RSG30_sec0_ttpair_ScaleDown_TriggernoWeight.root";
	files[names::RSG20_JERUP] = "Feb12_RSG20_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::RSG10_JERUP] = "Feb12_RSG10_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::RSG15_JERUP] = "Feb12_RSG15_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::RSG30_JERUP] = "Feb12_RSG30_sec0_ttpair_PtSmearUp_TriggernoWeight.root";
	files[names::RSG20_JERDN] = "Feb12_RSG20_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::RSG10_JERDN] = "Feb12_RSG10_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::RSG15_JERDN] = "Feb12_RSG15_sec0_ttpair_PtSmearDown_TriggernoWeight.root";
	files[names::RSG30_JERDN] = "Feb12_RSG30_sec0_ttpair_PtSmearDown_TriggernoWeight.root";



	ofstream event_list;
	event_list.open("data_events.txt");

	TFile *triggerFile = new TFile("trigger_weights.root");
	TH1F *triggerHist = (TH1F *) triggerFile->Get("triggerHist");

	for (int proc = 0; proc < names::NUM_PROCS; proc++){

	TChain *currentChain = new TChain("treeVars");
	currentChain->Add(files[proc]); 
	int nEvents = currentChain->GetEntries();


	float mttMass, mttMassPred, mistagWt, deltaY, jet1Pt, jet2Pt, jet1tau32, jet1Eta, jet2Eta, jet2tau32, jet1SubjetMaxBDisc, jet2SubjetMaxBDisc, jetPtForMistag;
	float cutflow, index, ptReweight;
	
	int run, lumi, event;











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
	currentChain->SetBranchAddress("deltaY", &deltaY);
	currentChain->SetBranchAddress("run", &run);
	currentChain->SetBranchAddress("event", &event);
	currentChain->SetBranchAddress("lumi", &lumi);
	currentChain->SetBranchAddress("jet1Eta", &jet1Eta);	
	currentChain->SetBranchAddress("jet2Eta", &jet2Eta);	

	cout << "Processing sample " << labels[proc] << " with   " << nEvents << " events" << endl;


	int QCDcount = 0;
	for (int i = 0; i < nEvents; i++){

		currentChain->GetEntry(i);

		float tau32cut = 0.7;
		float csvCut = 0.55;

		bool btag0 = jet1SubjetMaxBDisc < csvCut && jet2SubjetMaxBDisc < csvCut && jet1tau32 < tau32cut && jet2tau32 < tau32cut;
		bool btag1 = (jet1SubjetMaxBDisc > csvCut || jet2SubjetMaxBDisc > csvCut) && (jet1SubjetMaxBDisc < csvCut || jet2SubjetMaxBDisc <  csvCut) && jet1tau32 < tau32cut && jet2tau32 < tau32cut;
		bool btag2 = jet1SubjetMaxBDisc > csvCut && jet2SubjetMaxBDisc > csvCut && jet1tau32 < tau32cut && jet2tau32 < tau32cut;
		bool passDY = abs(deltaY) < 1.0;
		//if (ptReweight < 0.1) ptReweight = 0.70;
	
		bool passed = (btag0 || btag1 || btag2);
	
		float subSF1, subSF2 = 1.063;
		if (abs(jet1Eta) > 1.0) subSF1 = 0.906;
		if (abs(jet2Eta) > 1.0) subSF2 = 0.906;
		float triggerWt = triggerHist->GetBinContent( triggerHist->FindBin( jet1Pt + jet2Pt ) );
		float subSF = subSF1*subSF2;
		float bScore, tauScore = -999.9;
		
		if (proc == names::DATA){
			subSF = 1.0;
			triggerWt = 1.0;
		}

		if (proc <= names::TT10 && cutflow == 4 && index == 0 && passed) {

			if (proc == names::DATA && (btag2) && jet1Eta > -1 && jet1Eta < 0.0) event_list << run << " " << lumi << " " << event << endl;


			



			if (proc >= names::TT7_SCALEUP && proc <= names::TT10 ) {


				histos[proc][6]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag0 && passDY) histos[proc][0]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag1 && passDY) histos[proc][1]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag2 && passDY) histos[proc][2]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag0 && !passDY) histos[proc][3]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag1 && !passDY) histos[proc][4]->Fill(mttMass, ptReweight*subSF*triggerWt);
				if (btag2 && !passDY) histos[proc][5]->Fill(mttMass, ptReweight*subSF*triggerWt);
			}
			else {
				histos[proc][6]->Fill(mttMass, subSF*triggerWt);
				if (btag0 && passDY) histos[proc][0]->Fill(mttMass, subSF*triggerWt);
				if (btag1 && passDY) histos[proc][1]->Fill(mttMass, subSF*triggerWt);
				if (btag2 && passDY) histos[proc][2]->Fill(mttMass, subSF*triggerWt);
				if (btag0 && !passDY) histos[proc][3]->Fill(mttMass, subSF*triggerWt);
				if (btag1 && !passDY) histos[proc][4]->Fill(mttMass, subSF*triggerWt);
				if (btag2 && !passDY) histos[proc][5]->Fill(mttMass, subSF*triggerWt);
			}
		}
		else if (proc > names::TT10 && index == 1 && passed) {

			if (proc == names::TT7_MIS || proc == names::TT10_MIS) {
				histos[proc][6]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag0 && passDY) histos[proc][0]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag1 && passDY) histos[proc][1]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag2 && passDY) histos[proc][2]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag0 && !passDY) histos[proc][3]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag1 && !passDY) histos[proc][4]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
				if (btag2 && !passDY) histos[proc][5]->Fill(mttMassPred, mistagWt*ptReweight*triggerWt);
			}
			else {
				histos[proc][6]->Fill(mttMassPred, mistagWt);
				if (btag0 && passDY) histos[proc][0]->Fill(mttMassPred, mistagWt);
				if (btag1 && passDY) histos[proc][1]->Fill(mttMassPred, mistagWt);
				if (btag2 && passDY) histos[proc][2]->Fill(mttMassPred, mistagWt);
				if (btag0 && !passDY) histos[proc][3]->Fill(mttMassPred, mistagWt);
				if (btag1 && !passDY) histos[proc][4]->Fill(mttMassPred, mistagWt);
				if (btag2 && !passDY) histos[proc][5]->Fill(mttMassPred, mistagWt);
			}	
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
				arrayForErrors[6][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag0 && passDY) arrayForErrors[0][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag1 && passDY) arrayForErrors[1][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag2 && passDY) arrayForErrors[2][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag0 && !passDY) arrayForErrors[3][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag1 && !passDY) arrayForErrors[4][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
				if (btag2 && !passDY) arrayForErrors[5][ mtt_bin ][ int(floor(ptMap(jetPtForMistag)))][ int(floor(bMap(bScore)))][int(floor(tauMap(tauScore)))] += 1;		
		
			}
		}

	
	}	

	delete currentChain;



	}


	cout << "Done with Filling Histograms" << endl;

	event_list.close();

	//Keep track of b-tag SF cascading
	float btagSF[names::NUM_PROCS][nTagCats];
	float wB = 0.972;
	for (int proc = 0; proc < names::NUM_PROCS; proc++){

		if (proc == names::DATA || proc == names::QCD) {
			btagSF[proc][0] = 1.0;	
			btagSF[proc][1] = 1.0;	
			btagSF[proc][2] = 1.0;	
			btagSF[proc][3] = 1.0;	
			btagSF[proc][4] = 1.0;	
			btagSF[proc][5] = 1.0;	
			continue;
		}

	
		btagSF[proc][2] = wB * wB;
		btagSF[proc][5] = wB * wB;

		btagSF[proc][1] = wB + histos[proc][2]->GetEntries() / histos[proc][1]->GetEntries() * 2 * wB * ( 1 - wB );
		btagSF[proc][4] = wB + histos[proc][5]->GetEntries() / histos[proc][4]->GetEntries() * 2 * wB * ( 1 - wB );
	
		btagSF[proc][0] = 1 + histos[proc][2]->GetEntries() / histos[proc][0]->GetEntries() * (1-wB) * (1-wB) + histos[proc][1]->GetEntries() / histos[proc][0]->GetEntries() * (1-wB);	
		btagSF[proc][3] = 1 + histos[proc][5]->GetEntries() / histos[proc][3]->GetEntries() * (1-wB) * (1-wB) + histos[proc][4]->GetEntries() / histos[proc][3]->GetEntries() * (1-wB);	

		btagSF[proc][6] = 1.0;


		cout << "b-tag SFs: " ;
		cout << btagSF[proc][0] << " " ;
		cout << btagSF[proc][1] << " " ;
		cout << btagSF[proc][2] << " " ;
		cout << btagSF[proc][3] << " " ;
		cout << btagSF[proc][4] << " " ;
		cout << btagSF[proc][5] << " " ;
		cout << btagSF[proc][6] << " " << endl;


	}	



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

	for (int tag = 0; tag < nTagCats; tag++){


	histos[names::QCD][tag]->SetMarkerSize(0);

	histos[names::TT7][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7][tag] * 0.074 / 3082812. );
	histos[names::TTLOW][tag]->Scale( 245.8 * lumi  * btagSF[names::TTLOW][tag] * (204. / 16.) / 21675970. );
	histos[names::TT10][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10][tag] * 0.014 / 1249111. );
	histos[names::TT7_SCALEUP][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7_SCALEUP][tag] * 0.074 / 3082812. );
	histos[names::TT10_SCALEUP][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10_SCALEUP][tag] * 0.014 / 1249111. );
	histos[names::TT7_SCALEDN][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7_SCALEDN][tag] * 0.074 / 3082812. );
	histos[names::TT10_SCALEDN][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10_SCALEDN][tag] * 0.014 / 1249111. );
	histos[names::TT7_Q2UP][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7_Q2UP][tag] * 0.069 / 2243672. );
	histos[names::TT10_Q2UP][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10_Q2UP][tag] * 0.013 / 1241650. );
	histos[names::TT7_Q2DN][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7_Q2DN][tag] * 0.078 / 2170074. );
	histos[names::TT10_Q2DN][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10_Q2DN][tag] * 0.016 / 1308090. );
	histos[names::TT7_MIS][tag]->Scale( 245.8 * lumi  * btagSF[names::TT7_MIS][tag] * 0.074 / 3082812. );
	histos[names::TT10_MIS][tag]->Scale( 245.8 * lumi  * btagSF[names::TT10_MIS][tag] * 0.014 / 1249111. );


	histos[names::ZPN10][tag]->Scale( 1. * lumi * btagSF[names::ZPN10][tag]  / 101697. ); 
	histos[names::ZPN15][tag]->Scale( 1. * lumi * btagSF[names::ZPN15][tag]  /  94371. ); 
	histos[names::ZPN20][tag]->Scale( 1. * lumi * btagSF[names::ZPN20][tag]  /  90778. ); 
	histos[names::ZPN30][tag]->Scale( 1. * lumi * btagSF[names::ZPN30][tag]  /  91209. ); 
	histos[names::ZPN10_MIS][tag]->Scale( 1. * lumi * btagSF[names::ZPN10_MIS][tag]  / 101697. ); 
	histos[names::ZPN15_MIS][tag]->Scale( 1. * lumi * btagSF[names::ZPN15_MIS][tag]  /  94371. ); 
	histos[names::ZPN20_MIS][tag]->Scale( 1. * lumi * btagSF[names::ZPN20_MIS][tag]  /  90778. ); 
	histos[names::ZPN30_MIS][tag]->Scale( 1. * lumi * btagSF[names::ZPN30_MIS][tag]  /  91209. ); 
	histos[names::ZPN10_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN10_SCALEUP][tag]  / 101697. ); 
	histos[names::ZPN15_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN15_SCALEUP][tag]  /  94371. ); 
	histos[names::ZPN20_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN20_SCALEUP][tag]  /  90778. ); 
	histos[names::ZPN30_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN30_SCALEUP][tag]  /  91209. ); 
	histos[names::ZPN10_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN10_SCALEDN][tag]  / 101697. ); 
	histos[names::ZPN15_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN15_SCALEDN][tag]  /  94371. ); 
	histos[names::ZPN20_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN20_SCALEDN][tag]  /  90778. ); 
	histos[names::ZPN30_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN30_SCALEDN][tag] /  91209. ); 
	histos[names::ZPN10_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN10_JERUP][tag]  / 101697. ); 
	histos[names::ZPN15_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN15_JERUP][tag]  /  94371. ); 
	histos[names::ZPN20_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN20_JERUP][tag]  /  90778. ); 
	histos[names::ZPN30_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPN30_JERUP][tag]  /  91209. ); 
	histos[names::ZPN10_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN10_JERDN][tag]  / 101697. ); 
	histos[names::ZPN15_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN15_JERDN][tag]  /  94371. ); 
	histos[names::ZPN20_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN20_JERDN][tag]  /  90778. ); 
	histos[names::ZPN30_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPN30_JERDN][tag]  /  91209. ); 
	histos[names::ZPW10][tag]->Scale( 1. * lumi * btagSF[names::ZPW10][tag]  / 102618. ); 
	histos[names::ZPW15][tag]->Scale( 1. * lumi * btagSF[names::ZPW15][tag]  /  96246. ); 
	histos[names::ZPW20][tag]->Scale( 1. * lumi * btagSF[names::ZPW20][tag]  /  93869. ); 
	histos[names::ZPW30][tag]->Scale( 1. * lumi * btagSF[names::ZPW30][tag]  /  97234. ); 
	histos[names::ZPW10_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW10_SCALEUP][tag]  / 102618. ); 
	histos[names::ZPW15_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW15_SCALEUP][tag]  /  96246. ); 
	histos[names::ZPW20_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW20_SCALEUP][tag]  /  93869. ); 
	histos[names::ZPW30_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW30_SCALEUP][tag]  /  97234. ); 
	histos[names::ZPW10_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW10_SCALEDN][tag]  / 102618. ); 
	histos[names::ZPW15_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW15_SCALEDN][tag]  /  96246. ); 
	histos[names::ZPW20_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW20_SCALEDN][tag]  /  93869. ); 
	histos[names::ZPW30_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW30_SCALEDN][tag]  /  97234. ); 
	histos[names::ZPW10_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW10_JERUP][tag]  / 102618. ); 
	histos[names::ZPW15_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW15_JERUP][tag]  /  96246. ); 
	histos[names::ZPW20_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW20_JERUP][tag]  /  93869. ); 
	histos[names::ZPW30_JERUP][tag]->Scale( 1. * lumi * btagSF[names::ZPW30_JERUP][tag]  /  97234. ); 
	histos[names::ZPW10_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW10_JERDN][tag]  / 102618. ); 
	histos[names::ZPW15_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW15_JERDN][tag]  /  96246. ); 
	histos[names::ZPW20_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW20_JERDN][tag]  /  93869. ); 
	histos[names::ZPW30_JERDN][tag]->Scale( 1. * lumi * btagSF[names::ZPW30_JERDN][tag]  /  97234. ); 
	histos[names::RSG10][tag]->Scale( 1. * lumi * btagSF[names::RSG10][tag]  / 100000. ); 
	histos[names::RSG15][tag]->Scale( 1. * lumi * btagSF[names::RSG15][tag]  /  100000. ); 
	histos[names::RSG20][tag]->Scale( 1. * lumi * btagSF[names::RSG20][tag]  /  100000. ); 
	histos[names::RSG30][tag]->Scale( 1. * lumi * btagSF[names::RSG30][tag]  /  100000. ); 
	histos[names::RSG10_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::RSG10_SCALEUP][tag]  / 100000. ); 
	histos[names::RSG15_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::RSG15_SCALEUP][tag]  /  100000. ); 
	histos[names::RSG20_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::RSG20_SCALEUP][tag]  /  100000. ); 
	histos[names::RSG30_SCALEUP][tag]->Scale( 1. * lumi * btagSF[names::RSG30_SCALEUP][tag]  /  100000. ); 
	histos[names::RSG10_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::RSG10_SCALEDN][tag]  / 100000. ); 
	histos[names::RSG15_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::RSG15_SCALEDN][tag]  /  100000. ); 
	histos[names::RSG20_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::RSG20_SCALEDN][tag]  /  100000. ); 
	histos[names::RSG30_SCALEDN][tag]->Scale( 1. * lumi * btagSF[names::RSG30_SCALEDN][tag]  /  100000. ); 
	histos[names::RSG10_JERUP][tag]->Scale( 1. * lumi * btagSF[names::RSG10_JERUP][tag]  / 100000. ); 
	histos[names::RSG15_JERUP][tag]->Scale( 1. * lumi * btagSF[names::RSG15_JERUP][tag]  /  100000. ); 
	histos[names::RSG20_JERUP][tag]->Scale( 1. * lumi * btagSF[names::RSG20_JERUP][tag]  /  100000. ); 
	histos[names::RSG30_JERUP][tag]->Scale( 1. * lumi * btagSF[names::RSG30_JERUP][tag]  /  100000. ); 
	histos[names::RSG10_JERDN][tag]->Scale( 1. * lumi * btagSF[names::RSG10_JERDN][tag]  / 100000. ); 
	histos[names::RSG15_JERDN][tag]->Scale( 1. * lumi * btagSF[names::RSG15_JERDN][tag]  /  100000. ); 
	histos[names::RSG20_JERDN][tag]->Scale( 1. * lumi * btagSF[names::RSG20_JERDN][tag]  /  100000. ); 
	histos[names::RSG30_JERDN][tag]->Scale( 1. * lumi * btagSF[names::RSG30_JERDN][tag]  /  100000. ); 



	histos[names::TT7][tag]->Add(histos[names::TT10][tag]);
	//histos[names::TT7][tag]->Add(histos[names::TTLOW][tag]);
	histos[names::TT7_SCALEUP][tag]->Add(histos[names::TT10_SCALEUP][tag]);
	histos[names::TT7_SCALEDN][tag]->Add(histos[names::TT10_SCALEDN][tag]);
	histos[names::TT7_Q2DN][tag]->Add(histos[names::TT10_Q2DN][tag]);
	histos[names::TT7_Q2UP][tag]->Add(histos[names::TT10_Q2UP][tag]);

	histos[names::TT7][tag]->SetFillColor(kRed);
	histos[names::QCD][tag]->SetFillColor(kYellow);
	histos[names::QCD][tag]->Add(histos[names::TT7_MIS][tag], -1);
	histos[names::QCD][tag]->Add(histos[names::TT10_MIS][tag], -1);


	TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,500);
	c1->Range(0,0,1,1);
	c1->Draw();

	TPad *c1_1 = new TPad("c1_1", "newpad",0.01,0.01,0.99,0.25);
	c1_1->Draw();
	TPad *c1_2 = new TPad("c1_2", "newpad",0.01,0.25,0.99,0.99);
	c1_2->Draw(); 

	c1->cd();
	c1_2->cd();
	c1_2->SetTopMargin(0.1);
	c1_2->SetBottomMargin(0.1);
	c1_2->SetRightMargin(0.05);
	c1_2->SetLeftMargin(0.1);
	c1_2->SetFillStyle(0);




	
	histos[names::DATA][tag]->SetMaximum(1.4 * histos[names::DATA][tag]->GetMaximum() );
	histos[names::DATA][tag]->SetMarkerStyle(20);
	histos[names::DATA][tag]->GetYaxis()->SetTitle("Events / (50 GeV)");
	histos[names::DATA][tag]->Draw("E");	
 
	THStack *stack = new THStack();
	stack->Add(histos[names::TT7][tag]);
	stack->Add(histos[names::QCD][tag]);
	stack->Draw("hist same");
	histos[names::DATA][tag]->SetMarkerStyle(21);
	histos[names::DATA][tag]->Draw("E same");

	histos[names::ZPN10][tag]->SetLineColor(kBlue);
	histos[names::ZPN20][tag]->SetLineColor(kGreen+1);
	histos[names::ZPN30][tag]->SetLineColor(kViolet);
	histos[names::ZPN10][tag]->SetLineWidth(2);
	histos[names::ZPN20][tag]->SetLineWidth(2);
	histos[names::ZPN30][tag]->SetLineWidth(2);

	histos[names::ZPN10][tag]->Draw("hist same");
	histos[names::ZPN20][tag]->Draw("hist same");
	histos[names::ZPN30][tag]->Draw("hist same");


	TH1F *totalH = new TH1F("totalH", "totalH", nBins, min, max);	
    	totalH = (TH1F *) histos[names::QCD][tag]->Clone("totalH");
	totalH->Add(histos[names::TT7][tag]);
	totalH->SetFillStyle(3004);
    	totalH->SetFillColor(kBlack);
    	totalH->Draw("same E2");

	TLatex *cmsLabel = new TLatex();
	cmsLabel->SetNDC();
	cmsLabel->DrawLatex(0.1,0.91, "CMS Preliminary, #sqrt{s} = 8 TeV, 19.7 fb^{-1}");

	if (tag == 0) cmsLabel->DrawLatex(0.7, 0.85, "0 b-tags, |#Deltay| < 1.0");
	if (tag == 1) cmsLabel->DrawLatex(0.7, 0.85, "1 b-tags, |#Deltay| < 1.0");
	if (tag == 2) cmsLabel->DrawLatex(0.7, 0.85, "2 b-tags, |#Deltay| < 1.0");
	if (tag == 3) cmsLabel->DrawLatex(0.7, 0.85, "0 b-tags, |#Deltay| > 1.0");
	if (tag == 4) cmsLabel->DrawLatex(0.7, 0.85, "1 b-tags, |#Deltay| > 1.0");
	if (tag == 5) cmsLabel->DrawLatex(0.7, 0.85, "2 b-tags, |#Deltay| > 1.0");
	if (tag == 6) cmsLabel->DrawLatex(0.7, 0.85, "All Signal Regions");



	gPad->RedrawAxis();
	
	TLegend *leg = new TLegend(0.7, 0.4, 0.94, 0.8);
	leg->AddEntry(histos[names::DATA][tag], "Data", "lp");
	leg->AddEntry(histos[names::QCD][tag], "NTMJ", "f");
	leg->AddEntry(histos[names::TT7][tag], "Top", "f");
	leg->AddEntry(histos[names::ZPN10][tag], "1 TeV Narrow Z'", "l");
	leg->AddEntry(histos[names::ZPN20][tag], "2 TeV Narrow Z'", "l");
	leg->AddEntry(histos[names::ZPN30][tag], "3 TeV Narrow Z'", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(0);
	leg->Draw("same");

	TH1F *ratioH = new TH1F();
	ratioH = (TH1F*) histos[names::DATA][tag]->Clone("ratio");
	ratioH->Sumw2();
	ratioH->Divide(ratioH, totalH, 1, 1, "B");




	c1_1->cd();
	c1_1->SetTopMargin(0.01);
	c1_1->SetBottomMargin(0.3);
	c1_1->SetRightMargin(0.05);
	c1_1->SetLeftMargin(0.1);
	c1_1->SetFillStyle(0);

	ratioH->GetYaxis()->SetRangeUser(0.,2.);
	ratioH->GetYaxis()->SetTitle("Data / BG Ratio");
	ratioH->GetYaxis()->SetTitleOffset(0.4);
	ratioH->GetYaxis()->SetTitleSize(0.11);
	ratioH->GetXaxis()->SetLabelSize(0.11);
	ratioH->GetXaxis()->SetTitleSize(0.11);
	ratioH->GetXaxis()->SetTitle( "t#bar{t} Invariant Mass [GeV]");
	ratioH->Draw("E");
	
	TF1 *line = new TF1("line", "1", min, max);
	line->SetLineColor(kBlack);
	line->Draw("same");
	
	gPad->RedrawAxis();






	double error = -999.9;
	histos[names::QCD][tag]->IntegralAndError(0, nBins+1, error);
	double ttError = -999.9;
	histos[names::TT7][tag]->IntegralAndError(0, nBins+1, ttError);

	double totError = sqrt(error*error + ttError*ttError);


	cout << tag << " B-TAG CATEGORY" << endl;
	cout << "==================================" << endl;
	cout << "QCD       :" << histos[names::QCD][tag]->Integral() << "  +/-  " << error << endl;
	cout << "ttbar     :" << histos[names::TT7][tag]->Integral() << " +/- " << ttError << endl;
	cout << "ttbar (low)" << histos[names::TTLOW][tag]->Integral() << endl;
	cout << "Total BG  :" << histos[names::QCD][tag]->Integral() + histos[names::TT7][tag]->Integral() << " +/- " << totError << endl;
	cout << "Data      :" << histos[names::DATA][tag]->Integral() << endl;




	




	c1->SaveAs("errors"+tagLabels[tag]+".pdf");
	c1->SaveAs("errors"+tagLabels[tag]+".png");
	c1_2->SetLogy(1);
	c1->SaveAs("errors"+tagLabels[tag]+"_log.pdf");
	c1->SaveAs("errors"+tagLabels[tag]+"_log.png");
	}


	outFile->cd();
	//binFillHist->Write();
	for (int tag = 0; tag < 6; tag++){



		TH1F *upQCDH = (TH1F *) histos[names::QCD][tag]->Clone("upQCDH");
		TH1F *dnQCDH = (TH1F *) histos[names::QCD][tag]->Clone("dnQCDH");

		for (int Nbin = 0; Nbin < histos[names::QCD][tag]->GetNbinsX(); Nbin++){

			if (histos[names::QCD][tag]->GetBinContent(Nbin) <= 0.0) histos[names::QCD][tag]->SetBinContent(Nbin, 0.0);
			upQCDH->SetBinContent( Nbin, histos[names::QCD][tag]->GetBinContent(Nbin) + histos[names::QCD][tag]->GetBinError(Nbin) ); 
			dnQCDH->SetBinContent( Nbin, histos[names::QCD][tag]->GetBinContent(Nbin) - histos[names::QCD][tag]->GetBinError(Nbin) ); 
			histos[names::QCD][tag]->SetBinError(Nbin, sqrt(histos[names::QCD][tag]->GetBinContent(Nbin)));
			

		}



		cout << "TAG CATEGORY " << tag << endl;

		cout.precision(2);	
		cout << "NRW Z' 1000 :  "  << histos[names::ZPN10][tag]->GetEntries() / 101697. << endl;
		cout << "NRW Z' 1500 :  "  << histos[names::ZPN15][tag]->GetEntries() / 94371. << endl;
		cout << "NRW Z' 2000 :  "  << histos[names::ZPN20][tag]->GetEntries() / 90778. << endl;
		cout << "NRW Z' 3000 :  "  << histos[names::ZPN30][tag]->GetEntries() / 91209. << endl;
 	
		cout << "WDE Z' 1000 :  "  << histos[names::ZPW10][tag]->GetEntries() / 102618. << endl;
		cout << "WDE Z' 1500 :  "  << histos[names::ZPW15][tag]->GetEntries() / 96246. << endl;
		cout << "WDE Z' 2000 :  "  << histos[names::ZPW20][tag]->GetEntries() / 93869. << endl;
		cout << "WDE Z' 3000 :  "  << histos[names::ZPW30][tag]->GetEntries() / 97234. << endl;
		
		cout << "RSG    1000 :  "  << histos[names::RSG10][tag]->GetEntries() / 100000. << endl;
		cout << "RSG    1500 :  "  << histos[names::RSG15][tag]->GetEntries() / 100000. << endl;
		cout << "RSG    2000 :  "  << histos[names::RSG20][tag]->GetEntries() / 100000. << endl;
		cout << "RSG    3000 :  "  << histos[names::RSG30][tag]->GetEntries() / 100000. << endl;

		upQCDH->Write( Form("btag%d__qcd__misErr__plus", tag) );
		dnQCDH->Write( Form("btag%d__qcd__misErr__minus", tag) );

		histos[names::QCD][tag]->Write( Form("btag%d__qcd", tag) );
		histos[names::TT7][tag]->Write( Form("btag%d__ttbar", tag) );
		histos[names::TT7_SCALEUP][tag]->Write( Form("btag%d__ttbar__jes__plus", tag) );
		histos[names::TT7_SCALEDN][tag]->Write( Form("btag%d__ttbar__jes__minus", tag) );
		histos[names::TT7_Q2UP][tag]->Write( Form("btag%d__ttbar__q2__plus", tag) );
		histos[names::TT7_Q2DN][tag]->Write( Form("btag%d__ttbar__q2__minus", tag) );
		histos[names::DATA][tag]->Write( Form("btag%d__DATA", tag) );
		bool writeZPN = ZPNflag;
		if (writeZPN){
		histos[names::ZPN10][tag]->Write( Form("btag%d__Zprime1000", tag));
		histos[names::ZPN15][tag]->Write( Form("btag%d__Zprime1500", tag) );
		histos[names::ZPN20][tag]->Write( Form("btag%d__Zprime2000", tag) );
		histos[names::ZPN30][tag]->Write( Form("btag%d__Zprime3000", tag) );
		//histos[names::ZPN10_MIS][tag]->Write( Form("btag%d__Zprime1000_mis", tag));
		//histos[names::ZPN15_MIS][tag]->Write( Form("btag%d__Zprime1500_mis", tag) );
		//histos[names::ZPN20_MIS][tag]->Write( Form("btag%d__Zprime2000_mis", tag) );
		//histos[names::ZPN30_MIS][tag]->Write( Form("btag%d__Zprime3000_mis", tag) );
		histos[names::ZPN10_SCALEUP][tag]->Write( Form("btag%d__Zprime1000__jes__plus", tag));
		histos[names::ZPN15_SCALEUP][tag]->Write( Form("btag%d__Zprime1500__jes__plus", tag) );
		histos[names::ZPN20_SCALEUP][tag]->Write( Form("btag%d__Zprime2000__jes__plus", tag) );
		histos[names::ZPN30_SCALEUP][tag]->Write( Form("btag%d__Zprime3000__jes__plus", tag) );
		histos[names::ZPN10_SCALEDN][tag]->Write( Form("btag%d__Zprime1000__jes__minus", tag));
		histos[names::ZPN15_SCALEDN][tag]->Write( Form("btag%d__Zprime1500__jes__minus", tag) );
		histos[names::ZPN20_SCALEDN][tag]->Write( Form("btag%d__Zprime2000__jes__minus", tag) );
		histos[names::ZPN30_SCALEDN][tag]->Write( Form("btag%d__Zprime3000__jes__minus", tag) );
		histos[names::ZPN10_JERUP][tag]->Write( Form("btag%d__Zprime1000__jer__plus", tag));
		histos[names::ZPN15_JERUP][tag]->Write( Form("btag%d__Zprime1500__jer__plus", tag) );
		histos[names::ZPN20_JERUP][tag]->Write( Form("btag%d__Zprime2000__jer__plus", tag) );
		histos[names::ZPN30_JERUP][tag]->Write( Form("btag%d__Zprime3000__jer__plus", tag) );
		histos[names::ZPN10_JERDN][tag]->Write( Form("btag%d__Zprime1000__jer__minus", tag));
		histos[names::ZPN15_JERDN][tag]->Write( Form("btag%d__Zprime1500__jer__minus", tag) );
		histos[names::ZPN20_JERDN][tag]->Write( Form("btag%d__Zprime2000__jer__minus", tag) );
		histos[names::ZPN30_JERDN][tag]->Write( Form("btag%d__Zprime3000__jer__minus", tag) );
		}
		bool writeZPW = ZPWflag;
		if (writeZPW){
		histos[names::ZPW10][tag]->Write( Form("btag%d__ZprimeWide1000", tag));
		histos[names::ZPW15][tag]->Write( Form("btag%d__ZprimeWide1500", tag) );
		histos[names::ZPW20][tag]->Write( Form("btag%d__ZprimeWide2000", tag) );
		histos[names::ZPW30][tag]->Write( Form("btag%d__ZprimeWide3000", tag) );
		histos[names::ZPW10_SCALEUP][tag]->Write( Form("btag%d__ZprimeWide1000__jes__plus", tag));
		histos[names::ZPW15_SCALEUP][tag]->Write( Form("btag%d__ZprimeWide1500__jes__plus", tag) );
		histos[names::ZPW20_SCALEUP][tag]->Write( Form("btag%d__ZprimeWide2000__jes__plus", tag) );
		histos[names::ZPW30_SCALEUP][tag]->Write( Form("btag%d__ZprimeWide3000__jes__plus", tag) );
		histos[names::ZPW10_SCALEDN][tag]->Write( Form("btag%d__ZprimeWide1000__jes__minus", tag));
		histos[names::ZPW15_SCALEDN][tag]->Write( Form("btag%d__ZprimeWide1500__jes__minus", tag) );
		histos[names::ZPW20_SCALEDN][tag]->Write( Form("btag%d__ZprimeWide2000__jes__minus", tag) );
		histos[names::ZPW30_SCALEDN][tag]->Write( Form("btag%d__ZprimeWide3000__jes__minus", tag) );
		histos[names::ZPW10_JERUP][tag]->Write( Form("btag%d__ZprimeWide1000__jer__plus", tag));
		histos[names::ZPW15_JERUP][tag]->Write( Form("btag%d__ZprimeWide1500__jer__plus", tag) );
		histos[names::ZPW20_JERUP][tag]->Write( Form("btag%d__ZprimeWide2000__jer__plus", tag) );
		histos[names::ZPW30_JERUP][tag]->Write( Form("btag%d__ZprimeWide3000__jer__plus", tag) );
		histos[names::ZPW10_JERDN][tag]->Write( Form("btag%d__ZprimeWide1000__jer__minus", tag));
		histos[names::ZPW15_JERDN][tag]->Write( Form("btag%d__ZprimeWide1500__jer__minus", tag) );
		histos[names::ZPW20_JERDN][tag]->Write( Form("btag%d__ZprimeWide2000__jer__minus", tag) );
		histos[names::ZPW30_JERDN][tag]->Write( Form("btag%d__ZprimeWide3000__jer__minus", tag) );
		}
		bool writeRSG = RSGflag;
		if (writeRSG){
		histos[names::RSG10][tag]->Write( Form("btag%d__RSgluon1000", tag));
		histos[names::RSG15][tag]->Write( Form("btag%d__RSgluon1500", tag) );
		histos[names::RSG20][tag]->Write( Form("btag%d__RSgluon2000", tag) );
		histos[names::RSG30][tag]->Write( Form("btag%d__RSgluon3000", tag) );
		histos[names::RSG10_SCALEUP][tag]->Write( Form("btag%d__RSgluon1000__jes__plus", tag));
		histos[names::RSG15_SCALEUP][tag]->Write( Form("btag%d__RSgluon1500__jes__plus", tag) );
		histos[names::RSG20_SCALEUP][tag]->Write( Form("btag%d__RSgluon2000__jes__plus", tag) );
		histos[names::RSG30_SCALEUP][tag]->Write( Form("btag%d__RSgluon3000__jes__plus", tag) );
		histos[names::RSG10_SCALEDN][tag]->Write( Form("btag%d__RSgluon1000__jes__minus", tag));
		histos[names::RSG15_SCALEDN][tag]->Write( Form("btag%d__RSgluon1500__jes__minus", tag) );
		histos[names::RSG20_SCALEDN][tag]->Write( Form("btag%d__RSgluon2000__jes__minus", tag) );
		histos[names::RSG30_SCALEDN][tag]->Write( Form("btag%d__RSgluon3000__jes__minus", tag) );
		histos[names::RSG10_JERUP][tag]->Write( Form("btag%d__RSgluon1000__jer__plus", tag));
		histos[names::RSG15_JERUP][tag]->Write( Form("btag%d__RSgluon1500__jer__plus", tag) );
		histos[names::RSG20_JERUP][tag]->Write( Form("btag%d__RSgluon2000__jer__plus", tag) );
		histos[names::RSG30_JERUP][tag]->Write( Form("btag%d__RSgluon3000__jer__plus", tag) );
		histos[names::RSG10_JERDN][tag]->Write( Form("btag%d__RSgluon1000__jer__minus", tag));
		histos[names::RSG15_JERDN][tag]->Write( Form("btag%d__RSgluon1500__jer__minus", tag) );
		histos[names::RSG20_JERDN][tag]->Write( Form("btag%d__RSgluon2000__jer__minus", tag) );
		histos[names::RSG30_JERDN][tag]->Write( Form("btag%d__RSgluon3000__jer__minus", tag) );
		}





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






	


	

