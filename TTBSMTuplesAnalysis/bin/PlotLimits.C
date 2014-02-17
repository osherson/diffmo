{
TCanvas *c = new TCanvas("c", "", 860,600);
c->SetLogy(1);
gStyle->SetLineWidth(2);

Int_t n = 4;
Double_t x[n] = {1 , 1.5, 2, 3};

Double_t approved[n] = { 1.35, 0.138, 0.0406, 0.0167 };

Double_t inclusive[n] = { 0.908, 0.125, 0.057, 0.028 };
Double_t inclusiveNsub[n] = { 0.602, 0.083, 0.037, 0.0188 };
Double_t inclusiveNsubDY[n] = {0.597, 0.077, 0.0273, 0.0164};
Double_t inclusiveDY[n] = {0.8707, 0.113, 0.0383, 0.0139 };

Double_t zeroT[n] = { 19.83, 0.837, 0.2539, 0.0904 };
Double_t zeroTDY[n] = { 17.74, 0.792, 0.134, 0.0376 };
Double_t zeroTNsub[n] = { 286.5, 0.591, 0.1628, 0.0614 };
Double_t zeroTNsubDY[n] = { 99.56, 0.523, 0.0945, 0.0455 };

Double_t oneT[n] = { 1.202, 0.1409, 0.0644, 0.0303 };
Double_t oneTDY[n] = { 1.2114, 0.1182, 0.0421, 0.02229 };
Double_t oneTNsub[n] = { 0.8847, 0.09596, 0.0439, 0.0239};
Double_t oneTNsubDY[n] = { 0.8713, 0.0811, 0.0323, 0.02748};

Double_t twoT[n] = {0.359, 0.05528, 0.02544, 0.02167};
Double_t twoTNsub[n] = { 0.3272, 0.0521, 0.02204, 0.02042 };
Double_t twoTDY[n] = { 0.3479, 0.060636, 0.027456, 0.0453 };
Double_t twoTNsubDY[n] = { 0.316777, 0.06336, 0.0286, 0.058612 };

Double_t onetwoT[n] = {0.3324, 0.05169, 0.0232, 0.01597};
Double_t onetwoTDY[n] = { 0.3246, 0.05333, 0.021467, 0.0166 };
Double_t onetwoTNsub[n] = {0.294, 0.0452, 0.01885, 0.01354};
Double_t onetwoTNsubDY[n] = { 0.2947, 0.0466, 0.01915, 0.02004 };

Double_t zero12T[n] = { 0.337923, 0.051674, 0.023365, 0.015538};
Double_t zero12TDY[n] = { 0.331, 0.0526, 0.02074, 0.013285 };
Double_t zero12TNsub[n] = { 0.295444, 0.043939, 0.018019, 0.012209 };
Double_t zero12TNsubDY[n] = { 0.293, 0.0466, 0.0177, 0.0153 };





appG = new TGraph(n, x, approved);
appG->SetMaximum(2.0);
appG->SetMinimum(0.01);
appG->GetXaxis()->SetTitle("Z' Mass [TeV]");
appG->GetYaxis()->SetTitle("Expected Limit [pb]");

graph1 = new TGraph(n, x, twoT);
graph2 = new TGraph(n, x, twoTNsub);
graph3 = new TGraph(n, x, twoTDY);
graph4 = new TGraph(n, x, twoTNsubDY);

graph1->SetLineColor(kRed);
graph2->SetLineColor(kBlue);
graph3->SetLineColor(kGreen);
graph4->SetLineColor(kViolet);

appG->Draw("AL");
graph1->Draw("same");
graph2->Draw("same");
graph3->Draw("same");
graph4->Draw("same");


TLegend *leg = new TLegend(0.55, 0.55, 0.89, 0.89);
leg->AddEntry(appG, "B2G-12-005", "l");
leg->AddEntry(graph1, "2 b-tags", "l");
leg->AddEntry(graph2, "+ #tau_{32}", "l");
leg->AddEntry(graph3, "+ #Deltay", "l");
leg->AddEntry(graph4, "+ #tau_{32} + #Deltay", "l");
leg->AddEntry(graph1, "2 b-tags", "");
leg->SetFillColor(0);
leg->SetLineColor(0);
leg->Draw();

}
