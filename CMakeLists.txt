
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

include_directories(${DEPENDENCY_INCLUDE_DIR})
link_directories(${DEPENDENCY_INCLUDE_DIR})

#Libs
SET(DEPENDENCY_DIR "" CACHE PATH "")
SET(BOOST_INCLUDE_DIR "" CACHE PATH "")
SET(BOOST_LIB_DIR "" CACHE PATH "")

include_directories(${BOOST_INCLUDE_DIR})
include_directories(${BOOST_LIB_DIR})

include_directories(${DEPENDENCY_DIR}/include)
include_directories(${DEPENDENCY_DIR}/include/OGRE)
include_directories(${DEPENDENCY_DIR}/include/OGRE/Paging)

include_directories(${DEPENDENCY_DIR}/lib)
include_directories(${DEPENDENCY_DIR}/lib/Release)
include_directories(${DEPENDENCY_DIR}/lib/relwithdebinfo)
include_directories(${DEPENDENCY_DIR}/lib/relwithdebinfo/opt)

set(LIBS boost_system boost_thread)
set(LIBS ${LIBS} cg.lib FreeImage freetype OIS zlib zziplib)
set(LIBS ${LIBS} MyGUI.Ogreplatform.a MyGUIEngineStatic)
set(LIBS ${LIBS} OgreMainStatic OgreOverlayStatic OgrePagingStatic OgrePropertyStatic OgreRTShaderSystemStatic OgreTerreinStatic OgreVolumeStatic)
set(LIBS ${LIBS} Plugin_StrangeButtonStatic Plugin_BSPSceneManagerStatic Plugin_CgProgramManagerStatic Plugin_OctreeZoneStatic)
set(LIBS ${LIBS} Plugin_ParticleFXStatic Plugin_PCZSceneManagerStatic RenderSystem_GLStatic)

add_executable(DarkAgesOnline ${SOURCE})

target_link_libraries(DarkAgesOnline ${LIBS}])
