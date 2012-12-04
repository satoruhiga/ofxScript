#include "ofxScript.h"

#include "scriptstdstring.h"
#include "scriptbuilder.h"
#include "scriptmath.h"
#include "scriptarray.h"
#include "scripthandle.h"
#include "scripthelper.h"

namespace ofxScript
{

static void print(string &msg)
{
	printf("%s\n", msg.c_str());
}

static void print_any(void *ref, int typeId)
{
	if ((typeId & asTYPEID_MASK_OBJECT) == 0)
	{
		switch (typeId) {
			case asTYPEID_BOOL:
			{
				if (*((bool*)ref))
					printf("true\n");
				else
					printf("false\n");
				break;
			}
			case asTYPEID_INT8: printf("%i\n", *(int8_t*)ref); break;
			case asTYPEID_INT16: printf("%i\n", *(int16_t*)ref); break;
			case asTYPEID_INT32: printf("%i\n", *(int32_t*)ref); break;
			case asTYPEID_INT64: printf("%lli\n", *(int64_t*)ref); break;
			case asTYPEID_UINT8: printf("%i\n", *(uint8_t*)ref); break;
			case asTYPEID_UINT16: printf("%i\n", *(uint16_t*)ref); break;
			case asTYPEID_UINT32: printf("%i\n", *(uint32_t*)ref); break;
			case asTYPEID_UINT64: printf("%lli\n", *(uint64_t*)ref); break;
			
			case asTYPEID_FLOAT: printf("%f\n", *(float*)ref); break;
			case asTYPEID_DOUBLE: printf("%f\n", *(double*)ref); break;
			default:
				break;
		}
	}
	else
	{
		printf("[object]\n");
	}
}
	
static void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "[!ERR]";
	string path = ofFilePath::getFileName(msg->section);
	
	if( msg->type == asMSGTYPE_WARNING )
	{
		type = "[WARN]";
	}
	else if( msg->type == asMSGTYPE_INFORMATION )
	{
		return;
	}
	
	printf("%s: %s @ %s:%d\n", type, msg->message, path.c_str(), msg->row);
}

#pragma mark - Engine
	
Engine::~Engine()
{
	dispose();
}

void Engine::ConfigureEngine(asIScriptEngine *engine)
{
	int r;
	
	r = engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, true);
	
	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterScriptMath(engine);
	RegisterScriptHandle(engine);
	

	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterGlobalFunction("void print(?&in)", asFUNCTION(print_any), asCALL_CDECL); assert( r >= 0 );

	if( r < 0 )
	{
		cout << "Failed to register global function" << endl;
		return;
	}
}

	void Engine::onExit(ofEventArgs&)
	{
		dispose();
	}
	
bool Engine::setup()
{
	if (engine) return false;
	
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if( engine == 0 )
	{
		cout << "Failed to create script engine." << endl;
		return false;
	}
	
	int r;
	
	r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert( r >= 0 );
	
	ConfigureEngine(engine);
	
	
	ofAddListener(ofEvents().exit, this, &Engine::onExit);
	;
}

void Engine::dispose()
{
	for (int i = 0; i < contexts.size(); i++)
		contexts[i]->Release();
	contexts.clear();
	
	if (engine)
	{
		engine->Release();
		engine = NULL;
	}
}

bool Engine::compile(const string& path)
{
	if (!engine) setup();
	
	int r;
	
	CScriptBuilder builder;
	r = builder.StartNewModule(engine, 0);
	if( r < 0 )
	{
		cout << "Failed to start new module" << endl;
		return false;
	}
	
	string p = ofToDataPath(path);
	
	r = builder.AddSectionFromFile(p.c_str());
	if( r < 0 )
	{
		cout << "Failed to add script file" << endl;
		return false;
	}

	r = builder.BuildModule();
	if( r < 0 )
	{
		cout << "Failed to build the module" << endl;
		return false;
	}
	
	return true;
}

bool Engine::compileString(const string& src)
{
	if (!engine) setup();
	
	int r;
	
	CScriptBuilder builder;
	r = builder.StartNewModule(engine, 0);
	if( r < 0 )
	{
		cout << "Failed to start new module" << endl;
		return false;
	}
	
	r = builder.AddSectionFromMemory(src.c_str());
	if( r < 0 )
	{
		cout << "Failed to add source" << endl;
		return false;
	}

	r = builder.BuildModule();
	if( r < 0 )
	{
		cout << "Failed to build the module" << endl;
		return false;
	}
	
	return true;
}

bool Engine::hasFunction(const string& name)
{
	asIScriptModule *module = engine->GetModule(0);
	assert(module);
	
	return NULL != module->GetFunctionByName(name.c_str());
}

Function Engine::getFunction(const string& name)
{
	asIScriptModule *module = engine->GetModule(0);
	assert(module);
	
	asIScriptFunction *func = module->GetFunctionByName(name.c_str());
	return Function(this, func);
}

asIScriptContext* Engine::PrepareContextFromPool(asIScriptFunction *func)
{
	asIScriptContext *ctx = 0;
	if(contexts.size())
	{
		ctx = *contexts.rbegin();
		contexts.pop_back();
	}
	else
		ctx = engine->CreateContext();
	
	return ctx;
}

void Engine::ReturnContextToPool(asIScriptContext *ctx)
{
	if (!engine) return;
		
	contexts.push_back(ctx);
	ctx->Unprepare();
}
	
#pragma mark - Function

Function::Function(Engine *engine, asIScriptFunction *func)
: engine(engine), func(func), arg_index(0), has_error(false)
{
	if (!func)
	{
		has_error = true;
		return;
	}

	func->AddRef();
	ctx = engine->PrepareContextFromPool(func);
	prepare();
}

Function::~Function()
{
	if (func)
	{
		func->Release();
		func = NULL;
	}
	
	if (engine && ctx)
	{
		engine->ReturnContextToPool(ctx);
		engine = NULL;
	}
}

Function& Function::operator=(const Function& copy)
{
	engine = copy.engine;

	has_error = false;
	func = copy.func;
	func->AddRef();
	
	ctx = engine->PrepareContextFromPool(func);
	prepare();
}

bool Function::prepare()
{
	if (!func || !ctx) return false;
	
	arg_index = 0;
	
	int r = ctx->Prepare(func); assert( r >= 0 );
	if( r < 0 )
	{
		cout << "Failed to prepare the context." << endl;
		ctx->Release();
		return false;
	}
	
	return true;
}

bool Function::call()
{
	if (has_error) return;
	
	if (func->GetParamCount() != arg_index)
	{
		cout << "Invalid param count." << endl;
		return false;
	}
	
	int r = ctx->Execute();
	
	if (r == asEXECUTION_FINISHED)
		return true;
	
	has_error = true;
	
	if( r == asEXECUTION_ABORTED )
	{
		cout << "The script was aborted before it could finish. Probably it timed out." << endl;
	}
	else if( r == asEXECUTION_EXCEPTION )
	{
		printf("\n------------------\n");
		PrintException(ctx, true);
	}
	else
	{
		cout << "The script ended for some unforeseen reason (" << r << ")." << endl;
	}
	
	return false;
}

void AutoLoader::setup(Engine &e)
{
	if (!e) e.setup();
	
	engine = &e;
	ofAddListener(ofEvents().update, this, &AutoLoader::onUpdate);
}

void AutoLoader::addScript(const string &filename)
{
	assert(engine);
	
	update_time_map[filename] = 0;
}

void AutoLoader::onUpdate(ofEventArgs&)
{
	assert(engine);
	
	for (map<string, Poco::Timestamp>::iterator it = update_time_map.begin(); it != update_time_map.end(); it++)
	{
		ofFile file(it->first);
		Poco::Timestamp t = file.getPocoFile().getLastModified();
		
		if (t != it->second)
		{
			update_time_map[it->first] = t;
			if (engine->compile(file.path()))
			{
				ofLogVerbose("ofxScript::AutoLoader") << "loaded: " << file.getFileName() << endl;

				string s = it->first;
				ofNotifyEvent(loaded, s);
			}
		}
	}
}
	
}