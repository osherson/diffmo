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
	
	
	float jet1Eta, jet2Eta, deltaY, genTopPt1, genTopPt2, jet1Mass, jet2Mass, jet1MinMass, jet2MinMass, jet1BDisc, jet2BDisc, jet1SubjetMaxBDisc, jet2SubjetMaxBDisc,
			jet1tau32, jet2tau32, jet1Pt, jet2Pt, jetPtForMistag, mttMass, mttMassPred, mistagWt, mistagWtAll, mistagWtNsubAll, mistagWtNsub, index, cutflow, NNoutput, ptReweight;
	int jet1NSubjets, jet2NSubjets;
		
	
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

	
	TFile *newFile = new TFile(outFile.c_str(), "RECREATE");
	TTree *newTree = origFiles->CloneTree(0);
	newTree->Branch("mistagWtNsub", &mistagWtNsub, "mistagWtNsub/F");
	newTree->Branch("mistagWtNsubAll", &mistagWtNsubAll, "mistagWtNsubAll/F");
	newTree->Branch("mistagWtAll", &mistagWtAll, "mistagWtAll/F");
	newTree->Branch("ptReweight", &ptReweight, "ptReweight/F");
	
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
	TFile *mistagFileLow = new TFile("data_LowBscore_mistag_Dec16.root");
	TFile *mistagFileMed = new TFile("data_MedBscore_mistag_Dec16.root");
	TFile *mistagFileHi = new TFile("data_HiBscore_mistag_Dec16.root");
	TFile *mistagFile = new TFile("data_AllBscore_mistag_Dec16.root");
	histName = "MISTAG_RATE_SUB_TTBAR_Inclusive";
	TH1F *mistagRateHistAll = (TH1F *) mistagFile->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistLow = (TH1F *) mistagFileLow->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistMed = (TH1F *) mistagFileMed->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistHi = (TH1F *) mistagFileHi->Get( histName.c_str() )->Clone();	
	histName = "MISTAG_RATE_SUB_TTBAR_InclNsub";
	TH1F *mistagRateHistNSAll = (TH1F *) mistagFile->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistNSLow = (TH1F *) mistagFileLow->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistNSMed = (TH1F *) mistagFileMed->Get( histName.c_str() )->Clone();	
	TH1F *mistagRateHistNSHi = (TH1F *) mistagFileHi->Get( histName.c_str() )->Clone();	
	cout << histName << endl;
	cout << "Entries " << mistagRateHistAll->Integral() << endl;	
	
	for (int i = 0; i < nEntries; i++){
	
		origFiles->GetEntry(i);
		if  (i % 1000000 == 0) cout << 100*(float(i) / float(nEntries)) << " Percent Complete." << endl;
		mistagWt = 0.000;

	
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
					tauScore = jet2tau32;
				}
				if (jet2Pt == jetPtForMistag) {
					bScore = jet2SubjetMaxBDisc;
					tauScore = jet1tau32;
					probeJet = 2;
				}
				if (bScore > 0.679) mistagWt = mistagRateHistHi->GetBinContent( mistagRateHistHi->FindBin( jetPtForMistag ) );			
				else if (bScore > 0.244) mistagWt = mistagRateHistMed->GetBinContent( mistagRateHistMed->FindBin( jetPtForMistag ) );			
				else mistagWt = mistagRateHistLow->GetBinContent( mistagRateHistLow->FindBin( jetPtForMistag ) );			
				mistagWtAll = mistagRateHistAll->GetBinContent( mistagRateHistAll->FindBin( jetPtForMistag ) );


				if (tauScore < 0.7){

				if (bScore > 0.679) mistagWtNsub = mistagRateHistNSHi->GetBinContent( mistagRateHistNSHi->FindBin( jetPtForMistag ) );			
				else if (bScore > 0.244) mistagWtNsub = mistagRateHistNSMed->GetBinContent( mistagRateHistNSMed->FindBin( jetPtForMistag ) );			
				else mistagWtNsub = mistagRateHistNSLow->GetBinContent( mistagRateHistNSLow->FindBin( jetPtForMistag ) );			
				mistagWtNsubAll = mistagRateHistNSAll->GetBinContent( mistagRateHistNSAll->FindBin( jetPtForMistag ) );
				}
				else{
				mistagWtNsub = 0.0;
				mistagWtNsubAll = 0.0;
				}


				//mttMass = mttMassPred;
				//NNoutput = reader->EvaluateMVA("MLP");
			}
			
			
		
		}
		
		newTree->Fill();

	}


	
	
	
	



	newTree->Write();
	newFile->Write();
	newFile->Close();
	
}
	
