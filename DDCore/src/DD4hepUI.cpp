//==========================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

// Framework includes
#include "DD4hep/DD4hepUI.h"
#include "DD4hep/Printout.h"

using namespace std;
using namespace DD4hep;
using namespace DD4hep::Geometry;

/// Default constructor
DD4hepUI::DD4hepUI(LCDD& instance) : m_lcdd(instance)  {
}

/// Default destructor
DD4hepUI::~DD4hepUI()   {
}

/// Access to the LCDD instance
LCDD* DD4hepUI::instance()  const   {
  return &m_lcdd;
}

/// Access to the LCDD instance
LCDD* DD4hepUI::lcdd()  const   {
  return &m_lcdd;
}

/// Install the DD4hep conditions manager object
Handle<NamedObject> DD4hepUI::conditionsMgr()  const  {
  if ( !m_condMgr.isValid() )  {
    const void* argv[] = {"-handle",&m_condMgr,0};
    if ( 1 != apply("DD4hep_ConditionsManagerInstaller",2,(char**)argv) )  {
      except("DD4hepUI","Failed to install the conditions manager object.");
    }
    if ( !m_condMgr.isValid() )  {
      except("DD4hepUI","Failed to access the conditions manager object.");
    }
  }
  return m_condMgr;
}

/// Load conditions from file
long DD4hepUI::loadConditions(const std::string& fname)  const  {
  Handle<NamedObject> h = conditionsMgr();
  if ( h.isValid() )  {
    m_lcdd.fromXML(fname, BUILD_DEFAULT);
    return 1;
  }
  return 0;
}

/// Install the DD4hep alignment manager object
Handle<NamedObject> DD4hepUI::alignmentMgr()  const  {
  if ( !m_alignMgr.isValid() )  {
    const void* argv[] = {"-handle",&m_alignMgr,0};
    if ( 1 != apply("DD4hep_AlignmentsManagerInstaller",2,(char**)argv) )  {
      except("DD4hepUI","Failed to install the alignment manager object.");
    }
    if ( !m_alignMgr.isValid() )  {
      except("DD4hepUI","Failed to access the alignment manager object.");
    }
  }
  return m_alignMgr;
}

/// LCDD interface: Manipulate geometry using facroy converter
long DD4hepUI::apply(const char* factory, int argc, char** argv) const   {
  return m_lcdd.apply(factory,argc,argv);
}

/// LCDD interface: Read any geometry description or alignment file
void DD4hepUI::fromXML(const std::string& fname, LCDDBuildType type) const  {
  return m_lcdd.fromXML(fname, type);
}

/// LCDD interface: Re-draw the entire scene
void DD4hepUI::redraw() const   {
  m_lcdd.worldVolume()->Draw("oglsame");
}

/// Dump the volume tree
long DD4hepUI::dumpVols(int argc, char** argv)  const   {
  if ( argc==0 )  {
    const void* av[] = {"-positions","-pointers",0};
    return m_lcdd.apply("DD4hepVolumeDump",2,(char**)av);
  }
  return m_lcdd.apply("DD4hepVolumeDump",argc,argv);
}

/// Dump the DetElement tree
long DD4hepUI::dumpDet()  const   {
  return m_lcdd.apply("DD4hepDetectorVolumeDump",0,0);
}
