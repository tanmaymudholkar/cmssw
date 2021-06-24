import FWCore.ParameterSet.Config as cms

genLevelMinimalAnalyzer = cms.EDAnalyzer('GenLevelMinimalAnalyzer',
                                 genParticlesSrc = cms.untracked.InputTag("genParticles"),
)
