set(BINARY ${CMAKE_PROJECT_NAME}_test)

enable_language(CXX)
FILE(GLOB TEST_FILES *.cpp)

set(SOURCES ${TEST_FILES})

add_executable(${BINARY} ${TEST_FILES})

target_link_libraries(${BINARY} "smallengine" "SDL2" gtest)
