cmake_minimum_required(VERSION 2.6)
# - Try to find precompiled headers support for GCC 3.4 and 4.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#
# Macro:
#   ADD_PRECOMPILED_HEADER

IF(CMAKE_COMPILER_IS_GNUCXX)
  EXEC_PROGRAM(
    ${CMAKE_CXX_COMPILER}
    ARGS --version
    OUTPUT_VARIABLE _compiler_output)
  STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"
    gcc_compiler_version ${_compiler_output})
  #MESSAGE("GCC Version: ${gcc_compiler_version}")
  IF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    SET(PCHSupport_FOUND TRUE)
  ELSE(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    IF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
      SET(PCHSupport_FOUND TRUE)
    ENDIF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
  ENDIF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

MACRO(ADD_PRECOMPILED_HEADER _targetName _input )

  GET_FILENAME_COMPONENT(_name ${_input} NAME)
  SET(_source "${CMAKE_CURRENT_SOURCE_DIR}/${_input}")
  SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
  MAKE_DIRECTORY(${_outdir})
  SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
  STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
  SET(_compiler_FLAGS ${${_flags_var_name}})
 ## SET(CMAKE_CXX_FLAGS "-lsfml-audio")
  GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
  FOREACH(item ${_directory_flags})
    LIST(APPEND _compiler_FLAGS "-I${item}")
  ENDFOREACH(item)

  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  LIST(APPEND _compiler_FLAGS ${_directory_flags})

  SEPARATE_ARGUMENTS(_compiler_FLAGS)
  #MESSAGE("_compiler_FLAGS: ${_compiler_FLAGS}")
  message("${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header -o ${_output} ${_source}")
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_output}
    COMMAND ${CMAKE_CXX_COMPILER}
    ${_compiler_FLAGS}
    -x c++-header
    -o ${_output} ${_source}
    DEPENDS ${_source} )
  ADD_CUSTOM_TARGET(${_targetName}_gch DEPENDS ${_output})
  ADD_DEPENDENCIES(${_targetName} ${_targetName}_gch)
  #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-include ${_name} -Winvalid-pch -H")
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${_name} -Winvalid-pch")
  SET_TARGET_PROPERTIES(${_targetName} PROPERTIES
    COMPILE_FLAGS "-include ${_name} -Winvalid-pch -lpthread"
    )

ENDMACRO(ADD_PRECOMPILED_HEADER)

project(worms)

add_definitions(-std=c++11 -fPIC)

include_directories("dependancies/SFML-2.3.2/include/")
include_directories("dependancies/Ogre/OgreMain/include/")
include_directories("dependancies/Ogre/include/")
include_directories("dependancies/CEGUI/cegui/include/")
include_directories("dependancies/OIS/includes")
include_directories("dependancies/jsoncpp/include")
include_directories("./includes/")
link_directories(libs/)
add_custom_target(install ./install.sh)
add_library(wormsIA SHARED src/IA/IA.cpp)

add_executable(
	worms
	src/main.cpp
	src/WormsApp.cpp
	src/UIManager.cpp
	src/PerlinNoise.cpp
	src/RandMap.cpp
	src/InputListener.cpp
	src/PhysBody.cpp
	src/CollisionManager.cpp
	src/UIManager.cpp
	src/AScene.cpp
	src/Scene.cpp
	src/Menu.cpp
	src/SceneManager.cpp
	src/Game.cpp
	src/Team.cpp
	src/Worms.cpp
	src/World.cpp
	src/gun/Bazooka.cpp
	src/gun/Grenade.cpp
	src/gun/Gun.cpp
	src/gun/GunLongRange.cpp
	src/gun/GunShortRange.cpp
	src/gun/AWP.cpp
  src/gun/Nuke.cpp
	src/SoundManager.cpp
	src/UserInput.cpp
)

TARGET_LINK_LIBRARIES(worms wormsIA)
link_directories("./libs/")
add_precompiled_header( worms includes/data.hh)

target_link_libraries(
	worms

	OIS
	boost_system
  jsoncpp
	OgreMain
	OgreOverlay
	OgreTerrain
	CEGUIOgreRenderer-0
	CEGUIBase-0
	sfml-audio
	pthread
	dl
)
