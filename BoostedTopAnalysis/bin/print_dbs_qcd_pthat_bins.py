 #!/bin/python

import subprocess
import commands

prepend = 'dbs search --url=http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet --query='
datasets = commands.getoutput(prepend + '\"find dataset where dataset like *QCDDiJet_Pt*srappocc-ttbsm_361*\"').split('\n')

del datasets[0:4]


for dataset in datasets :
    print '------------------- ' + dataset + ' -----------------------'
    toks = dataset.split('/')
    files = commands.getoutput(prepend + '\"find file where dataset = '+ dataset +'\"').split('\n')
    del files[0:4]
    outfilestr = toks[1] + '.txt'
    outfile = open(outfilestr, 'w')
    for file in files :
        print 'appending dcap:///pnfs/cms/WAX/11' + file
        outfile.write('dcap:///pnfs/cms/WAX/11' + file + '\n')
    outfile.close()
