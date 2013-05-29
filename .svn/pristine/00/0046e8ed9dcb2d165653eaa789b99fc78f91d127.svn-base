/*

*/

%module pyplanet



%include "globals.i"
%{

#include <osg/Referenced>
#include <osg/MatrixTransform>

#include <ossimPlanet/ossimPlanetConstants.h>
#include <ossimPlanet/ossimPlanetApi.h>


#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <ossimPlanet/ossimPlanetGeoRefModel.h>
#include <ossimPlanet/ossimPlanetGrid.h>
#include <ossimPlanet/ossimPlanetTexture2D.h>
#include <osgUtil/GLObjectsVisitor>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ClusterCullingCallback>
#include <ossimPlanet/ossimPlanetBoundingBox.h>
#include <ossimPlanet/ossimPlanetReentrantMutex.h>

#include <ossimPlanet/ossimPlanetTerrainGeometryTechnique.h>
%}
%include "osg/Group"
%include "osg/Node"
%include "osg/Transform"
%include "osg/MatrixTransform"
%include "ossimPlanetCallback.i"
%include "ossimPlanet/ossimPlanetTerrainTechnique.h"
%include "ossimPlanet/ossimPlanetTerrainGeometryTechnique.h"
%include "ossimPlanet/ossimPlanetConstants.h"


/*
%rename(ossimPlanetAction_print) print;
%rename(__lshift__) operator<<;
%rename(__rshift__) operator>>;



%ignore ossimPlanetOperation::operator <;
%ignore ossimPlanetOperation::operator >;
%ignore ossimPlanetTextureLayer;

%ignore ossimPlanetIdolLayer::addChild;
%ignore ossimPlanetAnimationPath::setGeospatialPath;
%ignore ossimPlanetAnimationPath::geospatialPath;
*/

