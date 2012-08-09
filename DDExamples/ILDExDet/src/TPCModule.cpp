// $Id:$
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Author     : A.Muennich
//
//  This is a special implementation for a FixedAnglePadLayout!
//  Ideally this will become a purely virtual interface from which
//  to inherit the special implementations of different PadLayouts.
//
//====================================================================
#include "DD4hep/LCDD.h"

#include "TPCModule.h"
#include "TPCModuleData.h"
#include "TGeoTube.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace DD4hep {
  
  using namespace Geometry;

  void TPCModule::getExtension() {
    padLayout = isValid() ? extension<PadLayout>() : 0;
  }
  
  int TPCModule::getID()const {
    return _data().id;
  }
  
 }