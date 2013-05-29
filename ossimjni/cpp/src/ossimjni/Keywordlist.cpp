//-----------------------------------------------------------------------------
// File:  Keywordlist.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimKeywordlist.
//
//-----------------------------------------------------------------------------
// $Id: Keywordlist.cpp 21394 2012-07-26 20:56:57Z dburken $

#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimKeywordlist.h>

ossimjni::Keywordlist::Keywordlist() :
   m_kwl( new ossimKeywordlist() )
{
}

ossimjni::Keywordlist::~Keywordlist()
{
   if (m_kwl)
   {
      delete m_kwl;
      m_kwl = 0;
   }
}

ossimKeywordlist* ossimjni::Keywordlist::getKeywordlist()
{
   return m_kwl;
}

ossimjni::KeywordlistIterator ossimjni::Keywordlist::getIterator()
{
   return KeywordlistIterator(this);
}

bool ossimjni::Keywordlist::valid() const
{
   return m_kwl ? true : false;
}

int ossimjni::Keywordlist::size() const
{
   int result = 0;
   if ( m_kwl )
   {
      result = static_cast<int>( m_kwl->getSize() );
   }
   return result;
}

std::string ossimjni::Keywordlist::findKey( const std::string& key ) const
{
   std::string result;
   if ( m_kwl )
   {
      result = m_kwl->findKey(key);
   }
   return result;
}

std::string ossimjni::Keywordlist::findKey( const std::string& prefix,
                                            const std::string& key ) const
{
   std::string result;
   {
      result = m_kwl->findKey(prefix, key);
   }
   return result;
}

void ossimjni::Keywordlist::clearMap()
{
   if ( m_kwl )
   {
      m_kwl->clear();
   }
}

