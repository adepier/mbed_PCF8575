 
set(PCF8575_SOURCES PCF8575.cpp) 
add_library(PCF8575 STATIC ${PCF8575_SOURCES})
target_link_libraries( PCF8575 mbed-os)

set(PCF8575_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(PCF8575 PUBLIC ${PCF8575_HEADER_DIR})