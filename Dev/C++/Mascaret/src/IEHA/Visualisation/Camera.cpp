/*
 * Scene.cpp
 *
 *  Created on: 27 févr. 2011
 *      Author: ping
 */

#include "IEHA/Visualisation/Camera.h"
#include "IEHA/Visualisation/Scene.h"
#include "VEHA/Entity/Zone.h"

using namespace VEHA;

namespace IEHA
{
   void Camera::gotoPreviousViewPoint()
   {
        shared_ptr<Zone> zone=accessCurrentScene()->getZone();
        if(zone)
        {       
                map<string,shared_ptr<PointSpecification> > vps = zone->getViewPoints(); 
                map<string,shared_ptr<PointSpecification> >::iterator it = vps.find(_currentViewPoint); 
                shared_ptr<PointSpecification> viewPoint;

                if (it == vps.end())
                {       
                   if (vps.size() != 0)   
                   {       
                      viewPoint = vps.begin()->second;
                      gotoViewPoint(vps.begin()->first);
                   }       
                }       
                else    
                {       
                   if (it == vps.begin()) it = vps.end();
                   it--;   
                   viewPoint = it->second;
                   gotoViewPoint(it->first);
                }       
         }  
   }

   void Camera::gotoNextViewPoint()
   {
        cerr << "Camera::gotoNextViewPoint()"<<endl;
        shared_ptr<Zone> zone=accessCurrentScene()->getZone();
        if(zone)
        {       
                map<string,shared_ptr<PointSpecification> > vps = zone->getViewPoints();
                map<string,shared_ptr<PointSpecification> >::iterator it = vps.find(_currentViewPoint);
                shared_ptr<PointSpecification> viewPoint;

                if (it == vps.end())
                {
                   if (vps.size() != 0)
                   {
                      viewPoint = vps.begin()->second;
                      gotoViewPoint(vps.begin()->first);
                   }
                }
                else
                {
                   it++;
                   if (it == vps.end()) it = vps.begin();
                   viewPoint = it->second;
                   gotoViewPoint(it->first);
                }
         }
   }

   string Camera::getCurrentViewpoint()
   {
       // TODO: What to do if we move ? Stay on the same viewPoint or remove it ? (In case of head tracking ignore user move in the pod)
       return _currentViewPoint;
   }
}
