void plotShape(string var, string title, int nBins, float min, float max, string label, int tags, int nsub){

gROOT->SetBatch(0);

TFile *file1 = new TFile("Dec3_QCD_300_Inclusive_all.root");
TFile *file2 = new TFile("Dec3_QCD_470_Inclusive_all.root");
TFile *file3 = new TFile("Dec3_QCD_600_Inclusive_all.root");
TFile *file4 = new TFile("Dec3_QCD_800_Inclusive_all.root");
TFile *file5 = new TFile("Dec3_QCD_1000_Inclusive_all.root");
TFile *file6 = new TFile("Dec3_QCD_1400_Inclusive_all.root");


//TFile *data = new TFile("trees/data_all_mistag.root");


TChain *ttbar10 = new TChain("treeVars");
TChain *ttbar7 = new TChain("treeVars");

TChain *signal = new TChain("treeVars");
TChain *signal1 = new TChain("treeVars");
TChain *signal15 = new TChain("treeVars");
TChain *signal3 = new TChain("treeVars");


TTree *tree1 = (TTree *) file1->Get("treeVars");
TTree *tree2 = (TTree *) file2->Get("treeVars");
TTree *tree3 = (TTree *) file3->Get("treeVars");
TTree *tree4 = (TTree *) file4->Get("treeVars");
TTree *tree5 = (TTree *) file5->Get("treeVars");
TTree *tree6 = (TTree *) file6->Get("treeVars");
ttbar10->Add("Jan13_ttjets10_ptw.root");
ttbar7->Add("Jan13_ttjets7_ptw.root");
signal->Add("Aug19_Zp20_sec0_ttpair__TriggernoWeight.root");
signal1->Add("Aug19_Zp10_sec0_ttpair__TriggernoWeight.root");
signal15->Add("Aug19_Zp15_sec0_ttpair__TriggernoWeight.root");
signal3->Add("Aug19_Zp30_sec0_ttpair__TriggernoWeight.root");

//TTree *treeD = data->Get("treeVars");

string qcdvar = var;
if (var == "mttMass") qcdvar = "mttMassPred";
string btag = " ";
string btag0 = " && jet1SubjetMaxBDisc < 0.679 && jet2SubjetMaxBDisc < 0.679";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
string btag1 = " && (jet1SubjetMaxBDisc < 0.679 || jet2SubjetMaxBDisc < 0.679) && (jet1SubjetMaxBDisc > 0.679 || jet2SubjetMaxBDisc > 0.679)";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
string btag2 = " && jet1SubjetMaxBDisc > 0.679 && jet2SubjetMaxBDisc > 0.679";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
string nsub_sel = " ";
if (tags == 0) btag = btag0;
else if (tags == 1) btag = btag1;
else if (tags == 2) btag = btag2;
else btag = " ";
if (nsub == 1) nsub_sel = " && jet1tau32 < 0.7 && jet2tau32 < 0.7";

string sel = "(index == 0 && cutflow == 4 && abs(deltaY) < 1.0" + btag + nsub_sel + ")";
string qcdsel = "(mistagWt)*(index == 1 && abs(deltaY) < 1.0" + btag +")";
if (nsub == 1) qcdsel = "(mistagWt)*(index == 1 && abs(deltaY) < 1.0" + btag + " && (jet1tau32 < 0.7 && jet2tau32 < 0.7) )";
string ttbarsel = sel+"*(ptReweight)";

TString btag_string = "0+1+2 b-tags";
if (tags == 0) btag_string = "0 b-tags";
if (tags == 1) btag_string = "1 b-tags";
if (tags == 2) btag_string = "2 b-tags";

TH1F *qcd1H = new TH1F("qcd1H", "qcd1H", nBins, min, max);
TH1F *qcd2H = new TH1F("qcd2H", "qcd2H", nBins, min, max);
TH1F *qcd3H = new TH1F("qcd3H", "qcd3H", nBins, min, max);
TH1F *qcd4H = new TH1F("qcd4H", "qcd4H", nBins, min, max);
TH1F *qcd5H = new TH1F("qcd5H", "qcd5H", nBins, min, max);
TH1F *qcd6H = new TH1F("qcd6H", "qcd6H", nBins, min, max);

TH1F *qcd1predH = new TH1F("qcd1predH", "qcd1predH", nBins, min, max);
TH1F *qcd2predH = new TH1F("qcd2predH", "qcd2predH", nBins, min, max);
TH1F *qcd3predH = new TH1F("qcd3predH", "qcd3predH", nBins, min, max);
TH1F *qcd4predH = new TH1F("qcd4predH", "qcd4predH", nBins, min, max);
TH1F *qcd5predH = new TH1F("qcd5predH", "qcd5predH", nBins, min, max);
TH1F *qcd6predH = new TH1F("qcd6predH", "qcd6predH", nBins, min, max);

TH1F *ttbar10H = new TH1F("ttbar10H", "ttbar10H", nBins,min,max);
TH1F *ttbar7H = new TH1F("ttbar7H", "ttbar7H", nBins,min,max);
TH1F *signalH = new TH1F("signalH", "signalH", nBins,min,max);
TH1F *signal1H = new TH1F("signal1H", "signal1H", nBins,min,max);
TH1F *signal15H = new TH1F("signal15H", "signal15H", nBins,min,max);
TH1F *signal3H = new TH1F("signal3H", "signal3H", nBins,min,max);

cout << "VARIABLE: " << var << endl;

tree1->Draw(Form("%s>>qcd1H", var.c_str()), sel.c_str(), "goff");
tree2->Draw(Form("%s>>qcd2H", var.c_str()), sel.c_str(), "goff");
tree3->Draw(Form("%s>>qcd3H", var.c_str()), sel.c_str(), "goff");
tree4->Draw(Form("%s>>qcd4H", var.c_str()), sel.c_str(), "goff");
tree5->Draw(Form("%s>>qcd5H", var.c_str()), sel.c_str(), "goff");
tree6->Draw(Form("%s>>qcd6H", var.c_str()), sel.c_str(), "goff");

ttbar10->Draw(Form("%s>>ttbar10H", var.c_str()), ttbarsel.c_str());
ttbar7->Draw(Form("%s>>ttbar7H", var.c_str()), ttbarsel.c_str());
signal->Draw(Form("%s>>signalH", var.c_str()), sel.c_str());
signal1->Draw(Form("%s>>signal1H", var.c_str()), sel.c_str());
signal15->Draw(Form("%s>>signal15H", var.c_str()), sel.c_str());
signal3->Draw(Form("%s>>signal3H", var.c_str()), sel.c_str());

tree1->Draw(Form("%s>>qcd1predH", qcdvar.c_str()), qcdsel.c_str(), "goff");
tree2->Draw(Form("%s>>qcd2predH", qcdvar.c_str()), qcdsel.c_str(), "goff");
tree3->Draw(Form("%s>>qcd3predH", qcdvar.c_str()), qcdsel.c_str(), "goff");
tree4->Draw(Form("%s>>qcd4predH", qcdvar.c_str()), qcdsel.c_str(), "goff");
tree5->Draw(Form("%s>>qcd5predH", qcdvar.c_str()), qcdsel.c_str(), "goff");
tree6->Draw(Form("%s>>qcd6predH", qcdvar.c_str()), qcdsel.c_str(), "goff");

//treeD->Draw(Form("%s>>dataH%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");

cout << "HEREEEEEEEEEEEEE" << endl;


//dataH->Sumw2();
qcd1predH->Sumw2();
qcd2predH->Sumw2();
qcd3predH->Sumw2();
qcd4predH->Sumw2();
qcd5predH->Sumw2();
qcd6predH->Sumw2();
qcd1H->Sumw2();
qcd2H->Sumw2();
qcd3H->Sumw2();
qcd4H->Sumw2();
qcd5H->Sumw2();
qcd6H->Sumw2();

double lumi = 19400;
double k = 1.0;
qcd1H->Scale( k*1759.6*lumi / 5927300. );
qcd2H->Scale(  k*113.9*lumi / 3994848. );
qcd3H->Scale(    k*27.0*lumi / 3992760. );
qcd4H->Scale( k*3.57*lumi / 3978569. );
qcd5H->Scale( k*0.738*lumi / 1964086. );
qcd6H->Scale( k*0.0335*lumi / 2000062. );
qcd1predH->Scale( k*1759.6*lumi / 5927300. );
qcd2predH->Scale(  k*113.9*lumi / 3994848. );
qcd3predH->Scale(    k*27.0*lumi / 3992760. );
qcd4predH->Scale( k*3.57*lumi / 3978569. );
qcd5predH->Scale( k*0.738*lumi / 1964086. );
qcd6predH->Scale( k*0.0335*lumi / 2000062. );
ttbar7H->Scale(245. * lumi * 0.074 / 3082812.);
ttbar10H->Scale(245. * lumi * 0.014 / 1249111.);
signalH->Scale(1. * lumi / 90778. );
signal1H->Scale(1. * lumi / 101697. );
signal15H->Scale(1. * lumi / 20757. );
signal3H->Scale(1. * lumi / 91209. );

TH1F *totalH = qcd1H->Clone("totalH");
TH1F *totalPredH = qcd1predH->Clone("totalPredH");

totalH->Add(qcd2H);
totalH->Add(qcd3H);
totalH->Add(qcd4H);
totalH->Add(qcd5H);
totalH->Add(qcd6H);
totalPredH->Add(qcd2predH);
totalPredH->Add(qcd3predH);
totalPredH->Add(qcd4predH);
totalPredH->Add(qcd5predH);
totalPredH->Add(qcd6predH);


TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,400);
  c1->Range(0,0,1,1);
c1->Draw();
/*
TPad *c1_1 = new TPad("c1_1", "newpad",0.01,0.01,0.99,0.32);
  c1_1->Draw();
TPad *c1_2 = new TPad("c1_2", "newpad",0.01,0.33,0.99,0.99);
  c1_2->Draw(); 

c1->cd();
  c1_2->cd();
  c1_2->SetTopMargin(0.1);
  c1_2->SetBottomMargin(0.01);
  c1_2->SetRightMargin(0.05);
  c1_2->SetLeftMargin(0.1);
  c1_2->SetFillStyle(0);






c1->SetLogy(1);
*/
TH1F *ttbarH = (TH1F *) ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);


totalH->SetLineColor(kBlue);
totalH->SetLineWidth(2);
//totalH->DrawNormalized("hist");

//totalPredH->Draw("hist same");

qcd1predH->SetLineColor(kBlue);
qcd2predH->SetLineColor(kRed);
qcd3predH->SetLineColor(kGreen);
qcd4predH->SetLineColor(kOrange);
qcd5predH->SetLineColor(kViolet);
qcd6predH->SetLineColor(kCyan);

//qcd1predH->Draw("hist same");
//qcd2predH->Draw("hist same");
//qcd3predH->Draw("hist same");
//qcd4predH->Draw("hist same");
//qcd5predH->Draw("hist same");
//qcd6predH->Draw("hist same");
totalH->SetMarkerSize(0);
totalH->SetLineWidth(3);
totalH->SetLineColor(kBlack);

totalH->GetXaxis()->SetTitle(title.c_str());
totalH->GetYaxis()->SetTitle("Fraction of Events");
totalH->GetYaxis()->SetTitleOffset(1.25);
totalH->DrawNormalized("hist");
ttbarH->SetLineColor(kRed - 1);
ttbarH->SetLineWidth(3);
ttbarH->DrawNormalized("hist same");
signal1H->SetLineColor(kGreen + 1);
signal1H->SetLineWidth(2);
signal1H->DrawNormalized("hist same");
signalH->SetLineColor(kViolet);
signalH->SetLineWidth(2);
signalH->DrawNormalized("hist same");
signal3H->SetLineColor(kCyan + 2);
signal3H->SetLineWidth(2);
signal3H->DrawNormalized("hist same");


TLegend *leg = new TLegend(0.1,0.9,0.9,0.99);
leg->SetNColumns(3);
leg->AddEntry(totalH, btag_string, "");
leg->AddEntry(totalH, "QCD", "l");
leg->AddEntry(ttbarH, "t#bar{t}", "l");
leg->AddEntry(signal1H, "Narrow Z' (1 TeV)", "l");
leg->AddEntry(signalH, "Narrow Z' (2 TeV)", "l");
leg->AddEntry(signal3H, "Narrow Z' (3 TeV)", "l");
leg->SetFillColor(0);
leg->Draw("same");

TLatex *cmsLabel = new TLatex();
cmsLabel->SetNDC();
//cmsLabel.DrawLatex(0.1,0.9, "CMS Preliminary, #sqrt{s} = 8 TeV, 19.7 fb^{-1}");
//cmsLabel->DrawLatex(0.8, 0.9, label.c_str());


//qcd1H->Draw("same");
//qcd2H->Draw("same");
//qcd3H->Draw("same");
//qcd4H->Draw("same");
//qcd5H->Draw("same");
//qcd6H->Draw("same");


//dataH->DrawNormalized("E same");

  c1->cd();
  //c1->SetTopMargin(0.01);
  //c1->SetBottomMargin(0.3);
  //c1->SetRightMargin(0.05);
  //c1->SetLeftMargin(0.1);
  c1->SetFillStyle(0);


TH1F *ratioH = new TH1F();
ratioH->Sumw2();
ratioH = (TH1F *) totalH->Clone("ratioH");
ratioH->Divide(totalPredH);
ratioH->GetYaxis()->SetRangeUser(0.,2.);
ratioH->GetYaxis()->SetTitle("Data / BG Ratio");
ratioH->GetYaxis()->SetTitleOffset(0.4);
ratioH->GetYaxis()->SetTitleSize(0.11);
ratioH->GetXaxis()->SetLabelSize(0.11);
ratioH->GetXaxis()->SetTitleSize(0.11);
ratioH->GetXaxis()->SetTitle( "Dijet Invariant Mass");

//ratioH->Draw("E");


TF1 *line = new TF1("line", "1", 0, 5000);
line->SetLineColor(kBlack);
//line->Draw("same");


c1->SaveAs(Form("closureTest_%d_%d.pdf", tags, nsub));



}
