{

gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);


TChain *data = new TChain("treeVars");
TChain *ttbar10 = new TChain("treeVars");
TChain *ttbar7 = new TChain("treeVars");
TChain *qcd = new TChain("treeVars");
TChain *signal = new TChain("treeVars");


data->Add("Aug19_dataD.root");
ttbar10->Add("Aug19_ttjets10.root");
ttbar7->Add("Aug19_ttjets7.root");
qcd->Add("Aug19_dataD.root");
signal->Add("Aug19_Zprime.root");

int nVar = 26;

string vars[nVar] = {	
			"mttMass",
			"jet1Pt",
			"jet2Pt",
			"sumJetPt",
			"jet1Mass",
			"jet2Mass",
			"jet1Eta",
			"jet2Eta",
			"jet1Phi",
			"jet2Phi",
			"MET",
			"jet1SubjetMaxBDisc",
			"jet2SubjetMaxBDisc",
			"jet1BDisc",
			"jet2BDisc",
			"jet1MassGroomed",
			"jet2MassGroomed",
			"jet1tau32",
			"jet2tau32",
			"jet1minMass",
			"jet2minMass",
			"abs(deltaPhi)",
			"abs(deltaY)",
			"NNoutput+(NNoutputExp > 0.5)",
			"NNoutput",
			"NNoutputExp"
		};

int bins = 50;
float min[nVar] = {		
			500,//"mttMass",
			100,//"jet1Pt",
			100,//"jet2Pt",
			500,//"sumJetPt",
			100,//"jet1Mass",
			100,//"jet2Mass",
			-3,//"jet1Eta",
			-3,//"jet2Eta",
			0,//"jet1Phi",
			0,//"jet2Phi",
			0,//"MET",
			0,//"jet1SubjetMaxBDisc",
			0,//"jet2SubjetMaxBDisc",
			0,//"jet1BDisc",
			0,//"jet2BDisc",
			0,//"jet1MassGroomed",
			0,//"jet2MassGroomed",
			0,//"jet1tau32",
			0,//"jet2tau32",
			0,//"jet1minMass",
			0,//"jet2minMass",
			0,//"deltaPhi",
			0,//"deltaY"
			0.0,//"NNoutput",
			0.0,
			0.0
		};

float max[nVar] = {		
			5000,//"mttMass",
			1000,//"jet1Pt",
			1000,//"jet2Pt",
			2000,//"sumJetPt",
			300,//"jet1Mass",
			300,//"jet2Mass",
			3,//"jet1Eta",
			3,//"jet2Eta",
			4,//"jet1Phi",
			4,//"jet2Phi",
			200,//"MET",
			1,//"jet1SubjetMaxBDisc",
			1,//"jet2SubjetMaxBDisc",
			1,//"jet1BDisc",
			1,//"jet2BDisc",
			500,//"jet1MassGroomed",
			500,//"jet2MassGroomed",
			1,//"jet1tau32",
			1,//"jet2tau32",
			200,//"jet1minMass",
			200,//"jet2minMass",
			4,//"deltaPhi",
			4,//"deltaY"
			2.0,//"NNoutput"
			1.0,	
			1.0
		};


string titles[nVar] = {	
			"t#bar{t} Invariant Mass (GeV)",
			"Jet 1 p_{T} (GeV)",
			"Jet 2 p_{T} (GeV)",
			"Sum of jet p_{T} (GeV)",
			"Jet 1 Mass (GeV)",
			"Jet 2 Mass (GeV)",
			"Jet 1 #eta",
			"Jet 2 #eta",
			"Jet 1 #phi",
			"Jet 2 #phi",
			"Missing E_{T} (GeV)",
			"Jet 1 Maximum Subjet CSV Value",
			"Jet 2 Maximum Subjet CSV Value",
			"Jet 1 CSV Value",
			"Jet 2 CSV Value",
			"Jet 1 Groomed Mass (GeV)",
			"Jet 2 Groomed Mass (GeV)",
			"Jet 1 #tau_{3}/#tau_{2}",
			"Jet 2 #tau_{3}/#tau_{2}",
			"Jet 1 m_{min} (GeV)",
			"Jet 2 m_{min} (GeV)",
			"#Delta #phi (Jet 1, Jet 2)",
			"#Delta #eta (Jet 1, Jet2)",
			"Final Discriminant",
			"Signal Discriminant",
			"Expert Discriminant"
		};

int cutflow[3] = {4,5,6};
string mistag[3] = {"mistagWt", "2*mistagWt1B*(1-mistagWt1B)", "mistagWt2B"};
string cut_label[3] = {">= 0 b-tags", "1 b-tag", "2 b-tags"};
string cut_title[3] = {"0btag", "1btag", "2btag"};

for (int i = 0 ; i < nVar; i++){

for (int j = 0; j < 3; j++){



	cout << i << bins << " " << min[i] << " " << max[i] << endl;


	TH1F *dataH = new TH1F("dataH", "dataH", bins,min[i],max[i]);
	TH1F *ttbar10H = new TH1F("ttbar10H", "ttbar10H",bins,min[i],max[i]);
	TH1F *ttbar7H = new TH1F("ttbar7H", "ttbar7H", bins,min[i],max[i]);
	TH1F *qcdH = new TH1F("qcdH", "qcdH", bins,min[i],max[i]);
	TH1F *signalH = new TH1F("signalH", "signalH",bins,min[i],max[i]);
	
	data->Draw( Form("%s>>dataH",vars[i].c_str()), Form("index == 0 && cutflow == %d", cutflow[j]));
	ttbar10->Draw(Form("%s>>ttbar10H",vars[i].c_str()), Form("index == 0 && cutflow == %d", cutflow[j]));
	ttbar7->Draw(Form("%s>>ttbar7H",vars[i].c_str()), Form("index == 0 && cutflow == %d", cutflow[j]));
	if (i == 0) { 
		if (j == 0) qcd->Draw("mttMassPred>>qcdH", Form("%s*(index == 1)", mistag[j].c_str()));
		if (j == 1) qcd->Draw("mttMassPred>>qcdH", Form("%s*(index == 2)", mistag[j].c_str()));
		if (j == 2) qcd->Draw("mttMassPred>>qcdH", Form("%s*(index == 3)", mistag[j].c_str()));}
	else {
		if (j == 0) qcd->Draw(Form("%s>>qcdH",vars[i].c_str()), Form("%s*(index == 1)", mistag[j].c_str()));
		if (j == 1) qcd->Draw(Form("%s>>qcdH",vars[i].c_str()), Form("%s*(index == 2)", mistag[j].c_str()));
		if (j == 2) qcd->Draw(Form("%s>>qcdH",vars[i].c_str()), Form("%s*(index == 3)", mistag[j].c_str()));}
	signal->Draw(Form("%s>>signalH",vars[i].c_str()), Form("index == 0 && cutflow == %d", cutflow[j]));

dataH->SetLineColor(kBlack);
qcdH->SetLineColor(kYellow + 1);
qcdH->SetFillColor(kYellow + 1);
signalH->SetLineColor(kGreen - 1);
signalH->SetLineWidth(2);

dataH->GetXaxis()->SetTitle(titles[i].c_str());
dataH->GetYaxis()->SetTitle("Events");

signalH->Scale(1. * 7300. / 90000. );
ttbar7H->Scale(234. * 7300.* 0.074 / 3082812.);
ttbar10H->Scale(234. * 7300.* 0.014 / 1249111.);

TH1F *ttbarH = ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);

ttbarH->SetLineColor(kRed);
ttbarH->SetFillColor(kRed);

dataH->SetMarkerStyle(21);

THStack *stack = new THStack("stack", "stack");
stack->Add(ttbarH);
stack->Add(qcdH);

dataH->SetMinimum(0.01);
dataH->SetMaximum(qcdH->GetMaximum() * 1.3 + 50);
dataH->Draw("E");
stack->Draw("same");
dataH->Draw("E same");
signalH->Draw("same");

cout << "BG: " << qcdH->Integral() + ttbarH->Integral() << endl;
cout << "SIG: " << signalH->Integral() << endl;



c1->RedrawAxis();

TLegend *leg = new TLegend(0.1,0.91,0.9,0.99);
leg->SetFillColor(0);
leg->SetLineColor(0);
leg->SetNColumns(4);
leg->AddEntry(ttbarH, "t#bar{t}", "f");
leg->AddEntry(qcdH, "QCD", "f");
leg->AddEntry(signalH, "2 TeV Z'", "l");
leg->AddEntry(ttbarH, cut_label[j].c_str(), "");
leg->Draw("same");

c1->SetLogy(0);
c1->SaveAs(Form("%s_%s.png", vars[i].c_str(), cut_title[j].c_str())); 
c1->SetLogy(1);
c1->SaveAs(Form("%s_%s_log.png", vars[i].c_str(), cut_title[j].c_str())); 



delete dataH;
delete ttbar10H;
delete ttbar7H;
delete qcdH;
delete signalH;


}
}

}


