
import ROOT
import math
import copy

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

from DataFormats.FWLite import Events, Handle
from Analysis.TTBSMTuplesAnalysis import *

class TPrimeAnalyzer :
    """Analyzer to plot tprime kinematic information"""
    def __init__(self, outfile):

        self.outfile = outfile

        self.wTagHandle     = Handle( "vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > >")
        self.wTagLabel      = ( "ttbsmAna", "wTagP4" )

        self.wTagMuHandle   = Handle( "std::vector<double>")
        self.wTagMuLabel    = ( "ttbsmAna",  "wTagMu" )

        self.wTagBDiscHandle   = Handle( "std::vector<double>")
        self.wTagBDiscLabel    = ( "ttbsmAna",  "wTagBDisc" )

        self.__book__()

    def __del__(self):
        self.f.cd()
        self.f.Write()
        self.f.Close()

    def __book__(self):
        self.f = ROOT.TFile(self.outfile + ".root", "recreate")
        self.f.cd()

        self.maxJetsWTags = 4

        # book single histograms
        self.nJetsCA8Pruned = ROOT.TH1F('nJetsCA8Pruned', 'C-A 0.8 Pruned;N_{Jets};Number', 10, -0.5, 9.5 )
        self.nWJetsCA8Pruned = ROOT.TH1F('nWJetsCA8Pruned', 'C-A 0.8 Pruned;N_{WJets};Number', 10, -0.5, 9.5 )
        self.pbtagJetsCA8Pruned = ROOT.TH1F('nbtagvalueJetsCA8Pruned', 'C-A 0.8 Pruned;btag value;Number', 100, 0.0, 10.0 )
        self.nbtagJetsCA8Pruned = ROOT.TH1F('nbJetsCA8Pruned', 'C-A 0.8 Pruned;N_{bjets};Number', 10, -0.5, 9.5 )
        self.InvMassCA8Pruned = ROOT.TH1F('InvMassCA8Pruned', 'InvMass; Mass GeV/c^{2};Number', 100, 0.0, 900.0 )
        self.InvMassCutsCA8Pruned = ROOT.TH1F('InvMassCutsCA8Pruned', 'InvMass; CutsMass GeV/c^{2};Number', 100, 0.0, 900.0 )
        self.deltaPhiCA8Pruned = ROOT.TH1F('deltaPhiCA8Pruned', 'C-A 0.8 Pruned;deltaPhi;Number', 100, -3.14159, 3.14159 )
        self.MassAsymmCA8Pruned = ROOT.TH1F('MassAsymmCA8Pruned', 'C-A 0.8 Pruned;MassAsymm;Number', 100, 0.0, 1.0 )
        self.STCA8Pruned = ROOT.TH1F('STCA8Pruned', 'C-A 0.8 Pruned;S_T;Number', 100, 0.0, 2000.0 )
        self.leadingWPtJetsCA8Pruned = ROOT.TH1F('leadingWPtJetsCA8Pruned', 'C-A 0.8 Pruned;leading WJet p_{T};Number', 100, 0., 1000.) 
        self.subleadingWPtJetsCA8Pruned = ROOT.TH1F('subleadingWPtJetsCA8Pruned', 'C-A 0.8 Pruned;subleading WJet p_{T};Number', 100, 0., 1000.) 
        self.leadingBPtJetsCA8Pruned = ROOT.TH1F('leadingBPtJetsCA8Pruned', 'C-A 0.8 Pruned;leading BJet p_{T};Number', 100, 0., 1000.) 
        self.subleadingBPtJetsCA8Pruned = ROOT.TH1F('subleadingBPtJetsCA8Pruned', 'C-A 0.8 Pruned;subleading BJet p_{T};Number', 100, 0., 1000.) 
        self.deltaPhiWbCA8Pruned = ROOT.TH1F('deltaPhiWbCA8Pruned', 'C-A 0.8 Pruned;deltaPhiWb;Number', 100, -3.14159, 3.14159 )

        # book lists of histograms
        self.ptJetsCA8Pruned= []
        self.yJetsCA8Pruned = []
        self.mJetsCA8Pruned = []
        self.muJetsCA8Pruned= []
        self.enJetsCA8Pruned= []

        for ijet in range(0,self.maxJetsWTags+1):
            self.ptJetsCA8Pruned.append( ROOT.TH1F('ptJetsCA8Pruned' + str(ijet),'C-A 0.8 Pruned;Jet p_{T};Number', 100, 0., 1000.) )
            self.yJetsCA8Pruned .append( ROOT.TH1F('yJetsCA8Pruned' + str(ijet), 'C-A 0.8 Pruned;Jet Rapidity;Number', 100, -2.5, 2.5) )
            self.mJetsCA8Pruned .append( ROOT.TH1F('mJetsCA8Pruned' + str(ijet), 'C-A 0.8 Pruned;Jet Mass;Number', 100, 0, 500.) )
            self.muJetsCA8Pruned.append( ROOT.TH1F('muJetsCA8Pruned' + str(ijet),'C-A 0.8 Pruned;Mass Drop (#mu = m_{0}/m_{jet});Number', 100, 0, 1) )
            self.enJetsCA8Pruned.append( ROOT.TH1F('enJetsCA8Pruned' + str(ijet),'C-A 0.8 Pruned;Energy;Number',100, 0., 1500. ) )

    def analyze(self, event) :
        # get Wtag collection
        event.getByLabel (self.wTagLabel, self.wTagHandle)
        wJets = self.wTagHandle.product()

        event.getByLabel (self.wTagMuLabel, self.wTagMuHandle)
        wJetsMu = self.wTagMuHandle.product()

        event.getByLabel (self.wTagBDiscLabel, self.wTagBDiscHandle)
        wJetsBDisc = self.wTagBDiscHandle.product()

        nWJets = 0
        nJets = 0
        nWTag = 0
        S_T = 0
        npbtag = 0
        for ijet in range(0,len(wJets)) :
            jet = wJets[ijet]
            S_T += jet.pt()
            pbtag = wJetsBDisc[ijet]
            self.pbtagJetsCA8Pruned.Fill( wJetsBDisc[ijet] )
            if jet.pt() > 30 :
                nJets += 1
                if ijet <= self.maxJetsWTags :
                    self.ptJetsCA8Pruned[ijet].Fill( jet.pt() )
                    self.yJetsCA8Pruned[ijet].Fill( jet.Rapidity() )
                    self.mJetsCA8Pruned[ijet].Fill( jet.mass() )
                    self.muJetsCA8Pruned[ijet].Fill( wJetsMu[ijet] )
                    self.enJetsCA8Pruned[ijet].Fill( jet.energy() )

# run over all (wtagged)? Jets in the event with pt over 30 GeV/c and ordered by pt (take this first 4 of these), record their info and put in corresponding histogram 
                    
            if (60.0 < jet.mass()) and (jet.mass() < 100.0) and (wJetsMu[ijet] < 0.4 and jet.pt() > 200.0) :
               nWTag += 1
               wtag = 1
               #S_T += jet.pt()

               if( nWTag == 1 ):
                   W1Matched = jet
                   self.leadingWPtJetsCA8Pruned.Fill( jet.pt() )

               if( nWTag == 2 ):
                  W2Matched = jet
                  self.subleadingWPtJetsCA8Pruned.Fill( jet.pt() )

            elif (pbtag > -9999. and jet.pt() > 120.0) :
               npbtag += 1
               #S_T += jet.pt()

               if( npbtag == 1 ) :
                  b1Matched = jet
                  self.leadingBPtJetsCA8Pruned.Fill( jet.pt() )

               if( npbtag == 2 ) :
                  b2Matched = jet
                  self.subleadingBPtJetsCA8Pruned.Fill( jet.pt() )

        #end for loop running over all jets in event

        if( nWTag == 2 and npbtag == 2 ) :
	            
            # Optimize with lowest mass difference
            # First Hypothesis
            TopTemp1 = W1Matched + b1Matched
            TopTemp2 = W2Matched + b2Matched
            # Second Hypothesis
            TopTemp3 = W1Matched + b2Matched
            TopTemp4 = W2Matched + b1Matched 

            if ( abs(TopTemp1.mass() - TopTemp2.mass()) < abs(TopTemp3.mass() - TopTemp4.mass()) ) :
               T1Reconstructed = TopTemp1
               #print ( "T1Reconstructed: " )
               #print T1Reconstructed.phi()
               T2Reconstructed = TopTemp2
               #print ( "T2Reconstructed: " )
               #print T2Reconstructed.phi()

               deltaPhiWb1 = W1Matched.phi() - b1Matched.phi()
               #print ( "deltaPhi: " )
               #print deltaPhi
               if deltaPhiWb1 > ROOT.TMath.Pi():
                  deltaPhiWb1 = deltaPhiWb1 - 2*ROOT.TMath.Pi()
               if deltaPhiWb1 < -ROOT.TMath.Pi():
                  deltaPhiWb1 = deltaPhiWb1 + 2*ROOT.TMath.Pi()
               self.deltaPhiWbCA8Pruned.Fill( deltaPhiWb1 )

               deltaPhiWb2 = W2Matched.phi() - b2Matched.phi()
               #print ( "deltaPhi: " )
               #print deltaPhi
               if deltaPhiWb2 > ROOT.TMath.Pi():
                  deltaPhiWb2 = deltaPhiWb2 - 2*ROOT.TMath.Pi()
               if deltaPhiWb2 < -ROOT.TMath.Pi():
                  deltaPhiWb2 = deltaPhiWb2 + 2*ROOT.TMath.Pi()
               self.deltaPhiWbCA8Pruned.Fill( deltaPhiWb2 )

               self.InvMassCA8Pruned.Fill( T1Reconstructed.mass() )
               self.InvMassCA8Pruned.Fill( T2Reconstructed.mass() )

            else :
               T1Reconstructed = TopTemp3
               #print ( "T1Reconstructed: " )
               #print T1Reconstructed.phi()
               T2Reconstructed = TopTemp4
               #print ( "T2Reconstructed: " )
               #print T2Reconstructed.phi()

               deltaPhiWb1 = W1Matched.phi() - b2Matched.phi()
               #print ( "deltaPhi: " )
               #print deltaPhi
               if deltaPhiWb1 > ROOT.TMath.Pi():
                  deltaPhiWb1 = deltaPhiWb1 - 2*ROOT.TMath.Pi()
               if deltaPhiWb1 < -ROOT.TMath.Pi():
                  deltaPhiWb1 = deltaPhiWb1 + 2*ROOT.TMath.Pi()
               self.deltaPhiWbCA8Pruned.Fill( deltaPhiWb1 )

               deltaPhiWb2 = W2Matched.phi() - b1Matched.phi()
               #print ( "deltaPhi: " )
               #print deltaPhi
               if deltaPhiWb2 > ROOT.TMath.Pi():
                  deltaPhiWb2 = deltaPhiWb2 - 2*ROOT.TMath.Pi()
               if deltaPhiWb2 < -ROOT.TMath.Pi():
                  deltaPhiWb2 = deltaPhiWb2 + 2*ROOT.TMath.Pi()
               self.deltaPhiWbCA8Pruned.Fill( deltaPhiWb2 )

               self.InvMassCA8Pruned.Fill( T1Reconstructed.mass() )
               self.InvMassCA8Pruned.Fill( T2Reconstructed.mass() )


            deltaPhi = T1Reconstructed.phi() - T2Reconstructed.phi()
            #print ( "deltaPhi: " )
            #print deltaPhi
            if deltaPhi > ROOT.TMath.Pi():
               deltaPhi = deltaPhi - 2*ROOT.TMath.Pi()
            if deltaPhi < -ROOT.TMath.Pi():
               deltaPhi = deltaPhi + 2*ROOT.TMath.Pi()

            massAsymmT1T2 = abs( T1Reconstructed.mass() - T2Reconstructed.mass()) / ( T1Reconstructed.mass() + T2Reconstructed.mass())


            #sensible
            if (T1Reconstructed != T2Reconstructed) :

               self.deltaPhiCA8Pruned.Fill( deltaPhi )
               self.MassAsymmCA8Pruned.Fill( massAsymmT1T2 )
               self.STCA8Pruned.Fill( S_T )

            #Cuts
            if ( (T1Reconstructed != T2Reconstructed) and 
               (abs(deltaPhi) > (2.0*3.14159/3.0)) and
               ( massAsymmT1T2 < 0.1 ) and
	       ( S_T > 1000 )
               ):
               self.InvMassCutsCA8Pruned.Fill( T1Reconstructed.mass() )
               self.InvMassCutsCA8Pruned.Fill( T2Reconstructed.mass() )
               #self.MassAsymmCA8Pruned.Fill( massAsymmT1T2 )
               #self.deltaPhiCA8Pruned.Fill( deltaPhi )


        self.nWJetsCA8Pruned.Fill( nWTag )
        self.nJetsCA8Pruned.Fill( nJets )
        self.nbtagJetsCA8Pruned.Fill( npbtag )
