cmake_minimum_required (VERSION 2.8)
project(rle)

option(USE_SDL "Use SDL" OFF)
option(BUILD_EXAMPLES "Build Example Agents" ON)
option(BUILD_PYTHON "Usef when building Python with distutils" OFF)
option(DEBUG "Suspend optimization & enable debugging" OFF)
option(UNIT_TESTING "Build unit tests using Google Test" OFF)
option(INSTALL_CORES "Install the core libs. Install may require sudo" OFF)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wunused -fPIC -fomit-frame-pointer -D__STDC_CONSTANT_MACROS -std=c++11")
add_definitions(-DHAVE_INTTYPES)
set(LINK_LIBS z)

if(USE_RLGLUE)
  add_definitions(-D__USE_RLGLUE)
  list(APPEND LINK_LIBS rlutils rlgluenetdev)
endif()

if(DEBUG)
  add_definitions(-D__DEBUG)
  list(APPEND CMAKE_CXX_FLASGS -g )
else()
  list(APPEND CMAKE_CXX_FLASGS -O3)
endif()

if(USE_SDL)
  add_definitions(-D__USE_SDL)
  add_definitions(-DSOUND_SUPPORT)
  find_package(SDL)
  if(SDL_FOUND AND ${SDL_VERSION_STRING} VERSION_LESS 2)
    include_directories(${SDL_INCLUDE_DIR})
    list(APPEND LINK_LIBS ${SDL_LIBRARY} ${SDL_MAIN_LIBRARY} SDL_gfx asound)
    message("sdl_library:" ${SDL_LIBRARY} ${SDL_MAIN_LIBRARY})
  else()
    # Uncomment below to specify the path to your SDL library. Run "locate libSDL" if unsure.
    # link_directories(path_to_your_SDL)
    if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Cocoa")
      list(APPEND LINK_LIBS sdl sdlmain)
    else()
      list(APPEND LINK_LIBS SDL)
    endif()
  endif()
endif()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SNES9X_DIR ${CMAKE_CURRENT_SOURCE_DIR}/snes9x2010)
set(STELLA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stella-libretro)
set(GENESIS_PLUS_GX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Genesis-Plus-GX)
set(PYTHON_RLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rle_python_interface)
set(MODULES common controllers environment games games/supported external external/TinyMT)

foreach(module ${MODULES})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c)
  list(APPEND SOURCES ${module_sources})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c?[xp])
  list(APPEND SOURCES ${module_sources})
endforeach(module ${MODULES})

# OS-dependent specifics
if(APPLE)
  include_directories(/System/Library/Frameworks/vecLib.framework/Versions/Current/Headers)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

if(WINDOWS OR MINGW)
  list(APPEND SOURCES ${SOURCE_DIR}/os_dependent/FSNodeWin32.cxx)
else()
  list(APPEND SOURCES ${SOURCE_DIR}/os_dependent/FSNodePOSIX.cxx)
endif()

if(UNIX)
    list(APPEND LINK_LIBS dl)
endif()


include_directories(
  ${SOURCE_DIR}
  ${SOURCE_DIR}/common
  ${SOURCE_DIR}/environment
  ${SOURCE_DIR}/games
  ${SOURCE_DIR}/games/supported
  ${SOURCE_DIR}/os_dependent
  ${SOURCE_DIR}/external
  ${SOURCE_DIR}/external/TinyMT
)

set(INCDIR "${CMAKE_INSTALL_PREFIX}/include/rle")
set(LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")

MESSAGE(STATUS "INCLUDE_DIR ${INCDIR}")
MESSAGE(STATUS "LIBRARY_DIR ${LIBDIR}")


IF(INSTALL_CORES)
	set(SNES9X_INSTALL_COMMAND 
		mv snes9x2010_libretro.so
		${LIBDIR}/snes9x2010_libretro.so)
	set(STELLA_INSTALL_COMMAND 
		mv stella_libretro.so
		${LIBDIR}/stella_libretro.so)
	set(GENESIS_INSTALL_COMMAND 
		mv genesis_plus_gx_libretro.so
		${LIBDIR}/genesis_plus_gx_libretro.so)
ELSE()
	set(SNES9X_INSTALL_COMMAND "")
	set(STELLA_INSTALL_COMMAND "")
	set(GENESIS_INSTALL_COMMAND "")
ENDIF()

include(ExternalProject)
ExternalProject_add(snes9x2010_libretro
			GIT_REPOSITORY https://github.com/libretro/snes9x2010.git
			UPDATE_COMMAND ""
			SOURCE_DIR ${SNES9X_DIR}
			BUILD_IN_SOURCE 1
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ${MAKE}
			INSTALL_DIR ${LIBDIR}
			INSTALL_COMMAND "${SNES9X_INSTALL_COMMAND}"
		   )
ExternalProject_add(stella_libretro
			GIT_REPOSITORY https://github.com/libretro/stella-libretro.git
			UPDATE_COMMAND ""
			SOURCE_DIR ${STELLA_DIR}
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ${MAKE}
			BUILD_IN_SOURCE 1
			INSTALL_DIR ${LIBDIR}
			INSTALL_COMMAND "${STELLA_INSTALL_COMMAND}"
		   )
ExternalProject_add(genesis_plus_gx_libretro
			GIT_REPOSITORY https://github.com/libretro/Genesis-Plus-GX
			UPDATE_COMMAND ""
			SOURCE_DIR ${GENESIS_PLUS_GX_DIR}
			BUILD_IN_SOURCE 1
			CONFIGURE_COMMAND ""
			BUILD_COMMAND make -f Makefile.libretro
			INSTALL_DIR ${LIBDIR}
			INSTALL_COMMAND "${GENESIS_INSTALL_COMMAND}"
		   )

IF(BUILD_PYTHON)
	INSTALL(DIRECTORY ${SNES9X_DIR} DESTINATION ${PYTHON_RLE_DIR}/snes9x2010_libretro.so
		FILES_MATCHING PATTERN "*.so")
	INSTALL(DIRECTORY ${GENESIS_PLUS_GX_DIR} DESTINATION ${PYTHON_RLE_DIR}/stella_libretro.so
		FILES_MATCHING PATTERN "*.so")
	INSTALL(DIRECTORY ${STELLA_DIR} DESTINATION ${PYTHON_RLE_DIR}/genesis_plus_gx_libretro.so
		FILES_MATCHING PATTERN "*.so")	
ENDIF()

add_library(rle-lib SHARED ${SOURCE_DIR}/rle_interface.cpp ${SOURCES})
set_target_properties(rle-lib PROPERTIES OUTPUT_NAME rle)
set_target_properties(rle-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_library(rle-c-lib SHARED ${PYTHON_RLE_DIR}/rle_c_wrapper.cpp ${SOURCE_DIR}/rle_interface.cpp ${SOURCES})
set_target_properties(rle-c-lib PROPERTIES OUTPUT_NAME rle_c)
set_target_properties(rle-c-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rle_python_interface)

target_link_libraries(rle-lib ${LINK_LIBS})
target_link_libraries(rle-c-lib ${LINK_LIBS})

IF(INSTALL_CORES)
	INSTALL(
		FILES
		src/rle_interface.hpp
		src/libretro.h
		DESTINATION "${INCDIR}"
	       )
	
	IF(APPLE)
	  INSTALL(FILES
	          librle.a
	          DESTINATION "${LIBDIR}")
	ELSE()
	    INSTALL(FILES
	            librle.so
	            DESTINATION "${LIBDIR}")
	ENDIF()
ENDIF()

if(BUILD_EXAMPLES)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR})
  add_executable(sharedLibraryInterfaceExample ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples/sharedLibraryInterfaceExample.cpp)
  set_target_properties(sharedLibraryInterfaceExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples)
  target_link_libraries(sharedLibraryInterfaceExample rle)
  target_link_libraries(sharedLibraryInterfaceExample ${LINK_LIBS})
  add_dependencies(sharedLibraryInterfaceExample rle-lib)
  
endif()

if(UNIT_TESTING)
#  find_package(GTest HINTS ./googletest)
#  if(NOT GTest_FOUND)
#    message("GTest not found")
#    # We need thread support
#    find_package(Threads REQUIRED)
#    # Enable ExternalProject CMake module
#    include(ExternalProject)
#    # Download and install GoogleTest
#    ExternalProject_Add(
#	    gtest
#	    GIT_REPOSITORY https://github.com/google/googletest.git
#	    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
#	    # Disable install step
#	    INSTALL_COMMAND ""
#    )
#    # Create a libgtest target to be used as a dependency by test programs
#    add_library(libgtest IMPORTED STATIC GLOBAL)
#    add_dependencies(libgtest gtest)

#    # Set gtest properties
#    ExternalProject_Get_Property(gtest source_dir binary_dir)
#    set_target_properties(libgtest PROPERTIES
#                            "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
#                            "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    )

#  endif()
    add_definitions(-D__UNIT_TESTING)
    add_subdirectory(googletest)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS} ${PYTHON_RLE_DIR})

    file(GLOB test_sources test/*.cpp)
    list(APPEND TEST_SOURCES ${test_sources})

    # Add test cpp file
    add_executable( mainTest ${TEST_SOURCES})

    # Link test executable against gtest & gtest_main
    target_link_libraries(mainTest gtest gtest_main rle rle-c-lib)
    add_test( mainTest mainTest )
endif()
