#include "Tools/Loaders/XmlParser.h"
#include <iostream>
#include <sstream>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

using namespace VEHA;

//----------------------------------------------------------------------------
// Class XmlParser
//----------------------------------------------------------------------------
//-------- Construction / Destruction ----------------------------------------
XmlParser::XmlParser()
{
}


XmlParser::~XmlParser(void)
{
}


bool XmlParser::createFile(const string& rootname)
{
	_doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNode * root = xmlNewNode(NULL, BAD_CAST rootname.c_str());
	_root = shared_ptr<XmlNode>(new XmlNode());
	_root->_init(root);
	xmlDocSetRootElement(_doc, root);

	return true;
}

bool XmlParser::writeFile(const string& fileName)
{
	xmlSaveFormatFileEnc(fileName.c_str(), _doc, "UTF-8", 1);
	return true;
}

bool XmlParser::parseFile(const string& fileName)
{
#if _WIN32
	_doc = xmlParseFile(fileName.c_str());
#else
	_doc = xmlReadFile(fileName.c_str(), NULL, 0);
#endif
	if (_doc == NULL)
	{
		cerr << "Failed to parse "<< fileName << endl;
		return false;
	}
	else
	{
		_root = shared_ptr<XmlNode>(new XmlNode());
		_root->_init(xmlDocGetRootElement(_doc));
		return true;
	}
}


bool XmlParser::parseString(const string& str)
{
#if _WIN32
	_doc = xmlParseMemory(str.c_str(),str.size());
#else
	_doc = xmlReadMemory(str.c_str(),str.size(), "string" ,NULL, 0);
#endif
	if (_doc == NULL)
	{
		cerr << "Failed to parse string"<< endl;
		return false;
	}
	else
	{
		_root = shared_ptr<XmlNode>(new XmlNode());
		_root->_init(xmlDocGetRootElement(_doc));
		return true;
	}
}

string XmlParser::writeString()
{
	int doc_txt_len;
	char *doc_txt_ptr=NULL;

	xmlDocDumpFormatMemory(_doc, (xmlChar **)(void *)&doc_txt_ptr, &doc_txt_len, 1);

	string chaine(doc_txt_ptr);

	return chaine;

}


shared_ptr<XmlNode> XmlParser::getRoot(void)
{
	return _root;
}


//----------------------------------------------------------------------------
// Class XmlNode
//----------------------------------------------------------------------------
//-------- Construction / Destruction ----------------------------------------
XmlNode::XmlNode()
{
}

XmlNode::XmlNode(const string& name)
{
	_element = xmlNewNode(NULL, BAD_CAST name.c_str());
}

XmlNode::~XmlNode(void)
{
}


string XmlNode::getName(void)
{
	if (_element)
	{
		string s((const char*)(_element->name));
		return s;
	}
	else return "";
}

XmlNodeType XmlNode::getType()
{
	return _element->type;
}
string XmlNode::getProperty(const string& name)
{
	xmlAttr *properties = _element->properties;
	bool found = false;
	while ( (properties!=NULL) && (!found))
	{
		string propertyName((const char*)properties->name);
		if (propertyName == name)
			found = true;
		if (!found) properties = properties->next;
	}
	if (found)
	{
		xmlNode * value = properties->children;
		if (value != NULL) return ((const char *)(value->content));
		else return "" ;
	}
	return "";
}

string XmlNode::getProperty2(const string& name)
{
	xmlAttr *properties = _element->properties;
	xmlAttr *findProp = _element->properties;
	bool found = false;
	while ( (properties!=NULL))
	{
		string propertyName((const char*)properties->name);
		if (propertyName == name)
                {
			found = true;
                        findProp = properties;
                }
		properties = properties->next;
	}
	if (found)
	{
		xmlNode * value = findProp->children;
		if (value != NULL) return ((const char *)(value->content));
		else return "" ;
	}
	return "";
}


bool XmlNode::getPropertyString(const string& name, string& value)
{
	if (hasProperty(name))
	{
		value = getProperty(name);
		return true;
	}
	else return false;
}


bool XmlNode::getPropertyInteger(const string& name, int& value)
{
	if (hasProperty(name))
	{
		string val = getProperty(name);
		value = atoi(val.c_str());
		return true;
	}
	else return false;
}


bool XmlNode::getPropertyReal(const string& name, double& value)
{
	if (hasProperty(name))
	{
		string val = getProperty(name);
		value = atof(val.c_str());
		return true;
	}
	else return false;
}


bool XmlNode::getPropertyBoolean(const string& name, bool& value)
{
	if (hasProperty(name))
	{
		string val = getProperty(name);
		if ( val == "true" || val == "TRUE" )
			value = true;
		else
			value = false;
		return true;
	}
	else return false;
}

bool XmlNode::hasProperty(const string& name)
{
	xmlAttr *properties = _element->properties;
	bool found = false;
	while ( (properties!=NULL) && (!found))
	{
		string propertyName((const char*)properties->name);
		if (propertyName == name)
			found = true;
		properties = properties->next;
	}

	return found;
}

void XmlNode::setProperty(const char * name, char * value)
{
	xmlNewProp(_element, BAD_CAST name, BAD_CAST value);
}

void XmlNode::setProperty(const string& name, string& value)
{
	xmlNewProp(_element, BAD_CAST name.c_str(), BAD_CAST value.c_str());
}

void XmlNode::setProperty(const string& name, int& value)
{
	std::ostringstream oss;
	oss << value;
	std::string tmp = oss.str();

	//    string tmp;
	//    tmp += value;
	xmlNewProp(_element, BAD_CAST name.c_str(), BAD_CAST tmp.c_str());
}

void XmlNode::setProperty(const string& name, double& value)
{
	std::ostringstream oss;
	oss << value;
	std::string tmp = oss.str();

	//    string tmp;
	//    tmp += value;
	xmlNewProp(_element, BAD_CAST name.c_str(), BAD_CAST tmp.c_str());
}

void XmlNode::setProperty(const string& name, bool& value)
{
	string tmp;
	if (value) tmp = "true";
	else tmp = "false";

	xmlNewProp(_element, BAD_CAST name.c_str(), BAD_CAST tmp.c_str());
}

shared_ptr<XmlNode> XmlNode::getNext(void)
{
	xmlNode * nextElem = _element->next;
	if (nextElem != NULL)
	{
		shared_ptr<XmlNode> next(new XmlNode());
		next->_init(nextElem);
		return next;
	}
	else return shared_ptr<XmlNode>();
}


shared_ptr<XmlNode> XmlNode::getPrevious(void)
{
	xmlNode * prevElem = _element->prev;
	if (prevElem != NULL)
	{
		shared_ptr<XmlNode> prev(new XmlNode());
		prev->_init(prevElem);
		return prev;
	}
	else return shared_ptr<XmlNode>();
}


shared_ptr<XmlNode> XmlNode::getFirstChild(void)
{
	xmlNode * firstChild = _element->children;
	if (firstChild != NULL)
	{
		shared_ptr<XmlNode> first ( new XmlNode() );
		first->_init(firstChild);
		return first;
	}
	else return shared_ptr<XmlNode>();
}


shared_ptr<XmlNode> XmlNode::getLastChild(void)
{
	xmlNode * lastChild = _element->last;
	if (lastChild != NULL)
	{
		shared_ptr<XmlNode> last ( new XmlNode() );
		last->_init(lastChild);
		return last;
	}
	else return shared_ptr<XmlNode>();
}


shared_ptr<XmlNode> XmlNode::getParent(void)
{
	xmlNode * parent = _element->parent;
	if (parent != NULL)
	{
		shared_ptr<XmlNode> par ( new XmlNode() );
		par->_init(parent);
		return par;
	}
	else return shared_ptr<XmlNode>();
}
shared_ptr<XmlNode> XmlNode::selectSingleNode(const string& xpathExpr)
{
	map<string,string> namespaces;
	return selectSingleNode(xpathExpr,namespaces);
}
shared_ptr<XmlNode> XmlNode::selectSingleNode(const string& xpathExpr, const map<string,string>& namespaces)
{
	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(_element->doc);
	if(xpathCtx == NULL)
	{
		cerr << "Error: unable to create new XPath context" << endl;
		return shared_ptr<XmlNode>();
	}
	xpathCtx->node=_element;

	for(map<string,string>::const_iterator it=namespaces.begin();it != namespaces.end();it++)
	{
		if(xmlXPathRegisterNs(xpathCtx, BAD_CAST it->first.c_str(), BAD_CAST it->second.c_str()) != 0) {
			cerr << "Error: unable to register NS with prefix=\"" << it->first << "\" and href=\"" << it->second << "\"\n" << endl;
			xmlXPathFreeContext(xpathCtx);
			return shared_ptr<XmlNode>();
		}
	}

	xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathExpr.c_str(), xpathCtx);
	if(xpathObj == NULL)
	{
		cerr << "Error: unable to evaluate xpath expression \""<< xpathExpr << "\"" << endl;
		xmlXPathFreeContext(xpathCtx);
		return shared_ptr<XmlNode>();
	}

	xmlNodeSetPtr nodes=xpathObj->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
	if(size > 0)
	{
		shared_ptr<XmlNode> c ( new XmlNode() );
		c->_init(nodes->nodeTab[0]);
		return c;
	}
	return shared_ptr<XmlNode>();
}
vector<shared_ptr<XmlNode> > XmlNode::selectNodes(const string& xpathExpr)
{
	map<string,string> namespaces;
	return selectNodes(xpathExpr,namespaces);
}
vector<shared_ptr<XmlNode> > XmlNode::selectNodes(const string& xpathExpr, const map<string,string>& namespaces)
{
	vector<shared_ptr<XmlNode> > result;
	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(_element->doc);
	if(xpathCtx == NULL)
	{
		cerr << "Error: unable to create new XPath context" << endl;
		return result;
	}
	xpathCtx->node=_element;

	for(map<string,string>::const_iterator it=namespaces.begin();it != namespaces.end();it++)
	{
		if(xmlXPathRegisterNs(xpathCtx, BAD_CAST it->first.c_str(), BAD_CAST it->second.c_str()) != 0) {
			cerr << "Error: unable to register NS with prefix=\"" << it->first << "\" and href=\"" << it->second << "\"\n" << endl;
			xmlXPathFreeContext(xpathCtx);
			return result;
		}
	}

	xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathExpr.c_str(), xpathCtx);
	if(xpathObj == NULL)
	{
		cerr << "Error: unable to evaluate xpath expression \""<< xpathExpr << "\"" << endl;
		xmlXPathFreeContext(xpathCtx);
		return result;
	}

	xmlNodeSetPtr nodes=xpathObj->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
	for(int i = 0; i < size; ++i)
	{
		shared_ptr<XmlNode> c ( new XmlNode() );
		c->_init(nodes->nodeTab[i]);
		result.push_back(c);
	}
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	return result;
}

shared_ptr<XmlNode> XmlNode::getChild(const string& childName, size_t occurence)
{
	if (_element != NULL)
	{
		xmlNode * child = _element->children;
		bool found = false;
		size_t nb = 1;
		while ( child!=NULL && !found)
		{
			string cName((const char*)(child->name));
			if (cName == childName)
			{
				if (nb == occurence) found = true;
				nb++;
			}
			if (!found) child = child->next;
		}
		if (found)
		{
			shared_ptr<XmlNode> c ( new XmlNode() );
			c->_init(child);
			return c;
		}
		else return shared_ptr<XmlNode>();
	}
	else return shared_ptr<XmlNode>();
}
shared_ptr<XmlNode> XmlNode::getChildByName(const string& childName)
{
	if (_element != NULL)
	{
		xmlNode * child = _element->children;
		while (child != NULL)
		{
			string cName((const char*)(child->name));
			if (cName == childName)
			{
				shared_ptr<XmlNode> c ( new XmlNode() );
				c->_init(child);
				return c;
			}
			child = child->next;
		}
	}
	return shared_ptr<XmlNode>();
}
vector<shared_ptr<XmlNode> > XmlNode::getChildNodes()
{
	vector<shared_ptr<XmlNode> > children;
	xmlNode * child = _element->children;
	while (child != NULL)
	{
		shared_ptr<XmlNode> c(new XmlNode());
		c->_init(child);
		children.push_back(c);
		child = child->next;
	}
	return children;
}
vector<shared_ptr<XmlNode> > XmlNode::getChildNodesByName(const string& name)
{
	vector<shared_ptr<XmlNode> > children;
	xmlNode * child = _element->children;
	while (child != NULL)
	{
		string cName((const char*)(child->name));
		if (cName == name)
		{
			shared_ptr<XmlNode> c(new XmlNode());
			c->_init(child);
			children.push_back(c);
		}
		child = child->next;
	}
	return children;
}
string XmlNode::getInnerText()
{
	if(getType() == XML_TEXT_NODE)
		return (const char *) _element->content;
	string textContent;
	xmlNode * child = _element->children;
	while (child != NULL)
	{
		shared_ptr<XmlNode> c(new XmlNode());
		c->_init(child);
		textContent+=c->getInnerText();
		child = child->next;
	}
	return textContent;
}
void XmlNode::addChild(shared_ptr<XmlNode> node, bool recursive)
{
	xmlAddChild(_element, node->_element);

	if (recursive)
	{
		shared_ptr<XmlNode> lastChild = getLastChild();
		shared_ptr<XmlNode> child = node->getFirstChild();
		while (child)
		{
			lastChild->addChild(child, recursive);
			child = child->getNext();
		}
	}
}

void XmlNode::addTextChild(const string& childName, const string& textContent)
{
	xmlNewTextChild(_element, NULL, BAD_CAST childName.c_str(), BAD_CAST textContent.c_str());
}

string XmlNode::getString(void)
{
	return ((const char *)(_element->content));
}


void XmlNode::_init(xmlNode * node)
{
	_element=node;
}
