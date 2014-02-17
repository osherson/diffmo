{

Double_t x[9] = { 1.,  
		  2.,
		  3.,
		  4.,
		  5.,
		  6.,
		  7., 
		  8., 
		  9.
			};


Double_t y[9] = { 0.222,  //subSF
		  0.073,  //qcdNorm
		  0.929,  //scale
		  0.210,  //beta_sig
		  1.028,  //ptSmear
		  0.070,  //qcdShape
		  0.133,  //lumi
		  0.843,  //ttbarXS
	 	  0.166   //btagSF
			};


Double_t ey[9] = { 1.003,  //subSF
		   0.019,  //qcdNorm
		   0.502,  //scale
		   0.279,  //beta_sig
		   0.910,  //ptSmear
		   0.052,  //qcdShape
		   1.001,  //lumi
		   1.039,  //ttbarXS
	 	   1.002   //btagSF
			};

Double_t ex[9] = { 0., 0., 0., 0. ,0. ,0. ,0. ,0. ,0. };

Double_t exZ[11] = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
Double_t extend[11] = { 0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10. };
Double_t sig[11] = { 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1. };
Double_t twosig[11] = { 2., 2., 2., 2., 2., 2., 2., 2., 2., 2., 2. };


TGraphErrors *nuisance = new TGraphErrors(9, x, y, ex, ey);
TGraphErrors *grshade_1sig = new TGraphErrors(11, extend, exZ, exZ, sig);
TGraphErrors *grshade_2sig = new TGraphErrors(11, extend, exZ, exZ, twosig);


grshade_2sig->SetFillStyle(1001);
grshade_2sig->SetFillColor(kYellow);
grshade_2sig->Draw("A3");

grshade_1sig->SetFillStyle(1001);
grshade_1sig->SetFillColor(kGreen);
grshade_1sig->Draw("3 same");

nuisance->Draw("P same");
gPad->RedrawAxis();

}






