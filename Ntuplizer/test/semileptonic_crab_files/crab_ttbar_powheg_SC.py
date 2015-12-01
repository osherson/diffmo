from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'TTBar_Powheg_SC_v1'

config.section_('JobType')
config.JobType.psetName = 'semileptonic.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['../JEC/START53_V27_L1FastJet_AK7PFchs.txt', '../JEC/START53_V27_L2Relative_AK7PFchs.txt', '../JEC/START53_V27_L3Absolute_AK7PFchs.txt', '../JEC/START53_V27_Uncertainty_AK7PFchs.txt',
								'../JEC/START53_V27_L1FastJet_AK5PFchs.txt', '../JEC/START53_V27_L2Relative_AK5PFchs.txt', '../JEC/START53_V27_L3Absolute_AK5PFchs.txt', '../JEC/START53_V27_Uncertainty_AK5PFchs.txt',
								'../JEC/Winter14_V5_DATA_L1FastJet_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
								'../JEC/Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK7PFchs.txt',
								'../JEC/Winter14_V5_DATA_L1FastJet_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK5PFchs.txt',
								'../JEC/Winter14_V5_DATA_L2L3Residual_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK5PFchs.txt']
config.JobType.pyCfgParams = ['runOnData=0', 'JES=nominal', 'JER=nominal', 'includePDF=1', 'runOnCrab=1']

config.section_('Data')
config.Data.inputDataset = '/TT_SC_CT10_TuneZ2star_8TeV-powheg-tauola/lfeng-TT_SC_CT10_TuneZ2star_8TeV-powheg-tauola-75399751a8057e7a7c06fe82568440b6/USER'
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 5
config.Data.splitting = 'FileBased'
config.Data.publishDataName = 'TTBar_Powheg_SC_v1'

config.section_('User')

config.section_('Site')
config.Site.storageSite = 'T3_US_FNALLPC'
