//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: HDF5 Info class.
// 
//----------------------------------------------------------------------------
// $Id$

#include "ossimH5Info.h"
#include "ossimH5Util.h"

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimJ2kCodRecord.h>
#include <ossim/support_data/ossimJ2kSizRecord.h>
#include <ossim/support_data/ossimJ2kSotRecord.h>

//---
// This includes everything!  Note the H5 includes are order dependent; hence,
// the mongo include.
//---
#include <hdf5.h>
#include <cpp/H5Cpp.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

static ossimTrace traceDebug("ossimH5Info:debug");

ossimH5Info::ossimH5Info()
   : ossimInfoBase(),
     m_file()
{
}

ossimH5Info::~ossimH5Info()
{
}

bool ossimH5Info::open(const ossimFilename& file)
{
   bool result = false;

   // Check for empty filename.
   if (file.size())
   {
      try
      {
         //--
         // Turn off the auto-printing when failure occurs so that we can
         // handle the errors appropriately
         //---
         H5::Exception::dontPrint();
         
         if ( H5::H5File::isHdf5( file.string() ) )
         {
            m_file = file;
            result = true;
         }
      }
      catch( const H5::FileIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSet operations
      catch( const H5::DataSetIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataSpaceIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataTypeIException& error )
      {
         error.printError();
      }
      catch( ... )
      {
         
      }
   }

   return result;
}

std::ostream& ossimH5Info::print(std::ostream& out) const
{
   static const char MODULE[] = "ossimH5Info::open";
   
   // if (traceDebug())
   if ( 1 )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "File:  " << m_file.c_str()
         << std::endl;
   }

   // Check for empty filename.
   if (m_file.size())
   {
      try
      {
         //--
         // Turn off the auto-printing when failure occurs so that we can
         // handle the errors appropriately
         //---
         H5::Exception::dontPrint();

         H5::H5File* h5File = new H5::H5File();
         
         H5::FileAccPropList access_plist = H5::FileAccPropList::DEFAULT;
         
         h5File->openFile( m_file.string(), H5F_ACC_RDONLY, access_plist );
         
         // std::vector<std::string> names;
         // ossim_hdf5::getDatasetNames( h5File, names );
         ossim_hdf5::print( h5File, out );

         // cleanup...
         h5File->close();
         delete h5File;
         h5File = 0;
         
      } // End: try block
      
      catch( const H5::FileIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSet operations
      catch( const H5::DataSetIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataSpaceIException& error )
      {
         error.printError();
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataTypeIException& error )
      {
         error.printError();
      }
      catch( ... )
      {
         
      }
   }

   return out;
}
