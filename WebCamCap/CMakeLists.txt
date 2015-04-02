cmake_minimum_required(VERSION 2.8.11)
project(WebCamCap)

if (${CMAKE_VERSION} VERSION_GREATER 3.0.0)
  cmake_policy(SET CMP0043 OLD)
endif()


IF (UNIX)
        set(CMAKE_CXX_FLAGS "-m64 -pipe -pedantic -Wall -Wextra -O2 -Wall -W -D_REENTRANT ")
ENDIF (UNIX)


#c++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

ADD_DEFINITIONS(${QT_DEFINITIONS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HW_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Gui)
subdirs(Gui)

FIND_PATH(INC_GLM "glm/glm.hpp")
FIND_PATH(INC_GLUT "GL/glut.h")

IF (WIN32)
        FIND_LIBRARY(LIB_GLUT "freeglut.dll")
        SET(LIBS ${LIBS} ${LIB_GLUT})
ENDIF (WIN32)

include_directories(QT_INCLUDE_DIR ${HW_GUI_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${INC_GLM} ${INC_GLUT})

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5OpenGL)
FIND_PACKAGE(Qt5Network)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5Concurrent)
FIND_PACKAGE(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

file(GLOB SRC
    "*.h"
    "*.cpp"
    "*.tpp"
    "Gui/*.h"
    "Gui/*.cpp"
)

QT5_WRAP_UI(UIS
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/addcamera.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/addproject.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/animplayer.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/camwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/aboutwidget.ui
)

QT5_WRAP_CPP(CPPS

    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/addcamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/addproject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/animplayer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/camwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui/aboutwidget.h
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Pictures/EditIcon.png ${CMAKE_CURRENT_BINARY_DIR}/Pictures/EditIcon.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Pictures/SaveIcon.png ${CMAKE_CURRENT_BINARY_DIR}/Pictures/SaveIcon.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Pictures/PlayIcon.png ${CMAKE_CURRENT_BINARY_DIR}/Pictures/PlayIcon.png COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Pictures/main_icon.jpg ${CMAKE_CURRENT_BINARY_DIR}/Pictures/main_icon.jpg COPYONLY)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${SRC} ${UIS})

# Old Interfaces:
 set_property(TARGET ${PROJECT_NAME}
    APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG DEBUG_MODE )

 set_property(DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG DIR_DEBUG_MODE )


qt5_use_modules(${PROJECT_NAME} Widgets Core Gui OpenGL Concurrent Network)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
