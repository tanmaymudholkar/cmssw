import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
options.register(name="inputPath",
                 default="none",
                 mult=VarParsing.multiplicity.singleton,
		 mytype=VarParsing.varType.string,
		 info="Path to AOD input file.")
options.register(name="outputPath",
                 default="none",
                 mult=VarParsing.multiplicity.singleton,
		 mytype=VarParsing.varType.string,
		 info="Path to output file.")
# options.register(name="verbosity",
#                  default=0,
# 		 mult=VarParsing.multiplicity.singleton,
# 		 mytype=VarParsing.varType.int,
# 		 info="Verbosity.")
options.parseArguments()

process = cms.Process("GenLevelMinimalAnalyzer")
# process.load("temp.GenLevelDeltaRAnalyzer.genLevelDeltaRAnalyzer_cfi")
process.load("E2E.GenLevelMinimalAnalyzer.GenLevelMinimalAnalyzer_cfi")

outputPath = "deltaR.root"
if not(options.outputPath == "none"): outputPath = options.outputPath
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputPath)
)

listOfInputFiles = []
# if not(options.inputPath == "none"):
#     inputFileNamesFileObject = open(options.inputPath, 'r')
#     for inputFileName in inputFileNamesFileObject:
#         if (inputFileName[:5] != "file:" ):
#             listOfInputFiles.append("root://cms-xrd-global.cern.ch/" + inputFileName.strip())
#         else:
#             listOfInputFiles.append(inputFileName.strip())
#     inputFileNamesFileObject.close()
listOfInputFiles.append((options.inputPath).strip())

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(*tuple(listOfInputFiles))
)

process.p = cms.Path(process.genLevelMinimalAnalyzer)
