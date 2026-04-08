cmake_minimum_required(VERSION 3.23)

find_package(Doxygen)

if (NOT DOXYGEN_FOUND) 
    message("Could not find doxygen. Make sure it is installed.")
    return()
endif ()
    
set(DOXYGEN_IN docs/Doxyfile.in)
set(DOXYGEN_OUT Doxyfile)

configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

add_custom_target( doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )
