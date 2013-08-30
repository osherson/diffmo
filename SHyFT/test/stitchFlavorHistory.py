#!/bin/python

# ===================================================
#             stitchFlavorHistory.py
#
#   - Inputs number of events separated by path from
#     various files
#   - Inputs a template for each flavor (b,c,q)
#   - Scales the templates to the number of events
#     in each path
#   - Sums the contributions
#   - Outputs the sum
# ===================================================
# Usage:
#   - Parameters:
#        * lum : luminosity in pb-1
#        * xs  : Leading order cross sections
#        * xs_wtot: Full cross section to scale to
#        * n   : Number of *total* events generated
#   - File to path assignment
#
# ===================================================

from ROOT import *
from array import *
from sys import argv

#histdir = argv[1]

if argv[1] == 'pf' or argv[1] == 'PF' :
    histdir = 'pfShyftAna/'
    outfilestr = 'pf'
elif argv[1] == 'calo' or argv[1] == 'CALO' :
    histdir = 'caloShyftAna/'
    outfilestr = 'calo'
elif argv[1] == 'jpt' or argv[1] == 'JPT' :
    histdir = 'jptShyftAna/'
    outfilestr = 'jpt'    

# load up root
gROOT.Macro("rootlogon.C")

# ---------------------------------------------
# Get the files from whatever samples you want to stitch
# ---------------------------------------------
f_ttbar = TFile('TTbarJets-madgraph_shyftana_38xOn35x_v2.root')
f_wjets = TFile('WJets-madgraph_shyftana_38xOn35x_v2.root')
f_zjets = TFile('ZJets-madgraph_shyftana_38xOn35x_v2.root')
f_vqq   = TFile('VqqJets-madgraph_shyftana_38xOn35x_v2.root')
f_qcd   = TFile('InclusiveMu15_shyftana_38xOn35x_v2.root')
f_st_s  = TFile('SingleTop_sChannel-madgraph_shyftana_38xOn35x_v2.root')
f_st_t  = TFile('SingleTop_tChannel-madgraph_shyftana_38xOn35x_v2.root')
f_st_tW = TFile('SingleTop_tWChannel-madgraph_shyftana_38xOn35x_v2.root')

f_data   = TFile('shyftStudies_data_2sep2010.root')
#f_data  = TFile('gooddata_11sept2010_v3.root')

# ---------------------------------------------
# Assumed luminosity (pb-1)
# ---------------------------------------------
lum = 3.1

# ---------------------------------------------
# Leading order cross sections (pb)
# ---------------------------------------------
xs_ttbar =   157.5
xs_wjets = 24170.0 # LO
xs_wtot  = 31314.0 # NNLO
xs_zjets =  3048.0
xs_vqq   =    35.8
xs_qcd   = 79688.0
xs_st_t  =    20.93
xs_st_s  =     0.0 # xs_singS  =     4.6 * 0.32442
xs_st_tW =    10.6


# ---------------------------------------------
# Relative "k" factors.
# There is an overall K-factor to get to the
# NLO (or NNLO) cross section,
# and then individual factors for each
# heavy flavor species, which can be different.
# ---------------------------------------------

k_wtot = xs_wtot / xs_wjets
s_wbb = 1.0
s_wcc = 1.0
s_wqq = 1.0


# ---------------------------------------------
# Number of generated events
# ---------------------------------------------
n_ttbar =  1483404
n_wjets = 10068895
n_zjets =  1084921
n_vqq   =   936242
n_qcd   =  4377187
n_st_t  =   528593
n_st_s  =   412055
n_st_tW =   466437

# ---------------------------------------------
# get the output file
# ---------------------------------------------
f_out = TFile('shyftPlots_stitched_' + outfilestr + '.root', 'RECREATE')

# ---------------------------------------------
# Template name
# ---------------------------------------------
histnameTag = 'secvtxMass_'
histnames0Tag = ['MET', 'hT', 'wMT']
histnamesSing = ['muPt', 'nJets', 'nTags', 'm3']

# ---------------------------------------------
# Decide if we to output the numbers at the end
# ---------------------------------------------
outputNumbers=True

# ---------------------------------------------
# Decide if we to plot the flavor histograms
# ---------------------------------------------
plotFlavorHistos=False

# ---------------------------------------------
# Decide if we to print the input histo name
# ---------------------------------------------
printHistoName=False

# ---------------------------------------------
# Output tables at just numbers
# ---------------------------------------------
printNumbersOnly=True

# ---------------------------------------------
# These are the path-tuples.
#  Syntax:
#   <path>  <sample>   <n_gen> <xs> <index for W/Z>
# ---------------------------------------------

pathNames = [
    ['VqqW_path1'      , f_vqq   ,n_vqq,   xs_vqq,   0],# path 1
    ['VqqW_path2'      , f_vqq   ,n_vqq,   xs_vqq,   0],# path 2
    ['VqqW_path3'      , f_vqq   ,n_vqq,   xs_vqq,   0],# path 3
    ['WjetsW_path4'    , f_wjets ,n_wjets, xs_wjets, 0],# path 4
    ['WjetsW_path5'    , f_wjets ,n_wjets, xs_wjets, 0],# path 5
    ['WjetsW_path6'    , f_wjets ,n_wjets, xs_wjets, 0],# path 6
    ['WjetsW_path11'   , f_wjets ,n_wjets, xs_wjets, 0],# path 11
    ['VqqZ_path1'      , f_vqq   ,n_vqq,   xs_vqq,   1],# path 1
    ['VqqZ_path2'      , f_vqq   ,n_vqq,   xs_vqq,   1],# path 2
    ['VqqZ_path3'      , f_vqq   ,n_vqq,   xs_vqq,   1],# path 3
    ['ZjetsZ_path4'    , f_zjets ,n_zjets, xs_zjets, 1],# path 4
    ['ZjetsZ_path5'    , f_zjets ,n_zjets, xs_zjets, 1],# path 5
    ['ZjetsZ_path6'    , f_zjets ,n_zjets, xs_zjets, 1],# path 6
    ['ZjetsZ_path11'   , f_zjets ,n_zjets, xs_zjets, 1] # path 11
    ]

# ---------------------------------------------
# These are the templates
# ---------------------------------------------
#bWtemplate = f_wjets.Get(histdir+'bmass').Clone()
#cWtemplate = f_wjets.Get(histdir+'cmass').Clone()
#qWtemplate = f_wjets.Get(histdir+'lfmass').Clone()
bZtemplate = f_zjets.Get(histdir+'/bmass').Clone()
cZtemplate = f_zjets.Get(histdir+'/cmass').Clone()
qZtemplate = f_zjets.Get(histdir+'/lfmass').Clone()

bWtemplate = f_vqq.Get(histdir + '/bmass').Clone()
cWtemplate = f_vqq.Get(histdir + '/cmass').Clone()
qWtemplate = f_wjets.Get(histdir + '/lfmass').Clone()

templates = [
    bWtemplate, cWtemplate, qWtemplate,
    bZtemplate, cZtemplate, qZtemplate,
    ]

for template in templates:
    template.Scale( 1.0 / template.GetEntries() )

# ---------------------------------------------
# suffixes for the various pieces of the template,
# along with fill colors
# ---------------------------------------------
suffixes = [
    [],
    [['', TColor.kWhite],['_b', TColor.kRed],['_c',TColor.kGreen],['_q',TColor.kBlue]],
    [['', TColor.kWhite],['_bb',TColor.kRed+1],['_bc',TColor.kRed+2],['_bq',TColor.kRed+3],
     ['_cc',TColor.kGreen+1],['_cq',TColor.kGreen+2],
     ['_qq',TColor.kBlue+1]]
    ]

# ---------------------------------------------
# maximum number of jets and tags
# ---------------------------------------------
maxJets = 6
maxTags = 3

# ---------------------------------------------
# the histogram storage for each flavor
# ---------------------------------------------
hists_b = []
hists_c = []
hists_q = []
hists_0 = []

# ---------------------------------------------
# Legend
# ---------------------------------------------
leg = TLegend(0.86, 0.4, 1.0, 0.9)
leg.SetFillColor(0)
leg.SetBorderSize(0)

# ---------------------------------------------
# ---------------------------------------------
#                   STEP 1
# ---------------------------------------------
# - Get the templates from the file.
# - Scale them according to the
#   number of events in the histogram in each path
# - Classify as belonging to b,c,or q for each jet bin
# ---------------------------------------------
# ---------------------------------------------

for ijet in range(1,maxJets) :
    imax = ijet
    if ijet >= maxTags :
        imax = maxTags
    else :
        imax = ijet + 1

    hists_b.append( [] )
    hists_c.append( [] )
    hists_q.append( [] )

    for itag in range(1,imax):
        hists_b[ijet-1].append( [] )
        hists_c[ijet-1].append( [] )
        hists_q[ijet-1].append( [] )
        for path in pathNames :
            WZindex = path[4]
            hists_b[ijet-1][itag-1].append( [] )
            hists_c[ijet-1][itag-1].append( [] )
            hists_q[ijet-1][itag-1].append( [] )
            # Loop over the suffixes, which are the types# Loop over the suffixes, which are the types
            # ('_b', '_c', '_q', etc)
            allsuffix = suffixes[itag]
            for suffix in allsuffix :
                s = ''
                if path[0] != 'skip' :
                    s = path[0] + '_' + histnameTag + str(ijet) + 'j_' + str(itag) + 't' + suffix[0]
                if s != '' :
                    if printHistoName :
                        print 'Getting ' + s + ' from ' + path[1].GetName()

                    # Add values to the b templates
                    if suffix[0] == '_b' or suffix[0] == '_bb' or suffix[0] == '_bc' or suffix[0] == '_bq':
                        # Grab the b template
                        h1 = templates[3*WZindex+0].Clone()
                        h1.SetName( 'template_' + s )
                        # Grab the path's histogram for counts
                        h2 = path[1].Get(histdir + s)
                        # Scale to xs * lum
                        h1.Scale( k_wtot * path[3] * lum * h2.GetEntries() / path[2] )
                        h1.SetFillColor( suffix[1] )
                        hists_b[ijet-1][itag-1][WZindex].append( h1 )
                    # Add values to the c templates
                    elif suffix[0] == '_c' or suffix[0] == '_cc' or suffix[0] == '_cq' :
                        # Grab the c template                        
                        h1 = templates[3*WZindex+1].Clone()
                        h1.SetName( 'template_' + s )
                        # Grab the path's histogram for counts                        
                        h2 = path[1].Get(histdir + s)
                        # Scale to xs * lum                        
                        h1.Scale( k_wtot  * path[3] * lum * h2.GetEntries() / path[2] )
                        h1.SetFillColor( suffix[1] )                        
                        hists_c[ijet-1][itag-1][WZindex].append( h1 )
                    # Add values to the q templates
                    elif suffix[0] == '_q' or suffix[0] == '_qq' :
                        # Grab the q template                           
                        h1 = templates[3*WZindex+2].Clone()
                        h1.SetName( 'template_' + s )
                        # Grab the path's histogram for counts                                                
                        h2 = path[1].Get(histdir + s)
                        # Scale to xs * lum                                                
                        h1.Scale( k_wtot * path[3] * lum * h2.GetEntries() / path[2] )
                        h1.SetFillColor( suffix[1] )
                        hists_q[ijet-1][itag-1][WZindex].append( h1 )
                    

canv = []
hstacks = []

# Total b,c,q contributions
sum_b = []
sum_c = []
sum_q = []

# ---------------------------------------------
# ---------------------------------------------
#                   STEP 2
# ---------------------------------------------
# Loop over each W+Jets histogram set, sum the
# contributions from a single flavor.
# Then write out to the file
# ---------------------------------------------
# ---------------------------------------------


# first do the 0-tag samples
for hist_0 in hists_0 :
    
    c0tag = TCanvas( 'c0tag_'+hist_0.GetName(), 'c0tag ' + hist_0.GetName())
    hist_0.Draw()
    canv.append(c0tag)
    f_out.cd()
    hist_0.Write()


for WZindex in range(0,2):
    if WZindex==0:
        boson = "W"
    elif WZindex==1:
        boson = "Z"

    if outputNumbers and printNumbersOnly :
        print "Heavy Flavors: "+boson
        print "njet ntag     bx     cx      q"
    elif outputNumbers :
        print "Heavy Flavors ("+boson+"-boson):"

    sum_b.append( [] )
    sum_c.append( [] )
    sum_q.append( [] )

    for ijet in range(0,maxJets):
        imax = ijet
        if ijet >= maxTags :
            imax = maxTags
        else :
            imax = ijet + 1

        for itag in range(1,imax):
            name = boson+'_secvtxMassTemplate_' + str(ijet) + 'j_' + str(itag) + 't'
            title = boson+'_SecvtxMassTemplate, ' + str(ijet) + ' Jets, ' + str(itag) + ' Tags'
            hs = THStack( name, title )
            firstB = True
            firstC = True
            firstQ = True

            # Sum B's
            sum_b[WZindex] = hists_b[ijet-1][itag-1][WZindex][0].Clone()
            sum_b[WZindex].SetName( boson+'bx_secvtxMass_' + str(ijet) + 'j_' + str(itag) + 't' )
            sum_b[WZindex].SetTitle( 'Total '+boson+'+b+X Contribution' )
            for obj_b in hists_b[ijet-1][itag-1][WZindex] :
                if firstB :
                    firstB = False
                else :
                    sum_b[WZindex].Add( obj_b )
                hs.Add( obj_b )

            # Sum C's
            sum_c[WZindex] = hists_c[ijet-1][itag-1][WZindex][0].Clone()
            sum_c[WZindex].SetName( boson+'cx_secvtxMass_' + str(ijet) + 'j_' + str(itag) + 't' )
            sum_c[WZindex].SetTitle( 'Total '+boson+'+c+X Contribution' )
            for obj_c in hists_c[ijet-1][itag-1][WZindex] :
                if firstC :
                    firstC = False
                else :
                    sum_c[WZindex].Add( obj_c )
                hs.Add( obj_c )

            # Sum Q's
            sum_q[WZindex] = hists_q[ijet-1][itag-1][WZindex][0].Clone()
            sum_q[WZindex].SetName( boson+'qq_secvtxMass_' + str(ijet) + 'j_' + str(itag) + 't' )
            sum_q[WZindex].SetTitle( 'Total '+boson+'+qq Contribution' )
            for obj_q in hists_q[ijet-1][itag-1][WZindex] :
                if firstQ :
                    firstQ = False
                else :
                    sum_q[WZindex].Add( obj_q )
                hs.Add( obj_q )      
            
        
            hstacks.append( hs )

            # Plot em
            if plotFlavorHistos :
                c = TCanvas( name, title )
                hs.Draw('hist')
                canv.append(c)
                c.Print( name + '.gif')
                c.Print( name + '.pdf')
            # Print output
            if outputNumbers :
                if printNumbersOnly:
                    print '{0:4d} {1:4d} {2:6.3f} {3:6.3f} {4:6.3f}'.format( ijet,
                                                                             itag,
                                                                             sum_b[WZindex].Integral(),
                                                                             sum_c[WZindex].Integral(),
                                                                             sum_q[WZindex].Integral())
                else :
                    print 'Jet = {0:4d}, tag = {1:4d}, nb = {2:6.3f}, nc = {3:6.3f}, nq = {4:6.3f}'.format( ijet,
                                                                                                            itag,
                                                                                                            sum_b[WZindex].Integral(),
                                                                                                            sum_c[WZindex].Integral(),
                                                                                                            sum_q[WZindex].Integral())
            # Write the sums to the file. These are what get fit.
            f_out.cd()
            sum_b[WZindex].Write()
            sum_c[WZindex].Write()
            sum_q[WZindex].Write()


# ---------------------------------------------
# ---------------------------------------------
#                   STEP 3
# ---------------------------------------------
# Now write out the summed samples
# like single top
# ---------------------------------------------
# ---------------------------------------------

summedsamples = [
    ['SingleTopTWChan_', f_st_tW, xs_st_tW, n_st_tW, False],
    ['SingleTopTChan_',  f_st_t,  xs_st_t,  n_st_t,  False],
    ['SingleTopSChan_',  f_st_s,  xs_st_s,  n_st_s,  False]
    ]

summedhists = []

summedTagHistNames = [
    'b','c','q','x','bb','bc','bq','cc','cq','qq'
    ]

if printHistoName :
    print '--------------------'
    print 'SUMMING SINGLE TOP'
    print '--------------------'

# First sum the "all tags" histograms
for histname in histnamesSing :
    for isample in range(0, len(summedsamples) ):
        sample = summedsamples[isample]
        samplename = sample[0]
        samplefile = sample[1]
        samplexs   = sample[2]
        sampleN    = sample[3]
        s_summed = histdir + histname
        if printHistoName :
            print s_summed
        h = samplefile.Get(s_summed).Clone()
        h.Scale( samplexs * lum / sampleN)
        if isample == 0 :
            summedhist = h.Clone()
            summedhist.SetName('SingleTop_' + histname )
        else :
            summedhist.Add( h )
    summedhists.append(summedhist)

singTopPretag = []
for index,histname in enumerate(histnames0Tag) :
    singTopPretag.append( [] )
    for isample in range(0, len(summedsamples) ):
        sample = summedsamples[isample]
        samplename = sample[0]
        samplefile = sample[1]
        samplexs   = sample[2]
        sampleN    = sample[3]
        s_summed = histdir + samplename + histname
        if printHistoName :
            print s_summed
        h = samplefile.Get(s_summed).Clone()
        h.Scale( samplexs * lum / sampleN)
        if isample == 0 :
            summedhist = h.Clone()
            summedhist.SetName('SingleTop_' + histname )
        else :
            summedhist.Add( h )
    summedhists.append(summedhist)

    for ijet in range (0, maxJets ):
        singTopPretag[index].append([])
        if ijet==0:
            singTopPretag[index].append([])
            singTopPretag[index][0].append(0)
            singTopPretag[index][0].append(0)
        for tagindex,summedsuffix in enumerate(['', '_0t']) :
            for isample in range(0, len(summedsamples) ):
                sample = summedsamples[isample]
                samplename = sample[0]
                samplefile = sample[1]
                samplexs   = sample[2]
                sampleN    = sample[3]
                s_summed2 = histdir + samplename + histname + '_' + str(ijet) + 'j' + summedsuffix
                if printHistoName :
                    print s_summed2
                h2 = samplefile.Get(s_summed2).Clone()
                h2.Scale( samplexs * lum / sampleN)
                if isample == 0 :
                    summedhist2 = h2.Clone()
                    summedhist2.SetName('SingleTop_' + histname + '_' + str(ijet) + 'j' + summedsuffix)
                else :
                    summedhist2.Add( h2 )
            singTopPretag[index][ijet+1].append(summedhist2.Integral())
            singTopPretag[index][0][tagindex]+=summedhist2.Integral()
            summedhists.append(summedhist2)
        
singTop = []
singTop.append(0)
# Next sum the histograms per jet-tag bin
for ijet in range (0, maxJets ):

    if ijet >= maxTags :
        imax = maxTags
    else :
        imax = ijet + 1

    for itag in range(1,imax) :
        singTop.append(0)
        for suffix in suffixes[itag] :
            for isample in range(0, len(summedsamples) ):
                sample = summedsamples[isample]
                samplename = sample[0]
                samplefile = sample[1]
                samplexs = sample[2]
                sampleN = sample[3]
                s_summed3 = histdir + samplename + histnameTag + str(ijet) + 'j_' + str(itag) + 't' + suffix[0]
                if printHistoName :
                    print s_summed3
                h3 = samplefile.Get(s_summed3).Clone()
                h3.Scale( samplexs * lum / sampleN)
                if isample == 0 :
                    summedhist3 = h3.Clone()
                    summedhist3.SetName('SingleTop_' + histnameTag + str(ijet)+'j_' + str(itag) + 't' + suffix[0])
                else :
                    summedhist3.Add( h3 )
            if suffix[0]=='':
                singTop[(ijet-1)*2+itag-1]=summedhist3.Integral()
                singTop[1]+=summedhist3.Integral()
            summedhists.append(summedhist3)

for hist in summedhists:
    f_out.cd()
    hist.Write()


# ---------------------------------------------
# ---------------------------------------------
#                   STEP 4
# ---------------------------------------------
# Now write out the non-spliced samples
# like ttbar, QCD, and the data
# ---------------------------------------------
# ---------------------------------------------



quicksamples = [
    ['Top_', f_ttbar,   xs_ttbar, n_ttbar, False, False],
    ['QCD_', f_qcd,     xs_qcd,   n_qcd,   False, False],
    ['Wjets_', f_wjets, xs_wtot,  n_wjets, True,  False],
    ['Zjets_', f_zjets, xs_zjets, n_zjets, True,  False],
    ['Data_', f_data,   1.0,      None,    False, True ]
    ]

quickhists = []
h1a = []
for sample in quicksamples :
    sampleName  = sample[0]
    sampleFile  = sample[1]
    sampleXS    = sample[2]
    sampleN     = sample[3]
    sampleHF    = sample[4]
    sampleIsData= sample[5]

    for hist in histnamesSing:
        old_string = histdir + hist
        new_string = sampleName + hist
        if printHistoName :
            print ' Getting ' + old_string
        sh1 = sampleFile.Get ( old_string).Clone()
        sh1.SetName(new_string)

        if sampleIsData == False :
            sh1.Scale( sampleXS * lum / sampleN )
        quickhists.append(sh1)

    for hist in histnames0Tag :
        s = histdir + sampleName + hist
        if printHistoName :
            print ' Getting ' + s
        h1_all = sampleFile.Get( s  ).Clone()
        if sampleIsData == False :
            h1_all.Scale( sampleXS * lum / sampleN )
        quickhists.append(h1_all)

    for ijet in range (0, maxJets ) :
        for hist in histnames0Tag :
            for presuffix in ['', '_0t'] :
                s = histdir + sampleName + hist + '_' + str(ijet) + 'j' + presuffix
                if printHistoName :
                    print ' Getting ' + s
                h1_0t = sampleFile.Get( s  ).Clone()
                if sampleIsData == False :
                    h1_0t.Scale( sampleXS * lum / sampleN )
                quickhists.append(h1_0t)
        imax = ijet

        if ijet >= maxTags :
            imax = maxTags
        else :
            imax = ijet + 1
        for itag in range(1,imax) :
            if sampleIsData == False: 
                for tagsuffix in suffixes[itag] :
                    if sampleHF == True:
                        continue
                    else :
                        s0 = histdir + sampleName + histnameTag + str(ijet) + 'j_' + str(itag) + 't' + tagsuffix[0]
                        if printHistoName :
                            print ' Getting ' + s0
                    h0 = sampleFile.Get(s0).Clone()
                    h0.Scale( sampleXS * lum / sampleN )
                    quickhists.append(h0)
                    
            else :
                s0 = histdir + sampleName + histnameTag + str(ijet) + 'j_' + str(itag) + 't'
                if printHistoName :
                    print 'Getting ' + s0
                h = sampleFile.Get(s0).Clone()

for h in quickhists :
    f_out.cd()
    h.Write()



# ---------------------------------------------
# ---------------------------------------------
#                   STEP 5
# ---------------------------------------------
# Optionally print out contents of each 
# jet-tag bin for each sample
# ---------------------------------------------
# ---------------------------------------------

    
int_storage  = [0,0,0,0,0]
int_storage0 = [0,0,0,0,0]

# Output some numbers
if outputNumbers :
    for ihist,hist in enumerate(histnames0Tag) :
        int_storage_tot  = [0,0,0,0,0]
        int_storage0_tot = [0,0,0,0,0]
        if printNumbersOnly :
            print "Table for " + hist + ":"
            print "njet ntag     Top  SingTop      Wjets    Zjets      QCD  Data"
        else :
            print "Table for " + hist
        for ijet in range (0, maxJets ) :
            for isamp,sample in enumerate(quicksamples) :
                sampleName  = sample[0]
                sampleFile  = sample[1]
                sampleXS    = sample[2]
                sampleN     = sample[3]
                sampleHF    = sample[4]
                sampleIsData= sample[5]
                s = histdir + sampleName + hist + '_' + str(ijet) + 'j'
                t = histdir + sampleName + hist + '_' + str(ijet) + 'j_0t'
                h1 = sampleFile.Get( s ).Clone()
                h2 = sampleFile.Get( t ).Clone()
                if sampleIsData == False :
                    h1.Scale( sampleXS * lum / sampleN )
                    h2.Scale( sampleXS * lum / sampleN )
                int_storage[isamp]      =h1.Integral()
                int_storage0[isamp]     =h2.Integral()
                int_storage_tot[isamp] +=h1.Integral()
                int_storage0_tot[isamp]+=h2.Integral()
            if printNumbersOnly:
                print '{0:4d}    n {2:7.3f} {3:8.3f} {4:10.3f} {5:8.3f} {6:8.3f} {7:5.0f}'.format( ijet,
                                                                                                   0,
                                                                                                   int_storage[0],
                                                                                                   singTopPretag[ihist][ijet+1][0],
                                                                                                   int_storage[2],
                                                                                                   int_storage[3],
                                                                                                   int_storage[1],
                                                                                                   int_storage[4])
            else:
                print 'Jet = {0:4d}, tag = n, nTop = {2:7.3f}, nSingTop = {3:8.3f}, nWjets = {4:10.3f}, nZjets = {5:8.3f}, nQCD = {6:8.3f}, nData = {7:5.0f}'.format( ijet,
                                                                                                                                  0,
                                                                                                                                  int_storage[0],
                                                                                                                                  singTopPretag[ihist][jet+1][0],
                                                                                                                                  int_storage[2],
                                                                                                                                  int_storage[3],
                                                                                                                                  int_storage[1],
                                                                                                                                  int_storage[4])
        if printNumbersOnly:
            print '          {2:7.3f} {3:8.3f} {4:10.3f} {5:8.3f} {6:8.3f} {7:5.0f}'.format( ijet,
                                                                                             0,
                                                                                             int_storage_tot[0],
                                                                                             singTopPretag[ihist][0][0],
                                                                                             int_storage_tot[2],
                                                                                             int_storage_tot[3],
                                                                                             int_storage_tot[1],
                                                                                             int_storage_tot[4])
        else :
            print 'Jet =  Tot, tag = {0:4d}, nTop = {1:7.3f}, nSingTop = {2:8.3f}, nWjets = {3:10.3f}, nZjets = {4:8.3f}, nQCD = {5:8.3f}, nData = {6:5.0f}'.format( 0,
                                                                                                                            int_storage_tot[0],
                                                                                                                            singTopPretag[ihist][0][0], 
                                                                                                                            int_storage_tot[2],
                                                                                                                            int_storage_tot[3],
                                                                                                                            int_storage_tot[1],
                                                                                                                            int_storage_tot[4])

    int_storage     = [0,0,0,0,0]
    int_storage_tot = [0,0,0,0,0]
    if printNumbersOnly :
        print "Table for secvtxMass: "
        print "njet ntag     Top  SingTop      QCD  Data"
    else :
        print "Table for secvtxMass"
    for ijet in range (1, maxJets ) :    
        for itag in range(1,maxTags) :
            if itag > ijet :
                continue
            else:
                for isamp,sample in enumerate(quicksamples) :
                    sampleName  = sample[0]
                    sampleFile  = sample[1]
                    sampleXS    = sample[2]
                    sampleN     = sample[3]
                    sampleHF    = sample[4]
                    sampleIsData= sample[5]
                    if sampleHF :
                        continue
                    else:
                        s0 = histdir + sampleName + 'secvtxMass_' + str(ijet) + 'j_' + str(itag) + 't'
                        h0 = sampleFile.Get(s0).Clone()
                        if sampleIsData == False:
                            h0.Scale( sampleXS * lum / sampleN )
                        int_storage[isamp]=h0.Integral()
                        int_storage_tot[isamp]+=h0.Integral()
                if printNumbersOnly :
                    print '{0:4d} {1:4d} {2:7.3f} {3:8.3f} {4:8.3f} {5:5.0f}'.format( ijet,
                                                                                      itag,
                                                                                      int_storage[0],
                                                                                      singTop[(ijet-1)*2+itag-1],
                                                                                      int_storage[1]*2.0,
                                                                                      int_storage[4])
                else :
                    print 'Jet = {0:4d}, tag = {1:4d}, nTop = {2:7.3f}, nSingTop = {3:8.3f}, nQCD = {4:8.3f}, nData = {5:5.0f}'.format( ijet,
                                                                                                                   itag,
                                                                                                                   int_storage[0],
                                                                                                                   singTop[(ijet-1)*2+itag-1],
                                                                                                                   int_storage[1]*2.0,
                                                                                                                   int_storage[4])
    if printNumbersOnly :
        print '          {1:7.3f} {2:8.3f} {3:8.3f} {4:5.0f}'.format( itag,
                                                                      int_storage_tot[0],
                                                                      singTop[1],
                                                                      int_storage_tot[1]*2.0,
                                                                      int_storage_tot[4])
    else :
        print 'Jet =  Tot, tag = comb, nTop = {1:7.3f}, nSingTop = {2:8.3f}, nQCD = {3:8.3f}, nData = {4:5.0f}'.format( itag,
                                                                                                   int_storage_tot[0],
                                                                                                   singTop[1],
                                                                                                   int_storage_tot[1]*2.0,
                                                                                                   int_storage_tot[4])
    
