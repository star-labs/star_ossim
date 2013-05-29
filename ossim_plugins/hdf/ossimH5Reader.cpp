//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM HDF5 Reader.
//
//----------------------------------------------------------------------------
// $Id: ossimH5Reader.cpp 19878 2011-07-28 16:27:26Z dburken $

#include "ossimH5Reader.h"
#include "ossimH5Util.h"

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimU8ImageData.h>

#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimProjection.h>

//---
// This includes everything!  Note the H5 includes are order dependent; hence,
// the mongo include.
//---
#include <hdf5.h>
#include <cpp/H5Cpp.h>

RTTI_DEF1(ossimH5Reader, "ossimH5Reader", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimH5Reader:degug");

ossimH5Reader::ossimH5Reader()
   :
      ossimImageHandler(),
      m_h5File(0),
      m_entries(),
      m_currentEntry(0),
      m_tile(0),
      m_projection(0),
      m_projectionRect()
{
   m_projectionRect.makeNan();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimH5Reader::ossimH5Reader entered..." << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }
}

ossimH5Reader::~ossimH5Reader()
{
   if (isOpen())
   {
      close();
   }
}

void ossimH5Reader::allocate()
{
   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->initialize();
}

ossimRefPtr<ossimImageData> ossimH5Reader::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   if ( m_tile.valid() == false ) // First time through.
   {
      allocate();
   }
   
   if (m_tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_tile->setImageRectangle(rect);
      
      if ( getTile( m_tile.get(), resLevel ) == false )
      {
         if (m_tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_tile->makeBlank();
         }
      }
   }
   
   return m_tile;
}

bool ossimH5Reader::getTile(ossimImageData* result, ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref(); // Increment ref count.
      
      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);

      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect tile_rect = result->getImageRectangle();

         if ( ! tile_rect.completely_within(getImageRectangle(0)) )
         {
            // We won't fill totally so make blank first.
            result->makeBlank();
         }
         
         if (getImageRectangle(0).intersects(tile_rect))
         {
            // Make a clip rect.
            ossimIrect clipRect = tile_rect.clipToRect(getImageRectangle(0));
            
            if (tile_rect.completely_within( clipRect) == false)
            {
               // Not filling whole tile so blank it out first.
               result->makeBlank();
            }

            // Create buffer to hold the clip rect for a single band.
            ossim_uint32 clipRectSizeInBytes = clipRect.area() *
               ossim::scalarSizeInBytes( m_entries[m_currentEntry].getScalarType() );
            vector<char> dataBuffer(clipRectSizeInBytes);

            // Get the data.
            for (ossim_uint32 band = 0; band < getNumberOfInputBands(); ++band)
            {
               // Hdf5 file to buffer:
               m_entries[m_currentEntry].getTileBuf(&dataBuffer.front(), clipRect, band);

               // Buffer to tile:
               result->loadBand((void*)&dataBuffer.front(), clipRect, band);
            }

            // Validate the tile, i.e. full, partial, empty.
            result->validate();
         }
         else // No intersection...
         {
            result->makeBlank();
         }
      }

      result->unref();  // Decrement ref count.
   }

   return status;
}

ossimIrect
ossimH5Reader::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

bool ossimH5Reader::saveState(ossimKeywordlist& kwl,
                               const char* prefix) const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimH5Reader::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      return open();
   }

   return false;
}

bool ossimH5Reader::open()
{
   static const char MODULE[] = "ossimH5Reader::open";

   // if (traceDebug())
   if ( 1 )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "File:  " << theImageFile.c_str()
         << std::endl;
   }

   bool status = false;
   
   // Start with a clean slate.
   if (isOpen())
   {
      close();
   }

   // Check for empty filename.
   if (theImageFile.size())
   {
      try
      {
         //--
         // Turn off the auto-printing when failure occurs so that we can
         // handle the errors appropriately
         //---
         // H5::Exception::dontPrint();

         if ( H5::H5File::isHdf5( theImageFile.string() ) )
         {
            m_h5File = new H5::H5File();

            H5::FileAccPropList access_plist = H5::FileAccPropList::DEFAULT;
            
            m_h5File->openFile( theImageFile.string(), H5F_ACC_RDONLY, access_plist );

            std::vector<std::string> names;
            ossim_hdf5::getDatasetNames( m_h5File, names );

            if ( names.size() )
            {
               // Add the image dataset entries.
               addImageDatasetEntries( names );

               if ( m_entries.size() )
               {
                  // Set the return status.
                  status = true;
               }
               else
               {
                  m_h5File->close();
                  delete m_h5File;
                  m_h5File = 0;
               }
            }
         }
         
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
      
      if ( status )
      {
         completeOpen();
      }
   }

   return status;
}

void ossimH5Reader::addImageDatasetEntries(const std::vector<std::string>& names)
{
   if ( m_h5File && names.size() )
   {
      std::vector<std::string>::const_iterator i = names.begin();
      while ( i != names.end() )
      {
         if ( ossim_hdf5::isExcludedDataset( *i ) == false )
         {
            H5::DataSet dataset = m_h5File->openDataSet( *i );

            // Get the class of the datatype that is used by the dataset.
            H5T_class_t type_class = dataset.getTypeClass();
            
            if ( ( type_class == H5T_INTEGER ) || ( type_class == H5T_FLOAT ) )
            {
               // Get the extents:
               std::vector<ossim_uint32> extents;
               ossim_hdf5::getExtents( &dataset, extents ); 

               if ( extents.size() >= 2 )
               {
                  if ( ( extents[0] > 1 ) && ( extents[1] > 1 ) )
                  {
                     ossimH5ImageDataset hids;
                     hids.initialize( dataset, *i );
                     m_entries.push_back( hids );
                  }     
               }
            }

            dataset.close();
         }
            
         ++i;
      }
   }
   
#if 0
   std::vector<ossimH5ImageDataset>::const_iterator i = m_entries.begin();
   while ( i != m_entries.end() )
   {
      std::cout << (*i) << endl;
      ++i;
   }
#endif
      
} // End: ossimH5Reader::addImageDatasetEntries

ossim_uint32 ossimH5Reader::getTileWidth() const
{
   return ( m_tile.valid() ? m_tile->getWidth() : 0 );
}

ossim_uint32 ossimH5Reader::getTileHeight() const
{
   return ( m_tile.valid() ? m_tile->getHeight() : 0 );
}

ossim_uint32 ossimH5Reader::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   ossim_uint32 result = 0;
   
   if ( (reduced_res_level == 0) && ( m_currentEntry < m_entries.size() ) )
   {
      result = m_entries[m_currentEntry].getNumberOfLines();
   }
   else if ( theOverview.valid() )
   {
      result = theOverview->getNumberOfLines(reduced_res_level);
   }
   
   return result;
}

ossim_uint32 ossimH5Reader::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   ossim_uint32 result = 0;
   
   if ( (reduced_res_level == 0) && ( m_currentEntry < m_entries.size() ) )
   {
      result = m_entries[m_currentEntry].getNumberOfSamples();
   }
   else if ( theOverview.valid() )
   {
      result = theOverview->getNumberOfSamples(reduced_res_level);
   }
   
   return result; 
}

ossim_uint32 ossimH5Reader::getImageTileWidth() const
{
   return 0; // Not tiled format.
}

ossim_uint32 ossimH5Reader::getImageTileHeight() const
{
   return 0; // Not tiled format.
}

ossimString ossimH5Reader::getShortName()const
{
   return ossimString("ossim_hdf5_reader");
}
   
ossimString ossimH5Reader::getLongName()const
{
   return ossimString("ossim hdf5 reader");
}

ossimString  ossimH5Reader::getClassName()const
{
   return ossimString("ossimH5Reader");
}

ossim_uint32 ossimH5Reader::getNumberOfInputBands() const
{
   ossim_uint32 result = 1;

   if ( m_currentEntry < m_entries.size() )
   {
      result = m_entries[m_currentEntry].getNumberOfBands();
   }

   return result;
}

ossim_uint32 ossimH5Reader::getNumberOfOutputBands()const
{
   // Currently not band selectable.
   return getNumberOfInputBands();
}

ossimScalarType ossimH5Reader::getOutputScalarType() const
{
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;
   
   if ( m_currentEntry < m_entries.size() )
   {
      result = m_entries[m_currentEntry].getScalarType();
   }
   
   return result;
}

bool ossimH5Reader::isOpen()const
{
   return ( m_entries.size() && ( m_currentEntry < m_entries.size() ) ) ? true : false ;
}

void ossimH5Reader::close()
{
   // Close the datasets.
   m_entries.clear();

   // Then the file.
   if ( m_h5File )
   {
      m_h5File->close();
      delete m_h5File;
      m_h5File = 0;
   }

   // ossimRefPtr so assign to 0(unreferencing) will handle memory.
   m_tile = 0;
   m_projection = 0;

   m_projectionRect.makeNan();

   ossimImageHandler::close();
}

ossim_uint32 ossimH5Reader::getNumberOfEntries() const
{
   return (ossim_uint32)m_entries.size();
}

void ossimH5Reader::getEntryName( ossim_uint32 entryIdx, std::string& name ) const
{
   if ( entryIdx < m_entries.size() )
   {
      name = m_entries[ entryIdx ].getName();
   }
   else
   {
      name.clear();
   }
}

void ossimH5Reader::getEntryList(std::vector<ossim_uint32>& entryList) const
{
   const ossim_uint32 SIZE = m_entries.size();
   entryList.resize( SIZE );
   for ( ossim_uint32 i = 0; i < SIZE; ++i )
   {
      entryList[i] = i; 
   }
}

bool ossimH5Reader::setCurrentEntry( ossim_uint32 entryIdx )
{
   bool result = true;
   if (m_currentEntry != entryIdx)
   {
      if ( isOpen() )
      {
         // Entries always start at zero and increment sequentially..
         if ( entryIdx < m_entries.size() )
         {
            // Clear the geometry.
            theGeometry = 0;
            
            // Must clear or openOverview will use last entries.
            theOverviewFile.clear();
            
            m_currentEntry = entryIdx;
            
            //---
            // Since we were previously open and the the entry has changed we
            // need to reinitialize some things. Setting tile to 0 will force an
            // allocate call on first getTile.
            //---
            
            // ossimRefPtr so assign to 0(unreferencing) will handle memory.
            m_tile = 0;
            
            completeOpen();
         }
         else
         {
            result = false; // Entry index out of range.
         }
      }
      else
      {
         //---
         // Not open.
         // Allow this knowing that open will check for out of range.
         //---
         m_currentEntry = entryIdx;
      }
   }

   return result;
}

ossim_uint32 ossimH5Reader::getCurrentEntry() const
{
   return m_currentEntry;
}

ossimRefPtr<ossimImageGeometry> ossimH5Reader::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check for external geom:
      //---
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         //---
         // WARNING:
         // Must create/set the geometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---

         // Check the internal geometry first to avoid a factory call.
         theGeometry = getInternalImageGeometry();

         // At this point it is assured theGeometry is set.

         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}

ossimRefPtr<ossimImageGeometry> ossimH5Reader::getInternalImageGeometry()
{
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();

   if ( m_projection.valid() && ( m_projectionRect == getImageRectangle(0) ) )
   {
      // Set the geometry projection
      geom->setProjection( m_projection.get() );
   }
   else
   {
      ossimRefPtr<ossimProjection> proj = 0;
      ossimIrect rect;
      proj = processBilinearProjection(rect);
      
      if ( proj.valid() )
      {
         if ( rect == getImageRectangle(0) )
         {
            // Store it:
            m_projection = proj;
            m_projectionRect = rect;
            
            // Set the geometry projection
            geom->setProjection( proj.get() );
         }
      }
   }
 
   return geom;
}

ossimRefPtr<ossimProjection> ossimH5Reader::processBilinearProjection( ossimIrect& rect ) const
{
   ossimRefPtr<ossimProjection> proj = 0;

   if ( m_h5File )
   {
      // Find the "Latitude" and "Longitude" datasets if present.
      std::string latName;
      std::string lonName;
      if ( getLatLonDatasetNames( latName, lonName ) )
      {
         H5::DataSet latDataSet = m_h5File->openDataSet( latName );
         H5::DataSet lonDataSet = m_h5File->openDataSet( lonName );
         
         // Get dataspace of the dataset.
         H5::DataSpace latDataSpace = latDataSet.getSpace();
         H5::DataSpace lonDataSpace = lonDataSet.getSpace();
         
         // Number of dimensions of the input dataspace:
         const ossim_int32 DIM_COUNT = latDataSpace.getSimpleExtentNdims();
         
         if ( DIM_COUNT == 2 )
         {
            // Get the extents. Assuming dimensions are same for lat lon dataset. 
            std::vector<hsize_t> dimsOut(DIM_COUNT);
            latDataSpace.getSimpleExtentDims( &dimsOut.front(), 0 );

            if ( dimsOut[0] && dimsOut[1] )
            {
               //---
               // Capture the rectangle:
               // dimsOut[0] is height, dimsOut[1] is width:
               //---
               rect = ossimIrect( 0, 0,
                                  static_cast<ossim_int32>( dimsOut[1]-1 ),
                                  static_cast<ossim_int32>( dimsOut[0]-1 ) );
               
               std::vector<hsize_t> inputCount(DIM_COUNT);
               std::vector<hsize_t> inputOffset(DIM_COUNT);
               
               inputOffset[0] = 0;
               inputOffset[1] = 0;
               
               inputCount[0] = 1;
               inputCount[1] = 1;
               
               // Output dataspace dimensions.
               const ossim_int32 OUT_DIM_COUNT = 3;
               std::vector<hsize_t> outputCount(OUT_DIM_COUNT);
               outputCount[0] = 1; // single band
               outputCount[1] = 1; // single line
               outputCount[2] = 1; // single sample
               
               // Output dataspace offset.
               std::vector<hsize_t> outputOffset(OUT_DIM_COUNT);
               outputOffset[0] = 0;
               outputOffset[1] = 0;
               outputOffset[2] = 0;
               
               ossimScalarType scalar = ossim_hdf5::getScalarType( &latDataSet );
               if ( scalar == OSSIM_FLOAT32 )
               {
                  // See if we need to swap bytes:
                  ossimEndian* endian = 0;
                  if ( ( ossim::byteOrder() != ossim_hdf5::getByteOrder( &latDataSet ) ) )
                  {
                     endian = new ossimEndian();
                  }

                  // Native type:
                  H5::DataType latDataType = latDataSet.getDataType();
                  H5::DataType lonDataType = lonDataSet.getDataType();
                  
                  std::vector<ossimDpt> ipts;
                  std::vector<ossimGpt> gpts;
                  ossimDpt ipt(0.0, 0.0);
                  ossimGpt gpt(0.0, 0.0, 0.0); // Assuming WGS84...
                  ossim_float32 latValue = 0.0;
                  ossim_float32 lonValue = 0.0;
                  
                  // Only grab every 64th value.:
                  const ossim_int32 GRID_SIZE = 64;
                  
                  // Output dataspace always the same one pixel.
                  H5::DataSpace bufferDataSpace( OUT_DIM_COUNT, &outputCount.front());
                  bufferDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &outputCount.front(),
                                                   &outputOffset.front() );

                  //---
                  // Dataset sample has NULL lines at the end so scan for valid rect.
                  //---
                  
                  const ossim_float32 NULL_VALUE = -999.9;

                  // Find the first pixel:
                  ossimIpt firstPix = rect.ul();
                  bool found = false;
                  
                  // Line loop to find first pixel:
                  while ( firstPix.y <= rect.lr().y )
                  {
                     inputOffset[0] = static_cast<hsize_t>(firstPix.y);
                     
                     // Sample loop:
                     firstPix.x = rect.ul().x;
                     while ( firstPix.x <= rect.lr().x )
                     {
                        inputOffset[1] = static_cast<hsize_t>(firstPix.x);
                        
                        latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                      &inputCount.front(),
                                                      &inputOffset.front() );
                        lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                      &inputCount.front(),
                                                      &inputOffset.front() );
                        
                        // Read data from file into the buffer.
                        latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                        lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                        
                        if ( endian )
                        {
                           // If the endian pointer is initialized(not zero) swap the bytes.
                           endian->swap( latValue );
                           endian->swap( lonValue );  
                        }
                        
                        if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                        {
                           found = true;
                           break;
                        }
                        ++firstPix.x;
                     
                     } // End: sample loop
                     
                     if ( found )
                     {
                        break;
                     }

                     ++firstPix.y;
                     
                  } // End line loop to find first pixel:

                  // Find the last pixel:
                  ossimIpt lastPix = rect.lr();
                  found = false;
                  
                  // Line loop to find last pixel:
                  while ( lastPix.y >= rect.ul().y )
                  {
                     inputOffset[0] = static_cast<hsize_t>(lastPix.y);
                     
                     // Sample loop:
                     lastPix.x = rect.lr().x;
                     while ( lastPix.x >= rect.ul().x )
                     {
                        inputOffset[1] = static_cast<hsize_t>(lastPix.x);
                        
                        latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                      &inputCount.front(),
                                                      &inputOffset.front() );
                        lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                      &inputCount.front(),
                                                      &inputOffset.front() );
                        
                        // Read data from file into the buffer.
                        latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                        lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                        
                        if ( endian )
                        {
                           // If the endian pointer is initialized(not zero) swap the bytes.
                           endian->swap( latValue );
                           endian->swap( lonValue );  
                        }
                        
                        if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                        {
                           found = true;
                           break;
                        }
                        --lastPix.x;
                     
                     } // End: sample loop
                     
                     if ( found )
                     {
                        break;
                     }

                     --lastPix.y;
                     
                  } // End line loop to find last pixel.

                  ossimIrect validRect( firstPix, lastPix );

                  //---
                  // Get the tie points within the valid rect:
                  // Do this by walking the perimeter of the valid rect.
                  //---

                  //---
                  // Walk the top line upper left to upper right:
                  //---
                  
                  ossim_int32 line = validRect.ul().y;
                  ipt.y = line;
                  inputOffset[0] = static_cast<hsize_t>(line);
                  
                  // Sample loop:
                  ossim_int32 samp = validRect.ul().x;
                  while ( samp <= validRect.lr().x )
                  {
                     ipt.x = samp;
                     inputOffset[1] = static_cast<hsize_t>(samp);
                     
                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     
                     // Read data from file into the buffer.
                     latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                     lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                     
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( latValue );
                        endian->swap( lonValue );  
                     }
                     
                     if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                     {
                        gpt.lat = latValue;
                        gpt.lon = lonValue;
                        
                        ipts.push_back( ipt );
                        gpts.push_back( gpt );
                     }
                     
                     if ( samp < validRect.lr().x )
                     {
                        samp += GRID_SIZE;
                        if ( samp > validRect.lr().x )
                        {
                           samp = validRect.lr().x; // Clamp to last sample:
                        }
                     }
                     else
                     {  
                        break; // At the end:
                     }
                        
                  } // End: sample loop through top line.

                  //---
                  // Walk the end column upper right to lower right.
                  //---
                  line = validRect.ur().y + GRID_SIZE;
                  samp = validRect.ur().x;
                  ipt.x = samp;
                  inputOffset[1] = static_cast<hsize_t>(samp);
                  
                  while ( line <= validRect.lr().y )
                  {
                     ipt.y = line;
                     inputOffset[0] = static_cast<hsize_t>(line);
                     
                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     
                     // Read data from file into the buffer.
                     latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                     lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                     
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( latValue );
                        endian->swap( lonValue );  
                     }
                     
                     if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                     {
                        gpt.lat = latValue;
                        gpt.lon = lonValue;
                        
                        ipts.push_back( ipt );
                        gpts.push_back( gpt );
                     }
                     
                     if ( line < validRect.lr().y )
                     {
                        line += GRID_SIZE;
                        if ( line > validRect.lr().y )
                        {
                           line = validRect.lr().y; // Clamp to last line:
                        }
                     }
                     else
                     {
                        break; // At the end:
                     }
                     
                  } // End line loop from end column upper right to lower right.

                  //---
                  // Walk the bottom line from lower right to lower left:
                  //---
                  
                  line = validRect.lr().y;
                  ipt.y = line;
                  inputOffset[0] = static_cast<hsize_t>(line);
                  samp = validRect.lr().x - GRID_SIZE;
                     
                  // Sample loop:
                  while ( samp >= validRect.ll().x )
                  {
                     ipt.x = samp;
                     inputOffset[1] = static_cast<hsize_t>(samp);
                     
                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     
                     // Read data from file into the buffer.
                     latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                     lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                     
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( latValue );
                        endian->swap( lonValue );  
                     }
                     
                     if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                     {
                        gpt.lat = latValue;
                        gpt.lon = lonValue;
                        
                        ipts.push_back( ipt );
                        gpts.push_back( gpt );
                     }
                     
                     if ( samp > validRect.ll().x )
                     {
                        samp -= GRID_SIZE;
                        if ( samp < validRect.ll().x )
                        {
                           // Clamp to last sample:
                           samp = validRect.ll().x;
                        }
                     }
                     else
                     {  
                        break; // At the end:
                     }
                     
                  } // End sample loop through bottom line.
                  
                  //---
                  // Walk first column up from lower left to upper left:
                  //---:
                  samp = validRect.ll().x;
                  ipt.x = samp;
                  inputOffset[1] = static_cast<hsize_t>(samp);
                  line = validRect.ll().y - GRID_SIZE;
                  
                  while ( line >=  (validRect.ul().y + GRID_SIZE) )
                  {
                     ipt.y = line;
                     inputOffset[0] = static_cast<hsize_t>(line);                  
                        
                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     
                     // Read data from file into the buffer.
                     latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                     lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                     
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( latValue );
                        endian->swap( lonValue );  
                     }
                     
                     if ( ( latValue != NULL_VALUE ) && ( lonValue != NULL_VALUE ) )
                     {
                        gpt.lat = latValue;
                        gpt.lon = lonValue;
                        
                        ipts.push_back( ipt );
                        gpts.push_back( gpt );
                     }

                     if ( line > (validRect.ul().y + GRID_SIZE ) )
                     {
                        line -= GRID_SIZE;
                        if ( line < ( validRect.ul().y + GRID_SIZE ) )
                        {
                           // Clamp to last line:
                           line = validRect.ul().y + GRID_SIZE;
                        }
                     }
                     else
                     {
                        break; // At the end:
                     }
                     
                  } // End line loop walk from lower left to upper left.
                  
                  if ( endian )
                  {
                     delete endian;
                     endian = 0;
                  }
                  
                  if ( ipts.size() )
                  {
                     // Create the projection:
                     ossimRefPtr<ossimBilinearProjection> bp = new ossimBilinearProjection();
                     
                     // Add the tie points:
                     bp->setTiePoints( ipts, gpts );
                     
                     // Assign to output projection:
                     proj = bp.get();
                  }
               }
               else // Matches: if ( scalar == OSSIM_FLOAT32 ){...}
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "ossimH5Reader::processBilinearProjection WARNING!"
                     << "\nUnhandled scalar type: "
                     << ossimScalarTypeLut::instance()->getEntryString( scalar )
                     << std::endl;
               }
               
            } // Matches: if ( dimsOut...
            
         } // Matches: if ( IN_DIM_COUNT == 2 )
         
         latDataSpace.close();
         lonDataSpace.close();
         latDataSet.close();
         lonDataSet.close();
      }
      
   } // Matches: if ( m_h5File )
   
   return proj;
   
} // End: ossimH5Reader::processBilinearProjection()

ossimRefPtr<ossimProjection> ossimH5Reader::processGridProjection() const
{
   return ossimRefPtr<ossimProjection>(0);
}

bool ossimH5Reader::getLatLonDatasetNames( std::string& latName, std::string& lonName ) const
{
   bool result = false;

   latName.clear();
   lonName.clear();

   // Get the list of datasets.
   std::vector<std::string> datasetNames;
   ossim_hdf5::getDatasetNames( m_h5File, datasetNames );

   if ( datasetNames.size() )
   {
      std::vector<std::string>::const_iterator i = datasetNames.begin();
      while ( i != datasetNames.end() )
      {
         ossimString os = *i;
         if ( os.contains( "Latitude" ) )
         {
            latName = *i;
         }
         else if ( os.contains( "Longitude" ) )
         {
            lonName = *i;
         }
         
         if ( latName.size() && lonName.size() )
         {
            result = true;
            break;
         }
         
         ++i;
      }
   }
   
   return result;
}

