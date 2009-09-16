import FWCore.ParameterSet.Config as cms

process = cms.Process("DBGeometryTest")
#process.load("DetectorDescription.OfflineDBLoader.test.cmsIdealGeometryForWrite_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )
process.source = cms.Source("EmptySource")

process.myprint = cms.OutputModule("AsciiOutputModule")

process.prod = cms.EDAnalyzer("TestGeometryAnalyzer",
                                  ddRootNodeName = cms.string("cms:OCMS"),
                                  dumpPosInfo = cms.untracked.bool(False),
                                  dumpSpecs = cms.untracked.bool(False),
                                  dumpGeoHistory = cms.untracked.bool(False)
                              )



process.MessageLogger = cms.Service("MessageLogger",
    errors = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR'),
        extension = cms.untracked.string('.out')
    ),
    # No constraint on log content...equivalent to threshold INFO
    # 0 means none, -1 means all (?)
    log = cms.untracked.PSet(
        extension = cms.untracked.string('.out')
    ),
    debug = cms.untracked.PSet(
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        extension = cms.untracked.string('.out'),

        noLineBreaks = cms.untracked.bool(True),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        threshold = cms.untracked.string('DEBUG'),
    ),
    # For LogDebug/LogTrace output...
    debugModules = cms.untracked.vstring('*'),
    categories = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('log', 
        'errors', 
        'debug')
)


process.Timing = cms.Service("Timing")

process.p1 = cms.Path(process.prod)
process.e1 = cms.EndPath(process.myprint)
