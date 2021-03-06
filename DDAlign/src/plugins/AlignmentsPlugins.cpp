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
#include "DD4hep/Printout.h"
#include "DD4hep/DetectorTools.h"
#include "DD4hep/DetFactoryHelper.h"
#include "DDCond/ConditionsSlice.h"

using namespace std;
using namespace DD4hep;
using namespace DD4hep::Alignments;
using Conditions::UserPool;
using Conditions::ConditionsSlice;

// ======================================================================================
#include "DDAlign/AlignmentsManager.h"

namespace {
  /// Plugin function:
  /// Install the alignment manager as an extension to the central LCDD object
  int ddalign_install_align_mgr (Geometry::LCDD& lcdd, int argc, char** argv)  {
    Handle<AlignmentsManagerObject> mgr(lcdd.extension<AlignmentsManagerObject>(false));
    if ( !mgr.isValid() )  {
      AlignmentsManager mgr_handle("LCDD_AlignmentManager");
      lcdd.addExtension<AlignmentsManagerObject>(mgr_handle.ptr());
      printout(INFO,"AlignmentsManager",
               "+++ Successfully installed alignments manager instance to LCDD.");
      mgr = mgr_handle;
    }
    if ( argc == 2 )  {
      if ( ::strncmp(argv[0],"-handle",7)==0 )  {
        Handle<NamedObject>* h = (Handle<NamedObject>*)argv[1];
        *h = mgr;
      }
    }
    return 1;
  }
}  /* End anonymous namespace  */
DECLARE_APPLY(DD4hep_AlignmentsManagerInstaller,ddalign_install_align_mgr)

// ======================================================================================
#include "DDAlign/GlobalAlignmentWriter.h"
long create_global_alignment_file(Geometry::LCDD& lcdd, int argc, char** argv)   {
  namespace DetectorTools = DD4hep::Geometry::DetectorTools;
  Geometry::DetElement top;
  string output, path = "/world";
  bool enable_transactions = false, arg_error = false;
  for(int i=1; i<argc;++i) {
    if ( argv[i] && (argv[i][0]=='-' || argv[i][0]=='/') ) {
      const char* p = ::strchr(argv[i],'=');
      if ( p && strncmp(argv[i]+1,"-output",7)==0 )
        output = p+1;
      else if ( p && strncmp(argv[i]+1,"-path",5)==0 )
        path = p+1;
      else if ( strncmp(argv[i]+1,"-transactions",5)==0 )
        enable_transactions = true;
      else
        arg_error = true;
    }
  }

  if ( arg_error || output.empty() || path.empty() )  {
    /// Help printout describing the basic command line interface
    cout <<
      "Usage: -plugin <name> -arg [-arg]                                      \n"
      "     name:   factory nameDD4hep_GlobalAlignmentWriter                \n\n"
      "     -output <string>         Path to the output file generated.       \n"
      "     -path   <string>         Path to the detector element for which   \n"
      "                              the alignment file should be written.    \n"
      "     -transactions            Enable output transactions.              \n"
      "\tArguments given: " << arguments(argc,argv) << endl << flush;
    ::exit(EINVAL);
  }

  printout(ALWAYS,"AlignmentWriter",
           "++ Writing DD4hep alignment constants of the \"%s\" DetElement tree to file \"%s\"",
           path.c_str(), output.c_str());
  top = DetectorTools::findDaughterElement(lcdd.world(),path);
  if ( top.isValid() )   {
    GlobalAlignmentWriter wr(lcdd);
    return wr.write(wr.dump(top,enable_transactions), output);
  }
  except("AlignmentWriter","++ Invalid top level detector element name: %s",path.c_str());
  return 1;
}
DECLARE_APPLY(DD4hep_GlobalAlignmentWriter, create_global_alignment_file)

// ======================================================================================
#include "DDAlign/DDAlignUpdateCall.h"
static void* create_DDAlignUpdateCall(Geometry::LCDD& /* lcdd */, int /* argc */, char** /* argv */)   {
  return (AlignmentsUpdateCall*)(new DDAlignUpdateCall());
}
DECLARE_LCDD_CONSTRUCTOR(DDAlign_UpdateCall, create_DDAlignUpdateCall)

// ======================================================================================
#include "DDAlign/DDAlignForwardCall.h"
static void* create_DDAlignForwardCall(Geometry::LCDD& /* lcdd */, int /* argc */, char** /* argv */)   {
  return (AlignmentsUpdateCall*)(new DDAlignForwardCall());
}
DECLARE_LCDD_CONSTRUCTOR(DDAlign_ForwardCall, create_DDAlignForwardCall)

// ======================================================================================
#include "DDAlign/DDAlignResetCall.h"
static void* create_DDAlignResetCall(Geometry::LCDD& /* lcdd */, int /* argc */, char** /* argv */)   {
  return (AlignmentsUpdateCall*)(new DDAlignResetCall());
}
DECLARE_LCDD_CONSTRUCTOR(DDAlign_ResetCall, create_DDAlignResetCall)

// ======================================================================================
#include "DD4hep/PluginTester.h"
#include "DDCond/ConditionsPool.h"
static long compute_alignments(Geometry::LCDD& lcdd, int /* argc */, char** /* argv */)   {
  AlignmentsManager mgr = AlignmentsManager::from(lcdd);
  PluginTester*     tst = lcdd.extension<PluginTester>();
  ConditionsSlice*  slc = tst->extension<ConditionsSlice>("ConditionsTestSlice");
  mgr.compute(*slc);
  return 1;
}
DECLARE_APPLY(DDAlign_ComputeAlignments, compute_alignments)

// ======================================================================================
#include "DDAlign/AlignmentsRegister.h"
//#include "DD4hep/PluginTester.h"
//#include "DDCond/ConditionsPool.h"
/// Convert alignments conditions to alignment objects
static void* ddalign_AlignmentsRegister(Geometry::LCDD& lcdd, int argc, char** argv)  {
  std::vector<char*> args_prepare, args_call;
  bool arg_error = false;
  for(int i=0; i<argc && argv[i]; ++i)  {
    if ( ::strcmp(argv[i],"-prepare") == 0 )  {
      while( (++i)<argc && argv[i] && 0 != ::strcmp(argv[i],"-prepare-end") )
        args_prepare.push_back(argv[i]);
    }
    else if ( ::strcmp(argv[i],"-call") == 0 )  {
      while( (++i)<argc && argv[i] && 0 != ::strcmp(argv[i],"-call-end") )
        args_call.push_back(argv[i]);
    }
    else  {
      arg_error = true;
    }
  }

  if ( arg_error )  {
    /// Help printout describing the basic command line interface
    cout <<
      "Usage: -plugin <name> -arg [-arg]                                      \n"
      "     name:   factory          DDAlign_AlignmentsRegister             \n\n"
      "     Create and return the AlignmentsRegister object.                  \n"
      "                                                                       \n"
      "     -prepare ... args ... -prepare-end                                \n"
      "                              Arguments to the 'prepare' plugin.       \n"
      "     -call ... args ... -call-end                                      \n"
      "                              Arguments to the 'call' plugin, which    \n"
      "                              create the AlignmentsUpdateCall callback.\n"
      "\tArguments given: " << arguments(argc,argv) << endl << flush;
    ::exit(EINVAL);
  }
  
  PluginTester* test = lcdd.extension<PluginTester>();
  ConditionsSlice* slice = test->extension<ConditionsSlice>("ConditionsTestSlice");
  AlignmentsUpdateCall* call = (AlignmentsUpdateCall*)
    PluginService::Create<void*>((const char*)args_call[0],&lcdd,
                                 int(args_call.size())-1,
                                 (char**)&args_call[1]);
  if ( 0 == call )  {
    except("AlignRegister","++ Failed to create update call!");
  }
  AlignmentsRegister* obj = new AlignmentsRegister(*slice, call);
  return obj;
}
DECLARE_LCDD_CONSTRUCTOR(DDAlign_AlignmentsRegister,ddalign_AlignmentsRegister)

// ======================================================================================
#include "DDAlign/AlignmentsForward.h"
//#include "DD4hep/PluginTester.h"
//#include "DDCond/ConditionsPool.h"
/// Convert alignments conditions to alignment objects
static void* ddalign_AlignmentsForward(Geometry::LCDD& lcdd, int argc, char** argv)  {
  std::vector<char*> args_prepare, args_call;
  bool arg_error = false;
  for(int i=0; i<argc && argv[i]; ++i)  {
    if ( ::strcmp(argv[i],"-call") == 0 )  {
      while( (++i)<argc && argv[i] && 0 != ::strcmp(argv[i],"-call-end") )
        args_call.push_back(argv[i]);
    }
    else  {
      arg_error = true;
    }
  }

  if ( arg_error )  {
    /// Help printout describing the basic command line interface
    cout <<
      "Usage: -plugin <name> -arg [-arg]                                      \n"
      "     name:   factory          DDAlign_AlignmentsForward              \n\n"
      "     Create and return the AlignmentsForward object.                   \n"
      "                                                                       \n"
      "     -call ... args ... -call-end                                      \n"
      "                              Arguments to the 'call' plugin, which    \n"
      "                              create the AlignmentsUpdateCall callback.\n"
      "\tArguments given: " << arguments(argc,argv) << endl << flush;
    ::exit(EINVAL);
  }

  PluginTester* test = lcdd.extension<PluginTester>();
  ConditionsSlice* slice = test->extension<ConditionsSlice>("ConditionsTestSlice");
  AlignmentsUpdateCall*  call = (AlignmentsUpdateCall*)
    PluginService::Create<void*>((const char*)args_call[0],&lcdd,
                                 int(args_call.size())-1,
                                 (char**)&args_call[1]);
  if ( 0 == call )  {
    except("AlignForward","++ Failed to create update call!");
  }
  AlignmentsForward* obj = new AlignmentsForward(*slice, call);
  return obj;
}
DECLARE_LCDD_CONSTRUCTOR(DDAlign_AlignmentsForward,ddalign_AlignmentsForward)

