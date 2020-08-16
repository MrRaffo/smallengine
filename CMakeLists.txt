cmake_minimum_required(VERSION 3.7)
project (smallengine 
    VERSION 0.1.0 
    LANGUAGES C)

# get header files
file(GLOB HEADERS inc/smallengine/*.h)
#message(${HEADERS})

# get source files
file(GLOB GRAPHICS_SRCS src/graphics/*.c)
file(GLOB MATHS_SRCS src/maths/*.c)
file(GLOB SYS_SRCS src/sys/*.c)

add_library(smallengine SHARED
    ${HEADERS}
    ${GRAPHICS_SRC}
    ${MATHS_SRCS}
    ${SYS_SRCS}
)

set_target_properties(smallengine PROPERTIES 
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)
set_target_properties(smallengine PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_include_directories(smallengine PUBLIC "./inc/")
target_include_directories(smallengine PRIVATE "./src/")

include(GNUInstallDirs)
install(TARGETS smallengine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smallengine")

add_subdirectory(test)
add_subdirectory(lib/googletest)
