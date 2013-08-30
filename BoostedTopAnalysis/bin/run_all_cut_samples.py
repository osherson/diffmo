import subprocess
import sys

files = [
 'Zprime_M1TeV_W100GeV-madgraph',
# 'Zprime_M1TeV_W10GeV-madgraph',
 'Zprime_M1500GeV_W15GeV-madgraph',
# 'Zprime_M1500GeV_W150GeV-madgraph',
# 'Zprime_M2TeV_W20GeV-madgraph',
# 'Zprime_M2TeV_W200GeV-madgraph',
 'WJets-madgraph',
 'TTbarJets-madgraph',
 'InclusiveMu15'
    ]

cutpairs = [ #[260.0, 150.],
             #[225.0, 150.],
             #[200.0, 150.],
             [125.0, 150.],
             [100.0, 150.],
             [75.0, 150.],
             [50.0, 150.],
             ]
children = []
def pollChildren(children):
    bitset = []
    for child in children:
        if child.returncode == None:
            child.poll()
            #print 'Polling child...'
            bitset.append(False)
        else:
            bitset.append(True)
            # print 'Child finished, adding to list'
    for bit in bitset:
        if bit:
            continue
        else:
            return bit
    return True
    

for cutpair in cutpairs:
    oppLeadJetPt = cutpair[0]
    htLepCut = cutpair[1]
    for file in files :
        output = open('stdoutput/' + file + '_oppLeadJetPt' + str(oppLeadJetPt) + '_htLepCut' + str(htLepCut) + '_analysis.out','w')
        txtfile = 'samples/' + file + '.txt'
        rootfile = 'histogram_files/' + file + '_oppLeadJetPt' + str(oppLeadJetPt) + '_htLepCut' + str(htLepCut) + '.root'
        s = 'semilep_analysis_WTag semilep_analysis_WTag_skims_cfg.py ' + txtfile + ' ' + rootfile + ' ' + str(oppLeadJetPt) + ' ' + str(htLepCut)
        print 'executing ' + s
        child = subprocess.Popen('semilep_analysis_WTag semilep_analysis_WTag_skims_cfg.py ' + txtfile + ' ' + rootfile + ' ' + str(oppLeadJetPt) + ' ' + str(htLepCut), shell=True, stdout = output)
        children.append(child)
    while not pollChildren(children):
        if pollChildren(children):
            break
        else:
            continue
