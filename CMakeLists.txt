# CMake version
cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0037 OLD)

# CMake project
project ( libxse )
set ( LIBRARY "xse" )

# Variables
set ( SOURCE "./src/*.cpp" )											# Source directory
set ( TEST "./test/*.cpp" )												# Test directory
set ( INCLUDE "./include" )												# Include directory
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )	# Library directory

# Source files
file(GLOB SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} $
	"src/*.cpp"
)

# Test files
file(GLOB TEST_FILES RELATIVE ${CMAKE_SOURCE_DIR}
	"test/*.cpp"
)

# Include header files
include_directories( ${INCLUDE} )
link_directories( ${LIBRARY_OUTPUT_DIRECTORY} )

# Source build
add_library( ${LIBRARY} STATIC ${SRC_FILES})
target_include_directories( ${LIBRARY} INTERFACE /include)

# Test build
#foreach ( TEST_FILE ${TEST_FILES} )
#	string( REPLACE ".cpp" ".out" TEST_OBJ ${TEST_FILE} )
#	add_executable( ${TEST_OBJ} ${TEST_FILE} )
#	target_link_libraries(${TEST_OBJ} xse pthread)
#endforeach( TEST_FILE ${TEST_FILES} )

install(
	TARGETS ${LIBRARY} 
	ARCHIVE DESTINATION /usr/lib
)

install(
    DIRECTORY ${INCLUDE}
    DESTINATION /usr
    FILES_MATCHING PATTERN "*.h*"
)