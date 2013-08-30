{
	TCanvas *c1000= new TCanvas("c1000","",200,10,960,600);

	TFile *ROOT 			= new TFile("JAN15/data_mistag_Feb27.root");
	TFile *Signal			= new TFile("JAN15/ZprimeNrw_2000_sec0_mistag_TriggerNominal.root");
	TFile *ROOT_TTBAR1 		= new TFile("JAN15/ttjets_m700_mistag_cleanup.root");
	TFile *ROOT_TTBAR2 		= new TFile("JAN15/ttjets_m1000_mistag_cleanup.root");

   /* TH1D * topBigBinTagPt		    =  ROOT	-> Get("jet1PtTag");
	TH1D * topBigBinProbePt		    =  ROOT -> Get("jet1PtProbe");

	TH1D * topBigBinTagPtTTBAR		=  ROOT_TTBAR -> Get("jet1PtTag");
	TH1D * topBigBinProbePtTTBAR	=  ROOT_TTBAR -> Get("jet1PtProbe");	
*/
        TH1D * topTagPt			        =  ROOT	-> Get("jet1PtTag_mu");
	TH1D * topProbePt		        =  ROOT -> Get("jet1PtProbe_mu");

	TH1D * signalTagPt				= Signal->Get("jet1PtTag_mu");
	TH1D * signalProbePt				= Signal->Get("jet1PtProbe_mu");


	signalTagPt->Scale(0.08*19600./90000.);
	signalProbePt->Scale(0.08*19600./90000.);



	TH1D * topTagPtTTBAR		    =  ROOT_TTBAR1 -> Get("jet1PtTag_mu");
	TH1D * topProbePtTTBAR		    =  ROOT_TTBAR1 -> Get("jet1PtProbe_mu");	
	
	TH1D * topTagPtTTBAR2		    =  ROOT_TTBAR2 -> Get("jet1PtTag_mu");
	TH1D * topProbePtTTBAR2		    =  ROOT_TTBAR2 -> Get("jet1PtProbe_mu");	
	// Remove TTBAR contamination from data driven QCD estimate
	double luminosity = (19598.);  
	double DataSetNevents_TT1_TuneZ2  = 3082812.;
	double DataSetNevents_TT2_TuneZ2  = 1249111.;
	double sigma_TT_TuneZ2   =  234;
	double scale_TT_TuneZ2 = sigma_TT_TuneZ2 * luminosity;
        double tagging_eff_scale_factor = 0.926;
	
	topTagPtTTBAR	    ->Sumw2();
	topTagPtTTBAR	    ->Scale(scale_TT_TuneZ2*0.074/DataSetNevents_TT1_TuneZ2);
	topTagPtTTBAR	    ->Scale(tagging_eff_scale_factor);
	topProbePtTTBAR	    ->Sumw2();
	topProbePtTTBAR	    ->Scale(scale_TT_TuneZ2*0.074/DataSetNevents_TT1_TuneZ2);
	topProbePtTTBAR	    ->Scale(tagging_eff_scale_factor);

	topTagPtTTBAR2	    ->Sumw2();
	topTagPtTTBAR2	    ->Scale(scale_TT_TuneZ2*0.014/DataSetNevents_TT2_TuneZ2);
	topTagPtTTBAR2	    ->Scale(tagging_eff_scale_factor);
	topProbePtTTBAR2    ->Sumw2();
	topProbePtTTBAR2    ->Scale(scale_TT_TuneZ2*0.014/DataSetNevents_TT2_TuneZ2);
	topProbePtTTBAR2    ->Scale(tagging_eff_scale_factor);

	topTagPtTTBAR->Add(topTagPtTTBAR2);
	topProbePtTTBAR->Add(topProbePtTTBAR2);

	cout << "ttbar tag " << topTagPtTTBAR->Integral() << " " << topTagPtTTBAR2->Integral() << endl;
	cout << "ttbar probe " << topProbePtTTBAR->Integral() << " " << topProbePtTTBAR2->Integral() << endl;
	
	TH1D *topTagPtSubtractTTBAR         = topTagPt->Clone();
	TH1D *topProbePtSubtractTTBAR       = topProbePt->Clone();

	TH1D *topTagPtNoSubtractTTBAR         = topTagPt->Clone();
	TH1D *topProbePtNoSubtractTTBAR       = topProbePt->Clone();



        topTagPtSubtractTTBAR->Draw("hist");
        topTagPtTTBAR->Draw("hist same");

	
	topTagPtSubtractTTBAR       ->Add( topTagPtTTBAR,-1);
	topProbePtSubtractTTBAR     ->Add( topProbePtTTBAR,-1);

	//topTagPtSubtractTTBAR       ->Add(signalTagPt);
	//topProbePtSubtractTTBAR     ->Add(signalProbePt);


		// Rebin Histograms - Variable width bins
 	Int_t num_bins=29;
        // Double_t bins[num_bins+1]= {350,355,360,365,370,375,380,385,390,395,400,410,420,430,440,450,460,480,500,525,550,600,650,700,800,2000}; 
        //Double_t bins[num_bins+1] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 1000, 2000};
        Double_t bins[num_bins+1] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 650, 700, 750, 800, 900, 1000, 1100, 1250, 1500, 2000};
        //Double_t bins[num_bins+1] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 700, 800, 2000};


//	Int_t num_bins=7;
//	Double_t bins[num_bins+1] = {400, 500, 600, 700, 800, 900, 1000, 2000 };


	TH1D *topTagPtSubtractTTBAR_rebin 		= topTagPtSubtractTTBAR		->Rebin(num_bins,"topTagPtSubtractTTBAR_rebin",bins); 
	TH1D *topProbePtSubtractTTBAR_rebin 	= topProbePtSubtractTTBAR	->Rebin(num_bins,"topProbePtSubtractTTBAR_rebin",bins); 

	
	TH1D *topTagPtNoSubtractTTBAR_rebin 		= topTagPtNoSubtractTTBAR		->Rebin(num_bins,"topTagPtNoSubtractTTBAR_rebin",bins); 
	TH1D *topProbePtNoSubtractTTBAR_rebin 	= topProbePtNoSubtractTTBAR	->Rebin(num_bins,"topProbePtNoSubtractTTBAR_rebin",bins); 
	
    // Mistag Plot
	TH1D *MISTAG_RATE_SubtractTTBAR = topProbePtSubtractTTBAR_rebin->Clone();
	TH1D *MISTAG_RATE_NoSubtractTTBAR = topProbePtNoSubtractTTBAR_rebin->Clone();
	MISTAG_RATE_SubtractTTBAR->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
	MISTAG_RATE_NoSubtractTTBAR->SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate");
	MISTAG_RATE_SubtractTTBAR->Sumw2();
	MISTAG_RATE_NoSubtractTTBAR->Sumw2();
	MISTAG_RATE_SubtractTTBAR->Divide(topTagPtSubtractTTBAR_rebin,topProbePtSubtractTTBAR_rebin,1.,1.,"B");
	MISTAG_RATE_NoSubtractTTBAR->Divide(topTagPtNoSubtractTTBAR_rebin,topProbePtNoSubtractTTBAR_rebin,1.,1.,"B");
	MISTAG_RATE_SubtractTTBAR->SetMarkerStyle(20);
	MISTAG_RATE_SubtractTTBAR->SetMarkerColor(1);
	MISTAG_RATE_SubtractTTBAR->SetLineColor(1);
	MISTAG_RATE_NoSubtractTTBAR->SetMarkerStyle(20);
	MISTAG_RATE_NoSubtractTTBAR->SetMarkerColor(kRed);
	MISTAG_RATE_NoSubtractTTBAR->SetLineColor(kRed);
	
	
	MISTAG_RATE_SubtractTTBAR->Draw();
	//MISTAG_RATE_NoSubtractTTBAR->Draw("same");
	MISTAG_RATE_SubtractTTBAR   ->SetName("MISTAG_MU_REVERSE_SUB_TTBAR");
    	c1000->SetLogx(1);
	MISTAG_RATE_SubtractTTBAR->GetXaxis()->SetMoreLogLabels(1);
	MISTAG_RATE_SubtractTTBAR->GetXaxis()->SetNoExponent(1);
	prelim = TLatex();
    	prelim.SetNDC();
        prelim.DrawLatex( 0.5, 0.91, "#scale[0.8]{CMS Preliminary, 8 TeV, 19.6 fb^{-1}}" );
        //prelim.DrawLatex( 0.5, 0.91, "#scale[0.8]{Mistag Rate from QCD MC}" );
    
        c1000->SaveAs("mistag_plot_PAS.pdf");


  
    	TFile *Out;
	Out = new TFile("mistag_type12_rebin.root","RECREATE");  //MISTAG_4p7fb
	Out->cd();
	
	MISTAG_RATE_SubtractTTBAR->Write();
	
	Out->ls();      
	Out->Write();
  
}
	

