#
#  Crates ‐ extensible 3D puzzle game.
#  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
set (CRATES_VERSION_MAJOR 0)
set (CRATES_VERSION_MINOR 6)
set (CRATES_VERSION_PATCH 2)
set (CRATES_VERSION ${CRATES_VERSION_MAJOR}.${CRATES_VERSION_MINOR}.${CRATES_VERSION_PATCH})
set (CRATES_VERSION_STRING \"crates-${CRATES_VERSION}\")

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
message(FATAL_ERROR "
In-source builds for crates are not permitted! Remove the files that cmake just created and try again from another directory, for example:
rm -rf CMakeCache.txt CMakeFiles
cd ..
mkdir crates-${CRATES_VERSION}-build
cd crates-${CRATES_VERSION}-build
cmake ../crates-${CRATES_VERSION}")
endif(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)

cmake_minimum_required (VERSION 2.6)
project (crates C)
set (CMAKE_C_FLAGS "-Wall -ansi -pedantic")
set (CMAKE_C_FLAGS_DEBUG "-g -Wall -ansi -pedantic")
set (MAKE_C_FLAGS_RELEASE  "-Wall -O3")
configure_file ("${PROJECT_SOURCE_DIR}/src/oconfig.h.in" "${PROJECT_SOURCE_DIR}/src/oconfig.h")
include_directories ("${PROJECT_BINARY_DIR}")

find_package(Lua51 REQUIRED)
if(NOT LUA51_FOUND)
  message(FATAL_ERROR "Lua 5.1 not found. Install it and try again.")
endif(NOT LUA51_FOUND)

find_package(SDL REQUIRED)
if(NOT SDL_FOUND)
  message(FATAL_ERROR "SDL not found. Install it and try again.")
endif(NOT SDL_FOUND)

find_package(SDL_mixer REQUIRED)
if(NOT SDLMIXER_FOUND)
  message(FATAL_ERROR "SDL_mixer not found. Install it and try again.")
endif(NOT SDLMIXER_FOUND)

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message(FATAL_ERROR "OpenGL and GLU not found. Install them and try again.")
endif(NOT OPENGL_FOUND)

find_package(PNG REQUIRED)
if(NOT PNG_FOUND)
  message(FATAL_ERROR "PNG not found. Install libpng and try again.")
endif(NOT PNG_FOUND)

find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
  message(FATAL_ERROR "ZLIB not found. Install libz and try again.")
endif(NOT ZLIB_FOUND)

include_directories(${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

FILE (GLOB COMMONFILES ${CMAKE_SOURCE_DIR}/src/*.[ch])

# These compiler flags are set here again because we do not want "-ansi -pedantic" to
# the CFLAGS when the platform specific code in the subdirectories of src is compiled.
set (CMAKE_C_FLAGS "-Wall")
set (CMAKE_C_FLAGS_DEBUG "-g -Wall")
set (MAKE_C_FLAGS_RELEASE  "-Wall -O3")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  add_executable(crates ${COMMONFILES} src/posix/odir.c  src/posix/otimer.c  src/posix/linux/oexedirname.c)
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
  add_executable(crates ${COMMONFILES} src/posix/odir.c  src/posix/otimer.c  src/posix/macosx/oexedirname.c)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  add_executable(crates ${COMMONFILES} src/windows/odir.c  src/windows/otimer.c)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
  add_executable(crates ${COMMONFILES} src/posix/odir.c  src/posix/otimer.c  src/posix/freebsd/oexedirname.c)
endif ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

target_link_libraries(crates ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${OPENGL_LIBRARY} ${PNG_LIBRARY} ${ZLIB_LIBRARY} ${LUA_LIBRARY})

# Test that program starts and gives the correct version information.
# TODO: Tests in the test-directory should be run here also.
enable_testing()
add_test (cratesVersion ${CMAKE_SOURCE_DIR}/crates --version)
set_tests_properties (cratesVersion PROPERTIES PASS_REGULAR_EXPRESSION "crates-${CRATES_VERSION}")

include (InstallRequiredSystemLibraries)
set (ESCAPE_QUOTES ON)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_PACKAGE_VERSION_MAJOR "${CRATES_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${CRATES_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${CRATES_VERSION_PATCH}")
include (CPack)
