
aux_source_directory( "./src" src )
message( STATUS "src: \"${src}\"" )

include( "../cmake/findlibusb-1.0.cmake" )
message( STATUS ${LIBUSB_1_DEFINITIONS} )

include_directories( "./src" 
                     "${ROOT}/../common" 
                     ${GLOB_BIN_DIR} )
if ( UNIX )
    include_directories( "${LIBUSB_1_INCLUDE_DIRS}" )
elseif ( WIN32 )
    include_directories( "${LIBUSB_INC_PATH}" 
                         "./src/libusb" )
endif ( UNIX )
set( src ${src} ${s} )

add_library( io STATIC ${src} )
if ( UNIX )
    target_link_libraries( io ${LIBUSB_1_LIBRARIES} )
elseif ( WIN32 )
    find_library( libusb NAMES libusb-1.0.lib PATHS "${LIBUSB_LIB_PATH}" )
    target_link_libraries( io ${libusb} )
endif ( UNIX )




