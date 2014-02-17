import ROOT
import copy
from math import *
from array import array
from operator import itemgetter, attrgetter

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *


class Type11Analyzer :
    """Run 1 + 1 Analysis"""
    def __init__(self, useMC, outfile, mistagFile, collectionLabelSuffix, useGenWeight, triggerFile, modMassFile, pdfWeight="nominal", triggerWeight = "noWeight"):
        self.outfile = outfile
        self.mistagFileStr = mistagFile
        self.triggerFileStr = triggerFile
        self.modMassFileStr = modMassFile
        self.useMC = useMC
        self.useGenWeight = useGenWeight
        self.pdfWeight = pdfWeight
        self.triggerWeight = "noWeight"#triggerWeight
		
        label = 'ttbsmAna'+collectionLabelSuffix

	print label

        self.allTopTagHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagLabel  = ( label,   "topTagP4")
        self.allca8Handle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allca8Label  = ( label,   "ca8JetP4")
        self.allTopTagSubjetsHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagSubjetsLabel  = ( label,   "topTagSubjet1P4")

        self.allTopTagTopMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagTopMassLabel   = ( label,   "topTagTopMass" )
        self.allTopTagMinMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagMinMassLabel   = ( label,   "topTagMinMass" )
        self.allTopTagNSubjetsHandle    = Handle("std::vector<double>" )
        self.allTopTagNSubjetsLabel     = ( label,   "topTagNSubjets" )

	self.allNsub1Handle = Handle( "std::vector<double>")
	self.allNsub2Handle = Handle( "std::vector<double>")
	self.allNsub3Handle = Handle( "std::vector<double>")
	self.allNsub4Handle = Handle( "std::vector<double>")
	self.allNsub1Label = (label, "tau1")	
	self.allNsub2Label = (label, "tau2")	
	self.allNsub3Label = (label, "tau3")	
	self.allNsub4Label = (label, "tau4")	


	self.allTopTagSubjetBDiscHandle = Handle("std::vector<double>")
	self.allTopTagSubjetBDiscLabel = (label, "topTagSubjetBDisc")

	self.allTopTagSubjetJetIDHandle = Handle("std::vector<int>")
	self.allTopTagSubjetJetIDLabel = (label, "topTagSubjetJetID")

	self.allTopTagBDiscHandle = Handle("std::vector<double>")
	self.allTopTagBDiscLabel = (label, "topTagBDisc")


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
        
	self.allTopTagPassHandle  = Handle( "std::vector<int>" )
        self.allTopTagPassLabel   = ( label,   "topTagPass" )

        self.h_met = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.metLabel = ( label, "pfMET" )
        self.weightsHandle = Handle( "double" )
        self.weightsLabel = ( label, "weight" )
        self.pdfHandle = Handle("std::vector<double>")
        self.pdfLabel = ( label, "pdfWeights")
       
	self.textfile = open('txtfile11.txt', 'w')
	print self.textfile

	#self.badevents = open('AllBadHCALLaser.txt', 'r')
	#print self.badevents

	#self.badlist = []
	#self.badlist = self.badevents.readlines()


 
        self.__book__()



    def __del__ (self):
        """(Internal) Destructor"""
        print 'Goodbye from Type11Analyzer. Before I go, I will delete some stuff'
        self.f.cd()
        #self.mttPredDist.SetCalculatedErrors()
	self.mttPredDist.GetPredictedHist().Write()
        self.mttPredDist.GetObservedHist().Write()
        self.mttPredDist.GetTaggableHist().Write()
       
	self.ptPredDist.SetCalculatedErrors() 
        self.ptPredDist.GetPredictedHist().Write()
        
        #self.mttPredDistMassCut.SetCalculatedErrors()
	self.mttPredDistMassCut.GetPredictedHist().Write()
        self.mttPredDistMassCut.GetObservedHist().Write()
        self.mttPredDistMassCut.GetTaggableHist().Write()

        #self.mttPredDistModMassFlat.SetCalculatedErrors()
	self.mttPredDistModMassFlat.GetPredictedHist().Write()
        self.mttPredDistModMassFlat.GetObservedHist().Write()
        self.mttPredDistModMassFlat.GetTaggableHist().Write()

        #self.mttPredDistMod3MassFlat.SetCalculatedErrors()
	self.mttPredDistMod3MassFlat.GetPredictedHist().Write()
        self.mttPredDistMod3MassFlat.GetObservedHist().Write()
        self.mttPredDistMod3MassFlat.GetTaggableHist().Write()

	#self.mttPredDistMod3MassFlatSubtract.SetCalculatedErrors()
        self.mttPredDistMod3MassFlatSubtract.GetPredictedHist().Write()
        self.mttPredDistMod3MassFlatSubtract.GetObservedHist().Write()
        self.mttPredDistMod3MassFlatSubtract.GetTaggableHist().Write()

	self.mttPredDistModMassQCDdistSubtract.SetCalculatedErrors()
        self.mttPredDistModMassQCDdistSubtract.GetPredictedHist().Write()
        self.mttPredDistModMassQCDdistSubtract.GetObservedHist().Write()
        self.mttPredDistModMassQCDdistSubtract.GetTaggableHist().Write()

        
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
        self.modMassFile.Close()
        print '5'
	self.textfile.close()
        print 'So long!'

    def __book__( self ) :
        """(Internal) Books histograms"""

        print 'Booking histograms'
        self.mistagFile = ROOT.TFile("ALLjets_mistag.root")
        self.mistagFile.cd()
        self.mistag = self.mistagFile.Get("MISTAG_RATE_SUB_TTBAR_Inclusive").Clone()
        self.mistag.SetName('mistag')
        self.mistagMassCut = self.mistagFile.Get("MISTAG_RATE_SUB_TTBAR_Inclusive").Clone()
        self.mistagMassCut.SetName('mistagMassCut')
        if not self.useGenWeight:
            #self.mistagMassCutSubtract = self.mistagFile.Get("TYPE11_MISTAG_MASSCUT_SUBTRACT_TTBAR_LARGEBINS").Clone()
            self.mistagMassCutSubtract = self.mistagFile.Get("MISTAG_RATE_SUB_TTBAR_Inclusive").Clone()
            self.mistagMassCutSubtract.SetName('mistagMassCutSubtract')
        if self.useGenWeight:
            self.mistagMassCutSubtract = self.mistagFile.Get("MISTAG_RATE_SUB_TTBAR_Inclusive").Clone()
            self.mistagMassCutSubtract.SetName('mistagMassCutSubtract')
        print self.mistag.GetBinContent(3)
        ROOT.SetOwnership( self.mistag, False )
        ROOT.SetOwnership( self.mistagMassCut, False )
       
        self.triggerFile = ROOT.TFile(self.triggerFileStr + ".root")
        self.triggerFile.cd()
        self.triggerFile.ls()
        self.triggerHist = self.triggerFile.Get("TRIGGER_EFF").Clone()
        self.triggerHist.SetName('triggerHist')
        ROOT.SetOwnership( self.triggerHist, False )

        self.modMassFile = ROOT.TFile(self.modMassFileStr + ".root")
        print self.modMassFileStr
	self.modMassFile.cd()
        self.modMassFile.ls()
        self.modMassHist = ROOT.TH1F()
        self.modMassHist = self.modMassFile.Get("jetMassOneTag_MassWindow").Clone()
        self.modMassHist.SetName('modMassHist')
        ROOT.SetOwnership( self.modMassHist, False )
        print "integral "+str(self.modMassHist.Integral())
       # print "integral "+str(self.jetMassOneTag_MassWindow.Integral())
       
        self.f = ROOT.TFile( self.outfile + ".root", "recreate" )
        self.f.cd()

        self.mttPredDist                 = ROOT.PredictedDistribution( self.mistagMassCutSubtract, "mttPredDist", "mTT Mass",       1000, 0,  5000 )
        self.mttPredDistMassCut          = ROOT.PredictedDistribution( self.mistagMassCut, "mttPredDistMassCut", "mTT Mass",       1000, 0,  5000 )
        self.mttPredDistModMassFlat      = ROOT.PredictedDistribution( self.mistagMassCut, "mttPredDistModMassFlat", "mTT Mass",       1000, 0,  5000 )
        self.mttPredDistMod3MassFlat     = ROOT.PredictedDistribution( self.mistagMassCut, "mttPredDistMod3MassFlat", "mTT Mass",       1000, 0,  5000 )
        self.mttPredDistMod3MassFlatSubtract     = ROOT.PredictedDistribution( self.mistagMassCutSubtract, "mttPredDistMod3MassFlatSubtract", "mTT Mass",       1000, 0,  5000 )
        self.mttPredDistModMassQCDdistSubtract     = ROOT.PredictedDistribution( self.mistagMassCutSubtract, "mttPredDistModMassQCDdistSubtract", "mTT Mass",       1000, 0,  5000 )
	self.ptPredDist = ROOT.PredictedDistribution( self.mistagMassCutSubtract, "ptPredDist", "pt", 1000, 0, 2000)


	print 'here'


        ROOT.SetOwnership( self.mttPredDist, False )
        ROOT.SetOwnership( self.mttPredDistMassCut, False )
        ROOT.SetOwnership( self.mttPredDistModMassFlat, False )
        ROOT.SetOwnership( self.mttPredDistMod3MassFlat, False )
        ROOT.SetOwnership( self.mttPredDistMod3MassFlatSubtract, False )
        ROOT.SetOwnership( self.mttPredDistModMassQCDdistSubtract, False )
        ROOT.SetOwnership( self.ptPredDist, False )

        self.jetEta               = ROOT.TH1D("jetEta",               "jetEta",            100, -4,    4)
        self.jetMass              = ROOT.TH1D("jetMass",              "jetMass",        100,  0,  500 )
        self.jetMassOneTag        = ROOT.TH1D("jetMassOneTag",        "jetMassOneTag",        100,  0,  500 )
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
        self.mtt_gen                     = ROOT.TH1F("mtt_gen",                     "mtt gen",                  1000, 0,  5000 )
        self.mtt_gen_vs_reco      = ROOT.TH2D("mtt_gen_vs_reco",      "mtt gen vs reco",    1000, 0,  5000, 1000, 0,  5000)
       
	self.jetMassMW		  = ROOT.TH1D("jetMassMS",     "jetMass",   100, 0, 500)
	self.mttPredDistErr	  = ROOT.TH1D("mttPredDist_err",     "mttPredDist_Err",   1000, 0, 5000)
	self.ptPredDistErr	  = ROOT.TH1D("ptPredDist_err",     "ptPredDist_Err",   1000, 0, 2000)

	self.treeVars = ROOT.TTree('treeVars', 'treeVars')
	self.mttMassT = array('f', [-1.])
	self.mttMassPred = array('f', [-1.])
	self.mistagWt = array('f', [-1.])
	self.npv = array('i', [-1])
	self.index = array('i', [-1])
	self.trigWt = array('f', [-1.])
	self.jet1PtT = array('f', [-1.])
	self.jet2PtT = array('f', [-1.])
	self.sumJetPt = array('f', [-1.])
	self.jet1MassT = array('f', [-1.])
	self.jet2MassT = array('f', [-1.])
	self.jet1Eta = array('f', [-1.])
	self.jet2Eta = array('f', [-1.])
	self.jet1Phi = array('f', [-1.])
	self.jet2Phi = array('f', [-1.])
	self.MET = array('f', [-1.])
	self.run = array('i', [-1])
	self.event = array('l', [-1])
	self.lumi = array('i', [-1])
	self.jet1CHE = array('f', [-1.])
	self.jet1NE = array('f', [-1.])
	self.jet1NumPF = array('i', [-1])
	self.jet2CHE = array('f', [-1.])
	self.jet2NE = array('f', [-1.])
	self.jet2NumPF = array('i', [-1])
	self.jet1NumCH = array('i', [-1])
	self.jet2NumCH = array('i', [-1])
	self.jet1NumNE = array('i', [-1])
	self.jet2NumNE = array('i', [-1])
	self.jet1SubjetMaxBDisc = array('f', [-1.])
	self.jet1BDisc = array('f', [-1.])
	self.jet1MassGroomed = array('f', [-1.])
	self.jet1tau32 = array('f', [-1.])
	self.jet2SubjetMaxBDisc = array('f', [-1.])
	self.jet2BDisc = array('f', [-1.])
	self.jet2MassGroomed = array('f', [-1.])
	self.jet2tau32 = array('f', [-1.])
	self.jet1Nsubj = array('i', [-1])
	self.jet2Nsubj = array('i', [-1])
	self.jet1minMass = array('f', [-1.])
	self.jet2minMass = array('f', [-1.])

	
	self.treeVars.Branch('jet1SubjetMaxBDisc', self.jet1SubjetMaxBDisc, 'jet1SubjetMaxBDisc/F')
	self.treeVars.Branch('jet1BDisc', self.jet1BDisc, 'jet1BDisc/F')
	self.treeVars.Branch('jet1MassGroomed', self.jet1MassGroomed, 'jet1MassGroomed/F')
	self.treeVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
	self.treeVars.Branch('jet2SubjetMaxBDisc', self.jet2SubjetMaxBDisc, 'jet2SubjetMaxBDisc/F')
	self.treeVars.Branch('jet2BDisc', self.jet2BDisc, 'jet2BDisc/F')
	self.treeVars.Branch('jet2MassGroomed', self.jet2MassGroomed, 'jet2MassGroomed/F')
	self.treeVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
	self.treeVars.Branch('mttMass', self.mttMassT, 'mttMass/F')
	self.treeVars.Branch('mttMassPred', self.mttMassPred, 'mttMassPred/F')
	self.treeVars.Branch('mistagWt', self.mistagWt, 'mistagWt/F')
	self.treeVars.Branch('npv', self.npv, 'npv/I')
	self.treeVars.Branch('index', self.index, 'index/I')
	self.treeVars.Branch('trigWt', self.trigWt, 'trigWt/F')
	self.treeVars.Branch('jet1Pt', self.jet1PtT, 'jet1Pt/F')
	self.treeVars.Branch('jet2Pt', self.jet2PtT, 'jet2Pt/F')
	self.treeVars.Branch('sumJetPt', self.sumJetPt, 'sumJetPt/F')
	self.treeVars.Branch('jet1Mass', self.jet1MassT, 'jet1Mass/F')
	self.treeVars.Branch('jet2Mass', self.jet2MassT, 'jet2Mass/F')
	self.treeVars.Branch('jet1Eta', self.jet1Eta, 'jet1Eta/F')
	self.treeVars.Branch('jet2Eta', self.jet2Eta, 'jet2Eta/F')
	self.treeVars.Branch('jet1Phi', self.jet1Phi, 'jet1Phi/F')
	self.treeVars.Branch('jet2Phi', self.jet2Phi, 'jet2Phi/F')
	self.treeVars.Branch('MET', self.MET, 'MET/F')
	self.treeVars.Branch('run', self.run, 'run/I')
	self.treeVars.Branch('event', self.event, 'event/L')
	self.treeVars.Branch('lumi', self.lumi, 'lumi/I')
	self.treeVars.Branch('jet1CHE', self.jet1CHE, 'jet1CHE/F')
	self.treeVars.Branch('jet1NE', self.jet1NE, 'jet1NE/F')
	self.treeVars.Branch('jet1NumPF', self.jet1NumPF, 'jet1NumPF/I')
	self.treeVars.Branch('jet2CHE', self.jet2CHE, 'jet2CHE/F')
	self.treeVars.Branch('jet2NE', self.jet2NE, 'jet2NE/F')
	self.treeVars.Branch('jet2NumPF', self.jet2NumPF, 'jet2NumPF/I')
	self.treeVars.Branch('jet1NumCH', self.jet1NumCH, 'jet1NumCH/I')	
	self.treeVars.Branch('jet2NumCH', self.jet2NumCH, 'jet2NumCH/I')	
	self.treeVars.Branch('jet1NumNE', self.jet1NumNE, 'jet1NumNE/I')	
	self.treeVars.Branch('jet2NumNE', self.jet2NumNE, 'jet2NumNE/I')	
	self.treeVars.Branch('jet1Nsubj', self.jet1Nsubj, 'jet1Nsubj/I')
	self.treeVars.Branch('jet2Nsubj', self.jet2Nsubj, 'jet2Nsubj/I')
	self.treeVars.Branch('jet1minMass', self.jet1minMass, 'jet1minMass/F')
	self.treeVars.Branch('jet2minMass', self.jet2minMass, 'jet2minMass/F')



        self.mttMass.Sumw2()
        self.runPairs = []
        self.mttMassTriggerWeighted.Sumw2()

    def analyze(self, event) :
        """Analyzes event"""
       
	#badlistSet = set(self.badlist)

	self.run[0] = event.object().id().run()
        self.event[0] = event.object().id().event()
        self.lumi[0] = event.object().id().luminosityBlock() 
	
	#thisEvent = '{0}:{1}:{2}'.format(self.run[0], self.lumi[0], self.event[0])

	#if thisEvent in badlistSet:
	#	print "Found Bad HCAL Laser Event", thisEvent
	#	return

        event.getByLabel (self.allTopTagLabel, self.allTopTagHandle)
        topJets = self.allTopTagHandle.product()
        
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


	event.getByLabel (self.allTopTagSubjetsLabel, self.allTopTagSubjetsHandle)
	event.getByLabel (self.allca8Label, self.allca8Handle)
	event.getByLabel (self.allNsub1Label, self.allNsub1Handle)
	event.getByLabel (self.allNsub2Label, self.allNsub2Handle)
	event.getByLabel (self.allNsub3Label, self.allNsub3Handle)
	event.getByLabel (self.allNsub4Label, self.allNsub4Handle)
		

	event.getByLabel (self.allTopTagBDiscLabel, self.allTopTagBDiscHandle)
	event.getByLabel (self.allTopTagSubjetBDiscLabel, self.allTopTagSubjetBDiscHandle)
	event.getByLabel (self.allTopTagSubjetJetIDLabel, self.allTopTagSubjetJetIDHandle)


	topSubjets = self.allTopTagSubjetsHandle.product()
	ca8Jets = self.allca8Handle.product()
	tau1Vals = self.allNsub1Handle.product()
	tau2Vals = self.allNsub2Handle.product()
	tau3Vals = self.allNsub3Handle.product()
	tau4Vals = self.allNsub4Handle.product()

	topBDisc = self.allTopTagBDiscHandle.product()
	topSubjetsBDisc = self.allTopTagSubjetBDiscHandle.product()
	topJetID = self.allTopTagSubjetJetIDHandle.product()


 
        event.getByLabel( self.metLabel, self.h_met )
        mets = self.h_met.product()
	self.MET[0] = mets[0].pt()       
 
	self.cutflow.Fill(0.5,1)    

        upperWindowEdge = 250

        HTsum = 0.0
	nTopCand = 0
	for i in range(0,len(topJets) ) :
	  if( topJets[i].pt() > 400 ) :
            nTopCand = nTopCand + 1
	  HTsum += topJets[i].pt()    
        if nTopCand < 2 :
            return

        
        #pairMass = 0.0
        #ttMass = 0.0

        weight = 1.0
        if self.useGenWeight :
            event.getByLabel( self.weightsLabel, self.weightsHandle )
            weight = self.weightsHandle.product()[0]
	
        if self.triggerWeight != "noWeight" :
            jetTriggerWeight = 1.0
            jetTriggerWeight1 = 1.0
            jetTriggerWeight2 = 0.0
            if HTsum < 2000:
                bin0 = self.triggerHist.FindBin(HTsum)
                #bin0 = self.triggerHist.FindBin(topJets[0].pt()) 
                jetTriggerWeight1 = self.triggerHist.GetBinContent(bin0)
	    if HTsum < 2000:
		#bin1 = self.triggerHist.FindBin(topJets[1].pt())
                bin1 = self.triggerHist.FindBin(HTsum)
	        #jetTriggerWeight2 = self.triggerHist.GetBinContent(bin1)	
	    deltaTriggerEff1  = 0.5*(1.0-jetTriggerWeight1)
	    deltaTriggerEff  = 0.5*(1.0-jetTriggerWeight)
	    deltaTriggerEff2  = 0.5*(1.0-jetTriggerWeight2)
            jetTriggerWeightUp  =   jetTriggerWeight + deltaTriggerEff
            jetTriggerWeightDown  = jetTriggerWeight - deltaTriggerEff
            jetTriggerWeightUp  = min(1.0,jetTriggerWeightUp)
            jetTriggerWeightDown  = max(0.0,jetTriggerWeightDown)
	
	    jetTriggerWeightUp1  =   jetTriggerWeight1 + deltaTriggerEff1
            jetTriggerWeightDown1  = jetTriggerWeight1 - deltaTriggerEff1
            jetTriggerWeightUp1  = min(1.0,jetTriggerWeightUp1)
            jetTriggerWeightDown1  = max(0.0,jetTriggerWeightDown1)
            
	    jetTriggerWeightUp2  =   jetTriggerWeight2 + deltaTriggerEff2
            jetTriggerWeightDown2  = jetTriggerWeight2 - deltaTriggerEff2
            jetTriggerWeightUp2  = min(1.0,jetTriggerWeightUp2)
            jetTriggerWeightDown2  = max(0.0,jetTriggerWeightDown2)
            

            if self.triggerWeight == "Nominal" :
                weight = weight*(jetTriggerWeight1)
            if self.triggerWeight == "Up" :
                weight = weight*jetTriggerWeightUp1
            if self.triggerWeight == "Down" :
                weight = weight*jetTriggerWeightDown1




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
        deltaPhi = topJets[0].phi() - topJets[1].phi()
        if deltaPhi > ROOT.TMath.Pi():
            deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
        if deltaPhi < -ROOT.TMath.Pi():
            deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()

	CHE0 = topJetsCHE[0];
	CHE1 = topJetsCHE[1];
	
	NE0 = topJetsNE[0];
	NE1 = topJetsNE[1];

	if NE0 == 0:
	   NE0 = 0.0001
	if NE1 == 0:
	   NE1 = 0.0001

	#find matching ca8jet to top jet for Nsubjetiness
	deltaRmin1 = 999.
	deltaRmin2 = 999.
#	ca8jet0 = ca8Jets[0]
#	ca8jet1 = ca8Jets[1]
#	for ca8jet in ca8Jets:
#		if (ca8jet.DeltaR(topJets[0]) < deltaRmin1):
#			ca8jet0 = ca8jet
#			deltaRmin1 = ca8jet.DeltaR(topJets[0])
#		if (ca8jet.DeltaR(topJets[1]) < deltaRmin2):
#			ca8jet1 = ca8jet
#			deltaRmin2 = ca8jet.DeltaR(topJets[1])
#
#	print ca8jet0.pt(), ca8jet1.pt(), topJets[0].pt(), topJets[1].pt()	


	tagJet0Subjets = []
	tagJet0BDiscs = []
	tagJet1Subjets = []
	tagJet1BDiscs = []
	
	i = 0
	while i < (topJetNSubjets[0] + topJetNSubjets[1]):
		if i < topJetNSubjets[0]:
			tagJet0Subjets.append(topSubjets[i])
			tagJet0BDiscs.append(topSubjetsBDisc[i])
		elif i < (topJetNSubjets[0] + topJetNSubjets[1]):
			tagJet1Subjets.append(topSubjets[i])
			tagJet1BDiscs.append(topSubjetsBDisc[i])
		i += 1	
	tau32_jet1 = tau3Vals[0] / tau2Vals[0]
	tau32_jet2 = tau3Vals[1] / tau2Vals[1]

		
	bdisc_jet1 = topBDisc[0]
	bdisc_jet2 = topBDisc[1]

	maxSubjBDisc0 = max(tagJet0BDiscs)
	maxSubjBDisc1 = max(tagJet1BDiscs)


	groomed_jet1 = ROOT.TLorentzVector(0.0,0.0,0.0,0.0)
	groomed_jet2 = ROOT.TLorentzVector(0.0,0.0,0.0,0.0)
	for subjet in tagJet0Subjets:
		groomed_jet1 += ROOT.TLorentzVector(subjet.px(), subjet.py(), subjet.pz(), subjet.energy())
	for subjet in tagJet1Subjets:
		groomed_jet2 += ROOT.TLorentzVector(subjet.px(), subjet.py(), subjet.pz(), subjet.energy())

        #ptCuts = topJets[0].pt() > 450 and topJets[1].pt() > 450
        ptCuts = topJets[0].pt() > 400 and topJets[1].pt() > 400
        etaCuts = abs(topJets[0].Rapidity() - topJets[1].Rapidity()) < 1.0 and abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4 and not (abs(topJets[0].eta()) < 1.5 and abs(topJets[0].eta()) > 1.0 and topJetsCHE[0] / NE0 > 2.0) and not (abs(topJets[1].eta()) < 1.5 and abs(topJets[1].eta()) > 1.0 and topJetsCHE[1] / NE1 > 2.0)
        #etaCuts = abs(topJets[0].Rapidity() - topJets[1].Rapidity()) < 1.0 and abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4 and not (abs(topJets[0].eta()) < 1.5 and abs(topJets[0].eta()) > 1.0 and topJetsCHE[0] / NE0 > 2.0) and not (abs(topJets[1].eta()) < 1.5 and abs(topJets[1].eta()) > 1.0 and topJetsCHE[1] / NE1 > 2.0)
        #etaCuts = abs(topJets[0].Rapidity() - topJets[1].Rapidity()) < 1.0 and abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4
        #etaCuts = abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4
	deltaPhiCut = abs(deltaPhi)>2.1
        passType11KinCuts   = ptCuts #and etaCuts and deltaPhiCut

        if ptCuts:
            self.cutflow.Fill(1.5,1)  
            if etaCuts:
                self.cutflow.Fill(2.5,1)  
                if deltaPhiCut:
                    self.cutflow.Fill(3.5,1) 
                    topMassCuts = topJetMass[0] > 140 and topJetMass[0] < 250 and topJetMass[1] > 140 and topJetMass[1] < 250   
                    if topMassCuts:
                        self.cutflow.Fill(4.5,1)
                        nSubjetsCuts = topJetNSubjets[0] > 2 and topJetNSubjets[1] > 2
                        if nSubjetsCuts:
                            self.cutflow.Fill(5.5,1)
                            minMassCuts = topJetMinMass[0] > 50 and topJetMinMass[1] > 50
                            if minMassCuts:
                                self.cutflow.Fill(6.5,1)
 

        if passType11KinCuts :

            topTag0        =  topJetMass[0] > 140 and topJetMass[0] < upperWindowEdge and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
            topTag1        =  topJetMass[1] > 140 and topJetMass[1] < upperWindowEdge and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2
            failTopTag0        = topJetMass[0] > 140 and topJetMass[0] < upperWindowEdge and topJetMinMass[0] < 30 
            failTopTag1        = topJetMass[1] > 140 and topJetMass[1] < upperWindowEdge and topJetMinMass[1] < 30 
            passType11     = topTag0 and topTag1
            ttMass   = (topJets[0]+topJets[1]).mass()
           

	    MWweight1 = self.mistag.GetBinContent(self.mistag.FindBin(topJets[0].pt() ) )
	    MWweight2 = self.mistag.GetBinContent(self.mistag.FindBin(topJets[1].pt() ) )

 
            myrand = ROOT.gRandom.Uniform(140,250)
            jet0P4_massFlat = copy.copy(topJets[0])
            jet0P4_massFlat.SetM( myrand )
            ttMassJet0MassFlat = (topJets[1]+jet0P4_massFlat).mass()
            
            myrand2 = ROOT.gRandom.Uniform(140,250)
            jet1P4_massFlat = copy.copy(topJets[1])
            jet1P4_massFlat.SetM( myrand2 )
            ttMassJet1MassFlat = (topJets[0]+jet1P4_massFlat).mass()
           
            myrand_QCDdist_jet0 = self.modMassHist.GetRandom()
            jet0P4_modmassQCDdist = copy.copy(topJets[0])
            jet0P4_modmassQCDdist.SetM( myrand_QCDdist_jet0 )
            ttMassJet0modmassQCDdist = (topJets[1]+jet0P4_modmassQCDdist).mass()
            

            myrand_QCDdist_jet1 = self.modMassHist.GetRandom()
            jet1P4_modmassQCDdist = copy.copy(topJets[1])
            jet1P4_modmassQCDdist.SetM( myrand_QCDdist_jet1 )
            ttMassJet1modmassQCDdist = (topJets[0]+jet1P4_modmassQCDdist).mass()
		    
            #ibin0 = self.triggerHist.FindBin(topJets[0].pt())
		    #jetTriggerWeight = self.triggerHist.GetBinContent(bin0)

            ttMassMod3Jet0Flat = ttMass
            ttMassMod3Jet1Flat = ttMass
            if (topJets[0].mass() < 140 or topJets[0].mass() > 250) and (topJets[1].mass() < 140 or topJets[1].mass() > 250):
                ttMassMod3Jet0Flat = ttMassJet0MassFlat
                self.jetMassMW.Fill( myrand, MWweight1*MWweight2)
            if (topJets[1].mass() < 140 or topJets[1].mass() > 250) and (topJets[0].mass() < 140 or topJets[0].mass() > 250):
                ttMassMod3Jet1Flat = ttMassJet1MassFlat
                self.jetMassMW.Fill( myrand2, MWweight2*MWweight1)
           
            ttMassModJet0QCDdist = ttMass
            ttMassModJet1QCDdist = ttMass
            #if topJets[0].mass() < 140 or topJets[0].mass() > 250:
            ttMassModJet0QCDdist = ttMassJet0modmassQCDdist
            #if topJets[1].mass() < 140 or topJets[1].mass() > 250:
            ttMassModJet1QCDdist = ttMassJet1modmassQCDdist




	    self.jetMass.Fill( topJets[0].mass(), weight )
	    self.jetMass.Fill( topJets[1].mass(), weight )
            self.jetPt.Fill( topJets[0].pt(), weight )
            self.jetPt.Fill( topJets[1].pt(), weight )
            self.jetEta.Fill( topJets[0].eta(), weight )
            self.jetEta.Fill( topJets[1].eta(), weight )
            self.jetMinMass.Fill( topJetMinMass[0], weight )
            self.jetMinMass.Fill( topJetMinMass[1], weight )
            self.mttCandMass.Fill( ttMass, weight )
           


	    


 

            if passType11 :
                self.topTagMass.Fill( topJets[0].mass(), weight )
                self.topTagMass.Fill( topJets[1].mass(), weight )
                self.topTagPt.Fill( topJets[0].pt(), weight )
                self.topTagPt.Fill( topJets[1].pt(), weight )
                self.topTagMinMass.Fill( topJetMinMass[0], weight )
                self.topTagMinMass.Fill( topJetMinMass[1], weight )
                self.mttMassT[0] = ttMass
	    	self.jet1SubjetMaxBDisc[0] = maxSubjBDisc0
		self.jet1BDisc[0] = topBDisc[0] 
		self.jet1MassGroomed[0] = groomed_jet1.M()
		self.jet1tau32[0] = tau32_jet1
	    	self.jet2SubjetMaxBDisc[0] = maxSubjBDisc1
		self.jet2BDisc[0] = topBDisc[1] 
		self.jet2MassGroomed[0] = groomed_jet2.M()
		self.jet2tau32[0] = tau32_jet2
		self.jet1minMass[0] = topJetMinMass[0]
		self.jet2minMass[0] = topJetMinMass[1]
		self.jet1Nsubj[0] = topJetNSubjets[0]
		self.jet2Nsubj[0] = topJetNSubjets[1]
		self.index[0] = 1
	    	self.jet1PtT[0] = topJets[0].pt() 
            	self.jet2PtT[0] = topJets[1].pt()
            	self.jet1Eta[0] = topJets[0].Rapidity()
	    	self.jet2Eta[0] = topJets[1].Rapidity()
            	self.jet1Phi[0] = topJets[0].phi()
	    	self.jet2Phi[0] = topJets[1].phi()
            	self.sumJetPt[0] = HTsum
            	self.jet1MassT[0] = topJets[0].mass()
            	self.jet2MassT[0] = topJets[1].mass()
	    	self.jet1CHE[0] = topJetsCHE[0]
		self.jet1NE[0] = topJetsNE[0]
		self.jet1NumPF[0] = topJetsNumPF[0]
		self.jet1NumCH[0] = topJetsNumCH[0]
		self.jet1NumNE[0] = topJetsNumNE[0]
	    	self.jet2CHE[0] = topJetsCHE[1]
		self.jet2NE[0] = topJetsNE[1]
		self.jet2NumPF[0] = topJetsNumPF[1]
		self.jet2NumCH[0] = topJetsNumCH[1]
		self.jet2NumNE[0] = topJetsNumNE[1]
		self.trigWt[0] = weight
	    	self.event[0] = event.object().id().event()
		self.run[0] = event.object().id().run()
		self.lumi[0] = event.object().id().luminosityBlock()
		self.treeVars.Fill()
           	self.mttMass.Fill( ttMass, weight )
		if not self.useMC :
                    self.runPairs.append( [event.object().id().run(),
                                           event.object().id().event(),
                                           event.object().id().luminosityBlock() ,
                                           ttMass] )
                self.mttMassTriggerWeighted.Fill( ttMass, weight )   

		h_mtt = Handle("double")
                event.getByLabel( ("ttbsmAna", "mttgen"), h_mtt)
                if h_mtt.isValid():
                	mtt = h_mtt.product()
                	self.mtt_gen.Fill(mtt[0])
                	self.mtt_gen_vs_reco.Fill(mtt[0], ttMass)



            #background estiation
            x = ROOT.gRandom.Uniform()        
	    eventStr = str(event.object().id().run()) + ' ' + str(event.object().id().event()) + '\n'
	    self.textfile.write(eventStr)

	    mistagWt1 = self.mistag.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
	    mistagWt0 = self.mistag.GetBinContent( self.mistag.FindBin(topJets[0].pt()) ) 


    	    if x < 0.5 :
                if topTag0:
	    	    self.mistagWt[0] = self.mistag.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
                    mistagRateErr = self.mistag.GetBinError(self.mistag.FindBin(topJets[1].pt() ))
 		    self.jetPtOneTag.Fill( topJets[1].pt(), weight )
                    self.jetMassOneTag.Fill( topJets[1].mass(), weight )
                    self.mttPredDist.             Accumulate( ttMass,             topJets[1].pt(), topTag1, weight )
                    self.mttPredDistErr.Fill(ttMass, mistagRateErr)
                    self.ptPredDistErr.Fill(topJets[1].pt(), mistagRateErr)
                    self.mttPredDistMassCut.      Accumulate( ttMass,             topJets[1].pt(), topTag1, weight )
                    self.mttPredDistModMassFlat.  Accumulate( ttMassJet1MassFlat, topJets[1].pt(), topTag1, weight )
                    self.mttPredDistMod3MassFlat. Accumulate( ttMassMod3Jet1Flat, topJets[1].pt(), topTag1, weight )
                    self.mttPredDistMod3MassFlatSubtract. Accumulate( ttMassMod3Jet1Flat, topJets[1].pt(), topTag1, weight )
                    self.mttPredDistModMassQCDdistSubtract. Accumulate( ttMassModJet1QCDdist, topJets[1].pt(), topTag1, weight )
		    self.mttMassT[0] = ttMassModJet1QCDdist
	    	    self.index[0] = 0
	      	    self.jet1PtT[0] = topJets[0].pt() 
            	    self.jet2PtT[0] = topJets[1].pt()
            	    self.jet1Eta[0] = topJets[0].Rapidity()
	    	    self.jet2Eta[0] = topJets[1].Rapidity()
            	    self.jet1Phi[0] = topJets[0].phi()
	    	    self.jet2Phi[0] = topJets[1].phi()
	    	    self.jet1CHE[0] = topJetsCHE[0]
		    self.jet1NE[0] = topJetsNE[0]
		    self.jet1NumPF[0] = topJetsNumPF[0]
		    self.jet1NumCH[0] = topJetsNumCH[0]
		    self.jet1NumNE[0] = topJetsNumNE[0]
	    	    self.jet2CHE[0] = topJetsCHE[1]
	     	    self.jet2NE[0] = topJetsNE[1]
	  	    self.jet2NumPF[0] = topJetsNumPF[1]
	  	    self.jet2NumCH[0] = topJetsNumCH[1]
	  	    self.jet2NumNE[0] = topJetsNumNE[1]
            	    self.sumJetPt[0] = HTsum
            	    self.jet1MassT[0] = topJets[0].mass()
            	    self.jet2MassT[0] = myrand_QCDdist_jet1
	    	    self.trigWt[0] = weight
	    	    self.treeVars.Fill()
           	    self.ptPredDist.Accumulate(topJets[1].pt(), topJets[1].pt(), topTag1, weight)
            if x >= 0.5 :
                if topTag1:
	    	    self.mistagWt[0] = self.mistag.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
                    mistagRateErr = self.mistag.GetBinError(self.mistag.FindBin(topJets[0].pt()) )
		    self.jetPtOneTag.Fill( topJets[0].pt(), weight )
                    self.jetMassOneTag.Fill( topJets[0].mass(), weight )
                    self.mttPredDist.             Accumulate( ttMass,             topJets[0].pt(), topTag0, weight )
                    self.mttPredDistErr.Fill(ttMass, mistagRateErr)
                    self.ptPredDistErr.Fill(topJets[0].pt(), mistagRateErr)
		    self.mttPredDistMassCut.      Accumulate( ttMass,             topJets[0].pt(), topTag0, weight )
                    self.mttPredDistModMassFlat.  Accumulate( ttMassJet0MassFlat, topJets[0].pt(), topTag0, weight )
                    self.mttPredDistMod3MassFlat. Accumulate( ttMassMod3Jet0Flat, topJets[0].pt(), topTag0, weight )
                    self.mttPredDistMod3MassFlatSubtract. Accumulate( ttMassMod3Jet0Flat, topJets[0].pt(), topTag0, weight )
                    self.mttPredDistModMassQCDdistSubtract. Accumulate( ttMassModJet0QCDdist, topJets[0].pt(), topTag0, weight )
		    self.ptPredDist.Accumulate(topJets[0].pt(), topJets[0].pt(), topTag0, weight)
		    self.mttMassT[0] = ttMassModJet0QCDdist
	    	    self.index[0] = 0
	      	    self.jet1PtT[0] = topJets[0].pt() 
            	    self.jet2PtT[0] = topJets[1].pt()
            	    self.jet1Eta[0] = topJets[0].Rapidity()
	    	    self.jet2Eta[0] = topJets[1].Rapidity()
            	    self.jet1Phi[0] = topJets[0].phi()
	    	    self.jet2Phi[0] = topJets[1].phi()
	    	    self.jet1CHE[0] = topJetsCHE[0]
		    self.jet1NE[0] = topJetsNE[0]
	    	    self.jet1NumPF[0] = topJetsNumPF[0]
	    	    self.jet1NumCH[0] = topJetsNumCH[0]
	    	    self.jet1NumNE[0] = topJetsNumNE[0]
	    	    self.jet2CHE[0] = topJetsCHE[1]
		    self.jet2NE[0] = topJetsNE[1]
		    self.jet2NumPF[0] = topJetsNumPF[1]
		    self.jet2NumCH[0] = topJetsNumCH[1]
		    self.jet2NumNE[0] = topJetsNumNE[1]
            	    self.sumJetPt[0] = HTsum
            	    self.jet1MassT[0] = myrand_QCDdist_jet0
            	    self.jet2MassT[0] = topJets[1].mass()
	    	    self.trigWt[0] = weight
	    	    self.treeVars.Fill()



