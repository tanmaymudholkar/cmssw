# particle-guns
Template workflow for generating fully-simulated particle guns in CMSSW.

    $ export SCRAM_ARCH=slc7_amd64_gcc700
    $ cmsrel CMSSW_10_2_20_UL
    $ cd CMSSW_10_2_20_UL/src
    $ cmsenv
    $ git cms-init
    $ git cms-addpkg GeneratorInterface/Pythia8Interface
    $ scram b -j 6
    $ git clone git@github.com:cms-e2e/particle-guns.git
    $ cp -r particle-guns/* .
    $ rm -rf particle-guns
    $ scram b -j 6
    $ cd scripts

If you are starting from scratch, you will need to define a GEN fragment similar to the one in `Configuration/GenProduction/python/Guns/OctoPi0_e30_m200_ctau0To3_eta0To1p4_pythia8_cfi.py` that identifies which particle generator plugin should be used (`Pythia8EGunV2` in this case) and the parameters describing what should be produced. The script `generate_FullSimCfg.sh` will then generate a CMSSW process configuration file that outlines the execution path from particle generation all the way through to CMSSW full-sim reconstruction. The output will be a python configuration file that can be executed locally using `cmsRun` or passed as an input to CRAB.

To generate the full-sim configuration file

    $ . generate_FullSimCfg.sh

This should produce a file identical to the already included `OctoPi0_e30_m200_ctau0To3_eta0To1p4_pythia8_noPU_GEN2AODSIM_cfg.py`. To execute the full-sim locally

    $ cmsRun OctoPi0_e30_m200_ctau0To3_eta0To1p4_pythia8_noPU_GEN2AODSIM_cfg.py

If all looks ok, you can pass this filename as the `config.JobType.psetName` value in `crabConfig_FullSim_GEN2AODSIM.py` to initiate a batch production. It pays to generate a small test batch before going all out. To submit via CRAB

    $ crab submit -c crabConfig_FullSim_GEN2AODSIM.py
