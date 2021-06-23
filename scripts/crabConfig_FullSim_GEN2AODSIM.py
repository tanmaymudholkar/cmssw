from CRABClient.UserUtilities import config#, getUsernameFromSiteDB
config = config()
# See parameter defintions here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile#CRAB_configuration_parameters

CFG = 'OctoPi0_e30_m200_ctau0To3_eta0To1p4_pythia8_noPU_GEN2AODSIM'

# To submit to crab:
# crab submit -c crabConfig_data.py
# To check job status:
# crab status -d <config.General.workArea>/<config.General.requestName># To resubmit jobs:
# crab resubmit -d <config.General.workArea>/<config.General.requestName>

# Local job directory will be created in:
# <config.General.workArea>/<config.General.requestName>
config.General.workArea = 'crab_MC'
config.General.requestName = CFG
config.General.transferOutputs = True
config.General.transferLogs = False

# CMS cfg file goes here:
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = '%s_cfg.py'%CFG # FullSim cfg file
config.JobType.maxMemoryMB = 2800

# Define input and units per job here:
config.Data.splitting = 'EventBased'
config.Data.totalUnits = 1000000 # total no. of events to be generated by this submission
config.Data.unitsPerJob = 250 # no. of events to be generated by each job in the submission. NOTE: crab caps total jobs/submission at 10k
config.Data.publication = False # if dataset should be published in DAS

# Output files will be stored in config.Site.storageSite at directory:
# <config.Data.outLFNDirBase>/<config.Data.outputPrimaryDataset>/<config.Data.outputDatasetTag>/
config.Site.storageSite = 'T3_US_FNALLPC'
#config.Site.storageSite = 'T2_CH_CERN'
config.Data.outLFNDirBase = '/store/group/lpcml/mandrews/'
config.Data.outputPrimaryDataset = 'Run2018_PGunsv1'
config.Data.outputDatasetTag = config.General.requestName
