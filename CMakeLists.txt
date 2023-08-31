cmake_minimum_required(VERSION 3.24)

# Configuration of the project needs to set VERSION to generate .cmake files and
# to use find_package.
project(
  nodeflow
  LANGUAGES CXX
  VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# You need to explicitly set BUILD_SHARED_LIBS option to enable static / shared
# library. But don't customize variable name.
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Set default library path to install
include(GNUInstallDirs)
# Features to generate .cmake file for find_package.
include(CMakePackageConfigHelpers)

# Build of the library
add_subdirectory(source)

# Enable tests.
include(CTest)
# BUILD_TESTING option is defined via include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# package
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mylibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# install
install(
    TARGETS ${PROJECT_NAME} 
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# install header
install(
    DIRECTORY source/${PROJECT_NAME}  # Adjust the path to your header files
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)