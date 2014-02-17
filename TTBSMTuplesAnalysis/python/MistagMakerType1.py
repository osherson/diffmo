import ROOT
import copy

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *
from array import array


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
	self.allTopTagPartonHandle = Handle("std::vector<int>" )
	self.allTopTagPartonLabel = ( "ttbsmAna", "topTagPartonFlavour" )

        
        self.allTopTagSubjetBDiscHandle = Handle("std::vector<double>")
        self.allTopTagSubjetBDiscLabel = ("ttbsmAna", "topTagSubjetBDisc")

        self.allTopTagPassHandle  = Handle( "std::vector<int>" )
        self.allTopTagPassLabel   = ( "ttbsmAna",   "topTagPass" )
        
        self.allNsub1Handle = Handle( "std::vector<double>")
        self.allNsub2Handle = Handle( "std::vector<double>")
        self.allNsub3Handle = Handle( "std::vector<double>")
        self.allNsub4Handle = Handle( "std::vector<double>")
        self.allNsub1Label = ("ttbsmAna", "tau1")
        self.allNsub2Label = ("ttbsmAna", "tau2")
        self.allNsub3Label = ("ttbsmAna", "tau3")
        self.allNsub4Label = ("ttbsmAna", "tau4")
        
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
        self.lowmMinTagWP1Pt = ROOT.TH1D("lowmMinTagPtWP1",            "Top Tag Pt WP1",               400,  0,  2000 )
        self.lowmMinTagWP2Pt = ROOT.TH1D("lowmMinTagPtWP2",            "Top Tag Pt WP2",               400,  0,  2000 )
        self.lowmMinTagWP3Pt = ROOT.TH1D("lowmMinTagPtWP3",            "Top Tag Pt WP3",               400,  0,  2000 )
        self.lowmMinTagWP4Pt = ROOT.TH1D("lowmMinTagPtWP4",            "Top Tag Pt WP4",               400,  0,  2000 )
        self.lowmMinTagWP5Pt = ROOT.TH1D("lowmMinTagPtWP5",            "Top Tag Pt WP5",               400,  0,  2000 )
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

	self.mistagVars = ROOT.TTree('mistagVars', 'mistagVars')
	self.jet1Pt = array('f', [-1.])
	self.jet2Pt = array('f', [-1.])
	self.jet1Eta = array('f', [-1.])
	self.jet2Eta = array('f', [-1.])
	self.jet1Phi = array('f', [-1.])
	self.jet2Phi = array('f', [-1.])
	self.jet1Mass = array('f', [-1.])
	self.jet2Mass = array('f', [-1.])
	self.jet1MinMass = array('f', [-1.])
	self.jet2MinMass = array('f', [-1.])
	self.jet1BTag = array('f', [-1.])
	self.jet2BTag = array('f', [-1.])
	self.jet1TopTag = array('i', [-1])
	self.jet2TopTag = array('i', [-1])
	self.jet1tau32 = array('f', [-1.])
	self.jet2tau32 = array('f', [-1.])
	self.jet1Parton = array('i', [-999])
	self.jet2Parton = array('i', [-999])


	self.mistagVars.Branch('jet1Pt', self.jet1Pt, 'jet1Pt/F')
	self.mistagVars.Branch('jet2Pt', self.jet2Pt, 'jet2Pt/F')
	self.mistagVars.Branch('jet1Eta', self.jet1Eta, 'jet1Eta/F')
	self.mistagVars.Branch('jet2Eta', self.jet2Eta, 'jet2Eta/F')
	self.mistagVars.Branch('jet1Phi', self.jet1Phi, 'jet1Phi/F')
	self.mistagVars.Branch('jet2Phi', self.jet2Phi, 'jet2Phi/F')
	self.mistagVars.Branch('jet1Mass', self.jet1Mass, 'jet1Mass/F')
	self.mistagVars.Branch('jet2Mass', self.jet2Mass, 'jet2Mass/F')
	self.mistagVars.Branch('jet1MinMass', self.jet1MinMass, 'jet1MinMass/F')
	self.mistagVars.Branch('jet2MinMass', self.jet2MinMass, 'jet2MinMass/F')
	self.mistagVars.Branch('jet1BTag', self.jet1BTag, 'jet1BTag/F')
	self.mistagVars.Branch('jet2BTag', self.jet2BTag, 'jet2BTag/F')
	self.mistagVars.Branch('jet1TopTag', self.jet1TopTag, 'jet1TopTag/I')
	self.mistagVars.Branch('jet2TopTag', self.jet2TopTag, 'jet2TopTag/I')
	self.mistagVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
	self.mistagVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
	self.mistagVars.Branch('jet1Parton', self.jet1Parton, 'jet1Parton/I')
	self.mistagVars.Branch('jet2Parton', self.jet2Parton, 'jet2Parton/I')


        
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
	event.getByLabel (self.allTopTagPartonLabel, self.allTopTagPartonHandle )
	topPartonFlavors = self.allTopTagPartonHandle.product()




        event.getByLabel (self.allNsub2Label, self.allNsub2Handle)
        event.getByLabel (self.allNsub3Label, self.allNsub3Handle)
	tau2Vals = self.allNsub2Handle.product()
	tau3Vals = self.allNsub3Handle.product()	

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
        etaCuts = abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4
        deltaPhiCut = abs(deltaPhi)>2.1
        passType11KinCuts   = ptCuts and etaCuts and deltaPhiCut
    
    
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
   
	jet1tau32Val = tau3Vals[0] / tau2Vals[0]
	jet2tau32Val = tau3Vals[1] / tau2Vals[1]
 
    
        failMinMass0   = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] < 50  
        failMinMass1   = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] < 50
	failMinMassLow0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] < 30
	failMinMassLow1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] < 30
	fail2Nsub0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetNSubjets[0] < 2
	fail2Nsub1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetNSubjets[1] < 2
	fail3Nsub0 = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetNSubjets[0] < 3
	fail3Nsub1 = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetNSubjets[1] < 3
	bTag0 = jet0_maxB
	bTag1 = jet1_maxB
        topTag0        = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        topTag1        = topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
        topTag0WP1        = jet1tau32Val < 0.7 and bTag0 > 0.244 and topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        topTag1WP1        = jet2tau32Val < 0.7 and bTag1 > 0.244 and topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
        topTag0WP2        = jet1tau32Val < 0.6 and bTag0 > 0.244 and topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        topTag1WP2        = jet2tau32Val < 0.6 and bTag1 > 0.244 and topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
        topTag0WP3        = jet1tau32Val < 0.55 and bTag0 > 0.679 and topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        topTag1WP3        = jet2tau32Val < 0.55 and bTag1 > 0.679 and topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
        topTag0WP4        = jet1tau32Val < 0.5 and bTag0 > 0.679 and topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 65 and topJetNSubjets[0] > 2
        topTag1WP4        = jet2tau32Val < 0.5 and bTag1 > 0.679 and topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 65 and topJetNSubjets[1] > 2
        topTag0WP5        = jet1tau32Val < 0.4 and bTag0 > 0.679 and topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 55 and topJetNSubjets[0] > 2
        topTag1WP5        = jet2tau32Val < 0.4 and bTag1 > 0.679 and topJetMass[1] > 140 and topJetMass[1] < 250 and topJetMinMass[1] > 55 and topJetNSubjets[1] > 2
 
        if passType11KinCuts :
	    self.jet1Pt[0] = topJets[0].pt()
	    self.jet2Pt[0] = topJets[1].pt()
	    self.jet1Parton[0] = topPartonFlavors[0]
	    self.jet2Parton[0] = topPartonFlavors[1]
	    self.jet1Eta[0] = topJets[0].eta()
	    self.jet2Eta[0] = topJets[1].eta()
	    self.jet1Phi[0] = topJets[0].phi()
	    self.jet2Phi[0] = topJets[1].phi()
	    self.jet1Mass[0] = topJetMass[1]
	    self.jet2Mass[0] = topJetMass[0]
	    self.jet1MinMass[0] = topJetMinMass[0]
	    self.jet2MinMass[0] = topJetMinMass[1]
	    self.jet1BTag[0] = bTag0
	    self.jet2BTag[0] = bTag1
	    self.jet1TopTag[0] = topTag0
	    self.jet2TopTag[0] = topTag1
	    self.jet1tau32[0] = jet1tau32Val
	    self.jet2tau32[0] = jet2tau32Val
	    self.mistagVars.Fill()
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
                    if topTag1WP1:
			self.lowmMinTagWP1Pt.Fill( topJets[1].pt(), weight)
                    if topTag1WP2:
			self.lowmMinTagWP2Pt.Fill( topJets[1].pt(), weight)
                    if topTag1WP3:
			self.lowmMinTagWP3Pt.Fill( topJets[1].pt(), weight)
                    if topTag1WP4:
			self.lowmMinTagWP4Pt.Fill( topJets[1].pt(), weight)
                    if topTag1WP5:
			self.lowmMinTagWP5Pt.Fill( topJets[1].pt(), weight)
		    if topTag1:
			self.lowmMinTagPt.Fill( topJets[1].pt(), weight )
                    	if not bTag0:
				self.lowmMinBProbePt.Fill( topJets[1].pt(), weight )
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
                    if topTag0WP1:
			self.lowmMinTagWP1Pt.Fill( topJets[0].pt(), weight)
                    if topTag0WP2:
			self.lowmMinTagWP2Pt.Fill( topJets[0].pt(), weight)
                    if topTag0WP3:
			self.lowmMinTagWP3Pt.Fill( topJets[0].pt(), weight)
                    if topTag0WP4:
			self.lowmMinTagWP4Pt.Fill( topJets[0].pt(), weight)
                    if topTag0WP5:
			self.lowmMinTagWP5Pt.Fill( topJets[0].pt(), weight)
                    if topTag0:
			self.lowmMinTagPt.Fill( topJets[0].pt(), weight )
                    	if not bTag1:
				self.lowmMinBProbePt.Fill( topJets[0].pt(), weight )
		    		if bTag0 :
		     			self.lowmMinBTagPt.Fill( topJets[0].pt(), weight ) 
            	if failMinMassLow1 :
			if bTag1 and bTag0:
				self.lowmMin2BProbePt.Fill( topJets[0].pt(), weight )
			if topTag0 and bTag0 and bTag1 :
				 self.lowmMin2BTagPt.Fill( topJets[0].pt(), weight )
		if topTag0 and topTag1 and not bTag1:
			self.antiBProbePt.Fill( topJets[0].pt(), weight )
			if bTag0:
				self.antiBTagPt.Fill( topJets[0].pt(), weight)        
