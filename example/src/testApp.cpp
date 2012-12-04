#include "testApp.h"

#include "ofxScript.h"

ofxScript::Engine script;
ofxScript::Function func;

class TestClass
{
public:
	
	float my_param;
	
	TestClass()
	{
		cout << "constructor" << endl;
	}
	
	~TestClass()
	{
		cout << "destructor" << endl;
	}
	
	TestClass(int v)
	{
		cout << "constructor with 1 param: v = " << v << endl;
	}
	
	TestClass(int v0, int v1)
	{
		cout << "constructor with 2 param: v0 = " << v0 << ", v1 = " << v1 << endl;
	}
	
	void call()
	{
		cout << "method called" << endl;
	}
	
	int call(int v)
	{
		cout << "called with param: v = " << v << endl;
		
		my_param = v * 100;
		return  my_param;
	}
};

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	ofBackground(30);
	
	script.setup();
	
	{
		ofxScriptBindEngine(script.getEngine());
		
		// register custom c++ class to script engine
		ofxScriptRegisterClassCDEC(TestClass);
		
		// register constructor
		ofxScriptRegisterConstructWithParam1(TestClass, int);
		ofxScriptRegisterConstructWithParam2(TestClass, int, int);
		
		// register methods
		ofxScriptRegisterObjectMethod(TestClass, void, call, ());
		ofxScriptRegisterObjectMethod(TestClass, int, call, (int));
		
		// register variable
		ofxScriptRegisterObjectProperty(TestClass, float, my_param);
		
		ofxScriptUnbindEngine();
	}
	
#define _S(code) #code
	
	const char *as_code = _S(
		
		// angelscript world
		void as_test(TestClass &cpp_object)
		{
			TestClass s0;
			TestClass s1(10);
			TestClass s2(10, 20);
			
			s1.call();
			int rc = s2.call(42);
			print(">>> " + rc);
			print(">>> my_param = " + s2.my_param);
			
			// copy value to c++ world's object
			cpp_object = s2;
		}
		
	);
	
	script.compileString(as_code);
	
	TestClass cpp_object;
	
	// pass the object reference & call function
	script.getFunction("as_test").addRef(&cpp_object).call();
	
	cout << "<<< " << cpp_object.my_param << endl;
	
	ofExit();
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}