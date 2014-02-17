{

gROOT->SetBatch(0);

TFile *file1 = new TFile("Dec3_QCD_300_InclNsub_all.root");
TFile *file2 = new TFile("Dec3_QCD_470_InclNsub_all.root");
TFile *file3 = new TFile("Dec3_QCD_600_InclNsub_all.root");
TFile *file4 = new TFile("Dec3_QCD_800_InclNsub_all.root");
TFile *file5 = new TFile("Dec3_QCD_1000_InclNsub_all.root");
TFile *file6 = new TFile("Dec3_QCD_1400_InclNsub_all.root");


//TFile *data = new TFile("trees/data_all_mistag.root");





TTree *tree1 = (TTree *) file1->Get("treeVars");
TTree *tree2 = (TTree *) file2->Get("treeVars");
TTree *tree3 = (TTree *) file3->Get("treeVars");
TTree *tree4 = (TTree *) file4->Get("treeVars");
TTree *tree5 = (TTree *) file5->Get("treeVars");
TTree *tree6 = (TTree *) file6->Get("treeVars");

//TTree *treeD = data->Get("treeVars");

string variable = "mttMass";
string cut = "(cutflow == 4 && index == 0 && abs(deltaY) < 1.0 && jet1tau32 < 0.7 && jet2tau32 < 0.7)";
//string cut = "(cutflow == 4 && index == 0 && (jet1SubjetMaxBDisc > 0.679 || jet2SubjetMaxBDisc > 0.679) && (jet1SubjetMaxBDisc < 0.679 || jet2SubjetMaxBDisc < 0.679))";
//string cut = "(cutflow == 4 && index == 0 && jet1SubjetMaxBDisc > 0.679 && jet2SubjetMaxBDisc > 0.679)";
//string cut = "(cutflow == 4 && index == 0 && jet1SubjetMaxBDisc < 0.679 && jet2SubjetMaxBDisc < 0.679 && abs(deltaY) < 1.0)";

string binning = "(50, 500, 5000)";

string predVar = "mttMassPred";
string predCut = "(mistagWtNsub)*(index == 1 && abs(deltaY) < 1.0)";

tree1->Draw(Form("%s>>qcd1H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");
tree2->Draw(Form("%s>>qcd2H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");
tree3->Draw(Form("%s>>qcd3H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");
tree4->Draw(Form("%s>>qcd4H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");
tree5->Draw(Form("%s>>qcd5H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");
tree6->Draw(Form("%s>>qcd6H%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");


tree1->Draw(Form("%s>>qcd1predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");
tree2->Draw(Form("%s>>qcd2predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");
tree3->Draw(Form("%s>>qcd3predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");
tree4->Draw(Form("%s>>qcd4predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");
tree5->Draw(Form("%s>>qcd5predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");
tree6->Draw(Form("%s>>qcd6predH%s", predVar.c_str(), binning.c_str()), predCut.c_str(), "goff");

//treeD->Draw(Form("%s>>dataH%s", variable.c_str(), binning.c_str()), cut.c_str(), "goff");

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

TH1F *totalH = qcd2H->Clone();
TH1F *totalPredH = qcd2predH->Clone();

//totalH->Add(qcd2H);
totalH->Add(qcd3H);
totalH->Add(qcd4H);
totalH->Add(qcd5H);
totalH->Add(qcd6H);
//totalPredH->Add(qcd2predH);
totalPredH->Add(qcd3predH);
totalPredH->Add(qcd4predH);
totalPredH->Add(qcd5predH);
totalPredH->Add(qcd6predH);


TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,500);
  c1->Range(0,0,1,1);
c1->Draw();

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



totalH->SetLineColor(kRed);
totalH->SetLineWidth(2);
totalH->Draw("E");

totalPredH->Draw("hist same");

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
totalH->Draw("E same");

TLegend *leg = new TLegend(0.5,0.7,0.89,0.89);
leg->AddEntry(totalH, "QCD Selected", "lp");
leg->AddEntry(totalPredH, "QCD Predicted", "l");
leg->Draw("same");


//qcd1H->Draw("same");
//qcd2H->Draw("same");
//qcd3H->Draw("same");
//qcd4H->Draw("same");
//qcd5H->Draw("same");
//qcd6H->Draw("same");


//dataH->DrawNormalized("E same");

  c1_1->cd();
  c1_1->SetTopMargin(0.01);
  c1_1->SetBottomMargin(0.3);
  c1_1->SetRightMargin(0.05);
  c1_1->SetLeftMargin(0.1);
  c1_1->SetFillStyle(0);


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

ratioH->Draw("E");


TF1 *line = new TF1("line", "1", 0, 5000);
line->SetLineColor(kBlack);
line->Draw("same");






}
