#==========================================================================
#  AIDA Detector description implementation for LCD
#--------------------------------------------------------------------------
# Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
# All rights reserved.
#
# For the licensing terms see $DD4hepINSTALL/LICENSE.
# For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
#
#==========================================================================
dd4hep_package(    DDEve
  USES             DDCore [ROOT REQUIRED COMPONENTS Eve Gui Graf3d RGL EG]
  INCLUDE_DIRS     include
  INSTALL_INCLUDES include/DDEve)
#
#---Add Plugin Library-----------------------------------------------------
dd4hep_add_dictionary ( G__DDEve SOURCES include/DDEve/*.h )
#
dd4hep_add_plugin ( DDEve SOURCES src/*.cpp 
  GENERATED   G__DDEve.cxx
  OPTIONAL    [LCIO EXTERNAL SOURCES lcio/LCIOEventHandler.cpp] )

if(DD4HEP_USE_GEANT4)
  dd4hep_add_dictionary ( G__DDG4IO
    SOURCES DDEve/DDG4IO.C
    OPTIONAL DDG4
    )
  dd4hep_add_regular_library ( DDG4IO 
    SOURCES   DDEve/IO.cpp
    GENERATED G__DDG4IO.cxx
    OPTIONAL DDG4
    )
endif()

#---Package installation procedure(s) -------------------------------------
dd4hep_install_dir ( DDEve DESTINATION examples )
