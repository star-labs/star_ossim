//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  STAR-LABS developers - Espen Nilsen - Jørgen Markussen
//
//*******************************************************************
//  $Id:10T $

#include <iostream>
#include <vector>
using namespace std;

#include <ossim/ossimConfig.h>

#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>

#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageSourceFactory.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>

#include <ossim/imaging/ossimNitfWriter.h>

#include <ossim/init/ossimInit.h>

#include <ossim/plugin/ossimSharedPluginRegistry.h>

#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/support_data/ossimInfoFactoryRegistry.h>



void usage(string reason)
{
	cout << reason << endl;
	cout << "star-tag bilde-fil ut-fil" << endl;

}


//**************************************************************************
// Main Application
//**************************************************************************
int main(int argc, char *argv[])
{

	if(argc < 3)
	{
		usage("Mangler parametere");
		exit(EXIT_FAILURE);
	}

	ossimImageHandler* in1 = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[1]));

	ossimNitfWriter* writer = new ossimNitfWriter(ossimFilename(argv[2]));
	//ossimNitfBlockaTag ba_tag = new ossimNitfBlockaTag();

	writer->connectMyInputTo(in1);

	ossimNitfFileHeaderV2_1 tfh = writer->theFileHeader;
	ossimNitfImageHeaderV2_1 tih = writer->theImageHeader;



	/*
	 * FHDR    = NSIF
FVER    = 01.00
CLEVEL  = 03
STYPE   = BF01
OSTAID  = test
FDT     = 20100610182943
FTITLE  = TEST
FSCLAS  = U

FSCOP   = 00000
FSCPYS  = 00000
ENCRYP  = 0
FBKGC   =  ,  ,
ONAME   =
OPHONE  =
FL      = 000001354474
HL      = 000432
NUMI    = 1
NUMS    = 0
NUMT    = 0
NUMDES  = 0
NUMRES  = 0
UDHDL   = 0
XHDL    = 0
	 */

	tfh.theFileTypeVersion 		= "NSIF";
	tfh.theComplexityLevel 		= "03";
	tfh.theSystemType			= "BF01";
	tfh.theOriginatingStationId = "test";
	tfh.theDateTime				= "20100610182943";
	tfh.theFileTitle			= "TEST";
	tfh.theSecurityClassification = "U";

	//tih.setSecurityClassification(ossimString("U"));

	/*
	 ---------------- New Image Header ---------------
	Header size = 372
	IM       = IM
	IID1     = IMA001
	IDATIM   = 19730901000000
	TGTID    =
	IID2     = TEST
	ISCLASS  = U
	ISCLSY   =
	ISCODE   =
	ISCTLH   =
	ISREL    =
	ISDCTP   =
	ISDCDT   =
	ISDCXM   =
	ISDG     =
	ISDGDT   =
	ISCLTX   =
	ISCATP   =
	ISCAUT   =
	ISCRSN   =
	ISSRDT   =
	ISCTLN   =
	ENCRYP   = 0
	ISORCE   =
	NROWS    = 00000599
	NCOLS    = 00000751
	PVTYPE   = INT
	IREP     = RGB
	ICAT     = VIS
	ABPP     = 08
	PJUST    = R
	ICORDS   = D
	IGEOLO   = +37.160+067.461+37.085+069.142+36.013+069.058+36.085+067.399
	NICOM    = 0
	IC       = NC
	NBANDS   = 3
*/

	tih.theType 					= "IM";
	tih.theImageId 					= "IMA001";
	tih.theDateTime 				= "19730901000000";
	tih.theTitle 					= "TEST";
	tih.theSecurityClassification 	= "U";
	tih.theEncryption				= "0";

	tih.theGeographicLocation		= "+37.160+067.461+37.085+069.142+36.013+069.058+36.085+067.399";

	//ossimMapProjection mp = new ossimMapProjection();

	//writer->addBlockaTag();

	//ossimImageMetaData m = in1->theMetaData;


	writer->execute();




}


