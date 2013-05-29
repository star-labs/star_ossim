import sys
sys.path.append("../lib")
from lib.pyossim import *

def pyossimAdrgHeaderdump(argc,argv):
    if argc !=2:
        print "Usage: python ossim-adrg-dump.py <.imgfile>";
        sys.exit(0)



    # ADRG header file.
    f = ossimFilename(argv[1])

    print "Trying to open header:  "  + argv[1] 
        
    # Instantiate support data class to parse all needed header files.
    theAdrgHeader = ossimAdrgHeader(f)


    # Check for errors.
    if theAdrgHeader.errorStatus():
        print "Error in ossimAdrg header detected. "
        sys.exit(1)
   

    # Dump header to stdout.
    print theAdrgHeader

   

  
    

if __name__ == "__main__":
    ossimInit.instance().initialize()
    pyossimAdrgHeaderdump(len(sys.argv),sys.argv)

