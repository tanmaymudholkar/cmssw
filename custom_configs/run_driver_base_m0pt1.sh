CFGPATH=Configuration/GenProduction/python/Guns
CFG=pi0_back_to_back_m0pt1_cfg.py
CFG_BASE=`echo ${CFG} | sed "s|_cfg.py||"`
# for crab submission: NEVTS will be automatically overwritten by totalUnits in crab cfg
NEVTS=1000
EVTCONT=AODSIM
PU=noPU
GT=102X_upgrade2018_realistic_v15
ERA=Run2_2018
BEAM=Realistic25ns13TeVEarly2018Collision

## NoPileup ##
set -x
cmsDriver.py ${CFGPATH}/${CFG} --conditions ${GT} -n ${NEVTS} --mc --era ${ERA} --eventcontent ${EVTCONT} --runUnscheduled --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@fake,RAW2DIGI,L1Reco,RECO --geometry DB:Extended --datatier GEN-SIM-DIGI-RAW-RECO --beamspot ${BEAM} --customise_commands process.source.numberEventsInLuminosityBlock="cms.untracked.uint32(2000)"
set +x

## For full MC campaign cfgs, follow test or setup commands in Actions column on McM for desired campaign. For example, for: https://cms-pdmv.cern.ch/mcm/requests?dataset_name=HAHMHToAA_AToGG_MA-0p1GeV_TuneCP5_PSweights_13TeV-madgraph_pythia8&page=0&shown=127

# LHE->GEN->SIM (LHEGS): https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIFall18wmLHEGS-03374
# DIGI->RECO + pre-mixed PU: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18DRPremix-02702
# AOD->MINIAOD: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18MiniAOD-02720
# MINIAOD->NANOAOD: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_setup/HIG-RunIIAutumn18NanoAODv7-02726

# Some tweaking of the arguments might be needed to get them to work...
