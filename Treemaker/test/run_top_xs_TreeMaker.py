import os
import glob
import sys
from DataFormats.FWLite import Events, Handle
import ROOT

from optparse import OptionParser

from top_xs_TreeMaker import *

#### This bit allows us to run the analyzer using command-line options
parser = OptionParser()

parser.add_option('-i', '--dirs', metavar='F', type='string', action='store',
					default='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_A/',
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
					default=12345,
					dest='seed',
					help='Random Seed')

parser.add_option('-o', '--outfile', metavar='N', type='string', action='store',
					default='output',
					dest='outfile',
					help='output file')

parser.add_option('--mistagFile', metavar='N', type='string', action='store',
					default='',
					dest='mistagFile',
					help='mistag file')

parser.add_option('--modMassFile', metavar='N', type='string', action='store',
					default='',
					dest='modMassFile',
					help='mod mass file')

parser.add_option('--triggerFile', metavar='N', type='string', action='store',
					default='',
					dest='triggerFile',
					help='trigger file')

(options, args) = parser.parse_args()


files = glob.glob( options.dirs + "*.root" )
#print files

totalSection = options.totalSec
section = options.sec

numFiles = len(files) / totalSection

if section == totalSection:
	secFiles = files[section*numFiles:]
else :
	secFiles = files[section*numFiles:(section+1)*numFiles]

files = secFiles
print files

events = Events (files)
ntotal = events.size()

analyzer = tree_maker(options.outfile, options.seed, options.mistagFile, options.modMassFile, options.triggerFile)

count = 0
print "Start looping"
for event in events:
	count = count + 1
	if count % 10000 == 0 or count == 1:
			percentDone = float(count) / float(ntotal) * 100.0
			print 'Processing Job {0:2.0f} {1:10.0f}/{2:10.0f} : {3:5.2f} %'.format(section, count, ntotal, percentDone )
			
	error = analyzer.analyze(event)
	analyzer.reset()

	if count > options.Nevents and options.Nevents > 0: 
		break   

del analyzer
