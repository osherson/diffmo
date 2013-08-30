import ROOT
import copy

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *
from array import array
from math import *

class MistagMaker :
    """Makes mistag rates"""
    def __init__(self, outfile, useGenWeight, triggerWeight ):
        """Initialization"""
        self.outfileStr = outfile
        self.useGenWeight = useGenWeight
        self.triggerFile = ROOT.TFile("HT750_TYPE12.root")
        self.triggerHist = self.triggerFile.Get("TRIGGER_EFF").Clone()
        self.triggerWeight = triggerWeight
	print self.triggerWeight



	label = 'ttbsmAna'
        self.allTopTagHandle         = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagLabel          = ( label,   "topTagP4")
        self.allTopTagMinMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagMinMassLabel   = ( label,   "topTagMinMass" )
        self.allTopTagNSubsHandle    = Handle("std::vector<double>" )
        self.allTopTagNSubsLabel     = ( label,   "topTagNSubjets" )
        self.hemis0Handle            = Handle ("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >")
        self.hemis0Label             = ( label, "topTagP4Hemis0" )
        self.hemis0MinMassHandle     = Handle( "std::vector<double>" )
        self.hemis0MinMassLabel      = ( label, "topTagMinMassHemis0" )
        self.hemis0NSubjetsHandle    = Handle( "std::vector<double>" )
        self.hemis0NSubjetsLabel     = ( label, "topTagNSubjetsHemis0"  )
        self.hemis0TopMassHandle     = Handle( "std::vector<double>" )
        self.hemis0TopMassLabel      = ( label, "topTagTopMassHemis0" )
        self.hemis0PassHandle        = Handle( "std::vector<int>")
        self.hemis0PassLabel         = ( label, "topTagPassHemis0" )
        self.hemis1Handle            = Handle( "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >")
        self.hemis1Label             = ( label, "wTagP4Hemis1" )
        self.hemis1BdiscHandle       = Handle( "std::vector<double>" )
        self.hemis1BdiscLabel        = ( label,   "wTagBDiscHemis1" )
        self.hemis1MuHandle          = Handle( "std::vector<double>")
        self.hemis1MuLabel           = ( label,  "wTagMuHemis1" )
        self.hemis1Jet3Handle        = Handle("int")
        self.hemis1Jet3Label         = ( label, "jet3Hemis1" )
        self.weightsHandle           = Handle( "double" )
        self.weightsLabel            = ( label, "weight" )
	
	self.allTopTagCHEHandle = Handle("std::vector<double>" )
	self.allTopTagCHELabel = (label, "topTagCHE")
	self.allTopTagNEHandle = Handle("std::vector<double>")
	self.allTopTagNELabel = (label, "topTagNE")
	self.allTopTagNumPFHandle = Handle("std::vector<int>")
	self.allTopTagNumPFLabel = (label, "topTagNumPF")

	self.allTopTagNumCHHandle = Handle("std::vector<int>")
	self.allTopTagNumCHLabel = (label, "topTagNumCH")
	self.allTopTagNumNEHandle = Handle("std::vector<int>")
	self.allTopTagNumNELabel = (label, "topTagNumNE")
 
	self.triggerEventHandle = Handle( "std::vector<pat::TriggerObjectStandAlone>")
	self.triggerEventLabel = ("patTrigger", "")
        self.__book__()

    def __del__(self) :
        """(private) destructor"""
        self.f.cd()
        self.f.Write()
        self.f.Close()


    def __book__(self):
        """(private) creates histograms"""
        self.f = ROOT.TFile(self.outfileStr + '.root', "RECREATE")
        self.topJetCandPtSignal          = ROOT.TH1D("topJetCandPtSignal",          "Top Jet Cand Pt",          200,  0,  1000 )
        self.topJetCandPtSideBand        = ROOT.TH1D("topJetCandPtSideBand",        "Top Jet Cand Pt",          200,  0,  1000 )
        self.topJetCandPtAll             = ROOT.TH1D("topJetCandPtAll",             "Top Jet Cand Pt",          200,  0,  1000 )
        self.topJetCandMassSignal        = ROOT.TH1D("topJetCandMassSignal",        "Top Jet Cand Mass",        100,  0,  500 )
        self.topJetCandMassSideBand      = ROOT.TH1D("topJetCandMassSideBand",      "Top Jet Cand Mass",        100,  0,  500 )
        self.topJetCandMassAll           = ROOT.TH1D("topJetCandMassAll",            "Top Jet Cand Mass",        100,  0,  500 )
        self.topJetNsubsSignal           = ROOT.TH1D("topJetNsubsSignal",           "Top Jet Cand N Subjets",   10,   -0.5, 9.5 )
        self.topJetNsubsSideBand         = ROOT.TH1D("topJetNsubsSideBand",         "Top Jet Cand N Subjets",   10,   -0.5, 9.5 )
        self.topJetNsubsAll              = ROOT.TH1D("topJetNsubsAll",               "Top Jet Cand N Subjets",   10,   -0.5, 9.5 )
        self.topJetMinMassSignal         = ROOT.TH1D("topJetMinMassSignal",         "Top Jet Min Mass",         100,  0,  500 )
        self.topJetMinMassSideBand       = ROOT.TH1D("topJetMinMassSideBand",       "Top Jet Min Mass",         100,  0,  500 )
        self.topJetMinMassAll            = ROOT.TH1D("topJetMinMassAll",             "Top Jet Min Mass",         100,  0,  500 )
        self.type2TopProbe               = ROOT.TH1D("type2TopProbe",              "Top Probe Pt",               400,  0,  2000 )
        self.type2TopTag                 = ROOT.TH1D("type2TopTag",                "Top Tag Pt",                 400,  0,  2000 )
        self.type2TopTagMass             = ROOT.TH1D("type2TopTagMass",            "Top Tag Mass",               400,  0,  2000 )
        self.type2TopTagExp              = ROOT.TH1D("type2TopTagExp",             "Top Tag Pt Expected",        400,  0,  2000 )
        self.type2TopTagExp.Sumw2()
        self.type2TopTagExp800GeV        = ROOT.TH1D("type2TopTagExp800GeV",       "Top Tag Pt Expected",        400,  0,  2000 )
        self.type2TopTagExp800GeV.Sumw2()
        self.type2TopTag800GeV           = ROOT.TH1D("type2TopTag800GeV",          "Top Tag Pt",                 400,  0,  2000 )
        self.type2TopProbe800GeV         = ROOT.TH1D("type2TopProbe800GeV",        "Top Probe Pt",               400,  0,  2000 )
        self.type2KinTopProbe            = ROOT.TH1D("type2KinTopProbe",           "Top Probe Pt",               400,  0,  2000 )
        self.type2KinTopTag              = ROOT.TH1D("type2KinTopTag",             "Top Tag Pt",                 400,  0,  2000 )
        self.type2SideBandProbe          = ROOT.TH1D("type2SideBandProbe",         "Top Probe Pt",               400,  0,  2000 )
        self.type2SideBandTag            = ROOT.TH1D("type2SideBandTag",           "Top Tag Pt",                 400,  0,  2000 )
        self.randomTopProbe              = ROOT.TH1D("randomTopProbe",             "Top Probe Pt",               400,  0,  2000 )
        self.randomTopTag                = ROOT.TH1D("randomTopTag",               "Top Tag Pt",                 400,  0,  2000 )

        self.jet1PtProbe                 = ROOT.TH1D("jet1PtProbe",               "Top Tag Probe",              400,  0,  2000 )
        self.jet1PtTag                   = ROOT.TH1D("jet1PtTag",                 "Top Tag Pt",                 400,  0,  2000 )
        self.jet1PtProbe_mu                 = ROOT.TH1D("jet1PtProbe_mu",               "Top Tag Probe",              400,  0,  2000 )
        self.jet1PtTag_mu                   = ROOT.TH1D("jet1PtTag_mu",                 "Top Tag Pt",                 400,  0,  2000 )
	self.jet1EtaProbe_mu = ROOT.TH1D("jet1EtaProbe_mu", "Top Probe Eta", 100, -3, 3);
	self.jet1EtaTag_mu =  ROOT.TH1D("jet1EtaTag_mu", "Top Tag Eta", 100, -3, 3);



	self.jetVars = ROOT.TTree('jetVars', 'jetVars')
	self.topMass = array('f', [-1.])
	self.topPt = array('f', [-1.])
	self.Nsub = array('i', [-1])
	self.minMass = array('f', [-1.])
	self.WPt = array('f', [-1.])
	self.WMass = array('f', [-1.])
	self.WMu = array('f', [-1.])
	self.pairMass = array('f', [-1.])
	self.BPt = array('f', [-1.])
	self.tagged = array('i', [-1])



	self.jetVars.Branch('topMass', self.topMass, 'topMass/F')
	self.jetVars.Branch('topPt', self.topPt, 'topPt/F')
	self.jetVars.Branch('Nsub', self.Nsub, 'Nsub/I')
	self.jetVars.Branch('minMass', self.minMass, 'minMass/F')
	self.jetVars.Branch('WPt', self.WPt, 'WPt/F')
	self.jetVars.Branch('WMass', self.WMass, 'WMass/F')
	self.jetVars.Branch('WMu', self.WMu, 'WMu/F')
	self.jetVars.Branch('pairMass', self.pairMass, 'pairMass/F')
	self.jetVars.Branch('BPt', self.BPt, 'BPt/F')
	self.jetVars.Branch('tagged', self.tagged, 'tagged/I')

	
	self.pretagMass = ROOT.TH1D("pretagMass", "pretagMass", 400, 0, 2000);
	self.pretagPt = ROOT.TH1D("pretagPt", "pretagPt", 400, 0, 2000);
	self.pretagNsub = ROOT.TH1D("pretagNsub", "pretagNsub", 10, -0.5,9.5);
	self.pretagMinMass = ROOT.TH1D("pretagMinMass", "pretagMinMass", 100, 0, 500);
	self.pretagWPt = ROOT.TH1D("pretagWPt", "pretagWPt", 400, 0, 2000);
	self.pretagWMass = ROOT.TH1D("pretagWMass", "pretagWMass", 400, 0, 2000);
	self.pretagWMu = ROOT.TH1D("pretagWMu", "pretagWMu", 200, 0,2);
	self.pretagPairMass = ROOT.TH1D("pretagPairMass", "pretagPairMass", 100, 0, 500);
	self.pretagBPt = ROOT.TH1D("pretagBPt", "pretagBPt", 100, 0, 500);

        self.posttagMass = ROOT.TH1D("posttagMass", "posttagMass", 400, 0, 2000);
	self.posttagPt = ROOT.TH1D("posttagPt", "posttagPt", 400, 0, 2000);
	self.posttagNsub = ROOT.TH1D("posttagNsub", "posttagNsub", 10, -0.5,9.5);
	self.posttagMinMass = ROOT.TH1D("posttagMinMass", "posttagMinMass", 100, 0, 500);
	self.posttagWPt = ROOT.TH1D("posttagWPt", "posttagWPt", 400, 0, 2000);
	self.posttagWMass = ROOT.TH1D("posttagWMass", "posttagWMass", 400, 0, 2000);
	self.posttagWMu = ROOT.TH1D("posttagWMu", "posttagWMu", 200, 0,2);
	self.posttagPairMass = ROOT.TH1D("posttagPairMass", "posttagPairMass", 100, 0, 500);
	self.posttagBPt = ROOT.TH1D("posttagBPt", "posttagBPt", 100, 0, 500);



        self.type2KinTopProbe.Sumw2()
        self.type2KinTopTag.Sumw2()
        self.type2SideBandProbe.Sumw2()
        self.type2SideBandTag.Sumw2()
        self.randomTopProbe.Sumw2()
        self.randomTopTag.Sumw2()


    def analyze(self, event):
        """Runs the analyzer"""
        event.getByLabel (self.hemis0Label, self.hemis0Handle)
        topJets = self.hemis0Handle.product()

	
	#event.getByLabel (self.triggerEventLabel, self.triggerEventHandle)
	#triggerEvt = self.triggerEventHandle.product()
	#triggerObjMatches = []
	#for j in range(0, len(triggerEvt) ):
  #		if triggerEvt[j].hasPathName('HLT_PFJet320_v*', True, True):
  #			triggerObjMatches.append( triggerEvt[j].p4() )

			
	



        HTsum = 0.0
        nTopCand = 0
        for i in range(0,len(topJets) ) :
          if( topJets[i].pt() > 400 ) :
            nTopCand = nTopCand + 1
          HTsum += topJets[i].pt()

        if nTopCand < 1 :
            return
#	trigMatch = False
#	for k in range(0, len(triggerObjMatches)):
#		if sqrt( (triggerObjMatches[k].eta() - topJets[0].eta())*(triggerObjMatches[k].eta() - topJets[0].eta()) + (triggerObjMatches[k].phi() - topJets[0].phi())*(triggerObjMatches[k].phi() - topJets[0].phi())) < 0.4:
#			trigMatch = True
#			break
#	if trigMatch == False:
#	    return

        event.getByLabel (self.hemis1Label, self.hemis1Handle)
        wJets = self.hemis1Handle.product()


        pairMass = 0.0
        ttMass = 0.0

        if nTopCand < 1 or len(wJets) < 2 :
          return

        weight = 1.0
        if self.useGenWeight :
            event.getByLabel( self.weightsLabel, self.weightsHandle )
            weight = self.weightsHandle.product()[0]




        if self.triggerWeight != "noWeight" :
            jetTriggerWeight = 1.0
            if HTsum < 2000:
                bin0 = self.triggerHist.FindBin(HTsum)
                jetTriggerWeight = self.triggerHist.GetBinContent(bin0)
                weight = weight*jetTriggerWeight






        event.getByLabel (self.hemis0MinMassLabel, self.hemis0MinMassHandle)
        topJetMinMass = self.hemis0MinMassHandle.product()
        event.getByLabel (self.hemis0NSubjetsLabel, self.hemis0NSubjetsHandle)
        topJetNSubjets = self.hemis0NSubjetsHandle.product()
        event.getByLabel (self.hemis0TopMassLabel, self.hemis0TopMassHandle)
        topJetMass = self.hemis0TopMassHandle.product()
        event.getByLabel (self.hemis1BdiscLabel, self.hemis1BdiscHandle)
        wJetBDisc = self.hemis1BdiscHandle.product()
        event.getByLabel (self.hemis0PassLabel, self.hemis0PassHandle )
        topJetPass = self.hemis0PassHandle.product()
        event.getByLabel (self.hemis1MuLabel, self.hemis1MuHandle)
        wJetMu = self.hemis1MuHandle.product()
        event.getByLabel (self.hemis1Jet3Label, self.hemis1Jet3Handle )
        jet3 = (self.hemis1Jet3Handle.product())[0]
	event.getByLabel (self.allTopTagCHELabel, self.allTopTagCHEHandle)
	event.getByLabel (self.allTopTagNELabel, self.allTopTagNEHandle)
	event.getByLabel (self.allTopTagNumPFLabel, self.allTopTagNumPFHandle)
	event.getByLabel (self.allTopTagNumCHLabel, self.allTopTagNumCHHandle)
	event.getByLabel (self.allTopTagNumNELabel, self.allTopTagNumNEHandle)
	topJetsCHE = self.allTopTagCHEHandle.product()
	topJetsNE = self.allTopTagNEHandle.product()
	topJetsNumPF = self.allTopTagNumPFHandle.product()
	topJetsNumCH = self.allTopTagNumCHHandle.product()
	topJetsNumNE = self.allTopTagNumNEHandle.product()



        if jet3 < 1 :
          print "This is not expected, debug!"

        pairMass = (wJets[jet3]+wJets[0]).mass()
        ttMass = (wJets[jet3]+wJets[0]+topJets[0]).mass()
        jet1P4_massDown = copy.copy(topJets[0])
        jet1P4_massDown.SetM( max(jet1P4_massDown.mass()-100, 0.0) )
        #print topJets[0].px(), topJets[0].py(), topJets[0].pz(), topJets[0].mass() 
        #print jet1P4_massDown.px(), jet1P4_massDown.py(), jet1P4_massDown.pz(), jet1P4_massDown.mass()
        ttMassJet1MassDown = (wJets[jet3]+wJets[0]+jet1P4_massDown).mass()

        jet1P4_massUp = copy.copy(topJets[0])
        jet1P4_massUp.SetM( jet1P4_massUp.mass()+100 )
        ttMassJet1MassUp = (wJets[jet3]+wJets[0]+jet1P4_massUp).mass()
        #print jet1P4_massUp.px(), jet1P4_massUp.py(), jet1P4_massUp.pz(), jet1P4_massUp.mass()


        type2TopPtCut = (wJets[0] + wJets[jet3]).pt() > 400

	deltaY = (wJets[0]+wJets[jet3]).Rapidity() - topJets[0].Rapidity()

	NE0 = topJetsNE[0]
	if NE0 == 0:
		NE0 = 0.0001

	cleanup = not (abs(topJets[0].eta()) < 1.5 and abs(topJets[0].eta()) > 1.0 and (topJetsCHE[0] / NE0) > 2.0)

	passKinCuts = (nTopCand == 1) and (wJets[0].pt() > 200)  and (wJetMu[0] < 0.4) and (wJets[jet3].pt() > 30 ) and abs(deltaY) < 1.0 and cleanup

        passKinCuts2 = (nTopCand == 1) and (wJets[0].pt() > 200)  and (wJets[jet3].pt() > 30 ) and wJetMu[0]  > 0.4 and abs(deltaY) < 1.0 and cleanup

        hasBTag1    = wJetBDisc[jet3] > 3.3
        hasType2Top = wJets[0].mass() > 60 and wJets[0].mass() < 130 and pairMass > 140 and pairMass < 250
	hasTopTag   = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
        SBAndSR     = wJets[0].mass() > 40 and wJets[0].mass() < 150 and pairMass > 100 and pairMass < 300
        SBAndSR2    = wJets[0].mass() > 20 and wJets[0].mass() < 170 and pairMass > 60  and pairMass < 350
        passPairMass  = pairMass > 140 and pairMass < 250
        failWMass     = not (wJets[0].mass() > 60 and wJets[0].mass() < 130)
        passWiderTopMassCut   =   topJetMass[0] > 100 and topJetMass[0] < 300
        passTopMass           =   topJetMass[0] > 140 and topJetMass[0] < 250
        passNSubjetsCut       =   topJetNSubjets[0] > 2
        passMinMassCut        =   topJetMinMass[0] > 50
        topMassSideBand       =   (topJetMass[0] > 100 and topJetMass[0] < 140) or (topJetMass[0] > 250 and topJetMass[0] < 300)


	
        if passKinCuts  :

          self.topJetCandPtAll.Fill( topJets[0].pt(), weight )
          self.topJetCandMassAll.Fill( topJets[0].mass(), weight )
          self.topJetNsubsAll.Fill( topJetNSubjets[0], weight )
          self.topJetMinMassAll.Fill( topJetMinMass[0], weight )
          self.type2KinTopProbe.Fill( topJets[0].pt(), weight )
          if hasTopTag  :   self.type2KinTopTag.Fill(  topJets[0].pt(), weight )
          if not SBAndSR  :
            self.type2SideBandProbe.Fill( topJets[0].pt(), weight )
            if hasTopTag  :   self.type2SideBandTag.Fill( topJets[0].pt(), weight )

          if passPairMass and failWMass:
            self.jet1PtProbe.Fill(topJets[0].pt(), weight)
            if hasTopTag :
              self.jet1PtTag.Fill(topJets[0].pt(), weight)

          if hasType2Top  :
            self.topJetCandPtSignal.Fill( topJets[0].pt(), weight )
            self.topJetCandMassSignal.Fill( topJets[0].mass(), weight )
            self.topJetNsubsSignal.Fill( topJetNSubjets[0], weight )
            self.topJetMinMassSignal.Fill( topJetMinMass[0], weight )

            #Make Top mistag measurement
            self.type2TopProbe.Fill( topJets[0].pt(), weight )
            if hasTopTag :  
              self.type2TopTag.Fill( topJets[0].pt() , weight )
              self.type2TopTagMass.Fill( topJets[0].mass(), weight )


          if SBAndSR and (not hasType2Top) :
            self.topJetCandPtSideBand.Fill( topJets[0].pt(), weight )
            self.topJetCandMassSideBand.Fill( topJets[0].mass(), weight )
            self.topJetNsubsSideBand.Fill( topJetNSubjets[0], weight )
            self.topJetMinMassSideBand.Fill( topJetMinMass[0], weight )
          self.topMass[0] = topJets[0].mass()
	  self.topPt[0] = topJets[0].pt()
	  self.Nsub[0] = topJetNSubjets[0]
	  self.minMass[0] = topJetMinMass[0]
	  self.WPt[0] = wJets[0].pt()
	  self.WMass[0] = wJets[0].mass()
	  self.WMu[0] = wJetMu[0]
	  self.pairMass[0] = pairMass
	  self.BPt[0] = wJets[jet3].pt()
	  self.tagged[0] = 2
	  self.jetVars.Fill()

	
 
        if passKinCuts2 :
	  

          if hasType2Top:
		self.topMass[0] = topJets[0].mass()
		self.topPt[0] = topJets[0].pt()
		self.Nsub[0] = topJetNSubjets[0]
		self.minMass[0] = topJetMinMass[0]
		self.WPt[0] = wJets[0].pt()
		self.WMass[0] = wJets[0].mass()
		self.WMu[0] = wJetMu[0]
		self.pairMass[0] = pairMass
		self.BPt[0] = wJets[jet3].pt()
		self.tagged[0] = 0

		
	   	self.jet1PtProbe_mu.Fill( topJets[0].pt(), weight )
		self.jet1EtaProbe_mu.Fill( topJets[0].eta(), weight )
                if hasTopTag:   
		  self.tagged[0] = 1
		  self.jet1PtTag_mu.Fill( topJets[0].pt(), weight )
		  self.jet1EtaTag_mu.Fill( topJets[0].eta(), weight)

		self.jetVars.Fill()

