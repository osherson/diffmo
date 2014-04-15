void plotDistribution(string var, int nBins, float min, float max, string label, int tags, int nsub){

gROOT->Reset();

gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);

stringstream ss;
ss << tags;
string tags_label = ss.str();
stringstream ss2;
ss2 << nsub;
string nsub_label = ss2.str();


TChain *data = new TChain("treeVars");
TChain *ttbar10 = new TChain("treeVars");
TChain *ttbar7 = new TChain("treeVars");
TChain *qcd = new TChain("treeVars");
TChain *signal = new TChain("treeVars");
TChain *signal1 = new TChain("treeVars");
TChain *signal15 = new TChain("treeVars");
TChain *signal3 = new TChain("treeVars");
TChain *signal4 = new TChain("treeVars");


data->Add("FINAL/data.root");
ttbar10->Add("FINAL/ttjets10.root");
ttbar7->Add("FINAL/ttjets7.root");
qcd->Add("FINAL/data.root");
signal->Add("Aug19_Zp20_sec0_ttpair__TriggernoWeight.root");
signal1->Add("Aug19_Zp10_sec0_ttpair__TriggernoWeight.root");
signal15->Add("Aug19_Zp15_sec0_ttpair__TriggernoWeight.root");
signal3->Add("Aug19_Zp30_sec0_ttpair__TriggernoWeight.root");
signal4->Add("Aug19_Zp40_sec0_ttpair__TriggernoWeight.root");

//int nBins = 50;
//float min = 0;
//float max = 1500;


TH1F *dataH = new TH1F("dataH", "dataH", nBins,min,max);
TH1F *ttbar10H = new TH1F("ttbar10H", "ttbar10H", nBins,min,max);
TH1F *ttbar10misH = new TH1F("ttbar10misH", "ttbar10misH", nBins,min,max);
TH1F *ttbar7H = new TH1F("ttbar7H", "ttbar7H", nBins,min,max);
TH1F *ttbar7misH = new TH1F("ttbar7misH", "ttbar7misH", nBins,min,max);
TH1F *qcdH = new TH1F("qcdH", "qcdH", nBins,min,max);
TH1F *signalH = new TH1F("signalH", "signalH", nBins,min,max);
TH1F *signal1H = new TH1F("signal1H", "signal1H", nBins,min,max);
TH1F *signal15H = new TH1F("signal15H", "signal15H", nBins,min,max);
TH1F *signal3H = new TH1F("signal3H", "signal3H", nBins,min,max);
TH1F *signal4H = new TH1F("signal4H", "signal4H", nBins,min,max);

//string var = "jet1Pt";
string qcdvar = var;
if (var == "mttMass") qcdvar = "mttMassPred";
string btag = " ";
string btag0 = " && jet1SubjetMaxBDisc < 0.679 && jet2SubjetMaxBDisc < 0.679";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
string btag1 = " && (jet1SubjetMaxBDisc < 0.679 || jet2SubjetMaxBDisc < 0.679) && (jet1SubjetMaxBDisc > 0.679 || jet2SubjetMaxBDisc > 0.679)";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
string btag2 = " && jet1SubjetMaxBDisc > 0.679 && jet2SubjetMaxBDisc > 0.679 && jet1BDisc > 0.678 && jet2BDisc > 0.679";// && jet1SubjetMaxBDisc > 0 && jet2SubjetMaxBDisc > 0";
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


data->Draw(Form("%s>>dataH", var.c_str()), sel.c_str());
ttbar10->Draw(Form("%s>>ttbar10H", var.c_str()), ttbarsel.c_str());
ttbar7->Draw(Form("%s>>ttbar7H", var.c_str()), ttbarsel.c_str());
ttbar10->Draw(Form("%s>>ttbar10misH", var.c_str()), qcdsel.c_str());
ttbar7->Draw(Form("%s>>ttbar7misH", var.c_str()), qcdsel.c_str());

qcd->Draw(Form("%s>>qcdH", qcdvar.c_str()), qcdsel.c_str());
signal->Draw(Form("%s>>signalH", var.c_str()), sel.c_str());
signal1->Draw(Form("%s>>signal1H", var.c_str()), sel.c_str());
signal15->Draw(Form("%s>>signal15H", var.c_str()), sel.c_str());
signal3->Draw(Form("%s>>signal3H", var.c_str()), sel.c_str());
signal4->Draw(Form("%s>>signal4H", var.c_str()), sel.c_str());
dataH->Sumw2();
ttbar10H->Sumw2();
ttbar7H->Sumw2();
qcdH->Sumw2();
signalH->Sumw2();

dataH->AddBinContent(dataH->GetNbinsX(), dataH->GetBinContent(dataH->GetNbinsX()+1));
qcdH->AddBinContent(qcdH->GetNbinsX(), qcdH->GetBinContent(qcdH->GetNbinsX()+1));
signalH->AddBinContent(signalH->GetNbinsX(), signalH->GetBinContent(signalH->GetNbinsX()+1));
ttbar7H->AddBinContent(ttbar7H->GetNbinsX(), ttbar7H->GetBinContent(ttbar7H->GetNbinsX()+1));
ttbar7misH->AddBinContent(ttbar7misH->GetNbinsX(), ttbar7misH->GetBinContent(ttbar7misH->GetNbinsX()+1));
ttbar10misH->AddBinContent(ttbar10misH->GetNbinsX(), ttbar10misH->GetBinContent(ttbar10misH->GetNbinsX()+1));
ttbar10H->AddBinContent(ttbar10H->GetNbinsX(), ttbar10H->GetBinContent(ttbar10H->GetNbinsX()+1));

dataH->SetLineColor(kBlack);
qcdH->SetLineColor(kYellow);
qcdH->SetFillColor(kYellow);
signalH->SetLineColor(kGreen + 3);
signalH->SetLineWidth(2);
signal15H->SetLineWidth(2);
signal3H->SetLineWidth(2);
dataH->SetMarkerStyle(20);

float lumi = 19700.;
float subSF = 0.95*0.95;
float btagSF = 0.966*0.966;

signalH->Scale(1. * lumi / 90778. );
signal1H->Scale(1. * lumi / 101697. );
signal15H->Scale(1. * lumi / 20757. );
signal3H->Scale(1. * lumi / 91209. );
signal4H->Scale(1. * lumi / 98920. );



float btag7SF[4] = { 1.13, 1.022, 0.966*0.966, 1.00 };
float btag10SF[4] = { 1.13, 1.210, 0.966*0.966, 1.00 };  



ttbar7H->Scale(234. *btag7SF[tags]*subSF*subSF* lumi * 0.074 / 3082812.);
ttbar7misH->Scale(234. *subSF*subSF* lumi * 0.074 / 3082812.);
ttbar10H->Scale(234. * btag10SF[tags]*subSF*subSF*lumi * 0.014 / 1249111.);
ttbar10misH->Scale(234. * subSF*subSF*lumi * 0.014 / 1249111.);

cout << "tt7:  " << ttbar7H->Integral() << endl;
cout << "tt10: " << ttbar10H->Integral() << endl;

cout << "ttbar    : " << ttbar7H->Integral() + ttbar10H->Integral() << endl;
cout << "ttbar Mis: " << ttbar7misH->Integral() + ttbar10misH->Integral() << endl;
cout << "QCD      : " << qcdH->Integral() << endl;
cout << "DATA     : " << dataH->Integral() << endl;
cout << "--------------" << endl;
cout << "Z' 1 TeV : " << signal1H->GetEntries() / 101697. << endl;
cout << "Z' 1.5   : " << signal15H->GetEntries() / 20757. << endl;
cout << "Z' 2 TeV : " << signalH->GetEntries() / 90778. << endl;
cout << "Z' 3 TeV : " << signal3H->GetEntries() / 91209. << endl;
cout << "Z' 4 TeV : " << signal4H->GetEntries() / 98920. << endl;




TH1F *ttbarH = (TH1F *) ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);
qcdH->Add(ttbar7misH, -1);
qcdH->Add(ttbar10misH, -1);

ttbarH->SetLineColor(kRed);
ttbarH->SetFillColor(kRed);

ttbar10H->SetFillColor(kViolet);
ttbar7H->SetFillColor(kRed);

THStack *stack = new THStack("stack", "stack");
stack->Add(ttbarH);
cout << "Stack Integral: " << ttbarH->Integral() << endl;
//stack->Add(ttbar10H);
stack->Add(qcdH);

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

dataH->SetMaximum( 1.2* TMath::Max(dataH->GetMaximum(), signalH->GetMaximum() ) );

dataH->GetYaxis()->SetTitle("Events");
dataH->GetYaxis()->SetTitleOffset(0.7);

dataH->Draw("axis");
stack->Draw("hist same");
dataH->Draw("E same");
signalH->SetLineColor(kGreen);
signalH->Draw("hist same");
signal3H->SetLineColor(kBlue);
signal3H->Draw("hist same");
signal15H->SetLineColor(kViolet);
signal15H->Draw("hist same");


TLegend *leg = new TLegend(0.75,0.6,0.9,0.88);
leg->AddEntry(dataH, btag_string, "");
leg->AddEntry(dataH, "Data", "p");
leg->AddEntry(qcdH, "QCD", "f");
leg->AddEntry(ttbarH, "t#bar{t}", "f");
leg->AddEntry(signal15H, "1.5 TeV", "l");
leg->AddEntry(signalH, "2 TeV", "l");
leg->AddEntry(signal3H, "3 TeV", "l");
leg->SetFillColor(0);
leg->Draw("same");

TLatex *cmsLabel = new TLatex();
cmsLabel.SetNDC();
cmsLabel.DrawLatex(0.1,0.9, "CMS Preliminary, #sqrt{s} = 8 TeV, 19.7 fb^{-1}");
cmsLabel.DrawLatex(0.8, 0.9, label.c_str());


if (dataH->GetMaximum() > 1000) gPad->SetLogy(1);

gPad->RedrawAxis();


TH1F *totalH = new TH1F();
totalH = (TH1F *) qcdH->Clone("total");
totalH->Add(ttbar10H);
totalH->Add(ttbar7H);
cout << totalH->Integral() << endl;
cout << dataH->Integral() << endl;

TH1F *ratioH = new TH1F();
ratioH->Sumw2();
ratioH = (TH1F*) dataH->Clone("ratio");
ratioH->Divide(totalH);




  c1_1->cd();
  c1_1->SetTopMargin(0.01);
  c1_1->SetBottomMargin(0.3);
  c1_1->SetRightMargin(0.05);
  c1_1->SetLeftMargin(0.1);
  c1_1->SetFillStyle(0);

ratioH->GetYaxis()->SetRangeUser(0.,2.);
ratioH->GetYaxis()->SetTitle("Data / BG Ratio");
ratioH->GetYaxis()->SetTitleOffset(0.4);
ratioH->GetYaxis()->SetTitleSize(0.11);
ratioH->GetXaxis()->SetLabelSize(0.11);
ratioH->GetXaxis()->SetTitleSize(0.11);
ratioH->GetXaxis()->SetTitle( var.c_str());
ratioH->Draw("E");

TF1 *line = new TF1("line", "1", min, max);
line->SetLineColor(kBlack);
line->Draw("same");

gPad->RedrawAxis();

TString outname = var.c_str();

c1->SaveAs("ZPplots/"+outname+"_"+tags_label.c_str()+"_"+nsub_label.c_str()+".pdf");

TFile *outfile = new TFile(outname+"_"+tags_label.c_str()+".root", "RECREATE");
outfile->cd();

TH1F *ttbarTotH = new TH1F();
ttbarTotH = (TH1F *) ttbar7H->Clone(Form("%dbtag__ttbar", tags));
ttbarTotH->Add(ttbar10H);

dataH->SetName(Form("%dbtag__DATA", tags));
signalH->SetName(Form("%dbtag__zp2000", tags));
ttbarTotH->SetName(Form("%dbtag__ttbar", tags));
qcdH->SetName(Form("%dbtag__qcd", tags));
signalH->Write(Form("%dbtag__zp2000", tags));
signal1H->Write(Form("%dbtag__zp1000", tags));
signal15H->Write(Form("%dbtag__zp1500", tags));
signal3H->Write(Form("%dbtag__zp3000", tags));
signal4H->Write(Form("%dbtag__zp4000", tags));
qcdH->Write(Form("%dbtag__qcd", tags));
dataH->Write(Form("%dbtag__DATA", tags));

outfile->Write();
outfile->Close();




delete data;
delete ttbar10;
delete ttbar7;
delete signal;
delete signal1;
delete signal15;
delete signal3;
delete signal4;
delete qcd;


}


