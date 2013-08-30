import subprocess
import sys

files = [
    'QCDDiJet_Pt0to15',
    'QCDDiJet_Pt1000to1400',
    'QCDDiJet_Pt120to170',
    'QCDDiJet_Pt1400to1800',
    'QCDDiJet_Pt15to20',
    'QCDDiJet_Pt170to230',
    'QCDDiJet_Pt1800to2200',
    'QCDDiJet_Pt20to30',
    'QCDDiJet_Pt2200to2600',
    'QCDDiJet_Pt230to300',
    'QCDDiJet_Pt2600to3000',
    'QCDDiJet_Pt3000to3500',
    'QCDDiJet_Pt300to380',
    'QCDDiJet_Pt30to50',
    'QCDDiJet_Pt380to470',
    'QCDDiJet_Pt470to600',
    'QCDDiJet_Pt50to80',
    'QCDDiJet_Pt600to800',
    'QCDDiJet_Pt800to1000',
    'QCDDiJet_Pt80to120'
    ]

for file in files :
    txtfile = file + '.txt'
    rootfile = file + '_jetstudies.root'
    print 'executing : jetStudies jetStudies_mc_qcdbins_cfg.py ' + txtfile + ' ' + rootfile
    subprocess.call('jetStudies jetStudies_mc_qcdbins_cfg.py ' + txtfile + ' ' + rootfile, shell=True)
