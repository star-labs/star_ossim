/*

*/

%module pyplanet



%include "globals.i"
%{

#include <osg/Referenced>
#include <ossim/base/ossimString.h>
#include <ossimPlanet/ossimPlanetAction.h>
#include <ossimPlanet/ossimPlanetActionReceiver.h>
#include <ossimPlanet/ossimPlanetActionRouter.h>
#include "ossimPlanet/ossimPlanetArchive.h"
#include <ossimPlanet/ossimPlanetArchiveMapping.h>
#include <ossimPlanet/ossimPlanetBillboardIcon.h>
#include <ossimPlanet/ossimPlanetBoundingBox.h>
#include <ossimPlanet/ossimPlanetBillboardIcon.h>
#include <ossimPlanet/ossimPlanetCache.h>
#include <ossimPlanet/ossimPlanetOperation.h>

#include <ossimPlanet/ossimPlanetConstants.h>
#include <ossimPlanet/ossimPlanetCacheTextureLayer.h>

#include <ossimPlanet/ossimPlanetApi.h>

#include <ossimPlanet/ossimPlanetTextureLayer.h>
#include <ossimPlanet/ossimPlanetTextureLayerFactory.h>
#include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
#include <ossimPlanet/ossimPlanetClientThread.h>
#include <ossimPlanet/ossimPlanetNode.h>
#include <ossimPlanet/ossimPlanetNodeFactory.h>
#include <ossimPlanet/ossimPlanetNodeFactoryBase.h>
#include <ossimPlanet/ossimPlanetNodeRegistry.h>
#include <ossimPlanet/ossimPlanetCloudLayer.h>
#include <ossimPlanet/ossimPlanetLayer.h>
#include <ossimPlanet/ossimPlanetCompass.h>
#include <ossimPlanet/ossimPlanetGrid.h>
#include <ossimPlanet/ossimPlanetGridUtility.h>
#include <ossimPlanet/ossimPlanetCubeGrid.h>
#include <ossimPlanet/ossimPlanetDatabasePager.h>
#include <ossimPlanet/ossimPlanetDepthPartitionNode.h>
#include <ossimPlanet/ossimPlanetDestinationCommandAction.h>
#include <ossimPlanet/ossimPlanetDistanceAccumulator.h>
#include <ossimPlanet/ossimPlanetDtedElevationDatabase.h>
#include <ossimPlanet/ossimPlanetElevationRegistry.h>
#include <ossimPlanet/ossimPlanetElevationFactory.h>
#include <ossimPlanet/ossimPlanetElevationDatabase.h>
#include <ossimPlanet/ossimPlanetElevationDatabaseGroup.h>
#include <ossimPlanet/ossimPlanetElevationGrid.h>
/*#include <ossimPlanet/ossimPlanetFadeText.h>*/
/*#include <ossimPlanet/ossimPlanetGeneralRasterElevationDatabase.h>*/
/*#include <ossimPlanet/ossimPlanetGeocoder.h>*/
/*#include <ossimPlanet/ossimPlanetGeoRefModel.h>*/
/*#include <ossimPlanet/ossimPlanetExtents.h>*/



#include <ossimPlanet/ossimPlanetIconGeom.h>
#include <ossimPlanet/ossimPlanetId.h>
#include <ossimPlanet/ossimPlanetIdManager.h>



/*#include <ossimPlanet/ossimPlanetIdolBridge.h>*/
/*#include <ossimPlanet/ossimPlanetIdolLayer.h>*/




#include <ossimPlanet/ossimPlanetImage.h>
#include <ossimPlanet/ossimPlanetInputDevice.h>
#include <ossimPlanet/ossimPlanetInteractionController.h>
#include <ossimPlanet/ossimPlanetIntersectUserData.h>
#include <ossimPlanet/ossimPlanetIo.h>
#include <ossimPlanet/ossimPlanetIoMessageHandler.h>
#include <ossimPlanet/ossimPlanetIoRoutableMessageHandler.h>
#include <ossimPlanet/ossimPlanetIoSocket.h>
#include <ossimPlanet/ossimPlanetIoSocketServerChannel.h>
#include <ossimPlanet/ossimPlanetIoThread.h>
#include <ossimPlanet/ossimPlanetJpegImage.h>
#include <ossimPlanet/ossimPlanetKml.h>
#include <ossimPlanet/netSocket.h>
#include <ossimPlanet/netChannel.h>


#include <ossimPlanet/ossimPlanetKmlLayer.h>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <ossimPlanet/ossimPlanetKmlNetworkLinkNode.h>
#include <ossimPlanet/ossimPlanetKmlPlacemarkNode.h>
/*#include <ossimPlanet/ossimPlanetKmlReaderWriter.h>*/




#include <ossimPlanet/ossimPlanetKmlScreenOverlayNode.h>


#include <ossimPlanet/ossimPlanetLabelGeom.h>
#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanet/ossimPlanetLandCache.h>
#include <ossimPlanet/ossimPlanetLandCullCallback.h>

#include <ossimPlanet/ossimPlanetLandNormalType.h>
#include <ossimPlanet/ossimPlanetLandReaderWriter.h>
#include <ossimPlanet/ossimPlanetLandTextureRequest.h>
#include <ossimPlanet/ossimPlanetLandTreeNode.h>
#include <ossimPlanet/ossimPlanetLatLonHud.h>
#include "ossimPlanet/ossimPlanetIo.h"
#include "ossimPlanet/ossimPlanetIoMessageHandler.h"
#include "ossimPlanet/ossimPlanetIoRoutableMessageHandler.h"
#include "ossimPlanet/ossimPlanetIoSocket.h"
#include "ossimPlanet/ossimPlanetIoSocketServerChannel.h"
#include "ossimPlanet/ossimPlanetIoThread.h"
#include "ossimPlanet/ossimPlanetLayer.h"
#include "ossimPlanet/ossimPlanetLayerFactory.h"
#include "ossimPlanet/ossimPlanetLayerFactoryBase.h"
#include "ossimPlanet/ossimPlanetLayerRegistry.h"
#include "ossimPlanet/ossimPlanetLookAt.h"
#include "ossimPlanet/ossimPlanetLsrSpaceTransform.h"
#include "ossimPlanet/ossimPlanetManipulator.h"
#include <ossimPlanet/ossimPlanetMessage.h>
#include <ossimPlanet/ossimPlanetNavigator.h>
#include <ossimPlanet/ossimPlanetNetworkConnection.h>
/*#include <ossimPlanet/ossimPlanetActionRouter.h>*/



#include <ossimPlanet/ossimPlanetAnimationPath.h>
#include <ossimPlanet/ossimPlanetAnnotationLayerNode.h>
#include <ossimPlanet/ossimPlanetAnimatedPointModel.h>

#include <ossimPlanet/ossimPlanetAnnotationLayer.h>

using namespace osg;


%}

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

%include "ossimPlanet/ossimPlanetConstants.h"
//%include "ossimPlanet/ossimPlanetAction.h"
//%include "ossimPlanet/ossimPlanetActionReceiver.h"
/*%include "ossimPlanet/ossimPlanetActionRouter.h"*/


%include "ossimPlanet/ossimPlanetArchiveMapping.h"
%include "ossimPlanet/ossimPlanetArchive.h"
%include "ossimPlanetCallback.i"
%include <ossimPlanet/ossimPlanetOperation.h>
%include <ossimPlanet/ossimPlanetCache.h>
%include "ossimPlanet/ossimPlanetBoundingBox.h"
%include "ossimPlanet/ossimPlanetBillboardIcon.h"
%include <ossimPlanet/ossimPlanetTextureLayer.h>
%include <ossimPlanet/ossimPlanetTextureLayerFactory.h>
%include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
%include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
%include <ossimPlanet/ossimPlanetCacheTextureLayer.h>
%include <ossimPlanet/ossimPlanetClientThread.h>
%include <ossimPlanet/ossimPlanetNodeFactoryBase.h>
%include <ossimPlanet/ossimPlanetNodeFactory.h>
%include <ossimPlanet/ossimPlanetNodeRegistry.h>
%include <ossimPlanet/ossimPlanetNode.h>
%include <ossimPlanet/ossimPlanetCacheTextureLayer.h>
%include <ossimPlanet/ossimPlanetLayer.h>
%include <ossimPlanet/ossimPlanetCloudLayer.h>
%include <ossimPlanet/ossimPlanetCompass.h>
%include <ossimPlanet/ossimPlanetGrid.h>
%include <ossimPlanet/ossimPlanetGridUtility.h>
%include <ossimPlanet/ossimPlanetCubeGrid.h>
%include <ossimPlanet/ossimPlanetDatabasePager.h>
%include <ossimPlanet/ossimPlanetDepthPartitionNode.h>
/*%include <ossimPlanet/ossimPlanetDestinationCommandAction.h>*/


%include <ossimPlanet/ossimPlanetDistanceAccumulator.h>
%include <ossimPlanet/ossimPlanetElevationRegistry.h>
%include <ossimPlanet/ossimPlanetElevationFactory.h>
%include <ossimPlanet/ossimPlanetElevationGrid.h>
%include <ossimPlanet/ossimPlanetElevationDatabase.h>
%include <ossimPlanet/ossimPlanetElevationDatabaseGroup.h>
%include <ossimPlanet/ossimPlanetDtedElevationDatabase.h>
%include <ossimPlanet/ossimPlanetExtents.h>
/*%include <ossimPlanet/ossimPlanetFadeText.h>*/
/*%include <ossimPlanet/ossimPlanetGeneralRasterElevationDatabase.h>*/
/*%include <ossimPlanet/ossimPlanetGeocoder.h>*/
/*%include <ossimPlanet/ossimPlanetGeoRefModel.h>*/


%include <ossimPlanet/ossimPlanetIconGeom.h>
%include <ossimPlanet/ossimPlanetId.h>
%include <ossimPlanet/ossimPlanetIdManager.h>
/*%include <ossimPlanet/ossimPlanetIdolBridge.h>*/
/*%include <ossimPlanet/ossimPlanetIdolLayer.h>*/



%include <ossimPlanet/ossimPlanetImage.h>
%include <ossimPlanet/ossimPlanetInputDevice.h>
%include <ossimPlanet/ossimPlanetInteractionController.h>
%include <ossimPlanet/ossimPlanetIntersectUserData.h>
%include <ossimPlanet/ossimPlanetIo.h>
%include <ossimPlanet/ossimPlanetIoMessageHandler.h>
%include <ossimPlanet/ossimPlanetIoRoutableMessageHandler.h>
%include <ossimPlanet/ossimPlanetIoSocket.h>
%include <ossimPlanet/ossimPlanetIoSocketServerChannel.h>
%include <ossimPlanet/ossimPlanetIoThread.h>
%include <ossimPlanet/ossimPlanetJpegImage.h>
%include <ossimPlanet/ossimPlanetKml.h>
%include <ossimPlanet/ossimPlanetKmlLayer.h>
%include <ossimPlanet/ossimPlanetKmlLayerNode.h>
%include <ossimPlanet/ossimPlanetKmlNetworkLinkNode.h>
%include <ossimPlanet/ossimPlanetKmlPlacemarkNode.h>
/*%include <ossimPlanet/ossimPlanetKmlReaderWriter.h>*/


%include <ossimPlanet/ossimPlanetKmlScreenOverlayNode.h>
%include <ossimPlanet/ossimPlanetLabelGeom.h>
%include <ossimPlanet/ossimPlanetLand.h>
%include <ossimPlanet/ossimPlanetLandCache.h>
%include <ossimPlanet/ossimPlanetLandCullCallback.h>
%include <ossimPlanet/ossimPlanetLandNormalType.h>
%include <ossimPlanet/ossimPlanetLandReaderWriter.h>
%include <ossimPlanet/ossimPlanetLandTextureRequest.h>
%include <ossimPlanet/ossimPlanetLandTreeNode.h>
%include <ossimPlanet/ossimPlanetLatLonHud.h>
%include "ossimPlanet/ossimPlanetIo.h"
%include "ossimPlanet/ossimPlanetIoMessageHandler.h"
%include "ossimPlanet/ossimPlanetIoRoutableMessageHandler.h"
%include "ossimPlanet/ossimPlanetIoSocket.h"
%include "ossimPlanet/ossimPlanetIoSocketServerChannel.h"
%include "ossimPlanet/ossimPlanetIoThread.h"
%include "ossimPlanet/ossimPlanetLayer.h"
%include "ossimPlanet/ossimPlanetLayerFactory.h"
%include "ossimPlanet/ossimPlanetLayerFactoryBase.h"
%include "ossimPlanet/ossimPlanetLayerRegistry.h"
%include "ossimPlanet/ossimPlanetLookAt.h"
%include "ossimPlanet/ossimPlanetLsrSpaceTransform.h"
%include "ossimPlanet/ossimPlanetManipulator.h"
%include <ossimPlanet/ossimPlanetMessage.h>
%include <ossimPlanet/ossimPlanetNavigator.h>
%include <ossimPlanet/ossimPlanetNetworkConnection.h>
/*%include <ossimPlanet/ossimPlanetActionRouter.h>*/



%include <ossimPlanet/ossimPlanetAnimationPath.h>
%include <ossimPlanet/ossimPlanetAnnotationLayerNode.h>
%include <ossimPlanet/ossimPlanetAnimatedPointModel.h>

%include <ossimPlanet/ossimPlanetAnnotationLayer.h>
%ignore GeospatialPath;

