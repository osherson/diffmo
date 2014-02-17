void plotDistribution(string name, string label){

gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);

TChain *data = new TChain("treeVars");
TChain *ttbar10 = new TChain("treeVars");
TChain *ttbar7 = new TChain("treeVars");
TChain *qcd = new TChain("treeVars");
TChain *signal = new TChain("treeVars");
TChain *signal1 = new TChain("treeVars");
TChain *signal15 = new TChain("treeVars");
TChain *signal3 = new TChain("treeVars");
TChain *signal4 = new TChain("treeVars");


data->Add("Dec3_data_inclRate.root");
ttbar10->Add("Oct7_ttjets10.root");
ttbar7->Add("Oct7_ttjets7.root");
qcd->Add("Dec3_data_inclRate.root");
signal->Add("Aug19_Zp20_sec0_ttpair__TriggernoWeight.root");
signal1->Add("Aug19_Zp10_sec0_ttpair__TriggernoWeight.root");
signal15->Add("Aug19_Zp15_sec0_ttpair__TriggernoWeight.root");
signal3->Add("Aug19_Zp30_sec0_ttpair__TriggernoWeight.root");
signal4->Add("Aug19_Zp40_sec0_ttpair__TriggernoWeight.root");

int nBins = 100;
float min = 500.;
float max = 4500.;


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

string var = "mttMass";
string qcdvar = "mttMassPred";
//string sel = "index == 0 && cutflow == 4";
string sel = "(index == 0 && cutflow == 4)";
string qcdsel = "(mistagWt)*(index == 1 && type11 == 0)";




data->Draw(Form("%s>>dataH", var.c_str()), sel.c_str());
ttbar10->Draw(Form("%s>>ttbar10H", var.c_str()), sel.c_str());
ttbar7->Draw(Form("%s>>ttbar7H", var.c_str()), sel.c_str());
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

float lumi = 19395.;
float subSF = 0.95;


signalH->Scale(1. * lumi / 90778. );
signal1H->Scale(1. * lumi / 101697. );
signal15H->Scale(1. * lumi / 20757. );
signal3H->Scale(1. * lumi / 91209. );
signal4H->Scale(1. * lumi / 98920. );



ttbar7H->Scale(234. *subSF*subSF* lumi * 0.074 / 3082812.);
ttbar7misH->Scale(234. *subSF*subSF* lumi * 0.074 / 3082812.);
ttbar10H->Scale(234. * subSF*subSF*lumi * 0.014 / 1249111.);
ttbar10misH->Scale(234. * subSF*subSF*lumi * 0.014 / 1249111.);



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




ttbar7H->Clone("ttbarH");
ttbarH->Add(ttbar10H);
//ttbarH->Add(ttbar7misH, -1);
//ttbarH->Add(ttbar10misH, -1);

ttbarH->SetLineColor(kRed);
ttbarH->SetFillColor(kRed);

ttbar10H->SetFillColor(kViolet);
ttbar7H->SetFillColor(kRed);

THStack *stack = new THStack("stack", "stack");
stack->Add(ttbarH);
cout << ttbarH->Integral() << endl;
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
cmsLabel.DrawLatex(0.1,0.9, "CMS Preliminary, #sqrt{s} = 8 TeV, 19.6 fb^{-1}");
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

TString outname = name.c_str();

c1->SaveAs(outname+".pdf");
c1->SaveAs("c1.root");

TFile *outfile = new TFile(outname+".root", "RECREATE");
outfile->cd();

TH1F *ttbarTotH = new TH1F();
ttbarTotH = (TH1F *) ttbar7H->Clone("2btag__ttbar");
ttbarTotH->Add(ttbar10H);

dataH->SetName("2btag__DATA");
signalH->SetName("2btag__zp2000");
ttbarTotH->SetName("2btag__ttbar");
qcdH->SetName("2btag__qcd");
signalH->Write("2btag__zp2000");
signal1H->Write("2btag__zp1000");
signal15H->Write("2btag__zp1500");
signal3H->Write("2btag__zp3000");
signal4H->Write("2btag__zp4000");
qcdH->Write("2btag__qcd");
dataH->Write("2btag__DATA");

outfile->Write();
outfile->Close();


}


