import os
import glob
import sys
from DataFormats.FWLite import Events, Handle
import ROOT

from optparse import OptionParser

#from example_treemaker import *
from top_xs_tree import *


#### This bit allows us to run the analyzer using command-line options
parser = OptionParser()

parser.add_option('-i', '--dirs', metavar='F', type='string', action='store',
					default='/uscms_data/d3/dfehling/diffmo_072314_update/CMSSW_5_3_17/src/Analysis/Ntuplizer/test/Data_Run_C_lumi/res/',
					dest='dirs',
					help='Input Directories (glob format)')

parser.add_option('-N', '--Nevents', metavar='N', type='int', action='store',
					default = -1,
					dest='Nevents',
					help='numEvents')

parser.add_option('--sec', metavar='N', type='int', action='store',
					default = 1,
					dest='sec',
					help='Section number (should go from 0 to totalSec)')

parser.add_option('--totalSec', metavar='N', type='int', action='store',
					default = 10,
					dest='totalSec',
					help='Total number of sections (will be this number + 1)')

parser.add_option('--seed', metavar='N', type='int', action='store',
					default=1234,
					dest='seed',
					help='Random Seed')

parser.add_option('-o', '--outfile', metavar='N', type='string', action='store',
				  default='output',
				  dest='outfile',
				  help='output file')

(options, args) = parser.parse_args()


files = glob.glob( options.dirs + "*.root" )
#FILES = "../../Ntuplizer/test/diffmotester.root"
#FILES = "../../Ntuplizer/test/Data_Run_A_missing/res/*.root"
#files = glob.glob( FILES )
print files

totalSection = options.totalSec
section = options.sec

numFiles = len(files) / totalSection

if section == totalSection:
	secFiles = files[section*numFiles:]
else :
	secFiles = files[section*numFiles:(section+1)*numFiles]

files = secFiles

events = Events (files)
#events = 10000

ntotal = events.size()
#ntotal = 10000
prune = ["diffmoca8pp", "PrunedCA8"]
unprune = ["diffmoca8", "UnprunedCA8"]
analyzer = tree_maker(prune, unprune, options.outfile, options.seed)


count = 0
print "Start looping"
for event in events:
	count = count + 1
	if count % 10000 == 0 or count == 1:
      		percentDone = float(count) / float(ntotal) * 100.0
       		print 'Processing {0:10.0f}/{1:10.0f} : {2:5.2f} %'.format(count, ntotal, percentDone )
	error = analyzer.analyze(event)
	analyzer.reset()

del analyzer
