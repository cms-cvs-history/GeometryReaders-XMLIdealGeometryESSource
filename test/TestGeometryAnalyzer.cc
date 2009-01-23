// -*- C++ -*-
//
// Package:    PerfectGeometryAnalyzer
// Class:      PerfectGeometryAnalyzer
// 
/**\class PerfectGeometryAnalyzer PerfectGeometryAnalyzer.cc test/PerfectGeometryAnalyzer/src/PerfectGeometryAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tommaso Boccali
//         Created:  Tue Jul 26 08:47:57 CEST 2005
// $Id: PerfectGeometryAnalyzer.cc,v 1.13 2007/12/12 09:30:59 muzaffar Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDRoot.h"
#include "DetectorDescription/Parser/interface/DDLParser.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/OfflineDBLoader/interface/GeometryInfoDump.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "CondFormats/GeometryObjects/interface/GeometryFile.h"
#include "Geometry/Records/interface/GeometryFileRcd.h"

//
// class decleration
//

class TestGeometryAnalyzer : public edm::EDAnalyzer {
public:
  explicit TestGeometryAnalyzer( const edm::ParameterSet& );
  ~TestGeometryAnalyzer();

  
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
private:
  // ----------member data ---------------------------
  std::string label_;
  //  bool isMagField_;
  bool dumpHistory_;
  bool dumpPosInfo_;
  bool dumpSpecs_;
  std::string ddRootNodeName_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TestGeometryAnalyzer::TestGeometryAnalyzer( const edm::ParameterSet& iConfig ) :
  label_(iConfig.getUntrackedParameter<std::string>("label","")),
  dumpHistory_(iConfig.getUntrackedParameter<bool>("dumpGeoHistory","")),
  dumpPosInfo_(iConfig.getUntrackedParameter<bool>("dumpPosInfo","")),
  dumpSpecs_(iConfig.getUntrackedParameter<bool>("dumpSpecs","")),
  ddRootNodeName_(iConfig.getParameter<std::string>("ddRootNodeName"))
{
//   if ( isMagField_ ) {
//     label_ = "magfield";
//   }
}


TestGeometryAnalyzer::~TestGeometryAnalyzer()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TestGeometryAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;

   std::cout << "Here I am " << std::endl;
   edm::ESHandle<GeometryFile> gdd;
   iSetup.get<GeometryFileRcd>().get(label_, gdd);
//    std::cout << "first 10 characters..." << std::endl;
//    std::cout << "isValid = " << ( gdd.isValid() ? "true" : "false" ) << std::endl; 
//    if ( gdd.isValid() ) {
//      std::cout << "   compressed = " << ( (*gdd).isCompressed() ? "true" : "false" ) << std::endl;
//      std::cout << "   isize = " << (*gdd).size() << std::endl;
//    }
   DDLParser * parser = DDLParser::instance();
   DDRootDef::instance().set(DDName(ddRootNodeName_));
   std::vector<unsigned char>* tb = (*gdd).getUncompressedBlob();
//    for ( size_t it = tb->size() - 10 ; it < tb->size(); ++it ) {
//      std::cout << (*tb)[it] << " = " << (int) (*tb)[it] << std::endl;
//    }
//    std::cout << "(*gdd).size() = " << (*gdd).size() << "  tb->size() = " << tb->size() << std::endl;
   parser->parse(*tb, tb->size());   
   delete tb;
   
   std::cout << std::endl;
   GeometryInfoDump gidump;
   DDCompactView cpv;
   gidump.dumpInfo( dumpHistory_, dumpSpecs_, dumpPosInfo_, cpv );
   std::cout << "finished" << std::endl;
}


//define this as a plug-in
DEFINE_FWK_MODULE(TestGeometryAnalyzer);
