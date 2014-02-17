import ROOT
import copy
from math import *
from operator import itemgetter, attrgetter

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *
from array import array

class TTPairAnalyzer:
    
    def __init__(self, useMC, outfile, mistagFile, collectionLabelSuffix, useGenWeight, triggerFile, modMassFile, triggerWeight, readGenInfo, pdfWeight):
        
        self.outfile = outfile
        self.mistagFileStr = mistagFile
        self.triggerFileStr = triggerFile
        self.modMassFileStr = modMassFile
        self.useMC = useMC
        self.useGenWeight=useGenWeight
        self.readGenInfo=readGenInfo
        self.pdfWeight = "nominal"#pdfWeight
        self.triggerWeight = triggerWeight
        
        label = 'ttbsmAna'  + collectionLabelSuffix

        
        self.allTopTagHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagLabel  = ( label,   "topTagP4")
        self.allca8Handle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allca8Label  = ( label,   "ca8JetP4" )
        self.allwTagHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allwTagLabel  = ( label,   "wTagP4")
        self.allTopTagSubjetsHandle = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.allTopTagSubjetsLabel  = ( label,   "topTagSubjet1P4")

        self.allTopTagTopMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagTopMassLabel   = ( label,   "topTagTopMass" )
        self.allTopTagMinMassHandle  = Handle( "std::vector<double>" )
        self.allTopTagMinMassLabel   = ( label,   "topTagMinMass" )
        self.allTopTagNSubjetsHandle    = Handle("std::vector<double>" )
        self.allTopTagNSubjetsLabel     = ( label,   "topTagNSubjets" )

	self.allTopTagPartonHandle = Handle("std::vector<int>" )
	self.allTopTagPartonLabel = ( label, "topTagPartonFlavour" )
        
        self.allwTagBdiscHandle       = Handle( "std::vector<double>" )
        self.allwTagBdiscLabel  = ( label,   "wTagBDisc" )
        
        self.allwTagMuHandle           = Handle( "std::vector<double>" )
        self.allwTagMuLabel  = ( label,   "wTagMu" )

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
        
        self.h_met = Handle (  "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >"  )
        self.metLabel = ( label, "pfMET" )
        self.weightsHandle = Handle( "double" )
        self.weightsLabel = ( label, "weight" )
        self.pdfHandle = Handle("std::vector<double>")
        self.pdfLabel = ( label, "pdfWeights")
        
        
        self.__book__()
        
        
    def __book__(self):
    
        print 'Booking Histograms and Trees...'
        
        self.mistagFile = ROOT.TFile("ALLjets_mistag.root")
        self.mistagFile.cd()
        self.mistag = self.mistagFile.Get("MISTAG_RATE_SUB_TTBAR_Inclusive").Clone()
        self.mistag.SetName('mistag')
        ROOT.SetOwnership( self.mistag, False )
        
        
        self.mistagFileWP1 = ROOT.TFile("mistag_WP1.root")
        self.mistagFileWP1.cd()
        self.mistagWP1 = self.mistagFileWP1.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistagWP1.SetName('mistagWP1')
        ROOT.SetOwnership( self.mistagWP1, False )        
        
        self.mistagFileWP2 = ROOT.TFile("mistag_WP2.root")
        self.mistagFileWP2.cd()
        self.mistagWP2 = self.mistagFileWP2.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistagWP2.SetName('mistagWP2')
        ROOT.SetOwnership( self.mistagWP2, False ) 
        
        self.mistagFileWP3 = ROOT.TFile("mistag_WP3.root")
        self.mistagFileWP3.cd()
        self.mistagWP3 = self.mistagFileWP3.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistagWP3.SetName('mistagWP3')
        ROOT.SetOwnership( self.mistagWP3, False ) 
        
        self.mistagFileWP4 = ROOT.TFile("mistag_WP4.root")
        self.mistagFileWP4.cd()
        self.mistagWP4 = self.mistagFileWP4.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistagWP4.SetName('mistagWP4')
        ROOT.SetOwnership( self.mistagWP4, False ) 
        
        self.mistagFileWP5 = ROOT.TFile("mistag_WP5.root")
        self.mistagFileWP5.cd()
        self.mistagWP5 = self.mistagFileWP5.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistagWP5.SetName('mistagWP5')
        ROOT.SetOwnership( self.mistagWP5, False ) 
       
        self.mistagFile1B = ROOT.TFile("bjets_mistag.root")
        self.mistagFile1B.cd()
        self.mistag1B = self.mistagFile1B.Get("MISTAG_RATE_SUB_TTBAR_1btagT").Clone()
        self.mistag1B.SetName('mistag1B')
        ROOT.SetOwnership( self.mistag1B, False )

        self.mistagFile2B = ROOT.TFile("bjets_mistag.root")
        self.mistagFile2B.cd()
        self.mistag2B = self.mistagFile2B.Get("MISTAG_RATE_SUB_TTBAR_2btagT").Clone()
        self.mistag2B.SetName('mistag2B')
        ROOT.SetOwnership( self.mistag2B, False )

        self.mistagFile_type12 = ROOT.TFile("mistag_type12.root")
        self.mistagFile_type12.cd()
        self.mistag_type12 = self.mistagFile_type12.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
        self.mistag_type12.SetName('mistag_type12')
        ROOT.SetOwnership( self.mistag_type12, False )


        self.mistagFile_type12_1B = ROOT.TFile("mistag_type12_oneB.root")
        self.mistagFile_type12_1B.cd()
	self.mistag_type12_1B = self.mistagFile_type12.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
	self.mistag_type12_1B.SetName('mistag_type12_1B')
	ROOT.SetOwnership( self.mistag_type12_1B, False )
 
        self.triggerFile = ROOT.TFile(self.triggerFileStr + ".root")
        self.triggerFile.cd()
        self.triggerHist = self.triggerFile.Get("TRIGGER_EFF").Clone()
        self.triggerHist.SetName('triggerHist')
        ROOT.SetOwnership( self.triggerHist, False )
        
        self.modMassFile = ROOT.TFile(self.modMassFileStr + ".root")
        self.modMassFile.cd()
        self.modMassHist = ROOT.TH1F()
        self.modMassHist = self.modMassFile.Get("jetMassOneTag_MassWindow").Clone()
        self.modMassHist.SetName('modMassHist')
        ROOT.SetOwnership( self.modMassHist, False )
        
        self.f = ROOT.TFile( self.outfile + ".root", "recreate" )
        self.f.cd()
        self.f.ShowStreamerInfo() 

        
        self.treeVars = ROOT.TTree('treeVars', 'treeVars')
        self.mttMass = array('f', [-1.])
        self.mttMassGroomed = array('f', [-1.])
        self.mttMassPred = array('f', [-1.])
        self.mistagWt = array('f', [-1.])
        self.mistagWt1B = array('f', [-1.])
        self.mistagWt2B = array('f', [-1.])
        self.mistagWtType12 = array('f', [-1.])
        self.mistagWtType121B = array('f', [-1.])
	self.npv = array('i', [-1])
        self.index = array('f', [-1.])
        self.nTagsTau32 = array('i', [-1])
	self.nTagsBLoose = array('i', [-1])
	self.nTagsBTight = array('i', [-1])
	self.trigWt = array('f', [-1.])
        self.jet1Pt = array('f', [-1.])
        self.jet2Pt = array('f', [-1.])
        self.sumJetPt = array('f', [-1.])
        self.jet1Mass = array('f', [-1.])
        self.jet2Mass = array('f', [-1.])
        self.jet1Eta = array('f', [-1.])
        self.jet2Eta = array('f', [-1.])
        self.jet1Phi = array('f', [-1.])
        self.jet2Phi = array('f', [-1.])
        self.jet1Parton = array('i', [-999])
	self.jet2Parton = array('i', [-999])
	self.MET = array('f', [-1.])
        self.run = array('i', [-1])
        self.event = array('l', [-1])
        self.lumi = array('i', [-1])
        #self.jet1CHE = array('f', [-1.])
        #self.jet1NE = array('f', [-1.])
        #self.jet1NumPF = array('i', [-1])
        #self.jet2CHE = array('f', [-1.])
        #self.jet2NE = array('f', [-1.])
        #self.jet2NumPF = array('i', [-1])
        #self.jet1NumCH = array('i', [-1])
        #self.jet2NumCH = array('i', [-1])
        #self.jet1NumNE = array('i', [-1])
        #self.jet2NumNE = array('i', [-1])
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
        self.jet2WMu = array('f', [-1.])
	self.jet2Wtau21 = array('f', [-1.])
	self.deltaY = array('f', [-1.])
	self.deltaPhi = array('f', [-1.])
	self.type11 = array('i', [0])
	self.type12 = array('i', [0])
	self.jet3Pt = array('f', [-1.])
	self.jet3Eta = array('f', [-1.])
	self.jet3Phi = array('f', [-1.])
	self.jet3Mass = array('f', [-1.])
	self.jet3BDisc = array('f', [-1.])
	self.WbMass = array('f', [-1.])
	self.DRWb = array('f', [-1.])
	self.NNoutput = array('f', [-99.])
	self.NNoutputExp = array('f', [-99.])
	self.cutflow = array('f', [-1])
       
	self.treeVars.Branch('NNoutput', self.NNoutput, 'NNoutput/F') 
	self.treeVars.Branch('NNoutputExp', self.NNoutputExp, 'NNoutputExp/F') 
        self.treeVars.Branch('jet1SubjetMaxBDisc', self.jet1SubjetMaxBDisc, 'jet1SubjetMaxBDisc/F')
        self.treeVars.Branch('jet1BDisc', self.jet1BDisc, 'jet1BDisc/F')
        self.treeVars.Branch('jet1MassGroomed', self.jet1MassGroomed, 'jet1MassGroomed/F')
        self.treeVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
        self.treeVars.Branch('jet2SubjetMaxBDisc', self.jet2SubjetMaxBDisc, 'jet2SubjetMaxBDisc/F')
        self.treeVars.Branch('jet2BDisc', self.jet2BDisc, 'jet2BDisc/F')
        self.treeVars.Branch('jet2MassGroomed', self.jet2MassGroomed, 'jet2MassGroomed/F')
        self.treeVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
        self.treeVars.Branch('mttMass', self.mttMass, 'mttMass/F')
        self.treeVars.Branch('mttMassGroomed', self.mttMassGroomed, 'mttMassGroomed/F')
        self.treeVars.Branch('mttMassPred', self.mttMassPred, 'mttMassPred/F')
        self.treeVars.Branch('mistagWt', self.mistagWt, 'mistagWt/F')
        self.treeVars.Branch('mistagWt1B', self.mistagWt1B, 'mistagWt1B/F')
        self.treeVars.Branch('mistagWt2B', self.mistagWt2B, 'mistagWt2B/F')
        self.treeVars.Branch('mistagWtType12', self.mistagWtType12, 'mistagWtType12/F')
        self.treeVars.Branch('mistagWtType121B', self.mistagWtType121B, 'mistagWtType121B/F')
	self.treeVars.Branch('jet3Pt', self.jet3Pt, 'jet3Pt/F')
        self.treeVars.Branch('jet3Eta', self.jet3Eta, 'jet3Eta/F')
        self.treeVars.Branch('jet3Phi', self.jet3Phi, 'jet3Phi/F')
        self.treeVars.Branch('jet3Mass', self.jet3Mass, 'jet3Mass/F')
        self.treeVars.Branch('jet3BDisc', self.jet3BDisc, 'jet3BDisc/F')
        self.treeVars.Branch('DRWb', self.DRWb, 'DRWb/F')
	self.treeVars.Branch('npv', self.npv, 'npv/I')
        self.treeVars.Branch('index', self.index, 'index/F')
        self.treeVars.Branch('type11', self.type11, 'type11/I')
	self.treeVars.Branch('type12', self.type12, 'type12/I')
	self.treeVars.Branch('trigWt', self.trigWt, 'trigWt/F')
        self.treeVars.Branch('jet1Pt', self.jet1Pt, 'jet1Pt/F')
        self.treeVars.Branch('jet2Pt', self.jet2Pt, 'jet2Pt/F')
        self.treeVars.Branch('sumJetPt', self.sumJetPt, 'sumJetPt/F')
        self.treeVars.Branch('jet1Mass', self.jet1Mass, 'jet1Mass/F')
        self.treeVars.Branch('jet2Mass', self.jet2Mass, 'jet2Mass/F')
        self.treeVars.Branch('jet1Eta', self.jet1Eta, 'jet1Eta/F')
        self.treeVars.Branch('jet2Eta', self.jet2Eta, 'jet2Eta/F')
        self.treeVars.Branch('jet1Phi', self.jet1Phi, 'jet1Phi/F')
        self.treeVars.Branch('jet2Phi', self.jet2Phi, 'jet2Phi/F')
        self.treeVars.Branch('jet1Parton', self.jet1Parton, 'jet1Parton/I')
	self.treeVars.Branch('jet2Parton', self.jet2Parton, 'jet2Parton/I')
	self.treeVars.Branch('MET', self.MET, 'MET/F')
        self.treeVars.Branch('run', self.run, 'run/I')
        self.treeVars.Branch('event', self.event, 'event/L')
        self.treeVars.Branch('lumi', self.lumi, 'lumi/I')
        self.treeVars.Branch('WbMass', self.WbMass, 'WbMass/F')
	#self.treeVars.Branch('jet1CHE', self.jet1CHE, 'jet1CHE/F')
        #self.treeVars.Branch('jet1NE', self.jet1NE, 'jet1NE/F')
        #self.treeVars.Branch('jet1NumPF', self.jet1NumPF, 'jet1NumPF/I')
        #self.treeVars.Branch('jet2CHE', self.jet2CHE, 'jet2CHE/F')
        #self.treeVars.Branch('jet2NE', self.jet2NE, 'jet2NE/F')
        #self.treeVars.Branch('jet2NumPF', self.jet2NumPF, 'jet2NumPF/I')
        #self.treeVars.Branch('jet1NumCH', self.jet1NumCH, 'jet1NumCH/I')
        #self.treeVars.Branch('jet2NumCH', self.jet2NumCH, 'jet2NumCH/I')
        #self.treeVars.Branch('jet1NumNE', self.jet1NumNE, 'jet1NumNE/I')
        #self.treeVars.Branch('jet2NumNE', self.jet2NumNE, 'jet2NumNE/I')
        self.treeVars.Branch('jet1Nsubj', self.jet1Nsubj, 'jet1Nsubj/I')
        self.treeVars.Branch('jet2Nsubj', self.jet2Nsubj, 'jet2Nsubj/I')
        self.treeVars.Branch('jet2WMu', self.jet2WMu, 'jet2WMu/F')
	self.treeVars.Branch('jet2Wtau21', self.jet2Wtau21, 'jet2Wtau21/F')
	self.treeVars.Branch('jet1minMass', self.jet1minMass, 'jet1minMass/F')
        self.treeVars.Branch('jet2minMass', self.jet2minMass, 'jet2minMass/F')
        self.treeVars.Branch('nTagsTau32', self.nTagsTau32, 'nTagsTau32/I')
	self.treeVars.Branch('nTagsBLoose', self.nTagsBLoose, 'nTagsBLoose/I')
	self.treeVars.Branch('nTagsBTight', self.nTagsBTight, 'nTagsBTight/I') 
	self.treeVars.Branch('deltaY', self.deltaY, 'deltaY/F')
	self.treeVars.Branch('deltaPhi', self.deltaPhi, 'deltaPhi/F')
	
	self.treeVars.Branch('cutflow', self.cutflow, 'cutflow/F')
        
        self.runPairs = []
       
	self.reader = ROOT.TMVA.Reader()
	self.reader.AddVariable("jet1Eta", self.jet1Eta)
	self.reader.AddVariable("jet2Eta", self.jet2Eta)
	self.reader.AddVariable("deltaY", self.deltaY)
	self.reader.AddVariable("jet1Mass", self.jet1Mass)
	self.reader.AddVariable("jet2Mass", self.jet2Mass)
	self.reader.AddVariable("jet1BDisc", self.jet1BDisc)
	self.reader.AddVariable("jet2BDisc", self.jet2BDisc)
	self.reader.AddVariable("jet1SubjetMaxBDisc", self.jet1SubjetMaxBDisc)
	self.reader.AddVariable("jet2SubjetMaxBDisc", self.jet2SubjetMaxBDisc)
	self.reader.AddVariable("jet1tau32", self.jet1tau32)
	self.reader.AddVariable("jet2tau32", self.jet2tau32)
	self.reader.AddVariable("jet1Pt", self.jet1Pt)
	self.reader.AddVariable("jet2Pt", self.jet2Pt)
	self.reader.AddVariable("mttMass", self.mttMass)

	self.reader.BookMVA("MLP", "NNfiles/TMVA_tt_Zp_MLP.weights.Oct14_2btag_Zp20.xml")
	self.reader.BookMVA("ExpertMLP", "NNfiles/TMVA_tt_QCD_MLP.weights.xml")




 
    def __del__(self):
        
        self.f.cd()
        self.f.Write()
        self.f.Close()
        self.mistagFile.Close()
        self.triggerFile.Close()
        self.modMassFile.Close()
    
            
    def analyze(self,event):

    
        self.run[0] = event.object().id().run()
        self.event[0] = event.object().id().event()
        self.lumi[0] = event.object().id().luminosityBlock()
    
    
        event.getByLabel (self.allTopTagLabel, self.allTopTagHandle)
        topJets = self.allTopTagHandle.product()
        
        event.getByLabel (self.allwTagLabel, self.allwTagHandle)
        wJets = self.allwTagHandle.product()
        
        event.getByLabel (self.allTopTagSubjetsLabel, self.allTopTagSubjetsHandle)
        event.getByLabel (self.allca8Label, self.allca8Handle)
        event.getByLabel (self.allNsub1Label, self.allNsub1Handle)
        event.getByLabel (self.allNsub2Label, self.allNsub2Handle)
        event.getByLabel (self.allNsub3Label, self.allNsub3Handle)
        event.getByLabel (self.allNsub4Label, self.allNsub4Handle)

        event.getByLabel (self.allTopTagBDiscLabel, self.allTopTagBDiscHandle)
        event.getByLabel (self.allwTagBdiscLabel, self.allwTagBdiscHandle)
        event.getByLabel (self.allwTagMuLabel, self.allwTagMuHandle)
        event.getByLabel (self.allTopTagSubjetBDiscLabel, self.allTopTagSubjetBDiscHandle)
        event.getByLabel (self.allTopTagSubjetJetIDLabel, self.allTopTagSubjetJetIDHandle)
	event.getByLabel (self.allTopTagPartonLabel, self.allTopTagPartonHandle)

        topSubjets = self.allTopTagSubjetsHandle.product()
	topPartonFlavors = self.allTopTagPartonHandle.product()
        ca8Jets = self.allca8Handle.product()
        tau1Vals = self.allNsub1Handle.product()
        tau2Vals = self.allNsub2Handle.product()
        tau3Vals = self.allNsub3Handle.product()
        tau4Vals = self.allNsub4Handle.product()

        topBDisc = self.allTopTagBDiscHandle.product()
        wBDisc = self.allwTagBdiscHandle.product()
        wMu = self.allwTagMuHandle.product()
        topSubjetsBDisc = self.allTopTagSubjetBDiscHandle.product()
        topJetID = self.allTopTagSubjetJetIDHandle.product()
        
        event.getByLabel (self.allTopTagMinMassLabel, self.allTopTagMinMassHandle)
        topJetMinMass= self.allTopTagMinMassHandle.product()
        event.getByLabel (self.allTopTagNSubjetsLabel, self.allTopTagNSubjetsHandle)
        topJetNSubjets= self.allTopTagNSubjetsHandle.product()
        event.getByLabel (self.allTopTagTopMassLabel, self.allTopTagTopMassHandle)
        topJetMass= self.allTopTagTopMassHandle.product()
        
        
        event.getByLabel( self.metLabel, self.h_met )
        mets = self.h_met.product()
        self.MET[0] = mets[0].pt()
        self.type11[0] = 0
	self.type12[0] = 0 
        self.cutflow[0] = 0 #Number of events seen by analyzer
        self.index[0] = 0
	self.treeVars.Fill()
        #Break if not at least 1 type-1 candidate
        HTsum = 0.0 #Calculated for trigger efficiency
        nTopCand = 0
        for i in range(0,len(topJets) ) :
          if( topJets[i].pt() > 400 ) :
            nTopCand = nTopCand + 1
          HTsum += topJets[i].pt()
        if nTopCand < 1 :
            return
            
        self.jet1Pt[0] = topJets[0].pt()
        self.jet1Mass[0] = topJetMass[0]
        self.jet1Eta[0] = topJets[0].eta()
        self.jet1Phi[0] = topJets[0].phi()
   	self.jet1Parton[0] = topPartonFlavors[0] 
	        
    
        self.cutflow[0] = 1 #Events with >= 1 top candidate
        
	theMatchedW = -1

	jet3 = -1
        
        
        #Find subjets of first top candidate
        tagJet0Subjets = []
        tagJet0BDiscs = []
    
        i = 0   
        while i < (topJetNSubjets[0]):
            tagJet0Subjets.append(topSubjets[i])
            tagJet0BDiscs.append(topSubjetsBDisc[i])
            i += 1

        tau32_jet1 = tau3Vals[0] / tau2Vals[0]
        maxSubjBDisc_jet1 = max(tagJet0BDiscs)
        
        groomed_jet1 = ROOT.TLorentzVector(0.0,0.0,0.0,0.0)
        for subjet in tagJet0Subjets:
                groomed_jet1 += ROOT.TLorentzVector(subjet.px(), subjet.py(), subjet.pz(), subjet.energy())
                
        
        
        self.jet1SubjetMaxBDisc[0] = maxSubjBDisc_jet1
	self.jet1BDisc[0] = topBDisc[0]
	self.jet1MassGroomed[0] = groomed_jet1.M()
	self.jet1tau32[0] = tau32_jet1
	self.jet1Nsubj[0] = topJetNSubjets[0]
	self.jet1minMass[0] = topJetMinMass[0]
	self.treeVars.Fill()
        
        
	tagParams = [140.0, 250.0, 3.0, 50.0] #Mass window, Nsubj, and minMass cuts for tag algorithm

	failType11 = 0

	if ( self.jet1Mass[0] > tagParams[0] and self.jet1Mass[0] < tagParams[1] and self.jet1Nsubj[0] >= tagParams[2] and self.jet1minMass[0] > tagParams[3]):
		# jet 1 is top tagged
		self.cutflow[0] = 2
		self.treeVars.Fill()
		if (len(topJets) < 2):
			return
		if( topJets[1].pt() > 400 ) :		
			
			self.cutflow[0] = 3
			self.jet2Pt[0] = topJets[1].pt()
        		self.jet2Mass[0] = topJetMass[1]
        		self.jet2Eta[0] = topJets[1].eta()
        		self.jet2Phi[0] = topJets[1].phi()
   			self.jet2Parton[0] = topPartonFlavors[1]
			     
       			tagJet1Subjets = []
        		tagJet1BDiscs = []
        
       			i = 0
        		while i < (topJetNSubjets[1]):
            			tagJet1Subjets.append(topSubjets[i+int(topJetNSubjets[0])])
            			tagJet1BDiscs.append(topSubjetsBDisc[i+int(topJetNSubjets[0])])
            			i += 1

        		tau32_jet2 = tau3Vals[1] / tau2Vals[1]
        		maxSubjBDisc_jet2 = max(tagJet1BDiscs)

        		groomed_jet2 = ROOT.TLorentzVector(0.0,0.0,0.0,0.0)
        		for subjet in tagJet1Subjets:
                		groomed_jet2 += ROOT.TLorentzVector(subjet.px(), subjet.py(), subjet.pz(), subjet.energy())



 		        self.jet2SubjetMaxBDisc[0] = maxSubjBDisc_jet2
        		self.jet2BDisc[0] = topBDisc[1]
        		self.jet2MassGroomed[0] = groomed_jet2.M()
        		self.jet2tau32[0] = tau32_jet2
        		self.jet2Nsubj[0] = topJetNSubjets[1]
        		self.jet2minMass[0] = topJetMinMass[1]
        		self.treeVars.Fill() 
        
        
        
       			if ( self.jet2Mass[0] > tagParams[0] and self.jet2Mass[0] < tagParams[1] and self.jet2Nsubj[0] >= tagParams[2] and self.jet2minMass[0] > tagParams[3]):

				self.cutflow[0] = 4
				self.type11[0] = 1	
				
				self.NNoutput[0] = self.reader.EvaluateMVA("MLP")
				self.NNoutputExp[0] = self.reader.EvaluateMVA("ExpertMLP")
				if (self.NNoutput[0] < 0.0):
					self.NNoutput[0] = 0.001
				if (self.NNoutput[0] > 1.0):
					self.NNoutput[0] = 0.999
				if (self.NNoutputExp[0] < 0.0):
					self.NNoutputExp[0] = 0.001
				if (self.NNoutputExp[0] > 1.0):
					self.NNoutputExp[0] = 0.999
				
				self.mttMass[0] = (topJets[0] + topJets[1]).M()
				self.mttMassGroomed[0] = (groomed_jet1 + groomed_jet2).M()				

				self.deltaY[0] = (topJets[0].Rapidity() - topJets[1].Rapidity())
				self.deltaPhi[0] = (topJets[0].phi() - topJets[1].phi())

				self.nTagsTau32[0] = (tau32_jet1 < 0.70) + (tau32_jet2 < 0.70)
				self.nTagsBLoose[0] = (self.jet1SubjetMaxBDisc[0] > 0.244) + (self.jet2SubjetMaxBDisc[0] > 0.244)
				self.nTagsBTight[0] = (self.jet1SubjetMaxBDisc[0] > 0.679) + (self.jet2SubjetMaxBDisc[0] > 0.679)

				self.treeVars.Fill()

				if (tau32_jet1 < 0.70 and tau32_jet2 < 0.70 and self.jet1SubjetMaxBDisc[0] > 0.244 and self.jet2SubjetMaxBDisc[0] > 0.244) :
					self.cutflow[0] = 4.1
					self.treeVars.Fill()
					
				if (tau32_jet1 < 0.60 and tau32_jet2 < 0.60 and self.jet1SubjetMaxBDisc[0] > 0.244 and self.jet2SubjetMaxBDisc[0] > 0.244) :
					self.cutflow[0] = 4.2
					self.treeVars.Fill()
					
				if (tau32_jet1 < 0.55 and tau32_jet2 < 0.55 and self.jet1SubjetMaxBDisc[0] > 0.679 and self.jet2SubjetMaxBDisc[0] > 0.679) :
					self.cutflow[0] = 4.3
					self.treeVars.Fill()
					
				if (tau32_jet1 < 0.50 and tau32_jet2 < 0.50 and self.jet1SubjetMaxBDisc[0] > 0.679 and self.jet2SubjetMaxBDisc[0] > 0.679 and self.jet1minMass[0] > 65 and self.jet2minMass[0] > 65) :
					self.cutflow[0] = 4.4
					self.treeVars.Fill()					

				if (tau32_jet1 < 0.40 and tau32_jet2 < 0.40 and self.jet1SubjetMaxBDisc[0] > 0.679 and self.jet2SubjetMaxBDisc[0] > 0.679 and self.jet1minMass[0] > 55 and self.jet2minMass[0] > 55) :
					self.cutflow[0] = 4.5
					self.treeVars.Fill()
					
					

				if (bool(self.jet1SubjetMaxBDisc[0] > 0.679) != bool(self.jet2SubjetMaxBDisc[0] > 0.679)) :
					self.cutflow[0] = 5
					self.treeVars.Fill()
				if (self.jet1SubjetMaxBDisc[0] > 0.679 and self.jet2SubjetMaxBDisc[0] > 0.679) :
					self.cutflow[0] = 6
					self.treeVars.Fill()

			else :
				failType11 = 1

		if failType11:
			DR_top_wjet0 = sqrt( (topJets[0].eta() - wJets[0].eta())**2 + (topJets[0].phi() - wJets[0].phi())**2 )
			theWjetCand = wJets[0]
			theWjetCandIndex = 0
			if (DR_top_wjet0 < 0.8 and len(wJets) > 1) :
				theWjetCand = wJets[1]
				theWjetCandIndex = 1

			if ((theWjetCand.pt() > 200) and len(wJets) > 0):

				self.cutflow[0] = 7
				self.jet2Pt[0] = theWjetCand.pt()
                	        self.jet2Mass[0] = theWjetCand.M()
                	        self.jet2Eta[0] = theWjetCand.eta()
                	        self.jet2Phi[0] = theWjetCand.phi()
				self.jet2WMu[0] = wMu[theWjetCandIndex]
				self.jet2BDisc[0] = wBDisc[theWjetCandIndex]
	
	
				minDR = 999.
				minDRjet3 = 999.
				i = 0	
				for jet in ca8Jets:
					
					deltaR_W_ca8 = sqrt( (theWjetCand.eta() - jet.eta())**2 + (theWjetCand.phi() - jet.phi())**2 )
					if (deltaR_W_ca8 < minDR):
						minDR = deltaR_W_ca8
						theMatchedW = i
					
	
	
					i = i+1

			
				self.jet2Wtau21[0] = tau2Vals[theMatchedW] / tau1Vals[theMatchedW]
			


				self.treeVars.Fill()


				if (theWjetCand.M() > 60 and theWjetCand.M() < 100 and wMu[theWjetCandIndex] < 0.4):
					self.cutflow[0] = 8
					self.treeVars.Fill()
	
					
					i = 0
					for jet in wJets:
						deltaR_W_jet3 = sqrt( (theWjetCand.eta() - jet.eta())**2 + (theWjetCand.phi() - jet.phi())**2 )
						if (deltaR_W_jet3 < minDRjet3 and deltaR_W_jet3 > 0.8) :
							minDRjet3 = deltaR_W_jet3
							jet3 = i
						i = i+1

			


					if (theWjetCandIndex != jet3 and jet3 > 1):
					
						self.cutflow[0] = 9
						self.jet3Pt[0] = wJets[jet3].pt()
						self.jet3Eta[0] = wJets[jet3].eta()
						self.jet3Phi[0] = wJets[jet3].phi()
						self.jet3Mass[0] = wJets[jet3].M()
						self.jet3BDisc[0] = wBDisc[jet3] 
						
						self.DRWb[0] = minDRjet3
										
		
						self.type12[0] = 1
		
						type2top = (wJets[theWjetCandIndex] + wJets[jet3])
						self.WbMass[0] = type2top.M()
						self.deltaPhi[0] = type2top.phi() - topJets[0].phi()
						self.deltaY[0] = type2top.Rapidity() - topJets[0].Rapidity()
		
						self.mttMass[0] = (type2top + topJets[0]).M()
						self.treeVars.Fill()
	


						if (wBDisc[jet3] > 0.679):
							self.cutflow[0] = 10
							self.treeVars.Fill()
	
						if (wBDisc[jet3] > 0.679 and self.jet1SubjetMaxBDisc[0] > 0.679):
							self.cutflow[0] = 11
							self.treeVars.Fill()


	#Mistag contribution calculation

	topTag0 = (topJetMass[0] > tagParams[0] and topJetMass[0] < tagParams[1] and topJetNSubjets[0] >= tagParams[2] and topJetMinMass[0] > tagParams[3] and topJets[0].pt() > 400)	 
	topTag1 = 0
	if (nTopCand > 1):
		topTag1 = (topJetMass[1] > tagParams[0] and topJetMass[1] < tagParams[1] and topJetNSubjets[1] >= tagParams[2] and topJetMinMass[1] > tagParams[3] and topJets[1].pt() > 400)
	jet1BDiscs = []
	jet2BDiscs = []
	i = 0
        while i < (topJetNSubjets[0]):
        	jet1BDiscs.append(topSubjetsBDisc[i])
        	i += 1


	#Type 1+2 calculation
	type11 = topTag0 and topTag1
	if wJets.size() > 1 and not type11:

		DR_top_wjet0 = sqrt( (topJets[0].eta() - wJets[0].eta())**2 + (topJets[0].phi() - wJets[0].phi())**2 )
                theWjetCand = wJets[0]
                theWjetCandIndex = 0
                if (DR_top_wjet0 < 0.8 and len(wJets) > 1) :
                        theWjetCand = wJets[1]
                        theWjetCandIndex = 1	
		
	 	i = 0
		minDRjet3 = 999.
                for jet in wJets:
                	deltaR_W_jet3 = sqrt( (theWjetCand.eta() - jet.eta())**2 + (theWjetCand.phi() - jet.phi())**2 )
                        if (deltaR_W_jet3 < minDRjet3 and deltaR_W_jet3 > 0.8) :
                        	minDRjet3 = deltaR_W_jet3
                                jet3 = i
                        i = i+1	


		pairMass = (wJets[jet3]+theWjetCand).mass()
		hasType2Top = theWjetCand.pt() > 200 and theWjetCand.mass() > 60 and theWjetCand.mass() < 100 and pairMass > 140 and pairMass < 250
		if hasType2Top:
			jet1P4_mod = copy.copy(topJets[0])
			myrand_QCDdist_jet1 = self.modMassHist.GetRandom()
			jet1P4_mod.SetM(myrand_QCDdist_jet1)
			ttMassMod = (wJets[jet3]+theWjetCand+jet1P4_mod).mass()
			self.mttMassPred[0] = ttMassMod
			self.mistagWtType12[0] = self.mistag_type12.GetBinContent( self.mistag_type12.FindBin(topJets[0].pt()) )
			self.mistagWt[0] = self.mistag.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
			self.mistagWt1B[0] = self.mistag1B.GetBinContent( self.mistag1B.FindBin(topJets[0].pt()) )
			self.mistagWtType121B[0] = self.mistag_type12_1B.GetBinContent( self.mistag1B.FindBin(topJets[0].pt()) )
			self.mistagWt2B[0] = self.mistag2B.GetBinContent( self.mistag2B.FindBin(topJets[0].pt()) )
			self.jet1Pt[0] = topJets[0].pt()
			self.jet1Eta[0] = topJets[0].eta()
			self.jet1Mass[0] = jet1P4_mod.M()
			self.jet2Pt[0] = theWjetCand.pt()
                        self.jet2Mass[0] = theWjetCand.M()
                        self.jet2Eta[0] = theWjetCand.eta()
                        self.jet2Phi[0] = theWjetCand.phi()
			self.jet2WMu[0] = wMu[1]
			self.jet2BDisc[0] = wBDisc[1]
			self.jet3Pt[0] = wJets[jet3].pt()
			self.jet3Eta[0] = wJets[jet3].eta()
			self.jet3Phi[0] = wJets[jet3].phi()
			self.jet3Mass[0] = wJets[jet3].M()
			self.jet3BDisc[0] = wBDisc[jet3] 
			self.DRWb[0] = sqrt( (wJets[jet3].eta() - theWjetCand.eta())**2 + (wJets[jet3].phi() - theWjetCand.phi())**2 )
			type2top = (theWjetCand + wJets[jet3])
			self.WbMass[0] = type2top.M()
			self.deltaPhi[0] = type2top.phi() - topJets[0].phi()
			self.deltaY[0] = type2top.Rapidity() - topJets[0].Rapidity()
			self.index[0] = 8
			self.treeVars.Fill()
			if wBDisc[jet3] > 0.679:
				self.index[0] = 9
				self.treeVars.Fill()
				if  max(jet1BDiscs) > 0.679:
					self.index[0] = 10
					self.jet1SubjetMaxBDisc[0] = max(jet1BDiscs)	







	if nTopCand < 2:
		return


	x = ROOT.gRandom.Uniform()

	#deltaPhi12 = abs(topJets[0].phi() - topJets[1].phi())
	#if deltaPhi12 < 2.0:
#		return

	jet1BDiscs = []
	jet2BDiscs = []
	i = 0
        while i < (topJetNSubjets[0]):
        	jet1BDiscs.append(topSubjetsBDisc[i])
        	i += 1
	i = 0
        while i < (topJetNSubjets[1]):
        	jet2BDiscs.append(topSubjetsBDisc[i+int(topJetNSubjets[0])])
        	i += 1

	bTag0 = max(jet1BDiscs) > 0.679
	bTag1 = max(jet2BDiscs) > 0.679
	
	bTagLoose0 = max(jet1BDiscs) > 0.244
	bTagLoose1 = max(jet2BDiscs) > 0.244

	tau32_jet1 = tau3Vals[0] / tau2Vals[0]
	tau32_jet2 = tau3Vals[1] / tau2Vals[1]		

	
	topTag0WP1 = topTag0 and tau32_jet1 < 0.7 and bTagLoose0
	topTag1WP1 = topTag1 and tau32_jet2 < 0.7 and bTagLoose1
	
	topTag0WP2 = topTag0 and tau32_jet1 < 0.6 and bTagLoose0
	topTag1WP2 = topTag1 and tau32_jet2 < 0.6 and bTagLoose1

	topTag0WP3 = topTag0 and tau32_jet1 < 0.55 and bTag0
	topTag1WP3 = topTag1 and tau32_jet2 < 0.55 and bTag1

	topTag0WP4 = topTag0 and tau32_jet1 < 0.5 and bTag0 and topJetMinMass[0] > 65
	topTag1WP4 = topTag1 and tau32_jet2 < 0.5 and bTag1 and topJetMinMass[1] > 65

	topTag0WP5 = topTag0 and tau32_jet1 < 0.4 and bTag0 and topJetMinMass[0] > 55
	topTag1WP5 = topTag1 and tau32_jet2 < 0.4 and bTag1 and topJetMinMass[1] > 55
	

	if x < 0.5:
		if topTag0:
			myrand_QCDdist_jet1 = self.modMassHist.GetRandom()
        		jet1P4_modmassQCDdist = copy.copy(topJets[1])
        		jet1P4_modmassQCDdist.SetM( myrand_QCDdist_jet1 )
         		ttMassJet0modmassQCDdist = (topJets[0]+jet1P4_modmassQCDdist).mass()
	 		self.mttMassPred[0] = ttMassJet0modmassQCDdist
			self.mistagWt[0] = self.mistag.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
			self.mistagWt1B[0] = self.mistag1B.GetBinContent( self.mistag1B.FindBin(topJets[1].pt()) )
			self.mistagWt2B[0] = self.mistag2B.GetBinContent( self.mistag2B.FindBin(topJets[1].pt()) )
			self.index[0] = 1
			self.jet1Pt[0] = topJets[0].pt()
			self.jet2Pt[0] = topJets[1].pt()
			self.jet1Parton[0] = topPartonFlavors[0]
			self.jet2Parton[0] = topPartonFlavors[1]
			self.jet1Eta[0] = topJets[0].eta()
			self.jet2Eta[0] = topJets[1].eta()
			self.jet1Mass[0] = topJets[0].M()
			self.jet2Mass[0] = myrand_QCDdist_jet1
			self.deltaPhi[0] = topJets[0].phi() - topJets[1].phi()
			self.deltaY[0] = topJets[0].Rapidity() - topJets[1].Rapidity()
			self.jet1SubjetMaxBDisc[0] = max(jet1BDiscs)
			self.jet2SubjetMaxBDisc[0] = max(jet2BDiscs)
			self.NNoutput[0] = self.reader.EvaluateMVA("MLP")
			self.NNoutputExp[0] = self.reader.EvaluateMVA("ExpertMLP")
			if (self.NNoutput[0] < 0.0):
				self.NNoutput[0] = 0.001
			if (self.NNoutput[0] > 1.0):
				self.NNoutput[0] = 0.999
			if (self.NNoutputExp[0] < 0.0):
				self.NNoutputExp[0] = 0.001
			if (self.NNoutputExp[0] > 1.0):
				self.NNoutputExp[0] = 0.999
			self.treeVars.Fill()
			if topTag0WP1:
				self.mistagWt[0] = self.mistagWP1.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
				self.index[0] = 1.1
				self.treeVars.Fill()
			if topTag0WP2:
				self.mistagWt[0] = self.mistagWP2.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
				self.index[0] = 1.2
				self.treeVars.Fill()
			if topTag0WP3:
				self.mistagWt[0] = self.mistagWP3.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
				self.index[0] = 1.3
				self.treeVars.Fill()
			if topTag0WP4:
				self.mistagWt[0] = self.mistagWP4.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
				self.index[0] = 1.4
				self.treeVars.Fill()
			if topTag0WP5:
				self.mistagWt[0] = self.mistagWP5.GetBinContent( self.mistag.FindBin(topJets[1].pt()) )
				self.index[0] = 1.5
				self.treeVars.Fill()
			if topTag1 and topTag0:
				self.index[0] = 2
				self.jet2Mass[0] = topJets[1].M()
				self.treeVars.Fill()
			if bTag0 and bTag1:
				self.index[0] = 3
				self.treeVars.Fill()
			




	if x >= 0.5:
		if topTag1:
			myrand_QCDdist_jet0 = self.modMassHist.GetRandom()
                        jet0P4_modmassQCDdist = copy.copy(topJets[0])
                        jet0P4_modmassQCDdist.SetM( myrand_QCDdist_jet0 )
                        ttMassJet1modmassQCDdist = (topJets[1]+jet0P4_modmassQCDdist).mass()
                        self.mttMassPred[0] = ttMassJet1modmassQCDdist
                        self.mistagWt[0] = self.mistag.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
                        self.mistagWt1B[0] = self.mistag1B.GetBinContent( self.mistag1B.FindBin(topJets[0].pt()) )
                        self.mistagWt2B[0] = self.mistag2B.GetBinContent( self.mistag2B.FindBin(topJets[0].pt()) )
                        self.index[0] = 1
			self.jet1Pt[0] = topJets[0].pt()
			self.jet2Pt[0] = topJets[1].pt()
			self.jet1Eta[0] = topJets[0].eta()
			self.jet2Eta[0] = topJets[1].eta()
			self.jet1Parton[0] = topPartonFlavors[0]
			self.jet2Parton[0] = topPartonFlavors[1]
			self.jet2Mass[0] = topJets[1].M()
			self.jet1Mass[0] = myrand_QCDdist_jet0
			self.deltaPhi[0] = topJets[0].phi() - topJets[1].phi()
			self.deltaY[0] = topJets[0].Rapidity() - topJets[1].Rapidity()
			self.jet1SubjetMaxBDisc[0] = max(jet1BDiscs)
			self.jet2SubjetMaxBDisc[0] = max(jet2BDiscs)
			self.NNoutput[0] = self.reader.EvaluateMVA("MLP")
			self.NNoutputExp[0] = self.reader.EvaluateMVA("ExpertMLP")
			if (self.NNoutput[0] < 0.0):
				self.NNoutput[0] = 0.001
			if (self.NNoutput[0] > 1.0):
				self.NNoutput[0] = 0.999
			if (self.NNoutputExp[0] < 0.0):
				self.NNoutputExp[0] = 0.001
			if (self.NNoutputExp[0] > 1.0):
				self.NNoutputExp[0] = 0.999
                        self.treeVars.Fill()
                        
			if topTag1WP1:
				self.mistagWt[0] = self.mistagWP1.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
				self.index[0] = 1.1
				self.treeVars.Fill()
			if topTag1WP2:
				self.mistagWt[0] = self.mistagWP2.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
				self.index[0] = 1.2
				self.treeVars.Fill()
			if topTag1WP3:
				self.mistagWt[0] = self.mistagWP3.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
				self.index[0] = 1.3
				self.treeVars.Fill()
			if topTag1WP4:
				self.mistagWt[0] = self.mistagWP4.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
				self.index[0] = 1.4
				self.treeVars.Fill()
			if topTag1WP5:
				self.mistagWt[0] = self.mistagWP5.GetBinContent( self.mistag.FindBin(topJets[0].pt()) )
				self.index[0] = 1.5
				self.treeVars.Fill()	
			if topTag0 and topTag1:
				self.index[0] = 2
				self.jet1Mass[0] = topJets[0].M()
				self.treeVars.Fill()
			if bTag0 and bTag1:
				self.index[0] = 3
				self.treeVars.Fill()

			
 
