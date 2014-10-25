cmake_minimum_required(VERSION 2.8)
project(OgreMascaret)

set(SOURCE_FILES
	src/VRPlateform/OgreAnimationSpecification.cpp
	src/VRPlateform/OgreCamera.cpp
	src/VRPlateform/OgreLight.cpp
	src/VRPlateform/OgreScene.cpp
	src/VRPlateform/OgreMascaretApplication.cpp
	src/VRPlateform/OgrePointSpecification.cpp
	src/VRPlateform/OgreShapeSpecification.cpp
	src/VRPlateform/OgreSoundSpecification.cpp
	src/VRPlateform/OgreWindow3D.cpp
	src/VRPlateform/OgreRenderToTexture.cpp
	src/VRPlateform/cameraMan.cpp
	src/Animation/OgreAnimation.cpp
	src/Interaction/OgreMouse.cpp
	src/Interaction/OgreCursor.cpp
	src/Interaction/OgreKeyboard.cpp
	src/Assistance/OgreColor.cpp
	src/OgreVRComponentFactory.cpp
	src/Tools/Sound3D.cpp
	src/Tools/alffmpeg.c
)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-W -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-long-long)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions(-D_VEHA_LIB -D_HTTPSERVER_LIB -DNOPYTHON -D_OGRE_MASCARET_LIB)
include_directories(./include/ ./include/Tools/Network)

find_package(LibXml2 REQUIRED)
include_directories(SYSTEM ${LIBXML2_INCLUDE_DIR})
link_directories(${LIBXML2_LIBRARY_DIR})

find_package(OpenAL REQUIRED)
include_directories(SYSTEM ${OPENAL_INCLUDE_DIR})
link_directories(${OPENAL_LIBRARY_DIR})

set(BOOST_COMPONENTS thread date_time system filesystem regex signals)
message(BOOST_COMPONENTS=${BOOST_COMPONENTS})
# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
set(OGRE_BOOST_COMPONENTS thread date_time system filesystem iostreams program_options)
include(FindBoost)
find_package(
	Boost
	1.36.0
	COMPONENTS ${BOOST_COMPONENTS} ${OGRE_BOOST_COMPONENTS} REQUIRED
)
if (NOT Boost_FOUND)
  # Try again with the other type of libs
  set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
  find_package(
    Boost
    1.36.0
    COMPONENTS ${BOOST_COMPONTENTS} ${OGRE_BOOST_COMPONENTS} REQUIRED
  )
endif()

if(WIN32)
    if(NOT CMAKE_CROSSCOMPILING)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
        set(OGRE_SAMPLES_INCLUDEPATH $ENV{OGRE_HOME}/Samples/include)
    else(NOT CMAKE_CROSSCOMPILING)
        set(CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}/CMake;${CMAKE_MODULE_PATH}")
        #set(OGRE_SAMPLES_INCLUDEPATH $ENV{OGRE_HOME}/Samples/include)
    endif(NOT CMAKE_CROSSCOMPILING)
endif(WIN32)

if(UNIX)
  if(EXISTS "/usr/local/lib/OGRE/cmake")
    message("using /usr/local/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

  elseif(EXISTS "/usr/share/OGRE/cmake/modules")
    message("using /usr/share/OGRE/cmake/modules")
    set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

  elseif(EXISTS "/usr/lib/OGRE/cmake")
    message("using /usr/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

  elseif(APPLE)
    set(OGRE_HOME "$ENV{OGRE_HOME}")
    if(OGRE_HOME STREQUAL "")
        message(SEND_ERROR "\nOGRE_HOME not defined !\n")
    endif(OGRE_HOME STREQUAL "")
    message("using ${OGRE_HOME}")
    set(CMAKE_MODULE_PATH "${OGRE_HOME}/CMake;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "${OGRE_HOME}/Samples/Common/include/") # Otherwise, this one

 else ()
    message(SEND_ERROR "Failed to find module path.")
  endif(EXISTS "/usr/local/lib/OGRE/cmake")
endif(UNIX)
 

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (CMAKE_BUILD_TYPE STREQUAL "")

set(OGRE_COMPONENTS RenderSystem_GL)

find_package(OGRE REQUIRED)
message(OGRE_RenderSystem_GL_FOUND=${OGRE_RenderSystem_GL_FOUND})
message(OGRE_RenderSystem_GL_LIBRARY_REL=${OGRE_RenderSystem_GL_LIBRARY_REL})
find_package(OIS REQUIRED)
if(NOT OIS_FOUND)
 message(SEND_ERROR "Failed to find OIS.")
endif()
include_directories(SYSTEM
  ${OIS_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${OGRE_SAMPLES_INCLUDEPATH}
  ${Boost_INCLUDE_DIRS}
  ${OGRE_RenderSystem_GL_INCLUDE_DIRS}
)

#add_subdirectory(../Mascaret ${CMAKE_CURRENT_BINARY_DIR}/Mascaret/Mascaret)
add_subdirectory(../Mascaret Mascaret)
#set(MASCARET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Mascaret)
#find_library(Mascaret_FOUND Mascaret NAMES Mascaret PATHS ${MASCARET_DIR} NO_DEFAULT_PATH)
#message(STATUS "#### Mascaret_FOUND=${Mascaret_FOUND}")
##if(Mascaret_FOUND STREQUAL "Mascaret_FOUND-NOTFOUND")
##  add_custom_command(TARGET Mascaret PRE_BUILD COMMAND ${PYTHON} configure)
##endif(Mascaret_FOUND STREQUAL "Mascaret_FOUND-NOTFOUND")
#get_filename_component(Mascaret_LIB_DIR ${Mascaret_FOUND} PATH)

include_directories(${Mascaret_SOURCE_DIR}/include)

link_directories(${Mascaret_BINARY_DIR} ${Boost_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})

# Library
add_library(OgreMascaret SHARED ${SOURCE_FILES})
add_dependencies(OgreMascaret Mascaret)
target_link_libraries(OgreMascaret Mascaret ${Boost_LIBRARIES} ${OPENAL_LIBRARY} ${LIBXML2_LIBRARIES} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})
if(WIN32)
    set_target_properties(OgreMascaret PROPERTIES LINK_FLAGS "-Wl,--enable-auto-import")
    target_link_libraries(OgreMascaret ws2_32 mswsock)
    target_link_libraries(OgreMascaret ${CMAKE_PREFIX_PATH}/lib/libavformat.dll.a)
    target_link_libraries(OgreMascaret ${CMAKE_PREFIX_PATH}/lib/libavcodec.dll.a)
    target_link_libraries(OgreMascaret ${CMAKE_PREFIX_PATH}/lib/libavutil.dll.a)
endif(WIN32)

# Executable
add_executable(ogreMascaret src/main.cpp)
add_dependencies(ogreMascaret OgreMascaret)
target_link_libraries(ogreMascaret OgreMascaret)
if(WIN32)
    target_link_libraries(ogreMascaret ws2_32 mswsock avformat avcodec avutil)
else(WIN32)
    target_link_libraries(ogreMascaret dl pthread avformat avcodec avutil)
endif(WIN32)

if(WIN32 AND CMAKE_CROSSCOMPILING)
    target_link_libraries(ogreMascaret OgreMascaret ${LIBXML2_LIBRARIES} ${OPENAL_LIBRARY})
endif()

# Linkage statique de libgcc et libstdc++ en cas de crosscompilation
if(WIN32 AND CMAKE_CROSSCOMPILING AND CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(OgreMascaret PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
    set_target_properties(ogreMascaret PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()
