{

string condLabels[10] = { "Inclusive",
								  "1btagT",
								  "2btagT",
								  "1btagL",
								  "2btagL",
								  "1btagTNsub",
								  "2btagTNsub",
								  "1btagLNsub",
								  "2btagLNsub",
								  "InclNsub" };
								  
for (int i = 0; i < 10 ; i++){

	TreeFiller("Oct7_data?.root", "Oct22_"+condLabels[i]+".root", "MISTAG_RATE_SUB_TTBAR_"+condLabels[i]);
}

}