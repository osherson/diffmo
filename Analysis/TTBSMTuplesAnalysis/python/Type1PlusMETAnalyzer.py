import ROOT
import copy
from math import *
from operator import itemgetter, attrgetter

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *

class Type1PlusMETAnalyzer :
    """Run 1 + MET Analysis"""
    def __init__(self, useMC, outfile, mistagFile, collectionLabelSuffix, useGenWeight, triggerFile, pdfWeight="nominal", triggerWeight = "noWeight"):
        self.outfile = outfile
        self.mistagFileStr = mistagFile
        self.triggerFileStr = triggerFile
        self.useMC = useMC
        self.useGenWeight = useGenWeight
        self.pdfWeight = pdfWeight
        self.triggerWeight = triggerWeight
		
        label = 'ttbsmAna'+collectionLabelSuffix


        self.allTopTagHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagLabel  = ( label,   "topTagP4")

        self.allTopTagTopMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagTopMassLabel   = ( label,   "topTagTopMass" )
        self.allTopTagMinMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagMinMassLabel   = ( label,   "topTagMinMass" )
        self.allTopTagNSubjetsHandle    = Handle("std::vector<double>" )
        self.allTopTagNSubjetsLabel     = ( label,   "topTagNSubjets" )

        self.allTopTagPassHandle  = Handle( "std::vector<int>" )
        self.allTopTagPassLabel   = ( label,   "topTagPass" )

        self.weightsHandle = Handle( "double" )
        self.weightsLabel = ( label, "weight" )
        self.pdfHandle = Handle("std::vector<double>")
        self.pdfLabel = ( label, "pdfWeights")
        
        self.__book__()



    def __del__ (self):
        """(Internal) Destructor"""
        print 'Goodbye from Type11Analyzer. Before I go, I will delete some stuff'
        self.f.cd()

        self.pt0PredDist.GetPredictedHist().Write()
        self.pt0PredDist.GetObservedHist().Write()
        self.pt0PredDist.GetTaggableHist().Write()

        self.mtmetPredDist.GetPredictedHist().Write()
        self.mtmetPredDist.GetObservedHist().Write()
        self.mtmetPredDist.GetTaggableHist().Write()
        
        self.metPredDist.GetPredictedHist().Write()
        self.metPredDist.GetObservedHist().Write()
        self.metPredDist.GetTaggableHist().Write()

        self.metDphiPredDist.GetPredictedHist().Write()
        self.metDphiPredDist.GetObservedHist().Write()
        self.metDphiPredDist.GetTaggableHist().Write()

        
        for pair in sorted(self.runPairs, key=itemgetter(3)) :
            print '{0:12.2f} : Run {1:15.0f}, LumiBlock {2:15.0f}, Event {3:30.0f}'.format(
                pair[3], pair[0], pair[2], pair[1]
                )

        print '1'
        self.f.Write()
        self.f.Close()
        print '4'
        self.mistagFile.Close()
        self.triggerFile.Close()
        print '5'

        print 'So long!'

    def __book__( self ) :
        """(Internal) Books histograms"""

        print 'Booking histograms'
        self.mistagFile = ROOT.TFile(self.mistagFileStr + ".root")
        self.mistagFile.cd()
        self.mistag = self.mistagFile.Get("TYPE12_KIN_MISTAG_SUBTRACT_TTBAR").Clone()
        self.mistag.SetName('mistag')
        ROOT.SetOwnership( self.mistag, False )
        
        self.triggerFile = ROOT.TFile(self.triggerFileStr + ".root")
        self.triggerFile.cd()
        self.triggerHist = self.triggerFile.Get("TYPE11_TRIGGER_EFFIC").Clone()
        self.triggerHist.SetName('triggerHist')
        ROOT.SetOwnership( self.triggerHist, False )

        self.f = ROOT.TFile( self.outfile + ".root", "recreate" )
        self.f.cd()

        self.pt0PredDist                 = ROOT.PredictedDistribution( self.mistag, "pt0PredDist",     "Jet p_{T}",       1000, 0,  5000 )
        self.mtmetPredDist               = ROOT.PredictedDistribution( self.mistag, "mtmetPredDist",   "mTT Mass",        1000, 0,  5000 )
        self.metPredDist                 = ROOT.PredictedDistribution( self.mistag, "metPredDist",     "MET",             1000, 0,  5000 )
        self.metDphiPredDist             = ROOT.PredictedDistribution( self.mistag, "metDphiPredDist", "MET #Delta #phi", 1000, ROOT.TMath.Pi(), ROOT.TMath.Pi() )

        ROOT.SetOwnership( self.pt0PredDist, False )
        ROOT.SetOwnership( self.mtmetPredDist, False )
        ROOT.SetOwnership( self.metPredDist, False )
        ROOT.SetOwnership( self.metDphiPredDist, False )

        self.jetEta               = ROOT.TH1D("jetEta",               "jetEta",            100, -4,    4)
        self.jetMass              = ROOT.TH1D("jetMass",              "jetMass",        100,  0,  500 )
        self.jetPt                = ROOT.TH1D("jetPt",                "jetPt",          400,  0,  2000 )    
        self.jetPtOneTag          = ROOT.TH1D("jetPtOneTag",          "jetPtOneTag",    400,  0,  2000 )
        self.jetMinMass           = ROOT.TH1D("jetMinMass",           "jetMinMass",          400,  0,  200 )
        self.topTagMass           = ROOT.TH1D("topTagMass",           "Top Tag Mass",             100,  0,  500 )
        self.topTagMinMass        = ROOT.TH1D("topTagMinMass",               "Top Tag MinMass",             100,  0,  200 )
        self.topTagPt             = ROOT.TH1D("topTagPt",                    "Top Tag Pt",               400,  0,  2000 )
        self.mttCandMass          = ROOT.TH1D("mttCandMass",                     "mTT Cand Mass",                 1000, 0,  5000 )
        self.mttMass              = ROOT.TH1D("mttMass",                     "mTT Mass",                 1000, 0,  5000 )
        self.mttMassTriggerWeighted   = ROOT.TH1D("mttMassTriggerWeighted",                     "mTT Mass",                 1000, 0,  5000 )
        self.cutflow              = ROOT.TH1D("cutflow",                     "cutflow",                 7, 0,  7 ) 
        
        self.mttMass.Sumw2()
        self.runPairs = []
        self.mttMassTriggerWeighted.Sumw2()

    def analyze(self, event) :
        """Analyzes event"""
        
        event.getByLabel (self.allTopTagLabel, self.allTopTagHandle)
        topJets = self.allTopTagHandle.product()
         
        self.cutflow.Fill(0.5,1)    

        nTopCand = 0
        for i in range(0,len(topJets) ) :
          if( topJets[i].pt() > 450 ) :
            nTopCand = nTopCand + 1

        if nTopCand != 1 :
            return

        #pairMass = 0.0
        #ttMass = 0.0

        weight = 1.0
        if self.useGenWeight :
            event.getByLabel( self.weightsLabel, self.weightsHandle )
            weight = self.weightsHandle.product()[0]

        if self.triggerWeight != "noWeight" :
            jetTriggerWeight = 1.0
            if topJets[0].pt() < 800:
                bin0 = self.triggerHist.FindBin(topJets[0].pt()) 
                jetTriggerWeight = self.triggerHist.GetBinContent(bin0)
                deltaTriggerEff  = 0.5*(1.0-jetTriggerWeight)
                jetTriggerWeightUp  =   jetTriggerWeight + deltaTriggerEff
                jetTriggerWeightDown  = jetTriggerWeight - deltaTriggerEff
                jetTriggerWeightUp  = min(1.0,jetTriggerWeightUp)
                jetTriggerWeightDown  = max(0.0,jetTriggerWeightDown)
                if self.triggerWeight == "Nominal" :
                    weight = weight*jetTriggerWeight
                if self.triggerWeight == "Up" :
                    weight = weight*jetTriggerWeightUp
                if self.triggerWeight == "Down" :
                    weight = weight*jetTriggerWeightDown


        #print 'topJets[0].pt() ' + str(topJets[0].pt())    
        #print 'jetTriggerWeight ' + str(jetTriggerWeight)    

        event.getByLabel (self.allTopTagMinMassLabel, self.allTopTagMinMassHandle)
        topJetMinMass= self.allTopTagMinMassHandle.product()
        event.getByLabel (self.allTopTagNSubjetsLabel, self.allTopTagNSubjetsHandle)
        topJetNSubjets= self.allTopTagNSubjetsHandle.product()
        event.getByLabel (self.allTopTagTopMassLabel, self.allTopTagTopMassHandle)
        topJetMass= self.allTopTagTopMassHandle.product()
        event.getByLabel (self.allTopTagPassLabel, self.allTopTagPassHandle )
        topJetPass= self.allTopTagPassHandle.product()


        if self.pdfWeight != "nominal" :
            iweight = 0.0
            event.getByLabel( self.pdfLabel, self.pdfHandle )
            pdfs = self.pdfHandle.product()
            if self.pdfWeight == "up" :
                for pdf in pdfs[0::2] :
                    iweight = iweight + pdf*pdf
            else :
                for pdf in pdfs[1::2] :
                    iweight = iweight + pdf*pdf
            iweight = iweight / len(pdfs) * 0.5
            weight = sqrt(weight*weight + iweight*iweight)

        ttMass = 0.0


        jetP4Sum = ROOT.TLorentzVector()
        met_ex = 0.0
        met_ey = 0.0
        for i in range(0,len(topJets)) :
            if topJets[i].pt() > 30.0:
                met_ex += topJets[i].energy() * cos(topJets[i].phi())
                met_ey += topJets[i].energy() * sin(topJets[i].phi())

        mht = sqrt( met_ex * met_ex + met_ey * met_ey )
        leadJet = ROOT.TLorentzVector(topJets[0].px(),
                                      topJets[0].py(),
                                      topJets[0].pz(),
                                      topJets[0].energy() )
        negSumJets = ROOT.TLorentzVector( met_ex, met_ey, 0, mht)
        negSumJets.SetPhi( ROOT.TMath.Pi() - negSumJets.Phi() )
        deltaPhi = leadJet.DeltaPhi( negSumJets )

        #ptCuts = topJets[0].pt() > 450 and topJets[1].pt() > 450
        ptCuts = topJets[0].pt() > 450 
        #        etaCuts = abs(topJets[0].eta()) < 2.4
        etaCuts = True
        deltaPhiCut = abs(deltaPhi)>2.1
        passType11KinCuts   = ptCuts and etaCuts and deltaPhiCut

        if ptCuts:
            self.cutflow.Fill(1.5,1)  
            if etaCuts:
                self.cutflow.Fill(2.5,1)  
                if deltaPhiCut:
                    self.cutflow.Fill(3.5,1) 
                    topMassCuts = topJetMass[0] > 140 and topJetMass[0] < 250 
                    if topMassCuts:
                        self.cutflow.Fill(4.5,1)
                        nSubjetsCuts = topJetNSubjets[0] > 2 
                        if nSubjetsCuts:
                            self.cutflow.Fill(5.5,1)
                            minMassCuts = topJetMinMass[0] > 50 
                            if minMassCuts:
                                self.cutflow.Fill(6.5,1)
 

        if passType11KinCuts :

            topTag0Mass    = topJetMass[0] > 140 and topJetMass[0] < 250
            topTag0Sub     = topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
            ttMass   = (leadJet + negSumJets).Mt()
            
            ## myrand = ROOT.gRandom.Uniform(140,250)
            ## jet0P4_massFlat = copy.copy(topJets[0])
            ## jet0P4_massFlat.SetM( myrand )
            ## ttMassJet0MassFlat = (topJets[1]+jet0P4_massFlat).mass()
            
            ## myrand2 = ROOT.gRandom.Uniform(140,250)
            ## jet1P4_massFlat = copy.copy(topJets[1])
            ## jet1P4_massFlat.SetM( myrand2 )
            ## ttMassJet1MassFlat = (topJets[0]+jet1P4_massFlat).mass()
            

            ## ttMassMod3Jet0Flat = ttMass
            ## ttMassMod3Jet1Flat = ttMass
            ## if topJets[0].mass() < 140 or topJets[0].mass() > 250:
            ##     ttMassMod3Jet0Flat = ttMassJet0MassFlat
            ## if topJets[1].mass() < 140 or topJets[1].mass() > 250:
            ##     ttMassMod3Jet1Flat = ttMassJet1MassFlat
            


            self.jetMass.Fill( topJets[0].mass(), weight )
            self.jetPt.Fill( topJets[0].pt(), weight )
            self.jetEta.Fill( topJets[0].eta(), weight )
            self.jetMinMass.Fill( topJetMinMass[0], weight )
            self.mttCandMass.Fill( ttMass, weight )
            

            if topTag0Sub :
                self.topTagMass.Fill( topJets[0].mass(), weight )
            if topTag0Mass :
                self.topTagMinMass.Fill( topJetMinMass[0], weight )

            if topTag0Sub and topTag0Mass :
                self.topTagPt.Fill( topJets[0].pt(), weight )
                self.mttMass.Fill( ttMass, weight )
                if not self.useMC :
                    self.runPairs.append( [event.object().id().run(),
                                           event.object().id().event(),
                                           event.object().id().luminosityBlock() ,
                                           ttMass] )
                self.mttMassTriggerWeighted.Fill( ttMass, weight )   

            #background estiation

            self.pt0PredDist.               Accumulate( topJets[0].pt(),    topJets[0].pt(), topTag0Sub and topTag0Mass, weight )
            self.mtmetPredDist.             Accumulate( ttMass,             topJets[0].pt(), topTag0Sub and topTag0Mass, weight )
            self.metPredDist.               Accumulate( mht,                topJets[0].pt(), topTag0Sub and topTag0Mass, weight )
            self.metDphiPredDist.           Accumulate( deltaPhi,           topJets[0].pt(), topTag0Sub and topTag0Mass, weight )
            ## self.mtmetPredDistMassCut.      Accumulate( ttMass,             topJets[1].pt(), topTag1, weight )
            ## self.mtmetPredDistModMassFlat.  Accumulate( ttMassJet1MassFlat, topJets[1].pt(), topTag1, weight )
            ## self.mtmetPredDistMod3MassFlat. Accumulate( ttMassMod3Jet1Flat, topJets[1].pt(), topTag1, weight )
            ## self.mtmetPredDistMod3MassFlatSubtract. Accumulate( ttMassMod3Jet1Flat, topJets[1].pt(), topTag1, weight )
            


