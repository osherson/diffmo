{

gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);

TChain *data = new TChain("treeVars");
TChain *ttbar = new TChain("treeVars");
TChain *qcd = new TChain("treeVars");
TChain *signal = new TChain("treeVars");


data->Add("Aug19_dataD.root");
ttbar->Add("Aug19_ttjets10.root");
qcd->Add("Aug19_qcd300_470.root");
signal->Add("Aug19_Zprime2000.root");



data->Draw("cutflow>>dataH");
ttbar->Draw("cutflow>>ttbarH");
qcd->Draw("cutflow>>qcdH");
signal->Draw("cutflow>>signalH");

dataH->SetLineColor(kBlack);
ttbarH->SetLineColor(kRed);
qcdH->SetLineColor(kBlue);
signalH->SetLineColor(kGreen + 1);

signalH->GetXaxis()->SetBinLabel(1, "All");
signalH->GetXaxis()->SetBinLabel(2, "1 jet pT > 400");
signalH->GetXaxis()->SetBinLabel(3, "1 Top-Tag");
signalH->GetXaxis()->SetBinLabel(4, "2 jets pT > 400");
signalH->GetXaxis()->SetBinLabel(5, "TYPE 1+1");
signalH->GetXaxis()->SetBinLabel(6, "TYPE 1+1+b");
signalH->GetXaxis()->SetBinLabel(7, "TYPE 1+1+2b");
signalH->GetXaxis()->SetBinLabel(8, "1 Top-Tag + 2nd jet pT > 200");
signalH->GetXaxis()->SetBinLabel(9, "1 Top-Tag + 1 W-tag");
signalH->GetXaxis()->SetBinLabel(10, "TYPE 1+2");
signalH->GetXaxis()->SetBinLabel(11, "TYPE 1+2+b");


float signalNorm = signalH->GetBinContent(1);
float ttbarNorm = ttbarH->GetBinContent(1);
float qcdNorm = qcdH->GetBinContent(1);
float dataNorm = dataH->GetBinContent(1);

signalH->Scale(1./ signalNorm);
ttbarH->Scale(1./ ttbarNorm);
qcdH->Scale(1./ qcdNorm);
dataH->Scale(1./ dataNorm);



signalH->Draw();
ttbarH->Draw("same");
qcdH->Draw("same");
dataH->Draw("E same");






}


