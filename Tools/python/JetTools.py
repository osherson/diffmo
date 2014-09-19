import ROOT
import math
from math import *

def ClosestJet(jets, fourvec): #returns the index of the jet (from a collection "jets") closest to the given four-vector
	DR = 9999.
	index = -1
	for j in range(0,len(jets)):
	    if jets[j].Pt() > 0 :
		dR = fourvec.DeltaR(jets[j])
		if dR < DR :
			DR = fourvec.DeltaR(jets[j])
			index = j
	return index

def MatchCol(Col, jet, maxDR=0.4):
	j = -1
	for i in range(len(Col)):
        	C = ROOT.TLorentzVector()
       		C.SetPtEtaPhiM( Col[i].Pt(), Col[i].eta(), Col[i].phi(), Col[i].mass() )
		dr = abs(jet.DeltaR(C))
		if dr < maxDR :
			j = i
        		break
	if dr > maxDR:
		return -1
	return j


