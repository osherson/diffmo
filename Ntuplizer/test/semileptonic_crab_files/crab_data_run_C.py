from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'Data_Run_C_v4'

config.section_('JobType')
config.JobType.psetName = 'hadronic.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['JEC/START53_V27_L1FastJet_AK7PFchs.txt', 'JEC/START53_V27_L2Relative_AK7PFchs.txt', 'JEC/START53_V27_L3Absolute_AK7PFchs.txt', 'JEC/START53_V27_Uncertainty_AK7PFchs.txt',
								'JEC/START53_V27_L1FastJet_AK5PFchs.txt', 'JEC/START53_V27_L2Relative_AK5PFchs.txt', 'JEC/START53_V27_L3Absolute_AK5PFchs.txt', 'JEC/START53_V27_Uncertainty_AK5PFchs.txt',
								'JEC/Winter14_V5_DATA_L1FastJet_AK7PFchs.txt', 'JEC/Winter14_V5_DATA_L2Relative_AK7PFchs.txt', 'JEC/Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
								'JEC/Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt', 'JEC/Winter14_V5_DATA_Uncertainty_AK7PFchs.txt',
								'JEC/Winter14_V5_DATA_L1FastJet_AK5PFchs.txt', 'JEC/Winter14_V5_DATA_L2Relative_AK5PFchs.txt', 'JEC/Winter14_V5_DATA_L3Absolute_AK5PFchs.txt',
								'JEC/Winter14_V5_DATA_L2L3Residual_AK5PFchs.txt', 'JEC/Winter14_V5_DATA_Uncertainty_AK5PFchs.txt']
config.JobType.pyCfgParams = ['runOnData=1', 'JES=nominal', 'JER=nominal', 'runOnCrab=1']

config.section_('Data')
config.Data.inputDataset = '/JetHT/StoreResults-Run2012C_22Jan2013_TLBSM_53x_0161496fccaa0bf55fbb525b618345b5-v1/USER'
config.Data.unitsPerJob = 80
config.Data.splitting = 'LumiBased'
config.Data.lumiMask = 'Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
config.Data.publishDataName = 'Data_Run_C_v4'

config.section_('User')

config.section_('Site')
config.Site.storageSite = 'T3_US_FNALLPC'
