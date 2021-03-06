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

// Framework include files
#include "DD4hep/AlignmentData.h"
#include "DD4hep/objects/AlignmentsInterna.h"
#include "DD4hep/objects/ConditionsInterna.h"

// C/C++ include files
#include <sstream>

using namespace std;
using namespace DD4hep::Alignments;

/// Namespace for the AIDA detector description toolkit
namespace DD4hep {
  
  /// Namespace for the alignment part of the AIDA detector description toolkit
  namespace Alignments {

#if defined(__APPLE__) || defined(__clang__)
    /// Initializing constructor to create a new object (Specialized for AlignmentNamedObject)
    Alignment::Alignment(const string& nam)
    { assign(new Interna::AlignmentNamedObject(nam, "alignment"), nam, "alignment");    }

    /// Initializing constructor to create a new object (Specialized for AlignmentConditionObject)
    AlignmentCondition::AlignmentCondition(const string& nam)
    { assign(new AlignmentCondition::Object(nam, "alignment"), nam, "alignment");       }
#else
    /// Initializing constructor to create a new object (Specialized for AlignmentData)
    template <> Alignment::Alignment<AlignmentData>(const string& nam)
    { assign(new AlignmentData(), nam, "alignment");                                    }

    /// Initializing constructor to create a new object (Specialized for AlignmentNamedObject)
    template <> Alignment::Alignment<Interna::AlignmentNamedObject>(const string& nam)
    { assign(new Interna::AlignmentNamedObject(nam, "alignment"), nam, "alignment");    }

    /// Initializing constructor to create a new object (Specialized for AlignmentConditionObject)
    template <> AlignmentCondition::AlignmentCondition<Interna::AlignmentConditionObject>(const string& nam)
    { assign(new Object(nam, "alignment"), nam, "alignment");                           }
#endif
  } /* End namespace Aligments                    */
}   /* End namespace DD4hep                       */


/// Default constructor
Alignment::Processor::Processor() {
}

/// Access the hash identifier
Alignment::key_type Alignment::key()  const   {
  // If the alignment is bound to a condition, the key is the same as the one of the condition.
  // Otherwise the key is invalid and an exception is thrown
  Conditions::Condition cond = access()->condition;
  return cond.access()->hash;
}

/// Access the bound condition if the alignment is bound to a condition. Otherwise NULL.
AlignmentCondition Alignment::condition()  const   {
  return AlignmentCondition(data().condition);
}

/// Create cached matrix to transform to world coordinates
const TGeoHMatrix& Alignment::worldTransformation()  const  {
  return data().worldTransformation();
}

/// Access the alignment/placement matrix with respect to the world
const TGeoHMatrix& Alignment::detectorTransformation() const   {
  return data().detectorTransformation();
}

/// Transformation from local coordinates of the placed volume to the world system
void Alignment::localToWorld(const Position& local, Position& global) const  {
  return access()->localToWorld(local,global);
}

/// Transformation from local coordinates of the placed volume to the world system
void Alignment::localToWorld(const Double_t local[3], Double_t global[3]) const  {
  return access()->localToWorld(local,global);
}
/// Transformation from local coordinates of the placed volume to the world system
Position Alignment::localToWorld(const Position& local) const  {
  return access()->localToWorld(local);
}

/// Transformation from world coordinates of the local placed volume coordinates
void Alignment::worldToLocal(const Position& global, Position& local) const  {
  return access()->worldToLocal(global,local);
}

/// Transformation from world coordinates of the local placed volume coordinates
void Alignment::worldToLocal(const Double_t global[3], Double_t local[3]) const  {
  return access()->worldToLocal(global,local);
}

/// Transformation from local coordinates of the placed volume to the world system
Position Alignment::worldToLocal(const Position& global) const  {
  return access()->worldToLocal(global);
}

/// Transformation from local coordinates of the placed volume to the detector system
void Alignment::localToDetector(const Position& local, Position& detector) const  {
  return access()->localToDetector(local,detector);
}

/// Transformation from local coordinates of the placed volume to the detector system
void Alignment::localToDetector(const Double_t local[3], Double_t detector[3]) const  {
  return access()->localToDetector(local,detector);
}

/// Transformation from local coordinates of the placed volume to the world system
Position Alignment::localToDetector(const Position& local) const  {
  return access()->localToDetector(local);
}

/// Transformation from detector element coordinates to the local placed volume coordinates
void Alignment::detectorToLocal(const Position& detector, Position& local) const  {
  return access()->detectorToLocal(detector,local);
}

/// Transformation from detector element coordinates to the local placed volume coordinates
void Alignment::detectorToLocal(const Double_t detector[3], Double_t local[3]) const  {
  return access()->detectorToLocal(detector,local);
}

/// Transformation from detector element coordinates to the local placed volume coordinates
Position Alignment::detectorToLocal(const Position& detector) const  {
  return access()->detectorToLocal(detector);
}

/// Access the IOV type
const DD4hep::IOVType& AlignmentCondition::iovType() const   {
  return *(access()->iovType());
}

/// Access the IOV block
const DD4hep::IOV& AlignmentCondition::iov() const   {
  return *(access()->iovData());
}

/// Access the hash identifier
AlignmentCondition::key_type AlignmentCondition::key() const   {
  return access()->hash;
}

/// Data accessor for the use of decorators
AlignmentCondition::Data& AlignmentCondition::data()              {
  Object* o = access();
  return o->alignment_data ? *o->alignment_data : o->values();
}

/// Data accessor for the use of decorators
const AlignmentCondition::Data& AlignmentCondition::data() const  {
  Object* o = access();
  return o->alignment_data ? *o->alignment_data : o->values();
}

/// Check if object is already bound....
bool AlignmentCondition::is_bound()  const  {
  return isValid() ? ptr()->data.is_bound() : false;
}

/// Create cached matrix to transform to world coordinates
const TGeoHMatrix& AlignmentCondition::worldTransformation()  const  {
  return data().worldTransformation();
}

/// Access the alignment/placement matrix with respect to the world
const TGeoHMatrix& AlignmentCondition::detectorTransformation() const   {
  return data().detectorTransformation();
}

/// Default constructor
Container::Processor::Processor() {
}

/// Access the number of conditons keys available for this detector element
size_t Container::numKeys() const   {
  return access()->keys.size();
}

/// Known keys of conditions in this container
const Container::Keys& Container::keys()  const   {
  return access()->keys;
}

/// Insert a new key to the alignments access map. Ignores already existing keys.
bool Container::insertKey(const std::string& key_val)  {
  return access()->insertKey(key_val);
}

/// Insert a new key to the alignments access map: Allow for alias if key_val != data_val
bool Container::insertKey(const std::string& key_val, const std::string& data_val)  {
  return access()->insertKey(key_val, data_val);
}

/// Add a new key to the alignments access map
void Container::addKey(const string& key_val)  {
  access()->addKey(key_val);
}

/// Add a new key to the alignments access map: Allow for alias if key_val != data_val
void Container::addKey(const string& key_val, const string& data_val)  {
  access()->addKey(key_val, data_val);
}

/// Access to alignment objects
Alignment Container::get(const string& alignment_key, const iov_type& iov)  {
  return Alignment(access()->get(alignment_key, iov).access());
}

/// Access to alignment objects
Alignment Container::get(key_type alignment_key, const iov_type& iov)  {
  return Alignment(access()->get(alignment_key, iov).access());
}

/// Access to alignment objects
Alignment Container::get(const string& alignment_key, const UserPool& pool)  {
  return Alignment(access()->get(alignment_key, pool).access());
}

/// Access to alignment objects
Alignment Container::get(key_type alignment_key, const UserPool& pool)  {
  return Alignment(access()->get(alignment_key, pool).access());
}
