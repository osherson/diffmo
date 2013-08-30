{
#include <math.h>
#include <TMath.h>
#include <TMatrixT.h>
#include <TMatrixD.h>
#include <TVectorT.h>
#include <TVectorD.h>
#include <TLatex.h>
#include <iostream>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <sstream>
#include <TFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TPostScript.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TColor.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm> // sort, max_element, random_shuffle, keep_if, lower_bound 
#include <functional> // greater, bind2nd
	
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
	gROOT->SetStyle("Plain");
	//-----------------------------------------
	//    Canvas
	//-----------------------------------------
	//gStyle->SetCanvasBorderMode(0);
	//gStyle->SetCanvasColor(10);
	//-----------------------------------------
	//   StatsBox
	//-----------------------------------------
	gStyle->SetOptStat(0);
	//gStyle->SetOptStat("eou");
	//gStyle->SetOptStat(111111);
	//gStyle->SetStatColor(10);
	//gStyle->SetStatBorderSize(5);
	//gStyle->SetStatFontSize(0.08);
	//-----------------------------------------
	//    Histogram Attributes
	//-----------------------------------------
	gStyle->SetHistLineWidth(2);
	//gStyle->SetPalette(52,0); 
	gStyle->SetHatchesSpacing(0.5);
	gStyle->SetHatchesLineWidth(1.7);
	//-----------------------------------------
	//    Pad .  Use Canvas->SetLeftMargin(0.08) to do individually 
	//-----------------------------------------
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadBottomMargin(0.13);
	gStyle->SetPadLeftMargin(0.16);
	gStyle->SetPadRightMargin(0.04); 
	//gStyle->SetPadBorderMode(0);
	//-----------------------------------------
	//    Title, axis labels, and axis numbers
	//-----------------------------------------
	//gStyle->SetTitleColor();
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleXOffset(1.2);
	gStyle->SetTitleYOffset(1.7);  //gStyle->SetTitleOffset(2.9,"Y"); also works
	//gStyle->SetTitleXSize(0.05);
	//gStyle->SetTitleYSize(0.05);
	//gStyle->SetTitleSize(30);
	gStyle->SetTitleX(0.02); 
	gStyle->SetTitleY(0.99);
	gStyle->SetTitleW(0.5);
	//gStyle->SetTitleFontSize(0.06);	// Change size of histogram title
	//gStyle->SetTitleSize(0.05,"XY");	// Change the size of the axis labels    
	//gStyle->SetTitleOffset(10,"Y");
	//gStyle->SetTitleOffset(2,"X");
	//gStyle->SetLabelSize(0.08,"XY");	// Change size of axis numbers
	gStyle->SetLabelOffset(0.012,"X");
	gStyle->SetPadTickX(1);  
	gStyle->SetPadTickY(1);
	gStyle->SetTickLength(0.03, "XYZ");
	//-----------------------------------------
	//--- Force Style
	//-----------------------------------------
	gROOT->UseCurrentStyle();
	gROOT->ForceStyle();// forces the style chosen above to be used, not the style the rootfile was made with
	gROOT->Reset();
    /////////////////////////////////////////////////////////////////////////////////////////
		TCanvas *c1000 = new TCanvas("c1000","",10,10,700,550);

		TFile *ROOT 			= new TFile("TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_range1_range2_v4_fixedRand_mistag1.root");
    TH1D * topTagPt			=  ROOT	-> Get("topTagPt");
	TH1D * topProbePt		=  ROOT -> Get("topProbePt");
	TH1D * massCutTagPt		=  ROOT -> Get("testTagPt");
	TH1D * massCutProbePt	=  ROOT -> Get("testProbePt");	
	
 	Int_t num_bins=26;
    Double_t bins[num_bins+1]= {350,355,360,365,370,375,380,385,390,395,400,410,420,430,440,450,460,480,500,525,550,600,650,700,800,1000,1200}; 
	TH1D *topTagPt_rebin 		= topTagPt			->Rebin(num_bins,"topTagPt_rebin",bins); 
	TH1D *topProbePt_rebin 		= topProbePt		->Rebin(num_bins,"topProbePt_rebin",bins); 
	TH1D *massCutTagPt_rebin 	= massCutTagPt		->Rebin(num_bins,"massCutTagPt_rebin",bins); 
	TH1D *massCutProbePt_rebin 	= massCutProbePt	->Rebin(num_bins,"massCutProbePt_rebin",bins); 
	
 	Int_t num_bins2=9;
    Double_t bins2[num_bins2+1]= {350,375,400,450,500,600,700,800,1000,1200}; 
	TH1D *topTagPt_rebin2 		= topTagPt			->Rebin(num_bins2,"topTagPt_rebin2",bins2); 
	TH1D *topProbePt_rebin2 	= topProbePt		->Rebin(num_bins2,"topProbePt_rebin2",bins2); 
	TH1D *massCutTagPt_rebin2 	= massCutTagPt		->Rebin(num_bins2,"massCutTagPt_rebin2",bins2); 
	TH1D *massCutProbePt_rebin2 = massCutProbePt	->Rebin(num_bins2,"massCutProbePt_rebin2",bins2); 

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mistag plots
	

	TH1D *MISTAG_RATE = topProbePt_rebin->Clone();
	MISTAG_RATE->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
	MISTAG_RATE->Sumw2();
	MISTAG_RATE->Divide(topTagPt_rebin,topProbePt_rebin,1.,1.,"B");
	MISTAG_RATE->SetMarkerStyle(21);
	MISTAG_RATE->SetMarkerColor(2);
	MISTAG_RATE->SetLineColor(2);
	MISTAG_RATE->Draw();
	MISTAG_RATE->SetName("TYPE11_MISTAG");
	c1000->SaveAs("mistag_plots/MISTAG_RATE.png");

	TH1D *MISTAG_RATE_MASSCUT = massCutProbePt_rebin->Clone();
	MISTAG_RATE_MASSCUT->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
	MISTAG_RATE_MASSCUT->Sumw2();
	MISTAG_RATE_MASSCUT->Divide(massCutTagPt_rebin,massCutProbePt_rebin,1.,1.,"B");
	MISTAG_RATE_MASSCUT->SetMarkerStyle(22);
	MISTAG_RATE_MASSCUT->SetMarkerColor(4);
	MISTAG_RATE_MASSCUT->SetLineColor(4);
	MISTAG_RATE_MASSCUT->Draw("");
	MISTAG_RATE_MASSCUT->SetName("TYPE11_MISTAG_MASSCUT");
	c1000->SaveAs("mistag_plots/MISTAG_RATE_MASSCUT.png");
	
	MISTAG_RATE_MASSCUT->Draw("");
	MISTAG_RATE->Draw("same");

    double legXmin=0.3;
    double legYmin=0.18;
    double legXmax=0.8;//0.58
    double legYmax=0.35;

    leg = new TLegend(legXmin,legYmin,legXmax,legYmax);
    leg->SetBorderSize(1);
    leg->SetFillColor(0);
    leg->AddEntry(MISTAG_RATE,"Anti-tag and probe","LP");
    leg->AddEntry(MISTAG_RATE_MASSCUT,"Pass mass cut anti-tag and probe","LP");
    leg->Draw("same");
	c1000->SaveAs("mistag_plots/Compare_NminusOne_MISTAG_RATE.png");
	//------------------------------------------------------------------------------------------------------------//
 	
	TH1D *MISTAG_RATE2 = topProbePt_rebin2->Clone();
    MISTAG_RATE2->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
    MISTAG_RATE2->Sumw2();
    MISTAG_RATE2->Divide(topTagPt_rebin2,topProbePt_rebin2,1.,1.,"B");
    MISTAG_RATE2->SetMarkerStyle(21);
    MISTAG_RATE2->SetMarkerColor(2);
    MISTAG_RATE2->SetLineColor(2);
    MISTAG_RATE2->Draw();
    MISTAG_RATE2->SetName("TYPE11_MISTAG_LARGEBINS");
    c1000->SaveAs("mistag_plots/MISTAG_RATE_LARGEBINS.png");

    TH1D *MISTAG_RATE_MASSCUT2 = massCutProbePt_rebin2->Clone();
    MISTAG_RATE_MASSCUT2->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
    MISTAG_RATE_MASSCUT2->Sumw2();
    MISTAG_RATE_MASSCUT2->Divide(massCutTagPt_rebin2,massCutProbePt_rebin2,1.,1.,"B");
    MISTAG_RATE_MASSCUT2->SetMarkerStyle(22);
    MISTAG_RATE_MASSCUT2->SetMarkerColor(4);
    MISTAG_RATE_MASSCUT2->SetLineColor(4);
    MISTAG_RATE_MASSCUT2->Draw("");
    MISTAG_RATE_MASSCUT2->SetName("TYPE11_MISTAG_MASSCUT_LARGEBINS");
    c1000->SaveAs("mistag_plots/MISTAG_RATE_MASSCUT_LARGEBINS.png");

    MISTAG_RATE_MASSCUT2->Draw("");
    MISTAG_RATE2->Draw("same");
    leg->Draw("same");
	c1000->SaveAs("mistag_plots/Compare_NminusOne_MISTAG_RATE_LARGEBINS.png");

	
	//------------------------------------------------------------------------------------------------------------//


	TFile *Out;
	Out = new TFile("MISTAG_RATE_TYPE1.root","RECREATE");
	Out->cd();
	MISTAG_RATE->Write();
	MISTAG_RATE_MASSCUT->Write();
	MISTAG_RATE2->Write();
	MISTAG_RATE_MASSCUT2->Write();
	Out->ls();      
	Out->Write();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // test plots
    
    TCanvas *c1000= new TCanvas("c1000","",200,10,700,650);	

	topProbePt_rebin		->SetLineColor(1);
	topTagPt_rebin			->SetLineColor(2);
	massCutProbePt_rebin	->SetLineColor(3);
	massCutTagPt_rebin		->SetLineColor(4);

	topProbePt_rebin			->Draw();
	topTagPt_rebin				->Draw("same");
	massCutProbePt_rebin	->Draw("same");
	massCutTagPt_rebin	->Draw("same");
	
	c1000->SetLogy();
	c1000->SaveAs("REPLACEME_DIRECTORY/PT_rebin.png");
	c1000->SetLogy(0);
    
    topProbePt				->Rebin(2);
	topTagPt				->Rebin(2);
	massCutProbePt	->Rebin(2);
	massCutTagPt		->Rebin(2);
	
	topProbePt	->SetLineColor(1);
	topTagPt	->SetLineColor(2);

	
	topProbePt	->Draw();
	topTagPt->Draw("same");
	c1000->SetLogy();
	c1000->SaveAs("mistag_plots/PT.png");
    c1000->SetLogy(0);

	
	massCutProbePt->SetLineColor(3);
	massCutProbePt->Draw("same");
	massCutTagPt->SetLineColor(4);
	massCutTagPt->Draw("same");
	
	double legXmin=0.62;
    double legYmin=0.76;
    double legXmax=0.97;//0.58
    double legYmax=0.88;

    leg = new TLegend(legXmin,legYmin,legXmax,legYmax);
    leg->SetBorderSize(1);
    leg->SetFillColor(0);
    leg->AddEntry(topProbePt,"topProbePt","LP");
    leg->AddEntry(topTagPt,"topTagPt","LP");
    leg->AddEntry(massCutProbePt,"massCutProbePt","LP");
    leg->AddEntry(massCutTagPt,"massCutTagPt","LP");
    leg->Draw("same");
	
	
	c1000->SetLogy();
	c1000->SaveAs("mistag_plots/PTmore.png");
	c1000->SetLogy(0);


    


	
}
	
