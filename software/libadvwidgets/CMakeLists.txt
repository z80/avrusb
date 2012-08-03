
set( BUILD_SHARED_LIBS OFF )
find_package( Qt4 )
set( QT_USE_QTSVG    True )
set( QT_USE_QTSCRIPT True )
include( ${QT_USE_FILE} )

list( APPEND src "./src/led/qled.cpp" )
list( APPEND src "./src/switch/qswitch.cpp" )
list( APPEND src "./src/img_button/img_button.cpp" )

aux_source_directory( "./src/analog_widgets/" s )
list( APPEND src ${s} )
aux_source_directory( "./src/analog_widgets/chart" s )
list( APPEND src ${s} )
aux_source_directory( "./src/analog_widgets/chart/standard" s )
list( APPEND src ${s} )

include_directories( "./src/led" 
                     "./src/switch" 
                     "./src/img_button"
                     "./src/analog_widgets" )

QT4_ADD_RESOURCES( qrc_src "./src/led/qled.qrc" )
list( APPEND src ${qrc_src} )
QT4_ADD_RESOURCES( qrc_src "./src/img_button/img_button.qrc" )
list( APPEND src ${qrc_src} )
QT4_ADD_RESOURCES( qrc_src "./src/analog_widgets/analogwidgets.qrc" )
list( APPEND src ${qrc_src} )

list( APPEND moc_h "./src/led/qled.h" )
list( APPEND moc_h "./src/switch/qswitch.h" )
list( APPEND moc_h "./src/img_button/img_button.h" )

list( APPEND moc_h "./src/analog_widgets/abstractmeter.h" )
list( APPEND moc_h "./src/analog_widgets/bitmapbutton.h" )
list( APPEND moc_h "./src/analog_widgets/chart.h" )
list( APPEND moc_h "./src/analog_widgets/counter.h" )
list( APPEND moc_h "./src/analog_widgets/led.h" )
list( APPEND moc_h "./src/analog_widgets/manometer.h" )
list( APPEND moc_h "./src/analog_widgets/potentiometer.h" )
list( APPEND moc_h "./src/analog_widgets/scriptwidget.h" )
list( APPEND moc_h "./src/analog_widgets/thermometer.h" )
list( APPEND moc_h "./src/analog_widgets/wallclock.h" )
list( APPEND moc_h "./src/analog_widgets/widgetwithbackground.h" )

QT4_WRAP_CPP( moc_src ${moc_h} )
list( APPEND src ${moc_src} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )


add_library( advwidgets STATIC ${src} )
target_link_libraries( advwidgets ${QT_LIBRARIES} )

#set_property( TARGET advwidgets PROPERTY DEFINE_SYMBOL ADVWIDGETS_MAKEDLL )








