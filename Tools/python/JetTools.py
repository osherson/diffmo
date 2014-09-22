import ROOT
from math import *
from operator import itemgetter

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

#Make sure our jets are strictly sorted in pt
#Note that we start with a vector, but end up with a list.
def ReorderByPt(UnsortedCol):
	ColByPt = []
	ColPts = [(temp_col_index, temp_col.Pt()) for temp_col_index, temp_col in enumerate(UnsortedCol)]
	ColPts.sort(key=itemgetter(1), reverse=True)
	ColIndex = [sorted_col_index[0] for sorted_col_index in ColPts]
	for k in range(len(ColIndex)):
		OrderedJet = ROOT.TLorentzVector()
		OrderedJet.SetPtEtaPhiM( UnsortedCol[ColIndex[k]].Pt(), UnsortedCol[ColIndex[k]].eta(), UnsortedCol[ColIndex[k]].phi(), UnsortedCol[ColIndex[k]].mass())
		ColByPt.append(OrderedJet)
	return ColByPt

