import subprocess
import sys

files = [
    'QCD2Jets_Pt120to280-alpgen',
    'QCD2Jets_Pt280to500-alpgen',
    'QCD2Jets_Pt40to120-alpgen',
    'QCD2Jets_Pt500to5000-alpgen',
    'QCD3Jets_Pt120to280-alpgen',
    'QCD3Jets_Pt280to500-alpgen',
    'QCD3Jets_Pt40to120-alpgen',
    'QCD3Jets_Pt500to5000-alpgen',
    'QCD4Jets_Pt120to280-alpgen',
    'QCD4Jets_Pt280to500-alpgen',
    'QCD4Jets_Pt40to120-alpgen',
    'QCD4Jets_Pt500to5000-alpgen',
    'QCD5Jets_Pt120to280-alpgen',
    'QCD5Jets_Pt280to500-alpgen',
    'QCD5Jets_Pt40to120-alpgen',
    'QCD5Jets_Pt500to5000-alpgen',
    'QCD6Jets_Pt120to280-alpgen',
    'QCD6Jets_Pt280to500-alpgen',
    'QCD6Jets_Pt40to120-alpgen',
    'QCD6Jets_Pt500to5000-alpgen'

    ]

for file in files :
    txtfile = file + '.txt'
    rootfile = file + '_dijetHt100.root'
    print 'executing : multijetStudies multijetStudies_alpgen_pthatbins_cfg.py ' + txtfile + ' ' + rootfile
    subprocess.call('multijetStudies multijetStudies_alpgen_pthatbins_cfg.py ' + txtfile + ' ' + rootfile, shell=True)
