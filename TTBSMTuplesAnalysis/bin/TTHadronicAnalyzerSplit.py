#! /usr/bin/env python
import os
import glob

from optparse import OptionParser


parser = OptionParser()

parser.add_option('-i', '--dirs', metavar='F', type='string', action='store',
                  default='Jet_Run2011A-PromptReco-v4_range1_ttbsm_v6_ttbsmTuples_v3',
                  dest='dirs',
                  help='Input Directories (glob format)')


parser.add_option('-a', '--analyzer', metavar='F', type='string', action='store',
                  default='Type12Analyzer',
                  dest='analyzer',
                  help='Analyzer to run. Options are Type11Analyzer,Type12Analyzer,MistagMakerType1,MistagMaker,TTKinPlotsAnalyzer')


parser.add_option('-o', '--outfile', metavar='N', type='string', action='store',
                  default='TTHadronicAnalyzerCombined_Jet_PD_May10ReReco_PromptReco_2invfb',
                  dest='outfile',
                  help='output file')

parser.add_option('--useMC', action='store_true',
                  default=False,
                  dest='useMC',
                  help='Use Monte Carlo')

parser.add_option('--veto11', action='store_true',
                  default=False,
                  dest='veto11',
                  help='Veto the type 1+1 selection in the type 1+2 selection')


parser.add_option('--useGenWeight', action='store_true',
                  default=False,
                  dest='useGenWeight',
                  help='Use generator level weights (i.e. for QCD MC)')



parser.add_option('-m', '--mistagFile', metavar='N', type='string', action='store',
                  default='MISTAG',
                  dest='mistagFile',
                  help='mistag file')



parser.add_option('-w', '--pdfWeight', metavar='W', type='string', action='store',
                  default='nominal',
                  dest='pdfWeight',
                  help='weight PDF as nominal, up, or down')


parser.add_option('-l', '--collectionLabelSuffix', metavar='N', type='string', action='store',
                  default='',
                  dest='collectionLabelSuffix',
                  help='Collection label')

parser.add_option('-t', '--triggerFile', metavar='N', type='string', action='store',
                  default='TRIGGER_EFFIC',
                  dest='triggerFile',
                  help='trigger file')

parser.add_option('--triggerFile2', metavar='N', type='string', action='store',
                  default='TRIGGER_EFFIC',
                  dest='triggerFile2',
                  help='trigger file2')



parser.add_option('--triggerWeight', metavar='N', type='string', action='store',
                  default='noWeight',
                  dest='triggerWeight',
                  help='noWeight, or weight trigger as Nominal, Up, or Down')

parser.add_option('-z', '--modMassFile', metavar='N', type='string', action='store',
                  default='ModMassFile',
                  dest='modMassFile',
                  help='modMass file')

parser.add_option('-N', '--Nevents', metavar='N', type='int', action='store',
		default = -1,
		dest='Nevents',
		help='numEvents')

parser.add_option('--sec', metavar='N', type='int', action='store',
		default = 1,
		dest='sec',
		help='sectionNum')

parser.add_option('--totalSec', metavar='N', type='int', action='store',
		default = 1,
		dest='totalSec',
		help='totalSec')


(options, args) = parser.parse_args()


files = glob.glob( options.dirs + "*.root" )
print files

print len(files)

totalSection = options.totalSec
section = options.sec


print 'totalSections', totalSection
print 'this Section', section


numFiles = len(files) / totalSection
print 'numFiles', numFiles


if section == totalSection:
	secFiles = files[section*numFiles:]
else :
	secFiles = files[section*numFiles:(section+1)*numFiles]

print secFiles

files = secFiles


import ROOT
#ROOT.gROOT.Macro("rootlogon.C")


import sys
from DataFormats.FWLite import Events, Handle

from Analysis.TTBSMTuplesAnalysis.TriggerAndEventSelectionObject import TriggerAndEventSelectionObject
from Analysis.TTBSMTuplesAnalysis.Type12Analyzer import Type12Analyzer
from Analysis.TTBSMTuplesAnalysis.MistagMaker import MistagMaker
from Analysis.TTBSMTuplesAnalysis.TTKinPlotsAnalyzer import TTKinPlotsAnalyzer
from Analysis.TTBSMTuplesAnalysis.Type11Analyzer import Type11Analyzer
from Analysis.TTBSMTuplesAnalysis.MistagMakerType1 import MistagMakerType1
from Analysis.TTBSMTuplesAnalysis.Type1PlusMETAnalyzer import Type1PlusMETAnalyzer
from Analysis.TTBSMTuplesAnalysis.New2012Analyzer import New2012Analyzer
from Analysis.TTBSMTuplesAnalysis.TPrimeAnalyzer import TPrimeAnalyzer
from Analysis.TTBSMTuplesAnalysis.TTPairAnalyzer import TTPairAnalyzer

events = Events (files)


myAnaTrigs = [
   'HLT_HT750_v1',
   'HLT_HT750_v2',
   'HLT_HT750_v3',
   'HLT_HT750_v4',
   'HLT_HT750_v5',
   'HLT_HT750_v6',
   'HLT_HT750_v7',
   'HLT_HT750_v8',
   'HLT_HT750_v9',
   'HLT_HT750_v10',
   'HLT_HT750_v11',
   'HLT_HT750_v12'


     ]

triggerSelection = TriggerAndEventSelectionObject( myAnaTrigs )

if options.useMC :  print "Running on MC!"

if options.analyzer == "Type12Analyzer" :
    analyzer = Type12Analyzer(options.useMC, options.outfile+'_sec'+str(options.sec) + '_type12_'+options.collectionLabelSuffix + '_Trigger' + options.triggerWeight,
                              options.mistagFile, options.collectionLabelSuffix,
                              options.veto11, options.useGenWeight, options.triggerFile, options.modMassFile, options.triggerWeight, options.useGenWeight, options.pdfWeight)
elif options.analyzer == "MistagMaker" :
    analyzer = MistagMaker( options.outfile+'_sec'+str(options.sec) + '_mistag' + '_Trigger' + options.triggerWeight, options.useGenWeight, options.triggerWeight)
elif options.analyzer == "Type11Analyzer" :
    analyzer = Type11Analyzer(options.useMC, options.outfile+'_sec'+str(options.sec) + '_type11_'+options.collectionLabelSuffix + '_Trigger' + options.triggerWeight,
                              options.mistagFile, options.collectionLabelSuffix, options.useGenWeight, options.triggerFile, options.modMassFile, options.pdfWeight, options.triggerWeight)
elif options.analyzer == "MistagMakerType1" :
    analyzer = MistagMakerType1( options.outfile+'_sec'+str(options.sec) + '_mistag1', options.useGenWeight)
elif options.analyzer == "TTKinPlotsAnalyzer" :
    analyzer = TTKinPlotsAnalyzer( options.outfile+'_sec'+str(options.sec) + '_kinplots')
elif options.analyzer == "Type1PlusMETAnalyzer" :
    analyzer = Type1PlusMETAnalyzer(options.useMC, options.outfile+'_sec'+str(options.sec) + '_type1plusmet_'+options.collectionLabelSuffix + '_Trigger' + options.triggerWeight,
                                    options.mistagFile, options.collectionLabelSuffix, options.useGenWeight, options.triggerFile, options.pdfWeight, options.triggerWeight)
elif options.analyzer == "2012Analyzer" :
    analyzer = New2012Analyzer(options.useMC, options.outfile+'_sec'+str(options.sec) + '_2012analyzer' + options.collectionLabelSuffix+'_Trigger'+options.triggerWeight, options.mistagFile, options.collectionLabelSuffix, options.useGenWeight, options.triggerFile, options.triggerFile2, options.modMassFile, options.pdfWeight, options.triggerWeight)
elif options.analyzer == "TPrimeAnalyzer" :
    analyzer = TPrimeAnalyzer( options.outfile+'_sec'+str(options.sec) + '_tprime')
elif options.analyzer == "TTPairAnalyzer" :
    analyzer = TTPairAnalyzer(options.useMC, options.outfile+'_sec'+str(options.sec) + '_ttpair_'+options.collectionLabelSuffix + '_Trigger' + options.triggerWeight,
                              options.mistagFile, options.collectionLabelSuffix,
                              options.useGenWeight, options.triggerFile, options.modMassFile, options.triggerWeight, options.useGenWeight, options.pdfWeight)
else :
    print 'Invalid analyzer ' + analyzer
    exit(0)


print "Loaded Analyzer"

# loop over events
count = 0
ntotal = events.size()
print "Nevents = "+str(ntotal)
print "Running over ", options.Nevents
print "Start looping"
for event in events:

    count = count + 1
    if count % 10000 == 0 :
        percentDone = float(count) / float(ntotal) * 100.0
        print 'Processing {0:10.0f}/{1:10.0f} : {2:5.2f} %'.format(
            count, ntotal, percentDone )


    if count > options.Nevents and options.Nevents > 0: 
	break

    #if options.useGenWeight:
    #    analyzer.analyze(event)

    #if count % 2 == 0 :
    #	continue


    eventPassed = False
    if options.useMC :
    	 eventPassed = True

    #print eventPassed 

 
    if not options.useMC :
        eventPassed = triggerSelection.select(event)
    	

    if eventPassed:
    	analyzer.analyze(event)
    else:
    	continue

del analyzer


print 'Adieu!'
