import os
import glob
import sys
from DataFormats.FWLite import Events, Handle
import ROOT

from example_treemaker import *



FILES = "../../Ntuplizer/test/diffmotester.root"
files = glob.glob( FILES )

events = Events (files)

ntotal = events.size()
prune = ["diffmoca8pp", "PrunedCA8"]
unprune = ["diffmoca8", "UnprundedCA8"]
analyzer = tree_maker(prune, unprune, "testtree")


count = 0
print "Start looping"
for event in events:
	count = count + 1
	if count % 1000 == 0 or count == 1:
      		percentDone = float(count) / float(ntotal) * 100.0
       		print 'Processing {0:10.0f}/{1:10.0f} : {2:5.2f} %'.format(count, ntotal, percentDone )
	error = analyzer.analyze(event)
	analyzer.reset()

del analyzer
