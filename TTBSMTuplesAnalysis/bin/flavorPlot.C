{

TFile *b_file = new TFile("Jan16_DATAhiB.root");
TFile *c_file = new TFile("Jan16_DATAmedB.root");
TFile *udsg_file = new TFile("Jan16_DATAlowB.root");
TFile *all_file = new TFile("ALLjets_mistag.root");

//TFile *b_file = new TFile("data_HiBscore_mistag.root");
//TFile *c_file = new TFile("data_MedBscore_mistag.root");
//TFile *udsg_file = new TFile("data_LowBscore_mistag.root");
//TFile *all_file = new TFile("data_AllBscore_mistag.root");


string hist_name = "MISTAG_RATE_SUB_TTBAR_InclNsub";

TH1F *b_hist = (TH1F *) b_file->Get(hist_name.c_str() );
TH1F *c_hist = (TH1F *) c_file->Get(hist_name.c_str() );
TH1F *udsg_hist = (TH1F *) udsg_file->Get(hist_name.c_str() );
TH1F *all_hist = (TH1F *) all_file->Get(hist_name.c_str() );

b_hist->SetMarkerSize(0);
c_hist->SetMarkerSize(0);
udsg_hist->SetMarkerSize(0);
all_hist->SetMarkerSize(0);


b_hist->SetLineColor(kGreen -1);
c_hist->SetLineColor(kBlue);
udsg_hist->SetLineColor(kRed);
all_hist->SetLineColor(kBlack);
all_hist->SetLineWidth(2);


b_hist->SetMinimum(0.0);
b_hist->SetMaximum(0.1352);
b_hist->Draw();
c_hist->Draw("same");
udsg_hist->Draw("same");
//all_hist->Draw("same");

TLegend *leg = new TLegend (0.6, 0.6, 0.89, 0.89);
//leg->AddEntry(all_hist, "all jets", "l");
leg->AddEntry(b_hist, "CSVM jets", "l");
leg->AddEntry(c_hist, "CSVL jets", "l");
leg->AddEntry(udsg_hist, "CSV0 jets", "l");

leg->Draw("same");

}

