// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME GramsTofAnalysisDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "TOF_ChannelConversion.h"
#include "TOF_TdcQdcCalibration.h"
#include "TOF_TreeData.h"
#include "TOF_Constants.h"
#include "TOF_Struct.h"
#include "TOF_Attributes.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TOF_ChannelConversion(void *p = nullptr);
   static void *newArray_TOF_ChannelConversion(Long_t size, void *p);
   static void delete_TOF_ChannelConversion(void *p);
   static void deleteArray_TOF_ChannelConversion(void *p);
   static void destruct_TOF_ChannelConversion(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TOF_ChannelConversion*)
   {
      ::TOF_ChannelConversion *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TOF_ChannelConversion >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TOF_ChannelConversion", ::TOF_ChannelConversion::Class_Version(), "TOF_ChannelConversion.h", 7,
                  typeid(::TOF_ChannelConversion), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TOF_ChannelConversion::Dictionary, isa_proxy, 4,
                  sizeof(::TOF_ChannelConversion) );
      instance.SetNew(&new_TOF_ChannelConversion);
      instance.SetNewArray(&newArray_TOF_ChannelConversion);
      instance.SetDelete(&delete_TOF_ChannelConversion);
      instance.SetDeleteArray(&deleteArray_TOF_ChannelConversion);
      instance.SetDestructor(&destruct_TOF_ChannelConversion);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TOF_ChannelConversion*)
   {
      return GenerateInitInstanceLocal(static_cast<::TOF_ChannelConversion*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TOF_ChannelConversion*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TOF_TdcQdcCalibration(void *p = nullptr);
   static void *newArray_TOF_TdcQdcCalibration(Long_t size, void *p);
   static void delete_TOF_TdcQdcCalibration(void *p);
   static void deleteArray_TOF_TdcQdcCalibration(void *p);
   static void destruct_TOF_TdcQdcCalibration(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TOF_TdcQdcCalibration*)
   {
      ::TOF_TdcQdcCalibration *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TOF_TdcQdcCalibration >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TOF_TdcQdcCalibration", ::TOF_TdcQdcCalibration::Class_Version(), "TOF_TdcQdcCalibration.h", 11,
                  typeid(::TOF_TdcQdcCalibration), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TOF_TdcQdcCalibration::Dictionary, isa_proxy, 4,
                  sizeof(::TOF_TdcQdcCalibration) );
      instance.SetNew(&new_TOF_TdcQdcCalibration);
      instance.SetNewArray(&newArray_TOF_TdcQdcCalibration);
      instance.SetDelete(&delete_TOF_TdcQdcCalibration);
      instance.SetDeleteArray(&deleteArray_TOF_TdcQdcCalibration);
      instance.SetDestructor(&destruct_TOF_TdcQdcCalibration);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TOF_TdcQdcCalibration*)
   {
      return GenerateInitInstanceLocal(static_cast<::TOF_TdcQdcCalibration*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TOF_TdcQdcCalibration*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TOF_TreeData(void *p = nullptr);
   static void *newArray_TOF_TreeData(Long_t size, void *p);
   static void delete_TOF_TreeData(void *p);
   static void deleteArray_TOF_TreeData(void *p);
   static void destruct_TOF_TreeData(void *p);
   static void directoryAutoAdd_TOF_TreeData(void *obj, TDirectory *dir);
   static Long64_t merge_TOF_TreeData(void *obj, TCollection *coll,TFileMergeInfo *info);
   static void reset_TOF_TreeData(void *obj, TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TOF_TreeData*)
   {
      ::TOF_TreeData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TOF_TreeData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TOF_TreeData", ::TOF_TreeData::Class_Version(), "TOF_TreeData.h", 9,
                  typeid(::TOF_TreeData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TOF_TreeData::Dictionary, isa_proxy, 4,
                  sizeof(::TOF_TreeData) );
      instance.SetNew(&new_TOF_TreeData);
      instance.SetNewArray(&newArray_TOF_TreeData);
      instance.SetDelete(&delete_TOF_TreeData);
      instance.SetDeleteArray(&deleteArray_TOF_TreeData);
      instance.SetDestructor(&destruct_TOF_TreeData);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_TOF_TreeData);
      instance.SetMerge(&merge_TOF_TreeData);
      instance.SetResetAfterMerge(&reset_TOF_TreeData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TOF_TreeData*)
   {
      return GenerateInitInstanceLocal(static_cast<::TOF_TreeData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TOF_TreeData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TOF_Struct(void *p = nullptr);
   static void *newArray_TOF_Struct(Long_t size, void *p);
   static void delete_TOF_Struct(void *p);
   static void deleteArray_TOF_Struct(void *p);
   static void destruct_TOF_Struct(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TOF_Struct*)
   {
      ::TOF_Struct *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TOF_Struct >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TOF_Struct", ::TOF_Struct::Class_Version(), "TOF_Struct.h", 6,
                  typeid(::TOF_Struct), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TOF_Struct::Dictionary, isa_proxy, 4,
                  sizeof(::TOF_Struct) );
      instance.SetNew(&new_TOF_Struct);
      instance.SetNewArray(&newArray_TOF_Struct);
      instance.SetDelete(&delete_TOF_Struct);
      instance.SetDeleteArray(&deleteArray_TOF_Struct);
      instance.SetDestructor(&destruct_TOF_Struct);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TOF_Struct*)
   {
      return GenerateInitInstanceLocal(static_cast<::TOF_Struct*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TOF_Struct*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TOF_Attributes(void *p = nullptr);
   static void *newArray_TOF_Attributes(Long_t size, void *p);
   static void delete_TOF_Attributes(void *p);
   static void deleteArray_TOF_Attributes(void *p);
   static void destruct_TOF_Attributes(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TOF_Attributes*)
   {
      ::TOF_Attributes *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TOF_Attributes >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TOF_Attributes", ::TOF_Attributes::Class_Version(), "TOF_Attributes.h", 23,
                  typeid(::TOF_Attributes), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TOF_Attributes::Dictionary, isa_proxy, 4,
                  sizeof(::TOF_Attributes) );
      instance.SetNew(&new_TOF_Attributes);
      instance.SetNewArray(&newArray_TOF_Attributes);
      instance.SetDelete(&delete_TOF_Attributes);
      instance.SetDeleteArray(&deleteArray_TOF_Attributes);
      instance.SetDestructor(&destruct_TOF_Attributes);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TOF_Attributes*)
   {
      return GenerateInitInstanceLocal(static_cast<::TOF_Attributes*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TOF_Attributes*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TOF_ChannelConversion::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TOF_ChannelConversion::Class_Name()
{
   return "TOF_ChannelConversion";
}

//______________________________________________________________________________
const char *TOF_ChannelConversion::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_ChannelConversion*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TOF_ChannelConversion::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_ChannelConversion*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TOF_ChannelConversion::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_ChannelConversion*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TOF_ChannelConversion::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_ChannelConversion*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TOF_TdcQdcCalibration::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TOF_TdcQdcCalibration::Class_Name()
{
   return "TOF_TdcQdcCalibration";
}

//______________________________________________________________________________
const char *TOF_TdcQdcCalibration::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_TdcQdcCalibration*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TOF_TdcQdcCalibration::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_TdcQdcCalibration*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TOF_TdcQdcCalibration::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_TdcQdcCalibration*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TOF_TdcQdcCalibration::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_TdcQdcCalibration*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TOF_TreeData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TOF_TreeData::Class_Name()
{
   return "TOF_TreeData";
}

//______________________________________________________________________________
const char *TOF_TreeData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_TreeData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TOF_TreeData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_TreeData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TOF_TreeData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_TreeData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TOF_TreeData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_TreeData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TOF_Struct::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TOF_Struct::Class_Name()
{
   return "TOF_Struct";
}

//______________________________________________________________________________
const char *TOF_Struct::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_Struct*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TOF_Struct::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_Struct*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TOF_Struct::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_Struct*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TOF_Struct::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_Struct*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TOF_Attributes::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TOF_Attributes::Class_Name()
{
   return "TOF_Attributes";
}

//______________________________________________________________________________
const char *TOF_Attributes::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_Attributes*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TOF_Attributes::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TOF_Attributes*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TOF_Attributes::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_Attributes*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TOF_Attributes::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TOF_Attributes*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TOF_ChannelConversion::Streamer(TBuffer &R__b)
{
   // Stream an object of class TOF_ChannelConversion.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TOF_ChannelConversion::Class(),this);
   } else {
      R__b.WriteClassBuffer(TOF_ChannelConversion::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TOF_ChannelConversion(void *p) {
      return  p ? new(p) ::TOF_ChannelConversion : new ::TOF_ChannelConversion;
   }
   static void *newArray_TOF_ChannelConversion(Long_t nElements, void *p) {
      return p ? new(p) ::TOF_ChannelConversion[nElements] : new ::TOF_ChannelConversion[nElements];
   }
   // Wrapper around operator delete
   static void delete_TOF_ChannelConversion(void *p) {
      delete (static_cast<::TOF_ChannelConversion*>(p));
   }
   static void deleteArray_TOF_ChannelConversion(void *p) {
      delete [] (static_cast<::TOF_ChannelConversion*>(p));
   }
   static void destruct_TOF_ChannelConversion(void *p) {
      typedef ::TOF_ChannelConversion current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TOF_ChannelConversion

//______________________________________________________________________________
void TOF_TdcQdcCalibration::Streamer(TBuffer &R__b)
{
   // Stream an object of class TOF_TdcQdcCalibration.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TOF_TdcQdcCalibration::Class(),this);
   } else {
      R__b.WriteClassBuffer(TOF_TdcQdcCalibration::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TOF_TdcQdcCalibration(void *p) {
      return  p ? new(p) ::TOF_TdcQdcCalibration : new ::TOF_TdcQdcCalibration;
   }
   static void *newArray_TOF_TdcQdcCalibration(Long_t nElements, void *p) {
      return p ? new(p) ::TOF_TdcQdcCalibration[nElements] : new ::TOF_TdcQdcCalibration[nElements];
   }
   // Wrapper around operator delete
   static void delete_TOF_TdcQdcCalibration(void *p) {
      delete (static_cast<::TOF_TdcQdcCalibration*>(p));
   }
   static void deleteArray_TOF_TdcQdcCalibration(void *p) {
      delete [] (static_cast<::TOF_TdcQdcCalibration*>(p));
   }
   static void destruct_TOF_TdcQdcCalibration(void *p) {
      typedef ::TOF_TdcQdcCalibration current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TOF_TdcQdcCalibration

//______________________________________________________________________________
void TOF_TreeData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TOF_TreeData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TOF_TreeData::Class(),this);
   } else {
      R__b.WriteClassBuffer(TOF_TreeData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TOF_TreeData(void *p) {
      return  p ? new(p) ::TOF_TreeData : new ::TOF_TreeData;
   }
   static void *newArray_TOF_TreeData(Long_t nElements, void *p) {
      return p ? new(p) ::TOF_TreeData[nElements] : new ::TOF_TreeData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TOF_TreeData(void *p) {
      delete (static_cast<::TOF_TreeData*>(p));
   }
   static void deleteArray_TOF_TreeData(void *p) {
      delete [] (static_cast<::TOF_TreeData*>(p));
   }
   static void destruct_TOF_TreeData(void *p) {
      typedef ::TOF_TreeData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_TOF_TreeData(void *p, TDirectory *dir) {
      ((::TOF_TreeData*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t merge_TOF_TreeData(void *obj,TCollection *coll,TFileMergeInfo *info) {
      return ((::TOF_TreeData*)obj)->Merge(coll,info);
   }
   // Wrapper around the Reset function.
   static void reset_TOF_TreeData(void *obj,TFileMergeInfo *info) {
      ((::TOF_TreeData*)obj)->ResetAfterMerge(info);
   }
} // end of namespace ROOT for class ::TOF_TreeData

//______________________________________________________________________________
void TOF_Struct::Streamer(TBuffer &R__b)
{
   // Stream an object of class TOF_Struct.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TOF_Struct::Class(),this);
   } else {
      R__b.WriteClassBuffer(TOF_Struct::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TOF_Struct(void *p) {
      return  p ? new(p) ::TOF_Struct : new ::TOF_Struct;
   }
   static void *newArray_TOF_Struct(Long_t nElements, void *p) {
      return p ? new(p) ::TOF_Struct[nElements] : new ::TOF_Struct[nElements];
   }
   // Wrapper around operator delete
   static void delete_TOF_Struct(void *p) {
      delete (static_cast<::TOF_Struct*>(p));
   }
   static void deleteArray_TOF_Struct(void *p) {
      delete [] (static_cast<::TOF_Struct*>(p));
   }
   static void destruct_TOF_Struct(void *p) {
      typedef ::TOF_Struct current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TOF_Struct

//______________________________________________________________________________
void TOF_Attributes::Streamer(TBuffer &R__b)
{
   // Stream an object of class TOF_Attributes.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TOF_Attributes::Class(),this);
   } else {
      R__b.WriteClassBuffer(TOF_Attributes::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TOF_Attributes(void *p) {
      return  p ? new(p) ::TOF_Attributes : new ::TOF_Attributes;
   }
   static void *newArray_TOF_Attributes(Long_t nElements, void *p) {
      return p ? new(p) ::TOF_Attributes[nElements] : new ::TOF_Attributes[nElements];
   }
   // Wrapper around operator delete
   static void delete_TOF_Attributes(void *p) {
      delete (static_cast<::TOF_Attributes*>(p));
   }
   static void deleteArray_TOF_Attributes(void *p) {
      delete [] (static_cast<::TOF_Attributes*>(p));
   }
   static void destruct_TOF_Attributes(void *p) {
      typedef ::TOF_Attributes current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TOF_Attributes

namespace ROOT {
   static TClass *vectorlETOF_TdcQdcCalibrationgR_Dictionary();
   static void vectorlETOF_TdcQdcCalibrationgR_TClassManip(TClass*);
   static void *new_vectorlETOF_TdcQdcCalibrationgR(void *p = nullptr);
   static void *newArray_vectorlETOF_TdcQdcCalibrationgR(Long_t size, void *p);
   static void delete_vectorlETOF_TdcQdcCalibrationgR(void *p);
   static void deleteArray_vectorlETOF_TdcQdcCalibrationgR(void *p);
   static void destruct_vectorlETOF_TdcQdcCalibrationgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TOF_TdcQdcCalibration>*)
   {
      vector<TOF_TdcQdcCalibration> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TOF_TdcQdcCalibration>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TOF_TdcQdcCalibration>", -2, "vector", 389,
                  typeid(vector<TOF_TdcQdcCalibration>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETOF_TdcQdcCalibrationgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TOF_TdcQdcCalibration>) );
      instance.SetNew(&new_vectorlETOF_TdcQdcCalibrationgR);
      instance.SetNewArray(&newArray_vectorlETOF_TdcQdcCalibrationgR);
      instance.SetDelete(&delete_vectorlETOF_TdcQdcCalibrationgR);
      instance.SetDeleteArray(&deleteArray_vectorlETOF_TdcQdcCalibrationgR);
      instance.SetDestructor(&destruct_vectorlETOF_TdcQdcCalibrationgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TOF_TdcQdcCalibration> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<TOF_TdcQdcCalibration>","std::__1::vector<TOF_TdcQdcCalibration, std::__1::allocator<TOF_TdcQdcCalibration>>"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TOF_TdcQdcCalibration>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETOF_TdcQdcCalibrationgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TOF_TdcQdcCalibration>*>(nullptr))->GetClass();
      vectorlETOF_TdcQdcCalibrationgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETOF_TdcQdcCalibrationgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETOF_TdcQdcCalibrationgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_TdcQdcCalibration> : new vector<TOF_TdcQdcCalibration>;
   }
   static void *newArray_vectorlETOF_TdcQdcCalibrationgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_TdcQdcCalibration>[nElements] : new vector<TOF_TdcQdcCalibration>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETOF_TdcQdcCalibrationgR(void *p) {
      delete (static_cast<vector<TOF_TdcQdcCalibration>*>(p));
   }
   static void deleteArray_vectorlETOF_TdcQdcCalibrationgR(void *p) {
      delete [] (static_cast<vector<TOF_TdcQdcCalibration>*>(p));
   }
   static void destruct_vectorlETOF_TdcQdcCalibrationgR(void *p) {
      typedef vector<TOF_TdcQdcCalibration> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TOF_TdcQdcCalibration>

namespace ROOT {
   static TClass *vectorlETOF_StructgR_Dictionary();
   static void vectorlETOF_StructgR_TClassManip(TClass*);
   static void *new_vectorlETOF_StructgR(void *p = nullptr);
   static void *newArray_vectorlETOF_StructgR(Long_t size, void *p);
   static void delete_vectorlETOF_StructgR(void *p);
   static void deleteArray_vectorlETOF_StructgR(void *p);
   static void destruct_vectorlETOF_StructgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TOF_Struct>*)
   {
      vector<TOF_Struct> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TOF_Struct>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TOF_Struct>", -2, "vector", 389,
                  typeid(vector<TOF_Struct>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETOF_StructgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TOF_Struct>) );
      instance.SetNew(&new_vectorlETOF_StructgR);
      instance.SetNewArray(&newArray_vectorlETOF_StructgR);
      instance.SetDelete(&delete_vectorlETOF_StructgR);
      instance.SetDeleteArray(&deleteArray_vectorlETOF_StructgR);
      instance.SetDestructor(&destruct_vectorlETOF_StructgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TOF_Struct> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<TOF_Struct>","std::__1::vector<TOF_Struct, std::__1::allocator<TOF_Struct>>"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TOF_Struct>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETOF_StructgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TOF_Struct>*>(nullptr))->GetClass();
      vectorlETOF_StructgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETOF_StructgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETOF_StructgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_Struct> : new vector<TOF_Struct>;
   }
   static void *newArray_vectorlETOF_StructgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_Struct>[nElements] : new vector<TOF_Struct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETOF_StructgR(void *p) {
      delete (static_cast<vector<TOF_Struct>*>(p));
   }
   static void deleteArray_vectorlETOF_StructgR(void *p) {
      delete [] (static_cast<vector<TOF_Struct>*>(p));
   }
   static void destruct_vectorlETOF_StructgR(void *p) {
      typedef vector<TOF_Struct> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TOF_Struct>

namespace ROOT {
   static TClass *vectorlETOF_ChannelConversiongR_Dictionary();
   static void vectorlETOF_ChannelConversiongR_TClassManip(TClass*);
   static void *new_vectorlETOF_ChannelConversiongR(void *p = nullptr);
   static void *newArray_vectorlETOF_ChannelConversiongR(Long_t size, void *p);
   static void delete_vectorlETOF_ChannelConversiongR(void *p);
   static void deleteArray_vectorlETOF_ChannelConversiongR(void *p);
   static void destruct_vectorlETOF_ChannelConversiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TOF_ChannelConversion>*)
   {
      vector<TOF_ChannelConversion> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TOF_ChannelConversion>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TOF_ChannelConversion>", -2, "vector", 389,
                  typeid(vector<TOF_ChannelConversion>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETOF_ChannelConversiongR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TOF_ChannelConversion>) );
      instance.SetNew(&new_vectorlETOF_ChannelConversiongR);
      instance.SetNewArray(&newArray_vectorlETOF_ChannelConversiongR);
      instance.SetDelete(&delete_vectorlETOF_ChannelConversiongR);
      instance.SetDeleteArray(&deleteArray_vectorlETOF_ChannelConversiongR);
      instance.SetDestructor(&destruct_vectorlETOF_ChannelConversiongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TOF_ChannelConversion> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<TOF_ChannelConversion>","std::__1::vector<TOF_ChannelConversion, std::__1::allocator<TOF_ChannelConversion>>"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TOF_ChannelConversion>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETOF_ChannelConversiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TOF_ChannelConversion>*>(nullptr))->GetClass();
      vectorlETOF_ChannelConversiongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETOF_ChannelConversiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETOF_ChannelConversiongR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_ChannelConversion> : new vector<TOF_ChannelConversion>;
   }
   static void *newArray_vectorlETOF_ChannelConversiongR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_ChannelConversion>[nElements] : new vector<TOF_ChannelConversion>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETOF_ChannelConversiongR(void *p) {
      delete (static_cast<vector<TOF_ChannelConversion>*>(p));
   }
   static void deleteArray_vectorlETOF_ChannelConversiongR(void *p) {
      delete [] (static_cast<vector<TOF_ChannelConversion>*>(p));
   }
   static void destruct_vectorlETOF_ChannelConversiongR(void *p) {
      typedef vector<TOF_ChannelConversion> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TOF_ChannelConversion>

namespace ROOT {
   static TClass *vectorlETOF_AttributesgR_Dictionary();
   static void vectorlETOF_AttributesgR_TClassManip(TClass*);
   static void *new_vectorlETOF_AttributesgR(void *p = nullptr);
   static void *newArray_vectorlETOF_AttributesgR(Long_t size, void *p);
   static void delete_vectorlETOF_AttributesgR(void *p);
   static void deleteArray_vectorlETOF_AttributesgR(void *p);
   static void destruct_vectorlETOF_AttributesgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TOF_Attributes>*)
   {
      vector<TOF_Attributes> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TOF_Attributes>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TOF_Attributes>", -2, "vector", 389,
                  typeid(vector<TOF_Attributes>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETOF_AttributesgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TOF_Attributes>) );
      instance.SetNew(&new_vectorlETOF_AttributesgR);
      instance.SetNewArray(&newArray_vectorlETOF_AttributesgR);
      instance.SetDelete(&delete_vectorlETOF_AttributesgR);
      instance.SetDeleteArray(&deleteArray_vectorlETOF_AttributesgR);
      instance.SetDestructor(&destruct_vectorlETOF_AttributesgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TOF_Attributes> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<TOF_Attributes>","std::__1::vector<TOF_Attributes, std::__1::allocator<TOF_Attributes>>"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TOF_Attributes>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETOF_AttributesgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TOF_Attributes>*>(nullptr))->GetClass();
      vectorlETOF_AttributesgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETOF_AttributesgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETOF_AttributesgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_Attributes> : new vector<TOF_Attributes>;
   }
   static void *newArray_vectorlETOF_AttributesgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<TOF_Attributes>[nElements] : new vector<TOF_Attributes>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETOF_AttributesgR(void *p) {
      delete (static_cast<vector<TOF_Attributes>*>(p));
   }
   static void deleteArray_vectorlETOF_AttributesgR(void *p) {
      delete [] (static_cast<vector<TOF_Attributes>*>(p));
   }
   static void destruct_vectorlETOF_AttributesgR(void *p) {
      typedef vector<TOF_Attributes> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TOF_Attributes>

namespace {
  void TriggerDictionaryInitialization_libGramsTofAnalysisDict_Impl() {
    static const char* headers[] = {
"TOF_ChannelConversion.h",
"TOF_TdcQdcCalibration.h",
"TOF_TreeData.h",
"TOF_Constants.h",
"TOF_Struct.h",
"TOF_Attributes.h",
nullptr
    };
    static const char* includePaths[] = {
"/Users/hyebin/work/software/root_v6.32.12/include",
"/Users/hyebin/work/grams/software/grams_tof_lib_hyeb/Analysis",
"/Users/hyebin/work/software/root_v6.32.12/include/",
"/Users/hyebin/work/grams/software/grams_tof_lib_hyeb/build00/Analysis/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libGramsTofAnalysisDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TOF_Attributes.h")))  TOF_Attributes;
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$__memory/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
class __attribute__((annotate("$clingAutoload$TOF_Struct.h")))  TOF_Struct;
class __attribute__((annotate("$clingAutoload$TOF_TdcQdcCalibration.h")))  TOF_TdcQdcCalibration;
class __attribute__((annotate("$clingAutoload$TOF_ChannelConversion.h")))  TOF_ChannelConversion;
class __attribute__((annotate("$clingAutoload$TOF_TreeData.h")))  TOF_TreeData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libGramsTofAnalysisDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TOF_ChannelConversion.h"
#include "TOF_TdcQdcCalibration.h"
#include "TOF_TreeData.h"
#include "TOF_Constants.h"
#include "TOF_Struct.h"
#include "TOF_Attributes.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TOF_Attributes", payloadCode, "@",
"TOF_ChannelConversion", payloadCode, "@",
"TOF_Struct", payloadCode, "@",
"TOF_TdcQdcCalibration", payloadCode, "@",
"TOF_TreeData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libGramsTofAnalysisDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libGramsTofAnalysisDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libGramsTofAnalysisDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libGramsTofAnalysisDict() {
  TriggerDictionaryInitialization_libGramsTofAnalysisDict_Impl();
}
