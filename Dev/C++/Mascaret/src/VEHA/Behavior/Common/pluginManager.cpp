#include "Tools/filesystem.h"
#include "VEHA/Behavior/Common/pluginManager.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

DECLARE_NAMESPACE_FILESYSTEM;

#define MAGIC_CHAR_4_RAW_DATA '$'

namespace VEHA
{
using std::ifstream;
using std::stringbuf;
using std::ofstream;

typedef void (*MessageFct)();
typedef void (*initPluginFct)(shared_ptr<XmlNode> pluginNode);
PluginManager * PluginManager::_instance=NULL;

PluginManager * PluginManager::getInstance()
{
	if (!_instance)
		_instance=new PluginManager();
	return _instance;
}


typedef void (*fct) (void);
//TODO ->add
bool PluginManager::openPlugin(const string & name, shared_ptr<XmlNode> pluginNode)
{
#ifdef _WIN32
    string libName=name+".dll";
#else
	string libName="lib"+name+".so";
#endif

# if BOOST_FILESYSTEM_VERSION == 2
    string filefullpath = fs::complete(libName,_pluginDir).file_string();
# else
    string filefullpath = fs::complete(libName,_pluginDir).string();
# endif

    cerr << "[PluginManager Info] Loading: " << filefullpath << endl;

#ifdef _WIN32
    void * lib=(void*)LoadLibrary(filefullpath.c_str());
#else
	void * lib=dlopen(filefullpath.c_str(), RTLD_LAZY);
#endif

	if (lib)
	{
		_libs.push_back(lib);
		void* initFct;
#ifdef _WIN32
		if ((initFct=(void*)GetProcAddress((HINSTANCE)lib,"MASCARET_INIT")))
#else
		if ((initFct=dlsym(lib,"MASCARET_INIT")))
#endif
		{
			(*(initPluginFct)(intptr_t)initFct)(pluginNode);
		}
		else
		{
			cerr<<"[PluginManager Error] NO MASCARET_INIT found in " << name <<" !"<<endl;
		}
	}
	else
	{
		cerr << "[PluginManager Error] Opening plugin \""<< name <<"\": " << getPluginError() << endl;
	}
	return (lib!=NULL);
}


void * PluginManager::getSymbol(const string& symbolName)
{
	//    cerr<<"Want symbol:"<<symbolName<<"|"<<endl;
	void * result=NULL;
	for (size_t i=_libs.size();i--;)
	{
#ifdef _WIN32
		if ((result=(void*)GetProcAddress((HINSTANCE)_libs[i],symbolName.c_str())))
#else
		if ((result=dlsym(_libs[i],symbolName.c_str())))
#endif
			break;
	}
#ifdef _WIN32
  if (!result) result=(void*)GetProcAddress((HINSTANCE)NULL,symbolName.c_str());
#else
if (!result) result=dlsym(NULL,symbolName.c_str());
#endif 
	return result;

}


string PluginManager::getPluginError()
{
#ifdef _WIN32
	static char error_buffer[65535];
	DWORD errCode=GetLastError();

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			error_buffer, sizeof(error_buffer), NULL );
	return string(error_buffer);
#else
	char * err=dlerror();
if (err)return err;
else return "";
#endif
}


PluginManager::PluginManager()
:_pluginDir("/tmp/"), _init(false)
{
}

PluginManager::~PluginManager()
{
	for (size_t i=_libs.size();i--;)
	{
#ifdef _WIN32
		FreeLibrary((HINSTANCE)_libs[i]);
#else
		dlclose(_libs[i]);
#endif
	}
}
void PluginManager::test()
{
	vector<pair<string,string> > v;
	v.push_back(make_pair(string("Deflecteur_Ouvrir"),string("cerr<<\"Lancement set Pos\"<<endl;Vector3 b;dynamic_cast<Entity*>(_host.get())->setPosition(b);")));
	updatePlugin("MonPlug",v);
	if(!openPlugin("./MonPlug.so"))
		cerr<<"Cant open MonPlug.so"<<endl;
	//	_generateCppFile("monTest__","he he ho ho \n toto tutu");
	//cerr<<"et hop"<<endl;
}

void PluginManager::fireMascaretWindowsCreated()
{
	fire("MASCARET_WINDOWS_CREATED");
}
void PluginManager::fireMascaretModelLoaded()
{
	fire("MASCARET_MODEL_LOADED");
}
void PluginManager::fireMascaretStep()
{
	fire("MASCARET_STEP");
}
void PluginManager::fireMascaretDestroying()
{
	fire("MASCARET_DESTROYING");
}
void PluginManager::fire(const string& message)
{
	//cerr << " WANT : " << message << endl;
	for (size_t i=_libs.size();i--;)
	{
		void * result=NULL;
#ifdef _WIN32
		if ((result=(void*)GetProcAddress((HINSTANCE)_libs[i],message.c_str())))
#else
		if ((result=dlsym(_libs[i],message.c_str())))
#endif
		{
			//cerr << " PLUGIN : " << message << " FOUND" << endl;
			(*(MessageFct)(intptr_t)result)();
		}
	}
}
void PluginManager::updatePlugin(const string& pluginName,const vector<pair<string,string> >& behaviors)
{
        #if BOOST_FILESYSTEM_VERSION == 2
	string name=fs::complete(pluginName,_pluginDir).file_string();
        #else
	string name=fs::complete(pluginName,_pluginDir).string();
        #endif
	string cppFileData=_generateCppCodePlugin(behaviors);
	_generateCppFile(name,cppFileData);
	_generatePlugin(name);

}


string PluginManager::_generateCppCodePlugin(const vector<pair<string,string> >& behaviors)
{
	string result("\
			#include  \"VEHA/Kernel/InstanceSpecification.h\"\
			\n#include  \"VEHA/Kernel/Slot.h\"\
			\n#include  \"VEHA/Kernel/Property.h\"\
			\n#include  \"VEHA/Kernel/LiteralReal.h\"\
			\n#include  \"VEHA/Behavior/Common/BehaviorExecution.h\"\
			\n#include  \"VEHA/Behavior/Common/Behavior.h\"\
			\n#include  \"VEHA/Entity/Vector3.h\"\
			\n#include  \"VEHA/Entity/RotationVector.h\"\
			\n#include  \"VEHA/Entity/Entity.h\"\
			\n#include <iostream>\
			\n#include <vector>\
			\nusing namespace std;\
			\nusing namespace VEHA;\n");
	for(size_t i=behaviors.size();i--;)
	{
		const string& b=behaviors[i].second;
		const string& behaviorName=behaviors[i].first;
		if (b.size()&& b[0]==MAGIC_CHAR_4_RAW_DATA)
		{
			result+=b.substr(1);
		}
		else
		{
			string className=behaviorName;
			result+="\nclass  "+className+": public BehaviorExecution \n{\n"
					+"public:\n"
					//constructor
					+className+"(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification>  host,const Parameters& p):BehaviorExecution(specif,host,p) \n{}\ndouble execute()\n{\n";
			//executeFonction
			//			for (size_t i=
			result+=b
					+"return -1;\n}\n"
					//+"\nconst Parameters& parameters;\n};\n"
					+"\n};\n"
					//init
					+"extern \"C\"{\nshared_ptr<BehaviorExecution> "+behaviorName+"_init(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host,const Parameters& p)\n{\n"
					+"  return shared_ptr<BehaviorExecution>(new "+className+"(specif,host,p));\n}\n}\n";

		}
	}
	return result;
}


void PluginManager::_generateCppFile(const string& pluginName,const string& data)
{
	ifstream f((pluginName+".cpp").c_str());
	if (!f.fail())
	{
		stringbuf fileStr;
		f.get(fileStr,0);
		if (fileStr.str()==data)
			return;              //nothin to do: same data
		else f.close();
	}
	ofstream of((pluginName+".cpp").c_str());
	of<<data;
	of.close();

}


void PluginManager::_generatePlugin(const string& pluginName)
{
	struct stat infosPlugin, infosCpp;
	if (!stat((pluginName+".so").c_str(),&infosPlugin))
	{
		//FILE EXIST
		if (!stat((pluginName+".cpp").c_str(),&infosCpp))
		{
			if (infosPlugin.st_mtime>infosCpp.st_mtime)
				return ;         //nothing to do;
		}
		else
		{
			cerr<<"can't create Plugin :"<<pluginName<<".cpp don't exist"<<endl;
			return ;
		}
	}
	string compileLine("g++ -rdynamic -fPIC --shared -I ../VEHA/include -I ../Tools/include -o ");
	compileLine+=pluginName+".so "+pluginName+".cpp";
	compileLine+=" -g";          //debug
	if (system(compileLine.c_str()))
		cerr<<"Error in creating : "<<pluginName<<".so"<<endl;
}
}
