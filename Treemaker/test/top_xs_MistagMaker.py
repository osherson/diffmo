import ROOT
from DataFormats.FWLite import Events, Handle
from array import array
import math
from math import *
import sys

from Analysis.Tools.JetTools import *

class mistag_maker:
	def __init__(self, outputname, seed, useWeight=False):
		# load all the event info:
		self.name = outputname
		self.useWeight = useWeight
		self.seed=seed
		self.temp_x = ROOT.TRandom3(self.seed)
		self.useGenWeight = useWeight

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
		
		# self.weightsHandle = Handle ( "double" )
		# self.weightsLabel = ( "diffmoca8", "weight" )
		# self.allgenTopP4Handle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >" )
		# self.allgenTopP4Label = ("ttbsmAna", "genTopP4")
		
		self.__book__()

	def __book__(self):
		print "Booking Histograms and Trees..."
		self.f = ROOT.TFile( self.name + ".root", "recreate" )
		self.f.cd()

		self.mistagVars = ROOT.TTree('mistagVars', 'mistagVars')
		
		self.jet1pt = array('f', [-1.0])
		self.jet2pt = array('f', [-1.0])
		self.jet1eta = array('f', [-10.0])
		self.jet2eta = array('f', [-10.0])
		self.jet1phi = array('f', [-10.0])
		self.jet2phi = array('f', [-10.0])
		self.jet1mass = array('f', [-1.0])
		self.jet2mass = array('f', [-1.0])
		# self.jet1topMass = array('f', [-1.0])
		# self.jet2topMass = array('f', [-1.0])
		# self.jet1csv = array('f', [-1.0])
		# self.jet2csv = array('f', [-1.0])
		self.jet1maxSubjetCSV = array('f', [-1.0])
		self.jet2maxSubjetCSV = array('f', [-1.0])
		self.jet1tau32 = array('f', [-1.0])
		self.jet2tau32 = array('f', [-1.0])
		self.jet1tau31 = array('f', [-1.0])
		self.jet2tau31 = array('f', [-1.0])
		# self.jet1nSubj = array('i', [-1])
		# self.jet2nSubj = array('i', [-1])
		self.jet1minMass = array('f', [-1.0])
		self.jet2minMass = array('f', [-1.0])
		self.jet1BTag = array('f', [-1.])
		self.jet2BTag = array('f', [-1.])
		self.jet1TopTag = array('i', [-1])
		self.jet2TopTag = array('i', [-1])

		self.mistagVars.Branch('jet1pt', self.jet1pt, 'jet1pt/F')
		self.mistagVars.Branch('jet2pt', self.jet2pt, 'jet2pt/F')
		self.mistagVars.Branch('jet1eta', self.jet1eta, 'jet1eta/F')
		self.mistagVars.Branch('jet2eta', self.jet2eta, 'jet2eta/F')
		self.mistagVars.Branch('jet1phi', self.jet1phi, 'jet1phi/F')
		self.mistagVars.Branch('jet2phi', self.jet2phi, 'jet2phi/F')
		self.mistagVars.Branch('jet1mass', self.jet1mass, 'jet1mass/F')
		self.mistagVars.Branch('jet2mass', self.jet2mass, 'jet2mass/F')
		# self.mistagVars.Branch('jet1topMass', self.jet1topMass, 'jet1topMass/F')
		# self.mistagVars.Branch('jet2topMass', self.jet2topMass, 'jet2topMass/F')
		# self.mistagVars.Branch('jet1csv', self.jet1csv, 'jet1csv/F')
		# self.mistagVars.Branch('jet2csv', self.jet2csv, 'jet2csv/F')
		self.mistagVars.Branch('jet1maxSubjetCSV', self.jet1maxSubjetCSV, 'jet1maxSubjetCSV/F')
		self.mistagVars.Branch('jet2maxSubjetCSV', self.jet2maxSubjetCSV, 'jet2maxSubjetCSV/F')
		self.mistagVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')
		self.mistagVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')
		self.mistagVars.Branch('jet1tau31', self.jet1tau31, 'jet1tau31/F')
		self.mistagVars.Branch('jet2tau31', self.jet2tau31, 'jet2tau31/F')
		# self.mistagVars.Branch('jet1nSubj', self.jet1nSubj, 'jet1nSubj/I')
		# self.mistagVars.Branch('jet2nSubj', self.jet2nSubj, 'jet2nSubj/I')
		self.mistagVars.Branch('jet1minMass', self.jet1minMass, 'jet1minMass/F')
		self.mistagVars.Branch('jet2minMass', self.jet2minMass, 'jet2minMass/F')
		self.mistagVars.Branch('jet1BTag', self.jet1BTag, 'jet1BTag/F')
		self.mistagVars.Branch('jet2BTag', self.jet2BTag, 'jet2BTag/F')
		self.mistagVars.Branch('jet1TopTag', self.jet1TopTag, 'jet1TopTag/I')
		self.mistagVars.Branch('jet2TopTag', self.jet2TopTag, 'jet2TopTag/I')

		
		self.topTagPt         = ROOT.TH1D("topTagPt",         "Top Tag Pt",       400,  0,  2000 )
		self.topProbePt       = ROOT.TH1D("topProbePt",       "Top Probe Pt",     400,  0,  2000 )
		self.testTagPt        = ROOT.TH1D("testTagPt",        "Top Tag Pt",       400,  0,  2000 )
		self.testProbePt      = ROOT.TH1D("testProbePt",      "Top Probe Pt",     400,  0,  2000 )
		self.lowmMinTagPt     = ROOT.TH1D("lowmMinTagPt",     "Top Tag Pt",       400,  0,  2000 )
		self.lowmMinTagWP1Pt  = ROOT.TH1D("lowmMinTagPtWP1",  "Top Tag Pt WP1",   400,  0,  2000 )
		self.lowmMinTagWP2Pt  = ROOT.TH1D("lowmMinTagPtWP2",  "Top Tag Pt WP2",   400,  0,  2000 )
		self.lowmMinTagWP3Pt  = ROOT.TH1D("lowmMinTagPtWP3",  "Top Tag Pt WP3",   400,  0,  2000 )
		self.lowmMinTagWP4Pt  = ROOT.TH1D("lowmMinTagPtWP4",  "Top Tag Pt WP4",   400,  0,  2000 )
		self.lowmMinTagWP5Pt  = ROOT.TH1D("lowmMinTagPtWP5",  "Top Tag Pt WP5",   400,  0,  2000 )
		self.lowmMinTagTau31WP1Pt = ROOT.TH1D("lowmMinTagTau31WP1", "Top Tag Pt Tau31 WP1", 400, 0, 2000)
		self.lowmMinTagTau31WP2Pt = ROOT.TH1D("lowmMinTagTau31WP2", "Top Tag Pt Tau31 WP2", 400, 0, 2000)
		self.lowmMinProbePt   = ROOT.TH1D("lowmMinProbePt",   "Top Probe Pt",     400,  0,  2000 )
		self.lowmMinBTagPt    = ROOT.TH1D("lowmMinBTagPt",    "Top Tag Pt",       400,  0,  2000 )
		self.lowmMinBProbePt  = ROOT.TH1D("lowmMinBProbePt",  "Top Probe Pt",     400,  0,  2000 )
		self.lowmMin2BTagPt   = ROOT.TH1D("lowmMin2BTagPt",   "Top Tag Pt",       400,  0,  2000 )
		self.lowmMin2BProbePt = ROOT.TH1D("lowmMin2BProbePt", "Top Probe Pt",     400,  0,  2000 )
		self.antiBTagPt       = ROOT.TH1D("antiBTagPt",       "Top Tag Pt",       400,  0,  2000 ) 
		self.antiBProbePt     = ROOT.TH1D("antiBProbePt",     "Top Probe Pt",     400,  0,  2000 )

		self.lowmMinTagTau31Tau = ROOT.TH1D("lowmMinTagTau31Tau", "Top Tag Tau31", 20, 0, 1)
		self.lowmMinTagTau31WP1Tau = ROOT.TH1D("lowmMinTagTau31WP1Tau", "Top Tag Tau31 WP1", 20, 0, 1)
		self.lowmMinTagTau31WP2Tau = ROOT.TH1D("lowmMinTagTau31WP2Tau", "Top Tag Tau31 WP2", 20, 0, 1)

		self.topTagPt.Sumw2()
		self.topProbePt.Sumw2()
		self.testTagPt.Sumw2()
		self.testProbePt.Sumw2()
		self.lowmMinTagPt.Sumw2()
		self.lowmMinTagWP1Pt.Sumw2()
		self.lowmMinTagWP2Pt.Sumw2()
		self.lowmMinTagWP3Pt.Sumw2()
		self.lowmMinTagWP4Pt.Sumw2()
		self.lowmMinTagWP5Pt.Sumw2()
		self.lowmMinTagTau31WP1Pt.Sumw2()
		self.lowmMinTagTau31WP2Pt.Sumw2()
		self.lowmMinProbePt.Sumw2()
		self.lowmMinBTagPt.Sumw2()
		self.lowmMinBProbePt.Sumw2()
		self.lowmMin2BTagPt.Sumw2()
		self.lowmMin2BProbePt.Sumw2()
		self.antiBTagPt.Sumw2()
		self.antiBProbePt.Sumw2()
		self.lowmMinTagTau31Tau.Sumw2()
		self.lowmMinTagTau31WP1Tau.Sumw2()
		self.lowmMinTagTau31WP2Tau.Sumw2()

	def analyze(self, event):
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

		event.getByLabel (self.topTagTopMassLabel, self.topTagTopMassHandle)
		event.getByLabel (self.minMassLabel, self.minMassHandle)
		event.getByLabel (self.NsubjetsLabel, self.NsubjetsHandle)

		CSVVals = self.CSVHandle.product()

		subjet1CSV = self.subjet1CSVHandle.product()
		subjet2CSV = self.subjet2CSVHandle.product()
		subjet3CSV = self.subjet3CSVHandle.product()
		subjet4CSV = self.subjet4CSVHandle.product()

		Tau1  =  self.t1Handle.product()
		Tau2  =  self.t2Handle.product()
		Tau3  =  self.t3Handle.product()
		Tau4  =  self.t4Handle.product()

		topTagTopMass = self.topTagTopMassHandle.product()
		minMass = self.minMassHandle.product()
		nSubjets = self.NsubjetsHandle.product()

		weight = 1.0
		if self.useGenWeight :
			event.getByLabel( self.weightsLabel, self.weightsHandle )
			weight = self.weightsHandle.product()[0]

		unpj = self.unprunedHandle.product()
		pj = self.prunedHandle.product()
		ttpj = self.topTaggedHandle.product()

		#Require two jets minimum:
		if len(pj) < 2 or len(unpj) < 2 or len(ttpj) < 2:
			self.reset()
			return

		#Make sure there's at least 2 top condidiates by checking the pT
		nTopCand = 0
		for i in range(0,len(ttpj) ) :
			if( ttpj[i].pt() > 400 ) :
				nTopCand = nTopCand + 1

		if nTopCand < 2 :
			self.reset()
			return
		
		#Reorder to make sure the highest pT is first. This is after JEC
		pt_sorted_jets = ReorderByPt(ttpj)
		# pt_sorted_jets = ttpj
		
		#We need these for the matching for substructure
		ca1 = ROOT.TLorentzVector()
		ca2 = ROOT.TLorentzVector()

		#We create the CA jet to match and fill the jet parameters			
		ca1.SetPtEtaPhiM(pt_sorted_jets[0].Pt(), pt_sorted_jets[0].Eta(), pt_sorted_jets[0].Phi(), pt_sorted_jets[0].M())
		ca2.SetPtEtaPhiM(pt_sorted_jets[1].Pt(), pt_sorted_jets[1].Eta(), pt_sorted_jets[1].Phi(), pt_sorted_jets[1].M())

		#Jet Matching:
		jet1matchIndex = MatchCol(unpj, ca1)
		jet2matchIndex = MatchCol(unpj, ca2)
		jet1matchIndex_pj = MatchCol(pj, ca1)
		jet2matchIndex_pj = MatchCol(pj, ca2)
		jet1matchIndex_tt = MatchCol(ttpj, ca1)
		jet2matchIndex_tt = MatchCol(ttpj, ca2)

		#Nsubjettiness
		jet1tau32Val = 2.0
		jet2tau32Val = 2.0
		jet1tau31Val = 2.0
		jet2tau31Val = 2.0
		if not (Tau2[jet1matchIndex] == 0 or Tau2[jet2matchIndex] == 0) :
			jet1tau32Val = Tau3[jet1matchIndex] / Tau2[jet1matchIndex]
			jet2tau32Val = Tau3[jet2matchIndex] / Tau2[jet2matchIndex]
		if not (Tau1[jet1matchIndex] == 0 or Tau2[jet2matchIndex] == 0) :
			jet1tau31Val = Tau3[jet1matchIndex] / Tau1[jet1matchIndex]
			jet2tau31Val = Tau3[jet2matchIndex] / Tau1[jet2matchIndex]

		#Fill the btagging information - not used right now
		jet1csv = CSVVals[jet1matchIndex_pj]
		jet2csv = CSVVals[jet2matchIndex_pj]

		#Get the subjet btagging information
		jet1subjetCSVs = []
		jet2subjetCSVs = []
	
		if jet1matchIndex_pj>len(subjet1CSV) or jet1matchIndex_pj>len(subjet2CSV) or jet1matchIndex_pj>len(subjet3CSV) or jet1matchIndex_pj>len(subjet4CSV):
			print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet1matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_pj), len(unpj))
			self.reset()
			return
		jet1subjetCSVs.append(subjet1CSV[jet1matchIndex_pj])
		jet1subjetCSVs.append(subjet2CSV[jet1matchIndex_pj])
		jet1subjetCSVs.append(subjet3CSV[jet1matchIndex_pj])
		jet1subjetCSVs.append(subjet4CSV[jet1matchIndex_pj])

		if jet2matchIndex_pj>len(subjet1CSV) or jet2matchIndex_pj>len(subjet2CSV) or jet2matchIndex_pj>len(subjet3CSV) or jet2matchIndex_pj>len(subjet4CSV):
			print "Out of range: %i, %i, %i, %i, %i, %i, %i" %(jet2matchIndex_pj, len(subjet1CSV), len(subjet2CSV), len(subjet3CSV), len(subjet4CSV), len(pt_sorted_pj), len(unpj))
			self.reset()
			return
		jet2subjetCSVs.append(subjet1CSV[jet2matchIndex_pj])
		jet2subjetCSVs.append(subjet2CSV[jet2matchIndex_pj])
		jet2subjetCSVs.append(subjet3CSV[jet2matchIndex_pj])
		jet2subjetCSVs.append(subjet4CSV[jet2matchIndex_pj])

		jet1_maxSubjetCSV = max(jet1subjetCSVs)
		jet2_maxSubjetCSV = max(jet2subjetCSVs)
		
		if len(nSubjets)==0:
			print "No subjets present"
			self.reset()
			return

		#Top Tagging Parameters
		jet1_topTagTopMass = topTagTopMass[jet1matchIndex_tt]
		jet2_topTagTopMass = topTagTopMass[jet2matchIndex_tt]
		jet1_minMass = minMass[jet1matchIndex_tt]
		jet2_minMass = minMass[jet2matchIndex_tt]
		jet1_nSubjets = nSubjets[jet1matchIndex_tt]
		jet2_nSubjets = nSubjets[jet2matchIndex_tt]

		#Cuts - courtesy of JP
		failMinMass1    = jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass < 50  
		failMinMass2    = jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass < 50
		failMinMassLow1 = jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass < 30
		failMinMassLow2 = jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass < 30
		
		fail2Nsub1 = jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_nSubjets < 2
		fail2Nsub2 = jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_nSubjets < 2
		fail3Nsub1 = jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_nSubjets < 3
		fail3Nsub2 = jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_nSubjets < 3
		
		bTag1 = jet1_maxSubjetCSV
		bTag2 = jet2_maxSubjetCSV

		topTag1 = jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2 = jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2

		topTag1WP1 = jet1tau32Val < 0.7 and bTag1 > 0.244 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2WP1 = jet2tau32Val < 0.7 and bTag2 > 0.244 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2
		topTag1WP2 = jet1tau32Val < 0.6 and bTag1 > 0.244 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2WP2 = jet2tau32Val < 0.6 and bTag2 > 0.244 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2
		topTag1WP3 = jet1tau32Val < 0.55 and bTag1 > 0.679 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2WP3 = jet2tau32Val < 0.55 and bTag2 > 0.679 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2
		topTag1WP4 = jet1tau32Val < 0.5 and bTag1 > 0.679 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 65 and jet1_nSubjets > 2
		topTag2WP4 = jet2tau32Val < 0.5 and bTag2 > 0.679 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 65 and jet2_nSubjets > 2
		topTag1WP5 = jet1tau32Val < 0.4 and bTag1 > 0.679 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 55 and jet1_nSubjets > 2
		topTag2WP5 = jet2tau32Val < 0.4 and bTag2 > 0.679 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 55 and jet2_nSubjets > 2

		bTag1WP = bTag1 > 0.679
		bTag2WP = bTag2 > 0.679

		topTag1Tau31WP1 = jet1tau31Val < 0.55 and bTag1 > 0.679 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2Tau31WP1 = jet2tau31Val < 0.55 and bTag2 > 0.679 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2
		topTag1Tau31WP2 = jet1tau31Val < 0.40 and bTag1 > 0.679 and jet1_topTagTopMass > 140 and jet1_topTagTopMass < 250 and jet1_minMass > 50 and jet1_nSubjets > 2
		topTag2Tau31WP2 = jet2tau31Val < 0.40 and bTag2 > 0.679 and jet2_topTagTopMass > 140 and jet2_topTagTopMass < 250 and jet2_minMass > 50 and jet2_nSubjets > 2



		#Angular cuts
		deltaPhi = ca1.Phi() - ca2.Phi()
		if deltaPhi > ROOT.TMath.Pi():
			deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
		if deltaPhi < -ROOT.TMath.Pi():
			deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()
		
		deltaY = ca1.Rapidity() - ca2.Rapidity()

		etaCuts = 1
		deltaPhiCut = 1
		# etaCuts = abs(ca1.Rapidity()) < 2.4 and abs(ca2.Rapidity()) < 2.4
		# deltaPhiCut = abs(deltaPhi)>2.1

		if ca1.Pt() > 400. and ca2.Pt() > 400. and etaCuts and deltaPhiCut:
			self.jet1pt[0] = ca1.Pt()
			self.jet2pt[0] = ca2.Pt()
			self.jet1eta[0] = ca1.Eta()
			self.jet2eta[0] = ca2.Eta()
			self.jet1phi[0] = ca1.Phi()
			self.jet2phi[0] = ca2.Phi()
			self.jet1mass[0] = ca1.M()
			self.jet2mass[0] = ca2.M()
			self.jet1minMass[0] = jet1_minMass
			self.jet2minMass[0] = jet2_minMass
			self.jet1BTag[0] = bTag1
			self.jet2BTag[0] = bTag2
			self.jet1TopTag[0] = topTag1
			self.jet2TopTag[0] = topTag2
			self.jet1tau32[0] = jet1tau32Val
			self.jet2tau32[0] = jet2tau32Val
			self.jet1tau31[0] = jet1tau31Val
			self.jet2tau31[0] = jet2tau31Val
			self.jet1maxSubjetCSV[0] = jet1_maxSubjetCSV
			self.jet2maxSubjetCSV[0] = jet2_maxSubjetCSV
			self.mistagVars.Fill()
			
			#temp_x = ROOT.TRandom3(self.seed)
			x = self.temp_x.Uniform(1.0)

			# if topTag1 or topTag2:
			# 	print "jet1 ", x,failMinMass1, failMinMassLow1, fail2Nsub1, fail3Nsub1, bTag1, topTag1, topTag1WP1, topTag1WP2, topTag1WP3, topTag1WP4, topTag1WP5
			# 	print "jet2 ", x,failMinMass2, failMinMassLow2, fail2Nsub2, fail3Nsub2, bTag2, topTag2, topTag2WP1, topTag2WP2, topTag2WP3, topTag2WP4, topTag2WP5
			
			# if topTag1 or topTag2:
			# 	if not (topTag1 and topTag2):
			# 		print x, topTag1, topTag2

			if x < 0.5 :
				if not topTag1 :
					self.topProbePt.Fill( ca2.Pt(), weight )    
					if topTag2 :
						self.topTagPt.Fill( ca2.Pt(), weight )
				if failMinMass1 :
					self.testProbePt.Fill( ca2.Pt(), weight )
					if topTag2 :
						self.testTagPt.Fill( ca2.Pt(), weight )
				if failMinMassLow1 :
					self.lowmMinProbePt.Fill( ca2.Pt(), weight )
					if topTag2WP1:
						self.lowmMinTagWP1Pt.Fill( ca2.Pt(), weight)
					if topTag2WP2:
						self.lowmMinTagWP2Pt.Fill( ca2.Pt(), weight)
					if topTag2WP3:
						self.lowmMinTagWP3Pt.Fill( ca2.Pt(), weight)
					if topTag2WP4:
						self.lowmMinTagWP4Pt.Fill( ca2.Pt(), weight)
					if topTag2WP5:
						self.lowmMinTagWP5Pt.Fill( ca2.Pt(), weight)
					if topTag2:
						self.lowmMinTagPt.Fill( ca2.Pt(), weight )
						self.lowmMinTagTau31Tau.Fill( jet2tau31Val, weight)
					#I have a feeling that this is being done wrong
					# if not bTag1WP:
					# 	self.lowmMinBProbePt.Fill( ca2.Pt(), weight )
					# 	if bTag2WP and topTag2:
					# 		self.lowmMinBTagPt.Fill( ca2.Pt(), weight ) 
					if not bTag1WP:
						self.lowmMinBProbePt.Fill( ca2.Pt(), weight )
					if bTag2WP and topTag2:
						self.lowmMinBTagPt.Fill( ca2.Pt(), weight ) 
					

					if bTag2WP and bTag1WP:
						self.lowmMin2BProbePt.Fill( ca2.Pt(), weight )
						if topTag2:
							self.lowmMin2BTagPt.Fill( ca2.Pt(), weight )
					if topTag2Tau31WP1:
						self.lowmMinTagTau31WP1Pt.Fill( ca2.Pt(), weight )
						self.lowmMinTagTau31WP1Tau.Fill( jet2tau31Val, weight )
					if topTag2Tau31WP2:
						self.lowmMinTagTau31WP2Pt.Fill( ca2.Pt(), weight )
						self.lowmMinTagTau31WP2Tau.Fill( jet2tau31Val, weight )
				if topTag1 and topTag2 and not bTag1WP:
					self.antiBProbePt.Fill( ca2.Pt(), weight )
					if bTag2WP:
						self.antiBTagPt.Fill( ca2.Pt(), weight)        			

			if x >= 0.5 :
				if not topTag2 :
					self.topProbePt.Fill( ca1.Pt(), weight )
					if topTag1 :
						self.topTagPt.Fill( ca1.Pt(), weight )
				if failMinMass2 :
					self.testProbePt.Fill( ca1.Pt(), weight )
					if topTag1 :
						self.testTagPt.Fill( ca1.Pt(), weight )
				if failMinMassLow2 :
					self.lowmMinProbePt.Fill( ca1.Pt(), weight )
					if topTag1WP1:
						self.lowmMinTagWP1Pt.Fill( ca1.Pt(), weight)
					if topTag1WP2:
						self.lowmMinTagWP2Pt.Fill( ca1.Pt(), weight)
					if topTag1WP3:
						self.lowmMinTagWP3Pt.Fill( ca1.Pt(), weight)
					if topTag1WP4:
						self.lowmMinTagWP4Pt.Fill( ca1.Pt(), weight)
					if topTag1WP5:
						self.lowmMinTagWP5Pt.Fill( ca1.Pt(), weight)
					if topTag1:
						self.lowmMinTagPt.Fill( ca1.Pt(), weight )
						self.lowmMinTagTau31Tau.Fill( jet1tau31Val, weight)
					#I have a feeling that this is being done wrong
					# if not bTag2WP:
					# 	self.lowmMinBProbePt.Fill( ca1.Pt(), weight )
					# 	if bTag1WP and topTag1:
					# 		self.lowmMinBTagPt.Fill( ca1.Pt(), weight ) 
					if not bTag2WP:
						self.lowmMinBProbePt.Fill( ca1.Pt(), weight )
					if bTag1WP and topTag1:
						self.lowmMinBTagPt.Fill( ca1.Pt(), weight ) 

					if bTag2WP and bTag1WP:
						self.lowmMin2BProbePt.Fill( ca1.Pt(), weight )
						if topTag1 :
							self.lowmMin2BTagPt.Fill( ca1.Pt(), weight )
					if topTag1Tau31WP1:
						self.lowmMinTagTau31WP1Pt.Fill( ca1.Pt(), weight )
						self.lowmMinTagTau31WP1Tau.Fill( jet1tau31Val, weight )
					if topTag1Tau31WP2:
						self.lowmMinTagTau31WP2Pt.Fill( ca1.Pt(), weight )
						self.lowmMinTagTau31WP1Tau.Fill( jet1tau31Val, weight )
				if topTag1 and topTag2 and not bTag2WP:
					self.antiBProbePt.Fill( ca1.Pt(), weight )
					if bTag1WP:
						self.antiBTagPt.Fill( ca1.Pt(), weight)      

	def reset(self):
		self.jet1pt[0] = -1.0
		self.jet2pt[0] = -1.0
		self.jet1eta[0] = -10.0
		self.jet2eta[0] = -10.0
		self.jet1phi[0] = -10.0
		self.jet2phi[0] = -10.0
		self.jet1mass[0] = -1.0
		self.jet2mass[0] = -1.0
		# self.jet1topMass[0] = -1.0
		# self.jet2topMass[0] = -1.0
		# self.jet1csv[0] = -1.0
		# self.jet2csv[0] = -1.0
		self.jet1maxSubjetCSV[0] = -1.0
		self.jet2maxSubjetCSV[0] = -1.0
		self.jet1tau32[0] = -1.0
		self.jet2tau32[0] = -1.0
		self.jet1tau31[0] = -1.0
		self.jet2tau31[0] = -1.0
		# self.jet1nSubj[0] = -1
		# self.jet2nSubj[0] = -1
		self.jet1minMass[0] = -1.0
		self.jet2minMass[0] = -1.0
		self.jet1BTag[0] = -1
		self.jet2BTag[0] = -1
		self.jet1TopTag[0] = -1
		self.jet2TopTag[0] = -1

	def __del__(self):	
		self.f.cd()
		self.f.Write()
		self.f.Close()