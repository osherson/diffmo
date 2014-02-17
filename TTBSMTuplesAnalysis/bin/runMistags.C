{

string condLabels[10] = { "Inclusive",
								  "InclNsub",
								  "2btagT",
								  "InclNsub",
								  "2btagTNsub",
								  "1btagL",
								  "2btagL",
								  "1btagLNsub",
								  "2btagLNsub",
								  "InclNsub" };
								  
for (int i = 1; i < 2 ; i++){

	TreeFiller("Dec3_QCD_300_ttpair.root", "Dec3_QCD_300_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
	TreeFiller("Dec3_QCD_470_ttpair.root", "Dec3_QCD_470_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
	TreeFiller("Dec3_QCD_600_ttpair.root", "Dec3_QCD_600_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
	TreeFiller("Dec3_QCD_800_ttpair.root", "Dec3_QCD_800_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
	TreeFiller("Dec3_QCD_1000_ttpair.root", "Dec3_QCD_1000_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
	TreeFiller("Dec3_QCD_1400_ttpair.root", "Dec3_QCD_1400_"+condLabels[i]+"_all.root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
}

}
