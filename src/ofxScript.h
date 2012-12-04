#pragma once

#include "ofMain.h"

#include "ofxScriptHelper.h"

#include "angelscript.h"
#include "stdvector.h"

class asIScriptEngine;
class asIScriptContext;
class asIScriptFunction;

namespace ofxScript
{

class Engine;
	
#pragma mark - Function
	
class Function
{
public:
	
	Function() : engine(NULL), func(NULL), ctx(NULL) {}
	Function(Engine *engine, asIScriptFunction *func);
	~Function();
	
	Function(const Function& copy) { *this = copy; }
	Function& operator=(const Function& copy);

	template <typename T> inline Function& addArg(const T&) { throw; }
	inline Function& addRef(void *ptr);
	
	template <typename T> inline void getReturnValue(T&) {}
	
	bool prepare();
	bool call();
	
private:
	
	int arg_index;
	bool has_error;
	
	Engine *engine;
	asIScriptFunction *func;
	asIScriptContext *ctx;
};
	
#pragma mark - Engine
	
class Engine
{
	friend class Function;
	
public:
	
	Engine() : engine(NULL) {}
	virtual ~Engine();
	
	bool setup();
	void dispose();
	
	bool compile(const string& path);
	bool compileString(const string& src);
	
	template<typename T>
	bool registerGlobalProperty(const string& type, T& value)
	{
		assert(engine);
		
		int r;
		r = engine->RegisterGlobalProperty(type.c_str(), &value);
	}
	
	bool hasFunction(const string& name);
	Function getFunction(const string& name);
	
	asIScriptEngine* getEngine() { return engine; };
	
	void onExit(ofEventArgs&);
	
	operator bool() { return engine != NULL; }
	
protected:
	
	asIScriptEngine *engine;
	vector<asIScriptContext*> contexts;
	
	asIScriptContext* PrepareContextFromPool(asIScriptFunction *func);
	void ReturnContextToPool(asIScriptContext *ctx);
	
	virtual void ConfigureEngine(asIScriptEngine *engine);
};

class AutoLoader
{
public:
	
	ofEvent<string> loaded;
	
	void setup(Engine &e);
	void addScript(const string &filename);
	
	void onUpdate(ofEventArgs&);

protected:
	
	Engine *engine;
	map<string, Poco::Timestamp> update_time_map;
};

#pragma mark - inline
	
inline Function& Function::addRef(void *ptr) { ctx->SetArgAddress(arg_index++, ptr); return *this; }
	
template <> inline Function& Function::addArg(const float& v) { ctx->SetArgFloat(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const double& v) { ctx->SetArgDouble(arg_index++, v); return *this; }

template <> inline Function& Function::addArg(const char& v) { ctx->SetArgByte(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const unsigned char& v) { ctx->SetArgByte(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const short& v) { ctx->SetArgWord(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const unsigned short& v) { ctx->SetArgWord(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const int& v) { ctx->SetArgDWord(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const unsigned int& v) { ctx->SetArgDWord(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const long& v) { ctx->SetArgQWord(arg_index++, v); return *this; }
template <> inline Function& Function::addArg(const unsigned long& v) { ctx->SetArgQWord(arg_index++, v); return *this; }

template <> inline void Function::getReturnValue(float& v) { v = ctx->GetReturnFloat(); }
template <> inline void Function::getReturnValue(double& v) { v = ctx->GetReturnDouble(); }
template <> inline void Function::getReturnValue(char& v) { v = ctx->GetReturnByte(); }
template <> inline void Function::getReturnValue(unsigned char& v) { v = ctx->GetReturnByte(); }
template <> inline void Function::getReturnValue(short& v) { v = ctx->GetReturnWord(); }
template <> inline void Function::getReturnValue(unsigned short& v) { v = ctx->GetReturnWord(); }
template <> inline void Function::getReturnValue(int& v) { v = ctx->GetReturnDWord(); }
template <> inline void Function::getReturnValue(unsigned int& v) { v = ctx->GetReturnDWord(); }
template <> inline void Function::getReturnValue(long& v) { v = ctx->GetReturnQWord(); }
template <> inline void Function::getReturnValue(unsigned long& v) { v = ctx->GetReturnQWord(); }
	
template <> inline void Function::getReturnValue(string &v)
{
	string *str = (string*)ctx->GetReturnObject();
	assert(str);
	v = *str;
}

}
