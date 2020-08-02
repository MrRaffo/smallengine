cmake_minimum_required(VERSION 3.7)
project (smallengine 
    VERSION 0.1.0 
    LANGUAGES C)

# get header files
file(GLOB HEADERS inc/smallengine/**/*.h)

# get source files
file(GLOB GRAPHICS_SRCS src/smallengine/graphics/*.c)
file(GLOB MATHS_SRCS src/smallengine/maths/*.c)
file(GLOB SYS_SRCS src/smallengine/sys/*.c)

add_library(smallengine SHARED
    ${GRAPHICS_SRC}
    ${MATHS_SRCS}
    ${SYS_SRCS}
)

set_target_properties(smallengine PROPERTIES 
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)
set_target_properties(smallengine PROPERTIES PUBLIC_HEADER ${HEADERS})

target_include_directories(smallengine PRIVATE "./inc/")

include(GNUInstallDirs)
install(TARGETS smallengine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smallengine")

