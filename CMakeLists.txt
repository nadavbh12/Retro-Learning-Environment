cmake_minimum_required (VERSION 3.02)
project(ale)

option(USE_SDL "Use SDL" OFF)
option(USE_RLGLUE "Use RL-Glue" OFF)
option(BUILD_EXAMPLES "Build Example Agents" ON)
option(DEBUG "Suspend optimization & enable debugging" OFF)
option(UNIT_TESTING "Build unit tests using Google Test" OFF)


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
    list(APPEND LINK_LIBS ${SDL_LIBRARY} ${SDL_MAIN_LIBRARY} SDL_gfx)
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
set(SNES9X_DIR ${CMAKE_CURRENT_SOURCE_DIR}/snes9x-next)
set(STELLA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stella-libretro)
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
  list(APPEND SOURCES ${SOURCE_DIR}/os_dependent/SettingsWin32.cxx ${SOURCE_DIR}/os_dependent/OSystemWin32.cxx ${SOURCE_DIR}/os_dependent/FSNodeWin32.cxx)
else()
  list(APPEND SOURCES ${SOURCE_DIR}/os_dependent/SettingsUNIX.cxx ${SOURCE_DIR}/os_dependent/AleSystemUNIX.cxx ${SOURCE_DIR}/os_dependent/FSNodePOSIX.cxx)
endif()

if(UNIX)
    list(APPEND LINK_LIBS dl)
endif()


include_directories(
  ${SOURCE_DIR}
  ${SOURCE_DIR}/common
  ${SOURCE_DIR}/controllers
  ${SOURCE_DIR}/environment
  ${SOURCE_DIR}/games
  ${SOURCE_DIR}/games/supported
  ${SOURCE_DIR}/os_dependent
  ${SOURCE_DIR}/external
  ${SOURCE_DIR}/external/TinyMT
)

include(ExternalProject)

ExternalProject_add(snes9x2010_libretro
			GIT_REPOSITORY https://github.com/libretro/snes9x2010.git
			SOURCE_DIR ${SNES9X_DIR}
			BINARY_DIR ${SNES9X_DIR}
			CONFIGURE_COMMAND ""
			BUILD_COMMAND make
			INSTALL_COMMAND ""
		   )
ExternalProject_add(stella_libretro
			GIT_REPOSITORY https://github.com/libretro/stella-libretro.git
			SOURCE_DIR ${STELLA_DIR}
			BINARY_DIR ${STELLA_DIR}
			CONFIGURE_COMMAND ""
			BUILD_COMMAND "make"
			INSTALL_COMMAND ""
		   )

add_library(ale-lib SHARED ${SOURCE_DIR}/ale_interface.cpp ${SOURCES})
set_target_properties(ale-lib PROPERTIES OUTPUT_NAME ale)
set_target_properties(ale-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_dependencies(ale-lib snes9x2010_libretro)
add_dependencies(ale-lib stella_libretro)
add_executable(ale-bin ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/ale_interface.cpp ${SOURCES})
set_target_properties(ale-bin PROPERTIES OUTPUT_NAME ale)
set_target_properties(ale-bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_library(ale-c-lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/ale_python_interface/ale_c_wrapper.cpp ${SOURCE_DIR}/ale_interface.cpp ${SOURCES})
set_target_properties(ale-c-lib PROPERTIES OUTPUT_NAME ale_c)
set_target_properties(ale-c-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ale_python_interface)

target_link_libraries(ale-lib ${LINK_LIBS})
target_link_libraries(ale-bin ${LINK_LIBS})
target_link_libraries(ale-c-lib ${LINK_LIBS})

if(BUILD_EXAMPLES)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR})
  add_executable(sharedLibraryInterfaceExample ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples/sharedLibraryInterfaceExample.cpp)
  set_target_properties(sharedLibraryInterfaceExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples)
  target_link_libraries(sharedLibraryInterfaceExample ale)
  target_link_libraries(sharedLibraryInterfaceExample ${LINK_LIBS})
  add_dependencies(sharedLibraryInterfaceExample ale-lib)
  
  if (USE_SDL)
    add_executable(videoRecordingExample ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples/videoRecordingExample.cpp)
    set_target_properties(videoRecordingExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples)
    target_link_libraries(videoRecordingExample ale)
    target_link_libraries(videoRecordingExample ${LINK_LIBS})
    add_dependencies(videoRecordingExample ale-lib)
  endif()
endif()

if(USE_RLGLUE)
  add_executable(RLGlueAgent ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples/RLGlueAgent.c)
  set_target_properties(RLGlueAgent PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples)
  target_link_libraries(RLGlueAgent rlutils)
  target_link_libraries(RLGlueAgent rlagent)
  target_link_libraries(RLGlueAgent rlgluenetdev)
  add_executable(RLGlueExperiment ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples/RLGlueExperiment.c)
  set_target_properties(RLGlueExperiment PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/examples)
  target_link_libraries(RLGlueExperiment rlutils)
  target_link_libraries(RLGlueExperiment rlexperiment)
  target_link_libraries(RLGlueExperiment rlgluenetdev)
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
    add_subdirectory(googletest)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/ale_python_interface)

    file(GLOB test_sources test/*.cpp)
    list(APPEND TEST_SOURCES ${test_sources})

    # Add test cpp file
    add_executable( mainTest ${TEST_SOURCES})

    # Link test executable against gtest & gtest_main
    target_link_libraries(mainTest gtest gtest_main ale ale-c-lib)
    add_test( mainTest mainTest )
endif()
