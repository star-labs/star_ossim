//-----------------------------------------------------------------------------
// File:  OssimInfo.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java ossim-info app:
//
//-----------------------------------------------------------------------------
// $Id: OssimInfo.java 21254 2012-07-07 22:08:10Z dburken $

package org.ossim.jni.apps;

import org.ossim.jni.Info;
import org.ossim.jni.Init;

public class OssimInfo
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
      newArgs[0] = "org.ossim.jni.apps.OssimInfo";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( argc >= 1 )
      {
         org.ossim.jni.Info info = new org.ossim.jni.Info();

         //---
         // Info::initialize returns true to  indicate the process should continue with execute.
         //---
         if ( info.initialize( newArgs.length, newArgs) )
         {
            try
            {
               info.execute();
            }
            catch( Exception e )
            {
               System.err.println("Caught Exception: " + e.getMessage());
            }
         }
      }
   }
}

