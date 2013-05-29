//-----------------------------------------------------------------------------
// File:  Info.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for Info.
//
//-----------------------------------------------------------------------------
// $Id: Info.cpp 21395 2012-07-26 21:47:49Z dburken $

#include <ossimjni/Info.h>
#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/util/ossimInfo.h>

ossimjni::Info::Info() :
   m_info(new ossimInfo)
{
}
      
ossimjni::Info::~Info()
{
   if (m_info)
   {
      delete m_info;
      m_info = 0;
   }
}

bool ossimjni::Info::initialize(int argc, char* argv[])
{
   ossimArgumentParser ap(&argc, argv);
   return m_info->initialize(ap);
}

void ossimjni::Info::execute()
{
   m_info->execute();
}

void ossimjni::Info::getImageInfo( const std::string& file,
                                   bool dumpFlag,
                                   bool dnoFlag,
                                   bool imageGeomFlag,
                                   bool imageInfoFlag,
                                   bool metaDataFlag,
                                   bool paletteFlag,
                                   ossimjni::Keywordlist* kwl) const
{
   if ( kwl )
   {
      m_info->getImageInfo(ossimFilename(file),
                           dumpFlag,
                           dnoFlag,
                           imageGeomFlag,
                           imageInfoFlag,
                           metaDataFlag,
                           paletteFlag,
                           *(kwl->getKeywordlist()) );
   }
}
void ossimjni::Info::closeImage()
{
   if ( m_info )
   {
      m_info->closeImage();
   }
}

