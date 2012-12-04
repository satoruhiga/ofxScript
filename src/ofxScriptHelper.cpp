#include "ofxScriptHelper.h"

asIScriptEngine *__engine = NULL;

void ofxScriptBindEngine(asIScriptEngine *engine)
{
	__engine = engine;
}

void ofxScriptUnbindEngine()
{
	__engine = NULL;
}

vector<string> __ns_stack;

void ofxScriptPushNamespace(string ns)
{
	__ns_stack.push_back(ns);
	
	string m;
	
	for (int i = 0; i < __ns_stack.size(); i++)
	{
		m += __ns_stack[i];
		if (i != __ns_stack.size() - 1)
			m += "::";
	}
	
	__engine->SetDefaultNamespace(m.c_str());
}

void ofxScriptPopNamespace()
{
	__ns_stack.pop_back();
	
	string m;
	
	for (int i = 0; i < __ns_stack.size(); i++)
	{
		m += __ns_stack[i];
		if (i != __ns_stack.size() - 1)
			m += "::";
	}
	
	__engine->SetDefaultNamespace(m.c_str());
}

