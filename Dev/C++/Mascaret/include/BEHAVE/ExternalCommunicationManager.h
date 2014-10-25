/*
 * ExternalCommunicationManager.h
 *
 *  Created on: 27 août 2010
 *      Author: soler
 */

#ifndef EXTERNALCOMMUNICATIONMANAGER_H_
#define EXTERNALCOMMUNICATIONMANAGER_H_

#include "Tools/veha_plateform.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/AID.h"
namespace BEHAVE
{
class VEHA_API ExternalCommunicationManager
{
public:
	virtual bool sendDistantMessage(shared_ptr<AID> aid, shared_ptr<ACLMessage> msg)=0;
};
}

#endif /* EXTERNALCOMMUNICATIONMANAGER_H_ */
