set(LIB_NAME ossimmrsid_plugin)
MESSAGE( "************** LIBRARY SETUP FOR ossimmrsid_plugin******************")

find_package(MrSid)
if(MRSID_FOUND)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   include_directories(${MRSID_INCLUDE_DIR})
else(MRSID_FOUND)
   message(FATAL_ERROR "Could not find mrsid")
endif(MRSID_FOUND)

find_package(GDAL)
if( NOT GDAL_FOUND)
   message(FATAL_ERROR "Could not find gdal")
endif( not GDAL_FOUND)

MESSAGE( STATUS "MRSID_LIBRARIES     = ${MRSID_LIBRARIES}" )
MESSAGE( STATUS "MRSID_INCLUDE       = ${MRSID_INCLUDE_DIR}" )
MESSAGE( STATUS "MRSID_WRITE_FOUND   = ${MRSID_WRITE_FOUND}" )

####
# OSSIMPLUGINSMAKINGDLL controls dll linkage on windows.  
# Adding this sets OSSIM_PLUGINS_DLL #define TO "__declspec(dllexport)".
###
add_definitions("-DOSSIMPLUGINSMAKINGDLL")

FILE(GLOB_RECURSE OSSIMPLUGIN_SRCS *.cpp)
FILE(GLOB_RECURSE OSSIMPLUGIN_HEADERS *.h)

OSSIM_LINK_LIBRARY(${LIB_NAME}
                   COMPONENT_NAME ossim TYPE "${OSSIM_PLUGIN_LINK_TYPE}"
		   LIBRARIES ${OSSIM_LIBRARIES} ${MRSID_LIBRARIES} ${GDAL_LIBRARY} 
                   HEADERS "${OSSIMPLUGIN_HEADERS}"
		   SOURCE_FILES "${OSSIMPLUGIN_SRCS}"
                   INSTALL_LIB)

