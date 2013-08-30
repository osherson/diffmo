#!/usr/bin/python
import os 

files = [
    'qcd_1000_1400',
    'qcd_120_170',
    'qcd_1400_1800',
    'qcd_170_230',
    'qcd_1800_2200',
    'qcd_2200_2600',
    'qcd_230_300',
    'qcd_2600_3000',
    'qcd_3000_3500',
    'qcd_300_380',
    'qcd_380_470',
    'qcd_470_600',
    'qcd_600_800',
    'qcd_800_1000',
]

for x in files : 
    l = 'make_mistag_parameterization inputFiles_load=\'' + x + '.txt\' outputFile=\'' + x + '_mistag\' storeprepend=\'dcap:///pnfs/cms/WAX/11\' outputevery=5000'
    print 'executing ' + l
    os.system(l) 

print 'finished!'
