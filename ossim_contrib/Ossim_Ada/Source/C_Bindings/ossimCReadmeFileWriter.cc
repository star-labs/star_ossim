//*****************************************************************************
// Copyright (C) 2003 James E. Hopper & ImageLinks Inc. 
//
// This is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: James E. Hopper (hopperj@macconnect.com)
//
//*****************************************************************************
#include "ossimCReadmeFileWriter.h"
#include "imaging/metadata/ossimReadmeFileWriter.h"

	void deleteOssimReadmeFileWriter(ossimCReadmeFileWriterH readMe)
	{
		if(readMe)
		{
			delete (ossimReadmeFileWriter*)readMe;
		}
	}
   
 	ossimCReadmeFileWriterH createOssimReadmeFileWriter()
	{
		ossimReadmeFileWriter *rM = new ossimReadmeFileWriter();
		return (ossimCReadmeFileWriterH)rM;
	}
   
	ossimCReadmeFileWriterH createConnectedOssimReadmeFileWriter(
		ossimConnectableObjectH inputSource, const ossimFilenameH filename)
	{
		ossimReadmeFileWriter *rM = new ossimReadmeFileWriter(
			(ossimImageSource*)inputSource, *(ossimFilename*)filename);
		return (ossimCReadmeFileWriterH)rM;
	}
	
	ossimCReadmeFileWriterH createConnectedOssimReadmeFileWriterS(
		ossimConnectableObjectH inputSource, char* filename)
	{
		ossimFilename fn(filename);
		ossimReadmeFileWriter *rM = new ossimReadmeFileWriter(
			(ossimImageSource*)inputSource, fn);
		return (ossimCReadmeFileWriterH)rM;
	}

void ossimExecuteReadmeFileWriter(ossimCReadmeFileWriterH readMe)
{
	bool result;
	result = ((ossimReadmeFileWriter*)readMe)->execute();
}

