#include <cstdlib>
#include <iostream>
#include <map>
#include <string>


#include "TMVA/Reader.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMVA/Tools.h"

void TreeFiller(string inFiles, string outFile, string histName) {

	
	TChain *origFiles = new TChain("treeVars", "treeVars");
	origFiles->Add(inFiles.c_str());
	
	int nEntries = origFiles->GetEntries();
	cout << "The old chain contains " << nEntries << " entries." << endl;
	
	
	float puWeight, jet1Eta, jet2Eta, deltaY, genTopPt1, genTopPt2, jet1Mass, jet2Mass, jet1MinMass, jet2MinMass, jet1BDisc, jet2BDisc, jet1SubjetMaxBDisc, jet2SubjetMaxBDisc,
			jet1tau32, jet2tau32, jet1Pt, jet2Pt, jetPtForMistag, mttMass, mttMassPred, mistagWt, mistagWtErr, mistagWtAll, mistagWtNsubAll, mistagWtNsub, NNoutput, ptReweight, cutflow, index, trigWt;
	int jet1NSubjets, jet2NSubjets, npv;
		
	origFiles->SetBranchAddress("npv", &npv);
	origFiles->SetBranchAddress("jet1Eta", &jet1Eta);
	origFiles->SetBranchAddress("jet2Eta", &jet2Eta);
	origFiles->SetBranchAddress("deltaY", &deltaY);
	origFiles->SetBranchAddress("jet1Mass", &jet1Mass);
	origFiles->SetBranchAddress("jet2Mass", &jet2Mass);
	origFiles->SetBranchAddress("jet1minMass", &jet1MinMass);
	origFiles->SetBranchAddress("jet2minMass", &jet2MinMass);
	origFiles->SetBranchAddress("jet1Nsubj", &jet1NSubjets);
	origFiles->SetBranchAddress("jet2Nsubj", &jet2NSubjets);
	origFiles->SetBranchAddress("jet1BDisc", &jet1BDisc);
	origFiles->SetBranchAddress("jet2BDisc", &jet2BDisc);
	origFiles->SetBranchAddress("jet1SubjetMaxBDisc", &jet1SubjetMaxBDisc);
	origFiles->SetBranchAddress("jet2SubjetMaxBDisc", &jet2SubjetMaxBDisc);
	origFiles->SetBranchAddress("jet1tau32", &jet1tau32);
	origFiles->SetBranchAddress("jet2tau32", &jet2tau32);
	origFiles->SetBranchAddress("jet1Pt", &jet1Pt);
	origFiles->SetBranchAddress("jet2Pt", &jet2Pt);
	origFiles->SetBranchAddress("jetPtForMistag", &jetPtForMistag);
	origFiles->SetBranchAddress("mttMass", &mttMass);
	origFiles->SetBranchAddress("mttMassPred", &mttMassPred);
	origFiles->SetBranchAddress("index", &index);
	origFiles->SetBranchAddress("cutflow", &cutflow);
	origFiles->SetBranchAddress("NNoutput", &NNoutput);
	origFiles->SetBranchAddress("mistagWt", &mistagWt);
	origFiles->SetBranchAddress("genTopPt1", &genTopPt1);	
	origFiles->SetBranchAddress("genTopPt2", &genTopPt2);	
	origFiles->SetBranchAddress("trigWt", &trigWt);
	
	TFile *newFile = new TFile(outFile.c_str(), "RECREATE");
	TTree *newTree = origFiles->CloneTree(0);
	newTree->Branch("mistagWtNsub", &mistagWtNsub, "mistagWtNsub/F");
	newTree->Branch("mistagWtNsubAll", &mistagWtNsubAll, "mistagWtNsubAll/F");
	newTree->Branch("mistagWtAll", &mistagWtAll, "mistagWtAll/F");
	newTree->Branch("ptReweight", &ptReweight, "ptReweight/F");
	newTree->Branch("puWeight", &puWeight, "puWeight/F");
	newTree->Branch("mistagWtErr", &mistagWtErr, "mistagWtErr/F");

	newTree->SetBranchAddress("mistagWtNsub", &mistagWtNsub);
	newTree->SetBranchAddress("misagWtNsubAll", &mistagWtNsubAll);
	newTree->SetBranchAddress("mistagWtAll", &mistagWtAll);
	newTree->SetBranchAddress("ptReweight", &ptReweight);
	newTree->SetBranchAddress("puWeight", &puWeight);
	newTree->SetBranchAddress("mistagWtErr", &mistagWtErr);

/*	
	TMVA::Reader* reader = new TMVA::Reader();
	reader->AddVariable("jet1Eta", &jet1Eta);
	reader->AddVariable("jet2Eta", &jet2Eta);
	reader->AddVariable("deltaY", &deltaY);
	reader->AddVariable("jet1Mass", &jet1Mass);
	reader->AddVariable("jet2Mass", &jet2Mass);
	reader->AddVariable("jet1BDisc", &jet1BDisc);
	reader->AddVariable("jet2BDisc", &jet2BDisc);
	reader->AddVariable("jet1SubjetMaxBDisc", &jet1SubjetMaxBDisc);
	reader->AddVariable("jet2SubjetMaxBDisc", &jet2SubjetMaxBDisc);
	reader->AddVariable("jet1tau32", &jet1tau32);
	reader->AddVariable("jet2tau32", &jet2tau32);
	reader->AddVariable("jet1Pt", &jet1Pt);
	reader->AddVariable("jet2Pt", &jet2Pt);
	reader->AddVariable("mttMass", &mttMass);

	reader->BookMVA("MLP", "weights/TMVA_tt_Zp_MLP.weightsZp10_cut4.xml");
*/	
	//TFile *mistagFileLow = new TFile("notCSVL_notCSVM_mistag.root");
	//TFile *mistagFileMed = new TFile("CSVL_notCSVM_mistag.root");
	//TFile *mistagFileHi = new TFile("CSVM_mistag.root");
	TFile *mistagFile = new TFile("Mar26_mistag.root");//data_AllBscore_mistag_Dec16.root");
	histName = "MISTAG_RATE_SUB_TTBAR_Inclusive";
	TFile *triggerFile = new TFile("trigger_weights.root");
	TH1F *triggerHist = (TH1F *) triggerFile->Get("triggerHist");
	TH3F *mistagRateHistAll = (TH3F *) mistagFile->Get( histName.c_str() )->Clone();	
	cout << histName << endl;
	cout << "Entries " << mistagRateHistAll->Integral() << endl;	
	TFile *puFile = new TFile("puHists.root");
	TH1F *puWeightsHist = (TH1F *) puFile->Get("weightsH");
	
	for (int i = 0; i < origFiles->GetEntries(); i++){
	
		origFiles->GetEntry(i);
		if  (i % 1000000 == 0) cout << 100*(float(i) / float(nEntries)) << " Percent Complete." << endl;
		mistagWt = 0.000;
		mistagWtNsub = 30.0000;
		puWeight = puWeightsHist->GetBinContent(npv); 	
		triggerWt = triggerHist->GetBinContent( triggerHist->FindBin( jet1Pt + jet2Pt ) );

		if (cutflow == 4 || index == 1){
		
			if (genTopPt1 > 400) genTopPt1 = 400;
			if (genTopPt2 > 400) genTopPt2 = 400;
			//NNoutput =  reader->EvaluateMVA("MLP");
			ptReweight = sqrt( exp(0.156 - 0.00137*genTopPt1)*exp(0.156 - 0.00137*genTopPt2) );			
			
			if (index == 1) {
				
				float bScore = -9.99;
				float tauScore = 9.99;
				int probeJet = 0;
				if (jet1Pt == jetPtForMistag) {
					probeJet = 1;
					bScore = jet1SubjetMaxBDisc;
					tauScore = jet1tau32;
				}
				if (jet2Pt == jetPtForMistag) {
					bScore = jet2SubjetMaxBDisc;
					tauScore = jet2tau32;
					probeJet = 2;
				}

				int bin = mistagRateHistAll->FindBin( ptMap(jetPtForMistag),bMap(bScore),tauMap(tauScore) );
				mistagWt = mistagRateHistAll->GetBinContent( bin );
				mistagWtErr = mistagRateHistAll->GetBinError( bin );
			}
		
			newTree->Fill();
		
		}
		

	}


	
	
	


	newFile->cd();
	newTree->Write();
	newFile->Write();
	newFile->Close();
	
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
	else if (b < 1.00) out = 3.1;

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
	
	
