import ROOT
import copy
from math import *
from operator import itemgetter, attrgetter

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()


from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *

class New2012Analyzer :
    """Run 2012 Analysis"""
    def __init__(self, useMC, outfile, mistagFile, collectionLabelSuffix, useGenWeight, triggerFile1, triggerFile2, modMassFile, pdfWeight="nominal", triggerWeight="noWeight"):
    	self.outfile = outfile
    	self.mistagFileStr = mistagFile
	self.triggerFile1Str = triggerFile1
	self.triggerFile2Str = triggerFile2
	self.modMassFileStr = modMassFile
	self.useMC = useMC
	self.useGenWeight = useGenWeight
	self.pdfWeight = pdfWeight
	self.triggerWeight = triggerWeight

	label = 'ttbsmAna'+collectionLabelSuffix

	print label

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

	self.hemis0Handle   = Handle ("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >")
        self.hemis0Label    = ( label, "topTagP4Hemis0" )

        self.hemis0MinMassHandle     = Handle( "std::vector<double>" )
        self.hemis0MinMassLabel  = ( label, "topTagMinMassHemis0" )

        self.hemis0NSubjetsHandle    = Handle( "std::vector<double>" )
        self.hemis0NSubjetsLabel = ( label, "topTagNSubjetsHemis0"  )

        self.hemis0TopMassHandle     = Handle( "std::vector<double>" )
        self.hemis0TopMassLabel  = ( label, "topTagTopMassHemis0" )

        self.hemis0PassHandle        = Handle( "std::vector<int>")
        self.hemis0PassLabel   = ( label, "topTagPassHemis0" )

        self.hemis1Handle     = Handle( "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >")
        self.hemis1Label      = ( label, "wTagP4Hemis1" )

	self.hemis1MuHandle           = Handle( "std::vector<double>")
        self.hemis1MuLabel      = ( label,  "wTagMuHemis1" )

        self.hemis1Jet3Handle    = Handle("int")
        self.hemis1Jet3Label     = ( label, "jet3Hemis1" )

        self.weightsHandle = Handle( "double" )
        self.weightsLabel = ( label, "weight" )
        self.pdfHandle = Handle("std::vector<double>")
        self.pdfLabel = ( label, "pdfWeights")
       
	self.npvHandle = Handle( "unsigned int" )
	self.npvLabel = (label, "npv")

 
        self.__book__()

    def __del__ (self):
	"""Internal Destructor"""
	print 'Goodbye from 2012 Analyzer.'
	
	self.f.cd()
	
	self.mttPredDistType11.GetPredictedHist().Write()
	self.mttPredDistType11.GetObservedHist().Write()
	self.mttPredDistType11.GetTaggableHist().Write()
	self.mttPredDistType12.GetPredictedHist().Write()
	self.mttPredDistType12.GetObservedHist().Write()
	self.mttPredDistType12.GetTaggableHist().Write()
	
	for pair in sorted(self.runPairs, key=itemgetter(3)) :
		print '{0:12.2f} : Run {1:15.0f}, LumiBlock {2:15.0f}, Event {3:30.0f}'.format(
			pair[3], pair[0], pair[2], pair[1]
			)

	self.f.Write()
	self.f.Close()
	
	self.mistagFile.Close()
	self.triggerFile1.Close()
	self.triggerFile2.Close()
	self.modMassFile.Close()


    def __book__(self):
	"""Books Histograms"""

	self.mistagFile = ROOT.TFile(self.mistagFileStr + ".root")
	self.mistagFile.cd()
	self.mistag = self.mistagFile.Get("MISTAG_MU_REVERSE_SUB_TTBAR").Clone()
	self.mistag.SetName('mistag')
	
	ROOT.SetOwnership(self.mistag, False)

	self.triggerFile1 = ROOT.TFile(self.triggerFile1Str + ".root")
        self.triggerFile1.cd()
        self.triggerHist1 = self.triggerFile1.Get("TRIGGER_EFF").Clone()
        self.triggerHist1.SetName('triggerHist1')
        ROOT.SetOwnership( self.triggerHist1, False )
	self.triggerFile2 = ROOT.TFile(self.triggerFile2Str + ".root")
        self.triggerFile2.cd()
        self.triggerHist2 = self.triggerFile2.Get("TRIGGER_EFF").Clone()
        self.triggerHist2.SetName('triggerHist2')
        ROOT.SetOwnership( self.triggerHist2, False )


        self.modMassFile = ROOT.TFile(self.modMassFileStr + ".root")
        print self.modMassFileStr
	self.modMassFile.cd()
        self.modMassHist = ROOT.TH1F()
        self.modMassHist = self.modMassFile.Get("jetMassOneTag_MassWindow").Clone()
        self.modMassHist.SetName('modMassHist')
        ROOT.SetOwnership( self.modMassHist, False )
       
        self.f = ROOT.TFile( self.outfile + ".root", "recreate" )
        self.f.cd()

	self.mttPredDistType11 = ROOT.PredictedDistribution( self.mistag, "mttPredDistType11", "mTT Mass", 1000, 0, 5000)
	self.mttPredDistType12 = ROOT.PredictedDistribution( self.mistag, "mttPredDistType12", "mTT Mass", 1000, 0, 5000)
	self.ptPredDistType11 = ROOT.PredictedDistribution( self.mistag, "ptPredDistType11", "pT", 200,0,2000)
	self.ptPredDistType12 = ROOT.PredictedDistribution( self.mistag, "ptPredDistType12", "pT", 200,0,2000)
	

	ROOT.SetOwnership(self.mttPredDistType11, False)
	ROOT.SetOwnership(self.mttPredDistType12, False)

        self.mttMassType11 = ROOT.TH1D("mttMassType11", "mTT Mass", 1000, 0,  5000 )
        self.mttMassType12 = ROOT.TH1D("mttMassType12", "mTT Mass", 1000, 0,  5000 )

	self.ptType11 = ROOT.TH1D("ptType11", "pT", 200, 0, 2000)
	self.ptType12 = ROOT.TH1D("ptType12", "pT", 200, 0, 2000)

	self.muHist = ROOT.TH1D("mu", "mu", 200, -1,1)

	
	self.npvHist = ROOT.TH1F("npvHist", ";Number of Primary Vertices", 100, 0, 100)




	self.runPairs = []
	

	
    def analyze(self, event):
	"""Analyze Event"""

	event.getByLabel(self.allTopTagLabel, self.allTopTagHandle)
	topJets = self.allTopTagHandle.product()
	event.getByLabel (self.allTopTagMinMassLabel, self.allTopTagMinMassHandle)
        topJetMinMass= self.allTopTagMinMassHandle.product()
        event.getByLabel (self.allTopTagNSubjetsLabel, self.allTopTagNSubjetsHandle)
        topJetNSubjets= self.allTopTagNSubjetsHandle.product()
        event.getByLabel (self.allTopTagTopMassLabel, self.allTopTagTopMassHandle)
        topJetMass= self.allTopTagTopMassHandle.product()
        event.getByLabel (self.allTopTagPassLabel, self.allTopTagPassHandle )
        topJetPass= self.allTopTagPassHandle.product()
	event.getByLabel (self.npvLabel, self.npvHandle)
	vertices = self.npvHandle.product()

	npv = vertices[0]

	upperWindowEdge = 250 #Upper edge of mass cut for top tags
	jetPtCut = 400 #Cut on Type1 Jet Pt
		
	passType11Selection = 0


	

	if topJets.size() >= 2 and topJetMinMass.size() >= 2:

		if topJets[0].pt() > jetPtCut and topJets[1].pt() > jetPtCut :
			
			weight = 1.0
			if self.useMC :		
				trigEffLead = self.triggerHist1.GetBinContent(self.triggerHist1.FindBin(topJets[0].pt()))
				trigEffSec = self.triggerHist1.GetBinContent(self.triggerHist1.FindBin(topJets[1].pt()))

				triggerEffType11 = trigEffLead + trigEffSec - (trigEffLead*trigEffSec)
				weight = triggerEffType11
	
			deltaPhi = topJets[0].phi() - topJets[1].phi()
			if deltaPhi > ROOT.TMath.Pi() :
				deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
			if deltaPhi < -ROOT.TMath.Pi() :
				deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()

			deltaPhiCut = abs(deltaPhi) > 2.1
			etaCut = abs(topJets[0].Rapidity()) < 2.4 and abs(topJets[1].Rapidity()) < 2.4


			pass1 = topJets[0].mass() > 140 and topJets[0].mass() < upperWindowEdge and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
			pass2 = topJets[1].mass() > 140 and topJets[1].mass() < upperWindowEdge and topJetMinMass[1] > 50 and topJetNSubjets[1] > 2

			ttMassOrig = (topJets[0] + topJets[1]).mass()

			ttMassMod1 = ttMassOrig
			ttMassMod2 = ttMassOrig
	
			if deltaPhiCut and etaCut and pass1 and pass2 :
				passType11Selection = 1

				randomMass1 = self.modMassHist.GetRandom()
				jet0Mod = copy.copy(topJets[0])
				jet0Mod.SetM(randomMass1)
				ttMassMod1 = (jet0Mod + topJets[1]).mass()
				
				randomMass2 = self.modMassHist.GetRandom()
				jet1Mod = copy.copy(topJets[1])
				jet1Mod.SetM(randomMass2)
				ttMassMod2 = (topJets[0] + jet1Mod).mass()
	

	
			if passType11Selection :
				self.mttMassType11.Fill(ttMassOrig, weight)
				self.npvHist.Fill(npv)
				self.ptType11.Fill(topJets[0].pt(), weight)
				self.ptType11.Fill(topJets[1].pt(), weight)

			if deltaPhiCut and etaCut :
				xrand = ROOT.gRandom.Uniform()
				if xrand < 0.5 :
					if pass1 :
						self.mttPredDistType11.Accumulate( ttMassMod2, topJets[1].pt(), pass2, 1.0)
						self.ptPredDistType11.Accumulate( topJets[0].pt(), topJets[0].pt(), pass2, 1.0)
						self.ptPredDistType11.Accumulate( topJets[1].pt(), topJets[1].pt(), pass2, 1.0)
				if xrand >= 0.5 :
					if pass2 :
						self.mttPredDistType11.Accumulate( ttMassMod1, topJets[0].pt(), pass1, 1.0 )
						self.ptPredDistType11.Accumulate( topJets[0].pt(), topJets[0].pt(), pass2, 1.0)
						self.ptPredDistType11.Accumulate( topJets[1].pt(), topJets[1].pt(), pass2, 1.0)




	if not passType11Selection :

		event.getByLabel(self.hemis0Label, self.hemis0Handle)
		topJets = self.hemis0Handle.product()

		event.getByLabel(self.hemis1Label, self.hemis1Handle)
		wJets = self.hemis1Handle.product()

		event.getByLabel (self.hemis0MinMassLabel, self.hemis0MinMassHandle)
        	topJetMinMass   = self.hemis0MinMassHandle.product()
        	event.getByLabel (self.hemis0NSubjetsLabel, self.hemis0NSubjetsHandle)
       		topJetNSubjets  = self.hemis0NSubjetsHandle.product()
       		event.getByLabel (self.hemis0TopMassLabel, self.hemis0TopMassHandle)
       	 	topJetMass      = self.hemis0TopMassHandle.product()
      	  	event.getByLabel (self.hemis1MuLabel, self.hemis1MuHandle)
		wJetMu          = self.hemis1MuHandle.product()
	        event.getByLabel (self.hemis1Jet3Label, self.hemis1Jet3Handle )
        	jet3           = (self.hemis1Jet3Handle.product())[0]
			
		passType12Selection = 0
		if topJets.size() > 0 and wJets.size() >= 2 :
		
			if topJets[0].pt() > 400 and wJets[0].pt() > 200 and wJets[jet3].pt() > 30 :
						
				weight = 1.0
				if self.useMC :		
					trigEffLead = self.triggerHist2.GetBinContent(self.triggerHist2.FindBin(topJets[0].pt()))
					weight = trigEffLead
	
				pairMass = (wJets[0] + wJets[jet3]).mass()

				hasTopTag = topJetMass[0] > 140 and topJetMass[0] < upperWindowEdge and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2
				hasType2Top = wJets[0].mass() > 60 and wJets[0].mass() < 130 and pairMass > 140 and pairMass < 250 and wJetMu[0] < 0.4 
					
				toptag1 = topJetMass[0] > 140 and topJetMass[0] < upperWindowEdge and topJetMinMass[0] > 50 and topJetNSubjets[0] > 2




				ttMassOrig = (topJets[0] + wJets[0] + wJets[jet3]).mass()

				if hasTopTag and hasType2Top :
					passType12Selection = 1			
					self.mttMassType12.Fill(ttMassOrig, weight)
					self.ptType12.Fill(topJets[0].pt(), weight)
					



				if hasType2Top :
					
					randomMass = self.modMassHist.GetRandom()
                                	jet0Mod = copy.copy(topJets[0])
                                	jet0Mod.SetM(randomMass)
                                	ttMassMod = (jet0Mod + wJets[0] + wJets[jet3]).mass()	
					self.muHist.Fill(wJetMu[0], weight)					
					self.mttPredDistType12.Accumulate( ttMassMod, topJets[0].pt(), toptag1, 1.0)
					self.ptPredDistType12.Accumulate(topJets[0].pt(), topJets[0].pt(), toptag1, 1.0)

				





