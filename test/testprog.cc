#include "CondFormats/GeometryObjects/interface/GeometryFile.h"
#include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDRoot.h"

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "about to open geometry file and read it." << std::endl;
  std::string fn("fred.xml");
  GeometryFile* g = new GeometryFile(fn, true);
  // above does a read already...    g->read("fred.xml");
  std::cout << "about to get the parser." << std::endl;
  DDCompactView cpv;
  DDLParser ddparser(cpv);// * ddparser = DDLParser::instance();
  DDRootDef::instance().set(DDName("cms:OCMS"));
  std::cout << "about to try to parse the blob." << std::endl;

  //  std::vector<unsigned char>* tb= g->getUncompressedBlob();
  //  ddparser->parse(g->getBlob(), g->size());
  //  g->write("frednew.xml");
  //  std::cout << "g->size() = " << g->size() << "  tb->size() = " << tb->size() << std::endl;
  //  ddparser->parse(*tb, g->size());
  //  delete tb;
  std::vector<unsigned char> tb;
  g->getUncompressedBlob(tb);
  std::cout << "g->size() = " << g->size() << "  tb.size() = " << tb.size() << std::endl;
  ddparser.parse(tb, g->size());
}
