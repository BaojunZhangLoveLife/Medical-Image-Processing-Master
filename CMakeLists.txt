#set cmake version number
cmake_minimum_required(VERSION 3.4.0)

#set project name
project(Medical-Image-Processing-Master)
SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
# Enable C++17
set(CMAKE_CXX_STANDARD 17)

# Open3D
find_package(Open3D)
include_directories(${Open3D_INCLUDE_DIRS})
link_directories(${Open3D_LIBRARY_DIRS})
add_definitions(${Open3D_DEFINITIONS})


# VMTK
if(NOT VMTK_ROOT)
	set(VMTK_ROOT "" CACHE PATH "VMTK root path")
endif()
if(VMTK_ROOT)
	set(VMTK_LIB_DIR ${VMTK_ROOT}/lib/)
	set(VMTK_BIN_DIR ${VMTK_ROOT}/bin/)
	file(GLOB VMTK_LIB_FILES ${VMTK_ROOT}/lib/*.lib)
	file(GLOB VMTK_BIN_FILES ${VMTK_ROOT}/bin/*.dll)
	set(VMTK_INC_DIR ${VMTK_ROOT}/include/)
else()
	message(FATAL_ERROR "can not generate without VMTK")
endif(VMTK_ROOT)
include_directories(${VMTK_INC_DIR})

# ITK
find_package(ITK)
if(ITK_FOUND)
	include(${ITK_USE_FILE})
else(ITK_FOUND)
	message(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
endif(ITK_FOUND)
set(ITK_LIBS ${ITK_LIBRARIES})

# CUDA
if(NOT CUDA_ROOT)
        set(CUDA_ROOT "" CACHE PATH "cuda root path")
endif()
if(CUDA_ROOT)
	set(CUDA_LIB_DIR ${CUDA_ROOT}/lib/)
    set(CUDA_BIN_DIR ${CUDA_ROOT}/bin/)
    file(GLOB CUDA_LIB_FILES ${CUDA_ROOT}/lib/*.lib)
    file(GLOB CUDA_BIN_FILES ${CUDA_ROOT}/bin/*.dll)
    set(CUDA_INC_DIR ${CUDA_ROOT}/include/)
else()
    message(FATAL_ERROR "can not generate without cuda")
endif(CUDA_ROOT)
include_directories(${CUDA_INC_DIR})

# pcl
find_package(PCL 1.11 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

set(CMAKE_AUTOMOC ON) 
# qt
find_package(Qt5Widgets)  
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5LinguistTools)
find_package(Qt5OpenGL)
find_package(Qt5OpenGLExtensions)
file(GLOB SRC_FILES "./*.cpp" "./*.c") 
file(GLOB HEAD_FILES "./*.h" "./*.hpp" "./*.hxx") 
file(GLOB UI_FILES "./UI/*.ui") 
file(GLOB SHADER_FILES "./Shader/*.frag" "./Shader/*.vert") 

qt5_wrap_ui(WRAP_FILES ${UI_FILES})
qt5_add_resources(QRC_FILES ${RCC_FILES})

source_group("Ui" FILES ${UI_FILES})
source_group("Source" FILES ${SRC_FILES})
source_group("Header" FILES ${HEAD_FILES})
source_group("Shader" FILES ${SHADER_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${QRC_FILES} ${WRAP_FILES} ${SHADER_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::OpenGLExtensions)
target_link_libraries(${PROJECT_NAME} ${Open3D_LIBRARIES})
target_link_libraries (${PROJECT_NAME} ${PCL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${VMTK_LIBRARIES})
target_link_libraries (${PROJECT_NAME} ${CUDA_LIB_FILES})

# generate libraies
#add_library(DataProcessing DataProcessing.cpp)
#target_link_libraries(DataProcessing Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL)
