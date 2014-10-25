#include "Tools/filesystem.h"
#include "Tools/Loaders/InstanceWriter.h"
#include "boost/foreach.hpp"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Slot.h"

DECLARE_NAMESPACE_FILESYSTEM;

using namespace VEHA;
using namespace std;
using boost::shared_dynamic_cast;

string getRelativePath(const string& file,const string& base);

InstanceWriter::InstanceWriter(shared_ptr<Environment> environment, const string& filename)
:_environment(environment)
{
    //pour le moment, ecriture sans libxml
    _basedir=fs::path(filename).branch_path().file_string();
    ofstream f(filename.c_str());
    f<<"<Decors>"<<endl;
    pair<string,shared_ptr<InstanceSpecification> >p;
    BOOST_FOREACH(p,_environment->getInstances())
                                 //only root entity
        if (!shared_dynamic_cast<Entity>(p.second)->getParent())
        _writeEntity(f,shared_dynamic_cast<Entity>(p.second),"  ");
    f<<"</Decors>"<<endl;
    f.close();
}


void InstanceWriter::_writeEntity(ofstream& file,shared_ptr<Entity> entity,const string& startLine)
{
    file<<startLine<<"<Entity name=\""<<entity->getName()<<"\" class=\""
        <<entity->getClassifier()->getName()<<"\">"<<endl;
    //position
    Vector3 p=entity->getPosition();
    file<<startLine<<"<Position x=\""<<p.x<<"\" y=\""<<p.y<<"\" z=\""<<p.z<<"\"/>"<<endl;
    Vector3 o=entity->getOrientation();
    file<<startLine<<"<Orientation roll=\""<<o.x<<"\" pitch=\""<<o.y<<"\" yaw=\""<<o.z<<"\"/>"<<endl;
    //shape
    if (entity->get_DEP_current())
    {
        string url=entity->get_DEP_current()->getBody().url;
        url=getRelativePath(url,_basedir);
        file<<startLine<<"<Shape url=\""<<url<<"\"/>"<<endl;
    }
    //Attribute / Relation
    map<string,shared_ptr<Slot> >::const_iterator pSlot;
    for(pSlot=entity->getSlots().begin();pSlot!=entity->getSlots().end();pSlot++)
        //	BOOST_FOREACH(pSlot,entity->getSlots())
    {
        pair< string, shared_ptr< ValueSpecification > > pValue;
        BOOST_FOREACH(pValue,pSlot->second->getValues())
            file<<startLine<<"<Attribute name=\""<<pSlot->first<<"\" value=\""<<pValue.second->getStringFromValue() <<"\"/>"<<endl;
    }
    //children
    BOOST_FOREACH(shared_ptr<Entity> pInstance,entity->getChildren())
        _writeEntity(file,pInstance,startLine+"  ");
    file<<startLine<<"</Entity>"<<endl;
}


fs::path cleanPath(fs::path p)
{
    fs::path result;
    vector<string> s;
    fs::path::iterator it;
    for (it=p.begin();it!=p.end();it++)
    {
        if (!s.size() || !(*it=="." || *it==".."))
            s.push_back(*it);
        else if (*it==".." && s.back()!="..")
            s.pop_back();
    }
    for (size_t i=0;i<s.size();i++)
    {
        result/=s[i];
    }
    return result;
}


string getRelativePath(const string& file,const string& basedir)
{
    fs::path basePath=((!basedir.size())?fs::initial_path():complete(fs::path(basedir)));
    fs::path filePath=fs::complete(file);

    // on elimine les ..

    fs::path rbasePath=cleanPath(basePath);
    fs::path rfilePath=cleanPath(filePath.branch_path());
    fs::path::iterator it,it2;
    for (it=rbasePath.begin(),it2=rfilePath.begin();it!=rbasePath.end() && it2!=rfilePath.end();it++,it2++)
    {
        if (*it!=*it2)
            break;
    }
    // ajout .. pour tout les iteration manquant dans le chemin de base  + rfilePath restant
    fs::path newPath;
    while(it++!=rbasePath.end())
    {
        newPath/="..";
    }
    while(it2!=rfilePath.end())
    {
        newPath/=*it2++;
    }
    //	newPath/=
    //fs::path(it2,rfilePath.end());
    newPath/=filePath.leaf();
    return newPath.file_string();
}
