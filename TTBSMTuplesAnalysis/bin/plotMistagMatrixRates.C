{

TFile *mistag_file = new TFile("QCD_Feb10.root");

float tauBins[10] = { 0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 2.0 };
float csvBins[5] = { -1.0, 0.0, 0.244, 0.679, 1.0  };


for (int tauBin = 0; tauBin < 9; tauBin++){

	TH1F *csvNegH = (TH1F *) mistag_file->Get(Form("0_tagPt_0_%d", tauBin));
	TH1F *csvLowH = (TH1F *) mistag_file->Get(Form("0_tagPt_1_%d", tauBin));
	TH1F *csvMedH = (TH1F *) mistag_file->Get(Form("0_tagPt_2_%d", tauBin));
	TH1F *csvHiH = (TH1F *) mistag_file->Get(Form("0_tagPt_3_%d", tauBin));

	csvNegH->SetMarkerSize(0);
	csvLowH->SetMarkerSize(0);
	csvMedH->SetMarkerSize(0);
	csvHiH->SetMarkerSize(0);

	csvNegH->SetMarkerColor(kViolet);
	csvLowH->SetMarkerColor(kRed);
	csvMedH->SetMarkerColor(kBlue);
	csvHiH->SetMarkerColor(kGreen - 1);
	csvNegH->SetLineColor(kViolet);
	csvLowH->SetLineColor(kRed);
	csvMedH->SetLineColor(kBlue);
	csvHiH->SetLineColor(kGreen - 1);

	csvNegH->SetMinimum(0.001);
	csvNegH->SetMaximum(1.0);
	csvNegH->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
	csvNegH->GetYaxis()->SetTitle("Top Mistag Rate");


	csvNegH->Draw();
	csvLowH->Draw("same");
	csvMedH->Draw("same");
	csvHiH->Draw("same");

	TLatex *cmsLabel = new TLatex();
	cmsLabel.SetNDC();
	cmsLabel.DrawLatex(0.1, 0.93, "QCD MC");
	cmsLabel.DrawLatex(0.7,0.93, Form("%1.1f < #tau_{3}/#tau_{2} < %1.1f", tauBins[tauBin], tauBins[tauBin+1]));

	TLegend *leg = new TLegend(0.6,0.7, 0.89, 0.89);
	leg->SetNColumns(2);
	leg->AddEntry(csvHiH, "CSVM", "l");
	leg->AddEntry(csvMedH, "CSVL", "l");
	leg->AddEntry(csvLowH, "CSV0", "l");
	leg->AddEntry(csvNegH, "CSVNeg", "l");
	leg->SetFillColor(0);
	leg->SetLineColor(0);
	leg->Draw("same");





	c1->SetLogy(1);

	c1->SaveAs(Form("QCDmistagRate_tauBin%d.pdf", tauBin));



}


for (int csvBin = 0; csvBin < 4; csvBin++){


	TH1F *nsj1H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_0", csvBin));
	TH1F *nsj2H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_1", csvBin));
	TH1F *nsj3H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_2", csvBin));
	TH1F *nsj4H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_3", csvBin));
	TH1F *nsj5H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_4", csvBin));
	TH1F *nsj6H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_5", csvBin));
	TH1F *nsj7H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_6", csvBin));
	TH1F *nsj8H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_7", csvBin));
	TH1F *nsj9H = (TH1F *) mistag_file->Get(Form("0_tagPt_%d_8", csvBin));

	nsj1H->SetMarkerSize(0);	
	nsj2H->SetMarkerSize(0);	
	nsj3H->SetMarkerSize(0);	
	nsj4H->SetMarkerSize(0);	
	nsj5H->SetMarkerSize(0);	
	nsj6H->SetMarkerSize(0);	
	nsj7H->SetMarkerSize(0);	
	nsj8H->SetMarkerSize(0);	
	nsj9H->SetMarkerSize(0);	

	nsj1H->SetMarkerColor(1);
	nsj2H->SetMarkerColor(2);
	nsj3H->SetMarkerColor(3);
	nsj4H->SetMarkerColor(4);
	nsj5H->SetMarkerColor(5);
	nsj6H->SetMarkerColor(6);
	nsj7H->SetMarkerColor(7);
	nsj8H->SetMarkerColor(8);
	nsj9H->SetMarkerColor(9);
	nsj1H->SetLineColor(1);
	nsj2H->SetLineColor(2);
	nsj3H->SetLineColor(3);
	nsj4H->SetLineColor(4);
	nsj5H->SetLineColor(5);
	nsj6H->SetLineColor(6);
	nsj7H->SetLineColor(7);
	nsj8H->SetLineColor(8);
	nsj9H->SetLineColor(9);

	nsj1H->SetMinimum(0.001);
	nsj1H->SetMaximum(1.0);
	
	nsj1H->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
	nsj1H->GetYaxis()->SetTitle("Top Mistag Rate");

	nsj1H->Draw();
	nsj2H->Draw("same");	
	nsj3H->Draw("same");	
	nsj4H->Draw("same");	
	nsj5H->Draw("same");	
	nsj6H->Draw("same");	
	nsj7H->Draw("same");	
	nsj8H->Draw("same");	
	nsj9H->Draw("same");	

	TLatex *cmsLabel = new TLatex();
	cmsLabel.SetNDC();
	cmsLabel.DrawLatex(0.1, 0.93, "QCD MC");
	cmsLabel.DrawLatex(0.65,0.93, Form("%1.3f < CSV < %1.3f", csvBins[csvBin], csvBins[csvBin + 1]));

	TLegend *leg2 = new TLegend(0.6,0.5, 0.89, 0.89);
	leg2->SetNColumns(1);
	leg2->AddEntry(nsj1H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[0], tauBins[1]), "l");
	leg2->AddEntry(nsj2H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[1], tauBins[2]), "l");
	leg2->AddEntry(nsj3H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[2], tauBins[3]), "l");
	leg2->AddEntry(nsj4H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[3], tauBins[4]), "l");
	leg2->AddEntry(nsj5H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[4], tauBins[5]), "l");
	leg2->AddEntry(nsj6H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[5], tauBins[6]), "l");
	leg2->AddEntry(nsj7H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[6], tauBins[7]), "l");
	leg2->AddEntry(nsj8H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[7], tauBins[8]), "l");
	leg2->AddEntry(nsj9H, Form("%1.1f < #tau_{32} < %1.1f", tauBins[8], tauBins[9]), "l");
	leg2->SetFillColor(0);
	leg2->SetLineColor(0);
	leg2->Draw("same");


	c1->SetLogy(1);

	c1->SaveAs(Form("QCDmistagRate_csvBin%d.pdf", csvBin));




}


}
