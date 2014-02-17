{

TChain *data = new TChain("mistagVars");
TChain *ttjets7 = new TChain("mistagVars");
TChain *ttjets10 = new TChain("mistagVars");

data->Add("data_mistag_all.root");
ttjets7->Add("ttjets7_mistag_all.root");
ttjets10->Add("ttjets10_mistag_all.root");


int nBins = 50;
float min = 200.;
float max = 2000.;

nBins=29;
// Double_t bins[num_bins+1]= {350,355,360,365,370,375,380,385,390,395,400,410,420,430,440,450,460,480,500,525,550,600,650,700,800,2000}; 
Double_t bins[nBins+1] = {400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 460, 470, 480, 490, 500, 525, 550, 575, 600, 650, 700, 750, 800, 900, 1000, 1100, 1250, 1500, 2000};

TH1F *dataNumH = new TH1F("dataNumH", "Data", nBins, bins);
TH1F *ttjets7NumH = new TH1F("ttjets7NumH", "ttbar7", nBins, bins);
TH1F *ttjets10NumH = new TH1F("ttjets10NumH", "ttbar10", nBins, bins);
TH1F *dataDenH = new TH1F("dataDenH", "Data", nBins, bins);
TH1F *ttjets7DenH = new TH1F("ttjets7DenH", "ttbar7", nBins, bins);
TH1F *ttjets10DenH = new TH1F("ttjets10DenH", "ttbar10", nBins, bins);

dataNumH->Sumw2();
dataDenH->Sumw2();

string cutDen = "jet1TopTag == 1 && jet2TopTag == 1 && jet1BTag == 0"; //Probe jet selection
string cutNum = cutDen + " && jet2BTag == 1"; //Tag jet selection

data->Draw("jet2Pt >> dataNumH", cutNum.c_str());
data->Draw("jet2Pt >> dataDenH", cutDen.c_str());

dataNumH->Divide(dataDenH);

dataNumH->Draw("E");

}


