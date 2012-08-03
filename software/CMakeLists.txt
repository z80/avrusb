
cmake_minimum_required( VERSION 2.8 )

# ѕереопределение условий компил€ции, чтобы 
# не нужно было Microsoft Redistributable 
# устанавливать.
#set(CMAKE_USER_MAKE_RULES_OVERRIDE
#    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
#set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
#    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

project( "avrusb" )
set( INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/app" CACHE PATH "Path to install to" )
set( CMAKE_INSTALL_PREFIX ${INSTALL_PATH} )
set( ROOT ${CMAKE_CURRENT_SOURCE_DIR} )
if ( WIN32 )
    include( "./config-win32.cmake" )
elseif ( UNIX )
    include( "./config-unix.cmake" )
endif ( WIN32 )

add_subdirectory( "${ROOT}/3dparty/qwt-6.0.1" )
add_subdirectory( "${ROOT}/libio" )
add_subdirectory( "${ROOT}/libwidgets" )
add_subdirectory( "${ROOT}/libadvwidgets" )
add_subdirectory( "${ROOT}/application" )

