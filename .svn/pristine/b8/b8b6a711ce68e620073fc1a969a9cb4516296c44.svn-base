/*

*/

%module pyplanet
%define OSG_EXPORT
%enddef

%{
#include <osg/ref_ptr>
#include <OpenThreads/Version>
#include <OpenThreads/Exports>
#include <OpenThreads/Barrier>
#include <OpenThreads/Block>
#include <OpenThreads/Condition>

#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/ReadWriteMutex>
#include <OpenThreads/ReentrantMutex>
#include <OpenThreads/ScopedLock>
#include <osg/AnimationPath>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Transform>
#include <osg/Image>
#include <osg/CoordinateSystemNode>
using namespace osg;
%}

%ignore osg::Referenced::operator=;

%ignore osg::Node::ComputeBoundingSphereCallback;
%ignore osg::Node::setComputeBoundingSphereCallback;
%ignore osg::Node::getComputeBoundingSphereCallback;
%ignore osg::Node::getParents();

%ignore osg::Referenced::setThreadSafeRefUnref;

%include OpenThreads.i

%include osg/ref_ptr
#%ignore osg::ref_ptr::operator=;
#%ignore osg::ref_ptr::operator!;

%ignore osg::Referenced::operator=;
%ignore osg::ref_ptr::operator<;
%ignore osg::ref_ptr::operator==;
%ignore osg::ref_ptr::operator!=;
%ignore osg::ref_ptr::operator.;

%feature("ref")   osg::Referenced "$this->ref();"
%feature("unref") osg::Referenced  "$this->unref();"

%include "osg/Referenced"

%include "ossimPlanet/ossimPlanetExport.h"
%include "osg/Object"
%include "osg/Node"

%ignore ::operator == ;
