import ROOT
from DataFormats.FWLite import Events, Handle
from array import array
import math
from math import *
import sys

sys.path.insert(0, '/uscms_data/d3/osherson/HEP_GROUP_NTUPLES/DiFfMo/CMSSW_5_3_14/src/Analysis/Treemaker/Tools')
from JetTools import *
from lepWmaker import *


class tree_maker:
	def __init__(self, prunedname, unprunedname, outputname):
		# load all the event info:
		self.numbtag = 0
		self.name = outputname
		self.prunedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > ")
		self.prunedLabel = (prunedname[0], prunedname[1])			
		self.CSVHandle = Handle( "std::vector<double>" )
		self.CSVLabel = (prunedname[0], prunedname[1]+"csv")
		self.unprunedHandle = Handle("vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > > ")
		self.unprunedLabel = (unprunedname[0], unprunedname[1])	
		self.t2Handle = Handle( "std::vector<double>" )
		self.t2Label = (unprunedname[0], unprunedname[1]+"tau2")		
		self.t3Handle = Handle( "std::vector<double>" )
		self.t3Label = (unprunedname[0], unprunedname[1]+"tau3")
		self.__book__()
	def __book__(self):
		print "Booking Histograms and Trees..."
		self.f = ROOT.TFile( self.name + ".root", "recreate" )
        	self.f.cd()
		self.treeVars = ROOT.TTree('treeVars', 'treeVars')
		# making histograms:
		# self.object = array ("type", [initial value]) <- init val should be debug number.
		self.jet1pt = array('f', [-1.0])
		self.jet1eta = array('f', [100])
		self.jet1phi = array('f', [100])
		self.jet1mass = array('f', [-1.0])
		self.jet1csv = array('f', [-1.0])
		self.jet1tau32 = array('f', [-1.0])
		self.treeVars.Branch('jet1pt', self.jet1pt, 'jet1pt/F')
		self.treeVars.Branch('jet1eta', self.jet1eta, 'jet1eta/F')
		self.treeVars.Branch('jet1phi', self.jet1phi, 'jet1phi/F')
		self.treeVars.Branch('jet1mass', self.jet1mass, 'jet1mass/F')
		self.treeVars.Branch('jet1csv', self.jet1csv, 'jet1csv/F')
		self.treeVars.Branch('jet1tau32', self.jet1tau32, 'jet1tau32/F')

		self.jet2pt = array('f', [-1.0])
		self.jet2eta = array('f', [100])
		self.jet2phi = array('f', [100])
		self.jet2mass = array('f', [-1.0])
		self.jet2csv = array('f', [-1.0])
		self.jet2tau32 = array('f', [-1.0])
		self.treeVars.Branch('jet2pt', self.jet2pt, 'jet2pt/F')
		self.treeVars.Branch('jet2eta', self.jet2eta, 'jet2eta/F')
		self.treeVars.Branch('jet2phi', self.jet2phi, 'jet2phi/F')
		self.treeVars.Branch('jet2mass', self.jet2mass, 'jet2mass/F')
		self.treeVars.Branch('jet2csv', self.jet2csv, 'jet2csv/F')
		self.treeVars.Branch('jet2tau32', self.jet2tau32, 'jet2tau32/F')

		self.invarmass = array('f', [-1.0])
		self.jetangle = array('f', [-100.0])
		self.treeVars.Branch('invariant_mass', self.invarmass, 'invarmass/F')
		self.treeVars.Branch('angle_between_jets', self.jetangle, 'jetangle/F')

	def analyze(self, event):
		event.getByLabel (self.prunedLabel, self.prunedHandle)
	    	event.getByLabel (self.unprunedLabel, self.unprunedHandle)
		if (self.prunedHandle.isValid() and self.unprunedHandle.isValid()) :
			unpj = self.unprunedHandle.product()
			pj = self.prunedHandle.product()
			if len(pj) < 2:
				return 1
			ca1 = ROOT.TLorentzVector()
			ca2 = ROOT.TLorentzVector()
			ca1.SetPtEtaPhiM(pj[0].Pt(), pj[0].Eta(), pj[0].Phi(), pj[0].M())
			ca2.SetPtEtaPhiM(pj[1].Pt(), pj[1].Eta(), pj[1].Phi(), pj[1].M())

			self.jet1pt[0] = ca1.Pt()
			self.jet2pt[0] = ca2.Pt()
			self.jet1mass[0] = ca1.M()
			self.jet2mass[0] = ca2.M()
			self.jet1eta[0] = ca1.Eta()
			self.jet2eta[0] = ca2.Eta()
			self.jet1phi[0] = ca1.Phi()
			self.jet2phi[0] = ca2.Phi()
			self.invarmass[0] = (ca1+ca2).M()
			self.jetangle[0] = ca1.DeltaR(ca2)

			event.getByLabel (self.CSVLabel, self.CSVHandle)
			CSVVals = self.CSVHandle.product()
			self.jet1csv[0] = CSVVals[0]
			self.jet2csv[0] = CSVVals[1]
			if (self.jet1csv[0] > 0.697 or self.jet2csv[0] > 0.697):
				self.numbtag = self.numbtag + 1
	        	event.getByLabel (self.t3Label, self.t3Handle)
        		Tau3  =  self.t3Handle.product() 
	        	event.getByLabel (self.t2Label, self.t2Handle)
        		Tau2  =  self.t2Handle.product()

			up1 = MatchCol(unpj, ca1)
			up2 = MatchCol(unpj, ca2)

			self.jet1tau32[0] = Tau3[up1]/Tau2[up1]
			self.jet2tau32[0] = Tau3[up2]/Tau2[up2]
			self.treeVars.Fill()
			return 0
			

	def reset(self):
		self.jet1pt[0] = -1.0
		self.jet1eta[0] = 100
		self.jet1phi[0] = 100
		self.jet1mass[0] = -1.0
		self.jet1csv[0] = -1.0
		self.jet1tau32[0] = -1.0

		self.jet2pt[0] = -1.0
		self.jet2eta[0] = 100
		self.jet2phi[0] = 100
		self.jet2mass[0] = -1.0
		self.jet2csv[0] = -1.0
		self.jet2tau32[0] = -1.0

		self.invarmass[0] = -1.0
		self.jetangle[0] = -100.0

	def __del__(self):	
		print str(self.numbtag)
	        self.f.cd()
	        self.f.Write()
	        self.f.Close()
		
