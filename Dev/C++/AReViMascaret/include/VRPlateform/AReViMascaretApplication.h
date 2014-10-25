/*
 * AReViMascaretApplication.h
 *
 *  Created on: 27 févr. 2011
 *      Author: ping
 */

#ifndef AREVIMASCARETAPPLICATION_H_
#define AREVIMASCARETAPPLICATION_H_
#include "areviMascaret_plateform.h"
#include "IEHA/VRApplication.h"
#include "Interaction/AReViKeyboard.h"
#include "Interaction/AReViCursor.h"

using namespace AReVi;

namespace AReViMascaret
{
using IEHA::VRApplication;
class AREVI_MASCARET_API AReViMascaretApplication : public VRApplication
{
public:
	AReViMascaretApplication();
	virtual ~AReViMascaretApplication();

	virtual bool step();
	virtual void createDistributionServer(const string& sessionName, int port);
	virtual void createClient(const string& hostname, int port);
	virtual void parse(const string& name);
	static inline AReViMascaretApplication* getInstance();
private:
	static AReViMascaretApplication* _instance;
};
AReViMascaretApplication* AReViMascaretApplication::getInstance()
{
	return _instance;
}
}
#endif /* AREVIMASCARETAPPLICATION_H_ */
