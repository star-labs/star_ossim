/*

*/
%module pyplanet

%{

#include <osg/MatrixTransform>
#include <osgUtil/CullVisitor>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ClusterCullingCallback>
#include <ossimPlanet/ossimPlanetBoundingBox.h>
#include <ossimPlanet/ossimPlanetReentrantMutex.h>
%}

%{
   class OSSIMPLANET_DLL CullNode : public osg::MatrixTransform
   {
   public:
      CullNode(const ossimPlanetTerrainTileId& tileId):
      osg::MatrixTransform(),
      theTileId(tileId),
      theCulledFlag(false),
      theWithinFrustumFlag(true),
      theEyeDistance(0.0),
      theEyeToVolumeDistance(0.0),
      thePixelSize(0.0)
      {
         setCullingActive(false);
      }
      CullNode(const ossimPlanetTerrainTileId& tileId,
               osg::ref_ptr<ossimPlanetBoundingBox> boundingBox,
               const osg::Vec3d& clusterCullingControlPoint,
               const osg::Vec3d& clusterCullingNormal,
               double clusterCullingDeviation,
               double clusterCullingRadius):
      osg::MatrixTransform(),
      theTileId(tileId),
      theCulledFlag(false),
      theWithinFrustumFlag(true),
      theBoundingBox(boundingBox),
      theClusterCullingControlPoint(clusterCullingControlPoint),
      theClusterCullingNormal(clusterCullingNormal),
      theClusterCullingDeviation(clusterCullingDeviation),
      theClusterCullingRadius(clusterCullingRadius),
      theEyeDistance(0.0),
      theEyeToVolumeDistance(0.0),
      thePixelSize(0.0)
      {
         setCullingActive(false);
      }
      CullNode(const CullNode& src, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY)
      :osg::MatrixTransform(src, copyop),
      theTileId(src.theTileId),
      theCulledFlag(src.theCulledFlag),
      theWithinFrustumFlag(src.theWithinFrustumFlag),
      theBoundingBox(new ossimPlanetBoundingBox(*src.theBoundingBox)),
      theClusterCullingControlPoint(src.theClusterCullingControlPoint),
      theClusterCullingNormal(src.theClusterCullingNormal),
      theClusterCullingDeviation(src.theClusterCullingDeviation),
      theClusterCullingRadius(src.theClusterCullingRadius),
      theEyeDistance(0.0),
      theEyeToVolumeDistance(0.0),
      thePixelSize(0.0)
     {
         setCullingActive(false);
      }
      const ossimPlanetTerrainTileId& tileId()const
      {
         return theTileId;
      }
      void setTileId(const ossimPlanetTerrainTileId& id)
      {
         theTileId = id;
      }
      void setBoundingBox(osg::ref_ptr<ossimPlanetBoundingBox> box)
      {
         theBoundingBox = box.get();
      }
      
      const ossimPlanetBoundingBox* boundingBox()const
      {
         return theBoundingBox.get();
      }
      double eyeDistance()const
      {
         return theEyeDistance;
      }
      double eyeToVolumeDistance()const
      {
         return theEyeToVolumeDistance;
      }
      virtual void traverse(osg::NodeVisitor& nv);
      
      void setCulledFlag(bool flag)
      {
         theCulledFlag = flag;
      }
      bool isCulled()const
      {
         return theCulledFlag;
      }
      virtual osg::BoundingSphere computeBound() const;
      
      double pixelSize()const
      {
         return thePixelSize;
      }
      bool withinFrustumFlag()const
      {
         return theWithinFrustumFlag;
      }
   protected:
      ossimPlanetTerrainTileId theTileId;
      bool theCulledFlag;
      bool theWithinFrustumFlag;
      osg::ref_ptr<ossimPlanetBoundingBox> theBoundingBox;
      osg::Vec3d theClusterCullingControlPoint;
      osg::Vec3d theClusterCullingNormal;
      double theClusterCullingDeviation;
      double theClusterCullingRadius;
      double theEyeDistance;
      double theEyeToVolumeDistance;
      double thePixelSize;
  };
  
  
%}


/*%template(ossimPlanetCallbackIface_ossimPlanetOperationCallback) ossimPlanetCallbackListInterface<ossimPlanetOperationCallback>;*/
/*%template(ossimPlanetCallbackIface_ossimPlanetNodeCallback) ossimPlanetCallbackListInterface<ossimPlanetNodeCallback>;*/
/*%template(ossimPlanetCallbackIface_ossimPlanetTextureLayerCallback) ossimPlanetCallbackListInterface<ossimPlanetTextureLayerCallback>;*/
/*%template(ossimPlanetCallbackIface_ossimPlanetLsrSpaceTransformCallback)ossimPlanetCallbackListInterface< ossimPlanetLsrSpaceTransformCallback >;*/
