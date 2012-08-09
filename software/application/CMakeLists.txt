
find_package( Qt4 )
set( QT_USE_QTCORE    True )
set( QT_USE_QTGUI     True )
set( QT_USE_QTUITOOLS True )
if ( WIN32 )
    set( QT_USE_QTMAIN True )
endif ( WIN32 )
include( ${QT_USE_FILE} )

include_directories( "./src" 
                     "${ROOT}/libio/src" 
                     "${ROOT}/libwidgets/src" 
                     "${ROOT}/libadvwidgets/src/led" 
                     "${ROOT}/libadvwidgets/src/switch" 
                     "${ROOT}/3dparty/qwt-6.0.1/src" 
                     "${CMAKE_CURRENT_BINARY_DIR}" )
if ( WIN32 )
    include_directories( "${BOOST_INC_PATH}" )
endif ( WIN32 )

file( GLOB src     "./src/*.cpp" )
file( GLOB headers "./src/*.h" )
file( GLOB ui      "./src/*.ui" )
file( GLOB qrc     "./src/*.qrc" )
message( STATUS "SRC = ${src}" )
message( STATUS "HEADERS = ${headers}" )

Qt4_WRAP_CPP( mocSrc ${headers} )
set( src ${src} ${mocSrc} )

QT4_WRAP_UI( uiSrc ${ui} )
set( src ${src} ${uiSrc} )

QT4_ADD_RESOURCES( qrcSrc ${qrc} )
set( src ${src} ${qrcSrc} )

add_executable( application ${src} )
target_link_libraries( application qwt-6.0.1 widgets advwidgets io ${QT_LIBRARIES} )
install( TARGETS application DESTINATION "." )
