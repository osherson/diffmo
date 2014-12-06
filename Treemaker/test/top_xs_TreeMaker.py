import ROOT
from DataFormats.FWLite import Events, Handle
from array import array
import math
from math import *
import sys

from Analysis.Tools.JetTools import *

class tree_maker:
	def __init__(self, outputname, seed, mistagFileStr, modMassFileStr, triggerFileStr):
		# load all the event info:
		self.numbtag = 0
		self.name = outputname
		self.seed = seed
		self.mistagFileStr = mistagFileStr
		self.modMassFileStr = modMassFileStr
		self.triggerFileStr = triggerFileStr

		#General Quantities
		#N Primary Vertices
		self.npvHandle = Handle( "unsigned int" )
		self.npvLabel  = ( "diffmogen", "npv" )

		#MET Pt and Phi - why not eta?
		self.metPtHandle = Handle ( "double" )
		self.metPtLabel  = ( "diffmogen", "metpt" )
		self.metPhiHandle = Handle ( "double" )
		self.metPhiLabel  = ( "diffmogen", "metphi" )
		
		#Pruned Jet Collection
		#We need this for CSV values and subjet CSV values
		self.prunedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > " )
		self.prunedLabel  = ( "diffmoca8pp", "PrunedCA8CORR" )

		#Btagging CSV values
		self.CSVHandle = Handle( "std::vector<double>" )
		self.CSVLabel  = ( "diffmoca8pp", "PrunedCA8csv" )

		#Subjet btagging CSV values
		self.subjet1CSVHandle = Handle( "std::vector<double>" )
		self.subjet1CSVLabel  = ( "diffmoca8pp", "PrunedCA8sub0csv" )
		self.subjet2CSVHandle = Handle( "std::vector<double>" )
		self.subjet2CSVLabel  = ( "diffmoca8pp", "PrunedCA8sub1csv" )
		self.subjet3CSVHandle = Handle( "std::vector<double>" )
		self.subjet3CSVLabel  = ( "diffmoca8pp", "PrunedCA8sub2csv" )
		self.subjet4CSVHandle = Handle( "std::vector<double>" )
		self.subjet4CSVLabel  = ( "diffmoca8pp", "PrunedCA8sub3csv" )

		#TopTagged Jet collection
		#Eventually this is the one we will use most likely
		self.topTaggedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > " )
		self.topTaggedLabel  = ( "diffmoca8tt", "TopTaggedPrunedCA8CORR")
		
		#Number of Subjets
		self.NsubjetsHandle = Handle( "std::vector<unsigned int>" )
		self.NsubjetsLabel  = ( "diffmoca8tt", "TopTaggedPrunedCA8nsub" )

		#Min-pairwise Mass
		self.minMassHandle = Handle ( "std::vector<double>")
		self.minMassLabel  = ( "diffmoca8tt", "TopTaggedPrunedCA8topTagMinMass")

		#Top Mass
		self.topTagTopMassHandle = Handle ( "std::vector<double>")
		self.topTagTopMassLabel  = ( "diffmoca8tt", "TopTaggedPrunedCA8topTagTopMass")

		#Unpruned Jet collection
		#We need this to get N-subjettiness
		self.unprunedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > ")
		self.unprunedLabel  = ( "diffmoca8", "UnprunedCA8CORR" )

		#N-Subjettiness(tau)
		self.t1Handle = Handle( "std::vector<double>" )
		self.t1Label  = ("diffmoca8", "UnprunedCA8tau1")
		self.t2Handle = Handle( "std::vector<double>" )
		self.t2Label  = ("diffmoca8", "UnprunedCA8tau2")
		self.t3Handle = Handle( "std::vector<double>" )
		self.t3Label  = ("diffmoca8", "UnprunedCA8tau3")
		self.t4Handle = Handle( "std::vector<double>" )
		self.t4Label  = ("diffmoca8", "UnprunedCA8tau4")
		
		#Number of Daughters
		self.NdaughtersHandle = Handle( "std::vector<unsigned int>" )
		self.NdaughtersLabel  = ("diffmoca8", "UnprunedCA8nsub" )

		#Trigger
		self.triggerHandle = Handle( "edm::TriggerResults" )
		self.triggerLabel  = ( "TriggerResults" )

		self.__book__()

	def __book__(self):

		if (self.mistagFileStr == ''):
			self.doMistag = False
		else:
			self.mistagFile = ROOT.TFile(self.mistagFileStr + ".root")
			self.mistagFile.cd()
			self.mistag = ROOT.TH1F()
			self.mistag = self.mistagFile.Get("MISTAG_MPM_REVERSE_SUB_TTBAR").Clone()
			self.mistag.SetName('mistag')
			ROOT.SetOwnership( self.mistag, False )
			self.doMistag = True
		
		if (self.modMassFileStr == ''):
			self.doModMass = False
		else:
			self.modMassFile = ROOT.TFile(self.modMassFileStr + ".root")
			self.modMassFile.cd()
			self.modMass = ROOT.TH1F()
			self.modMass = self.modMassFile.Get("jetMassOneTag_MassWindow").Clone()
			self.modMass.SetName('modMass')
			ROOT.SetOwnership( self.modMass, False )
			self.doModMass = True
		
		if (self.triggerFileStr == ''):
			self.doTrigger = False
		else:
			self.triggerFile = ROOT.TFile(self.triggerFileStr + ".root")
			self.triggerFile.cd()
			self.trigger = self.triggerFile.Get("TRIGGER_EFF").Clone()
			self.trigger.SetName('trigger')
			ROOT.SetOwnership( self.trigger, False )
			self.doTrigger = True

		print "Booking Histograms and Trees..."
		self.f = ROOT.TFile( self.name + ".root", "recreate" )
		self.f.cd()
		self.treeVars = ROOT.TTree('treeVars', 'treeVars')

		self.run = array('i', [-1])
		self.event = array('l', [-1])
		self.lumi = array('i', [-1])
		
		self.npv = array('i', [-1])
		self.index = array('i', [-1])
		self.trigWt = array('f', [-1.0])
		self.MET = array('f', [-1.0])

		self.jet1pt = array('f', [-1.0])
		self.jet2pt = array('f', [-1.0])
		self.jet1eta = array('f', [-10.0])
		self.jet2eta = array('f', [-10.0])
		self.jet1phi = array('f', [-10.0])
		self.jet2phi = array('f', [-10.0])
		self.jet1mass = array('f', [-1.0])
		self.jet2mass = array('f', [-1.0])
		self.jet1topMass = array('f', [-1.0])
		self.jet2topMass = array('f', [-1.0])
		self.jet1topMassRaw = array('f', [-1.0])
		self.jet2topMassRaw = array('f', [-1.0])
		self.jet1csv = array('f', [-1.0])
		self.jet2csv = array('f', [-1.0])
		self.jet1maxSubjetCSV = array('f', [-1.0])
		self.jet2maxSubjetCSV = array('f', [-1.0])
		self.jet1tau32 = array('f', [-1.0])
		self.jet2tau32 = array('f', [-1.0])
		self.jet1tau31 = array('f', [-1.0])
		self.jet2tau31 = array('f', [-1.0])
		self.jet1tau21 = array('f', [-1.0])
		self.jet2tau21 = array('f', [-1.0])
		self.jet1nSubj = array('i', [-1])
		self.jet2nSubj = array('i', [-1])
		self.jet1minMass = array('f', [-1.0])
		self.jet2minMass = array('f', [-1.0])

		self.jet1nDaughters = array('i', [-1])
		self.jet2nDaughters = array('i', [-1])
		
		self.deltaY = array('f', [-10.0])
		self.deltaPhi = array('f', [-10.0])
		self.cutflow = array('f', [-1.0])

		self.mistagWt = array('f', [-1.0])
		self.mistagWt1B = array('f', [-1.0])
		self.mistagWt2B = array('f', [-1.0])

		self.htSum = array('f', [-1.0])
		self.triggerEff = array('f', [1.0])

		self.treeVars.Branch('run', self.run, 'run/I')
		self.treeVars.Branch('event', self.event, 'event/L')
		self.treeVars.Branch('lumi', self.lumi, 'lumi/I')

		self.treeVars.Branch('npv', self.npv, 'npv/I')
		self.treeVars.Branch('index', self.index, 'index/I')
		self.treeVars.Branch('trigWt', self.trigWt, 'trigWt/F')
		self.treeVars.Branch('MET', self.MET, 'MET/F')

		self.treeVars.Branch('jet1pt', self.jet1pt, 'jet1pt/F')
		self.treeVars.Branch('jet2pt', self.jet2pt, 'jet2pt/F')
		self.treeVars.Branch('jet1eta', self.jet1eta, 'jet1eta/F')
		self.treeVars.Branch('jet2eta', self.jet2eta, 'jet2eta/F')
		self.treeVars.Branch('jet1phi', self.jet1phi, 'jet1phi/F')
		self.treeVars.Branch('jet2phi', self.jet2phi, 'jet2phi/F')
		self.treeVars.Branch('jet1mass', self.jet1mass, 'jet1mass/F')
		self.treeVars.Branch('jet2mass', self.jet2mass, 'jet2mass/F')
		self.treeVars.Branch('jet1topMass', self.jet1topMass, 'jet1topMass/F')
		self.treeVars.Branch('jet2topMass', self.jet2topMass, 'jet2topMass/F')
		self.treeVars.Branch('jet1topMassRaw', self.jet1topMassRaw, 'jet1topMassRaw/F')
		self.treeVars.Branch('jet2topMassRaw', self.jet2topMassRaw, 'jet2topMassRaw/F')
		self.treeVars.Branch('jet1csv', self.jet1csv, 'jet1csv/F')
		self.treeVars.Branch('jet2csv', self.jet2csv, 'jet2csv/F')
		self.treeVars.Branch('jet1maxSubjetCSV', self.jet1maxSubjetCSV, 'jet1maxSubjetCSV/F')
		self.treeVars.Branch('jet2maxSubjetCSV', self.jet2maxSubjetCSV, 'jet2maxSubjetCSV/F')
		self.treeVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
		self.treeVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
		self.treeVars.Branch('jet1tau31', self.jet1tau31, 'jet1tau31/F')
		self.treeVars.Branch('jet2tau31', self.jet2tau31, 'jet2tau31/F')
		self.treeVars.Branch('jet1tau21', self.jet1tau21, 'jet1tau21/F')
		self.treeVars.Branch('jet2tau21', self.jet2tau21, 'jet2tau21/F')
		self.treeVars.Branch('jet1nSubj', self.jet1nSubj, 'jet1nSubj/I')
		self.treeVars.Branch('jet2nSubj', self.jet2nSubj, 'jet2nSubj/I')
		self.treeVars.Branch('jet1minMass', self.jet1minMass, 'jet1minMass/F')
		self.treeVars.Branch('jet2minMass', self.jet2minMass, 'jet2minMass/F')

		self.treeVars.Branch('jet1nDaughters', self.jet1nDaughters, 'jet1nDaughters/I')
		self.treeVars.Branch('jet2nDaughters', self.jet2nDaughters, 'jet2nDaughters/I')

		self.treeVars.Branch('deltaY', self.deltaY, 'deltaY/F')
		self.treeVars.Branch('deltaPhi', self.deltaPhi, 'deltaPhi/F')
		self.treeVars.Branch('cutflow', self.cutflow, 'cutflow/F')

		self.treeVars.Branch('mistagWt', self.mistagWt, 'mistagWt/F')
		self.treeVars.Branch('mistagWt1B', self.mistagWt1B, 'mistagWt1B/F')
		self.treeVars.Branch('mistagWt2B', self.mistagWt2B, 'mistagWt2B/F')

		self.treeVars.Branch('htSum', self.htSum, 'htSum/F')
		self.treeVars.Branch('triggerEff', self.triggerEff, 'triggerEff/F')

		self.invarmass = array('f', [-1.0])
		self.jetangle = array('f', [-10.0])
		self.treeVars.Branch('invariant_mass', self.invarmass, 'invarmass/F')
		self.treeVars.Branch('angle_between_jets', self.jetangle, 'jetangle/F')

	def analyze(self, event):
		self.run[0] = event.object().id().run()
		self.event[0] = event.object().id().event()
		self.lumi[0] = event.object().id().luminosityBlock()

		event.getByLabel (self.npvLabel, self.npvHandle)
		event.getByLabel (self.metPtLabel, self.metPtHandle)
		event.getByLabel (self.metPhiLabel, self.metPhiHandle)

		event.getByLabel (self.prunedLabel, self.prunedHandle)
		event.getByLabel (self.unprunedLabel, self.unprunedHandle)
		event.getByLabel (self.topTaggedLabel, self.topTaggedHandle)

		event.getByLabel (self.CSVLabel, self.CSVHandle)

		event.getByLabel (self.subjet1CSVLabel, self.subjet1CSVHandle)
		event.getByLabel (self.subjet2CSVLabel, self.subjet2CSVHandle)
		event.getByLabel (self.subjet3CSVLabel, self.subjet3CSVHandle)
		event.getByLabel (self.subjet4CSVLabel, self.subjet4CSVHandle)

		event.getByLabel (self.t1Label, self.t1Handle)
		event.getByLabel (self.t2Label, self.t2Handle)
		event.getByLabel (self.t3Label, self.t3Handle)
		event.getByLabel (self.t4Label, self.t4Handle)

		event.getByLabel (self.NdaughtersLabel, self.NdaughtersHandle)
		event.getByLabel (self.NsubjetsLabel, self.NsubjetsHandle)

		event.getByLabel (self.minMassLabel, self.minMassHandle)
		event.getByLabel (self.topTagTopMassLabel, self.topTagTopMassHandle)

		npv = self.npvHandle.product()[0]
		metPt = self.metPtHandle.product()[0]
		metPhi = self.metPhiHandle.product()[0]

		CSVVals = self.CSVHandle.product()

		subjet1CSV = self.subjet1CSVHandle.product()
		subjet2CSV = self.subjet2CSVHandle.product()
		subjet3CSV = self.subjet3CSVHandle.product()
		subjet4CSV = self.subjet4CSVHandle.product()

		Tau1  =  self.t1Handle.product()
		Tau2  =  self.t2Handle.product()
		Tau3  =  self.t3Handle.product()
		Tau4  =  self.t4Handle.product()

		nSubjets = self.NsubjetsHandle.product()
		nDaughters = self.NdaughtersHandle.product()

		minMass = self.minMassHandle.product()
		topTagTopMass = self.topTagTopMassHandle.product()

		jet1topTagged = 0
		jet2topTagged = 0
		HTsum = 0

		if (self.prunedHandle.isValid() and self.unprunedHandle.isValid() and self.topTaggedHandle.isValid()) :
			unpj = self.unprunedHandle.product()
			pj = self.prunedHandle.product()
			ttpj = self.topTaggedHandle.product()
			
			#Total number of events seen:
			self.cutflow[0] = 0.0
			self.treeVars.Fill()

			## approximately 3% of the time we have more pj than unpj. Why? Shouldn't this be impossible?
			## The pruning can separate 2 subjets into standalone jets so this makes sense
			if len(unpj)<len(pj):
				self.numbtag=self.numbtag+1

			#One jet, including top tagged:
			if len(pj) < 1 or len(unpj) < 1 or len(ttpj) < 1:
				self.cutflow[0] = 0.1
				self.treeVars.Fill()
				if len(pj) < 1:
					self.cutflow[0] = 0.11
					self.treeVars.Fill()
				if len(unpj) < 1:
					self.cutflow[0] = 0.12
					self.treeVars.Fill()
				if len(ttpj) < 1:
					self.cutflow[0] = 0.13
					self.treeVars.Fill()
				self.reset()
				return

			#Make sure there's at least a top condidiate by checking the pT
			nTopCand = 0
			for i in range(0,len(ttpj) ) :
				if( ttpj[i].pt() > 400 ) :
					nTopCand = nTopCand + 1
					HTsum += ttpj[i].pt()
				if nTopCand < 1 :
					self.reset()
					return
			
			self.htSum[0] = HTsum
			self.MET[0] = metPt
			self.npv[0] = npv

			#Number of events with >=1 i top jet with pt >400
			self.cutflow[0] = 1.0
			self.treeVars.Fill()

			#Reorder to make sure the highest pT is first. This is after any JEC. Is this correct?
			#pt_sorted_jets = ttpj
			pt_sorted_jets = ReorderByPt(ttpj)

			#We need these for the substructure
			ca1 = ROOT.TLorentzVector()

			#We create the CA jet to match and fill the first jet's parameters			
			ca1.SetPtEtaPhiM(pt_sorted_jets[0].Pt(), pt_sorted_jets[0].Eta(), pt_sorted_jets[0].Phi(), pt_sorted_jets[0].M())
			self.jet1pt[0] = ca1.Pt()
			self.jet1eta[0] = ca1.Eta()
			self.jet1phi[0] = ca1.Phi()
			self.jet1mass[0] = ca1.M()

			#Match unpruned jets with pruned - so we have both subjet btagging and nsubjettiness
			#This returns the index of the jet in the first collection that matches within dr = 0.4 to the jet of the second argument
			jet1matchIndex = MatchCol(unpj, ca1)
			#This is temporary so we get the correct pj index for CSV and subjet CSV
			jet1matchIndex_pj = MatchCol(pj, ca1)
			#Again temporary to match toptagged for Nsubjets, minMass, and topMass
			jet1matchIndex_tt = MatchCol(ttpj, ca1)

			#Make sure matching is correct for jet1
			if jet1matchIndex==-1 or jet1matchIndex_pj==-1 or jet1matchIndex_tt ==-1:
				self.cutflow[0] = 1.1
				self.treeVars.Fill()
				if jet1matchIndex==-1:
					self.cutflow[0] = 1.11
					self.treeVars.Fill()
				if jet1matchIndex_pj==-1:
					self.cutflow[0] = 1.12
					self.treeVars.Fill()
				if jet1matchIndex_tt==-1:
					self.cutflow[0] = 1.13
					self.treeVars.Fill()
				self.reset()
				return

			#Make sure the subjets are filled
			if len(nSubjets)==0:
				self.cutflow[0] = 1.2
				self.treeVars.Fill()
				self.reset()
				return

			#Top Tagging Parameters
			jet1minMassVal = minMass[jet1matchIndex_tt]
			self.jet1nSubj[0] = nSubjets[jet1matchIndex_tt]
			self.jet1minMass[0] = jet1minMassVal
			#### This should rightfully be jet1mass and jet2mass ####
			self.jet1topMass[0] = topTagTopMass[jet1matchIndex_tt]
			self.jet1topMassRaw[0] = topTagTopMass[jet1matchIndex_tt]
	
			#Check if jet 1 is top taggged
			if self.jet1topMass[0] > 140.0 and self.jet1topMass[0] < 250.0 and self.jet1minMass[0] > 50.0 and self.jet1nSubj[0] > 2:
				#First jet is fully Top-Tagged
				self.cutflow[0] = 2.0
				self.treeVars.Fill()
				jet1topTagged = 1
			else:
				self.cutflow[0] = 1.3
				self.treeVars.Fill()
				# self.reset()					#We don't want to return so that we can fill the mistag contribution
				# return
		
			#Do the same as above for a second jet now
			if len(pj) < 2 or len(unpj) < 2 or len(ttpj) < 2:
				if jet1topTagged:
					self.cutflow[0] = 2.1
					self.treeVars.Fill()
					if len(pj) < 2:
						self.cutflow[0] = 2.11
						self.treeVars.Fill()
					if len(unpj) < 2:
						self.cutflow[0] = 2.12
						self.treeVars.Fill()
					if len(ttpj) < 2:
						self.cutflow[0] = 2.13
						self.treeVars.Fill()
				self.cutflow[0] = 2.2
				self.treeVars.Fill()
				self.reset()
				return

			#Jet 2 Basic Parameters
			ca2 = ROOT.TLorentzVector()
			ca2.SetPtEtaPhiM(pt_sorted_jets[1].Pt(), pt_sorted_jets[1].Eta(), pt_sorted_jets[1].Phi(), pt_sorted_jets[1].M())
			self.jet2pt[0] = ca2.Pt()
			self.jet2eta[0] = ca2.Eta()
			self.jet2phi[0] = ca2.Phi()
			self.jet2mass[0] = ca2.M()
			
			#Jet 2 Matching
			jet2matchIndex_tt = MatchCol(ttpj, ca2)
			jet2matchIndex_pj = MatchCol(pj, ca2)
			jet2matchIndex = MatchCol(unpj, ca2)

			#Make sure matching is correct for jet2
			if jet2matchIndex==-1 or jet2matchIndex_pj==-1 or jet2matchIndex_tt ==-1:
				if jet1topTagged:
					self.cutflow[0] = 2.3
					self.treeVars.Fill()
					if jet2matchIndex==-1:
						self.cutflow[0] = 2.31
						self.treeVars.Fill()
					if jet2matchIndex_pj==-1:
						self.cutflow[0] = 2.32
						self.treeVars.Fill()
					if jet2matchIndex_tt==-1:
						self.cutflow[0] = 2.33
						self.treeVars.Fill()
				self.cutflow[0] = 2.4
				self.treeVars.Fill()
				self.reset()
				return

			#2nd jet with pT > 400:
			jet2ptcheck = 0
			if self.jet2pt[0] > 400.:
				jet2ptcheck = 1
				if jet1topTagged:
					self.cutflow[0] = 3.0
					self.treeVars.Fill()
				#Temporary
				else:
					self.cutflow[0] = 9.1
					self.treeVars.Fill()
			else:
				if jet1topTagged:
					self.cutflow[0] = 2.5
					self.treeVars.Fill()
				#Temporary
				else:
					self.cutflow[0] = 9.2
					self.treeVars.Fill()
				# self.reset()
				# return

			#Jet 2 top-tagging parameters
			jet2minMassVal = minMass[jet2matchIndex_tt]
			self.jet2nSubj[0] = nSubjets[jet2matchIndex_tt]
			self.jet2minMass[0] = jet2minMassVal
			self.jet2topMass[0] = topTagTopMass[jet2matchIndex_tt]
			self.jet2topMassRaw[0] = topTagTopMass[jet2matchIndex_tt]

			##################### We don't really need this but here it is ###################################
			if len(nDaughters)>0:
				self.jet1nDaughters[0] = nDaughters[jet1matchIndex]
				self.jet2nDaughters[0] = nDaughters[jet2matchIndex]

			#Invarient Mass			
			self.invarmass[0] = (ca1+ca2).M()

			#Angular Parameters
			self.jetangle[0] = ca1.DeltaR(ca2)
			self.deltaY[0] = (ca1.Rapidity() - ca2.Rapidity())
			deltaPhi = ca1.Phi() - ca2.Phi()
			if deltaPhi > ROOT.TMath.Pi():
				deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
			if deltaPhi < -ROOT.TMath.Pi():
				deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()
			self.deltaPhi[0] = deltaPhi

			#Nsubjettiness
			if Tau2[jet1matchIndex] == 0 or Tau2[jet2matchIndex] == 0 or Tau1[jet1matchIndex] == 0 or Tau1[jet2matchIndex] == 0:
				if jet1topTagged and jet2ptcheck:
					self.cutflow[0] == 3.1
					self.treeVars.Fill()
				self.cutflow[0] = 3.2
				self.treeVars.Fill()
				self.reset()
				return

			jet1tau32Val = Tau3[jet1matchIndex] / Tau2[jet1matchIndex]
			jet2tau32Val = Tau3[jet2matchIndex] / Tau2[jet2matchIndex]
			self.jet1tau32[0] = jet1tau32Val
			self.jet2tau32[0] = jet2tau32Val

			jet1tau31Val = Tau3[jet1matchIndex] / Tau1[jet1matchIndex]
			jet2tau31Val = Tau3[jet2matchIndex] / Tau1[jet2matchIndex]
			self.jet1tau31[0] = jet1tau31Val
			self.jet2tau31[0] = jet2tau31Val

			jet1tau21Val = Tau2[jet1matchIndex] / Tau1[jet1matchIndex]
			jet2tau21Val = Tau2[jet2matchIndex] / Tau1[jet2matchIndex]
			self.jet1tau21[0] = jet1tau21Val
			self.jet2tau21[0] = jet2tau21Val

			#Fill the btagging information
			self.jet1csv[0] = CSVVals[jet1matchIndex_pj]
			self.jet2csv[0] = CSVVals[jet2matchIndex_pj]
			
			#Fill the subjet btagging information
			if jet1matchIndex_pj>len(subjet1CSV) or jet1matchIndex_pj>len(subjet2CSV) or jet1matchIndex_pj>len(subjet3CSV) or jet1matchIndex_pj>len(subjet4CSV):
				print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet1matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_jets), len(unpj))
				self.reset()
				return
			jet1subjetCSVs = []
			jet1subjetCSVs.append(subjet1CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet2CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet3CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet4CSV[jet1matchIndex_pj])

			if jet2matchIndex_pj>len(subjet1CSV) or jet2matchIndex_pj>len(subjet2CSV) or jet2matchIndex_pj>len(subjet3CSV) or jet2matchIndex_pj>len(subjet4CSV):
				print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet2matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_jets), len(unpj))
				self.reset()
				return
			jet2subjetCSVs = []
			jet2subjetCSVs.append(subjet1CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet2CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet3CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet4CSV[jet2matchIndex_pj])

			self.jet1maxSubjetCSV[0] = max(jet1subjetCSVs)
			self.jet2maxSubjetCSV[0] = max(jet2subjetCSVs)

			#Check if the second jet is top-tagged
			if self.jet2topMass[0] > 140.0 and self.jet2topMass[0] < 250.0 and self.jet2minMass[0] > 50.0 and self.jet2nSubj[0] > 2:
				#Two top-tagged jets with pt>400
				if jet1topTagged and jet2ptcheck:
					jet2topTagged = 1
					self.cutflow[0] = 4.0
					self.index[0] = 1
					self.treeVars.Fill()
				#Second jet is top tagged, but the first is not
				elif jet2ptcheck:
					jet2topTagged = 1
					self.cutflow[0] = 3.4
					self.treeVars.Fill()
				#Temporary
				else:
					self.cutflow[0] = 10.1
					self.treeVars.Fill()
			else:
				self.cutflow[0] = 3.3
				self.treeVars.Fill()
				# self.reset()
				# return

			#Mistag Contribution
			if nTopCand < 2:
				self.reset()
				return

			#Temporary
			self.cutflow[0] = 11
			self.treeVars.Fill()

			temp_x = ROOT.TRandom3(self.seed)
			x = temp_x.Uniform(1.0)

			bTag1 = max(jet1subjetCSVs) > 0.679
			bTag2 = max(jet2subjetCSVs) > 0.679
	
			bTagLoose1 = max(jet1subjetCSVs) > 0.244
			bTagLoose2 = max(jet2subjetCSVs) > 0.244

			topTag1WP1 = jet1topTagged and jet1tau32Val < 0.7 and bTagLoose1
			topTag2WP1 = jet2topTagged and jet2tau32Val < 0.7 and bTagLoose2
			
			topTag1WP2 = jet1topTagged and jet1tau32Val < 0.6 and bTagLoose1
			topTag2WP2 = jet2topTagged and jet2tau32Val < 0.6 and bTagLoose2

			topTag1WP3 = jet1topTagged and jet1tau32Val < 0.55 and bTag1
			topTag2WP3 = jet2topTagged and jet2tau32Val < 0.55 and bTag2

			topTag1WP4 = jet1topTagged and jet1tau32Val < 0.5 and bTag1 and jet1minMassVal > 65
			topTag2WP4 = jet2topTagged and jet2tau32Val < 0.5 and bTag2 and jet2minMassVal > 65

			topTag1WP5 = jet1topTagged and jet1tau32Val < 0.4 and bTag1 and jet1minMassVal > 55
			topTag2WP5 = jet2topTagged and jet2tau32Val < 0.4 and bTag2 and jet2minMassVal > 55
	
			if self.doMistag:
				mistagBin1 = self.mistag.FindBin(self.jet1pt[0])
				mistagBin2 = self.mistag.FindBin(self.jet2pt[0])
				# mistag_1B_Bin1 = self.mistag_1B.FindBin(self.jet1pt[0])
				# mistag_1B_Bin2 = self.mistag_1B.FindBin(self.jet2pt[0])
				# mistag_2B_Bin1 = self.mistag_2B.FindBin(self.jet1pt[0])
				# mistag_2B_Bin2 = self.mistag_12.FindBin(self.jet2pt[0])

				#Make sure we're not in the overflow and underflow (we should never be under)
				if self.mistag.IsBinOverflow(mistagBin1):
					mistagBin1 = self.mistag.GetNbinsX()
				elif mistagBin1 == 0:
					mistagBin1 = 1
				if self.mistag.IsBinOverflow(mistagBin2):
					mistagBin2 = self.mistag.GetNbinsX()
				elif mistagBin2 == 0:
					mistagBin2 = 1

				# if self.mistag_1B.IsBinOverflow(mistag_1B_Bin1):
				# 	mistag_1B_Bin1 = self.mistag_1B.GetNbinsX()
				# elif mistag_1B_Bin1 == 0:
				# 	mistag_1B_Bin1 = 1
				# if self.mistag_1B.IsBinOverflow(mistag_1B_Bin2):
				# 	mistag_1B_Bin2 = self.mistag_1B.GetNbinsX()
				# elif mistag_1B_Bin2 == 0:
				# 	mistag_1B_Bin2 = 1

				# if self.mistag_2B.IsBinOverflow(mistag_2B_Bin1):
				# 	mistag_2B_Bin1 = self.mistag_2B.GetNbinsX()
				# elif mistag_2B_Bin1 == 0:
				# 	mistag_2B_Bin1 = 1
				# if self.mistag_2B.IsBinOverflow(mistag_2B_Bin2):
				# 	mistag_2B_Bin2 = self.mistag_2B.GetNbinsX()
				# elif mistag_2B_Bin2 == 0:
				# 	mistag_2B_Bin2 = 1

			if self.doTrigger:
				triggerBin = self.trigger.FindBin(HTsum)
				if self.trigger.IsBinOverflow(triggerBin):
					triggerBin = self.trigger.GetNbinsX()
				elif triggerBin == 0:
					triggerBin = 1
				
				self.triggerEff = self.trigger.GetBinContent( triggerBin )

			if x < 0.5:
				if jet1topTagged:
					
					if self.doModMass:
						self.jet2topMass[0] = self.modMass.GetRandom()

					if self.doMistag:
						self.mistagWt[0] = self.mistag.GetBinContent( mistagBin2 )
					else:
						self.mistagWt[0] = 0

					self.mistagWt1B[0] = 0#self.mistag_1B.GetBinContent( mistagBin2 )
					self.mistagWt2B[0] = 0#self.mistag_2B.GetBinContent( mistagBin2 )
					self.index[0] = 2
					self.treeVars.Fill()
					self.reset()

			if x >= 0.5:
				if jet2topTagged:

					if self.doModMass:
						self.jet1topMass[0] = self.modMass.GetRandom()

					if self.doMistag:
						self.mistagWt[0] = self.mistag.GetBinContent( mistagBin1 )
					else:
						self.mistagWt[0] = 0
					self.mistagWt1B[0] = 0#self.mistag_1B.GetBinContent( mistagBin1 )
					self.mistagWt2B[0] = 0#self.mistag_2B.GetBinContent( mistagBin1 )
					self.index[0] = 2
					self.treeVars.Fill()
					self.reset()

			#self.treeVars.Fill()
			self.reset()
			return 			

	def reset(self):

		self.run[0] = -1
		self.event[0] = -1
		self.lumi[0] = -1

		self.npv[0] = -1
		self.index[0] = -1
		self.trigWt[0] = -1.0
		self.MET[0] = -1.0

		self.jet1pt[0] = -1.0
		self.jet2pt[0] = -1.0
		self.jet1eta[0] = -10.0
		self.jet2eta[0] = -10.0
		self.jet1phi[0] = -10.0
		self.jet2phi[0] = -10.0
		self.jet1mass[0] = -1.0
		self.jet2mass[0] = -1.0
		self.jet1topMass[0] = -1.0
		self.jet2topMass[0] = -1.0
		self.jet1topMassRaw[0] = -1.0
		self.jet2topMassRaw[0] = -1.0
		self.jet1csv[0] = -1.0
		self.jet2csv[0] = -1.0
		self.jet1maxSubjetCSV[0] = -1.0
		self.jet2maxSubjetCSV[0] = -1.0
		self.jet1tau32[0] = -1.0
		self.jet2tau32[0] = -1.0
		self.jet1tau31[0] = -1.0
		self.jet2tau31[0] = -1.0
		self.jet1tau21[0] = -1.0
		self.jet2tau21[0] = -1.0
		self.jet1nSubj[0] = -1
		self.jet2nSubj[0] = -1
		self.jet1minMass[0] = -1.0
		self.jet2minMass[0] = -1.0

		self.jet1nDaughters[0] = -1
		self.jet2nDaughters[0] = -1

		self.deltaY[0] = -10.0
		self.deltaPhi[0] = -10.0
		self.cutflow[0] = -1.0

		self.mistagWt[0] = -1.0
		self.mistagWt1B[0] = -1.0
		self.mistagWt2B[0] = -1.0

		self.htSum[0] = -1.0
		self.triggerEff[0] = -1.0

		self.invarmass[0] = -1.0
		self.jetangle[0] = -10.0

	def __del__(self):	
		print str(self.numbtag)
		self.f.cd()
		self.f.Write()
		self.f.Close()
		if self.doMistag:
			self.mistagFile.Close()
		if self.doModMass:
			self.modMassFile.Close()
		if self.doTrigger:
			self.triggerFile.Close()