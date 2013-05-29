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
// $Id

#ifndef ossimH5Reader_HEADER
#define ossimH5Reader_HEADER 1

#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>
#include "ossimH5ImageDataset.h"
#include <vector>

// Forward class declarations:
namespace H5
{
   class H5File;
}

class ossimH5Reader : public ossimImageHandler
{
public:

   /** default constructor */
   ossimH5Reader();

   /** virtual destructor */
   virtual ~ossimH5Reader();

   /** @return "hdf5" */
   virtual ossimString getShortName() const;

   /** @return "ossim hdf5" */
   virtual ossimString getLongName()  const;

   /** @return "ossimH5Reader" */
   virtual ossimString getClassName()    const;

   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   /**
    * Method to get a tile.   
    *
    * @param result The tile to stuff.  Note The requested rectangle in full
    * image space and bands should be set in the result tile prior to
    * passing.  It will be an error if:
    * result.getNumberOfBands() != this->getNumberOfOutputBands()
    *
    * @return true on success false on error.  If return is false, result
    *  is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    */
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);

    /**
     *  Returns the number of bands in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const;

   /**
    * Returns the number of bands in a tile returned from this TileSource.
    * Note: we are supporting sources that can have multiple data objects.
    * If you want to know the scalar type of an object you can pass in the
    */
   virtual ossim_uint32 getNumberOfOutputBands()const;

   /**
     *  Returns the number of lines in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;

   /**
    *  Returns the number of samples in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   /**
    * Returns the zero based image rectangle for the reduced resolution data
    * set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the width of the output tile.
    */
   virtual ossim_uint32    getTileWidth() const;

   /**
    * Returns the height of the output tile.
    */
   virtual ossim_uint32    getTileHeight() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   bool isOpen()const;

   /** Close method. */
   virtual void close();

   /**
    * @return The number of entries (images) in the image file.
    */
   virtual ossim_uint32 getNumberOfEntries()const;

   /**
    * @brief Get the name of entry as a string.
    *
    * Example given from HDF5 file:
    * 
    * entry_name: /All_Data/VIIRS-IMG-GTM-EDR-GEO_All/QF1_VIIRSGTMGEO
    *
    * @param entryIdx Zero based entry index. If out of range name will
    * be cleared.
    * 
    * @param name Initialized by this.
    */
   virtual void getEntryName( ossim_uint32 entryIdx,
                              std::string& name ) const;

   /**
    * @param entryList This is the list to initialize with entry indexes.
    *
    * @note This implementation returns puts one entry "0" in the list.
    */
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;

   virtual bool setCurrentEntry(ossim_uint32 entryIdx);

   /**
    * @return The current entry number.
    */
   virtual ossim_uint32 getCurrentEntry() const;

   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if non defined.  The geometry contains full-to-local image
    * transform as well as projection (image-to-world).
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();


protected:

   /**
    * @brief Method to get geometry from hdf file.
    *
    * Method is not const because it set m_projection and m_projectionRect
    * if it matches our rectangle.
    */
   virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry();

   /**
    * @brief Gets projection from Latitude, Longitude datasets if
    * present and creates an ossimBilinearProjection from them. This walks the
    * perimeter of the image and sets a tie currently every 64 pixels.
    * @param rect Initialized by this to the extents of the Lat/Lon datasets.
    * @return Projection if found, null ref pointer if not.
    */
   ossimRefPtr<ossimProjection> processBilinearProjection(
      ossimIrect& rect ) const;
   
   /**
    * @brief Gets projection from Latitude, Longitude, Height datasets if
    * present.
    */
   ossimRefPtr<ossimProjection> processGridProjection() const;

   /**
    * @brief Get dataset names for Latiitude and Longitude datasets.
    * @param latName Initializes by this.
    * @param lonName Initializes by this.
    * @return true on sucess, false on error.
    */
   bool getLatLonDatasetNames( std::string& latName, std::string& lonName ) const;
   
   bool getDataSetRect( std::string& name, std::string& lonName ) const;
   
  /**
    *  @brief open method.
    *  @return true on success, false on error.
    */
   virtual bool open();

   /** @brief Allocates the tile. */ 
   void allocate();

   /**
    * @brief Adds image datasets from list of names.
    * @param names List of dataset paths from hdf5 file.
    */
   void addImageDatasetEntries(const std::vector<std::string>& names);
   
   H5::H5File*                      m_h5File;
   std::vector<ossimH5ImageDataset> m_entries;
   ossim_uint32                     m_currentEntry;
   ossimRefPtr<ossimImageData>      m_tile;
   ossimRefPtr<ossimProjection>     m_projection;
   ossimIrect                       m_projectionRect;
   
TYPE_DATA
};

#endif /* #ifndef ossimH5Reader_HEADER */
