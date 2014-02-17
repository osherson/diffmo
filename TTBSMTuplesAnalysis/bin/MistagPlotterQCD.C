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



	TString filenames[8] = { 
				"QCDSamples/Nov18_QCD_300_mistag.root",
				"QCDSamples/Nov18_QCD_470_mistag.root",
				"QCDSamples/Nov18_QCD_600_mistag.root",
				"QCDSamples/Nov18_QCD_800_mistag.root",
				"QCDSamples/Nov18_QCD_1000_mistag.root", 
				"QCDSamples/Nov18_QCD_1400_mistag.root",
				"Oct7_mistag_ttjets7.root", 
				"Oct7_mistag_ttjets10.root" };
	
	
	
	float jet1Pt, jet2Pt, jet1Mass, jet2Mass, jet1BTag, jet2BTag, jet1MinMass, jet2MinMass, jet1tau32, jet2tau32, x;
	int jet1TopTag, jet2TopTag;
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
							   0.7 };
							   
	float nSubProbeConds[10] = { 99.,
								 99.,
								 99.,
								 99.,
								 99.,
								 99.,
								 0.7,
								 99.,
								 0.7,
								 99. };
	
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
	
	int num_bins = 29;
    double bins[30] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 650, 700, 750, 800, 900, 1000, 1100, 1250, 1500, 2000};

	
	
	TH1F *topTag1D[4][9][6];
		TH1F *topProbe1D[4][9][6];

		for (int sample = 0; sample < 6; sample++){
			for (int csvBin = 0; csvBin < 4; csvBin++){
				for (int tauBin = 0; tauBin < 9; tauBin++){
	

					topTag1D[csvBin][tauBin][sample] = new TH1F(Form("%d_tagPt_%d_%d", sample, csvBin, tauBin), Form("%d_tagPt_%d_%d", sample, csvBin, tauBin), num_bins, bins);
					topProbe1D[csvBin][tauBin][sample] = new TH1F(Form("%d_probePt_%d_%d", sample, csvBin, tauBin), Form("%d_probePt_%d_%d", sample, csvBin, tauBin), num_bins, bins);
				
					topTag1D[csvBin][tauBin][sample]->Sumw2();
					topProbe1D[csvBin][tauBin][sample]->Sumw2();
					
				}
			}
		}
	
	
	
	
	
	
	
	for  (int cond = 0; cond < 1; cond++){
	
		TH3D *topTagPt[8];
		topTagPt[0] = new TH3D("qcd1_tagPt", "qcd1_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[1] = new TH3D("qcd2_tagPt", "qcd2_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[2] = new TH3D("qcd3_tagPt", "qcd3_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[3] = new TH3D("qcd4_tagPt", "qcd4_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[4] = new TH3D("qcd5_tagPt", "qcd5_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[5] = new TH3D("qcd6_tagPt", "qcd6_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[6] = new TH3D("tt07_tagPt", "tt07_tagPt", 29,0,29, 4,0,4, 9,0,9);
		topTagPt[7] = new TH3D("tt10_tagPt", "tt10_tagPt", 29,0,29, 4,0,4, 9,0,9);

		TH3D *topProbePt[8];
		topProbePt[0] = new TH3D("qcd1_probePt", "qcd1_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[1] = new TH3D("qcd2_probePt", "qcd2_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[2] = new TH3D("qcd3_probePt", "qcd3_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[3] = new TH3D("qcd4_probePt", "qcd4_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[4] = new TH3D("qcd5_probePt", "qcd5_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[5] = new TH3D("qcd6_probePt", "qcd6_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[6] = new TH3D("tt07_probePt", "tt07_probePt", 29,0,29, 4,0,4, 9,0,9);
		topProbePt[7] = new TH3D("tt10_probePt", "tt10_probePt", 29,0,29, 4,0,4, 9,0,9);
	
	
	for (int i = 0; i < 6; i++){
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
		
		
		
		
		for (int ev = 0; ev < files->GetEntries(); ev++){
		
			files->GetEntry(ev);
			
			x = random->Rndm();
			
			if (x < 0.5){
			
				if (jet1MinMass < 30.0) {
					
					topProbePt[i]->Fill(ptMap(jet2Pt), bMap(jet2BTag), tauMap(jet2tau32));
					topProbe1D[int(bMap(jet2BTag))][int(tauMap(jet2tau32))][i]->Fill(jet2Pt);
					if (jet2TopTag) {
						topTagPt[i]->Fill(ptMap(jet2Pt), bMap(jet2BTag), tauMap(jet2tau32));
						topTag1D[int(bMap(jet2BTag))][int(tauMap(jet2tau32))][i]->Fill(jet2Pt);
					}
				}
					
			}
			else {
			
				if (jet2MinMass < 30.0) {
					
					topProbePt[i]->Fill(ptMap(jet1Pt), bMap(jet1BTag), tauMap(jet1tau32));
					topProbe1D[int(bMap(jet1BTag))][int(tauMap(jet1tau32))][i]->Fill(jet1Pt);
					if (jet1TopTag) {
						topTagPt[i]->Fill(ptMap(jet1Pt), bMap(jet1BTag), tauMap(jet1tau32));
						topTag1D[int(bMap(jet1BTag))][int(tauMap(jet1tau32))][i]->Fill(jet1Pt);
					}
					
				}
			}
			
		
		}
		
		
		
		
		files->Delete();
		
	}
	
	
//	int num_bins = 29;
//    double bins[30] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 650, 700, 750, 800, 900, 1000, 1100, 1250, 1500, 2000};
	
	double lumi = 19395.0;
	double subSF = 0.95;
	double topXS = 234.;
	
	topTagPt[0]->Scale(lumi*1759.6 / 5927300.);
	topProbePt[0]->Scale(lumi*1759.6 / 5927300.);

	topTagPt[1]->Scale(lumi*113.9 / 3994848. );
	topProbePt[1]->Scale(lumi*113.9 / 3994848. );

	topTagPt[2]->Scale(lumi*27.0 / 3992760. );
	topProbePt[2]->Scale(lumi*27.0 / 3992760. );

	topTagPt[3]->Scale(lumi*3.57 / 3978569. );
	topProbePt[3]->Scale(lumi*3.57 / 3987569. );

	topTagPt[4]->Scale(lumi*0.738 / 1964086. );
	topProbePt[4]->Scale(lumi*0.738 / 1964086.);

	topTagPt[5]->Scale(lumi*0.0335 / 2000062. );
	topProbePt[5]->Scale(lumi*0.0335 / 2000062. );
	
	topTagPt[6]->Scale(lumi*topXS*0.074 / 3082812. );
	topProbePt[6]->Scale(lumi*topXS*0.074 / 3082812. );
	
	topTagPt[7]->Scale(lumi*topXS*0.014 / 1249111.);
	topProbePt[7]->Scale(lumi*topXS*0.014 / 1249111.);
	
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
	
	
	
	for (int csvBin = 0; csvBin < 4; csvBin++){
			for (int tauBin = 0; tauBin < 9; tauBin++){
			
			
	
		topTag1D[csvBin][tauBin][0]->Scale(lumi*1759.6 / 5927300.);
		topProbe1D[csvBin][tauBin][0]->Scale(lumi*1759.6 / 5927300.);

		topTag1D[csvBin][tauBin][1]->Scale(lumi*113.9 / 3994848. );
		topProbe1D[csvBin][tauBin][1]->Scale(lumi*113.9 / 3994848. );

		topTag1D[csvBin][tauBin][2]->Scale(lumi*27.0 / 3992760. );
		topProbe1D[csvBin][tauBin][2]->Scale(lumi*27.0 / 3992760. );

		topTag1D[csvBin][tauBin][3]->Scale(lumi*3.57 / 3978569. );
		topProbe1D[csvBin][tauBin][3]->Scale(lumi*3.57 / 3987569. );

		topTag1D[csvBin][tauBin][4]->Scale(lumi*0.738 / 1964086. );
		topProbe1D[csvBin][tauBin][4]->Scale(lumi*0.738 / 1964086.);

		topTag1D[csvBin][tauBin][5]->Scale(lumi*0.0335 / 2000062. );
		topProbe1D[csvBin][tauBin][5]->Scale(lumi*0.0335 / 2000062. );
		
		
		topTag1D[csvBin][tauBin][0]->Add(topTag1D[csvBin][tauBin][1]);
		topTag1D[csvBin][tauBin][0]->Add(topTag1D[csvBin][tauBin][2]);
		topTag1D[csvBin][tauBin][0]->Add(topTag1D[csvBin][tauBin][3]);
		topTag1D[csvBin][tauBin][0]->Add(topTag1D[csvBin][tauBin][4]);
		topTag1D[csvBin][tauBin][0]->Add(topTag1D[csvBin][tauBin][5]);
		
		topProbe1D[csvBin][tauBin][0]->Add(topProbe1D[csvBin][tauBin][1]);
		topProbe1D[csvBin][tauBin][0]->Add(topProbe1D[csvBin][tauBin][2]);
		topProbe1D[csvBin][tauBin][0]->Add(topProbe1D[csvBin][tauBin][3]);
		topProbe1D[csvBin][tauBin][0]->Add(topProbe1D[csvBin][tauBin][4]);
		topProbe1D[csvBin][tauBin][0]->Add(topProbe1D[csvBin][tauBin][5]);
		
		topTag1D[csvBin][tauBin][0]->Divide(topProbe1D[csvBin][tauBin][0]);		
	
		}	
	}
	


	rebin_tagH = (TH3F *) topTagPt[0]->Clone("rebin_tagH");
	rebin_probeH = (TH3F *) topProbePt[0]->Clone("rebin_probeH");
	
	rebin_tagH->Divide(rebin_probeH);
	
	
//	TH1F *rebin_tagH = (TH1F *) topTagPt[0]->Rebin(num_bins, "rebin_tagH", bins);
//	TH1F *rebin_probeH = (TH1F *) topProbePt[0]->Rebin(num_bins, "rebin_probeH", bins);	
	
//	rebin_tagH->Divide(rebin_probeH);
//	rebin_tagH_nosub->Divide(rebin_probeH_nosub);
	
/*	TCanvas *c = new TCanvas("c", "", 860,600);
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
		
//	c->SaveAs(Form("mistagRate_%s.pdf", condLabels[cond].c_str()));
	
//	rebin_tagH_nosub->SetName(Form("MISTAG_RATE_SUB_TTBAR_%s", condLabels[cond].c_str()));
//	rebin_tagH_nosub->Write();
*/	
	

	rebin_tagH->SetName(Form("MISTAG_RATE_SUB_TTBAR_%s", condLabels[cond].c_str()));
	rebin_tagH->Write();

	
	}
	
	
	
	
	outFile->Write();
	outFile->Close();
	
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
	else out = 3.1;

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
	
	
