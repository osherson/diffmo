import ROOT
from DataFormats.FWLite import Events, Handle
from array import array
import math
from math import *
import sys
#from operator import itemgetter,attrgetter

#sys.path.insert(0, '/uscms_data/d3/osherson/HEP_GROUP_NTUPLES/DiFfMo/CMSSW_5_3_14/src/Analysis/Treemaker/Tools')
sys.path.insert(0, '../Tools')
from JetTools import *
from lepWmaker import *


class tree_maker:
	def __init__(self, prunedname, unprunedname, outputname):
		# load all the event info:
		self.numbtag = 0
		self.name = outputname

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
		########################## I want to eventually compare this pruned collection vs the top tagged one. For now, let's use prunded because marc ###############
		self.prunedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > " )
		self.prunedLabel  = ( prunedname[0], prunedname[1] )

		#Btagging CSV values
		self.CSVHandle = Handle( "std::vector<double>" )
		self.CSVLabel  = ( prunedname[0], prunedname[1]+"csv" )

		#Subjet btagging CSV values
		self.subjet1CSVHandle = Handle( "std::vector<double>" )
		self.subjet1CSVLabel  = ( prunedname[0], prunedname[1]+"sub0csv" )
		self.subjet2CSVHandle = Handle( "std::vector<double>" )
		self.subjet2CSVLabel  = ( prunedname[0], prunedname[1]+"sub1csv" )
		self.subjet3CSVHandle = Handle( "std::vector<double>" )
		self.subjet3CSVLabel  = ( prunedname[0], prunedname[1]+"sub2csv" )
		self.subjet4CSVHandle = Handle( "std::vector<double>" )
		self.subjet4CSVLabel  = ( prunedname[0], prunedname[1]+"sub3csv" )

		#TopTagged Jet collection
		#Eventually this is the one we will use most likely
		self.topTaggedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > " )
		self.topTaggedLabel  = ( "diffmoca8tt", "TopTaggedPrunedCA8")
		
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
		self.unprunedLabel  = (unprunedname[0], unprunedname[1])	
		
		#N-Subjettiness(tau)
		self.t1Handle = Handle( "std::vector<double>" )
		self.t1Label  = (unprunedname[0], unprunedname[1]+"tau1")
		self.t2Handle = Handle( "std::vector<double>" )
		self.t2Label  = (unprunedname[0], unprunedname[1]+"tau2")
		self.t3Handle = Handle( "std::vector<double>" )
		self.t3Label  = (unprunedname[0], unprunedname[1]+"tau3")
		self.t4Handle = Handle( "std::vector<double>" )
		self.t4Label  = (unprunedname[0], unprunedname[1]+"tau4")
		
		#Number of Daughters
		self.NdaughtersHandle = Handle( "std::vector<unsigned int>" )
		self.NdaughtersLabel  = ( unprunedname[0], unprunedname[1]+"nsub" )

		self.__book__()

	def __book__(self):


		#Mistag files?!

		print "Booking Histograms and Trees..."
		self.f = ROOT.TFile( self.name + ".root", "recreate" )
		self.f.cd()
		self.treeVars = ROOT.TTree('treeVars', 'treeVars')
		# making histograms:
		# self.object = array ("type", [initial value]) <- init val should be debug number.
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
		self.jet1csv = array('f', [-1.0])
		self.jet2csv = array('f', [-1.0])
		self.jet1maxSubjetCSV = array('f', [-1.0])
		self.jet2maxSubjetCSV = array('f', [-1.0])
		self.jet1tau32 = array('f', [-1.0])
		self.jet2tau32 = array('f', [-1.0])
		self.jet1nSubj = array('i', [-1])
		self.jet2nSubj = array('i', [-1])
		self.jet1minMass = array('f', [-1.0])
		self.jet2minMass = array('f', [-1.0])

		self.jet1nDaughters = array('i', [-1])
		self.jet2nDaughters = array('i', [-1])
		
		self.deltaY = array('f', [-10.0])
		self.deltaPhi = array('f', [-10.0])
		self.cutflow = array('f', [-1.0])

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
		self.treeVars.Branch('jet1csv', self.jet1csv, 'jet1csv/F')
		self.treeVars.Branch('jet2csv', self.jet2csv, 'jet2csv/F')
		self.treeVars.Branch('jet1maxSubjetCSV', self.jet1maxSubjetCSV, 'jet1maxSubjetCSV/F')
		self.treeVars.Branch('jet2maxSubjetCSV', self.jet2maxSubjetCSV, 'jet2maxSubjetCSV/F')
		self.treeVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
		self.treeVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
		self.treeVars.Branch('jet1nSubj', self.jet1nSubj, 'jet1nSubj/I')
		self.treeVars.Branch('jet2nSubj', self.jet2nSubj, 'jet2nSubj/I')
	
		self.treeVars.Branch('jet1nDaughters', self.jet1nDaughters, 'jet1nDaughters/I')
		self.treeVars.Branch('jet2nDaughters', self.jet2nDaughters, 'jet2nDaughters/I')
		
		self.treeVars.Branch('jet1minMass', self.jet1minMass, 'jet1minMass/F')
		self.treeVars.Branch('jet2minMass', self.jet2minMass, 'jet2minMass/F')

		self.treeVars.Branch('deltaY', self.deltaY, 'deltaY/F')
		self.treeVars.Branch('deltaPhi', self.deltaPhi, 'deltaPhi/F')
		self.treeVars.Branch('cutflow', self.cutflow, 'cutflow/F')

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

		#if (self.prunedHandle.isValid() and self.unprunedHandle.isValid()) :
		if (self.prunedHandle.isValid() and self.unprunedHandle.isValid() and self.topTaggedHandle.isValid()) :
			unpj = self.unprunedHandle.product()
			pj = self.prunedHandle.product()
			ttpj = self.topTaggedHandle.product()
			
			## approximately 3% of the time we have more pj than unpj. Why? Shouldn't this be impossible?
			## The pruning can separate 2 subjets into standalone jets so this makes sense
			if len(unpj)<len(pj):
				self.numbtag=self.numbtag+1

			if len(pj) < 2 or len(unpj) < 2 or len(ttpj) < 2:
				self.cutflow[0] = 1.0
				self.treeVars.Fill()
				if len(pj) < 2:
					self.cutflow[0] = 1.1
					self.treeVars.Fill()
				if len(unpj) < 2:
					self.cutflow[0] = 1.2
					self.treeVars.Fill()
					#print len(unpj),len(pj)
				if len(ttpj) < 2:
					self.cutflow[0] = 1.3
					self.treeVars.Fill()
					#print len(unpj),len(pj)
				self.reset()
				return

			
			#We need these for the substructure
			ca1 = ROOT.TLorentzVector()
			ca2 = ROOT.TLorentzVector()
			
			pt_sorted_pj = ReorderByPt(pj)

			ca1.SetPtEtaPhiM(pt_sorted_pj[0].Pt(), pt_sorted_pj[0].Eta(), pt_sorted_pj[0].Phi(), pt_sorted_pj[0].M())
			ca2.SetPtEtaPhiM(pt_sorted_pj[1].Pt(), pt_sorted_pj[1].Eta(), pt_sorted_pj[1].Phi(), pt_sorted_pj[1].M())

			#Match unpruned jets with pruned - so we have both subjet btagging and nsubjettiness
			#This returns the index of the jet in the first collection that matches within dr = 0.4 to the jet of the second argument
			jet1matchIndex = MatchCol(unpj, ca1)
			jet2matchIndex = MatchCol(unpj, ca2)

			#This is temporary so we get the correct pj index for CSV and subjet CSV
			jet1matchIndex_pj = MatchCol(pj, ca1)
			jet2matchIndex_pj = MatchCol(pj, ca2)

			#Again temporary to match toptagged for Nsubjets, minMass, and topMass
			jet1matchIndex_tt = MatchCol(ttpj, ca1)
			jet2matchIndex_tt = MatchCol(ttpj, ca2)

			up_match1 = ROOT.TLorentzVector()
			up_match1.SetPtEtaPhiM(unpj[jet1matchIndex].Pt(), unpj[jet1matchIndex].Eta(), unpj[jet1matchIndex].Phi(), unpj[jet1matchIndex].M())

			# Do we have matches?
			if jet1matchIndex==-1 or jet2matchIndex==-1:
				self.cutflow[0] = 2.0
				self.treeVars.Fill()
				if jet1matchIndex==-1:
					self.cutflow[0] = 2.1
					self.treeVars.Fill()
				if jet2matchIndex==-1:
					self.cutflow[0] = 2.2
					self.treeVars.Fill()
				self.reset()
				return

			################# subjet csv is tied with pj, so we want only the index that matches that appropriate jet #####################

			############# can be removed eventually ###################
			if jet1matchIndex == jet2matchIndex:
				print "jet indices %i match" %jet1matchIndex
				print "jet1 = %d %d %d %d" %(ca1.Pt(), ca1.Eta(), ca1.Phi(), ca1.M())
				print "jet2 = %d %d %d %d" %(ca2.Pt(), ca2.Eta(), ca2.Phi(), ca2.M())
				print "up   = %d %d %d %d" %(unpj[jet1matchIndex].Pt(), unpj[jet1matchIndex].Eta(), unpj[jet1matchIndex].Phi(), unpj[jet1matchIndex].M())
				print "deltaR1 = %d deltaR2 = %d" %(up_match1.DeltaR(ca1), up_match1.DeltaR(ca2))

			self.MET[0] = metPt
			self.npv[0] = npv

			# Do we have matches to topTags?
			if jet1matchIndex_tt==-1 or jet2matchIndex_tt==-1:
				self.cutflow[0] = 2.3
				self.treeVars.Fill()
				if jet1matchIndex_tt==-1:
					self.cutflow[0] = 2.4
					self.treeVars.Fill()
				if jet2matchIndex_tt==-1:
					self.cutflow[0] = 2.5
					self.treeVars.Fill()
				self.reset()
				return
			
			# print nSubjets
			# print jet1matchIndex,jet2matchIndex
			# print jet1matchIndex_tt,jet2matchIndex_tt
			# print len(nSubjets)
			# print nsubjetsPruned
			# print jet2matchIndex_pj
			# print len(nsubjetsPruned)

			if len(nSubjets)==0:
				self.cutflow[0] = 3.0
				self.treeVars.Fill()
				self.reset()
				return
			else:
				self.jet1nSubj[0] = nSubjets[jet1matchIndex_tt]
				self.jet2nSubj[0] = nSubjets[jet2matchIndex_tt]
				if len(nSubjets)<3:
					self.cutflow[0] = 4.0
					self.treeVars.Fill()
					self.reset()
					return
			##################### We don't really need this but here it is ###################################
			if len(nDaughters)>0:
				self.jet1nDaughters[0] = nDaughters[jet1matchIndex]
				self.jet2nDaughters[0] = nDaughters[jet2matchIndex]

			################ We should probably get the mass and mimass from the tag infos after adding to the ntuple ######################
			################ At this moment, we're actually completely unable to make the min mass, but it's correlated to tau32, so let's look there first ##################

		 	self.jet1pt[0] = ca1.Pt()
			self.jet2pt[0] = ca2.Pt()
			self.jet1eta[0] = ca1.Eta()
			self.jet2eta[0] = ca2.Eta()
			self.jet1phi[0] = ca1.Phi()
			self.jet2phi[0] = ca2.Phi()
			self.jet1mass[0] = ca1.M()
			self.jet2mass[0] = ca2.M()
			self.invarmass[0] = (ca1+ca2).M()
			self.jetangle[0] = ca1.DeltaR(ca2)
			self.deltaY[0] = (ca1.Rapidity() - ca2.Rapidity())
			deltaPhi = ca1.Phi() - ca2.Phi()
			if deltaPhi > ROOT.TMath.Pi():
				deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
			if deltaPhi < -ROOT.TMath.Pi():
				deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()
			self.deltaPhi[0] = deltaPhi

			self.jet1csv[0] = CSVVals[jet1matchIndex_pj]
			self.jet2csv[0] = CSVVals[jet2matchIndex_pj]

			self.jet1minMass[0] = minMass[jet1matchIndex_tt]
			self.jet2minMass[0] = minMass[jet2matchIndex_tt]

			#### This should rightfully be jet1mass and jet2mass ####
			self.jet1topMass[0] = topTagTopMass[jet1matchIndex_tt]
			self.jet2topMass[0] = topTagTopMass[jet2matchIndex_tt]
			
			#if jet2matchIndex_pj<jet1matchIndex_pj:
				#print "sorted pj"
			# if Tau2[0]
			self.jet1tau32[0] = Tau3[jet1matchIndex] / Tau2[jet1matchIndex]
			self.jet2tau32[0] = Tau3[jet2matchIndex] / Tau2[jet2matchIndex]

			############# Let's make this a loop over all subjets eventually and not just the leading 4 ##############################
			if jet1matchIndex_pj>len(subjet1CSV) or jet1matchIndex_pj>len(subjet2CSV) or jet1matchIndex_pj>len(subjet3CSV) or jet1matchIndex_pj>len(subjet4CSV):
				print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet1matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_pj), len(unpj))
				return
			jet1subjetCSVs = []
			jet1subjetCSVs.append(subjet1CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet2CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet3CSV[jet1matchIndex_pj])
			jet1subjetCSVs.append(subjet4CSV[jet1matchIndex_pj])

			if jet2matchIndex_pj>len(subjet1CSV) or jet2matchIndex_pj>len(subjet2CSV) or jet2matchIndex_pj>len(subjet3CSV) or jet2matchIndex_pj>len(subjet4CSV):
				print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet2matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_pj), len(unpj))
				return
			jet2subjetCSVs = []
			jet2subjetCSVs.append(subjet1CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet2CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet3CSV[jet2matchIndex_pj])
			jet2subjetCSVs.append(subjet4CSV[jet2matchIndex_pj])

			self.jet1maxSubjetCSV[0] = max(jet1subjetCSVs)
			self.jet2maxSubjetCSV[0] = max(jet2subjetCSVs)

			if self.jet1topMass[0] <= 140.0 or self.jet1topMass[0] >= 250.0 or self.jet2topMass[0] <= 140.0 or self.jet2topMass[0] >= 250.0:
				self.cutflow[0] = 5.0
				self.treeVars.Fill()
				if self.jet1topMass[0] <= 140.0 or self.jet1topMass[0] >= 250.0:
					self.cutflow[0] = 5.1
					self.treeVars.Fill()
				if self.jet2topMass[0] <= 140.0 or self.jet2topMass[0] >= 250.0:
					self.cutflow[0] = 5.2
					self.treeVars.Fill()
				self.reset()
				return

			if self.jet1minMass[0] <= 50.0:
				self.cutflow[0] = 6.0
				self.treeVars.Fill()
				return

			self.index[0] = 1
			self.treeVars.Fill()
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
		self.jet1csv[0] = -1.0
		self.jet2csv[0] = -1.0
		self.jet1maxSubjetCSV[0] = -1.0
		self.jet2maxSubjetCSV[0] = -1.0
		self.jet1tau32[0] = -1.0
		self.jet2tau32[0] = -1.0
		self.jet1nSubj[0] = -1
		self.jet2nSubj[0] = -1
		self.jet1minMass[0] = -1.0
		self.jet2minMass[0] = -1.0

		self.deltaY[0] = -10.0
		self.deltaPhi[0] = -10.0
		self.cutflow[0] = -1.0

		self.invarmass[0] = -1.0
		self.jetangle[0] = -10.0

		self.jet1nDaughters[0] = -1
		self.jet2nDaughters[0] = -1

	def __del__(self):	
		print str(self.numbtag)
		self.f.cd()
		self.f.Write()
		self.f.Close()
		
