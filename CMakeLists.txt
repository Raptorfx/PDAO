
cmake_minimum_required (VERSION 2.6)
project (DarkAgesOnline)

set (DarkAgesOnline_VERSION_MAJOR 0)
set (DarkAgesOnline_VERSION_MINOR 0)
set (DarkAgesOnline_VERSION_PATCH 0)

# determine system endianess
include(TestBigEndian)
test_big_endian(MYGUI_TEST_BIG_ENDIAN)


include_directories("${PROJECT_BINARY_DIR}")

set(SOURCE
    src/CameraControlSystem.cpp
    src/Character.cpp
    src/CollisionTools.cpp
    src/DAO_Application.cpp
    src/DAO_FrameListener.cpp
    src/DAO_ResourceManager.cpp
    src/GameContext.cpp
    src/GlobalValueStore.cpp
    src/InputManager.cpp
    src/LogManager.cpp
    src/main.cpp
    src/PlayerCharacter.cpp
    src/RenderManager.cpp
    src/Resource.cpp
    src/ResourceGroup.cpp
    src/TerrainManager.cpp
    src/Functions/md5.cpp
)





IF(MINGW)

    #Libs
    SET(DEPENDENCY_DIR "" CACHE PATH "")
    SET(BOOST_INCLUDE_DIR "" CACHE PATH "")
    SET(BOOST_LIB_DIR "" CACHE PATH "")
    
    include_directories(${DEPENDENCY_INCLUDE_DIR})
    include_directories(${BOOST_INCLUDE_DIR})
    
    include_directories(${BOOST_LIB_DIR})
    link_directories(${BOOST_LIB_DIR})
    
    include_directories(${DEPENDENCY_DIR}/include)
    include_directories(${DEPENDENCY_DIR}/include/OGRE)
    include_directories(${DEPENDENCY_DIR}/include/OGRE/Paging)
    
    link_directories(${DEPENDENCY_DIR}/lib)
    link_directories(${DEPENDENCY_DIR}/lib/Release)
    link_directories(${DEPENDENCY_DIR}/lib/relwithdebinfo)
    link_directories(${DEPENDENCY_DIR}/lib/relwithdebinfo/opt)
    
    message(STATUS "DEPENDENCY_DIR = ${DEPENDENCY_DIR}")
    message(STATUS "Adding linker directory = ${DEPENDENCY_DIR}/lib")
    message(STATUS "Adding linker directory = ${DEPENDENCY_DIR}/lib/Release")
    message(STATUS "Adding linker directory = ${DEPENDENCY_DIR}/lib/relwithdebinfo")
    message(STATUS "Adding linker directory = ${DEPENDENCY_DIR}/lib/relwithdebinfo/opt")

    set(LIBS boost_system)
    set(LIBS ${LIBS} boost_thread)
    set(LIBS ${LIBS} pthreadGC2)
    set(LIBS ${LIBS} cg.lib)
    set(LIBS ${LIBS} libFreeImage.a)
    set(LIBS ${LIBS} libfreetype.a)
    set(LIBS ${LIBS} libOIS.dll.a)
    set(LIBS ${LIBS} libzlib.a)
    set(LIBS ${LIBS} libzziplib.a)
    set(LIBS ${LIBS} libMyGUI.OgrePlatform.a)
    set(LIBS ${LIBS} MyGUIEngineStatic.a)
    set(LIBS ${LIBS} libOgreMainStatic.a)
    set(LIBS ${LIBS} libOgreOverlayStatic.a)
    set(LIBS ${LIBS} libOgrePagingStatic.a)
    set(LIBS ${LIBS} libOgrePropertyStatic.a)
    set(LIBS ${LIBS} libOgreRTShaderSystemStatic.a)
    set(LIBS ${LIBS} libOgreTerrainStatic.a)
    set(LIBS ${LIBS} libOgreVolumeStatic.a)
    set(LIBS ${LIBS} libPlugin_StrangeButtonStatic.a)
    set(LIBS ${LIBS} libPlugin_BSPSceneManagerStatic.a)
    set(LIBS ${LIBS} libPlugin_CgProgramManagerStatic.a)
    set(LIBS ${LIBS} libPlugin_OctreeSceneManagerStatic.a)
    set(LIBS ${LIBS} libPlugin_OctreeZoneStatic.a)
    set(LIBS ${LIBS} libPlugin_ParticleFXStatic.a)
    set(LIBS ${LIBS} libPlugin_PCZSceneManagerStatic.a)
    set(LIBS ${LIBS} RenderSystemGLStatic)
ENDIF(MINGW)

IF(MSVC)
    
    #BOOST
    
    set(Boost_USE_STATIC_LIBS ON) 
    set(Boost_USE_MULTITHREADED ON)  
    set(Boost_USE_STATIC_RUNTIME ON) 
    find_package(Boost 1.55.0 COMPONENTS thread system) 
    
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS}) 
        set(LIBS ${LIBS} ${Boost_LIBRARIES})   
        add_definitions( -DBOOST_ALL_NO_LIB )                                                                                                                                                                                                                       
    endif()
    
    
    SET(OGRE_DIR "" CACHE PATH "")
    include_directories(${OGRE_DIR}/include)
    include_directories(${OGRE_DIR}/include/OGRE)
    include_directories(${OGRE_DIR}/include/OGRE/Paging)
    include_directories(${OGRE_DIR}/include/OIS)
    
    link_directories(${OGRE_DIR}/lib/RelWithDebInfo)
    link_directories(${OGRE_DIR}/lib/RelWithDebInfo/opt)
    
    set(LIBS ${LIBS} OgreMain.lib)
    set(LIBS ${LIBS} OgreOverlay.lib)
    set(LIBS ${LIBS} OgrePaging.lib)
    set(LIBS ${LIBS} OgreProperty.lib)
    set(LIBS ${LIBS} OgreRTShaderSystem.lib)
    set(LIBS ${LIBS} OgreTerrain.lib)
    set(LIBS ${LIBS} OgreVolume.lib)
    set(LIBS ${LIBS} Plugin_BSPSceneManager.lib)
    set(LIBS ${LIBS} Plugin_CgProgramManager.lib)
    set(LIBS ${LIBS} Plugin_OctreeSceneManager.lib)
    set(LIBS ${LIBS} Plugin_OctreeZone.lib)
    set(LIBS ${LIBS} Plugin_ParticleFX.lib)
    set(LIBS ${LIBS} Plugin_PCZSceneManager.lib)
    set(LIBS ${LIBS} RenderSystem_GL.lib)
    
    SET(OGRE_PLUGIN_DIR "" CACHE PATH "")

ENDIF(MSVC)

add_executable(DarkAgesOnline ${SOURCE})

target_link_libraries(DarkAgesOnline ${LIBS})

# This prints out ALL variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
