//-----------------------------------------------------------------------------
// File:  KeywordlistIterator.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimKeywordlistIterator.
//
//-----------------------------------------------------------------------------
// $Id: KeywordlistIterator.cpp 22121 2013-01-26 16:34:32Z dburken $

#include <ossimjni/KeywordlistIterator.h>
#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimKeywordlist.h>

ossimjni::KeywordlistIterator::KeywordlistIterator()
   :
   m_kwl(0),
   m_currentPair(),
   m_iter()
{
}

ossimjni::KeywordlistIterator::KeywordlistIterator(const ossimjni::KeywordlistIterator& iter)
   :
   m_kwl(iter.m_kwl),
   m_currentPair(iter.m_currentPair),
   m_iter(iter.m_iter)
{
}

ossimjni::KeywordlistIterator::KeywordlistIterator(ossimjni::Keywordlist* kwl)
   :
   m_kwl(kwl),
   m_currentPair(),
   m_iter()
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
}

ossimjni::KeywordlistIterator::~KeywordlistIterator()
{
}


const ossimjni::KeywordlistIterator& ossimjni::KeywordlistIterator::operator=(
   const ossimjni::KeywordlistIterator& iter )
{
   if ( this != &iter )
   {
      m_kwl         = iter.m_kwl;
      m_currentPair = iter.m_currentPair;
      m_iter        = iter.m_iter;
   }
   return *this;
}

void ossimjni::KeywordlistIterator::initialize(ossimjni::Keywordlist* kwl)
{
   m_kwl = kwl;
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
   m_currentPair.clear();
}

void ossimjni::KeywordlistIterator::reset()
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
}

bool ossimjni::KeywordlistIterator::hasNext() const
{
   bool result = false;
   if ( m_kwl  && m_kwl->valid() )
   {
      result = (m_iter != m_kwl->getKeywordlist()->getMap().end());
   }
   return result;
}

void ossimjni::KeywordlistIterator::next()
{
   if ( m_kwl  && m_kwl->valid() )
   {
      // Set current:
      m_currentPair.setKey( (*m_iter).first );
      m_currentPair.setValue( (*m_iter).second );

      // Increment:
      ++m_iter;
   }
   else
   {
      m_currentPair.clear();
   }
}

std::string ossimjni::KeywordlistIterator::getKey() const
{
   return m_currentPair.getKey();
}

std::string ossimjni::KeywordlistIterator::getValue() const
{
   return m_currentPair.getValue();
}

void ossimjni::KeywordlistIterator::getKeyValue( ossimjni::StringPair* pair ) const
{
   if ( pair )
   {
      *pair = m_currentPair;
   }
}
