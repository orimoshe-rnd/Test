cmake_minimum_required(VERSION 2.8.11)
project(osgTool)

if (WIN32)
    set(Qt5_DIR "C:\\Qt\\Qt5.12.10\\5.12.10\\msvc2017_64\\lib\\cmake\\Qt5")
	set(OSG_DIR "C:/Program Files/OpenSceneGraph")
endif (WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL)
FIND_PACKAGE(osgEarth)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer)

if (UNIX)
    FIND_PACKAGE(osgEarth)
endif (UNIX)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set (CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES})

qt5_use_modules(${PROJECT_NAME} Widgets)
qt5_use_modules(${PROJECT_NAME} Widgets)


target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::OpenGL
    
    )

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/Program Files/OpenSceneGraph/bin/" $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if (WIN32)
    file(GLOB Qt5_dlls "C:\\Qt\\Qt5.12.10\\5.12.10\\msvc2017_64\\bin/*.dll")
	foreach(Qt5_dll ${Qt5_dlls})
		add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E
				copy ${Qt5_dll}  $<TARGET_FILE_DIR:${PROJECT_NAME}>)
	endforeach()
endif (WIN32)

