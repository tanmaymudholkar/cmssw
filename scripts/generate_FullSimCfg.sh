#!/bin/bash

# Script to generate CMSSW full-sim cfg files
# from GEN cfgs (aka Pythia8 or RandomGun cfg files)

# After checking out CMSSW_10_2_20_UL,
# you must put the GEN cfg in Configuration/GenProduction/python/
CFGPATH=Configuration/GenProduction/python/Guns
#CFGPATH=Configuration/GenProduction/python/ThirteenTeV

# Specify the filename of the GEN cfg
CFG=OctoPi0_e30_m200_ctau0To3_eta0To1p4_pythia8_cfi.py
# for crab submission: NEVTS will be automatically overwritten by totalUnits in crab cfg
NEVTS=10
TAG=${CFG%_*}
EVTCONT=AODSIM
#EVTCONT=FEVTDEBUG
PU=noPU
GT=102X_upgrade2018_realistic_v15
#GT=102X_upgrade2018_realistic_v11
#GT=100X_upgrade2018_realistic_v10
ERA=Run2_2018
BEAM=Realistic25ns13TeVEarly2018Collision

##__________ Commands to generate the CMSSW full-sim cfg __________#
# These were generated using the runTheMatrix.py script
# with suitable change of parameters

## NoPileup ##
cmsDriver.py $CFGPATH/$CFG --conditions $GT -n $NEVTS --mc --era $ERA --eventcontent $EVTCONT --runUnscheduled --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@fake,RAW2DIGI,L1Reco,RECO --geometry DB:Extended --datatier GEN-SIM-DIGI-RAW-RECO --beamspot $BEAM --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(2000)" --fileout file:step_GEN2${EVTCONT}.root --python_filename ${CFG%_*}_noPU_GEN2${EVTCONT}_cfg.py --no_exec

ls -rtl

## For full MC campaign cfgs, follow test or setup commands in Actions column on McM for desired campaign. For example, for: https://cms-pdmv.cern.ch/mcm/requests?dataset_name=HAHMHToAA_AToGG_MA-0p1GeV_TuneCP5_PSweights_13TeV-madgraph_pythia8&page=0&shown=127

# LHE->GEN->SIM (LHEGS): https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIFall18wmLHEGS-03374
# DIGI->RECO + pre-mixed PU: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18DRPremix-02702
# AOD->MINIAOD: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18MiniAOD-02720
# MINIAOD->NANOAOD: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18NanoAODv7-02726

# Some tweaking of the arguments might be needed to get them to work...
