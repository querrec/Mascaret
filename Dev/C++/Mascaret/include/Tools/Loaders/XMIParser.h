#ifndef XMIPARSER_H
#define XMIPARSER_H

#include "Tools/veha_plateform.h"
#include "Tools/Loaders/XmlParser.h"

using namespace VEHA;

/* Constantes de noms des balises XMI */
#define XMICONTENT              "XMI.content"
#define XMIMODELELEMENTNAME     "ModelElement.name"
#define XMIOWNEDELEMENT         "Namespace.ownedElement"
#define XMIID                   "xmi.id"
#define XMIIDREF                "xmi.idref"
#define XMIVALUE                "xmi.value"
#define XMIFEATURE              "Classifier.feature"
#define XMISTRUCTURALTYPE       "StructuralFeature.type"

#define XMIINITIALVALUE         "Attribute.initialValue"
#define XMIATTREXPRESSION       "Expression"
#define XMIEXPRESSIONBODY       "Expression.body"
#define XMIGUARDEXPRESSION      "Guard.expression"
#define XMIBOOLEANEXPRESSION    "BooleanExpression"

#define XMICLASSIFIER           "Classifier"
#define XMIGENERALIZATION       "Generalization"
#define XMIGENERALIZATIONPARENT "Generalization.parent"
#define XMIGENERALIZATIONCHILD  "Generalization.child"
#define XMICONNECTION           "Association.connection"
#define XMIEND                  "AssociationEnd"
#define XMIENDPARTICPANT        "AssociationEnd.participant"
#define XMIINSTANCE             "Instance"
#define XMIINSTANCESLOT         "Instance.slot"
#define XMIINSTANCECLASS        "Instance.classifier"
#define XMIATTRIBUTELINK        "AttributeLink"
#define XMIATTRIBUTELINKVAL     "AttributeLink.value"
#define XMITRANSITIONS          "StateMachine.transitions"
#define XMITRANSITIONSOURCE     "Transition.source"
#define XMITRANSITIONTARGET     "Transition.target"
#define XMITRANSITIONTRIGGER    "Transition.trigger"
#define XMITRANSITIONGUARD      "Transition.guard"
#define XMITRANSITIONEFFECT     "Transition.effect"
#define XMISTATEVERTEX          "StateVertex"
#define XMISTATEMACHINETOP      "StateMachine.top"
#define XMISTATEMACHINECS       "CompositeState"
#define XMISTATEMACHINECONTEXT  "StateMachine.context"
#define XMISTATESUBVERTEX   "CompositeState.subvertex"
#define XMIMODELELEMENT     "ModelElement"
#define XMIPARTITIONS       "ActivityGraph.partition"
#define XMIPARTITION        "Partition"
#define XMIPARTITIONCONTENT     "Partition.contents"
#define XMISTATEMACHINETOP  "StateMachine.top"
#define XMISUBACTIVITYSTATE     "SubactivityState"
#define XMICOMPOSSTATEVERTEX    "CompositeState.subvertex"
#define XMIPSEUDOSTATE      "Pseudostate"
#define XMIFINALSTATE       "FinalState"
#define XMIACTIONSTATE      "ActionState"
#define XMIOBJECTFLOWSTATE  "ObjectFlowState"
#define XMIOBJECTFLOWSTATETYPE  "ObjectFlowState.type"
#define XMIDOACTIVITY       "State.doActivity"
#define XMIENTRYACTIVITY        "State.entry"
#define XMIEXITACTIVITY         "State.exit"
#define XMICALLACTION       "CallAction"
#define XMICALLACTIONOP         "CallAction.operation"
#define XMISENDACTI0N       "SendAction"
#define XMISENDACTI0NSIG        "SendAction.signal"
#define XMIELTTAGGEDVALUE   "ModelElement.taggedValue"
#define XMITAGGEDVALUE      "TaggedValue"
#define XMITAGGEDVALUETYPE  "TaggedValue.type"
#define XMITAGGEDVALUEDEF   "TagDefinition"
#define XMITAGGEDVALUETTYPE "TagDefinition.tagType"
#define XMITAGGEDVALUEVALUE "TaggedValue.dataValue"
#define XMISIGNALEVENTSIG       "SignalEvent.signal"
#define XMITIMEEVENTWHEN        "TimeEvent.when"
#define XMICHANGEEVENTEXP       "ChangeEvent.changeExpression"
#define XMICALLEVENTOPER        "CallEvent.operation"
#define XMITIMEEXPRESSION       "TimeExpression"

#define XMIMODEL                "Model"
#define XMIPACKAGE              "Package"
#define XMIDATATYPE             "DataType"
#define XMICLASS                "Class"
#define XMIATTRIBUTE            "Attribute"
#define XMIOPERATION            "Operation"
#define XMIASSOCIATION          "Association"
#define XMIOBJECT               "Object"
#define XMISTATEMACHINE         "StateMachine"
#define XMISIMPLESTATE          "SimpleState"
#define XMITRANSITION           "Transition"
#define XMIACTIVITYGRAPH        "ActivityGraph"
#define XMISIGNAL               "Signal"
#define XMIEVENT                "Event"
#define XMISIGNALEVENT          "SignalEvent"
#define XMITIMEEVENT            "TimeEvent"
#define XMICHANGEEVENT          "ChangeEvent"
#define XMICALLEVENT            "CallEvent"
#define XMIGUARD                "Guard"

#define XMIATTMULTIPLICITY  "StructuralFeature.multiplicity"
#define XMIENDMULTIPLICITY  "AssociationEnd.multiplicity"
#define XMIENDNAVIGABLE     "AssociationEnd.isNavigable"
#define XMIENDAGGREGATIONTYPE   "AssociationEnd.aggregation"
#define XMIMULTIPLICITY     "Multiplicity"
#define XMIMULTIPLICITY_RANGE   "Multiplicity.range"
#define XMIMULTIPLICITYRANGE    "MultiplicityRange"
#define XMIMULTIPLICITYLOWER    "MultiplicityRange.lower"
#define XMIMULTIPLICITYUPPER    "MultiplicityRange.upper"

#define XMIOPERATIONPARAMETER   "BehavioralFeature.parameter"
#define XMIPARAMETER        "Parameter"
#define XMIPARAMETERKIND    "Parameter.kind"
#define XMIPARAMETERTYPE    "Parameter.type"

#define XMIMODELELEMENTVISIBILITY "ModelElement.visibility"
#define XMIMODELELEMENTSTEREOTYPE "ModelElement.stereotype"
#define XMISTEREOTYPE       "Stereotype"

#define XMIMODELELTCLIENTDEPENDENCY "ModelElement.clientDependency"
#define XMIDEPENDENCY       "Dependency"
#define XMIDEPENDENCYCLIENT         "Dependency.client"
#define XMIDEPENDENCYSUPPLIER       "Dependency.supplier"

#define XMIEXTENSIONS       "XMI.extensions"
#define XMIEXTENSION        "XMI.extension"
#define XMIREFERENCE        "XMI.reference"

#define XMIMODELELEMENTCOMMENT          "ModelElement.comment"
#define XMICOMMENT                      "Comment"

/*
class XMI_WIN32_API XMITaggedValue
{
XMITaggedValue (CkXml * node);
StlString getType();
StlString getData();
protected:
CkXml * _root;
};
*/

class VEHA_API XMIElement
{
    public :
        XMIElement();

        XMIElement(shared_ptr<XmlNode> node);
        XMIElement(string nodeName, string name, string ID);

        virtual ~XMIElement(){}

        virtual string getName();
        virtual string getId();

        virtual string getSummary();
        virtual string getDescription();
        map<string,string> getTags(void);

        string getVisibility(void);
        string getStereotypeRef(void);
        string getExtensionRef(void);
        string getDependencyRef(unsigned int occurence = 1);
        bool getTaggedValue(const string type, string & data);
    
        shared_ptr<XmlNode> getRoot(void) {return _root;}

    protected :
        shared_ptr<XmlNode> _getChildByName(string XMIElementType, string XMIElementName);
        inline void _setContent(shared_ptr<XmlNode> content) {_content = content;}
        inline shared_ptr<XmlNode> _getContent(void) {return _content;}

        shared_ptr<XmlNode> _root;
        shared_ptr<XmlNode> _content;
};

class VEHA_API XMIStereotype : public XMIElement
{
    public :

        XMIStereotype(shared_ptr<XmlNode> node);

};

class VEHA_API XMIDependency : public XMIElement
{
    public :
        XMIDependency(shared_ptr<XmlNode> node);
        string getSupplierRef();
        string getClientRef();

};

class VEHA_API XMIAttributeLink : public XMIElement
{
    public :
        XMIAttributeLink(shared_ptr<XmlNode> node);

        string getValue(void) ;
};

class VEHA_API XMIObject : public XMIElement
{
    public :
        XMIObject(shared_ptr<XmlNode> node);

        string getClassifierID(void);
        shared_ptr<XMIAttributeLink> getAttributeLink(string attributeName);
        shared_ptr<XMIAttributeLink> getAttributeLink(unsigned int occurence);
};

class VEHA_API XMILinkEnd : public XMIElement
{
    public :
        XMILinkEnd(shared_ptr<XmlNode> node);

        string getParticipantId(void);
        string getMultiplicity(void);
        string getMultiplicityLower(void);
        bool isNavigable(void);
        bool isComposite(void);
};

class VEHA_API XMIRelation : public XMIElement
{
    public :
        XMIRelation (shared_ptr<XmlNode> node);

        shared_ptr<XMILinkEnd> getLinkEnd(string linkEndName);
        shared_ptr<XMILinkEnd> getLinkEnd(unsigned int occurence);
};

class VEHA_API XMIAttribute : public XMIElement
{
    public :
        XMIAttribute(shared_ptr<XmlNode> node);

        string getTypeRef(void) ;
        bool hasInitialValue(void) ;
        string getInitialValue(void) ;
        string getMultiplicity(void) ;
        string getMultiplicityLower(void) ;
};

class VEHA_API XMIParameter : public XMIElement
{
    public :
        XMIParameter(shared_ptr<XmlNode> node);
        string getKind(void) ;
        string getTypeRef(void) ;
};

class VEHA_API XMIOperation : public XMIElement
{
    public :
        XMIOperation(shared_ptr<XmlNode> node);
        shared_ptr<XMIParameter> getParameter(unsigned int occurence);
};

class VEHA_API XMISignalEvent : public XMIElement
{
    public :
        XMISignalEvent(shared_ptr<XmlNode> node);

        string getSignalID(void);
};

class VEHA_API XMITimeEvent : public XMIElement
{
    public :
        XMITimeEvent (shared_ptr<XmlNode> node);

        string getBody(void);
};

class VEHA_API XMIChangeEvent : public XMIElement
{
    public :
        XMIChangeEvent(shared_ptr<XmlNode> node);

        string getBody(void);
};

class VEHA_API XMIDataType : public XMIElement
{
    public :
        XMIDataType(shared_ptr<XmlNode> node);
};

class VEHA_API XMICallEvent : public XMIElement
{
    public :
        XMICallEvent(shared_ptr<XmlNode> node);

        string getOperationID(void);
};

class VEHA_API XMIClasse : public XMIElement
{
    public :
        XMIClasse(shared_ptr<XmlNode> node);
        XMIClasse(string name, string ID);

        vector<string> getParentsID(void);
        vector<string> getChildsID(void);
        void addGeneralisation(string genId, string parentID, string myID);

        void addComment(string comment, string commentID);

        shared_ptr<XMIAttribute> getAttribute(string attributeName);
        shared_ptr<XMIAttribute> getAttribute(unsigned int occurence);

        shared_ptr<XMIOperation> getOperation(string operationName);
        shared_ptr<XMIOperation> getOperation(unsigned int occurence);

        shared_ptr<XMIClasse> getClasse(string classeName);
        shared_ptr<XMIClasse> getClasse(unsigned int occurence);

        shared_ptr<XMIDataType> getDataType(string dataTypeName);
        shared_ptr<XMIDataType> getDataType(unsigned int occurence);

        //ArRef<XMISignalEvent> getSignalEvent(StlString signalName);
        shared_ptr<XMISignalEvent> getSignalEvent(unsigned int occurence);
        shared_ptr<XMITimeEvent> getTimeEvent(unsigned int occurence);
        shared_ptr<XMIChangeEvent> getChangeEvent(unsigned int occurence);
        shared_ptr<XMICallEvent> getCallEvent(unsigned int occurence);
};

class VEHA_API XMITransition : public XMIElement
{
    public :
        XMITransition(shared_ptr<XmlNode> node);

        string getSourceID(void);
        string getTargetID(void);
        string getTriggerID(void);
        string getGuardExpression(void);
        bool hasEffect(void);
        string getCallOperationRef(void);
        string getSendSignalRef(void);
};

class VEHA_API XMISimpleState : public XMIElement
{
    public :
        XMISimpleState(shared_ptr<XmlNode> node);

        bool hasDoActivity(void);
        bool hasEntryActivity(void);
        bool hasExitActivity(void);

        string getDoActivityOperationRef(void);
        string getEntryActivityOperationRef(void);
        string getExitActivityOperationRef(void);
};

class VEHA_API XMIPseudoState : public XMIElement
{
    public :
        XMIPseudoState(shared_ptr<XmlNode> node);

        string getKind(shared_ptr<XmlParser> loader);
};

class VEHA_API XMIActionState : public XMIElement
{
    public :
        XMIActionState(shared_ptr<XmlNode> node);

        string getArguments(void);
        string getSendSignalRef(void);
        string getCallOperationRef(void);
};

class VEHA_API XMIObjectFlowState : public XMIElement
{
    public :
        XMIObjectFlowState(shared_ptr<XmlNode> node);

        string getType(void);
        string getState(void);
        bool inParticularState;
};

class VEHA_API XMIPartition : public XMIElement
{
    public :
        XMIPartition(shared_ptr<XmlNode> node);

        string getContent(unsigned int iContent);
};

class VEHA_API XMIActivityGraph : public XMIElement
{
    public :
        XMIActivityGraph(shared_ptr<XmlNode> node);

        string getContextId(void);

        shared_ptr<XMIPartition> getPartition(string partitionName);
        shared_ptr<XMIPartition> getPartition(unsigned int occurence);

        shared_ptr<XMIActionState> getActionState(string actionStateName);
        shared_ptr<XMIActionState> getActionState(unsigned int occurence);

        shared_ptr<XMIObjectFlowState> getObjectFlowState(string objectFlowStateName);
        shared_ptr<XMIObjectFlowState> getObjectFlowState(unsigned int occurence);

        shared_ptr<XMIPseudoState> getPseudoState(string pseudoStateName);
        shared_ptr<XMIPseudoState> getPseudoState(unsigned int occurence);
        shared_ptr<XMIPseudoState> getFinalState(unsigned int occurence);

        shared_ptr<XMITransition> getTransition(string transitionName);
        shared_ptr<XMITransition> getTransition(unsigned int occurence);
};

class VEHA_API XMIStateMachine : public XMIElement
{
    public :
        XMIStateMachine(shared_ptr<XmlNode> node);

        string getContextClassifierID(void);

        shared_ptr<XMISimpleState> getSimpleState(string stateName);
        shared_ptr<XMISimpleState> getSimpleState(unsigned int occurence);

        shared_ptr<XMITransition> getTransition(string transitionName);
        shared_ptr<XMITransition> getTransition(unsigned int occurence);

        shared_ptr<XMIPseudoState> getPseudoState(unsigned int occurence);
};

class VEHA_API XMISignal : public XMIClasse
{
    public :
        XMISignal(shared_ptr<XmlNode> node);
};

class VEHA_API XMIPackage : public XMIElement
{
    public :
        XMIPackage(shared_ptr<XmlNode> node);
        XMIPackage(string nodeName, string packageName, string ID);

        shared_ptr<XMIPackage> getPackage(string packageName);
        shared_ptr<XMIPackage> getPackage(unsigned int occurence);

        shared_ptr<XMIDataType> getDataType(string dataTypeName);
        shared_ptr<XMIDataType> getDataType(unsigned int occurence);

        shared_ptr<XMIClasse> getClasse(string classeName, bool recursive = false);
        shared_ptr<XMIClasse> getClasse(unsigned int occurence);
        void addClasse(shared_ptr<XMIClasse> classe);

        shared_ptr<XMISignal> getSignal(string signalName);
        shared_ptr<XMISignal> getSignal(unsigned int occurence);

        shared_ptr<XMIRelation> getRelation(string relationName);
        shared_ptr<XMIRelation> getRelation(unsigned int occurence);

        shared_ptr<XMIObject> getObject(string objectName);
        shared_ptr<XMIObject> getObject(unsigned int occurence);

        shared_ptr<XMIStateMachine> getStateMachine(string stateMachineName);
        shared_ptr<XMIStateMachine> getStateMachine(unsigned int occurence);

        shared_ptr<XMIActivityGraph> getActivityGraph(string activityGraphName);
        shared_ptr<XMIActivityGraph> getActivityGraph(unsigned int occurence);

        shared_ptr<XMIStereotype> getStereotype(string stereotypeId);
        shared_ptr<XMIStereotype> getStereotype(unsigned int occurence);

        shared_ptr<XMIDependency> getDependency(string dependencyId);
        shared_ptr<XMIDependency> getDependency(unsigned int occurence);
};

class VEHA_API XMIExtension
{
    public :
        XMIExtension(shared_ptr<XmlNode> node);

        string getId();
        string getType();
        shared_ptr<XmlNode> getNode(){return _root;}
    protected:
        shared_ptr<XmlNode> _root;

};
class VEHA_API XMIExtensions
{
    public :
        XMIExtensions(shared_ptr<XmlNode> node);

        shared_ptr<XMIExtension> getExtension(unsigned int occurence);
    protected:
        shared_ptr<XmlNode> _root;
};

class VEHA_API XMIParser : public XMIElement
{
    public :
        XMIParser();

        void setPrefix(string prefix);
        bool parseFile(string filename);
        bool parseNode(shared_ptr<XmlNode> rootNode);
        void createFile(string modelName);
        void writeFile(string fileName);
        bool parseString(const string& str);
        shared_ptr<XMIPackage> getPackage(string packageName);
        shared_ptr<XMIPackage> getPackage();
        shared_ptr<XMIExtensions> getExtensions();

        void addPackage(shared_ptr<XMIPackage> package);

        inline string getName(void) {return _name;}
        inline string getId(void) {return "";}

        inline shared_ptr<XmlParser> getParser(void) {return _loader;}
        void setParser(shared_ptr<XmlParser> parser);

    private :
        shared_ptr<XmlParser> _loader;
        string _name;
        string _prefix;
};
#endif                           // Attribute
