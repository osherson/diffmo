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
signal->Add("Aug19_Zprime2000.root");

int nVar = 23;

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
			"abs(deltaY)"
		};

int bins = 50;
int min[nVar] = {		
			500,//"mttMass",
			100,//"jet1Pt",
			100,//"jet2Pt",
			500,//"sumJetPt",
			0,//"jet1Mass",
			0,//"jet2Mass",
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
		};

int max[nVar] = {		
			5000,//"mttMass",
			1000,//"jet1Pt",
			1000,//"jet2Pt",
			2000,//"sumJetPt",
			500,//"jet1Mass",
			500,//"jet2Mass",
			3,//"jet1Eta",
			3,//"jet2Eta",
			7,//"jet1Phi",
			7,//"jet2Phi",
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
			7,//"deltaPhi",
			7,//"deltaY"
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
			"#Delta #eta (Jet 1, Jet2)"
		};

int cutflow[3] = {4,5,6};
string mistag[3] = {"mistagWt", "mistagWt1B", "mistagWt2B"};
string cut_label[3] = {">= 0 b-tags", "1 b-tag", "2 b-tags"};


for (int i = 0; i < nVar; i++){

for (int j = 0; j < 3; j++){

	TH1F *dataH = new TH1F("dataH", Form("dataH;%s", titles[i].c_str()), bins,min[i],max[i]);
	TH1F *ttbar10H = new TH1F("ttbar10H", Form("ttbar10H;%s", titles[i].c_str()),bins,min[i],max[i]);
	TH1F *ttbar7H = new TH1F("ttbar7H", Form("ttbar7H;%s", titles[i].c_str()), bins,min[i],max[i]);
	TH1F *qcdH = new TH1F("qcdH", Form("qcdH;%s", titles[i].c_str()), bins,min[i],max[i]);
	TH1F *signalH = new TH1F("signalH", Form("signalH;%s", titles[i].c_str()),bins,min[i],max[i]);

	
	data->Draw( Form("%s>>dataH",vars[i].c_str()), Form("cutflow == %d", cutflow[j]));
	ttbar10->Draw(Form("%s>>ttbar10H",vars[i].c_str()), Form("cutflow == %d", cutflow[j]));
	ttbar7->Draw(Form("%s>>ttbar7H",vars[i].c_str()), Form("cutflow == %d", cutflow[j]));
	if (i == 0) qcd->Draw("mttMassPred>>qcdH", Form("%s*(index == 1)", mistag[j].c_str()));
	else qcd->Draw(Form("%s>>qcdH",vars[i].c_str()), Form("%s*(index == 1)", mistag[j].c_str()));
	signal->Draw(Form("%s>>signalH",vars[i].c_str()), Form("cutflow == %d", cutflow[j]));

dataH->SetLineColor(kBlack);
qcdH->SetLineColor(kBlue);
qcdH->SetFillColor(kBlue);
signalH->SetLineColor(kGreen + 1);



signalH->Scale(1. * 7300. / 90000. );
ttbar7H->Scale(234. * 7300.* 0.074 / 3082812.);
ttbar10H->Scale(234. * 7300.* 0.014 / 1249111.);

ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);

ttbarH->SetLineColor(kRed);
ttbarH->SetFillColor(kRed);

THStack *stack = new THStack("stack", "stack");
stack->Add(ttbarH);
stack->Add(qcdH);

stack->Draw();
dataH->Draw("E same");
signalH->Draw("same");

TLegend *leg = new TLegend(0.75,0.75,0.9,0.9);
leg->AddEntry(ttbarH, "t#bar{t}", "f");
leg->AddEntry(qcdH, "QCD", "f");
leg->AddEntry(signalH, "2 TeV Z' (1 pb)", "l");
leg->AddEntry(ttbarH, cut_label.c_str(), "");
leg->Draw("same");

}
}

}


