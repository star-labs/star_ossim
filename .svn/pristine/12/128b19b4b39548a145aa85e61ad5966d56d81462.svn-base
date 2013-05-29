import sys
sys.path.append("../lib")
from pyossim import *

def pyossimDem(argc,argv):

   originalArgCount = argc
   if argc > 1  or argc == originalArgCount:
      # Make the generator.
      eu = ossimElevUtil()
      try:

         # NOTE: ossimElevUtil::initialize handles the application usage which will
         # false, to end things if certain options (e.g. "--help") are provided.
         #
         # ossimElevUtil::initialize can throw an exception.
         #--
         print "doing ossim-dem"
         continue_after_init = eu.initialize(ap)
         if continue_after_init:
             
            # ossimElevUtil::execute can throw an excepion.
            eu.execute()
            #print "elapsed time in seconds: " ossimTimer.instance().time_s()

      except:
         sys.exit(1)
      
      
   sys.exit(0)
   
if __name__ == "__main__":
    ossimInit.instance().initialize()
    pyossimDem(len(sys.argv),sys.argv)
