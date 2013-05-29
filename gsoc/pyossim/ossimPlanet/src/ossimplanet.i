//#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
%{

#include <osgDB/DatabasePager>
#include <ossimPlanet/ossimPlanetExport.h>
#include <ossimPlanet/ossimPlanetActionReceiver.h>
#include <ossimPlanet/ossimPlanetElevationDatabase.h>
#include <ossimPlanet/ossimPlanetCallback.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanet/ossimPlanetNode.h>
#include <osgViewer/Viewer>
//#include <osgViewer/Renderer>
#include <ossimPlanet/ossimPlanet.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <ossimPlanet/ossimPlanetNode.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <osgUtil/LineSegmentIntersector>
#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanet/ossimPlanetTerrain.h>
#include <ossimPlanet/ossimPlanetAnnotationLayer.h>
#include <ossimPlanet/ossimPlanetKmlLayer.h>
#include <ossimPlanet/ossimPlanetVisitors.h>
#include <ossimPlanet/ossimPlanetEphemeris.h>
#include <OpenThreads/Mutex>

%}
%include osg/Referenced

%include "ossimPlanet/ossimPlanetExport.h"
%include <ossimPlanet/ossimPlanetCallback.h>
%include <ossimPlanet/ossimPlanetViewer.h>
/*
%include <ossimPlanet/ossimPlanetManipulator.h>
%include <ossimPlanet/ossimPlanetLatLonHud.h>
%include <ossimPlanet/ossimPlanetTerrain.h>
%include <ossimPlanet/ossimPlanetTerrainGeometryTechnique.h>
%include <ossimPlanet/ossimPlanetAnimatedPointModel.h>
%include <ossimPlanet/ossimPlanetViewMatrixBuilder.h>
%include <ossimPlanet/ossimPlanetCloudLayer.h>
%include <ossimPlanet/ossimPlanetEphemeris.h>

/*
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/init/ossimInit.h>
#include <ossim/elevation/ossimElevManager.h>
#include <osg/Texture>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osg/Drawable>
#include <osgViewer/ViewerEventHandlers>




/*
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <ossimPlanet/ossimPlanetAnimationPath.h>
#include <ossimPlanet/ossimPlanetKmlLayer.h>
#include <ossimPlanet/ossimPlanetAnimatedPointModel.h>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osgUtil/CullVisitor>
#include <osg/ClipPlane>

#include <osg/Program>
#include <osg/Uniform>

#include <osg/Geode>
#include <osg/Geometry>
*/
