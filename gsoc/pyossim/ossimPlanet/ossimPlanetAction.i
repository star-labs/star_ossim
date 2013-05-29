/*

*/

%module pyplanet
%{
#include <osg/Referenced>
#include <ossim/base/ossimString.h>
using namespace osg;
%}


%include osg/Referenced
%include "ossimPlanet/ossimPlanetExport.h"
%import "ossimPlanet/ossimPlanetAction.h"

