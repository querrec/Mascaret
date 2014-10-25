/*
 * ModelLoader.cpp
 *
 *  Created on: 18 janv. 2011
 *      Author: soler
 */

#include "Tools/Loaders/AbstractModelLoader.h"
#include "Tools/Loaders/XmlParser.h"

shared_ptr<Model> AbstractModelLoader::parseFile(const string& filename)
{
	shared_ptr<XmlParser> parser=shared_ptr<XmlParser>(new XmlParser());
	if(parser->parseFile(filename))
		return getModel(parser->getRoot());
	cerr << "Impossible de charger le fichier "+filename << endl;
	return shared_ptr<Model>();
}
shared_ptr<Model> AbstractModelLoader::parseString(const string& str)
{
	shared_ptr<XmlParser> parser=shared_ptr<XmlParser>(new XmlParser());
	if(parser->parseString(str))
		return getModel(parser->getRoot());
	cerr << "Impossible de charger la chaine de caractere :"<<endl<<str<< endl;
	return shared_ptr<Model>();
}
