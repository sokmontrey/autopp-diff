add_library(${PROJECT_NAME}
    node.h
    node.cpp

    operator.h
)

set(TARGETS ${PROJECT_NAME})

# Target aliases
add_library(${PROJECT_NAME}::nodeflow ALIAS nodeflow)

# Include dirs
target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)