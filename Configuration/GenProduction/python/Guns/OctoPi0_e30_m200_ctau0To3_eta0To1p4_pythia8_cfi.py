import FWCore.ParameterSet.Config as cms

generator = cms.EDFilter("Pythia8EGunV2",
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(True),

    PGunParameters = cms.PSet(
        AddAntiParticle = cms.bool(True),
        MaxEta = cms.double(1.4),
        MinEta = cms.double(-1.4),
        MaxMass = cms.double(0.2), #GeV
        MinMass = cms.double(0.199), #GeV
        MaxPhi = cms.double(3.14159265359),
        MinPhi = cms.double(-3.14159265359),
        MaxE = cms.double(30.), #GeV
        MinE = cms.double(29.), #GeV
        MaxCTau = cms.double(3.), #mm/c
        MinCTau = cms.double(3.e-5), #mm/c
        ParticleID = cms.vint32(111,111,111,111) #pdgid
    ),
    Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts
    psethack = cms.string('octo pi0 Egun'),
    firstRun = cms.untracked.uint32(1),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring('processParameters'),
        processParameters = cms.vstring(
            '111:onMode = off',
            '111:onIfMatch = 22 22')
    )
)
