{

TFile *offFile = new TFile("triggerOFF.root");
TFile *onFile =  new TFile("triggerHT.root");

TH1F *numHist = (TH1F *) onFile->Get("boostedTopTrigger/HThistAll");
TH1F *denHist = (TH1F *) offFile->Get("boostedTopTrigger/HThistAll");
numHist->Sumw2();
denHist->Sumw2();


numHist->Divide(denHist);

numHist->Draw("E");

}



