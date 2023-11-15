cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
# find_package(GLEW REQUIRED)
# message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
if(APPLE)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype irrKlang)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
 else()
   set(LIBS )
 endif(WIN32)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

function(create_project_from_sources)
	file(GLOB_RECURSE SOURCE
             "src/*.h"
             "src/*.hpp"
             "src/*.cpp"
             "src/*.vs"
             "src/*.fs"
             "src/*.tcs"
             "src/*.tes"
             "src/*.gs"
             "src/*.cs"
             "src/**/*.h"
             "src/**/*.hpp"
             "src/**/*.cpp"
             "src/**/*.vs"
             "src/**/*.fs"
             "src/**/*.tcs"
             "src/**/*.tes"
             "src/**/*.gs"
             "src/**/*.cs"
    )
    set(NAME "Game")
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})
    if(MSVC)
		    target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
        target_link_options(${NAME} PUBLIC /ignore:4099)
    else()
        target_compile_options(${NAME} PRIVATE -std=c++17)
    endif(MSVC)
    
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")
    endif(WIN32)
    # copy shader files to build directory
    file(GLOB_RECURSE SHADERS
             "src/*.vs"
             "src/*.frag"
             "src/*.fs"
             "src/*.tcs"
             "src/*.tes"
             "src/*.gs"
             "src/*.cs"
             "src/*.vert"
             "src/*.png"
             "src/*.jpg"
             "src/*.jpeg"
             "src/*.mtl"
             "src/*.txt"
             "src/*.bin"
             "src/*.gltf"
             "src/**/*.vs"
             "src/**/*.frag"
             "src/**/*.fs"
             "src/**/*.tcs"
             "src/**/*.tes"
             "src/**/*.gs"
             "src/**/*.cs"
             "src/**/*.vert"
             "src/**/*.png"
             "src/**/*.jpg"
             "src/**/*.jpeg"
             "src/**/*.mtl"
             "src/**/*.txt"
             "src/**/*.bin"
             "src/**/*.gltf"
    )
	# copy dlls
	file(GLOB DLLS "dlls/*.dll")

    foreach(SHADER ${SHADERS})
      get_filename_component(SHADER_PATH "${SHADER}" PATH)
      get_filename_component(SHADER_NAME "${SHADER}" NAME)

      # Strip the 'src/' from the SHADER_PATH to get the relative path
      if("${SHADER_PATH}" STREQUAL "${CMAKE_SOURCE_DIR}/src")
        set(TARGET_SHADER_DIR "${CMAKE_SOURCE_DIR}/bin")
      else()
        string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" RELATIVE_SHADER_PATH "${SHADER_PATH}")
        set(TARGET_SHADER_DIR "${CMAKE_SOURCE_DIR}/bin/${RELATIVE_SHADER_PATH}")
      endif()
        

      if(WIN32)
        # Crear el directorio si no existe
        add_custom_command(TARGET ${NAME} PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_SHADER_DIR}")

        # Copiar el archivo al directorio correspondiente
        add_custom_command(TARGET ${NAME} PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${SHADER} "${TARGET_SHADER_DIR}/${SHADER_NAME}")
      elseif(UNIX AND NOT APPLE)
        # Similar a lo que se haría en Windows, pero para sistemas Unix
        file(MAKE_DIRECTORY "${TARGET_SHADER_DIR}")
        file(COPY ${SHADER} DESTINATION "${TARGET_SHADER_DIR}")
      elseif(APPLE)
        # Similar a lo que se haría en Windows, pero para macOS
        file(MAKE_DIRECTORY "${TARGET_SHADER_DIR}")
        makeLink(${SHADER} "${TARGET_SHADER_DIR}/${SHADER_NAME}" ${NAME})
      endif()
    endforeach()

    # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
endfunction()

# then create a project file per projects
create_project_from_sources()

include_directories(${CMAKE_SOURCE_DIR}/includes)
