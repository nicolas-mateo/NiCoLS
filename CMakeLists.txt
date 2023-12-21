cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  NiCoLS
  VERSION 1.0
  LANGUAGES CXX
)

# Particularities

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 9.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

find_package(VTK COMPONENTS CommonCore CommonDataModel CommonMath IOLegacy)
message(STATUS "VTK_VERSION: ${VTK_VERSION}")
if(NOT VTK_FOUND)
  list(
    APPEND
    VTK_OPTIONS
    "BUILD_SHARED_LIBS OFF"
    "BUILD_TESTING OFF"
    "VTK_BUILD_EXAMPLES OFF"
    "VTK_BUILD_TESTING OFF"
    "VTK_ENABLE_WRAPPING OFF"
    "VTK_GROUP_ENABLE_Rendering DONT_WANT"
    "VTK_GROUP_ENABLE_StandAlone DONT_WANT"
    "VTK_USE_64BIT_IDS ON"
    "VTK_MODULE_ENABLE_VTK_CommonCore YES"
    "VTK_MODULE_ENABLE_VTK_CommonDataModel YES"
    "VTK_MODULE_ENABLE_VTK_CommonMath YES"
    "VTK_MODULE_ENABLE_VTK_IOLegacy YES"
  )

  CPMAddPackage(
    NAME VTK
    GITHUB_REPOSITORY kitware/vtk
    VERSION 9.3.0
    OPTIONS ${VTK_OPTIONS}
    EXCLUDE_FROM_ALL YES GIT_SUBMODULES "" # Disable submodules
  )
endif()

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(
  ${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->" -Wall -O3
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PUBLIC ${VTK_LIBRARIES})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 9.1.0" "VTK 9.3.0"
)
