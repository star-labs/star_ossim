//-----------------------------------------------------------------------------
// File:  ElevMgr.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class declaration for ElevMgr. Wrapper for ossimElevManager
// class.
//
//-----------------------------------------------------------------------------
// $Id: ElevMgr.h 20182 2011-10-20 13:11:24Z dburken $
#ifndef ossimjniElevMgr_HEADER
#define ossimjniElevMgr_HEADER 1


#include <ossimjni/Constants.h>

namespace ossimjni
{
   class OSSIMJNIDLL ElevMgr
   {
   public:

      /** @brief destructor */
      ~ElevMgr();

      /**
       * @brief Instance method.
       *
       * @return The instance of this class.
       */
      static ossimjni::ElevMgr* instance();

      /**
       * @brief Get height above ellipsoid.
       *
       * This is geoid offset plus height above MSL.
       * 
       * @param latitude
       * @param longitude
       * @return Height above ellipsoid.
       */
      double getHeightAboveEllipsoid(double latitude, double longitude);

      /**
       * @brief Get height above mean sea level(MSL).
       * @param latitude
       * @param longitude
       * @return Height above MSL..
       */
      double getHeightAboveMSL(double latitude, double longitude);
      
   private:
      
      /** @brief default constructor - hidden from use */
      ElevMgr();

      /** @brief copy constructor - hidden from use */
      ElevMgr(const ossimjni::ElevMgr& obj);

      /** @brief assignment operator - hidden from use */ 
      const ossimjni::ElevMgr& operator=(const ossimjni::ElevMgr& obj);

      /** @brief The single static instance of this class. */
      static ossimjni::ElevMgr* m_instance;
   };

} // End of namespace ossimjni.

#endif /* #ifndef ossimjniElevMgr_HEADER */
