from ROOT import *
from array import *
gROOT.Macro("rootlogon.C")
gROOT.ForceStyle()


def SetHistStyle( hist, x ) :
    colors = range(1,10)
    hist.SetLineColor(colors[x])
    hist.SetMarkerStyle( 20+x )
    hist.SetMarkerColor( colors[x] )




if __name__   ==  "__main__"  :
    
    f = TFile("Mistag_Jet_PD_May10ReReco_PromptReco_range1_range2_v4_mistag.root")

    pretagHists = [
      "topJetCandPtAll",  "topJetCandMassAll",  "topJetNsubsAll", "topJetMinMassAll" ]
    type2tagHists = [ 
      "topJetCandPtSignal", "topJetCandMassSignal",  "topJetNsubsSignal",  "topJetMinMassSignal"  ]
    titles = [
      ";Jet 1 pT (GeV/c^{2}); Number", 
      ";Jet 1 Mass (GeV/c^{2}); Number",
      ";Jet 1 NSubjets; Number",
      ";Jet 1 Min Mass (GeV/c^{2}); Number"
    ]
    rebins = [ 2, 2,  1,  2           ]

    canvs = []
    stacks = []
    hists = []
    legs = []

    for i in range(0,len(pretagHists) ) :
        c = TCanvas(type2tagHists[i], type2tagHists[i])
        pretag = f.Get( pretagHists[i] )
        type2tag = f.Get( type2tagHists[i] )
        stack = THStack( pretagHists[i], titles[i] )
        SetHistStyle( pretag, 0 )
        SetHistStyle( type2tag, 1 )
        pretag.Sumw2()
        type2tag.Sumw2()
        pretag.Scale( 1./pretag.Integral() )
        type2tag.Scale( 1./type2tag.Integral() )
        pretag.Rebin( rebins[i] )
        type2tag.Rebin( rebins[i] )

        stack.Add( pretag, "e" )
        stack.Add( type2tag, "e" )
        leg = TLegend( 0.6, 0.6, 0.85, 0.85 )
        leg.SetFillStyle(0)
        leg.AddEntry( pretag, "Before Type 2 Selection", "LP" )
        leg.AddEntry( type2tag, "Type 2 Signal Region", "LP" )

        stack.Draw("nostack")
        leg.Draw()
        c.Print( type2tagHists[i] + "_VS_All.pdf" )
        c.Print( type2tagHists[i] + "_VS_All.png" )
        legs.append( leg )
        hists.append( [pretag, type2tag] )
        stacks.append(stack)
        canvs.append(c)

    #numer =   [ "type2KinTopTag", "type2TopTag",  "leadTopTag", "secTopTag",  "randomTopTag", "antitagTag" ]
    #denom =   [ "type2KinTopProbe", "type2TopProbe", "leadTopProbe", "secTopProbe",  "randomTopProbe", "antitagProbe"  ]
    #titles  = ["Type 1+2 Kin Cuts Only", "Type 2 Signal Region",    "Lead Jet Dijet Sample",  "Sec Jet Dijet Sample", 
    #          "Random Jet Dijet Sample",  "Antitag Probe Dijet Sample"]
    #hisTitles = [ "TYPE12_KIN_MISTAG",  "TYPE12_SR_MISTAG", "LEADJET_MISTAG", "SECJET_MISTAG", "RANDOM_MISTAG",
    #              "ANTITAG_MISTAG" ]
    numer =   [ "type2KinTopTag", "type2TopTag", "type2SideBandTag", "randomTopTag", "type2TopTag800GeV" ]
    denom =   [ "type2KinTopProbe", "type2TopProbe", "type2SideBandProbe", "randomTopProbe", "type2TopProbe800GeV" ]
    titles  = ["Type 1+2 Kin Cuts Only", "Type 2 Signal Region",   "Type 2 SideBand",  "Random Jet Dijet Sample", "800GeVBin" ]
    hisTitles = [ "TYPE12_KIN_MISTAG",  "TYPE12_SR_MISTAG", "TYPE12_SB_MISTAG", "RANDOM_MISTAG", "TYPE12_800GeV" ]
    stack = THStack("mistag", "Top Mistag Measurement; Jet pT (GeV/c)" )
    leg = TLegend( 0.15, 0.6, 0.4, 0.9 )
    leg.SetFillStyle(0)
    xbins = array( 'd',
                [350, 360,  370,  380,  390,  400, 410,  420, 430,  440,  460,  480,  500,  
                540,  580,  620,  640,  680,  720,  800, 1000, 1200  ] )
    
    outfile = TFile("MISTAG_TYPE12_NEW.root", "RECREATE")
    outfile.cd()
    for i in range(0,len(numer) ) :
        tagHist = (f.Get( numer[i] )).Rebin( len(xbins)-1, hisTitles[i], xbins )
        probeHist = (f.Get( denom[i] )).Rebin( len(xbins)-1, "probeHist_"+hisTitles[i], xbins )
        tagHist.Sumw2()
        probeHist.Sumw2()
        tagHist.Divide( probeHist )
        SetHistStyle( tagHist, i )
        stack.Add( tagHist )
        hists.append( [tagHist, probeHist] )
        leg.AddEntry( tagHist,  titles[i], "LP" )
        if numer[i] == "type2KinTopTag" :
            type2TopMistag = tagHist.Clone("type2TopMistag")
            type2TopMistag.SetTitle(";Jet p_{T} (GeV/c); Top Mistag Rate")
            type2TopMistag.SetMarkerStyle(20)
            type2TopMistag.SetStats(kFALSE)
            cgood = TCanvas('cgood', 'cgood')
            hists.append( [type2TopMistag] )
            type2TopMistag.Draw()
            prelim = TLatex()
            prelim.SetNDC()
            prelim.DrawLatex(0.2,0.85, "#scale[0.8]{CMS Preliminary, 865 pb^{-1}}")
            cgood.Print("TYPE12_TOP_MISTAG.pdf")
            cgood.Print("TYPE12_TOP_MISTAG.png")
            canvs.append(cgood)
    d = TCanvas('d','d')
    stack.Draw("nostack")
    stack.GetXaxis().SetRangeUser(350,800)
    leg.Draw()
    d.Print("TOP_MISTAG.pdf")
    d.Print("TOP_MISTAG.png")
    hist = (f.Get( "type2TopTagMass" )).Clone( "TYPE2_TOP_TAG_MASS" )
    outfile.Write()
    outfile.Close()
    hists.append( hist )
    stacks.append(stack)
    canvs.append(c)


