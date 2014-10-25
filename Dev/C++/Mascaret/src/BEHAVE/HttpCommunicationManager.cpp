/*
 * HttpCommunicationManager.cpp
 *
 *  Created on: 27 août 2010
 *      Author: soler
 */
#include <boost/asio.hpp>
#include "BEHAVE/HttpCommunicationManager.h"
#include "httpServer/httpServer.h"
#include "Tools/Network/Servlets/ManageACLMessageServlet.h"
#include "Tools/Network/Servlets/ManageActionServlet.h"
#include "Tools/Network/Servlets/ManageAgentServlet.h"
#include "Tools/Network/Servlets/ManageAgentsServlet.h"
#include "Tools/Network/Servlets/ManageApplicationServlet.h"
#include "Tools/Network/Servlets/ManageCenterViewServlet.h"
#include "Tools/Network/Servlets/ManageChangeGeometryServlet.h"
#include "Tools/Network/Servlets/ManageClassServlet.h"
#include "Tools/Network/Servlets/ManageReloadClassServlet.h"
#include "Tools/Network/Servlets/ManageConnectServlet.h"
#include "Tools/Network/Servlets/ManageCreateEntityServlet.h"
#include "Tools/Network/Servlets/ManageCreateMessageServlet.h"
#include "Tools/Network/Servlets/ManageCreateUserMessageServlet.h"
#include "Tools/Network/Servlets/ManageEntityServlet.h"
#include "Tools/Network/Servlets/ManageEntityJSONServlet.h"
#include "Tools/Network/Servlets/ManageExecuteQueryServlet.h"
#include "Tools/Network/Servlets/ManageFIPASLServlet.h"
#include "Tools/Network/Servlets/ManageFIPASLTestServlet.h"
#include "Tools/Network/Servlets/ManageHeadServlet.h"
#include "Tools/Network/Servlets/ManageIndexServlet.h"
#include "Tools/Network/Servlets/ManageInstancesServlet.h"
#include "Tools/Network/Servlets/ManageLaunchOperationServlet.h"
#include "Tools/Network/Servlets/ManageMessagesEnvoyesServlet.h"
#include "Tools/Network/Servlets/ManageMessagesRecusServlet.h"
#include "Tools/Network/Servlets/ManageMessagesServlet.h"
#include "Tools/Network/Servlets/ManageModelServlet.h"
#include "Tools/Network/Servlets/ManageOperationServlet.h"
#include "Tools/Network/Servlets/ManageOrganisationalEntityServlet.h"
#include "Tools/Network/Servlets/ManageOrganisationalStructureServlet.h"
#include "Tools/Network/Servlets/ManageOrganisationsServlet.h"
#include "Tools/Network/Servlets/ManageSpeedServlet.h"
#include "Tools/Network/Servlets/ManageViewpointServlet.h"
#include "Tools/Network/Servlets/ManageProcedureServlet.h"
#include "Tools/Network/Servlets/ManageLaunchProcedureServlet.h"
#include "Tools/Network/Servlets/ManageQueryServlet.h"
#include "Tools/Network/Servlets/ManageRoleClassServlet.h"
#include "Tools/Network/Servlets/ManageSendMessageServlet.h"
#include "Tools/Network/Servlets/ManageSendUserMessageServlet.h"
#include "Tools/Network/Servlets/ManageSetRedServlet.h"
#include "Tools/Network/Servlets/ManageSignalServlet.h"
#include "Tools/Network/Servlets/ManageXOCLServlet.h"
#include "Tools/Network/Servlets/ManageXOCLHelperServlet.h"
#include "Tools/Network/Servlets/ManageXOCLHelperRemoveServlet.h"
#include "Tools/Network/Servlets/ManageScenarioServlet.h"
#include "Tools/Network/Servlets/ManageChangeAttributeServlet.h"
#include "Tools/Network/Servlets/ManageSetAttributeValueServlet.h"
#include "Tools/Network/Servlets/ManageAttributeServlet.h"
#include "Tools/Network/Servlets/ManageAgentKnowledgeBaseServlet.h"
#include "Tools/Network/Servlets/ManageAgentKBHeadServlet.h"

namespace BEHAVE
{
HttpCommunicationManager::HttpCommunicationManager(int port,const std::string& ressourcesDir, bool noServer)
: httpServer(new HttpServer(port,ressourcesDir))
{
	initTypeMime();
	initServlets();
	if(!noServer) httpServer->start();
	else cerr << " HTTP Server not started !" << endl;
}
HttpCommunicationManager::~HttpCommunicationManager()
{
	delete httpServer;
}
bool HttpCommunicationManager::sendDistantMessage(shared_ptr<AID> aid,shared_ptr<ACLMessage> message)
{
	using boost::asio::ip::tcp;

	boost::asio::io_service io_service;

	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(tcp::v4(),aid->getPlateformName(), boost::lexical_cast<std::string>(aid->getPlateformPort()));
	boost::system::error_code error;
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query,error);
	if(error)
	{
		cerr << "Impossible to resolve end_point of "<< aid->getPlateformName() << ":" << aid->getPlateformPort() << " --> "<< error.message() << endl;
		return false;
	}
	tcp::resolver::iterator end;

	// Try each endpoint until we successfully establish a connection.
	tcp::socket socket(io_service);
	error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
		//tcp::endpoint ep=*endpoint_iterator;
		//cerr << ep.address().to_string() << endl;
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
	{
		std::cerr << "Impossible to connect to host:" << aid->getPlateformName() << ":" << aid->getPlateformPort() << " --> ";
		std::cerr << error << endl;
		return false;
	}

	// Form the request. We specify the "Connection: close" header so that the
	// server will close the socket after transmitting the response. This will
	// allow us to treat all data up until the EOF as the content.
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "POST /ACLMessage HTTP/1.0\r\n";
	request_stream << "Host: " << aid->getPlateformName() << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n";
	string xmlMsg = message->toXML();
	int messageSize=xmlMsg.length();
	request_stream << "Content-Length: " << messageSize << "\r\n\r\n";
	request_stream << xmlMsg;

	// Send the request.
	boost::asio::write(socket, request);
	return true;
}

void HttpCommunicationManager::initTypeMime()
{
	httpServer->setExtensionTypeMime(".css"  ,"text/css");
	httpServer->setExtensionTypeMime(".gif"  ,"image/gif");
	httpServer->setExtensionTypeMime(".html" ,"text/html");
	httpServer->setExtensionTypeMime(".jpg"  ,"image/jpg");
	httpServer->setExtensionTypeMime(".js"   ,"text/javascript");
	httpServer->setExtensionTypeMime(".png"  ,"image/png");
	httpServer->setExtensionTypeMime(".svg"  ,"image/svg+xml");
	httpServer->setExtensionTypeMime(".txt"  ,"text/plain");
	httpServer->setExtensionTypeMime(".xhtml","application/xhtml+xml");
	httpServer->setExtensionTypeMime(".xml"  ,"text/xml");
}
void HttpCommunicationManager::initServlets()
{
	httpServer->addServlet("/",new ManageIndexServlet());
	httpServer->addServlet("/Head",new ManageHeadServlet());
	httpServer->addServlet("/Model",new ManageModelServlet());
	httpServer->addServlet("/Class",new ManageClassServlet());
	httpServer->addServlet("/ReloadClass",new ManageReloadClassServlet());
	httpServer->addServlet("/Instances",new ManageInstancesServlet());
	httpServer->addServlet("/Agents",new ManageAgentsServlet());
	httpServer->addServlet("/Organisations",new ManageOrganisationsServlet());
	httpServer->addServlet("/OrgStruct",new ManageOrganisationalStructureServlet());
	httpServer->addServlet("/OrgEntity",new ManageOrganisationalEntityServlet());
	httpServer->addServlet("/RoleClass",new ManageRoleClassServlet());
	httpServer->addServlet("/Speed", new ManageSpeedServlet());
	httpServer->addServlet("/Procedure",new ManageProcedureServlet());
	httpServer->addServlet("/launchProcedure",new ManageLaunchProcedureServlet());
	httpServer->addServlet("/Actions",new ManageActionServlet());
	httpServer->addServlet("/Application",new ManageApplicationServlet());
	httpServer->addServlet("/Entity",new ManageEntityServlet());
 	httpServer->addServlet("/EntityJSON",new ManageEntityJSONServlet());
	httpServer->addServlet("/Agent",new ManageAgentServlet());
	httpServer->addServlet("/Operation",new ManageOperationServlet());
	httpServer->addServlet("/launchOperation",new ManageLaunchOperationServlet());
	httpServer->addServlet("/Attribute",new ManageAttributeServlet());
	httpServer->addServlet("/CreateEntity",new ManageCreateEntityServlet());
	httpServer->addServlet("/changeGeometry",new ManageChangeGeometryServlet());
	httpServer->addServlet("/createMessage",new ManageCreateMessageServlet());
	httpServer->addServlet("/createUserMessage",new ManageCreateUserMessageServlet());
	httpServer->addServlet("/sendMessage",new ManageSendMessageServlet());
	httpServer->addServlet("/sendUserMessage",new ManageSendUserMessageServlet());

//RQ    sigServlet = new ManageSignalServlet(); 
//RQ	httpServer->addServlet("/Signal",sigServlet);
	httpServer->addServlet("/Signal",new ManageSignalServlet());
	httpServer->addServlet("/messages",new ManageMessagesServlet());
	httpServer->addServlet("/messagesRecus",new ManageMessagesRecusServlet());
	httpServer->addServlet("/messagesEnvoyes",new ManageMessagesEnvoyesServlet());
	httpServer->addServlet("/XOCL",new ManageXOCLServlet());
	//httpServer->addServlet("/XOCLHelper",new ManageXOCLHelperServlet());
	//httpServer->addServlet("/XOCLHelperRemove",new ManageXOCLHelperRemoveServlet());
	httpServer->addServlet("/Query",new ManageQueryServlet());
	httpServer->addServlet("/ExecuteQuery",new ManageExecuteQueryServlet());
	httpServer->addServlet("/connect", createManageConnectServlet(httpServer->getPort()));
	httpServer->addServlet("/ACLMessage",new ManageACLMessageServlet());
	httpServer->addServlet("/Scenario",new ManageScenarioServlet());
	httpServer->addServlet("/changeAttributes",new ManageChangeAttributeServlet());
	httpServer->addServlet("/CenterView",new ManageCenterViewServlet());
	httpServer->addServlet("/SetRed",new ManageSetRedServlet());
	httpServer->addServlet("/FIPASL",new ManageFIPASLServlet());
	httpServer->addServlet("/fipaSLTest",new ManageFIPASLTestServlet());
	httpServer->addServlet("/setAttributeValue",new ManageSetAttributeValueServlet());
	httpServer->addServlet("/KnowledgeBase",new ManageAgentKnowledgeBaseServlet());
	httpServer->addServlet("/HeadAgentKB",new ManageAgentKBHeadServlet());
	httpServer->addServlet("/Viewpoint",new ManageViewpointServlet());

}
void HttpCommunicationManager::addServlet(string url,HttpServlet* servlet)
{
	httpServer->addServlet(url,servlet);
}
}
