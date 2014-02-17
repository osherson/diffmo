#include <cstdlib>
#include <iostream>
#include <map>
#include <string>


#include "TMVA/Reader.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMVA/Tools.h"
#include "TRandom3.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLegend.h"

void MistagPlotter(TString outfileName) {



	TString filenames[8] = {"QCDSamples/Nov18_QCD_300_mistag.root",
				"QCDSamples/Nov18_QCD_470_mistag.root",
				"QCDSamples/Nov18_QCD_600_mistag.root",
				"QCDSamples/Nov18_QCD_800_mistag.root",
				"QCDSamples/Nov18_QCD_1000_mistag.root", 
				"QCDSamples/Nov18_QCD_1400_mistag.root",
				"Oct7_mistag_ttjets7.root", 
				"Oct7_mistag_ttjets10.root" };
	
	
	
	float jet1Pt, jet2Pt, jet1Mass, jet2Mass, jet1BTag, jet2BTag, jet1MinMass, jet2MinMass, jet1tau32, jet2tau32, x;
	int jet1TopTag, jet2TopTag, jet1Parton, jet2Parton;
	TRandom3 *random = new TRandom3();
	
	
	int nConds = 10;
	float bTagConds[10] = { -99.,
							0.679,
							0.679,
							0.244,
							0.244,
							0.679,
							0.679,
							0.244,
							0.244,
							-99.
							};
							
	float bProbeConds[10] = { -99.,
							  -99.,
							  0.679,
							  -99.,
							  0.244,
							  -99.,
							  0.679,
							  -99.,
							  0.244,
							  -99. };
							  
	float nSubTagConds[10] = { 99.,
							   99.,
							   99.,
							   99.,
							   99.,
							   0.7,
							   0.7,
							   0.7,
							   0.7,
							   99. };
							   
	float nSubProbeConds[10] = { 99.,
								 99.,
								 99.,
								 99.,
								 99.,
								 99.,
								 0.7,
								 99.,
								 0.7,
								 0.7 };
	
	string condLabels[10] = { "Inclusive",
								  "1btagT",
								  "2btagT",
								  "1btagL",
								  "2btagL",
								  "1btagTNsub",
								  "2btagTNsub",
								  "1btagLNsub",
								  "2btagLNsub",
								  "InclNsub" };
	
	TFile *outFile = new TFile(outfileName, "RECREATE");
	outFile->cd();
	
	for  (int cond = 0; cond < nConds; cond++){
	
		if (cond == 1) cond = 9;

		TH1D *topTagPt[8];
		topTagPt[0] = new TH1D("qcd1_tagPt", "qcd1_tagPt", 400, 0, 2000);
		topTagPt[1] = new TH1D("qcd2_tagPt", "qcd2_tagPt", 400, 0, 2000);
		topTagPt[2] = new TH1D("qcd3_tagPt", "qcd3_tagPt", 400, 0, 2000);
		topTagPt[3] = new TH1D("qcd4_tagPt", "qcd4_tagPt", 400, 0, 2000);
		topTagPt[4] = new TH1D("qcd5_tagPt", "qcd5_tagPt", 400, 0, 2000);
		topTagPt[5] = new TH1D("qcd6_tagPt", "qcd6_tagPt", 400, 0, 2000);
		topTagPt[6] = new TH1D("tt07_tagPt", "tt07_tagPt", 400, 0, 2000);
		topTagPt[7] = new TH1D("tt10_tagPt", "tt10_tagPt", 400, 0, 2000);
	
		TH1D *topProbePt[8];
		topProbePt[0] = new TH1D("qcd1_probePt", "qcd1_probePt", 400, 0, 2000);
		topProbePt[1] = new TH1D("qcd2_probePt", "qcd2_probePt", 400, 0, 2000);
		topProbePt[2] = new TH1D("qcd3_probePt", "qcd3_probePt", 400, 0, 2000);
		topProbePt[3] = new TH1D("qcd4_probePt", "qcd4_probePt", 400, 0, 2000);
		topProbePt[4] = new TH1D("qcd5_probePt", "qcd5_probePt", 400, 0, 2000);
		topProbePt[5] = new TH1D("qcd6_probePt", "qcd6_probePt", 400, 0, 2000);
		topProbePt[6] = new TH1D("tt07_probePt", "tt07_probePt", 400, 0, 2000);
		topProbePt[7] = new TH1D("tt10_probePt", "tt10_probePt", 400, 0, 2000);
	
	
	for (int i = 0; i < 8; i++){
		topTagPt[i]->Sumw2();
		topProbePt[i]->Sumw2();
	
		TChain *files = new TChain("mistagVars", "mistagVars");
		
		files->Add(filenames[i]);
		cout << files->GetEntries() << " entries found for " << filenames[i] << endl;	
		
		files->SetBranchAddress("jet1Pt", &jet1Pt);
		files->SetBranchAddress("jet2Pt", &jet2Pt);
		files->SetBranchAddress("jet1Mass", &jet1Mass);
		files->SetBranchAddress("jet2Mass", &jet2Mass);
		files->SetBranchAddress("jet1BTag", &jet1BTag);
		files->SetBranchAddress("jet2BTag", &jet2BTag);
		files->SetBranchAddress("jet1TopTag", &jet1TopTag);
		files->SetBranchAddress("jet2TopTag", &jet2TopTag);
		files->SetBranchAddress("jet1MinMass", &jet1MinMass);
		files->SetBranchAddress("jet2MinMass", &jet2MinMass);
		files->SetBranchAddress("jet1tau32", &jet1tau32);
		files->SetBranchAddress("jet2tau32", &jet2tau32);
		files->SetBranchAddress("jet1Parton", &jet1Parton);
		files->SetBranchAddress("jet2Parton", &jet2Parton);	
		
		
		
		for (int ev = 0; ev < files->GetEntries(); ev++){
		
			files->GetEntry(ev);
			
			x = random->Rndm();
		
	
			if (x < 0.5){
			
				if (jet1MinMass < 30.0 && jet1tau32 > 0.7) {
		//			if (jet2BTag < 0.244 ){
					topProbePt[i]->Fill(jet2Pt);
					if (jet2TopTag && jet2BTag > bTagConds[cond] && jet2tau32 < nSubTagConds[cond] && jet1BTag > bProbeConds[cond] && jet1tau32 < nSubProbeConds[cond]) topTagPt[i]->Fill(jet2Pt);
		//		}
				}
					
			}
			else {
			
				if (jet2MinMass < 30.0 && jet2tau32 > 0.7) {
		//			if (jet1BTag < 0.244){
					topProbePt[i]->Fill(jet1Pt);
					if (jet1TopTag && jet1BTag > bTagConds[cond] && jet1tau32 < nSubTagConds[cond]  && jet2BTag > bProbeConds[cond] && jet2tau32 < nSubProbeConds[cond]) topTagPt[i]->Fill(jet1Pt);
		//		}
				}
			}
			
		
		}
		
		
		
		
		files->Delete();
		
	}
	
	
	int num_bins = 29;
    double bins[30] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 650, 700, 750, 800, 900, 1000, 1100, 1250, 1500, 2000};
	
	double lumi = 19395.0;
	double subSF = 0.95;
	double topXS = 234.;
	
	topTagPt[6]->Scale(lumi*topXS*0.074 / 3082812. );
	topProbePt[6]->Scale(lumi*topXS*0.074 / 3082812. );
	
	topTagPt[7]->Scale(lumi*topXS*0.014 / 1249111.);
	topProbePt[7]->Scale(lumi*topXS*0.014 / 1249111.);
	
	
	topTagPt[0]->Scale(  lumi*1759.6 / 5927300.);
	topProbePt[0]->Scale(lumi*1759.6 / 5927300.);

	topTagPt[1]->Scale(  lumi*113.9 / 3994848. );
	topProbePt[1]->Scale(lumi*113.9 / 3994848. );

	topTagPt[2]->Scale(  lumi*27.0 / 3992760. );
	topProbePt[2]->Scale(lumi*27.0 / 3992760. );

	topTagPt[3]->Scale(  lumi*3.57 / 3978569. );
	topProbePt[3]->Scale(lumi*3.57 / 3987569. );

	topTagPt[4]->Scale(  lumi*0.738 / 1964086. );
	topProbePt[4]->Scale(lumi*0.738 / 1964086.);

	topTagPt[5]->Scale(  lumi*0.0335 / 2000062. );
	topProbePt[5]->Scale(lumi*0.0335 / 2000062. );


	topTagPt[0]->Add(topTagPt[1]);
	topTagPt[0]->Add(topTagPt[2]);
	topTagPt[0]->Add(topTagPt[3]);
	topTagPt[0]->Add(topTagPt[4]);
	topTagPt[0]->Add(topTagPt[5]);

	topProbePt[0]->Add(topProbePt[1]);
	topProbePt[0]->Add(topProbePt[2]);
	topProbePt[0]->Add(topProbePt[3]);
	topProbePt[0]->Add(topProbePt[4]);
	topProbePt[0]->Add(topProbePt[5]);



	TH1F *rebin_tagH_nosub = (TH1F *) topTagPt[0]->Rebin(num_bins, "rebin_tagH", bins);
	TH1F *rebin_probeH_nosub = (TH1F *) topProbePt[0]->Rebin(num_bins, "rebin_probeH", bins);
/*
	for (int n = 1; n < 8; n++){

		topTagPt[n]->Rebin(num_bins, Form("rebin_tagH_%d", n), bins);
		topProbePt[n]->Rebin(num_bins, Form("rebin_probeH_%d", n), bins);
	}
*/
	
	//topTagPt[0]->Add(topTagPt[6], -1);
	//topTagPt[0]->Add(topTagPt[7], -1);
	


//	topProbePt[0]->Add(topProbePt[6], -1);
//	topProbePt[0]->Add(topProbePt[7], -1);
	
	
	
	TH1F *rebin_tagH = (TH1F *) topTagPt[0]->Rebin(num_bins, "rebin_tagH", bins);
	TH1F *rebin_probeH = (TH1F *) topProbePt[0]->Rebin(num_bins, "rebin_probeH", bins);	
	
	rebin_tagH->Divide(rebin_probeH);
	rebin_tagH_nosub->Divide(rebin_probeH_nosub);
	
	TCanvas *c = new TCanvas("c", "", 860,600);
	c->SetLogx(1);
	//rebin_tagH->Draw();
	rebin_tagH_nosub->GetXaxis()->SetMoreLogLabels(1);
	rebin_tagH_nosub->GetXaxis()->SetNoExponent(1);
	rebin_tagH_nosub->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
	rebin_tagH_nosub->GetYaxis()->SetTitle("Top Mistag Rate");
	rebin_tagH_nosub->SetMarkerColor(kRed);
	rebin_tagH_nosub->SetMarkerSize(0.5);
	rebin_tagH_nosub->SetLineColor(kRed);
	rebin_tagH_nosub->Draw();
	
	TLatex *prelim = new TLatex();
    prelim->SetNDC();
    prelim->DrawLatex( 0.1, 0.915, "#scale[0.8]{CMS Preliminary, #sqrt{s} = 8 TeV, 19.6 fb^{-1}}" );
	
	TLegend *leg = new TLegend(0.7,0.7,0.89,0.89);
	leg->AddEntry(rebin_tagH, "t#bar{t} Corrected");
	leg->AddEntry(rebin_tagH_nosub, "Inclusive");
	leg->SetLineColor(0);
	leg->SetFillColor(0);
	leg->Draw("same");
		
	c->SaveAs(Form("mistagRate_%s.pdf", condLabels[cond].c_str()));
	
	rebin_tagH_nosub->SetName(Form("MISTAG_RATE_SUB_TTBAR_%s", condLabels[cond].c_str()));
	rebin_tagH_nosub->Write();
	
	
	
	}
	
	
	
	
	outFile->Write();
	outFile->Close();
	
}	
	
