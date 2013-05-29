//-----------------------------------------------------------------------------
// File:  ImageTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Test app for Image class.
//
// Usage example:
// $ java -Djava.library.path=/work/osgeo/ossimjni/java/build/lib -cp
// /work/osgeo/ossimjni/java/build/lib/ossimjni.jar org.ossim.jni.test.ElevMgrTest
// <latitude> <longitude> 
//
//-----------------------------------------------------------------------------
// $Id: ElevMgrTest.java 20188 2011-10-23 23:13:30Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.ElevMgr;
import org.ossim.jni.Init;
import java.lang.Double;

public class ElevMgrTest
{
   static
   {
      System.loadLibrary( "ossimjni-swig" );
   }
   
   /**
    * @param args
    */
   public static void main( String[] args )
   {
      // Copy the args with app name for c++ initialize.
      String[] newArgs = new String[args.length + 1];
      newArgs[0] = "org.ossim.oms.apps.ImageTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( args.length == 2 )
      {
         try
         {
            double lat = Double.parseDouble( args[0] );
            double lon = Double.parseDouble( args[1] );
            double hEllips = ElevMgr.instance().getHeightAboveEllipsoid( lat, lon );
            double hMsl    = ElevMgr.instance().getHeightAboveMSL( lat, lon );            

            System.out.println( "latitude:            " + lat );
            System.out.println( "longitude:           " + lon );
            System.out.println( "Height of ellipsoid: " + hEllips );
            System.out.println( "Height of MSL:       " + hMsl );            
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.oms.test.ElevMgrTest <latitude> <longitude>" );
      }
   } // end of main
}

