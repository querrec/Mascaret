#include "Tools/Loaders/XMIParser.h"
#include <iostream>

XMIElement::XMIElement()
{
}


XMIElement::XMIElement(shared_ptr<XmlNode> node) : _root(node)
{
	_content = _root->getChild(XMIOWNEDELEMENT);
}

XMIElement::XMIElement(string nodeName, string name, string ID)
{
	_root = shared_ptr<XmlNode> (new XmlNode(nodeName));
	_root->setProperty("xmi.id", ID);

	_root->addTextChild("UML:ModelElement.name",name);

	_content = shared_ptr<XmlNode> (new XmlNode("UML:Namespace.ownedElement"));
	_root->addChild(_content);
}


//XMIElement::~XMIElement() {}

string XMIElement::getName(void)
{
	if (_root->getChild(XMIMODELELEMENTNAME))
		if (_root->getChild(XMIMODELELEMENTNAME)->getChild("text"))
			return _root->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
		else
			return "";
	else return "";
}


string XMIElement::getId(void)
{
	string id;
	_root->getPropertyString(XMIID, id);
	return id;
}


string XMIElement::getSummary(void)
{
	string summary = "";
	int i=1;
	shared_ptr<XmlNode> children = _root->getChild (XMIELTTAGGEDVALUE,i++);
	while (children)
	{
		children = children->getFirstChild();
		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMITAGGEDVALUE)
			{
				shared_ptr<XmlNode> t2 = children;
				if (t2->getChild(XMITAGGEDVALUETYPE))
				{
					shared_ptr<XmlNode> t3 = t2->getChild(XMITAGGEDVALUETYPE);
					if (t3->getChild(XMITAGGEDVALUEDEF))
					{
						string tagType = t3->getChild(XMITAGGEDVALUEDEF)->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString();
						if (tagType == "summary")
						{
							summary = t2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")->getString();
							found = true;
						}
					}
				}
			}
			children = children->getNext();
		}
		children = _root->getChild (XMIELTTAGGEDVALUE,i++);
	}
	return summary;

}


string XMIElement::getDescription(void)
{
	string description = "";
	int i=1;
	shared_ptr<XmlNode> children = _root->getChild (XMIELTTAGGEDVALUE,i++);
	while (children)
	{
		children = children->getFirstChild();
		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMITAGGEDVALUE)
			{
				shared_ptr<XmlNode> t2 = children;
				if (t2->getChild(XMITAGGEDVALUETYPE))
				{
					shared_ptr<XmlNode> t3 = t2->getChild(XMITAGGEDVALUETYPE);
					if (t3->getChild(XMITAGGEDVALUEDEF))
					{
						string tagType = t3->getChild(XMITAGGEDVALUEDEF)->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString();
						if (tagType == "documentation")
						{
							description = t2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")->getString();
							found = true;
						}
					}
				}
			}
			children = children->getNext();
		}
		children = _root->getChild (XMIELTTAGGEDVALUE,i++);
	}
	return description;

}
map<string,string> XMIElement::getTags(void)
{
	map<string,string> tags;
	shared_ptr<XmlNode> children = _root->getChild (XMIELTTAGGEDVALUE);
	if (children)
	{
		children = children->getFirstChild();
		while ( children )
		{
			if (children->getName() == XMITAGGEDVALUE)
			{
				shared_ptr<XmlNode> t2 = children;
				if (t2->getChild(XMITAGGEDVALUETYPE))
				{
					shared_ptr<XmlNode> t3 = t2->getChild(XMITAGGEDVALUETYPE);
					if (t3->getChild(XMITAGGEDVALUEDEF))
					{
						string tagType = t3->getChild(XMITAGGEDVALUEDEF)->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString();
						if (tagType != "documentation" && tagType != "summary")
						{
							tags[tagType]=t2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")->getString();
						}
					}
				}
			}
			children = children->getNext();
		}
	}
	return tags;

}

string XMIElement::getVisibility(void)
{
	string visibility;
	_root->getChild(XMIMODELELEMENTVISIBILITY)->getPropertyString(XMIVALUE,visibility);
	return visibility;

}


string XMIElement::getStereotypeRef(void)
{
	string s;

	if (_root != NULL)
	{

		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIMODELELEMENTSTEREOTYPE);
		if(xmlNode != NULL)
		{
			shared_ptr<XmlNode> xmlNode2 = xmlNode->getChild(XMISTEREOTYPE);
			if (xmlNode2 != NULL)
			{
				xmlNode2->getPropertyString(XMIIDREF,s);
			}
		}
	}
	return s;
}


string XMIElement::getExtensionRef(void)
{
	string s;
	shared_ptr<XmlNode> xmlNode = _content->getChild(XMIEXTENSION);
	if(xmlNode)
	{
		xmlNode->getChild(XMIREFERENCE)->getPropertyString(XMIIDREF,s);
	}
	return s;
}


string XMIElement::getDependencyRef(unsigned int occurence)
{
	string s;
	shared_ptr<XmlNode> xmlNode = _content->getChild(XMIMODELELTCLIENTDEPENDENCY);
	if(xmlNode)
	{
		shared_ptr<XmlNode> dep=xmlNode->getChild(XMIDEPENDENCY,occurence);
		if (dep)
			dep->getPropertyString(XMIIDREF,s);
	}
	return s;
}


bool XMIElement::getTaggedValue(const string type, string & data)
{
	if (_root)
	{
		int occurence=1;
		shared_ptr<XmlNode> xmlNode;
		while( (xmlNode = _root->getChild(XMIELTTAGGEDVALUE, occurence++)))
		{
			if (xmlNode)
			{
				int occurence2=1;
				shared_ptr<XmlNode> xmlNode2;
				while( (xmlNode2=xmlNode->getChild(XMITAGGEDVALUE,occurence2++)))
				{
					if(type==xmlNode2->getChild(XMITAGGEDVALUETYPE)->getChild(XMITAGGEDVALUEDEF)
							->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString())
					{
						shared_ptr<XmlNode> textChild;
						if ((textChild=xmlNode2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")))
							data= textChild->getString();
						return true;
					}

				}
			}
		}
	}
	return false;
}


shared_ptr<XmlNode> XMIElement::_getChildByName(string XMIElementType, string XMIElementName)
{
	shared_ptr<XmlNode> childNode;
	shared_ptr<XmlNode> children = _content->getFirstChild();

	bool found = false;
	while ( (children) && (!found) )
	{
		if (children->getName() == XMIElementType)
		{
			string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
			if (name == XMIElementName)
			{
				childNode = children;
				found = true;
			}
		}
		children = children->getNext();
	}

	return childNode;
}


/* ***************************************************** */
/* ***************************************************** */

XMIParser::XMIParser()
{
	_prefix ="";
}


//XMIParser::~XMIParser() {}

void XMIParser::setPrefix(string prefix)
{
	_prefix = prefix;
}


void XMIParser::setParser(shared_ptr<XmlParser> parser)
{
	_loader = parser;
	_root = _loader->getRoot();
	_setContent(_root->getChild(XMICONTENT));
}


bool XMIParser::parseFile(string fileName)
{
	_loader =shared_ptr<XmlParser>(new XmlParser());
	if (!_loader->parseFile(fileName))
	{
		cerr << "Error while loading XML file : " << fileName << endl;
		return false;
	}
	else
	{
		_root = _loader->getRoot();
		_setContent(_root->getChild(XMICONTENT));
		return true;
	}
}

bool XMIParser::parseNode(shared_ptr<XmlNode> rootNode)
{
	_loader =shared_ptr<XmlParser>(new XmlParser());
	_loader->createFile(rootNode->getName());
	shared_ptr<XmlNode> child = rootNode->getFirstChild();
	_root = _loader->getRoot();
	while(child)
	{
		_root->addChild(child);
		child = child->getNext();
	}
	_setContent(_root->getChild(XMICONTENT));
	return true;
}

void XMIParser::writeFile(string filename)
{
	_loader->writeFile(filename);
}

void XMIParser::createFile(string /*modelName*/)
{
	_loader =shared_ptr<XmlParser>(new XmlParser());
	_loader->createFile("XMI");
	_root = _loader->getRoot();

	string xmiVersion ("xmi.version");
	string xmiVersionValue ("1.1");
	_root->setProperty(xmiVersion, xmiVersionValue);

	string xmiNS ("xmlns:UML");
	string xmiNSValue ("//org.omg/UML/1.4");
	_root->setProperty(xmiNS, xmiNSValue);

	shared_ptr<XmlNode> header = shared_ptr<XmlNode>(new XmlNode("XMI.header"));
	_root->addChild(header);

	_content = shared_ptr<XmlNode>(new XmlNode("XMI.content"));
	_root->addChild(_content);

	//shared_ptr<XMIPackage> rootPackage = shared_ptr<XMIPackage> (new XMIPackage("UML:Model",modelName,modelName));
	//addPackage(rootPackage);

	//_setContent(_root->getChild(XMICONTENT));
}

void XMIParser::addPackage(shared_ptr<XMIPackage> package)
{
	_content->addChild(package->getRoot());
}


bool XMIParser::parseString(const string& str)
{
	_loader =shared_ptr<XmlParser>(new XmlParser());
	if (!_loader->parseString(str))
	{
		cerr << "Error while loading XML String : " << str << endl;
		return false;
	}
	else
	{
		_root = _loader->getRoot();
		_setContent(_root->getChild(XMICONTENT));
		return true;
	}

}


shared_ptr<XMIPackage> XMIParser::getPackage(string packageName)
{
	shared_ptr<XmlNode> xmlNode = _getChildByName(XMIPACKAGE, packageName);
	if (xmlNode != NULL)
	{
		shared_ptr<XMIPackage> package ( new XMIPackage(xmlNode));
		return package;
	}
	else
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIMODEL, packageName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIPackage> package ( new XMIPackage(xmlNode) );
			return package;
		}
		else
			return shared_ptr<XMIPackage>();
	}
}


shared_ptr<XMIPackage> XMIParser::getPackage()
{
	shared_ptr<XmlNode> xmlNode = _content->getChild(XMIPACKAGE);
	if (xmlNode != NULL)
	{
		shared_ptr<XMIPackage> package (new XMIPackage(xmlNode));
		return package;
	}
	else
	{
		cerr << " ######################### " << endl;
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIMODEL);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIPackage> package ( new XMIPackage(xmlNode) );
			return package;
		}
		else
			return shared_ptr<XMIPackage>();
	}
}


shared_ptr<XMIExtensions> XMIParser::getExtensions()
{
	shared_ptr<XmlNode> ext=_root->getChild(XMIEXTENSIONS);
	if (ext)
	{
		return shared_ptr<XMIExtensions>(new XMIExtensions(ext));
	}
	return shared_ptr<XMIExtensions>();
}


/* ***************************************************** */
/* ***************************************************** */

XMILinkEnd::XMILinkEnd(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMILinkEnd::~XMILinkEnd() {}

string XMILinkEnd::getParticipantId(void)
{
	string idClass;
	shared_ptr<XmlNode> linkNode = _content->getChild(XMIENDPARTICPANT);
	linkNode->getChild(XMICLASSIFIER)->getPropertyString(XMIIDREF, idClass);
	return idClass;
}


string XMILinkEnd::getMultiplicity(void)
{
	shared_ptr<XmlNode> multRange = _content->getChild(XMIENDMULTIPLICITY)->getChild(XMIMULTIPLICITY)->getChild(XMIMULTIPLICITY_RANGE);

	shared_ptr<XmlNode> mult = multRange->getChild(XMIMULTIPLICITYRANGE)->getChild(XMIMULTIPLICITYUPPER);
	if (mult->getChild("text") != NULL)
		return mult->getChild("text")->getString();
	else return "1";
}


string XMILinkEnd::getMultiplicityLower(void)
{
	shared_ptr<XmlNode> multRange = _content->getChild(XMIENDMULTIPLICITY)->getChild(XMIMULTIPLICITY)->getChild(XMIMULTIPLICITY_RANGE);

	shared_ptr<XmlNode> mult = multRange->getChild(XMIMULTIPLICITYRANGE)->getChild(XMIMULTIPLICITYLOWER);
	if (mult->getChild("text") != NULL)
		return mult->getChild("text")->getString();
	else return "1";
}


bool XMILinkEnd::isNavigable(void)
{
	string val;
	_content->getChild(XMIENDNAVIGABLE)->getPropertyString(XMIVALUE,val);
	return (val=="true");
}


bool XMILinkEnd::isComposite(void)
{
	string val;
	_content->getChild(XMIENDAGGREGATIONTYPE)->getPropertyString(XMIVALUE,val);
	return (val=="composite");
}


/* ***************************************************** */
/* ***************************************************** */

XMIRelation::XMIRelation(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root->getChild(XMICONNECTION);
}


//XMIRelation::~XMIRelation() {}

shared_ptr<XMILinkEnd> XMIRelation::getLinkEnd(string linkEndName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIEND, linkEndName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMILinkEnd> xmiLinkEnd (new XMILinkEnd(xmlNode));
			return xmiLinkEnd;
		}
		else
			return shared_ptr<XMILinkEnd>();
	}
	else
		return  shared_ptr<XMILinkEnd>();
}


shared_ptr<XMILinkEnd> XMIRelation::getLinkEnd(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIEND, occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMILinkEnd> xmiLinkEnd ( new XMILinkEnd(xmlNode) );
			return xmiLinkEnd;
		}
		else
			return shared_ptr<XMILinkEnd>();
	}
	else
		return  shared_ptr<XMILinkEnd>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIStereotype::XMIStereotype(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIStereotype::~XMIStereotype() {}

/* ***************************************************** */
/* ***************************************************** */

XMIDependency::XMIDependency(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIDependency::~XMIDependency() {}

string XMIDependency::getSupplierRef()
{
	string supplier;
	shared_ptr<XmlNode> node;
	if ((node=_root->getChild(XMIDEPENDENCYSUPPLIER)))
		node->getChild(XMIMODELELEMENT)->getPropertyString(XMIIDREF,supplier);
	return supplier;
}


string XMIDependency::getClientRef()
{
	string client;
	shared_ptr<XmlNode> node;
	if ((node=_root->getChild(XMIDEPENDENCYCLIENT)))
		node->getChild(XMIMODELELEMENT)->getPropertyString(XMIIDREF,client);
	return client;
}


/* ***************************************************** */
/* ***************************************************** */

XMIAttributeLink::XMIAttributeLink(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root->getChild(XMIATTRIBUTELINKVAL);
}


//XMIAttributeLink::~XMIAttributeLink() {}

string XMIAttributeLink::getValue(void)
{
	string value;
	value = _content->getChild(XMIINSTANCE)->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
	return value;
}


/* ***************************************************** */
/* ***************************************************** */

XMIObject::XMIObject(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root->getChild(XMIINSTANCESLOT);
}


//XMIObject::~XMIObject() {}

string XMIObject::getClassifierID(void)
{
	string idClass;
	_root->getChild(XMIINSTANCECLASS)->getChild(XMICLASSIFIER)->getPropertyString(XMIIDREF, idClass);
	return idClass;
}


shared_ptr<XMIAttributeLink> XMIObject::getAttributeLink(string attributeName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIATTRIBUTELINK, attributeName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIAttributeLink> attribute ( new XMIAttributeLink(xmlNode));
			return attribute;
		}
		else
			return shared_ptr<XMIAttributeLink>();
	}
	else
		return shared_ptr<XMIAttributeLink>();
}


shared_ptr<XMIAttributeLink> XMIObject::getAttributeLink(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIATTRIBUTELINK,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIAttributeLink> attribute (new XMIAttributeLink(xmlNode));
			return attribute;
		}
		else
			return shared_ptr<XMIAttributeLink>();
	}
	else
		return shared_ptr<XMIAttributeLink>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIAttribute::XMIAttribute(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIAttribute::~XMIAttribute() {}

string XMIAttribute::getTypeRef(void)
{
	shared_ptr<XmlNode> typeNode = _content->getChild(XMISTRUCTURALTYPE);
	string idType;
	typeNode->getChild(XMICLASSIFIER)->getPropertyString(XMIIDREF, idType);
	return idType;
}


bool XMIAttribute::hasInitialValue(void)
{
	shared_ptr<XmlNode> initNode = _content->getChild(XMIINITIALVALUE);
	if (initNode)
	{
		shared_ptr<XmlNode> expression = initNode->getChild(XMIATTREXPRESSION);
		if (expression)
		{
			shared_ptr<XmlNode> body = expression->getChild(XMIEXPRESSIONBODY);
			if (body)
			{
				if (body->getChild("text"))
				{
					string value = body->getChild("text")->getString();
					if (value != "") return true;
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
	return false;
}


string XMIAttribute::getInitialValue(void)
{
	shared_ptr<XmlNode> initNode = _content->getChild(XMIINITIALVALUE);
	if (initNode)
	{
		shared_ptr<XmlNode> expression = initNode->getChild(XMIATTREXPRESSION);
		if (expression)
		{
			shared_ptr<XmlNode> body = expression->getChild(XMIEXPRESSIONBODY);
			if (body)
			{
				if (body->getChild("text"))
				{
					string value = body->getChild("text")->getString();
					if (value != "") return value;
					else return "";
				}
				else return "";
			}
			else return "";
		}
		else return "";
	}
	else return "";
	return "";
}


string XMIAttribute::getMultiplicity(void)
{
	string idClass;
	shared_ptr<XmlNode> multRange = _content->getChild(XMIATTMULTIPLICITY)->getChild(XMIMULTIPLICITY)->getChild(XMIMULTIPLICITY_RANGE);

	shared_ptr<XmlNode> mult = multRange->getChild(XMIMULTIPLICITYRANGE)->getChild(XMIMULTIPLICITYUPPER);
	if (mult->getChild("text"))
		return mult->getChild("text")->getString();
	else return "1";
}


string XMIAttribute::getMultiplicityLower(void)
{
	string idClass;
	shared_ptr<XmlNode> multRange = _content->getChild(XMIATTMULTIPLICITY)->getChild(XMIMULTIPLICITY)->getChild(XMIMULTIPLICITY_RANGE);

	shared_ptr<XmlNode> mult = multRange->getChild(XMIMULTIPLICITYRANGE)->getChild(XMIMULTIPLICITYLOWER);
	return mult->getChild("text")->getString();
}


/* ***************************************************** */
/* ***************************************************** */

XMIOperation::XMIOperation(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIOperation::~XMIOperation() {}

shared_ptr<XMIParameter> XMIOperation::getParameter(unsigned int occurence)
{
	if (_content!=NULL)
	{

		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIOPERATIONPARAMETER)->getChild(XMIPARAMETER,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIParameter> parameter ( new XMIParameter(xmlNode) );
			return parameter;
		}
		else
		{
			return shared_ptr<XMIParameter>();
		}
	}
	else
		return shared_ptr<XMIParameter>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIParameter::XMIParameter(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content=_root;
}


//XMIParameter::~XMIParameter() {}

string XMIParameter::getKind(void)
{
	string kind;
	_content->getChild(XMIPARAMETERKIND)->getPropertyString(XMIVALUE, kind);
	return kind;
}


string XMIParameter::getTypeRef(void)
{
	shared_ptr<XmlNode> typeNode = _content->getChild(XMIPARAMETERTYPE);
	string idType;
	typeNode->getChild(XMICLASSIFIER)->getPropertyString(XMIIDREF, idType);
	return idType;
}


/* ***************************************************** */
/* ***************************************************** */

XMITransition::XMITransition(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMITransition::~XMITransition() {}

string XMITransition::getSourceID(void)
{
	string idSource;
	_content->getChild(XMITRANSITIONSOURCE)->getChild(XMISTATEVERTEX)->getPropertyString(XMIIDREF, idSource);
	return idSource;
}


string XMITransition::getTargetID(void)
{
	string idTarget;
	_content->getChild(XMITRANSITIONTARGET)->getChild(XMISTATEVERTEX)->getPropertyString(XMIIDREF, idTarget);
	return idTarget;
}


string XMITransition::getTriggerID(void)
{
	string idTarget;
	if (_content->getChild(XMITRANSITIONTRIGGER))
		if (_content->getChild(XMITRANSITIONTRIGGER)->getChild(XMIEVENT))
			_content->getChild(XMITRANSITIONTRIGGER)->getChild(XMIEVENT)->getPropertyString(XMIIDREF, idTarget);
	return idTarget;
}


string XMITransition::getGuardExpression(void)
{
	string expression="";
	if (_content->getChild(XMITRANSITIONGUARD))
		if (_content->getChild(XMITRANSITIONGUARD)->getChild(XMIGUARD))
			if (_content->getChild(XMITRANSITIONGUARD)->getChild(XMIGUARD)->getChild(XMIGUARDEXPRESSION))
				if (_content->getChild(XMITRANSITIONGUARD)->getChild(XMIGUARD)->getChild(XMIGUARDEXPRESSION)->getChild(XMIBOOLEANEXPRESSION))
					expression = _content->getChild(XMITRANSITIONGUARD)->getChild(XMIGUARD)->getChild(XMIGUARDEXPRESSION)->getChild(XMIBOOLEANEXPRESSION)->getChild(XMIEXPRESSIONBODY)->getChild("text")->getString();
	return expression;

}

bool XMITransition::hasEffect(void)
{
	if (_content->getChild(XMITRANSITIONEFFECT))
		return true;
	else return false;
}

string XMITransition::getSendSignalRef(void)
{
	string idSig;
	shared_ptr<XmlNode> tmp = _content->getChild("Transition.effect");
	tmp = tmp->getChild(XMISENDACTI0N);
	if (tmp)
	{
		tmp=tmp->getChild(XMISENDACTI0NSIG);
		if(tmp)
		{
			tmp=tmp->getChild(XMISIGNAL);
			if (tmp)
			{
				tmp->getPropertyString(XMIIDREF, idSig);
				return idSig;
			}
			else return "";
		}
		else return "";
	}
	else return "";
}

string XMITransition::getCallOperationRef(void)
{
	string idOper;
	shared_ptr<XmlNode> tmp = _content->getChild("Transition.effect");
	if (tmp)
	{
		tmp = tmp->getChild(XMICALLACTION);
		if (tmp)
		{
			tmp=tmp->getChild(XMICALLACTIONOP);
			if(tmp)
			{
				tmp=tmp->getChild(XMIOPERATION);
				if (tmp)
				{
					tmp->getPropertyString(XMIIDREF, idOper);
					return idOper;
				}
				else return "";
			}
			else return "";
		}
		else return "";
	}
	else return "";
}


/* ***************************************************** */
/* ***************************************************** */

XMISimpleState::XMISimpleState(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMISimpleState::~XMISimpleState() {}

bool XMISimpleState::hasDoActivity(void)
{
	if (_content->getChild(XMIDOACTIVITY))
	{
		return true;
	}
	else return false;
}


string XMISimpleState::getDoActivityOperationRef(void)
{
	if (hasDoActivity())
	{
		return _content->getChild(XMIDOACTIVITY)->getChild(XMICALLACTION)->getChild(XMICALLACTIONOP)->getChild(XMIOPERATION)->getProperty(XMIIDREF);
	}
	else return "";
}


bool XMISimpleState::hasEntryActivity(void)
{
	if (_content->getChild(XMIENTRYACTIVITY))
	{
		return true;
	}
	else return false;
}


string XMISimpleState::getEntryActivityOperationRef(void)
{
	if (hasEntryActivity())
	{
		return _content->getChild(XMIENTRYACTIVITY)->getChild(XMICALLACTION)->getChild(XMICALLACTIONOP)->getChild(XMIOPERATION)->getProperty(XMIIDREF);
	}
	else return "";
}


bool XMISimpleState::hasExitActivity(void)
{
	if (_content->getChild(XMIEXITACTIVITY))
	{
		return true;
	}
	else return false;
}


string XMISimpleState::getExitActivityOperationRef(void)
{
	if (hasExitActivity())
	{
		return _content->getChild(XMIEXITACTIVITY)->getChild(XMICALLACTION)->getChild(XMICALLACTIONOP)->getChild(XMIOPERATION)->getProperty(XMIIDREF);
	}
	else return "";
}


/* ***************************************************** */
/* ***************************************************** */

XMIPseudoState::XMIPseudoState(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIPseudoState::~XMIPseudoState() {}

string XMIPseudoState::getKind(shared_ptr<XmlParser> loader)
{
	string kind;

	if (_content->getChild("Pseudostate.kind")!=NULL)
	{
		kind = _content->getChild("Pseudostate.kind")->getProperty("xmi.value");
	}
	else
	{
		string id = _content->getChild("XMI.extension")->getChild("XMI.reference")->getProperty("xmi.idref");
		shared_ptr<XmlNode> tmp = loader->getRoot()->getChild("XMI.extensions");
		shared_ptr<XmlNode> childExt = tmp->getFirstChild();
		bool foundState = false;
		while ( (childExt) && (!foundState) )
		{
			if (childExt->getName() == "ObjingPseudoState")
			{
				if (childExt->getProperty("xmi.id") == id)
				{
					foundState = true;
					kind = childExt->getChild("ObjingPseudoState.kind")->getProperty("xmi.value");
				}
			}
			childExt = childExt->getNext();
		}

	}
	return kind;
}


/* ***************************************************** */
/* ***************************************************** */

XMIActionState::XMIActionState(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIActionState::~XMIActionState() {}

string XMIActionState::getArguments(void)
{
	bool dynamic;
	_content->getChild("ActionState.isDynamic")->getPropertyBoolean(XMIVALUE,dynamic);
	if (dynamic)
	{
		shared_ptr<XmlNode> tmp = _content->getChild("ActionState.dynamicArguments");
		if (tmp)
		{
			tmp = tmp->getChild("ArgListsExpression");
			if (tmp)
			{
				tmp = tmp->getChild("Expression.body");
				if (tmp)
				{
					tmp = tmp->getChild("text");
					if (tmp)
					{
						return tmp->getString();
					}
					else return "";
				}
				else
					return "";
			}
			else
				return "";
		}
		else return "";
	}
	else
		return "";
}


string XMIActionState::getSendSignalRef(void)
{
	string idSig;
	shared_ptr<XmlNode> tmp = _content->getChild("State.internalTransition");
	if (tmp)
	{
		tmp = tmp->getChild(XMITRANSITION);
		if (tmp)
		{
			tmp = tmp->getChild("Transition.effect");
			if (tmp)
			{
				tmp = tmp->getChild(XMISENDACTI0N);
				if (tmp)
				{
					tmp=tmp->getChild(XMISENDACTI0NSIG);
					if(tmp)
					{
						tmp=tmp->getChild(XMISIGNAL);
						if (tmp)
						{
							tmp->getPropertyString(XMIIDREF, idSig);
							return idSig;
						}
						else return "";
					}
					else return "";
				}
				else return "";
			}
			else return "";
		}
		else return "";
	}
	else return "";
}

string XMIActionState::getCallOperationRef(void)
{
	string idOper;
	shared_ptr<XmlNode> tmp = _content->getChild("State.internalTransition");
	if (tmp)
	{
		tmp = tmp->getChild(XMITRANSITION);
		if (tmp)
		{
			tmp = tmp->getChild("Transition.effect");
			if (tmp)
			{
				tmp = tmp->getChild(XMICALLACTION);
				if (tmp)
				{
					tmp=tmp->getChild(XMICALLACTIONOP);
					if(tmp)
					{
						tmp=tmp->getChild(XMIOPERATION);
						if (tmp)
						{
							tmp->getPropertyString(XMIIDREF, idOper);
							return idOper;
						}
						else return "";
					}
					else return "";
				}
				else return "";
			}
			else return "";
		}
		else return "";
	}
	else return "";
}


/* ***************************************************** */
/* ***************************************************** */

XMIObjectFlowState::XMIObjectFlowState(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
	inParticularState = false;
	if(_root->getChild(XMIMODELELEMENTCOMMENT))
	{inParticularState = true;}
}


//XMIObjectFlowState::~XMIObjectFlowState() {}

string XMIObjectFlowState::getType(void)
{
	string idClass;
	shared_ptr<XmlNode> type = _root->getChild(XMIOBJECTFLOWSTATETYPE);
	if (type)
	{
		shared_ptr<XmlNode> classifier = type->getChild(XMICLASSIFIER);
		if (classifier) classifier->getPropertyString(XMIIDREF, idClass);
	}
	return idClass;
}


string XMIObjectFlowState::getState(void)
{
	//Renvoie l'etat dans lequel on met l'ObjectFlowState
	//Pour l'instant, on rcupre la valeur du commentaire associ
	string state;

	if(_root->getChild(XMIMODELELEMENTCOMMENT))
	{
		state = _root->getChild(XMIMODELELEMENTCOMMENT)->getChild(XMICOMMENT)->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
	}
	return state;
}


/* ***************************************************** */
/* ***************************************************** */

XMIPartition::XMIPartition(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root->getChild(XMIPARTITIONCONTENT);
}


//XMIPartition::~XMIPartition() {}

string XMIPartition::getContent(unsigned int iContent)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIMODELELEMENT,iContent);

		if (xmlNode != NULL)
		{
			return xmlNode->getProperty(XMIIDREF);
		}
		else
			return "";
	}
	else
		return "";

}


/* ***************************************************** */
/* ***************************************************** */
XMIActivityGraph::XMIActivityGraph(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIActivityGraph::~XMIActivityGraph() {}

string XMIActivityGraph::getContextId(void)
{
	string idClass;
	_root->getChild(XMISTATEMACHINECONTEXT)->getChild(XMIMODELELEMENT)->getPropertyString(XMIIDREF, idClass);
	return idClass;
}


shared_ptr<XMIPartition> XMIActivityGraph::getPartition(string partitionName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode ;
		shared_ptr<XmlNode> children = _content->getChild(XMIPARTITIONS)->getChild(XMIPARTITION);

		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMIPARTITION)
			{
				string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
				if (name == partitionName)
				{
					xmlNode = children;
					found = true;
				}
			}
			children = children->getNext();
		}
		if (xmlNode != NULL)
		{
			shared_ptr<XMIPartition> xmiPartition ( new XMIPartition(xmlNode) );
			return xmiPartition;
		}
		else
			return shared_ptr<XMIPartition>();
	}
	else
		return shared_ptr<XMIPartition>();
}


shared_ptr<XMIPartition> XMIActivityGraph::getPartition(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIPARTITIONS);
		if (xmlNode != NULL)
		{
			xmlNode = xmlNode->getChild(XMIPARTITION,occurence);

			if (xmlNode != NULL)
			{
				shared_ptr<XMIPartition> xmiPartition ( new XMIPartition(xmlNode));
				return xmiPartition;
			}
			else
				return shared_ptr<XMIPartition>();
		}
		else
			return shared_ptr<XMIPartition>();
	}
	else
		return shared_ptr<XMIPartition>();
}


shared_ptr<XMIPseudoState> XMIActivityGraph::getPseudoState(string /*pseudoStateName*/)
{
	cerr << "Not implemented yet" << endl;
	return shared_ptr<XMIPseudoState>();
}


shared_ptr<XMIPseudoState> XMIActivityGraph::getPseudoState(unsigned int occurence)
{
	shared_ptr<XMIPseudoState> xmiPseudoState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISUBACTIVITYSTATE);

		if (child )
		{
			unsigned int nPseudoState = 0;
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childPS = children->getChild(XMIPSEUDOSTATE);
					while (childPS)
					{
						if (childPS->getName() == XMIPSEUDOSTATE)
						{
							nPseudoState++;
							if (nPseudoState == occurence)
							{
								xmiPseudoState =shared_ptr<XMIPseudoState>( new XMIPseudoState(childPS)) ;
							}
						}
						childPS = childPS->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiPseudoState;
}


shared_ptr<XMIPseudoState> XMIActivityGraph::getFinalState(unsigned int occurence)
{
	shared_ptr<XMIPseudoState> xmiPseudoState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode>  child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISUBACTIVITYSTATE);

		if (child)
		{
			unsigned int nPseudoState = 0;
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childPS = children->getChild(XMIFINALSTATE);
					while (childPS)
					{
						if (childPS->getName() == XMIFINALSTATE)
						{
							nPseudoState++;
							if (nPseudoState == occurence)
							{
								xmiPseudoState =shared_ptr<XMIPseudoState>( new XMIPseudoState(childPS));
							}
						}
						childPS = childPS->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiPseudoState;
}


shared_ptr<XMIActionState> XMIActivityGraph::getActionState(string /*actionStateName*/)
{
	cerr << "Not implemented yet" << endl;
	return shared_ptr<XMIActionState>();
}


shared_ptr<XMIActionState> XMIActivityGraph::getActionState(unsigned int occurence)
{
	shared_ptr<XMIActionState> xmiActionState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISUBACTIVITYSTATE);

		unsigned int nActionState = 0;
		if (child )
		{
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childAS = children->getChild(XMIACTIONSTATE);
					while (childAS)
					{
						if (childAS->getName() == XMIACTIONSTATE)
						{
							nActionState++;
							if (nActionState == occurence)
							{
								xmiActionState =shared_ptr<XMIActionState>( new XMIActionState(childAS));
							}
						}
						childAS = childAS->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiActionState;
}


shared_ptr<XMIObjectFlowState> XMIActivityGraph::getObjectFlowState(string /*objectFlowStateName*/)
{
	cerr << "Not implemented yet" << endl;
	return shared_ptr<XMIObjectFlowState>();
}


shared_ptr<XMIObjectFlowState> XMIActivityGraph::getObjectFlowState(unsigned int occurence)
{
	shared_ptr<XMIObjectFlowState> xmiObjectFlowState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISUBACTIVITYSTATE);

		if (child != NULL)
		{
			unsigned int nObjectFlow = 0;
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childOF = children->getChild(XMIOBJECTFLOWSTATE);
					while (childOF)
					{
						if (childOF->getName() == XMIOBJECTFLOWSTATE)
						{
							nObjectFlow++;
							if (nObjectFlow == occurence)
							{
								xmiObjectFlowState =shared_ptr<XMIObjectFlowState>(new XMIObjectFlowState(childOF));
							}
						}
						childOF = childOF->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiObjectFlowState;
}


shared_ptr<XMITransition> XMIActivityGraph::getTransition(string transitionName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode ;
		shared_ptr<XmlNode> children = _content->getChild(XMITRANSITIONS);

		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMITRANSITION)
			{
				string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
				if (name == transitionName)
				{
					xmlNode = children;
					found = true;
				}
			}
			children = children->getNext();
		}
		if (xmlNode != NULL)
		{
			shared_ptr<XMITransition> xmiTransition (new XMITransition(xmlNode));
			return xmiTransition;
		}
		else
			return shared_ptr<XMITransition>();
	}
	else
		return shared_ptr<XMITransition>();
}


shared_ptr<XMITransition> XMIActivityGraph::getTransition(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMITRANSITIONS);
		xmlNode = xmlNode->getChild(XMITRANSITION,occurence);

		if (xmlNode != NULL)
		{
			shared_ptr<XMITransition> xmiTransition(new XMITransition(xmlNode));
			return xmiTransition;
		}
		else
			return shared_ptr<XMITransition>();
	}
	else
		return shared_ptr<XMITransition>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIStateMachine::XMIStateMachine(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root;
}


//XMIStateMachine::~XMIStateMachine() {}

string XMIStateMachine::getContextClassifierID(void)
{
	string idClass;
	_root->getChild(XMISTATEMACHINECONTEXT)->getChild(XMIMODELELEMENT)->getPropertyString(XMIIDREF, idClass);
	return idClass;
}


shared_ptr<XMISimpleState> XMIStateMachine::getSimpleState(string stateName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode ;
		shared_ptr<XmlNode> children = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISTATEMACHINECS)->getChild(XMISTATESUBVERTEX);

		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMISIMPLESTATE)
			{
				string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
				if (name == stateName)
				{
					xmlNode = children;
					found = true;
				}
			}
			children = children->getNext();
		}
		if (xmlNode != NULL)
		{
			shared_ptr<XMISimpleState> xmiState ( new XMISimpleState(xmlNode));
			return xmiState;
		}
		else
			return  shared_ptr<XMISimpleState>();
	}
	else
		return  shared_ptr<XMISimpleState>();
}


shared_ptr<XMISimpleState> XMIStateMachine::getSimpleState(unsigned int occurence)
{
	shared_ptr<XMISimpleState> xmiSimpleState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISTATEMACHINECS);

		if (child != NULL)
		{
			unsigned int nSimpleState = 0;
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childSS = children->getChild(XMISIMPLESTATE);
					while (childSS)
					{
						if (childSS->getName() == XMISIMPLESTATE)
						{
							nSimpleState++;
							if (nSimpleState == occurence)
							{
								xmiSimpleState = shared_ptr<XMISimpleState>(new XMISimpleState(childSS));
							}
						}
						childSS = childSS->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiSimpleState;
}


shared_ptr<XMIPseudoState> XMIStateMachine::getPseudoState(unsigned int occurence)
{
	shared_ptr<XMIPseudoState> xmiPseudoState ;
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> child = _content->getChild(XMISTATEMACHINETOP)->getChild(XMISTATEMACHINECS);

		if (child != NULL)
		{
			unsigned int nPseudoState = 0;
			shared_ptr<XmlNode> children = child->getChild(XMICOMPOSSTATEVERTEX);
			while (children)
			{
				if (children->getName() == XMICOMPOSSTATEVERTEX)
				{
					shared_ptr<XmlNode> childPS = children->getChild(XMIPSEUDOSTATE);
					while (childPS)
					{
						if (childPS->getName() == XMIPSEUDOSTATE)
						{
							nPseudoState++;
							if (nPseudoState == occurence)
							{
								xmiPseudoState = shared_ptr<XMIPseudoState>(new XMIPseudoState(childPS));
							}
						}
						childPS = childPS->getNext();
					}
				}
				children = children->getNext();
			}
		}
	}
	return xmiPseudoState;
}


shared_ptr<XMITransition> XMIStateMachine::getTransition(string transitionName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode ;
		shared_ptr<XmlNode> children = _content->getChild(XMITRANSITIONS);

		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMITRANSITION)
			{
				string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
				if (name == transitionName)
				{
					xmlNode = children;
					found = true;
				}
			}
			children = children->getNext();
		}
		if (xmlNode != NULL)
		{
			shared_ptr<XMITransition> xmiTransition (new XMITransition(xmlNode));
			return xmiTransition;
		}
		else
			return shared_ptr<XMITransition>();
	}
	else
		return shared_ptr<XMITransition>();
}


shared_ptr<XMITransition> XMIStateMachine::getTransition(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMITRANSITIONS);
		xmlNode = xmlNode->getChild(XMITRANSITION,occurence);

		if (xmlNode != NULL)
		{
			shared_ptr<XMITransition> xmiTransition (new XMITransition(xmlNode) );
			return xmiTransition;
		}
		else
			return shared_ptr<XMITransition>();
	}
	else
		return shared_ptr<XMITransition>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIDataType::XMIDataType(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
}


//XMIDataType::~XMIDataType() {}

/* ***************************************************** */
/* ***************************************************** */

XMIClasse::XMIClasse(shared_ptr<XmlNode> node) :
		XMIElement(node)
{
	_content = _root->getChild(XMIFEATURE);
}

XMIClasse::XMIClasse(string name, string ID) :
		XMIElement("UML:Class",name, ID)
{
	//_content = _root->getChild(XMIFEATURE);
}


//XMIClasse::~XMIClasse() {}

vector<string> XMIClasse::getParentsID(void)
{
	vector<string> parents;

	shared_ptr<XmlNode> children = _root->getChild(XMIOWNEDELEMENT);
	children = children->getFirstChild();
	while (children)
	{
		if (children->getName() == XMIGENERALIZATION)
		{
			shared_ptr<XmlNode> tmp;
			// Lecture parent
			string idParent;
			tmp = children->getChild(XMIGENERALIZATIONPARENT);
			if (tmp)
			{
				tmp->getChild("GeneralizableElement")->getPropertyString(XMIIDREF, idParent);
				parents.push_back(idParent);

			}
		}
		children = children->getNext();
	}
	return parents;

}


vector<string> XMIClasse::getChildsID(void)
{
	vector<string> childs;

	shared_ptr<XmlNode> children = _root->getChild(XMIOWNEDELEMENT);
	children = children->getFirstChild();
	while (children)
	{
		if (children->getName() == XMIGENERALIZATION)
		{
			shared_ptr<XmlNode> tmp;
			// Lecture fille
			string idChild;
			tmp = children->getChild(XMIGENERALIZATIONCHILD);
			if (tmp)
			{
				tmp->getChild("GeneralizableElement")->getPropertyString(XMIIDREF, idChild);
				childs.push_back(idChild);
			}
		}
		children = children->getNext();
	}
	return childs;
}

void XMIClasse::addComment(string comment, string commentID)
{
	shared_ptr<XmlNode> meTaggedNode = shared_ptr<XmlNode>(new XmlNode("UML:ModelElement.taggedValue"));
	shared_ptr<XmlNode> taggedNode = shared_ptr<XmlNode>(new XmlNode("UML:TaggedValue"));
	taggedNode->setProperty("xmi.id",commentID);
	shared_ptr<XmlNode> taggedTypeNode = shared_ptr<XmlNode>(new XmlNode("UML:TaggedValue.type"));
	taggedNode->addChild(taggedTypeNode);
	shared_ptr<XmlNode> taggedDefinitionNode = shared_ptr<XmlNode>(new XmlNode("UML:TagDefinition"));
	taggedTypeNode->addChild(taggedDefinitionNode);
	taggedDefinitionNode->addTextChild("UML:TagDefinition.tagType","documentation");
	taggedNode->addTextChild("UML:TaggedValue.dataValue",comment);
	meTaggedNode->addChild(taggedNode);
	_root->addChild(meTaggedNode);
}

void XMIClasse::addGeneralisation(string genID, string parentID, string myID)
{
	shared_ptr<XmlNode> genElemNode = shared_ptr<XmlNode>(new XmlNode("UML:GeneralizableElement.generalization"));
	_root->addChild(genElemNode);
	shared_ptr<XmlNode> genNode = shared_ptr<XmlNode>(new XmlNode("UML:Generalization"));
	genNode->setProperty("xmi.idref",genID);
	genElemNode->addChild(genNode);

	shared_ptr<XmlNode> genNode2 = shared_ptr<XmlNode>(new XmlNode("UML:Generalization"));
	genNode2->setProperty("xmi.id",genID);
	genNode2->addTextChild("UML:ModelElement.name","Generalization");

	shared_ptr<XmlNode> childNode = shared_ptr<XmlNode>(new XmlNode("UML:Generalization.child"));
	shared_ptr<XmlNode> genElemChildNode = shared_ptr<XmlNode>(new XmlNode("UML:GeneralizableElement"));
	genElemChildNode->setProperty("xmi.idref",myID);
	childNode->addChild(genElemChildNode);
	genNode2->addChild(childNode);

	shared_ptr<XmlNode> parentNode = shared_ptr<XmlNode>(new XmlNode("UML:Generalization.parent"));
	shared_ptr<XmlNode> genElemParentNode = shared_ptr<XmlNode>(new XmlNode("UML:GeneralizableElement"));
	genElemParentNode->setProperty("xmi.idref",parentID);
	parentNode->addChild(genElemParentNode);
	genNode2->addChild(parentNode);

	_content->addChild(genNode2);
}


shared_ptr<XMIAttribute> XMIClasse::getAttribute(string attributeName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIATTRIBUTE, attributeName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIAttribute> attribute ( new XMIAttribute(xmlNode) );
			return attribute;
		}
		else
			return shared_ptr<XMIAttribute>();
	}
	else
		return shared_ptr<XMIAttribute>();
}


shared_ptr<XMIAttribute> XMIClasse::getAttribute(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIATTRIBUTE,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIAttribute> attribute (new XMIAttribute(xmlNode));
			return attribute;
		}
		else
			return shared_ptr<XMIAttribute>();
	}
	else
		return shared_ptr<XMIAttribute>();
}


shared_ptr<XMIOperation> XMIClasse::getOperation(string operationName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIOPERATION, operationName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIOperation> operation ( new XMIOperation(xmlNode));
			return operation;
		}
		else
			return shared_ptr<XMIOperation>();
	}
	else
		return shared_ptr<XMIOperation>();
}


shared_ptr<XMIOperation> XMIClasse::getOperation(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIOPERATION,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIOperation> operation ( new XMIOperation(xmlNode));
			return operation;
		}
		else
			return shared_ptr<XMIOperation>();
	}
	else
		return shared_ptr<XMIOperation>();
}


shared_ptr<XMIClasse> XMIClasse::getClasse(string classeName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode ;
		shared_ptr<XmlNode> children = _root->getChild(XMIOWNEDELEMENT)->getFirstChild();

		bool found = false;
		while ( (children) && (!found) )
		{
			if (children->getName() == XMICLASS)
			{
				string name = children->getChild(XMIMODELELEMENTNAME)->getChild("text")->getString();
				if (name == classeName)
				{
					xmlNode = children;
					found = true;
				}
			}
			children = children->getNext();
		}
		if (xmlNode != NULL)
		{
			shared_ptr<XMIClasse> xmiClasse ( new XMIClasse(xmlNode) );
			return xmiClasse;
		}
		else
			return shared_ptr<XMIClasse>();
	}
	else
		return shared_ptr<XMIClasse>();
}


shared_ptr<XMIClasse> XMIClasse::getClasse(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIOWNEDELEMENT);
		xmlNode = xmlNode->getChild(XMICLASS,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIClasse> xmiClasse ( new XMIClasse(xmlNode));
			return xmiClasse;
		}
		else
			return shared_ptr<XMIClasse>();
	}
	else
		return shared_ptr<XMIClasse>();
}


shared_ptr<XMISignalEvent> XMIClasse::getSignalEvent(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIOWNEDELEMENT);
		xmlNode = xmlNode->getChild(XMISIGNALEVENT,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMISignalEvent> xmiSignalEvent (new XMISignalEvent(xmlNode));
			return xmiSignalEvent;
		}
		else
			return shared_ptr<XMISignalEvent>();
	}
	else
		return shared_ptr<XMISignalEvent>();
}


shared_ptr<XMITimeEvent> XMIClasse::getTimeEvent(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIOWNEDELEMENT);
		xmlNode = xmlNode->getChild(XMITIMEEVENT,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMITimeEvent> xmiTimeEvent ( new XMITimeEvent(xmlNode));
			return xmiTimeEvent;
		}
		else
			return shared_ptr<XMITimeEvent>();
	}
	else
		return shared_ptr<XMITimeEvent>();
}


shared_ptr<XMIChangeEvent> XMIClasse::getChangeEvent(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIOWNEDELEMENT);
		xmlNode = xmlNode->getChild(XMICHANGEEVENT,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIChangeEvent> xmiChangeEvent ( new XMIChangeEvent(xmlNode));
			return xmiChangeEvent;
		}
		else
			return shared_ptr<XMIChangeEvent>();
	}
	else
		return shared_ptr<XMIChangeEvent>();
}


shared_ptr<XMIDataType> XMIClasse::getDataType(string dataTypeName)
{

	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIDATATYPE, dataTypeName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIDataType> xmiDataType ( new XMIDataType(xmlNode));
			return xmiDataType;
		}
		else
			return shared_ptr<XMIDataType>();
	}
	else
		return shared_ptr<XMIDataType>();
}


shared_ptr<XMIDataType> XMIClasse::getDataType(unsigned int occurence)
{
	if (_content!=NULL)
	{
		int iElem = 1;
		bool found = true;
		while (found)
		{
			shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
			if (element !=NULL)
			{
				shared_ptr<XmlNode> xmlNode = element->getChild(XMIDATATYPE, occurence);
				if (xmlNode != NULL)
				{
					shared_ptr<XMIDataType> xmiDataType ( new XMIDataType(xmlNode));
					return xmiDataType;
				}
			}
			else
				found = false;
		}
	}

	return shared_ptr<XMIDataType>();
}


shared_ptr<XMICallEvent> XMIClasse::getCallEvent(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _root->getChild(XMIOWNEDELEMENT);
		xmlNode = xmlNode->getChild(XMICALLEVENT,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMICallEvent> xmiCallEvent ( new XMICallEvent(xmlNode));
			return xmiCallEvent;
		}
		else
			return shared_ptr<XMICallEvent>();
	}
	else
		return shared_ptr<XMICallEvent>();
}


/* ***************************************************** */
/* ***************************************************** */

XMIPackage::XMIPackage(shared_ptr<XmlNode> node) :
XMIElement(node)
{
}

XMIPackage::XMIPackage(string nodeName, string packageName, string ID) :
		XMIElement(nodeName, packageName, ID)
{
}


//XMIPackage::~XMIPackage() {}

shared_ptr<XMIPackage> XMIPackage::getPackage(string packageName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIPACKAGE, packageName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIPackage> package ( new XMIPackage(xmlNode));
			return package;
		}
		else
			return shared_ptr<XMIPackage>();
	}
	else
		return shared_ptr<XMIPackage>();
}


shared_ptr<XMIPackage> XMIPackage::getPackage(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIPACKAGE,occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIPackage> package ( new XMIPackage(xmlNode));
			return package;
		}
		else
			return shared_ptr<XMIPackage>();
	}
	else
		return shared_ptr<XMIPackage>();
}


shared_ptr<XMIDataType> XMIPackage::getDataType(string dataTypeName)
{

	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIDATATYPE, dataTypeName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIDataType> xmiDataType ( new XMIDataType(xmlNode));
			return xmiDataType;
		}
		else
			return shared_ptr<XMIDataType>();
	}
	else
		return shared_ptr<XMIDataType>();
}


shared_ptr<XMIDataType> XMIPackage::getDataType(unsigned int occurence)
{
	if (_content!=NULL)
	{
		int iElem = 1;
		bool found = true;
                int nbChildRed = 0;
		while (found)
		{
			shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
			if (element !=NULL)
			{
				shared_ptr<XmlNode> xmlNode = element->getFirstChild();
                                while (xmlNode != NULL)
                                {
                                   if (xmlNode->getName() == XMIDATATYPE)
                                   {
                                      nbChildRed++;
                                      if (occurence == nbChildRed)
                                      {
					   shared_ptr<XMIDataType> xmiDataType ( new XMIDataType(xmlNode));
					   return xmiDataType;
                                      }
                                   }
                                   xmlNode = xmlNode->getNext();
                                }
                                /*
				shared_ptr<XmlNode> xmlNode = element->getChild(XMIDATATYPE, occurence);
				if (xmlNode != NULL)
				{
					shared_ptr<XMIDataType> xmiDataType ( new XMIDataType(xmlNode));
					return xmiDataType;
				}
                                */
			}
			else
				found = false;
		}
	}

	return shared_ptr<XMIDataType>();
}


shared_ptr<XMIClasse> XMIPackage::getClasse(string classeName, bool recursive)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMICLASS, classeName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIClasse> xmiClasse ( new XMIClasse(xmlNode));
			return xmiClasse;
		}
		else
		{
			if (recursive)
			{

				unsigned int nPackage = 1;
				shared_ptr<XMIPackage>xmiPackage;
				bool found2 = true;
				while (found2)
				{
					xmiPackage = getPackage(nPackage++);
					if (xmiPackage != NULL)
					{
						shared_ptr<XMIClasse> xmiClasse = xmiPackage->getClasse(classeName,true);
						if (xmiClasse) return xmiClasse;
					}
					else
						found2 = false;
				}
				return shared_ptr<XMIClasse>();
			}
			else
				return shared_ptr<XMIClasse>();
		}
	}
	else
		return shared_ptr<XMIClasse>();
}


void XMIPackage::addClasse(shared_ptr<XMIClasse> classe)
{
	_content->addChild(classe->getRoot());
}

shared_ptr<XMIClasse> XMIPackage::getClasse(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMICLASS, occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIClasse> xmiClasse ( new XMIClasse(xmlNode));
			return xmiClasse;
		}
		else
			return shared_ptr<XMIClasse>();
	}
	else
		return shared_ptr<XMIClasse>();
}


shared_ptr<XMISignal> XMIPackage::getSignal(string signalName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMISIGNAL, signalName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMISignal> xmiSignal ( new XMISignal(xmlNode));
			return xmiSignal;
		}
		else
			return shared_ptr<XMISignal>();
	}
	else
		return shared_ptr<XMISignal>();
}


shared_ptr<XMISignal> XMIPackage::getSignal(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMISIGNAL, occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMISignal> xmiSignal ( new XMISignal(xmlNode));
			return xmiSignal;
		}
		else
			return shared_ptr<XMISignal>();
	}
	else
		return shared_ptr<XMISignal>();
}


shared_ptr<XMIRelation> XMIPackage::getRelation(string relationName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIASSOCIATION, relationName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIRelation> xmiRelation ( new XMIRelation(xmlNode));
			return xmiRelation;
		}
		else
			return shared_ptr<XMIRelation>();
	}
	else
		return shared_ptr<XMIRelation>();
}


shared_ptr<XMIRelation> XMIPackage::getRelation(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIASSOCIATION, occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIRelation> xmiRelation ( new XMIRelation(xmlNode));
			return xmiRelation;
		}
		else
			return shared_ptr<XMIRelation>();
	}
	else
		return shared_ptr<XMIRelation>();
}


shared_ptr<XMIObject> XMIPackage::getObject(string objectName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIOBJECT, objectName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIObject> xmiObject ( new XMIObject(xmlNode));
			return xmiObject;
		}
		else
			return shared_ptr<XMIObject>();
	}
	else
		return shared_ptr<XMIObject>();
}


shared_ptr<XMIObject> XMIPackage::getObject(unsigned int occurence)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _content->getChild(XMIOBJECT, occurence);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIObject> xmiObject ( new XMIObject(xmlNode));
			return xmiObject;
		}
		else
			return shared_ptr<XMIObject>();
	}
	else
		return shared_ptr<XMIObject>();
}


shared_ptr<XMIStateMachine> XMIPackage::getStateMachine(string stateMachineName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMISTATEMACHINE, stateMachineName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIStateMachine> xmiStateMachine ( new XMIStateMachine(xmlNode));
			return xmiStateMachine;
		}
		else
			return shared_ptr<XMIStateMachine>();
	}
	else
		return shared_ptr<XMIStateMachine>();
}


shared_ptr<XMIStateMachine> XMIPackage::getStateMachine(unsigned int occurence)
{
	if (_root!=NULL)
	{
		int iElem = 1;
		bool found = true;
		shared_ptr<XMIStateMachine> xmiStateMachine ;
		while (found)
		{
			shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
			if (element !=NULL)
			{
				shared_ptr<XmlNode> xmlNode = element->getChild(XMISTATEMACHINE, occurence);
				if (xmlNode != NULL)
				{
					xmiStateMachine =shared_ptr<XMIStateMachine>( new XMIStateMachine(xmlNode));
				}
			}
			else found = false;
		}
		return xmiStateMachine;
	}
	else
		return shared_ptr<XMIStateMachine>();
}


shared_ptr<XMIActivityGraph> XMIPackage::getActivityGraph(string activityGraphName)
{
	if (_content!=NULL)
	{
		shared_ptr<XmlNode> xmlNode = _getChildByName(XMIACTIVITYGRAPH, activityGraphName);
		if (xmlNode != NULL)
		{
			shared_ptr<XMIActivityGraph> xmiActivityGraph ( new XMIActivityGraph(xmlNode));
			return xmiActivityGraph;
		}
		else
			return shared_ptr<XMIActivityGraph>();
	}
	else
		return shared_ptr<XMIActivityGraph>();
}


shared_ptr<XMIActivityGraph> XMIPackage::getActivityGraph(unsigned int occurence)
{
	if (_root!=NULL)
	{
		int iElem = 1;
		bool found = true;
		shared_ptr<XMIActivityGraph> xmiActivityGraph ;
		while (found)
		{
			shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
			if (element !=NULL)
			{
				shared_ptr<XmlNode> xmlNode = element->getChild(XMIACTIVITYGRAPH, occurence);
				if (xmlNode != NULL)
				{
					xmiActivityGraph =shared_ptr<XMIActivityGraph>( new XMIActivityGraph(xmlNode));
				}
			}
			else found = false;
		}
		return xmiActivityGraph;
	}
	else
		return shared_ptr<XMIActivityGraph>();
}


shared_ptr<XMIStereotype> XMIPackage::getStereotype(string stereotypeId)
{
	int iElem = 1;
	bool found = true;
	while (found)
	{
		shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
		if (element !=NULL)
		{
			string id;
			int occurence=1;
			shared_ptr<XmlNode> xmlNode;
			while((xmlNode= element->getChild(XMIDEPENDENCY, occurence++)))
			{
				xmlNode->getPropertyString(XMIIDREF,id);
				if (id== stereotypeId)
				{
					shared_ptr<XMIStereotype> xmiStereotype ( new XMIStereotype(xmlNode));
					return xmiStereotype;
				}
			}
		}
		else
			found = false;
	}

	return shared_ptr<XMIStereotype>();
}


shared_ptr<XMIStereotype> XMIPackage::getStereotype(unsigned int occurence)
{
	int iElem = 1;
	bool found = true;
	while (found)
	{
		shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
		if (element !=NULL)
		{
			shared_ptr<XmlNode> xmlNode = element->getChild(XMISTEREOTYPE, occurence);
			if (xmlNode != NULL)
			{
				shared_ptr<XMIStereotype> xmiStereotype ( new XMIStereotype(xmlNode));
				return xmiStereotype;
			}
		}
		else
			found = false;
	}

	return shared_ptr<XMIStereotype>();
}


shared_ptr<XMIDependency> XMIPackage::getDependency(string dependencyId)
{
	int iElem = 1;
	bool found = true;
	while (found)
	{
		shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
		if (element !=NULL)
		{
			string id;
			int occurence=1;
			shared_ptr<XmlNode> xmlNode;
			while((xmlNode= element->getChild(XMIDEPENDENCY, occurence++)))
			{
				xmlNode->getPropertyString(XMIIDREF,id);
				if (id== dependencyId)
				{
					shared_ptr<XMIDependency> xmiDependency ( new XMIDependency(xmlNode));
					return xmiDependency;
				}
			}
		}
		else
			found = false;
	}

	return shared_ptr<XMIDependency>();
}


shared_ptr<XMIDependency> XMIPackage::getDependency(unsigned int occurence)
{
	int iElem = 1;
	bool found = true;
	while (found)
	{
		shared_ptr<XmlNode> element = _root->getChild(XMIOWNEDELEMENT,iElem++);
		if (element !=NULL)
		{
			shared_ptr<XmlNode> xmlNode = element->getChild(XMIDEPENDENCY, occurence);
			if (xmlNode != NULL)
			{
				shared_ptr<XMIDependency> xmiDependency ( new XMIDependency(xmlNode));
				return xmiDependency;
			}
		}
		else
			found = false;
	}

	return shared_ptr<XMIDependency>();
}


/* ***************************************************** */

XMISignalEvent::XMISignalEvent(shared_ptr<XmlNode> node) :
XMIElement(node)
{
	_content = _root;
}


//XMISignalEvent::~XMISignalEvent() {}

string XMISignalEvent::getSignalID(void)
{
	return _content->getChild(XMISIGNALEVENTSIG)->getChild(XMISIGNAL)->getProperty(XMIIDREF);
}


/* ***************************************************** */

XMITimeEvent::XMITimeEvent(shared_ptr<XmlNode> node) :
XMIElement(node)
{
	_content = _root;
}


//XMITimeEvent::~XMITimeEvent() {}

string XMITimeEvent::getBody(void)
{
	return _content->getChild(XMITIMEEVENTWHEN)->getChild(XMITIMEEXPRESSION)->getChild(XMIEXPRESSIONBODY)->getChild("text")->getString();
}


/* ***************************************************** */

XMIChangeEvent::XMIChangeEvent(shared_ptr<XmlNode> node) :
XMIElement(node)
{
	_content = _root;
}


//XMIChangeEvent::~XMIChangeEvent() {}

string XMIChangeEvent::getBody(void)
{
	string exp = _content->getChild(XMICHANGEEVENTEXP)->getChild(XMIBOOLEANEXPRESSION)->getChild(XMIEXPRESSIONBODY)->getChild("text")->getString();
	return exp;
}


/* ***************************************************** */

XMICallEvent::XMICallEvent(shared_ptr<XmlNode> node) :
XMIElement(node)
{
	_content = _root;
}


//XMICallEvent::~XMICallEvent() {}

string XMICallEvent::getOperationID(void)
{
	string idOper;
	_content->getChild(XMICALLEVENTOPER)->getChild(XMIOPERATION)->getPropertyString(XMIIDREF, idOper);
	return idOper;
}


/* ***************************************************** */

XMISignal::XMISignal(shared_ptr<XmlNode> node) :
XMIClasse(node)
{
}


//XMISignal::~XMISignal() {}

XMIExtension::XMIExtension(shared_ptr<XmlNode> node)
:_root(node)
{
}


//XMIExtension::~XMIExtension(){}

string XMIExtension::getId()
{
	string id;
	_root->getPropertyString(XMIID,id);
	return id;
}


string XMIExtension::getType()
{
	return _root->getName();
}


XMIExtensions::XMIExtensions(shared_ptr<XmlNode> node)
:_root(node)
{
}


//XMIExtensions::~XMIExtensions(){}

shared_ptr<XMIExtension> XMIExtensions::getExtension(unsigned int occurence)
{
	if (_root!=NULL)
	{
		shared_ptr<XmlNode> child=_root->getFirstChild();

		unsigned int iElem = 1;
		while (child && iElem<occurence)
		{
			child=child->getNext();
			iElem++;
		}
		if (child)
		{
			return shared_ptr<XMIExtension>(new XMIExtension(child));
		}
	}

	return shared_ptr<XMIExtension>();

}
