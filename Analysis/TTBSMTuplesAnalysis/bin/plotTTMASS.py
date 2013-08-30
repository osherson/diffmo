from ROOT import *
from array import array
gROOT.Macro("~/rootlogon.C")
gROOT.ForceStyle()
gROOT.SetBatch(0)

if __name__ ==  "__main__" :
    #dataFile = TFile("../crossChecks/Jet_Run2011A-May10ReReco_ttbsm_v6_ttbsmTuples_v3.root")
    #dataFile = TFile("Jet_Run2011A-May10ReReco_ttbsm_v6_ttbsmTuples_v3_KinOnlyBkg.root")
    dataFile = TFile("all.root")
    ttbarFile = TFile("JAN15/ttjets_m700_type11__TriggerNominal.root")
    ttbarFile2 = TFile("JAN15/ttjets_m1000_type11__TriggerNominal.root")
    signalFile2 = TFile("JAN15/RSGluon_2000_type11__TriggerNominal.root")
    signalFile1 = TFile("JAN15/RSGluon_1000_type11__TriggerNominal.root")
    signalFile3 = TFile("JAN15/RSGluon_3000_type11__TriggerNominal.root")



    lumi = 7052.
    ttXS = 234.0

    subSF = 0.926
    cleanupSF = 0.95

    nbins = 3
    bins = array('d',[0,1000,2000,5000])
    mttRebin = 10

    mea = dataFile.Get( "mttMass" )
    ttbarHist = ttbarFile.Get("mttMass" )
    ttbarHist.Scale(cleanupSF*subSF*subSF*lumi*ttXS*0.074/ 3082812. )
    ttbarHist2 = ttbarFile2.Get("mttMass" )
    ttbarHist2.Scale(cleanupSF*subSF*subSF*lumi*ttXS*0.014 / 1249111.)

    ttbarHist.Add(ttbarHist2)

    ttbarHist.GetYaxis().SetTitle("Events / 100 GeV/c^{2}")
    ttbarHist.GetYaxis().SetTitleOffset(1.5) 

    bkg = dataFile.Get( "mttPredDistModMassQCDdistSubtract_pred")
    #bkg = dataFile.Get( "mttPredDistModQCDMass_pred" )
    #bkg = dataFile.Get( "mttBkgShapeWithSideBand4Mass")

    #ttbar_mistag = ttbarFile.Get("mttPredDistModQCDMass_pred")  
    #ttbar_mistag = ttbarFile.Get("jet1PtPredDist_pred")  
    ttbar_mistag = ttbarFile.Get("mttPredDistModMassQCDdistSubtract_pred")  
    ttbar_mistag.Scale(subSF*subSF*lumi*ttXS*0.074/ 3082812. )
    ttbar2_mistag = ttbarFile2.Get("mttPredDistModMassQCDdistSubtract_pred")
    ttbar2_mistag.Scale(subSF*subSF*lumi*ttXS*0.014/ 1249111. )
    bkg.Add(ttbar_mistag, -1)
    bkg.Add(ttbar2_mistag, -1)
    print ttbar_mistag.Integral()


    ttbarHist.Sumw2()
    bkg.Sumw2()


    sig1 = signalFile1.Get( "mttMass" )
    sig1.Scale(subSF*subSF*lumi*1 / 90000. ) 

    sig2 = signalFile2.Get( "mttMass" )
    sig2.Scale(subSF*subSF*lumi*1 / 90000. ) 

    sig3 = signalFile3.Get( "mttMass" )
    sig3.Scale(subSF*subSF*lumi*1 / 90000. ) 

   # print (zp1.Integral() / 179315.)
#    print zp2.Integral() / 179315. 
#    print zp3.Integral() / 179315. 

#    bkg = dataFile.Get( "mttBkgWithMistagMod2Mass" )
#    bkg = dataFile.Get( "mttBkgWithMistag" )
#    ttMistagBkg = ttbarFile.Get("mttBkgWithMistag")
#    ttMistagBkg.Scale( lumi*ttXS/1285626. )
#    print ttbarHist.Integral(), ttMistagBkg.Integral()

    print mea.GetNbinsX()
    print ttbarHist.GetNbinsX()
    print bkg.GetNbinsX()


    c = TCanvas("c","c",1000,700)

    main = TPad("main", "main", 0, 0.3, 1, 1)
    sub = TPad("sub", "sub", 0, 0, 1, 0.3)
    main.Draw()
    sub.SetFillStyle(4000)
    sub.Draw()
    main.cd()

    nbins = ttbarHist.GetNbinsX()

    
    #mea.Rebin( nbins, "mea", bins )
    #ttbarHist.Rebin( nbins, "ttbarHist", bins)
    #bkg.Rebin( nbins, "bkg", bins)

    mea.Rebin( mttRebin)
    ttbarHist.Rebin( mttRebin)
    bkg.Rebin( mttRebin) 
    sig1.Rebin( mttRebin)
    sig2.Rebin( mttRebin)
    sig3.Rebin( mttRebin)

    print mea.GetNbinsX()
    print ttbarHist.GetNbinsX()
    print bkg.GetNbinsX()
    
    for bin in range(nbins):
	ttbarHist.SetBinError(bin, 0.50*ttbarHist.GetBinContent(bin))
	print 0.50*ttbarHist.GetBinContent(bin)
	if bkg.GetBinContent(bin) > 0: 
		bkgerr = sqrt(.048*bkg.GetBinContent(bin)*.048*bkg.GetBinContent(bin) + bkg.GetBinContent(bin))
	else:
		bkgerr = 0
	bkg.SetBinError(bin, bkgerr)



    print 'TTJets : ', ttbarHist.Integral()
    print 'QCD :', bkg.Integral()
    print 'DATA :', mea.Integral()
    print 'Signal : ', sig2.Integral()

    mea.SetLineColor( kBlack )
    mea.SetMarkerStyle( 20 )
    mea.SetMarkerColor( kBlack )

    ttbarHist.SetFillColor( kRed )
    bkg.SetFillColor( kYellow )

    #sig.SetFillColor( kGreen - 1 )

    sig1.SetLineColor(kGreen - 1)
    sig1.SetLineWidth(2)

    sig2.SetLineColor(kBlue - 1)
    sig2.SetLineWidth(2)

    sig3.SetLineColor(kViolet - 1)
    sig3.SetLineWidth(2)


    totalH = bkg.Clone("totalH")
    totalH.Add(ttbarHist)  

    errorH = totalH.Clone("errorH")   


    for bin in range(nbins):
	errorH.SetBinContent(bin, totalH.GetBinError(bin))


 
    stack = THStack("ttMass", "; m_{t#bar{t}} (GeV/c^{2});Events / 50 GeV/c^{2}")
    #stack = THStack("ttMass", "; Tag Jet p_{T} (GeV/c); # Events / 50 GeV")
    stack.Add( ttbarHist, "Hist" )
    stack.Add( bkg, "Hist" )
    #stack.Add( sig, "Hist" )
    


    leg = TLegend( 0.6, 0.4, 0.85, 0.8 )
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)

    leg.AddEntry( mea, "Data", "LP" )
    leg.AddEntry( bkg, "QCD Multijet", "F" )
    leg.AddEntry( ttbarHist, "t#bar{t}", "F" )
    leg.AddEntry( totalH, "Total Uncertainty", "F")
    leg.AddEntry( sig1, "1 TeV RS KK Gluon (1 pb)", "L")
    leg.AddEntry( sig2, "2 TeV RS KK Gluon (1 pb)", "L")
    leg.AddEntry( sig3, "3 TeV RS KK Gluon (1 pb)", "L")


    stack.SetMaximum( max(mea.GetMaximum(),stack.GetMaximum()) * 1.2 )
    stack.Draw()
    stack.GetXaxis().SetRangeUser(500,3000)
    stack.GetYaxis().SetTitleOffset(1.3)
    mea.Draw("samee")
  
    #rs2.Draw("same")
    #rs3.Draw("same")
    totalH.SetFillStyle(3002)
    totalH.SetFillColor(kBlack)
    totalH.Draw("same E2")
    sig1.Draw("hist same")
    sig2.Draw("hist same")
    sig3.Draw("hist same")

    leg.Draw()
    prelim = TLatex()
    prelim.SetNDC()
    #prelim.DrawLatex( 0.4, 0.91, "#scale[0.8]{CMS Preliminary, 8 TeV, 9.8 fb^{-1}, Type I+II}" )
    prelim.DrawLatex( 0.4, 0.91, "#scale[0.8]{CMS Preliminary, 8 TeV, 19.6 fb^{-1}, Type I+I}" )
    #prelim.DrawLatex( 0.5, 0.91, "#scale[0.8]{CMS Preliminary, 8 TeV, 1.4 fb^{-1}}" )

    sub.cd()
    ratio = mea.Clone("ratio")
    ratio.Add(totalH, -1)
    subH = ratio.Clone("sub")
    subH.Divide(totalH)

    #newbins = array('i', [500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000,2050,2100,2150,2200,2250,2300,2350,2400,2400,2600,2800,3000,5000])
    ratio.Sumw2()
    #ratio.Rebin(len(newbins) - 1, "ratio", newbins)
    ratio.Divide(errorH)


    outfile = TFile("errors.root", "RECREATE")
    outfile.cd()
    errorH.Write()




    ratio.GetXaxis().SetRangeUser(500,3000)
    ratio.GetYaxis().SetRangeUser(-4,4)
    ratio.GetYaxis().CenterTitle(1)
    ratio.GetYaxis().SetTitle("N_{sigma}")
    ratio.GetYaxis().SetTitleSize(0.10)
    #ratio.GetYaxis().SetTicks("-")
    ratio.GetYaxis().SetLabelSize(0.11)
    ratio.GetXaxis().SetLabelSize(0.11)
    ratio.GetYaxis().SetTitleOffset(0.3)
    #ratio.GetYaxis().SetTitle("Data / Background")
    ratio.Draw("hist")
    #rs2.Draw("hist same")
    
    f = TF1("f", "0", 0, 10000)
    f.Draw("same")
    ratio.Draw("hist same") 
    sub2 = TPad("sub2", "sub2", 0, 0, 1, 1)
    sub2.SetFillStyle(4000)
    sub2.Draw()
    sub2.cd()
    subH.GetXaxis().SetRangeUser(500,3000)
    subH.GetXaxis().SetTitle("m_{t#bar{t}} (GeV/c^{2})")
    subH.GetYaxis().SetRangeUser(-2.0,2.0)
    subH.GetYaxis().CenterTitle(1)
    subH.SetLineColor(kRed)
    subH.SetMarkerColor(kRed)
    subH.GetYaxis().SetAxisColor(kRed)
    subH.GetYaxis().SetLabelColor(kRed)
    subH.GetYaxis().SetTitleColor(kRed)
    subH.GetYaxis().SetTitle("#frac{Data - Predicted}{Predicted}")
    subH.GetYaxis().SetTitleSize(0.10)
    subH.GetYaxis().SetLabelSize(0.11)
    subH.GetXaxis().SetLabelSize(0.11)
    subH.GetYaxis().SetTitleOffset(0.4)
    #subH.Scale(gPad.GetUymax() / subH.GetMaximum() )
    subH.Draw("Y+")
    f.Draw("same")
    subH.Draw("Y+ same")
    sub.cd()
    ratio.Draw("hist same")

 
    #Raxis = TGaxis(sub.GetUxmax(), -0.5, sub.GetUxmax(), 0.5, -0.5, 0.5, 510, "+L")
    #Raxis.SetLineColor(kRed)
    #Raxis.SetLabelColor(kRed)
    #Raxis.SetTitleColor(kRed)
    #Raxis.SetTitle("#frac{Data - Predicted}{Predicted}")
    #Raxis.Draw()
    #errorH.Draw("same E2")
    #ratio.Fit("pol0", "", "", 500, 2000)
	


    c.SaveAs("TTMASS_TYPE11_2012.pdf")

    main.SetLogy(1) 
    main.Print("TTMASS_TYPE11_LOG.png")


