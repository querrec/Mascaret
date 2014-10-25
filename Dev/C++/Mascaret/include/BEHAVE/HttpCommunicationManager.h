/*
 * HttpCommunicationManager.h
 *
 *  Created on: 27 août 2010
 *      Author: soler
 */

#ifndef HTTPCOMMUNICATIONMANAGER_H_
#define HTTPCOMMUNICATIONMANAGER_H_


//#include "Tools/Network/Servlets/ManageSignalServlet.h"
#include "Tools/veha_plateform.h"
#include "BEHAVE/ExternalCommunicationManager.h"
class HttpServer;
class HttpServlet;
namespace BEHAVE
{
class VEHA_API HttpCommunicationManager : public ExternalCommunicationManager
{
public:
	HttpCommunicationManager(int port, const string& ressourcesDir, bool noServer=false);
	virtual ~HttpCommunicationManager();
	virtual bool sendDistantMessage(shared_ptr<AID> aid,shared_ptr<ACLMessage> msg);
	void addServlet(string url,HttpServlet* servlet);

        // RQ...... PAS BO
       // ManageSignalServlet * sigServlet;

private:
	void initTypeMime();
	void initServlets();
	HttpServer* httpServer;
};
}


#endif /* HTTPCOMMUNICATIONMANAGER_H_ */
