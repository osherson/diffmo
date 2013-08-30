// .L make_table.C
// run11(true, true) for type 1+1 with table and histograms
// run12(true, true) for type 1+2 with table and histograms
// run12(true, false) for type 1+2 with table but no histograms
// run12(false, true) for type 1+2 histograms only

#include <iomanip> 
#include <iostream>
#include <string>

using namespace RooFit;

double tagging_eff_scale_factor = 0.96; 
double tagging_eff_scale_factor_error = 0.08;

// Update to 1070 pb. scale factor 0.96 +- 0.08.
// In the PAS we used 0.93 +- 0.13// based on W tagging but also used for top tagging (0.92 +- 0.06) * (1.01 +- 0.11) = 0.93 +- 0.12

void run11(bool makeTable, bool makeHists)
{	    
	int type =11;

	TFile *ROOT_DATA 		= new TFile("TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_range1_range2_v4_Subtract_type11_.root");
	TFile *ROOT_TT_TuneZ2 	= new TFile("TTHadronicAnalyzerCombined_TTJets_TuneZ2_trig2_type11_.root");		
	
	TH1D * mttMass_Data							=  ROOT_DATA				-> Get("mttMass");
	TH1D * mttMass_TT_TuneZ2					=  ROOT_TT_TuneZ2			-> Get("mttMassTriggerWeighted");
	TH1D * mttMass2_TT_TuneZ2					=  ROOT_TT_TuneZ2			-> Get("mttMass");
	
	TH1D * mttMass_Predicted					    =  ROOT_DATA			-> Get("mttPredDist_pred");
	TH1D * mttMass_Predicted_MassCutMistag		    =  ROOT_DATA			-> Get("mttPredDistMassCut_pred");
	TH1D * mttMass_Predicted_ModMassFlat		    =  ROOT_DATA			-> Get("mttPredDistModMassFlat_pred");
	TH1D * mttMass_Predicted_Mod3MassFlat		    =  ROOT_DATA			-> Get("mttPredDistMod3MassFlat_pred");
	TH1D * mttMass_Predicted_Mod3MassFlatSubtract	=  ROOT_DATA			-> Get("mttPredDistMod3MassFlatSubtract_pred");
		
	double luminosity = 888.4;  
	double DataSetNevents_TT_TuneZ2  = 3701947;
	double sigma_TT_TuneZ2   =  157;//  94;
	double scale_TT_TuneZ2 = sigma_TT_TuneZ2 * luminosity / DataSetNevents_TT_TuneZ2;
    double PatTupleNevents_TT_TuneZ2 = 3683795;
   // double tagging_eff_scale_factor = 0.93; 			// 0.93 +- 0.13// based on W tagging but also used for top tagging (0.92 +- 0.06) * (1.01 +- 0.11) = 0.93 +- 0.12

	mttMass_TT_TuneZ2	->Sumw2();
	mttMass_TT_TuneZ2	->Scale(scale_TT_TuneZ2);
	//mttMass_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);
	mttMass2_TT_TuneZ2	->Sumw2();
	mttMass2_TT_TuneZ2	->Scale(scale_TT_TuneZ2);
	//mttMass2_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);
	
	cout<<"mttMass_TT_TuneZ2->Integral() "<<mttMass_TT_TuneZ2->Integral() <<endl;
	cout<<"mttMass2_TT_TuneZ2->Integral() "<<mttMass2_TT_TuneZ2->Integral() <<endl;
	//---------------------------------------------------------------------
	// Make efficiency table and morphed histograms
	
	//Explanation: MakeTable( Data, ttbar, primary QCD estimation, cross-check QCD estimation, type (11 or 12) )
	if (makeTable) MakeTable( mttMass_Data, mttMass_TT_TuneZ2, mttMass_Predicted_Mod3MassFlatSubtract, mttMass_Predicted, mttMass2_TT_TuneZ2, type);
	
	if (makeHists)
	{
	
	    //double tagging_eff_scale_factor = 0.93; 			

	    mttMass_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);
	    mttMass2_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);

	
	MakeHistogram3(
		mttMass_Data, 							// data histogram
		mttMass_TT_TuneZ2, 						// ttbar histogram
		mttMass2_TT_TuneZ2, 						// backup ttbar histogram
		mttMass_Predicted_Mod3MassFlatSubtract, 		// qcd histogram
		mttMass_Predicted,						// alternate qcd estimate histogram - used for systematics
		";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV", 	// histogram title
		"none",//"JetPD - 270_v1, 300, 370 - pT>350",					// legend_header
		"Data", 														// legend label 1
		"QCD background estimate", 	// legend label 2  "#splitline{QCD estimation - }{Mass Modified Flat(140,250)}"
		"t#bar{t} Madgraph + Pythia Tune Z2    ", 							// legend label 3
		0.4, 0.61, 0.82,0.81, 											// legend coordinates
		0.8, 															// Y axis label offset
		"ttMassType11_mttPredDist_Mod3MassFlatSubtract_cmsStyle", 			// savename
		20, 															// rebin
		500, 															// X axis lowerbound
		5000,															// Y axis lowerbound
		11 );       													// type 11 or type 22
																	
	
	}
}


void run12(bool makeTable, bool makeHists){
	//static const int n_signal_bins = 24;
	/////////////////////////////////////////////////////////////
	
	int type =12;
	cout<<"type "<<type<<endl;

	TFile *ROOT_DATA 			= new TFile("TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_range1_range2_range4_type12_.root" );
	TFile *ROOT_TT_TuneZ2 		= new TFile("TTHadronicAnalyzerCombined_TTJets_TuneZ2_trig2_type12_.root");		
	
	TH1D * mttMass_Data							=  ROOT_DATA			-> Get("mttMassVeto11");
	TH1D * mttMass_TT_TuneZ2					=  ROOT_TT_TuneZ2		-> Get("mttMassTriggerWeightedVeto11");
	TH1D * mttMass2_TT_TuneZ2					=  ROOT_TT_TuneZ2		-> Get("mttMassVeto11");
	
	TH1D * mttPredDist					        =  ROOT_DATA			-> Get("mttPredDist_pred");
	TH1D * mttPredDistMod3MassFlat		        =  ROOT_DATA			-> Get("mttPredDistMod3MassFlat_pred");
	TH1D * mttPredDistMod3MassFlatSubtract		=  ROOT_DATA			-> Get("mttPredDistMod3MassFlatSubtract_pred");
	
	cout<<" mttMass_Data "<<mttMass_Data->Integral()<<endl;
	cout<<" mttPredDistMod3MassFlatSubtract "<<mttPredDistMod3MassFlatSubtract->Integral()<<endl;
	double luminosity = 1047;  
	double DataSetNevents_TT_TuneZ2  = 3701947;
	double sigma_TT_TuneZ2   =  157;//  94;
	double scale_TT_TuneZ2 = sigma_TT_TuneZ2 * luminosity / DataSetNevents_TT_TuneZ2;
    double PatTupleNevents_TT_TuneZ2 = 3683795;
    //double tagging_eff_scale_factor = 0.93; 	// 0.93 +- 0.13	for each jet	//  based on W tagging but also used for top tagging (0.92 +- 0.06) * (1.01 +- 0.11) = 0.93 +- 0.12

	mttMass_TT_TuneZ2	->Sumw2();
	mttMass_TT_TuneZ2	->Scale(scale_TT_TuneZ2);
	//mttMass_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);
	
	mttMass2_TT_TuneZ2	->Sumw2();
	mttMass2_TT_TuneZ2	->Scale(scale_TT_TuneZ2);
	//---------------------------------------------------------------------
	// Make efficiency table and morphed histograms
	
	//Explanation: MakeTable( Data, ttbar, primary QCD estimation, cross-check QCD estimation, type (11 or 12) )
	if (makeTable) MakeTable( mttMass_Data, mttMass_TT_TuneZ2, mttPredDistMod3MassFlatSubtract, mttPredDist, mttMass2_TT_TuneZ2,  type);
	
	if (makeHists)
	{
	/*
	 CompareHists(
	mttPredDistMod3MassFlat,
	mttPredDistMod3MassFlatSubtract,
	";Type 1+2 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV",
	"none",
	"Data Mistag", 
	"Mistag w/ subtracted ttbar MC", 
	0.48, 0.72, 0.92,0.91, 
	1.4, 															// Y axis label offset
	"ttMassType12_SubtractTTbarMistag", 						// savename
	20, 															// rebin
	500, 															// X axis lowerbound
	5000,                                                   // Y axis lowerbound
	"PE",
	"PE",
	"",
	"",
	2,
	3
	);			
	*/
	
	  //double tagging_eff_scale_factor = 0.93; 			// 0.93 +- 0.13// based on W tagging but also used for top tagging (0.92 +- 0.06) * (1.01 +- 0.11) = 0.93 +- 0.12

	    mttMass_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);
	    mttMass2_TT_TuneZ2	->Scale(tagging_eff_scale_factor*tagging_eff_scale_factor);

	
	MakeHistogram3(
		mttMass_Data, 							// data histogram
		mttMass_TT_TuneZ2, 						// ttbar histogram
		mttMass2_TT_TuneZ2, 						// ttbar histogram
		mttPredDistMod3MassFlatSubtract, 		// qcd histogram
		mttPredDist,						// alternate qcd estimate histogram - used for systematics
		";Type 1+2 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV", 	// histogram title
		"none",//"JetPD - 270_v1, 300, 370 - pT>350",					// legend_header
		"Data", 														// legend label 1
		"QCD background estimate", 	// legend label 2 //"#splitline{QCD estimation - }{Mass Modified Flat(140,250)}", 
		"t#bar{t} Madgraph + Pythia Tune Z2 ", 							// legend label 3
		0.4, 0.61, 0.82,0.81, 											// legend coordinates
		0.8, 															// Y axis label offset
		"ttMassType12_mttPredDist_Mod3MassFlatTriggerWeightVeto11Subtract_cmsStyle", 						// savename
		20, 															// rebin
		500, 															// X axis lowerbound
		5000,															// Y axis lowerbound
		12 );       													// type 11 or type 22

	}
}

void MakeTable( TH1D * mttMass_Data, TH1D * mttMass_TT, TH1D * mttMass_Pred1, TH1D* mttMass_Pred2, TH1D* mttMass2_TT, int type )
{
	cout<<"MakeTable - type = "<<type<<endl;
	static const int n_signal_bins = 24;

	TCanvas *c2000= new TCanvas("c2000","",200,10,700,650);	

	string string_type = "";
	if (type==11) string_type = "type11_.root";
	if (type==12) string_type = "type12_.root";
	cout<<" string_type = "<<string_type<<endl;
	string file_string1 = "TTHadronicAnalyzerCombined_Zprime_M750GeV_W7500MeV_ttbsm_v8_ttbsmTuples_v4_noPDF_trig2_"+string_type;
	string file_string2 = "TTHadronicAnalyzerCombined_Zprime_M1000GeV_W10GeV_ttbsm_v8_ttbsmTuples_v4_noPDF_trig2_"+string_type;
	string file_string3 = "TTHadronicAnalyzerCombined_Zprime_M1500GeV_W15GeV_ttbsm_v8_ttbsmTuples_v4_noPDF_trig2_"+string_type;
	string file_string4 = "TTHadronicAnalyzerCombined_Zprime_M2000GeV_W20GeV_ttbsm_v8_ttbsmTuples_v4_noPDF_trig2_"+string_type;
	string file_string5 = "TTHadronicAnalyzerCombined_Zprime_M3000GeV_W30GeV_ttbsm_v8_ttbsmTuples_v4_noPDF_trig2_"+string_type;

	TFile *ROOT_Zprime_M750GeV_W7500MeV = new TFile( file_string1.c_str() );			
	TFile *ROOT_Zprime_M1000GeV_W10GeV 	= new TFile( file_string2.c_str() );			
	TFile *ROOT_Zprime_M1500GeV_W15GeV 	= new TFile( file_string3.c_str() );			
	TFile *ROOT_Zprime_M2000GeV_W20GeV 	= new TFile( file_string4.c_str() );			
	TFile *ROOT_Zprime_M3000GeV_W30GeV 	= new TFile( file_string5.c_str() );		
		
	string hist_string="";
	string hist_string2="";
	if (type==11) hist_string = "mttMassTriggerWeighted";
	if (type==11) hist_string2 = "mttMass";  
	if (type==12) hist_string = "mttMassTriggerWeightedVeto11";
	if (type==12) hist_string2 = "mttMassVeto11";
	
	TH1D * mttMass_Zprime_M750GeV_W7500MeV		=  ROOT_Zprime_M750GeV_W7500MeV -> Get(hist_string.c_str());
	TH1D * mttMass_Zprime_M1000GeV_W10GeV		=  ROOT_Zprime_M1000GeV_W10GeV 	-> Get(hist_string.c_str());
	TH1D * mttMass_Zprime_M1500GeV_W15GeV		=  ROOT_Zprime_M1500GeV_W15GeV 	-> Get(hist_string.c_str());
	TH1D * mttMass_Zprime_M2000GeV_W20GeV		=  ROOT_Zprime_M2000GeV_W20GeV 	-> Get(hist_string.c_str());
	TH1D * mttMass_Zprime_M3000GeV_W30GeV		=  ROOT_Zprime_M3000GeV_W30GeV 	-> Get(hist_string.c_str());
	
	TH1D * mttMass2_Zprime_M750GeV_W7500MeV		=  ROOT_Zprime_M750GeV_W7500MeV -> Get(hist_string2.c_str());
	TH1D * mttMass2_Zprime_M1000GeV_W10GeV		=  ROOT_Zprime_M1000GeV_W10GeV 	-> Get(hist_string2.c_str());
	TH1D * mttMass2_Zprime_M1500GeV_W15GeV		=  ROOT_Zprime_M1500GeV_W15GeV 	-> Get(hist_string2.c_str());
	TH1D * mttMass2_Zprime_M2000GeV_W20GeV		=  ROOT_Zprime_M2000GeV_W20GeV 	-> Get(hist_string2.c_str());
	TH1D * mttMass2_Zprime_M3000GeV_W30GeV		=  ROOT_Zprime_M3000GeV_W30GeV 	-> Get(hist_string2.c_str());
	

	TH1D * mttMass_Zprime_M750GeV_W7500MeV_Rebin 	=  mttMass_Zprime_M750GeV_W7500MeV->Clone();
	TH1D * mttMass_Zprime_M1000GeV_W10GeV_Rebin		=  mttMass_Zprime_M1000GeV_W10GeV->Clone();
	TH1D * mttMass_Zprime_M1500GeV_W15GeV_Rebin		=  mttMass_Zprime_M1500GeV_W15GeV->Clone();
	TH1D * mttMass_Zprime_M2000GeV_W20GeV_Rebin		=  mttMass_Zprime_M2000GeV_W20GeV->Clone();
	TH1D * mttMass_Zprime_M3000GeV_W30GeV_Rebin		=  mttMass_Zprime_M3000GeV_W30GeV->Clone();
	
	TH1D * mttMass_Zprime_M750GeV_W7500MeV_Smooth		=  mttMass_Zprime_M750GeV_W7500MeV->Clone();
	TH1D * mttMass_Zprime_M1000GeV_W10GeV_Smooth		=  mttMass_Zprime_M1000GeV_W10GeV->Clone();
	TH1D * mttMass_Zprime_M1500GeV_W15GeV_Smooth		=  mttMass_Zprime_M1500GeV_W15GeV->Clone();
	TH1D * mttMass_Zprime_M2000GeV_W20GeV_Smooth		=  mttMass_Zprime_M2000GeV_W20GeV->Clone();
	TH1D * mttMass_Zprime_M3000GeV_W30GeV_Smooth		=  mttMass_Zprime_M3000GeV_W30GeV->Clone();

	TH1D * blank = new TH1D ("blank","blank",10,0,10);

	TH1D * rebin_histos[5] = { mttMass_Zprime_M750GeV_W7500MeV_Rebin, mttMass_Zprime_M1000GeV_W10GeV_Rebin, mttMass_Zprime_M1500GeV_W15GeV_Rebin,  mttMass_Zprime_M2000GeV_W20GeV_Rebin,  mttMass_Zprime_M3000GeV_W30GeV_Rebin};
	TH1D * smooth_histos[5] = { mttMass_Zprime_M750GeV_W7500MeV_Smooth, mttMass_Zprime_M1000GeV_W10GeV_Smooth, mttMass_Zprime_M1500GeV_W15GeV_Smooth,  mttMass_Zprime_M2000GeV_W20GeV_Smooth,  mttMass_Zprime_M3000GeV_W30GeV_Smooth};
	int colors[5] = { 1, 5, 3,  6,  7};

	int rebin=1;
	int smooth =1;
	for (int i=0; i<5; i++)
	{		
		rebin_histos[i]->SetLineColor(1);
		rebin_histos[i]->SetLineWidth(4);
		smooth_histos[i]->SetLineColor(colors[i]);
		
		
		if (type==11) smooth =1000;
		if (type==12) smooth =10000;
		cout<<"hist "<<i<<" smooth "<<smooth<<endl;
		smooth_histos[i]->Smooth(smooth);	
		
		rebin_histos[i]->Rebin(rebin);
		smooth_histos[i]->Rebin(rebin);
	}
	
	TH1D * signal_histos[n_signal_bins] = { smooth_histos[0], blank, blank, smooth_histos[1], blank, blank, blank, blank, smooth_histos[2], blank, blank, blank, blank, smooth_histos[3], blank, blank, blank, blank, blank, blank, blank, blank, blank,  smooth_histos[4]};
	
	TH1D * signal_histos_check[n_signal_bins] = { rebin_histos[0], blank, blank, rebin_histos[1], blank, blank, blank, blank, rebin_histos[2], blank, blank, blank, blank, rebin_histos[3], blank, blank, blank, blank, blank, blank, blank, blank, blank,  rebin_histos[4]};
	
	TH1D * signal_histos2_for_trigger_error[n_signal_bins] = { mttMass2_Zprime_M750GeV_W7500MeV, blank, blank, mttMass2_Zprime_M1000GeV_W10GeV, blank, blank, blank, blank, mttMass2_Zprime_M1500GeV_W15GeV, blank, blank, blank, blank, mttMass2_Zprime_M2000GeV_W20GeV, blank, blank, blank, blank, blank, blank, blank, blank, blank, mttMass2_Zprime_M3000GeV_W30GeV};

	Double_t dataset_nevents[n_signal_bins] = {206525, 0, 0, 209447, 0, 0, 0, 0, 170783, 0, 0, 0, 0, 182364, 0, 0, 0, 0, 0, 0, 0, 0, 0, 198763 };
	Double_t m_zp_mc[n_signal_bins] = {   750,    0,    0, 1000,    0,    0,    0,    0, 1500,    0,    0,    0,    0, 2000,    0,    0,    0,    0,    0,    0,       0, 0, 0, 3000 };
	Double_t m_zp_new[n_signal_bins] = {  750,  800,  900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000 };
 	Double_t windowLB[n_signal_bins] = {  600,  600,  700,  900,  900,  900, 1000, 1100, 1200, 1200, 1200, 1200, 1200, 1300, 1300, 1400, 1500, 1700, 1700, 1700, 1700, 1800, 1900, 2000 };
 	Double_t windowUB[n_signal_bins] = { 1100, 1100, 1100, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1900, 2000, 2200, 2400, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300 };
 	Double_t JEC_up[n_signal_bins] = { 1, 1, 1, 1.10, 1, 1, 1, 1, 0.97, 1, 1, 1, 1, 0.98, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.93 }; //type11
 	Double_t JEC_down[n_signal_bins] = { 1, 1, 1, 0.81, 1, 1, 1, 1, 0.94, 1, 1, 1, 1, 0.98, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.0 }; //type11
 	Double_t JEC_large[n_signal_bins] = { 1, 1, 1, 0.81, 1, 1, 1, 1, 0.94, 1, 1, 1, 1, 0.98, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.93 }; //type11
 	Double_t TTBAR_JEC_up[n_signal_bins] = { 1, 1, 1, 0.97, 1, 1, 1, 1, 1.29, 1, 1, 1, 1, 1.4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.53 }; //type11
 	Double_t TTBAR_JEC_up[n_signal_bins] = { 1, 1, 1, 0.99, 1, 1, 1, 1, 0.72, 1, 1, 1, 1, 0.73, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.82 }; //type11
 	Double_t TTBAR_JEC_large[n_signal_bins] = { 1, 1, 1, 0.97, 1, 1, 1, 1, 0.72, 1, 1, 1, 1, 1.4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.53 }; //type11
 	

 	TH1D *mttMass_Bkgd = mttMass_Pred1->Clone();
	mttMass_Bkgd->Add( mttMass_Bkgd, mttMass_TT,1,1);
 	
	int low[n_signal_bins]={0};
	int up[n_signal_bins]={0};
	
	for (int i =0; i<n_signal_bins; i++)
	{
		low[i] = mttMass_Bkgd->FindBin( windowLB[i] )   ;
		up[i]  = mttMass_Bkgd->FindBin( windowUB[i] ) -1;
	}
	
	//-------------------------------------------------------------------------------------
	cout<<"table type "<<type<<endl;
	cout<<"table mass     LB     UB     Eff     sigEff     Nbkg     sigNbkg     Ndata   "<<endl;
	//loop over signal mass points
	for (int j =3; j<n_signal_bins; j++) 
	{
		cout<<"\n\n&Signal mass m_zp_new["<<j<<"] = "<< m_zp_new[j]<<endl;
		double count_sig=0;
		double check_count_sig=0;
		double count_sig2=0;
		double count_data=0;
		//double count_bkgd=0;
		double count_ttbar=0;
		double count_ttbar2=0;
		double count_qcd=0;
		double count_qcd2=0;
		
		double count_sig_before_window=0;
		
		double error2_sig=0;
		double error2_data=0;
		double error2_bkgd=0;
		double error2_qcd=0;
		double error2_ttbar=0;
	
		for (int i=1; i<=mttMass_Bkgd->GetNbinsX(); i++)  //loop through histogram bins and count entries in signal window
		{
			double center = mttMass_Bkgd->GetBinCenter(i);	
			if  ( m_zp_new[j] ==  m_zp_mc[j] ) count_sig_before_window += signal_histos[j]->GetBinContent(i);

			if ( center>windowLB[j] && center<windowUB[j] )
			{
			    //cout<<"hist "<<signal_histos[j]->GetBinContent(i)<<endl;
			    //cout<<"hist check"<<signal_histos_check[j]->GetBinContent(i)<<endl;
				if  ( m_zp_new[j] ==  m_zp_mc[j] ) check_count_sig += signal_histos_check[j]->GetBinContent(i);
				if  ( m_zp_new[j] ==  m_zp_mc[j] ) count_sig += signal_histos[j]->GetBinContent(i);
				if  ( m_zp_new[j] ==  m_zp_mc[j] ) count_sig2 += signal_histos2_for_trigger_error[j]->GetBinContent(i);
				count_data 			+= mttMass_Data->GetBinContent(i);
				count_ttbar			+= mttMass_TT->GetBinContent(i);
				count_ttbar2		+= mttMass2_TT->GetBinContent(i);
				//count_bkgd 			+= mttMass_Bkgd->GetBinContent(i);
				count_qcd			+= mttMass_Pred1->GetBinContent(i);
				count_qcd2			+= mttMass_Pred2->GetBinContent(i);
				
				if  ( m_zp_new[j] ==  m_zp_mc[j] ) error2_sig += pow(signal_histos[j]->GetBinError(i),2);
				error2_data 		+= pow(mttMass_Data->GetBinError(i),2);
				error2_ttbar		+= pow(mttMass_TT->GetBinError(i),2);
				error2_bkgd			+= pow(mttMass_Bkgd->GetBinError(i),2);
				error2_qcd			+= pow(mttMass_Pred1->GetBinError(i),2);
			}
		}	
		cout << "count_sig_before_window "<<count_sig_before_window<<endl;
		cout << "check_count_sig   " << check_count_sig   << endl;
		cout << "count_sig   " << count_sig   << endl;
		cout << "count_sig2   " << count_sig2   << endl;
		cout << "count_data  " << count_data  << endl;
		cout << "count_ttbar " << count_ttbar << endl;
		cout << "count_ttbar2 " << count_ttbar2 << endl;
		//cout << "count_bkgd  " << count_bkgd  << endl;
		cout << "count_qcd   " << count_qcd   << endl;
		cout << "count_qcd2  " << count_qcd2  << endl;
		cout << "error2_sig   " << error2_sig   << endl;
		cout << "error2_data  " << error2_data  << endl;
		cout << "error2_ttbar " << error2_ttbar << endl;
		cout << "error2_bkgd  " << error2_bkgd  << endl;
		cout << "error2_qcd   " << error2_qcd   << endl;
		// Find the two closest MC mass points
		int lo =-1;
		int hi =-1;
		for ( int k = j ; k>=0 ; k--){
			if ( m_zp_mc[k]!=0 ) { lo=k; break;}
		}
		for ( int k = j ; k<n_signal_bins ; k++){
			if ( m_zp_mc[k]!=0 ) {hi=k; break;}
		}
		
		//-------------------------------------------------------------------------------------
		// Calculate efficiency and stat error
		cout<<" calculate efficiency "<<endl;
	
		double signal_window_eff = 0 ;	
		double signal_eff_prewindow = 0 ;	
		double signal_eff_selected = 0 ;	
		double signal_eff_selected_error = 0 ;	
		//double JEC_uncer = 0.05;



		if (dataset_nevents[j]!=0) 
		{ 
			signal_eff_selected 		= count_sig / dataset_nevents[j];
			signal_eff_selected_error 	= sqrt( error2_sig )/ dataset_nevents[j];
			
			
		
            
            //cout<<"NEW fabs(1-trig_eff) = "<<fabs(1-trig_eff)<<endl;
		    
		    //trig_eff_error = fabs(count_sig - count_sig2)/2
		    
			//trig_eff_error = fabs( count_sig - (count_sig+count_sig2)/2 )/dataset_nevents[j];
            //double trig_eff_error_sal = fabs( 1-trig_eff )/2 *trig_eff;

		    
		    //cout << "|count_sig - average|   " << fabs(count_sig-average_count)   << endl;
		    //cout << "|count_sig - average|/2   " << fabs(count_sig-average_count)/2   << endl;
			//cout<<"fabs( count_sig - (count_sig+count_sig2)/2 ) "<<fabs( count_sig - (count_sig+count_sig2)/2 )<<endl;
			//trig_eff = count_sig/count_sig2 ;
			//trig_eff_error = fabs( count_sig - (count_sig+count_sig2)/2 )/dataset_nevents[j];
            //double trig_eff_error_sal = fabs( 1-trig_eff )/2 *trig_eff;

			signal_eff_prewindow 		= count_sig_before_window / dataset_nevents[j];
			if ( count_sig_before_window!=0 ) signal_window_eff = count_sig / count_sig_before_window ;

		}	
		else 
		{
			cout<<"   no signal mc - lets morph "<<endl;
		
		 	signal_eff_selected = morphEff( signal_histos[lo], signal_histos[hi], m_zp_new[j], windowLB[j], windowUB[j], m_zp_mc[lo], m_zp_mc[hi], dataset_nevents[lo], dataset_nevents[hi], type );
	
		}

		//-------------------------------------------------------------------------------------
		// Efficiency and Efficiency Error
				
		//double trig_eff = 1;
		//double trig_eff_error = 0;
		//if (type==11) trig_eff =  0.9969;   
		//if (type==11) trig_eff_error =  0.0015;   // +0.10 -0.15%
		//if (type==12) trig_eff =  0.9718;   
		//if (type==12) trig_eff_error =  0.0025;   // +0.10 -0.15%
			
	   	
	   	double trig_eff =0;
        double trig_eff_error =0;
        double trig_eff_relative_error =0;
        
        cout << "weighted count  =  " << count_sig   << endl;
        cout << "unweighted count =  " << count_sig2   << endl;
        
        if (count_sig2!=0) trig_eff = count_sig/count_sig2 ;
        trig_eff_error = fabs(1-trig_eff)/2;
        if (trig_eff!=0) trig_eff_relative_error = trig_eff_error / trig_eff;

        cout<<"NEW trig_eff                  = "<<trig_eff<<endl;
        cout<<"NEW trig_eff_error            = "<<trig_eff_error<<endl;
        cout<<"NEW trig_eff_relative_error   = "<<trig_eff_relative_error<<endl;
        
		// // 0.93 +- 0.13	for each jet//  based on W tagging but also used for top tagging (0.92 +- 0.06) * (1.01 +- 0.11) = 0.93 +- 0.12	
		//double tagging_eff_scale_factor =  0.93; 
		//double tagging_eff_scale_factor_error = 0.13;	
		//double tagging_eff_event_scale_factor =  tagging_eff_scale_factor*tagging_eff_scale_factor; 
		double tagging_eff_scale_factor_relative_error = tagging_eff_scale_factor_error / tagging_eff_scale_factor;		
				
		double jec_relative_error = fabs(1-JEC_large[j]);		
		cout<<"fabs(1-JEC_large[j]) "<<fabs(1-JEC_large[j])<<endl;
		
		double signal_eff = signal_eff_selected * tagging_eff_scale_factor * tagging_eff_scale_factor;
		double signal_eff_error = 0;
		
		if  ( m_zp_new[j] ==  m_zp_mc[j] )  //for signal MC
		{
			if ( signal_eff_selected != 0  )
			{		  

			
			    signal_eff_error = signal_eff * sqrt ( 
			                                       pow( signal_eff_selected_error / signal_eff_selected, 2) 
												 + pow( trig_eff_relative_error, 2) 
												 + pow( 2*tagging_eff_scale_factor_relative_error, 2) 
												 //+ pow( tagging_eff_scale_factor_relative_error, 2) 
												 + pow( jec_relative_error, 2) 
												 );
			//sqrt( 0.0157273^2 + 0.195364^2 + 0.27957^2 + 0.19^2)
			    cout<<"pow( signal_eff_selected_error / signal_eff_selected, 2)  "<<pow( signal_eff_selected_error / signal_eff_selected, 2) <<endl;
			    cout<<"pow( trig_eff_relative_error, 2)  "<<pow( trig_eff_relative_error, 2)  <<endl;
			    cout<<"pow( tagging_eff_scale_factor_relative_error, 2) +pow( tagging_eff_scale_factor_relative_error, 2) "<<pow( tagging_eff_scale_factor_relative_error, 2) +pow( tagging_eff_scale_factor_relative_error, 2)<<endl;
			    cout<<"signal_eff_error "<<signal_eff_error<<endl;
			    cout<<"signal_eff_error/signal_eff "<<signal_eff_error/signal_eff<<endl;
			    
			    //double signal_eff_error_before_scaling = sqrt( pow( signal_eff_selected_error, 2) 
			    //                                             + pow( trig_eff_error, 2) 
			    //                                             + pow( jec_error, 2) 
				
				//signal_eff_error = signal_eff * sqrt ( pow( signal_eff_error_before_scaling / signal_eff_selected, 2) 
				//								 + 2* pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2); 
				
				
				//signal_eff_error = signal_eff * sqrt ( pow( signal_eff_selected_error / signal_eff_selected, 2) 
				//								 + pow( trig_eff_error / trig_eff, 2) 
				//								 + pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2) 
				//								 + pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2) 
				//								 + pow( jec_error, 2) );
			}
	    }
		else // for  morphed points
		{
			// find statisitical error of two closest MC mass points and use the larger error
			double error2_sig_hi=0;
			double error2_sig_lo=0;
			double count_sig_hi=0;
			double count_sig2_hi=0;
			double count_sig_lo=0;
			double count_sig2_lo=0;
		
			for (int i=1; i<=mttMass_Bkgd->GetNbinsX(); i++)
			{
				double center = mttMass_Bkgd->GetBinCenter(i);	
				if ( center>windowLB[hi] && center<windowUB[hi] )  error2_sig_hi += pow( signal_histos[hi]->GetBinError(i), 2);
				if ( center>windowLB[lo] && center<windowUB[lo] )  error2_sig_lo += pow( signal_histos[lo]->GetBinError(i), 2);	
				
				if ( center>windowLB[hi] && center<windowUB[hi] )
				{
				    count_sig_hi += signal_histos[hi]->GetBinContent(i);
				    count_sig2_hi += signal_histos2_for_trigger_error[hi]->GetBinContent(i);
				}
				
				//cout<<"center "<<center<<"  windowLB[lo] "<<windowLB[lo]<<"  windowUB[lo]  "<<windowUB[lo]<<endl;
				if ( center>windowLB[lo] && center<windowUB[lo] )
				{
				    
				    count_sig_lo += signal_histos[lo]->GetBinContent(i);
				    count_sig2_lo += signal_histos2_for_trigger_error[lo]->GetBinContent(i);
				    //cout<<"  signal_histos[j]->GetBinContent(i) "<<signal_histos[lo]->GetBinContent(i)<<endl;
				    //cout<<"  count_sig_lo "<<count_sig_lo<<endl;
				}
			}
			double error_sig_hi = sqrt(error2_sig_hi) / dataset_nevents[hi];
			double error_sig_lo = sqrt(error2_sig_lo) / dataset_nevents[lo];

            
//I am getting the morphed JEC error, trigger error, and statistical error from the larger error of the closests two MC points

			double neighbor_MC_larger_stat_error = 0;
			if ( error_sig_hi > error_sig_lo ) neighbor_MC_larger_stat_error = error_sig_hi;
			else neighbor_MC_larger_stat_error = error_sig_lo;
			signal_eff_selected_error = neighbor_MC_larger_stat_error;
			
			cout<<"error_sig_lo "<<error_sig_lo<<"  error_sig_hi "<<error_sig_hi<<"  neighbor_MC_larger_stat_error "<<neighbor_MC_larger_stat_error<<endl;

			// find JEC error of two closest MC mass points and use the larger error

			double neighbor_MC_larger_JEC_error = 0;
			if ( fabs(1-TTBAR_JEC_large[lo]) > fabs(1- JEC_large[hi]) ) neighbor_MC_larger_JEC_error = fabs(1-JEC_large[lo]);
			else neighbor_MC_larger_JEC_error = fabs(1-JEC_large[hi]);
		
			cout<<"TTBAR_JEC_large[lo] "<<JEC_large[lo]<<"  TTBAR_JEC_large[hi] "<<JEC_large[hi]<<"  neighbor_MC_larger_JEC_error "<<neighbor_MC_larger_JEC_error<<endl;
			jec_relative_error = neighbor_MC_larger_JEC_error;
			
			// trigger
			
			 double trig_eff_lo =0;
             double trig_eff_error_lo =0;
             double trig_eff_relative_error_lo =0;
        
             double trig_eff_hi =0;
             double trig_eff_error_hi =0;
             double trig_eff_relative_error_hi =0;
        
           
             if (count_sig2_lo!=0) trig_eff_lo = count_sig_lo/count_sig2_lo ;
             trig_eff_error_lo = fabs(1-trig_eff_lo)/2;
             if (trig_eff_error_lo!=0) trig_eff_relative_error_lo = trig_eff_error_lo / trig_eff_lo;
   
             if (count_sig2_hi!=0) trig_eff_hi = count_sig_hi/count_sig2_hi ;
             trig_eff_error_hi = fabs(1-trig_eff_hi)/2;
             if (trig_eff_error_hi!=0) trig_eff_relative_error_hi = trig_eff_error_hi / trig_eff_hi;

            cout<<"count_sig_lo "<<count_sig_lo<<endl;
            cout<<"count_sig2_lo "<<count_sig2_lo<<endl;
			
			double neighbor_MC_larger_trig_relative_error = 0;
			if ( trig_eff_relative_error_lo > trig_eff_relative_error_hi ) neighbor_MC_larger_trig_relative_error = trig_eff_relative_error_lo;
			else neighbor_MC_larger_trig_relative_error = trig_eff_relative_error_hi;
		
			cout<<"trig_eff_relative_error_lo "<<trig_eff_relative_error_lo<<"  trig_eff_relative_error_hi "<<trig_eff_relative_error_hi<<"  neighbor_MC_larger_trig_relative_error "<<neighbor_MC_larger_trig_relative_error<<endl;
			trig_eff_relative_error = neighbor_MC_larger_trig_relative_error;
			
			
			//interpolate trigger efficiency 
			double EFF_slope =  (trig_eff_hi-trig_eff_lo) / (m_zp_mc[hi] - m_zp_mc[lo] );
            double EFF_intercept = trig_eff_hi - EFF_slope* m_zp_mc[hi];

            trig_eff = EFF_slope*m_zp_new[j]+EFF_intercept;

                
            cout<<"trig_eff_lo "<<trig_eff_lo<<"  trig_eff_hi "<<trig_eff_hi<<"  trig_eff "<<trig_eff<<endl;
			
			// calculate efficiency error
			if ( signal_eff_selected != 0  )
			{
			    
			    //double signal_eff_error_before_scaling = sqrt( pow( signal_eff_selected_error, 2) 
			    //                                             + pow( trig_eff_error, 2) 
			    //                                             + pow( jec_error, 2) 
				
				//signal_eff_error = signal_eff * sqrt ( pow( signal_eff_error_before_scaling / signal_eff_selected, 2) 
				//								 + 2* pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2); 
				
				signal_eff_error = signal_eff * sqrt ( pow( signal_eff_selected_error / signal_eff_selected, 2) 
												 + pow( trig_eff_relative_error, 2) 
												 + pow( 2*tagging_eff_scale_factor_relative_error, 2) 
												 //+ pow( tagging_eff_scale_factor_relative_error, 2) 
												 + pow( jec_relative_error, 2) );
			    cout<<"signal_eff_error "<<signal_eff_error<<endl;
				//signal_eff_error = signal_eff * sqrt ( pow( signal_eff_selected_error / signal_eff_selected, 2) 
				//									 + pow( trig_eff_error / trig_eff, 2) 
				//									 + pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2) 
				//									 + pow( tagging_eff_scale_factor_error / tagging_eff_scale_factor, 2) 
				//									 + pow( jec_error, 2) );
			}										 
		}
		cout<<"  & "<<endl;
		cout<<"  & "<<endl;
		cout<<"  & Mass = "<<m_zp_new[j]<<endl;
		cout<<"  & ______________________________  "<<endl;
		cout<<"  & Efficiency - type "<<type<<" - mass = "<<m_zp_new[j]<<endl;
		cout<<fixed << setprecision(1);
		cout<<"  & ______________________________  "<<endl;
		cout<<"  & selection efficiency MC only    "<<100*signal_eff_selected/trig_eff<<endl;
		cout<<"  & trigger efficiency              "<<100*trig_eff<<endl;
		cout<<"  & scale factor                    "<<100*tagging_eff_scale_factor*tagging_eff_scale_factor<<endl;
		cout<<"  & Total efficiency                "<<100*signal_eff<<endl;
		//cout<<"  & ______________________________  "<<endl;
		//cout<<"  & error on selection efficiency   "<<signal_eff_selected_error<<endl;
		//cout<<"  & error on trigger efficiency     "<<trig_eff_error<<endl;
		//cout<<"  & error on scale factor           "<<tagging_eff_scale_factor_error<<endl;
		//cout<<"  & error on JEC                    "<<jec_error<<endl;
		//cout<<"  & Total efficiency error          "<<signal_eff_error<<endl;
		//cout<<"  & relative error on selection efficiency   "<< signal_eff_selected_error / signal_eff_selected <<endl;
		//cout<<"  & relative error on trigger efficiency     "<< trig_eff_error / trig_eff <<endl;
		//cout<<"  & relative error on scale factor           "<< sqrt(2) * tagging_eff_scale_factor_error / tagging_eff_scale_factor <<endl;
		//cout<<"  & relative error on JEC                    "<< jec_error <<endl;
		///cout<<"  & Total relative error                     "<< signal_eff_error / signal_eff <<endl;	
		cout<<"  & ______________________________  "<<endl;
		cout<<"  & Relative efficiency error in %  "<<endl;
		
		cout<<"  &   selection efficiency   "<< 100*signal_eff_selected_error / signal_eff_selected <<endl;
		cout<<"  &   trigger efficiency     "<< 100*trig_eff_relative_error <<endl;
		cout<<"  &   scale factor           "<< 100*2*tagging_eff_scale_factor_relative_error <<endl;
		cout<<"  &   JEC                    "<< 100*jec_relative_error <<endl;
		cout<<"  &   TOTAL                  "<< 100*signal_eff_error / signal_eff <<endl;		
		
		
		//-------------------------------------------------------------------------------------
		// Background Error calculation
		
	
		double ttbar_count_scaled = count_ttbar * tagging_eff_scale_factor * tagging_eff_scale_factor;
			
		// find JEC error of two closest MC mass points and use the larger error

		double neighbor_MC_larger_TTBAR_JEC_error = 0;
		if ( fabs(1-TTBAR_JEC_large[lo]) > fabs(1- TTBAR_JEC_large[hi]) ) neighbor_MC_larger_TTBAR_JEC_error = fabs(1-TTBAR_JEC_large[lo]);
		else neighbor_MC_larger_TTBAR_JEC_error = fabs(1-TTBAR_JEC_large[hi]);
		
		cout<<"TTBAR_JEC_large[lo] "<<TTBAR_JEC_large[lo]<<"  TTBAR_JEC_large[hi] "<<TTBAR_JEC_large[hi]<<"  neighbor_MC_larger_TTBAR_JEC_error "<<neighbor_MC_larger_TTBAR_JEC_error<<endl;
		
		double ttbar_jec_error 			= neighbor_MC_larger_TTBAR_JEC_error;		

        // trigger
		
		double trig_ttbar_eff =0;
        double trig_ttbar_eff_error =0;
        double trig_ttbar_eff_relative_error =0;
        
        cout << "weighted count  =  " << count_ttbar   << endl;
        cout << "unweighted count =  " << count_ttbar2   << endl;
        
        if (count_ttbar2!=0) trig_ttbar_eff = count_ttbar/count_ttbar2 ;
        trig_ttbar_eff_error = fabs(1-trig_ttbar_eff)/2;
        if (trig_ttbar_eff!=0) trig_ttbar_eff_relative_error = trig_ttbar_eff_error / trig_ttbar_eff;

        cout<<"NEW trig_ttbar_eff                  = "<<trig_ttbar_eff<<endl;
        cout<<"NEW trig_ttbar_eff_error            = "<<trig_ttbar_eff_error<<endl;
        cout<<"NEW trig_ttbar_eff_relative_error   = "<<trig_ttbar_eff_relative_error<<endl;
		
		//other ttbar errors
		
	    double ttbar_stat_error 		= sqrt( error2_ttbar ) / count_ttbar;
		double ttbar_lumi_error         = 0.06;
		double ttbar_xsec_error         = 0.1;
		
		
		// total ttbar error
		double total_ttbar_relative_error	=   sqrt(  pow( ttbar_stat_error, 2)
		                                             + pow( trig_ttbar_eff_relative_error, 2)
		                                             + pow( 2*tagging_eff_scale_factor_relative_error, 2) 
		                                             + pow( ttbar_jec_error, 2)
		                                             + pow( ttbar_lumi_error, 2)
		                                             + pow( ttbar_xsec_error, 2)
		                                             );
		
		// qcd errors
 		double qcd_stat_error 		= sqrt( error2_qcd ) / count_qcd;
 		double qcd_mistag_error 	= fabs(1-count_qcd2/count_qcd);  
 		//cout<<" fabs( count_qcd - (count_qcd+count_qcd2)/2 ) /count_qcd  = "<<fabs( count_qcd - (count_qcd+count_qcd2)/2 ) /count_qcd <<endl;;
 		double total_qcd_relative_error		= sqrt( pow( qcd_stat_error, 2)    + pow( qcd_mistag_error, 2)  );

			
		// total bkg errors
		double count_bkgd = count_qcd + ttbar_count_scaled;
		double bkgd_error 	= sqrt( pow( total_ttbar_relative_error*ttbar_count_scaled, 2) + pow( total_qcd_relative_error*count_qcd, 2)  );
		double ttbar_weight = (ttbar_count_scaled / count_bkgd );
		double qcd_weight   = (count_qcd / count_bkgd );
		double relative_bkgd_error = sqrt( pow( ttbar_weight,2) * pow( total_ttbar_relative_error, 2) 
		                                 + pow( qcd_weight,2)   * pow( total_qcd_relative_error,   2) 
		                                 );

        
		
		cout<<"  & ______________________________ " << endl;
		cout<<"  & Background - type "<<type<<" - mass = " << m_zp_new[j]<<endl;
		cout<<"  & ______________________________ " << endl;
		cout<<"  & number of events               " << count_bkgd <<endl;
		cout<<"  & number of qcd                  " << count_qcd <<endl;
		cout<<"  & number of ttbar                " << ttbar_count_scaled <<endl;
		cout<<"  & ______________________________ " << endl;
		cout<<"  & Relative errors in %: " << endl;
		cout<<"  &  ttbar stat            "<<100*ttbar_stat_error<<endl;
		cout<<"  &  ttbar trig eff        "<<100*trig_ttbar_eff_relative_error<<endl;
		cout<<"  &  ttbar tagging eff     "<<100*2*tagging_eff_scale_factor_relative_error<<endl;
		cout<<"  &  ttbar jec             "<<100*ttbar_jec_error<<endl;
		cout<<"  &  ttbar lumi            "<<100*ttbar_lumi_error<<endl;
		cout<<"  &  ttbar xsection        "<<100*ttbar_xsec_error<<endl;
		cout<<"  &  ttbar TOTAL           "<<100*total_ttbar_relative_error<<endl;
	    cout<<"  & ______________________________ " << endl;
        cout<<"  &  qcd mistag            "<<100*qcd_mistag_error<<endl;
        cout<<"  &  qcd stat              "<<100*qcd_stat_error<<endl;
        cout<<"  &  qcd TOTAL             "<<100*total_qcd_relative_error<<endl;
        cout<<"  & ______________________________ " << endl;
        cout<<fixed << setprecision(3);
        cout<<"  &  ttbar_weight          "<<ttbar_weight <<endl;
        cout<<"  &  qcd_weight            "<<qcd_weight <<endl;
        cout<<fixed << setprecision(1);
        cout<<"  &  bkgd TOTAL            "<<100*relative_bkgd_error <<endl;
        cout<<"  & ______________________________ " << endl;
        cout<<"  &   check bkgd_error/(count_qcd+count_ttbar) "<<bkgd_error/(count_qcd+ttbar_count_scaled)<<endl;
		
		//cout<<"  & statistical error              " << bkgd_stat_error <<endl;
		//cout<<"  & mistag kinematics systematic   " << bkgd_mistag_error <<endl;
		//cout<<"  & JEC error                      " << bkgd_JEC_error	<<endl;
		//cout<<"  & Trigger error                  " << bkgd_trigger_error	<<endl;
		//cout<<"  & Total bkgd error               " << bkgd_error	<<endl;
		//cout<<"  & ______________________________ " << endl;
		//cout<<"  & ttbar stat error               " << ttbar_stat_error <<endl;
		//cout<<"  & JEC error                      " << ttbar_jec_error	<<endl;
		//cout<<"  & Trigger error                  " << ttbar_trig_error	<<endl;
		//cout<<"  & Scale error                    " << ttbar_scale_error	<<endl;
		//cout<<"  & Lumi error                     " << ttbar_lumi_error	<<endl;
		//cout<<"  & Xsection error                 " << ttbar_xsec_error	<<endl;
		//cout<<"  & Total ttbar error              " << total_ttbar_error	<<endl;		
		//cout<<"  & ______________________________ " << endl;
		//cout<<"  & qcd stat error                 " << qcd_stat_error <<endl;
		//cout<<"  & mistag kinematics systematic   " << qcd_mistag_error <<endl;
		//cout<<"  & Total QCD estimate error       " << total_qcd_error	<<endl;
		//cout<<"  & ______________________________ " << endl;
		//cout<<"  & Total bkgd error               " << bkgd_error	<<endl;
		//cout<<"  & ______________________________ " << endl;
		/*if ( count_ttbar!=0 && count_qcd!=0 && count_bkgd!=0 )
		{
		cout<<"  & ttbar stat relative error      " << ttbar_stat_error  / count_ttbar <<endl;
		cout<<"  & JEC relative error             " << ttbar_jec_error   / count_ttbar 	<<endl;
		cout<<"  & Trigger relative error         " << ttbar_trig_error  / count_ttbar 	<<endl;
		cout<<"  & Scale relative error           " << sqrt(2)*ttbar_scale_error / count_ttbar 	<<endl;
		cout<<"  & Lumi relative error            " << ttbar_lumi_error  / count_ttbar 	<<endl;
		cout<<"  & Xsection relative error        " << ttbar_xsec_error  / count_ttbar 	<<endl;
		cout<<"  & Total ttbar relative error     " << total_ttbar_error / count_ttbar 	<<endl;		
		cout<<"  & ______________________________ " << endl;
		cout<<"  & qcd stat relative error        " << qcd_stat_error   / count_qcd  <<endl;
		cout<<"  & mistag kinematics systematic   " << qcd_mistag_error / count_qcd <<endl;
		cout<<"  & Total QCD relative error       " << total_qcd_error  / count_qcd	<<endl;
		cout<<"  & ______________________________ " << endl;
		cout<<"  & Total bkgd relative error      " << bkgd_error / count_bkgd	<<endl;
		cout<<"  & ______________________________ " << endl;
		}
		
		//double bkgd_stat_error 		= sqrt( error2_bkgd );
		//double ttbar_trig_error 		= trig_eff_error;
		//double ttbar_scale_error 		= count_ttbar * tagging_eff_scale_factor_error;
				
		//double ttbar_stat_error 		= sqrt( error2_ttbar );
		//double ttbar_jec_error 			= count_ttbar * neighbor_MC_larger_TTBAR_JEC_error;		
 		//double ttbar_trig_error 		= count_ttbar * trig_eff_error;
		//double ttbar_scale_error 		= count_ttbar * tagging_eff_scale_factor_error;
		//double ttbar_lumi_error         = count_ttbar * 0.06;
		//double ttbar_xsec_error         = count_ttbar * 0.1;
				
        //sqrt( pow( ttbar_stat_error, 2)  + pow( ttbar_jec_error, 2)  + pow( ttbar_trig_error, 2)
		//								  + pow( ttbar_scale_error, 2) + pow( ttbar_scale_error, 2)  
		//								  + pow( ttbar_lumi_error, 2) + pow( ttbar_xsec_error, 2) );
		//cout<<"qcd_mistag_error "<<qcd_mistag_error<<endl;								  
		//double paratial_qcd_relative_error		= sqrt( pow( qcd_stat_error, 2)    + pow( qcd_mistag_error, 2)  );
		
		//double partial_qcd_error = count_qcd * paratial_qcd_relative_error;
		
		//double total_qcd_error = sqrt( pow( partial_qcd_error, 2) +   pow( ttbar_lumi_error, 2)  + pow( ttbar_xsec_error, 2)  ); 
		
		//double total_qcd_relative_error = total_qcd_error/ count_qcd;
			
		//cout<<"paratial_qcd_relative_error "<<paratial_qcd_relative_error<<endl;
		//cout<<"partial_qcd_error "<<partial_qcd_error<<endl;
		//cout<<"total_qcd_error "<<total_qcd_error<<endl;
		//cout<<"total_qcd_relative_error "<<total_qcd_relative_error<<endl;
		
		
		*/
		// MAKE TABLE
		//mass    LB       UB       Eff            sigEff         Nbkg        sigNbkg    Ndata   
		//if  ( m_zp_new[j] ==  m_zp_mc[j] ) signal_eff_error=0;
		
		cout.unsetf(ios::floatfield);  //turn off fixed
		cout << "table "<< setprecision(6) << m_zp_new[j]<<"     "<< windowLB[j] <<"     "<< windowUB[j] << "     " << signal_eff << "     " << signal_eff_error << "    " << count_bkgd <<"     "<< bkgd_error<<"     "<< count_data <<"     "<<endl;
		cout << "notetable "<< m_zp_new[j]<<"  &   "<< windowLB[j] <<"-"<< windowUB[j] << "  &   " <<fixed << setprecision(2)<< 100*signal_eff_selected/trig_eff <<"\\%  &   " << 100*trig_eff <<"\\%  &   " << 100*signal_eff << "\\%  &   " << 100*signal_eff_error<< "\\%  &   " << count_bkgd <<"  &   "<< bkgd_error<<"  &   "<< setprecision(0)<<count_data <<"  \\\\   "<<endl;
			
	}
}

double morphEff( TH1D * h1, TH1D * h2, double m_zp, double window_LB, double window_UB, double m_lo, double m_hi, double dataset_nevents_lo, double dataset_nevents_hi, int type )   {

  RooRealVar mtt("mtt", "m_{tt}", 0, 5000, "GeV/c^{2}");

  RooDataHist dh1("dh1","dh1",mtt,Import(*h1)) ;
  RooDataHist dh2("dh2","dh2",mtt,Import(*h2)) ;

 // TCanvas *c1000= new TCanvas("c1000","",200,10,700,650);	
  TCanvas *c1000= new TCanvas("c1000","",200,10,800,500);	

  stringstream tempj;
  tempj << m_zp;
  string name = tempj.str();
  string savename = "";
  string titlename = "";
  if (type == 11 )
  {	
  	string savename = "morphed_type11_m"+name+".pdf";
  	titlename = "interpolate  m = "+name+" GeV; Type 1+1 m_{tt} (GeV/c^{2})";
  }
  if (type == 12 )
  {
    string savename = "morphed_type12_m"+name+".pdf";
  	titlename = "interpolate  m = "+name+" GeV; Type 1+2 m_{tt} (GeV/c^{2})";
  }
  
  RooPlot* fr = mtt.frame(Title("m_{tt}")) ;
  dh1.plotOn(fr) ;
  dh2.plotOn(fr) ;
  fr->GetXaxis()->SetRangeUser(m_lo-500, m_hi+500);
  fr->SetTitle(titlename.c_str());
  fr->Draw();

  RooHistPdf hpdf1("hpdf1","hpdf1",mtt,dh1,0) ;
  RooHistPdf hpdf2("hpdf2","hpdf2",mtt,dh2,0) ;

  RooPlot* fr2 = mtt.frame(Title("m_{tt}")) ;
  hpdf1.plotOn(fr2,LineColor(kBlue)) ;
  hpdf2.plotOn(fr2,LineColor(kGreen)) ;


  //--- Calculate alpha, which needs to be between 0 and 1
  Double_t malpha = (-m_zp + m_hi) / (m_hi- m_lo);
  std::cout << "Alpha for " << m_zp << " is " << malpha << std::endl;

  RooRealVar alpha("alpha","alpha", malpha, 0., 1.) ;
  RooIntegralMorph ipdf("ipdf","Interpolated histo pdf", hpdf1, hpdf2, mtt, alpha) ;

  mtt.setRange("signal", window_LB, window_UB);

  RooNumIntConfig* cfg = RooAbsReal::defaultIntegratorConfig();
  cfg->setEpsAbs(1E-6);
  cfg->setEpsRel(1E-6);
  ipdf.setIntegratorConfig(*cfg);
  ipdf.plotOn(fr2,LineColor(kRed));
  fr2->GetXaxis()->SetRangeUser(m_lo-500, m_hi+500);
  if (type == 11 ) fr2->SetTitle(";Type 1+1 m_{tt} (GeV/c^{2});Arbitrary Units");
  if (type == 12 ) fr2->SetTitle(";Type 1+2 m_{tt} (GeV/c^{2});Arbitrary Units");
  fr2->Draw();
  
  TH1F* dummy1 = new TH1F("dummy1","dummy1",500,0,4000);	
  TH1F* dummy2 = new TH1F("dummy2","dummy2",500,0,4000);	
  TH1F* dummy3 = new TH1F("dummy3","dummy3",500,0,4000);	
  dummy1->SetLineColor(kBlue);
  dummy2->SetLineColor(kGreen);
  dummy3->SetLineColor(kRed);
  dummy1->SetMarkerColor(kBlue);
  dummy2->SetMarkerColor(kGreen);
  dummy3->SetMarkerColor(kRed);
  
  stringstream temp1;
  temp1 << m_lo;
  string name1 = temp1.str();
  string L1 = "Signal MC m = "+name1;
  
  stringstream temp2;
  temp2 << m_hi;
  string name2 = temp2.str();
  string L2 = "Signal MC m = "+name2;
  
  stringstream temp3;
  temp3 << m_zp;
  string name3 = temp3.str();
  string L3 = "Interpolated m = "+name3;
  
  leg3 = new TLegend(0.66,0.7,0.95,0.92);
  //leg3->SetHeader(legend_header.c_str());
  leg3->SetBorderSize(0);
  leg3->SetFillColor(0);
  leg3->AddEntry(dummy1,L1.c_str(),"L");
  leg3->AddEntry(dummy2,L2.c_str(),"L");
  leg3->AddEntry(dummy3,L3.c_str(),"L");
  leg3->Draw("same");
  
  c1000->SaveAs(savename.c_str());
  //c1000->SaveAs(savename.c_str());
   
  RooAbsReal* ipdf_sig = ipdf.createIntegral(mtt,NormSet(mtt),Range("signal")) ;
  double effic_mass_window = ipdf_sig->getVal();
  cout<<"effic_mass_window "<<effic_mass_window<<endl;
 
  double effic_pre_mass_window_lo =  h1->GetEntries()/dataset_nevents_lo;
  double effic_pre_mass_window_hi =  h2->GetEntries()/dataset_nevents_hi;
  double effic_pre_mass_window_slope 		= (effic_pre_mass_window_hi-effic_pre_mass_window_lo) / (m_hi - m_lo );
  double effic_pre_mass_window_intercept 	= effic_pre_mass_window_hi - effic_pre_mass_window_slope* m_hi;

  double effic_pre_mass_window = effic_pre_mass_window_slope*m_zp + effic_pre_mass_window_intercept;
  cout<<"effic_pre_mass_window_lo "<<effic_pre_mass_window_lo<<"  effic_pre_mass_window_hi "<< effic_pre_mass_window_hi <<"  effic_pre_mass_window "<<effic_pre_mass_window<<endl;
  		
  double efficiency = effic_pre_mass_window * effic_mass_window;//  * effic_type11_trig ;
   cout<<"effic_pre_mass_window "	<<effic_pre_mass_window<<endl;
   cout<<"effic_mass_window "		<<effic_mass_window<<endl;
   cout<<"efficiency "		<<efficiency<<endl;
	cout<<"exiting morph"<<endl;
  return efficiency;
    
}


void MakeHistogram3(
	TH1D * h_DATA,
	TH1D * h_TTBAR, 
	TH1D * h_TTBAR2, 
	TH1D * h_QCD,
	TH1D * h_QCD2,
	std::string title,
	std::string legend_header,
	std::string L1, 
	std::string L2, 
	std::string L3, 
	double legXmin, double legYmin, double legXmax, double legYmax, 
	double YaxisTitleOffset, 
	std::string savename, 
	int rebin, 
	double lowerbound, 
	double upperbound,
	int type)
{
	//TCanvas *c1000= new TCanvas("c1000","",200,10,700,650);	
	  //TCanvas *c1000= new TCanvas("c1000","",200,10,800,500);	
	  TCanvas *c1000= new TCanvas("c1000","",200,10,960,600);	

	string savename2pdf = savename + "_QCDplusTTBAR.pdf";
	string savename2png = savename + "_QCDplusTTBAR.png";
	string savename2eps = savename + "_QCDplusTTBAR.eps";
	
	string savename3pdf = savename + "_QCDplusTTBAR_LOG.pdf";
	string savename3png = savename + "_QCDplusTTBAR_LOG.png";
	string savename3eps = savename + "_QCDplusTTBAR_LOG.eps";
	
	TH1D *h_BKGD = h_QCD->Clone();
	h_BKGD->Add( h_QCD, h_TTBAR,1,1);
	
	
	TH1D *h_QCD_noErrors = h_QCD->Clone();
	TH1D *h_BKGD_noErrors = h_BKGD->Clone();

	for (int i=1; i< h_QCD->GetNbinsX(); i++)
	{
	
		h_BKGD_noErrors->SetBinContent( i, h_BKGD->GetBinContent(i) );
		h_BKGD_noErrors->SetBinError( i, 0 );
	
		h_QCD_noErrors->SetBinContent(i,h_QCD->GetBinContent(i) );
		h_QCD_noErrors->SetBinError(i,0);
	/*	
		double trig_eff_error = 0;
		if (type==11) trig_eff_error =  0.0015;   // +0.10 -0.15%
		if (type==12) trig_eff_error =  0.0025;   // +0.10 -0.15%
		
		double JEC_uncer = 0.06; 

    	double tagging_eff_scale_factor_error = h_TTBAR->GetBinContent(i) * 0.13;	
	    double ttbar_lumi_error         = h_TTBAR->GetBinContent(i) * 0.06;
		double ttbar_xsec_error         = h_TTBAR->GetBinContent(i) * 0.1;
		double bkgd_stat_error 		= h_BKGD->GetBinError(i);
		double bkgd_mistag_error 	= fabs( h_QCD->GetBinContent(i) - ( h_QCD->GetBinContent(i) + h_QCD2->GetBinContent(i))/2 );
		double bkgd_JEC_error 		= h_TTBAR->GetBinContent(i) * JEC_uncer;
		double bkgd_trigger_error 	= h_TTBAR->GetBinContent(i) * trig_eff_error;
		double bkgd_error 			= sqrt( pow( bkgd_stat_error, 2) + pow( bkgd_mistag_error, 2)  + pow( bkgd_JEC_error, 2) 
		                                  + pow( tagging_eff_scale_factor_error , 2) + pow( tagging_eff_scale_factor_error , 2) 
		                                  + pow( ttbar_lumi_error, 2) + pow( bkgd_trigger_error, 2) 
		                                  + pow( ttbar_xsec_error, 2) );
	*/	
		
		
		//----------------------------------------------------------------------------------
					
        // trigger
		
		double trig_ttbar_eff =0;
        double trig_ttbar_eff_error =0;
        double trig_ttbar_eff_relative_error =0;
        
        cout << "weighted count  =  " << h_TTBAR->GetBinContent(i)   << endl;
        cout << "unweighted count =  " << h_TTBAR2->GetBinContent(i)   << endl;
        
        if (h_TTBAR2->GetBinContent(i)!=0) trig_ttbar_eff = h_TTBAR->GetBinContent(i)/h_TTBAR2->GetBinContent(i) ;
        trig_ttbar_eff_error = fabs(1-trig_ttbar_eff)/2;
        if (trig_ttbar_eff!=0) trig_ttbar_eff_relative_error = trig_ttbar_eff_error / trig_ttbar_eff;

        cout<<"NEW trig_ttbar_eff                  = "<<trig_ttbar_eff<<endl;
        cout<<"NEW trig_ttbar_eff_error            = "<<trig_ttbar_eff_error<<endl;
        cout<<"NEW trig_ttbar_eff_relative_error   = "<<trig_ttbar_eff_relative_error<<endl;
		
		//other ttbar errors
		//double tagging_eff_scale_factor =  0.93; 
		//double tagging_eff_scale_factor_error = 0.13;	
		double tagging_eff_scale_factor_relative_error = tagging_eff_scale_factor_error / tagging_eff_scale_factor;		

        double ttbar_stat_error  = 0;
	    if (h_TTBAR->GetBinContent(i)!=0) ttbar_stat_error 		= sqrt(  h_TTBAR->GetBinError(i) ) /  h_TTBAR->GetBinContent(i);
		double ttbar_lumi_error         = 0.06;
		double ttbar_xsec_error         = 0.1;
		double ttbar_jec_error          = 0.06; 

		// total ttbar error
		double total_ttbar_relative_error	=   sqrt(  pow( ttbar_stat_error, 2)
		                                             + pow( trig_ttbar_eff_relative_error, 2)
		                                             + pow( 2*tagging_eff_scale_factor_relative_error, 2) 
		                                             + pow( ttbar_jec_error, 2)
		                                             + pow( ttbar_lumi_error, 2)
		                                             + pow( ttbar_xsec_error, 2)
		                                             );
		
		// qcd errors
		double qcd_stat_error = 0;
 		if (h_QCD->GetBinContent(i)!=0)  		= sqrt( h_QCD->GetBinError(i) ) / h_QCD->GetBinContent(i);
 		double qcd_mistag_error =0;
 		if (h_QCD->GetBinContent(i)!=0) qcd_mistag_error 	= fabs(1-h_QCD2->GetBinContent(i)/h_QCD->GetBinContent(i));  
 		//cout<<" fabs( count_qcd - (count_qcd+count_qcd2)/2 ) /count_qcd  = "<<fabs( count_qcd - (count_qcd+count_qcd2)/2 ) /count_qcd <<endl;;
 		double total_qcd_relative_error		= sqrt( pow( qcd_stat_error, 2)    + pow( qcd_mistag_error, 2)  );

			
		// total bkg errors
		double count_bkgd = h_QCD->GetBinContent(i) + h_TTBAR->GetBinContent(i);
		double bkgd_error 	= sqrt( pow( total_ttbar_relative_error*h_TTBAR->GetBinContent(i), 2) + pow( total_qcd_relative_error*h_QCD->GetBinContent(i), 2)  );
		
		cout<<"  & Relative errors in %: " << endl;
		cout<<"  &  ttbar stat            "<<100*ttbar_stat_error<<endl;
		cout<<"  &  ttbar trig eff        "<<100*trig_ttbar_eff_relative_error<<endl;
		cout<<"  &  ttbar tagging eff     "<<100*2*tagging_eff_scale_factor_relative_error<<endl;
		cout<<"  &  ttbar jec             "<<100*ttbar_jec_error<<endl;
		cout<<"  &  ttbar lumi            "<<100*ttbar_lumi_error<<endl;
		cout<<"  &  ttbar xsection        "<<100*ttbar_xsec_error<<endl;
		cout<<"  &  ttbar TOTAL           "<<100*total_ttbar_relative_error<<endl;
	    cout<<"  & ______________________________ " << endl;
        cout<<"  &  qcd mistag            "<<100*qcd_mistag_error<<endl;
        cout<<"  &  qcd stat              "<<100*qcd_stat_error<<endl;
        cout<<"  &  qcd TOTAL             "<<100*total_qcd_relative_error<<endl;
        cout<<"  & ______________________________ " << endl;
        cout<<fixed << setprecision(1);
        cout<<"  &  bkgd TOTAL            "<<bkgd_error <<endl;
        cout<<"  & ______________________________ " << endl;
		
		
		
		//----------------------------------------------------------------------------------
		
		
		h_BKGD->SetBinError(i,bkgd_error);
		
	}
	
	h_DATA				->SetTitle(title.c_str());
	h_DATA				->SetTitleOffset( YaxisTitleOffset,"Y");
	h_DATA				->SetTitleOffset( 0.98,"X.");

	h_DATA				->Rebin(rebin);
	h_QCD 				->Rebin(rebin);
	h_TTBAR 			->Rebin(rebin);
	h_BKGD 				->Rebin(rebin);
	h_QCD_noErrors		->Rebin(rebin);
	h_BKGD_noErrors		->Rebin(rebin);
	
	if ( lowerbound!=-1 && upperbound!=-1) h_DATA				->GetXaxis()->SetRangeUser(lowerbound,upperbound);
	

	//--------------------------------------------------------------------
	h_DATA	->SetMarkerStyle(20);	
	h_DATA	->SetLineWidth(2);
	
	h_BKGD->SetMarkerSize(0);
	h_BKGD->SetLineColor(kRed);
	h_BKGD->SetLineWidth(0);
	h_BKGD->SetFillColor((13)); //16
	//h_BKGD->SetFillStyle(3654); //3XYZ. X=(1-9) space between hatch, Y=(0-9) angle between 0 and 90 degrees (5 = Not drawn). Z= opposite line
	//h_BKGD->SetFillStyle(3254); //3XYZ. X=(1-9) space between hatch, Y=(0-9) angle between 0 and 90 degrees (5 = Not drawn). Z= opposite line
	h_BKGD->SetFillStyle(3154);
	
	h_BKGD_noErrors->SetLineColor(1);//kRed);
	h_BKGD_noErrors->SetFillColor((kRed)); //old way->Delete this
	h_BKGD_noErrors->SetLineWidth(1); //3
	
	h_QCD_noErrors->SetLineWidth(1);//2
	h_QCD_noErrors->SetLineColor(1);//kYellow); //1
	h_QCD_noErrors->SetFillColor(kYellow);
	
	h_DATA->Draw("PE");
	h_BKGD_noErrors->Draw("same");
	h_QCD_noErrors->Draw("same");
	h_BKGD->Draw("9E2same");
	h_DATA->Draw("PEsame");

	leg = new TLegend(legXmin,legYmin,legXmax,legYmax);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	//leg->SetTextFont(42);
	if (legend_header!="none") leg->SetHeader( legend_header.c_str() );
	leg->AddEntry(	h_DATA,				L1.c_str(),"LP");
	leg->AddEntry(	h_QCD_noErrors,		L2.c_str(),"F");
	leg->AddEntry(	h_BKGD_noErrors,	L3.c_str(),"F");
	leg->Draw("same");

	//TPaveText *textbox = new TPaveText(legXmin-0.01,legYmin-0.11,legXmax-0.05,legYmin-0.01,"NDC");
	TPaveText *textbox1 = new TPaveText(0.35,0.85,0.85,0.92,"NDC");
	textbox1->SetFillColor(0);
	textbox1->SetLineColor(0);
	TText *line1 = textbox1->AddText("CMS Preliminary, 1047 pb^{-1} at #sqrt{s} = 7 TeV");
	line1->SetTextColor(1);
	line1->SetTextAlign(12); //first number = horizontal alignment (1 left, 2 center, 3 right). second number =vertical alignment (1 top, 2 center, 3 bottom)
	//textbox1->SetTextFont(42);
	textbox1->Draw("same");
	
	//TPaveText *textbox = new TPaveText(legXmin-0.02,legYmin-0.10,legXmax-0.06,legYmin-0.01,"NDC");
	//TText *line2 = textbox->AddText("886 pb^{-1} at #sqrt{s} = 7 TeV");
	//textbox->SetFillColor(0);
	//textbox->SetLineColor(0);
	//line2->SetTextColor(1);
	//line2->SetTextAlign(12); //first number = horizontal alignment (1 left, 2 center, 3 right). second number =vertical alignment (1 top, 2 center, 3 bottom)
	//textbox->SetTextFont(42);
	//textbox->Draw("same");
	
	gPad->RedrawAxis();

	c1000->SaveAs(savename2pdf.c_str());
	c1000->SaveAs(savename2png.c_str());
	c1000->SaveAs(savename2eps.c_str());
	
	c1000->SetLogy();
	c1000->SaveAs(savename3pdf.c_str());
	c1000->SaveAs(savename3png.c_str());
	c1000->SaveAs(savename3eps.c_str());
    c1000->SetLogy(0);
	//--------------------------------------------------------------------

	delete c1000;
	rebin = -1;	
}


void CompareHists(
	TH1D * h1,
	TH1D * h2,
	std::string title,
	std::string legend_header,
	std::string L1, 
	std::string L2, 
	double legXmin, double legYmin, double legXmax, double legYmax, 
	double YaxisTitleOffset, 
	std::string savename, 
	int rebin, 
	double lowerbound, 
	double upperbound,
	string draw_style1,
	string draw_style2,
	string text1,  //"CMS Preliminary"
	string text2,
	int color1,
	int color2)  //"886 pb^{-1} at #sqrt{s} = 7 TeV"
{
	TCanvas *c1000= new TCanvas("c1000","",200,10,960,600);	
		
	h1->SetLineColor(color1);
	h2->SetLineColor(color2);
	
	h1->SetLineWidth(2);
	h2->SetLineWidth(2);
	
	h1->SetMarkerColor(color1);
	h2->SetMarkerColor(color2);
	
	h1->SetMarkerStyle(20);
	h2->SetMarkerStyle(21);
	
	string savename2pdf = savename + ".pdf";
	string savename2png = savename + ".png";
	string savename2eps = savename + ".eps";
	
	h1				->SetTitle(title.c_str());
	h2				->SetTitle(title.c_str());
	//h1				->SetTitleOffset( YaxisTitleOffset,"Y");
	//h2				->SetTitleOffset( YaxisTitleOffset,"Y");
	h1				->SetTitleOffset( 0.9,"Y");

	h1				->Rebin(rebin);
	h2 				->Rebin(rebin);

	
	if ( lowerbound!=-1 && upperbound!=-1) h1				->GetXaxis()->SetRangeUser(lowerbound,upperbound);
	if ( lowerbound!=-1 && upperbound!=-1) h2				->GetXaxis()->SetRangeUser(lowerbound,upperbound);
	
	
	string draw_style2_use = draw_style2+"same";
	
	h1->Draw(draw_style1.c_str());
	h2->Draw(draw_style2_use.c_str());
	

	leg = new TLegend(legXmin,legYmin,legXmax,legYmax);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	if (legend_header!="none") leg->SetHeader( legend_header.c_str() );
	leg->AddEntry(	h1,				L1.c_str(),"PL");
	leg->AddEntry(	h2,		L2.c_str(),"PL");
	leg->Draw("same");

	//TPaveText *textbox = new TPaveText(legXmin-0.01,legYmin-0.11,legXmax-0.05,legYmin-0.01,"NDC");
	//TPaveText *textbox = new TPaveText(legXmin-0.02,legYmin-0.10,legXmax-0.06,legYmin-0.01,"NDC");
	TPaveText *textbox = new TPaveText(0.6,0.85,0.85,0.92,"NDC");
	textbox->SetFillColor(0);
	textbox->SetLineColor(0);
	TText *line1 = textbox->AddText(text1.c_str());
	line1->SetTextColor(1);
	line1->SetTextAlign(12); //first number = horizontal alignment (1 left, 2 center, 3 right). second number =vertical alignment (1 top, 2 center, 3 bottom)
	//TText *line2 = textbox->AddText(text2.c_str());
	//line2->SetTextColor(1);
	//line2->SetTextAlign(12); //first number = horizontal alignment (1 left, 2 center, 3 right). second number =vertical alignment (1 top, 2 center, 3 bottom)
	textbox->Draw("same");
	
	gPad->RedrawAxis();

	c1000->SaveAs(savename2pdf.c_str());
	c1000->SaveAs(savename2png.c_str());
	c1000->SaveAs(savename2eps.c_str());
	
	

	//--------------------------------------------------------------------

	delete c1000;
	//rebin = -1;	
}




void compareQCD(){
	//static const int n_signal_bins = 24;
	
	TFile *ROOT_DATA 					= new TFile( "TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_range1_range2_v4_type11_.root" );
	TFile *ROOT_TT_TuneZ2 				= new TFile( "TTHadronicAnalyzerCombined_TTJets_TuneZ2_type11_.root" );		
	TFile *ROOT_QCD 				    = new TFile( "TTHadronicAnalyzerCombined_QCD_Pt-15to3000_TuneZ2_Flat_QCDMistag_type11_.root");		
	
	TH1D * mttMass_Data							=  ROOT_DATA					-> Get("mttMass");
	TH1D * mttMass_TT_TuneZ2					=  ROOT_TT_TuneZ2			 	-> Get("mttMass");
	
	TH1D * mttMass_Predicted					=  ROOT_DATA					-> Get("mttPredDist_pred");
	TH1D * mttMass_Predicted_MassCutMistag		=  ROOT_DATA					-> Get("mttPredDistMassCut_pred");
	TH1D * mttMass_Predicted_ModMassFlat		=  ROOT_DATA					-> Get("mttPredDistModMassFlat_pred");
	TH1D * mttMass_Predicted_Mod3MassFlat		=  ROOT_DATA					-> Get("mttPredDistMod3MassFlat_pred");
		
	TH1D * mttMass_QCD_measured					=  ROOT_QCD					-> Get("mttMass");
	TH1D * mttMass_QCD_predicted				=  ROOT_QCD		    		-> Get("mttPredDistMod3MassFlat_pred");
	
	
	mttMass_QCD_measured  ->Sumw2();
	mttMass_QCD_predicted ->Sumw2();
	double scale1 = mttMass_Predicted_Mod3MassFlat->Integral() / mttMass_QCD_predicted->Integral() ;
	double scale2 = mttMass_Predicted_Mod3MassFlat->Integral() / mttMass_QCD_measured->Integral() ;
    cout<<"scale1 "<<scale1<<endl;
    cout<<"scale2 "<<scale2<<endl;

	mttMass_QCD_measured  ->Scale(scale1);
	mttMass_QCD_predicted ->Scale(scale2);
	cout<<"mttMass_Predicted_Mod3MassFlat->Integral() " << mttMass_Predicted_Mod3MassFlat->Integral()<<endl;
	cout<<"mttMass_QCD_predicted->Integral() " << mttMass_QCD_predicted->Integral()<<endl;
	//mttMass_QCD_measured   ->Scale( mttMass_Predicted_Mod3MassFlat->Integral() / mttMass_QCD_measured->Integral() );
	//mttMass_QCD_predicted  ->Scale( mttMass_Predicted_Mod3MassFlat->Integral() / mttMass_QCD_predicted->Integral() );
	
	mttMass_QCD_measured->Rebin(20);
	mttMass_QCD_predicted->Rebin(20);
	mttMass_Predicted_Mod3MassFlat->Rebin(20);
    
    for (int i=1; i< mttMass_QCD_measured->GetNbinsX(); i++){
    mttMass_QCD_measured->SetBinError( i,  sqrt( mttMass_QCD_measured->GetBinContent(i) ) );
    }
    
    
    
    
    
    CompareHists(
    mttMass_QCD_predicted,
	mttMass_QCD_measured,
	";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV",
	"none",
	"#splitline{QCD Monte Carlo}{predicted background}", 
	"#splitline{QCD Monte Carlo}{measured}", 
	0.45, 0.55, 0.82,0.81, 
	1.4, 															// Y axis label offset
	"ttMassType11_QCD_MCcompare_MCmistag", 						// savename
	1, 															// rebin
	500, 															// X axis lowerbound
	5000,                                                   // Y axis lowerbound
	"EP",
	"EP",
	"CMS Simulation",
	"",
	2,
	4
	);			
	
	 CompareHists(
	mttMass_QCD_measured,
	mttMass_Predicted_Mod3MassFlat,
	";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV",
	"none",
	"QCD MC measured", 
	"Data predicted", 
	0.45, 0.65, 0.82,0.81, 
	1.4, 															// Y axis label offset
	"ttMassType11_QCD_DataMCmeasured", 						// savename
	1, 															// rebin
	500, 															// X axis lowerbound
	5000,                                                   // Y axis lowerbound
	"HIST",
	"HIST",
	"",
	"",
	2,
	1
	);			
    
     CompareHists(
	mttMass_QCD_predicted,
	mttMass_Predicted_Mod3MassFlat,
	";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV",
	"none",
	"QCD MC predicted", 
	"Data predicted", 
	0.45, 0.65, 0.82,0.81, 
	1.4, 															// Y axis label offset
	"ttMassType11_QCD_DataMCpredicted", 						// savename
	1, 															// rebin
	500, 															// X axis lowerbound
	5000,                                                   // Y axis lowerbound
	"HIST",
	"HIST",
	"",
	"",
	4,
	1
	);	
	/*
	MakeHistogram3(
		mttMass_Data, 							// data histogram
		mttMass_TT_TuneZ2, 						// ttbar histogram
		mttMass_Predicted_Mod3MassFlat, 		// qcd histogram
		mttMass_Predicted,						// alternate qcd estimate histogram - used for systematics
		";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV", 	// histogram title
		"none",//"JetPD - 270_v1, 300, 370 - pT>350",					// legend_header
		"Data", 														// legend label 1
		"#splitline{QCD estimation - }{Mass Modified Flat(140,250)}", 	// legend label 2
		"t#bar{t} Madgraph + Pythia Tune Z2 ", 							// legend label 3
		0.48, 0.72, 0.92,0.91, 											// legend coordinates
		1.4, 															// Y axis label offset
		"ttMassType11_mttPredDist_Mod3MassFlat", 						// savename
		20, 															// rebin
		500, 															// X axis lowerbound
		5000,															// Y axis lowerbound
		11 );       													// type 11 or type 22
		*/
																	
	}

void checkSubtract(){
	//static const int n_signal_bins = 24;
	
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
	//gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadTopMargin(0.05); //0.25
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
	gStyle->SetTitleX(0.2); //0.02 
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
    /////////////////////////////////////////////////////////////////////////////////////////
	
	

	string file_string_data  = "TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_range1_range2_v4_Subtract_type11_.root";
	

	TFile *ROOT_DATA 					= new TFile( file_string_data.c_str() );
	
	

	TH1D * mttMass_Predicted_Mod3MassFlat		    =  ROOT_DATA					-> Get("mttPredDistMod3MassFlat_pred");
	TH1D * mttMass_Predicted_Mod3MassFlatSubtract		=  ROOT_DATA					-> Get("mttPredDistMod3MassFlatSubtract_pred");
		
	
    
    CompareHists(
	mttMass_Predicted_Mod3MassFlat,
	mttMass_Predicted_Mod3MassFlatSubtract,
	";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV",
	"none",
	"Data Mistag", 
	"Subtract ttbar MC", 
	0.48, 0.72, 0.92,0.91, 
	1.4, 															// Y axis label offset
	"ttMassType11_SubtractTTbarMistag", 						// savename
	20, 															// rebin
	500, 															// X axis lowerbound
	5000,                                                   // Y axis lowerbound
	"PE",
	"PE",
	"",
	""
	);															
    
	/*
	MakeHistogram3(
		mttMass_Data, 							// data histogram
		mttMass_TT_TuneZ2, 						// ttbar histogram
		mttMass_Predicted_Mod3MassFlat, 		// qcd histogram
		mttMass_Predicted,						// alternate qcd estimate histogram - used for systematics
		";Type 1+1 t#bar{t} mass (GeV/c^{2}); # of Events / 100 GeV", 	// histogram title
		"none",//"JetPD - 270_v1, 300, 370 - pT>350",					// legend_header
		"Data", 														// legend label 1
		"#splitline{QCD estimation - }{Mass Modified Flat(140,250)}", 	// legend label 2
		"t#bar{t} Madgraph + Pythia Tune Z2 ", 							// legend label 3
		0.48, 0.72, 0.92,0.91, 											// legend coordinates
		1.4, 															// Y axis label offset
		"ttMassType11_mttPredDist_Mod3MassFlat", 						// savename
		20, 															// rebin
		500, 															// X axis lowerbound
		5000,															// Y axis lowerbound
		11 );       													// type 11 or type 22
		*/
																	
	}
#if 0
  // Construct a custom configuration which uses the adaptive Gauss-Kronrod technique
  // for closed 1D integrals
  RooNumIntConfig customConfig(*RooAbsReal::defaultIntegratorConfig()) ;
  customConfig.method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D") ;
  customConfig.setEpsAbs(1E-6);
  customConfig.setEpsRel(1E-6);
  ipdf.setIntegratorConfig(customConfig);
#endif
