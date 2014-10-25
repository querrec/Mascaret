#ifndef XMLPARSER_H
#define XMLPARSER_H 1

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "Tools/veha_plateform.h"
#include "Tools/utils.h"

#include "Tools/veha_plateform.h"

//using boost::shared_ptr;

using std::string;
using std::vector;
using std::map;

#define XMLPARSER_DEFAULT_ENCODING    "ISO-8859-1"
typedef xmlElementType XmlNodeType;

class XmlNode;

//----------------------------------------------------------------------------
// Class XmlParser
//----------------------------------------------------------------------------

/*! \class XmlParser
 * \brief classe permettant de creer le dom du xml
 *
 *  Cette classe permet de parser un fichier xml. Elle represente en même temps le document dom.
 */
class VEHA_API XmlParser
{
public:

	XmlParser();
	virtual ~XmlParser();

	bool parseFile(const string& fileName);
	bool parseString(const string& str);
	boost::shared_ptr<XmlNode> getRoot(void);

	bool createFile(const string& rootName);
	bool writeFile(const string& filename);
	string writeString();

private :
	xmlDoc * _doc;
	boost::shared_ptr<XmlNode> _root;


};

/*! \class XmlNode
 * \brief classe representant un noeud du document dom.
 *
 * Cette classe represente un noeud xml.
 * Elle contient les methodes necessaires a la navigation et a l'execution de requetes xpath.
 */
class VEHA_API XmlNode
{
public:

	XmlNode();
	XmlNode(const string& name);
	virtual ~XmlNode();

	friend class XmlParser;

	string getName(void);
	XmlNodeType getType();

	string getProperty(const string& name);
	string getProperty2(const string& name);
	bool   getPropertyString(const string& name, string& value);
	bool   getPropertyInteger(const string& name, int& value);
	bool   getPropertyReal(const string& name, double& value);
	bool   getPropertyBoolean(const string& name, bool& value);
	bool   hasProperty(const string& name);

	void   setProperty(const char* name, char * value);
	void   setProperty(const string& name, string& value);
	void   setProperty(const string& name, int& value);
	void   setProperty(const string& name, double& value);
	void   setProperty(const string& name, bool& value);

	string getString(void);

	boost::shared_ptr<XmlNode> getNext(void);
	boost::shared_ptr<XmlNode> getPrevious(void);
	boost::shared_ptr<XmlNode> getFirstChild(void);
	boost::shared_ptr<XmlNode> getLastChild(void);
	boost::shared_ptr<XmlNode> getParent(void);

	PRE_DEPRECATED boost::shared_ptr<XmlNode> getChild(const string& childName, size_t occurence=1) POST_DEPRECATED;
	boost::shared_ptr<XmlNode> getChildByName(const string& name);
	vector<boost::shared_ptr<XmlNode> > getChildNodes();
	vector<boost::shared_ptr<XmlNode> > getChildNodesByName(const string& name);
	string getInnerText();

	void addChild(boost::shared_ptr<XmlNode> node, bool recursive = false);
	void addTextChild(const string& childName, const string& textContent);

	boost::shared_ptr<XmlNode> selectSingleNode(const string& xpathExpr, const map<string,string>& namespaces);
	vector<boost::shared_ptr<XmlNode> > selectNodes(const string& xpathExpr, const map<string,string>& namespaces);
	boost::shared_ptr<XmlNode> selectSingleNode(const string& xpathExpr);
	vector<boost::shared_ptr<XmlNode> > selectNodes(const string& xpathExpr);




private :
	void _init(xmlNode * node);
	xmlNode * _element;
};

#endif                           // XMLPARSER_H

//----------------------------------------------------------------------------
