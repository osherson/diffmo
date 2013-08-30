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

TH1F *dataH = new TH1F("dataH", "dataH", 50,0,5000);
TH1F *ttbar10H = new TH1F("ttbar10H", "ttbar10H", 50,0,5000);
TH1F *ttbar7H = new TH1F("ttbar7H", "ttbar7H", 50,0,5000);
TH1F *qcdH = new TH1F("qcdH", "qcdH", 50,0,5000);
TH1F *signalH = new TH1F("signalH", "signalH", 50,0,5000);

data->Draw("mttMass>>dataH", "cutflow == 6");
ttbar10->Draw("mttMass>>ttbar10H", "cutflow == 6");
ttbar7->Draw("mttMass>>ttbar7H", "cutflow == 6");
qcd->Draw("mttMassPred>>qcdH", "mistagWt2B*(index == 1)");
signal->Draw("mttMass>>signalH", "cutflow == 6");

dataH->SetLineColor(kBlack);
qcdH->SetLineColor(kBlue);
qcdH->SetFillColor(kBlue);
signalH->SetLineColor(kGreen + 1);



signalH->Scale(1. * 7300. / 90000. );
ttbar7H->Scale(234. * 7300.* 0.074 / 3082812.);
ttbar10H->Scale(234. * 7300.* 0.014 / 1249111.);

cout << ttbar7H->Integral() << " " << ttbar10H->Integral() << endl;


ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);

ttbarH->SetLineColor(kRed);
ttbarH->SetFillColor(kRed);

THStack *stack = new THStack("stack", "stack");
stack->Add(ttbarH);
stack->Add(qcdH);

stack->Draw();
dataH->Draw("E same");






}


