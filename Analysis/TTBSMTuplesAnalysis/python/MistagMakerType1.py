import ROOT
import copy

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *

class MistagMakerType1 :
    """Run 1 + 1 Mistag Rate"""
    def __init__(self, outfile, useGenWeight=False):
        self.outfile = outfile
        self.useGenWeight=useGenWeight
        
        self.allTopTagHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagLabel  = ( "ttbsmAna",   "topTagP4")

        self.allTopTagTopMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagTopMassLabel   = ( "ttbsmAna",   "topTagTopMass" )
        self.allTopTagMinMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagMinMassLabel   = ( "ttbsmAna",   "topTagMinMass" )
        self.allTopTagNSubjetsHandle = Handle("std::vector<double>" )
        self.allTopTagNSubjetsLabel  = ( "ttbsmAna",   "topTagNSubjets" )
        
        self.allTopTagSubjetBDiscHandle = Handle("std::vector<double>")
        self.allTopTagSubjetBDiscLabel = ("ttbsmAna", "topTagSubjetBDisc")

        self.allTopTagPassHandle  = Handle( "std::vector<int>" )
        self.allTopTagPassLabel   = ( "ttbsmAna",   "topTagPass" )
        
        self.weightsHandle = Handle( "double" )
        self.weightsLabel = ( "ttbsmAna", "weight" )


        self.__book__()



    def __del__ (self):
        """(Internal) Destructor"""
        print 'Goodbye from MistagMakerType1. Before I go, I will delete some stuff'
        self.f.cd()
        print '1'
        self.f.Write()
        self.f.Close()
        print '4'
        print '5'

        print 'So long!'

    def __book__( self ) :
        """(Internal) Books histograms"""

        print 'Booking histograms'
        self.f = ROOT.TFile( self.outfile + ".root", "recreate" )
        self.f.cd()

        self.topTagPt             = ROOT.TH1D("topTagPt",             "Top Tag Pt",               400,  0,  2000 )
        self.topProbePt           = ROOT.TH1D("topProbePt",           "Top Probe Pt",             400,  0,  2000 )
        self.testTagPt            = ROOT.TH1D("testTagPt",            "Top Tag Pt",               400,  0,  2000 )
        self.testProbePt          = ROOT.TH1D("testProbePt",          "Top Probe Pt",             400,  0,  2000 )
        self.lowmMinTagPt            = ROOT.TH1D("lowmMinTagPt",            "Top Tag Pt",               400,  0,  2000 )
        self.lowmMinProbePt          = ROOT.TH1D("lowmMinProbePt",          "Top Probe Pt",             400,  0,  2000 )
        self.lowmMinBTagPt            = ROOT.TH1D("lowmMinBTagPt",            "Top Tag Pt",               400,  0,  2000 )
        self.lowmMinBProbePt          = ROOT.TH1D("lowmMinBProbePt",          "Top Probe Pt",             400,  0,  2000 )
        self.lowmMin2BTagPt            = ROOT.TH1D("lowmMin2BTagPt",            "Top Tag Pt",               400,  0,  2000 )
        self.lowmMin2BProbePt          = ROOT.TH1D("lowmMin2BProbePt",          "Top Probe Pt",             400,  0,  2000 )
       
	self.antiBTagPt = ROOT.TH1D("antiBTagPt",          "Top Tag Pt",             400,  0,  2000 ) 
	self.antiBProbePt = ROOT.TH1D("antiBProbePt",          "Top Probe Pt",             400,  0,  2000 ) 


        
        self.topTagPt.Sumw2()
        self.topProbePt.Sumw2()
        self.testTagPt.Sumw2()
        self.testProbePt.Sumw2()
        self.lowmMinTagPt.Sumw2()
        self.lowmMinProbePt.Sumw2()
        self.lowmMinBTagPt.Sumw2()
        self.lowmMinBProbePt.Sumw2()
        self.lowmMin2BTagPt.Sumw2()
        self.lowmMin2BProbePt.Sumw2()
	self.antiBTagPt.Sumw2()
	self.antiBProbePt.Sumw2()

        
    def analyze(self, event) :
        """Analyzes event"""
        event.getByLabel (self.allTopTagLabel, self.allTopTagHandle)
        topJets = self.allTopTagHandle.product()

        nTopCand = 0
        for i in range(0,len(topJets) ) :
          if( topJets[i].pt() > 350 ) :
            nTopCand = nTopCand + 1

        if nTopCand < 2 :
            return

        event.getByLabel (self.allTopTagMinMassLabel, self.allTopTagMinMassHandle)
        topJetMinMass= self.allTopTagMinMassHandle.product()
        event.getByLabel (self.allTopTagNSubjetsLabel, self.allTopTagNSubjetsHandle)
        topJetNSubjets= self.allTopTagNSubjetsHandle.product()
        event.getByLabel (self.allTopTagTopMassLabel, self.allTopTagTopMassHandle)
        topJetMass= self.allTopTagTopMassHandle.product()
        event.getByLabel (self.allTopTagPassLabel, self.allTopTagPassHandle )
        topJetPass= self.allTopTagPassHandle.product()
        event.getByLabel (self.allTopTagSubjetBDiscLabel, self.allTopTagSubjetBDiscHandle )
	topJetSubjetBDisc = self.allTopTagSubjetBDiscHandle.product()
		
		

        weight = 1.0
        if self.useGenWeight :
            event.getByLabel( self.weightsLabel, self.weightsHandle )
            weight = self.weightsHandle.product()[0]


        deltaPhi = topJets[0].phi() - topJets[1].phi()
        if deltaPhi > ROOT.TMath.Pi():
            deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
        if deltaPhi < -ROOT.TMath.Pi():
            deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()

        ptCuts = topJets[0].pt() > 350 and topJets[1].pt() > 350
        deltaY = (topJets[0].Rapidity() - topJets[1].Rapidity())
        etaCuts = abs(deltaY) < 1.0 and abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4
        deltaPhiCut = abs(deltaPhi)>2.1
        passType11KinCuts   = ptCuts #and etaCuts and deltaPhiCut
    
    
    	Jet0BDiscs = []
    	Jet1BDiscs = []
    
    
        i = 0   
        while i < (topJetNSubjets[0]):
            Jet0BDiscs.append(topJetSubjetBDisc[i])
            i = i + 1
        
        i = 0
        while i < (topJetNSubjets[1]):
        	Jet1BDiscs.append(topJetSubjetBDisc[i+int(topJetNSubjets[0])])
        	i = i + 1
    
    	jet0_maxB = max(Jet0BDiscs)
    	jet1_maxB = max(Jet1BDiscs)
    
    
        topTag0        = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        topTag1        = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
        failMinMass0   = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] < 50  
        failMinMass1   = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] < 50
	failMinMassLow0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] < 30
	failMinMassLow1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] < 30
	fail2Nsub0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetNSubjets[0] < 2
	fail2Nsub1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetNSubjets[1] < 2
	fail3Nsub0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetNSubjets[0] < 3
	fail3Nsub1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetNSubjets[1] < 3
	bTag0 = jet0_maxB > 0.679
	bTag1 = jet1_maxB > 0.679
 
        if passType11KinCuts :
            x = ROOT.gRandom.Uniform(1.0)        
            if x < 0.5 :
                if not topTag0 :
                    self.topProbePt.Fill( topJets[1].pt(), weight )    
                    if topTag1 :
                        self.topTagPt.Fill( topJets[1].pt(), weight )
                if failMinMass0 :
                    self.testProbePt.Fill( topJets[1].pt(), weight )
                    if topTag1 :
                        self.testTagPt.Fill( topJets[1].pt(), weight )
                if failMinMassLow0 :
                    self.lowmMinProbePt.Fill( topJets[1].pt(), weight )
                    if bTag1 and not bTag0:
			self.lowmMinBProbePt.Fill( topJets[1].pt(), weight )
                    if topTag1 :
                        self.lowmMinTagPt.Fill( topJets[1].pt(), weight )
		        if bTag1 :
		        	self.lowmMinBTagPt.Fill( topJets[1].pt(), weight ) 
		if failMinMassLow0 :
			if bTag1 and bTag0:
				self.lowmMin2BProbePt.Fill( topJets[1].pt(), weight )
			if topTag1 and bTag1 and bTag0:
				self.lowmMin2BTagPt.Fill( topJets[1].pt(), weight )
		if topTag0 and topTag1 and not bTag0:
			self.antiBProbePt.Fill( topJets[1].pt(), weight )
			if bTag1:
				self.antiBTagPt.Fill( topJets[1].pt(), weight)        

    
            if x >= 0.5 :
                if not topTag1 :
                    self.topProbePt.Fill( topJets[0].pt(), weight )
                    if topTag0 :
                        self.topTagPt.Fill( topJets[0].pt(), weight )
                if failMinMass1 :
                    self.testProbePt.Fill( topJets[0].pt(), weight )
                    if topTag0 :
                        self.testTagPt.Fill( topJets[0].pt(), weight )
                if failMinMassLow1 :
                    self.lowmMinProbePt.Fill( topJets[0].pt(), weight )
                    if bTag0 and not bTag1:
			self.lowmMinBProbePt.Fill( topJets[0].pt(), weight )
                    if topTag0 :
                        self.lowmMinTagPt.Fill( topJets[0].pt(), weight )
        		if bTag0:
       				self.lowmMinBTagPt.Fill( topJets[0].pt(), weight)
            	if failMinMassLow1 :
			if bTag1 and bTag0:
				self.lowmMin2BProbePt.Fill( topJets[0].pt(), weight )
			if topTag0 and bTag0 and bTag1 :
				 self.lowmMin2BTagPt.Fill( topJets[0].pt(), weight )
		if topTag0 and topTag1 and not bTag1:
			self.antiBProbePt.Fill( topJets[0].pt(), weight )
			if bTag0:
				self.antiBTagPt.Fill( topJets[0].pt(), weight)        
