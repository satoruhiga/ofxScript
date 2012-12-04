#pragma once

#include "ofMain.h"

#include "angelscript.h"

namespace ofxScript
{
template <typename T>
void construct(void* address)
{
	new (address) T();
}

template <typename T, typename P0>
void construct(void* address, P0 p0)
{
	new (address) T(p0);
}

template <typename T, typename P0, typename P1>
void construct(void* address, P0 p0, P1 p1)
{
	new (address) T(p0, p1);
}
	
template <typename T, typename P0, typename P1, typename P2>
void construct(void* address, P0 p0, P1 p1, P2 p2)
{
	new (address) T(p0, p1, p2);
}

template <typename T, typename P0, typename P1, typename P2, typename P3>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3)
{
	new (address) T(p0, p1, p2, p3);
}
	
template <typename T, typename P0, typename P1, typename P2, typename P3, typename P4>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
{
	new (address) T(p0, p1, p2, p3, p4);
}

template <typename T, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	new (address) T(p0, p1, p2, p3, p4, p5);
}

template <typename T, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	new (address) T(p0, p1, p2, p3, p4, p5, p6);
}

template <typename T, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
	new (address) T(p0, p1, p2, p3, p4, p5, p6, p7);
}

template <typename T, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
void construct(void* address, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
	new (address) T(p0, p1, p2, p3, p4, p5, p6, p7, p8);
}

template <typename T>
void destroy(T* object)
{
	object->~T();
}

template <typename T>
void copy_construct(void* address, T* other)
{
	new (address)T(*other);
}

template <typename T>
void assign(T * lhs, T* rhs)
{
	*lhs = *rhs;
}
}

void ofxScriptBindEngine(asIScriptEngine *engine);
void ofxScriptUnbindEngine();

void ofxScriptPushNamespace(string ns);
void ofxScriptPopNamespace();

extern asIScriptEngine *__engine;

#define ofxScriptAssert(code) \
{ \
	if ((code) < 0) \
	{ \
		ofLogError("ofxScript") << "Assertion failed at: " __FILE__ << ":" << __LINE__ << " @ " << __PRETTY_FUNCTION__ << "\n\tcode = " << #code; \
	} \
}

#define ofxScriptRegisterEnum(enum_name) \
{ \
	ofxScriptAssert(__engine->RegisterEnum(#enum_name)); \
}

#define ofxScriptRegisterEnumValue(enum_name, value) \
{ \
	ofxScriptAssert(__engine->RegisterEnumValue(#enum_name, #value, value)); \
}

#define ofxScriptRegisterGlobalFunc(return_type, method_name, params) \
{ \
	ofxScriptAssert(__engine->RegisterGlobalFunction(#return_type " " #method_name " " #params, asFUNCTIONPR(method_name, params, return_type), asCALL_CDECL)); \
}

#define ofxScriptRegisterGlobalFunc_P(return_type, method_name, params, params_string) \
{ \
	ofxScriptAssert(__engine->RegisterGlobalFunction(#return_type " " #method_name " " params_string, asFUNCTIONPR(method_name, params, return_type), asCALL_CDECL));	\
}

#define ofxScriptRegisterClass(class) \
{ \
	ofxScriptAssert(__engine->RegisterObjectType(#class, sizeof(class), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));	\
}

#define ofxScriptRegisterConstruct(class) \
{ \
	ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(ofxScript::construct<class>), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam1(class, P0) \
{ \
	ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 ")", asFUNCTION((ofxScript::construct<class, P0>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam2(class, P0, P1) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 ")", asFUNCTION((ofxScript::construct<class, P0, P1>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam3(class, P0, P1, P2) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam4(class, P0, P1, P2, P3) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 "," #P3 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2, P3>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam5(class, P0, P1, P2, P3, P4) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 "," #P3 "," #P4 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2, P3, P4>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam6(class, P0, P1, P2, P3, P4, P5) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 "," #P3 "," #P4 "," #P5 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2, P3, P4, P5>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam7(class, P0, P1, P2, P3, P4, P5, P6) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 "," #P3 "," #P4 "," #P5 "," #P6 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2, P3, P4, P5, P6>)), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterConstructWithParam8(class, P0, P1, P2, P3, P4, P5, P6, P7) \
{ \
ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(" #P0 "," #P1 "," #P2 "," #P3 "," #P4 "," #P5 "," #P6 "," #P7 ")", asFUNCTION((ofxScript::construct<class, P0, P1, P2, P3, P4, P5, P6, P7>)), asCALL_CDECL_OBJFIRST)); \
}


#define ofxScriptRegisterDestruct(class) \
{ \
	ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(ofxScript::destroy<class>), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterCopyConstruct(class) \
{ \
	ofxScriptAssert(__engine->RegisterObjectBehaviour(#class, asBEHAVE_CONSTRUCT,  "void f(const " #class " &copy)", asFUNCTION(ofxScript::copy_construct<class>), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterAssign(class)	\
{ \
	ofxScriptAssert(__engine->RegisterObjectMethod(#class, #class " & opAssign(const " #class " &copy)", asFUNCTION(ofxScript::assign<class>), asCALL_CDECL_OBJFIRST)); \
}

#define ofxScriptRegisterClassCDEC(class) \
{ \
	ofxScriptRegisterClass(class);	\
	ofxScriptRegisterConstruct(class);	\
	ofxScriptRegisterDestruct(class); \
	ofxScriptRegisterCopyConstruct(class);	\
	ofxScriptRegisterAssign(class); \
}

#define ofxScriptRegisterObjectMethod(class, return_type, method_name, params) \
{ \
	ofxScriptAssert(__engine->RegisterObjectMethod(#class, #return_type " " #method_name " " #params, asMETHODPR(class, method_name, params, return_type), asCALL_THISCALL)); \
}

#define ofxScriptRegisterObjectMethod_P(class, return_type, method_name, params, params_string) \
{ \
	ofxScriptAssert(__engine->RegisterObjectMethod(#class, #return_type " " #method_name " " params_string, asMETHODPR(class, method_name, params, return_type), asCALL_THISCALL)); \
}

#define ofxScriptRegisterObjectStaticMethod(class, return_type, method_name, params) \
{ \
	ofxScriptPushNamespace(#class); \
	ofxScriptAssert(__engine->RegisterGlobalFunction(#return_type " " #class "::" #method_name " " #params, asFUNCTIONPR(class::method_name, params, return_type), asCALL_CDECL)); \
	ofxScriptPopNamespace(); \
}

#define ofxScriptRegisterObjectStaticMethod_P(class, return_type, method_name, params, params_string) \
{ \
	ofxScriptPushNamespace(#class); \
	ofxScriptAssert(__engine->RegisterGlobalFunction(#return_type " " #class "::" #method_name " " params_string, asFUNCTIONPR(class::method_name, params, return_type), asCALL_CDECL));	\
	ofxScriptPopNamespace(); \
}

#define ofxScriptRegisterObjectProperty(class, type, name) \
{ \
	ofxScriptAssert(__engine->RegisterObjectProperty(#class, #type " " #name, asOFFSET(class, name))); \
}
