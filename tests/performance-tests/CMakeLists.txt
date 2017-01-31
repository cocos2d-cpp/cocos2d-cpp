set(APP_NAME performance-tests)

# Use same method as in cocos library
if(LINUX)
  foreach(_pkg FONTCONFIG GTK3)
    cocos_find_package(${_pkg} ${_pkg} REQUIRED)
  endforeach()
endif()

if(WIN32)
  set(PLATFORM_SRC proj.win32/main.cpp)
  set(RES_PREFIX "")
elseif(MACOSX)
  set(PLATFORM_SRC proj.mac/main.cpp)

  file(GLOB_RECURSE RES_FILES Resources/*)
  cocos_mark_resources(FILES ${RES_FILES} BASEDIR Resources)
  list(APPEND PLATFORM_SRC ${RES_FILES})

elseif(LINUX)
  set(PLATFORM_SRC proj.linux/main.cpp)
  set(RES_PREFIX "/Resources")
elseif(ANDROID)
  set(PLATFORM_SRC
          Classes/JNITest/JNITest.cpp
          proj.android/jni/main.cpp)
  set(RES_PREFIX "/Resources")
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()

set(TESTS_SRC
  Classes/Profile.cpp
  Classes/AppDelegate.cpp
  Classes/tests/PerformanceCallbackTest.cpp
  Classes/tests/PerformanceScenarioTest.cpp
  Classes/tests/PerformanceMathTest.cpp
  Classes/tests/PerformanceEventDispatcherTest.cpp
  Classes/tests/PerformanceAllocTest.cpp
  Classes/tests/PerformanceParticle3DTest.cpp
  Classes/tests/PerformanceNodeChildrenTest.cpp
  Classes/tests/VisibleRect.cpp
  Classes/tests/PerformanceSpriteTest.cpp
  Classes/tests/BaseTest.cpp
  Classes/tests/PerformanceParticleTest.cpp
  Classes/tests/controller.cpp
  Classes/tests/PerformanceLabelTest.cpp
  Classes/tests/PerformanceTextureTest.cpp
  ${PLATFORM_SRC}
)

include_directories(
  Classes
  Classes/tests
)

if(ANDROID)
  add_library(${APP_NAME} SHARED ${TESTS_SRC})
  if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} lib${APP_NAME}.so)
  endif()
else()
  # add the executable
  add_executable(${APP_NAME} ${TESTS_SRC})
endif()

target_link_libraries(${APP_NAME} cocos2d)

if(LINUX)
  foreach(_pkg FONTCONFIG GTK3)
    cocos_use_pkg(${APP_NAME} ${_pkg})
  endforeach()
endif()

if(MACOSX OR APPLE)
  set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE 1
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
elseif(ANDROID)
  set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
  set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")
else()
  set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
  set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")
    
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}${RES_PREFIX}
  )
endif()
