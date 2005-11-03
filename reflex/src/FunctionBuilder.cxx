// @(#)root/reflex:$Name:$:$Id:$
// Author: Stefan Roiser 2004

// Copyright CERN, CH-1211 Geneva 23, 2004-2005, All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#include "Reflex/Builder/FunctionBuilder.h"

#include "Reflex/PropertyList.h"
#include "Reflex/Scope.h"
#include "Reflex/Any.h"
#include "Reflex/Type.h"
#include "Reflex/Tools.h"

#include "FunctionMember.h"
#include "FunctionMemberTemplateInstance.h"
#include "Namespace.h"


//-------------------------------------------------------------------------------
ROOT::Reflex::FunctionBuilderImpl::FunctionBuilderImpl( const char * nam, 
                                                        const Type & typ,
                                                        StubFunction stubFP,
                                                        void * stubCtx,
                                                        const char * params, 
                                                        unsigned char modifiers) 
//-------------------------------------------------------------------------------
  : fFunction( Member(0)) {

  std::string fullname( nam );
  std::string declScope;
  std::string funcName;
  size_t pos = Tools::GetTemplateName(nam).rfind( "::" );
  // Name contains declaring ScopeNth
  if ( pos != std::string::npos ) {   
    funcName  = fullname.substr( pos + 2 );
    declScope = fullname.substr( 0, pos ); 
  }
  else {
    funcName  = nam;
    declScope = "";
  }

  Scope sc = Scope::ByName(declScope);
  if ( ! sc ) {
    // Let's create the namespace here
    sc = (new Namespace(declScope.c_str()))->ScopeGet();
  }

  if ( ! sc.IsNamespace() ) throw RuntimeError("Declaring scope is not a namespace");
  if ( Tools::IsTemplated( funcName.c_str())) fFunction = Member( new FunctionMemberTemplateInstance( funcName.c_str(),
                                                                                                      typ,
                                                                                                      stubFP,
                                                                                                      stubCtx,
                                                                                                      params,
                                                                                                      modifiers | STATIC,
                                                                                                      sc ));
  else                                        fFunction = Member(new FunctionMember(funcName.c_str(), 
                                                                                    typ, 
                                                                                    stubFP, 
                                                                                    stubCtx, 
                                                                                    params, 
                                                                                    modifiers  | STATIC));
  sc.AddFunctionMember(fFunction);
}


//-------------------------------------------------------------------------------
ROOT::Reflex::FunctionBuilderImpl::~FunctionBuilderImpl() {
//-------------------------------------------------------------------------------
  FireFunctionCallback( fFunction );
}
 

//-------------------------------------------------------------------------------
void ROOT::Reflex::FunctionBuilderImpl::AddProperty( const char * key, 
                                                     const char * value ) {
//-------------------------------------------------------------------------------
   fFunction.PropertyListGet().AddProperty( key , value );
}


//-------------------------------------------------------------------------------
void ROOT::Reflex::FunctionBuilderImpl::AddProperty( const char * key, 
                                                     Any value ) {
//-------------------------------------------------------------------------------
  fFunction.PropertyListGet().AddProperty( key , value );
}


//-------------------------------------------------------------------------------
ROOT::Reflex::FunctionBuilder::FunctionBuilder( const Type & typ,
                                                const char * nam, 
                                                StubFunction stubFP,
                                                void * stubCtx,
                                                const char * params, 
                                                unsigned char modifiers) 
//-------------------------------------------------------------------------------
  : fFunction(Member(0)) {

  std::string fullname( nam );
  std::string declScope;
  std::string funcName;
  size_t pos = Tools::GetTemplateName( nam ).rfind( "::" );
  // Name contains declaring scope
  if ( pos != std::string::npos ) {   
    funcName  = fullname.substr( pos + 2 );
    declScope = fullname.substr( 0, pos ); 
  }
  else {
    funcName  = nam;
    declScope = "";
  }
  Scope sc = Scope::ByName(declScope);
  if ( ! sc ) {
    // Let's create the namespace here
    sc = (new Namespace(declScope.c_str()))->ScopeGet();
  }
  if ( ! sc.IsNamespace() ) throw RuntimeError("2Declaring ScopeNth is not a namespace");
  if ( Tools::IsTemplated( funcName.c_str())) fFunction = Member( new FunctionMemberTemplateInstance( funcName.c_str(),
                                                                                                      typ,
                                                                                                      stubFP,
                                                                                                      stubCtx,
                                                                                                      params,
                                                                                                      modifiers | STATIC,
                                                                                                      sc ));
  else                                 fFunction = Member(new FunctionMember( funcName.c_str(), 
                                                                              typ, 
                                                                              stubFP, 
                                                                              stubCtx, 
                                                                              params, 
                                                                              modifiers  | STATIC));
  sc.AddFunctionMember(fFunction);
}

