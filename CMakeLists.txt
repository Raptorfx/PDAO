
cmake_minimum_required (VERSION 2.6)
project (DarkAgesOnline)

set (DarkAgesOnline_VERSION_MAJOR 0)
set (DarkAgesOnline_VERSION_MINOR 0)
set (DarkAgesOnline_VERSION_PATCH 0)

# determine system endianess
include(TestBigEndian)
test_big_endian(MYGUI_TEST_BIG_ENDIAN)


include_directories("${PROJECT_BINARY_DIR}")

set(DAO_SOURCE
    src/client/CameraControlSystem.h
    src/client/CameraControlSystem.cpp
    src/client/Character.h
    src/client/Character.cpp
  	src/client/DAO_Application.h
    src/client/DAO_Application.cpp
  	src/client/DAO_FrameListener.h
    src/client/DAO_FrameListener.cpp
  	src/client/DAO_ResourceManager.h
    src/client/DAO_ResourceManager.cpp
  	src/client/GameContext.h
    src/client/GameContext.cpp
    src/client/GlobalValueStore.h
    src/client/GlobalValueStore.cpp
  	src/client/InputManager.h
    src/client/InputManager.cpp
  	src/client/LogManager.h
    src/client/LogManager.cpp
    src/client/main.cpp
  	src/client/PlayerCharacter.h
    src/client/PlayerCharacter.cpp
  	src/client/RenderManager.h
    src/client/RenderManager.cpp
  	src/client/Resource.h
    src/client/Resource.cpp
  	src/client/ResourceGroup.h
    src/client/ResourceGroup.cpp

    src/client/Functions/md5.h
    src/client/Functions/md5.cpp

  	#Terrain
  	src/client/Terrain/TerrainManager.h
  	src/client/Terrain/TerrainManager.cpp
  	src/client/Terrain/TerrainPageProvider.h
  	src/client/Terrain/TerrainPageProvider.cpp
  	src/client/Terrain/TerrainDefiner.h
    src/client/Terrain/TerrainDefiner.cpp

  	#GUI
  	src/client/GUI/GUIMain.h
    src/client/GUI/GUIMain.cpp
)

set(HEIGHTMAP_GENERATOR_SOURCE
    src/utils/heightmap_generator/main.cpp
    src/utils/heightmap_generator/noiseutils.cpp
)

set(LIBNOISE_SOURCE
    src/utils/heightmap_generator/noise/latlon.cpp
    src/utils/heightmap_generator/noise/noisegen.cpp
    src/utils/heightmap_generator/noise/model/cylinder.cpp
    src/utils/heightmap_generator/noise/model/line.cpp
    src/utils/heightmap_generator/noise/model/plane.cpp
    src/utils/heightmap_generator/noise/model/sphere.cpp
    src/utils/heightmap_generator/noise/module/abs.cpp
    src/utils/heightmap_generator/noise/module/add.cpp
    src/utils/heightmap_generator/noise/module/billow.cpp
    src/utils/heightmap_generator/noise/module/blend.cpp
    src/utils/heightmap_generator/noise/module/cache.cpp
    src/utils/heightmap_generator/noise/module/checkerboard.cpp
    src/utils/heightmap_generator/noise/module/clamp.cpp
    src/utils/heightmap_generator/noise/module/const.cpp
    src/utils/heightmap_generator/noise/module/curve.cpp
    src/utils/heightmap_generator/noise/module/cylinders.cpp
    src/utils/heightmap_generator/noise/module/displace.cpp
    src/utils/heightmap_generator/noise/module/exponent.cpp
    src/utils/heightmap_generator/noise/module/invert.cpp
    src/utils/heightmap_generator/noise/module/max.cpp
    src/utils/heightmap_generator/noise/module/min.cpp
    src/utils/heightmap_generator/noise/module/modulebase.cpp
    src/utils/heightmap_generator/noise/module/multiply.cpp
    src/utils/heightmap_generator/noise/module/perlin.cpp
    src/utils/heightmap_generator/noise/module/power.cpp
    src/utils/heightmap_generator/noise/module/ridgedmulti.cpp
    src/utils/heightmap_generator/noise/module/rotatepoint.cpp
    src/utils/heightmap_generator/noise/module/scalebias.cpp
    src/utils/heightmap_generator/noise/module/scalepoint.cpp
    src/utils/heightmap_generator/noise/module/select.cpp
    src/utils/heightmap_generator/noise/module/spheres.cpp
    src/utils/heightmap_generator/noise/module/terrace.cpp
    src/utils/heightmap_generator/noise/module/translatepoint.cpp
    src/utils/heightmap_generator/noise/module/turbulence.cpp
    src/utils/heightmap_generator/noise/module/voronoi.cpp
)

IF(MSVC)

    #BOOST

    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    find_package(Boost 1.55.0 COMPONENTS thread system date_time chrono)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        set(BOOST_LIBS ${Boost_LIBRARIES})
        add_definitions( -DBOOST_ALL_NO_LIB )
    endif()

    # OGRE SDK

    SET(OGRE_DIR "" CACHE PATH "")
    include_directories(${OGRE_DIR}/include)
    include_directories(${OGRE_DIR}/include/OGRE)
    include_directories(${OGRE_DIR}/include/OGRE/Paging)
    include_directories(${OGRE_DIR}/include/OIS)

    link_directories(${OGRE_DIR}/lib)
    link_directories(${OGRE_DIR}/lib/$(Configuration)/opt)

    set(LIB_OGREMAIN debug OgreMain_d.lib optimized OgreMain.lib)
    set(LIB_OGREOVERLAY debug OgreOverlay_d.lib optimized OgreOverlay.lib)
    set(LIB_OGREPAGING debug OgrePaging_d.lib optimized OgrePaging.lib)
    set(LIB_OGREPROPERTY debug OgreProperty_d.lib optimized OgreProperty.lib)
    set(LIB_OGRERTSHADERSYSTEM debug OgreRTShaderSystem_d.lib optimized OgreRTShaderSystem.lib)
    set(LIB_OGRETERRAIN debug OgreTerrain_d.lib optimized OgreTerrain.lib)
    set(LIB_OGREVOLUME debug OgreVolume_d.lib optimized OgreVolume.lib)
    set(LIB_PLUGIN_BSPSCENEMANAGER debug Plugin_BSPSceneManager_d.lib optimized Plugin_BSPSceneManager.lib)
    set(LIB_PLUGIN_CGPROGRAMMANAGER debug Plugin_CgProgramManager_d.lib optimized Plugin_CgProgramManager.lib)
    set(LIB_PLUGIN_OCTREESCENEMANAGER debug Plugin_OctreeSceneManager_d.lib optimized Plugin_OctreeSceneManager.lib)
    set(LIB_PLUGIN_OCTREEZONE debug Plugin_OctreeZone_d.lib optimized Plugin_OctreeZone.lib)
    set(LIB_PLUGIN_PARTICLEFX debug Plugin_ParticleFX_d.lib optimized Plugin_ParticleFX.lib)
    set(LIB_PLUGIN_PCZSCENEMANAGER debug Plugin_PCZSceneManager_d.lib optimized Plugin_PCZSceneManager.lib)
    set(LIB_RENDERSYSTEM_DIRECT3D9 debug RenderSystem_Direct3D9_d.lib optimized RenderSystem_Direct3D9.lib)
    set(LIB_OIS debug OIS_d.lib optimized OIS.lib)

    # OGRE DEPENDENCIES

    SET(OGRE_DEPENDENCY_DIR "" CACHE PATH "")
    include_directories(${OGRE_DEPENDENCY_DIR}/include)
    link_directories(${OGRE_DEPENDENCY_DIR}/lib/Debug)
    link_directories(${OGRE_DEPENDENCY_DIR}/lib/Release)

    set(LIB_FREETYPE250 debug freetyped.lib optimized freetype.lib)

    # OGRE PLUGINS

    SET(OGRE_PLUGIN_DIR "" CACHE PATH "")
  	include_directories(${OGRE_PLUGIN_DIR}/include)
  	link_directories(${OGRE_PLUGIN_DIR}/lib)
  	set(LIB_MYGUIENGINE debug MyGUIEngine_d.lib optimized MyGUIEngine.lib)
  	set(LIB_MYGUIOGREPLATFORM debug MyGUI.OgrePlatform_d.lib optimized MyGUI.OgrePlatform.lib)

  	# PROTOBUF

  	SET(PROTOBUF_DIR "" CACHE PATH "")
  	include_directories(${PROTOBUF_DIR}/include)
  	link_directories(${PROTOBUF_DIR}/lib/Debug)
  	link_directories(${PROTOBUF_DIR}/lib/Release)
  	set(LIB_PROTOBUF debug libprotobuf_d.lib optimized libprotobuf.lib)
  	set(LIB_PROTOC debug libprotoc_d.lib optimized libprotoc.lib)

  	# VISUAL SETTINGS
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

ENDIF(MSVC)

set(DAO_LIBS
    ${BOOST_LIBS}
    ${LIB_OGREMAIN}
    ${LIB_OGREOVERLAY}
    ${LIB_OGREPAGING}
    ${LIB_OGREPROPERTY}
    ${LIB_OGRERTSHADERSYSTEM}
    ${LIB_OGRETERRAIN}
    ${LIB_OGREVOLUME}
    ${LIB_PLUGIN_BSPSCENEMANAGER}
    ${LIB_PLUGIN_CGPROGRAMMANAGER}
    ${LIB_PLUGIN_OCTREESCENEMANAGER}
    ${LIB_PLUGIN_OCTREEZONE}
    ${LIB_PLUGIN_PARTICLEFX}
    ${LIB_PLUGIN_PCZSCENEMANAGER}
    ${LIB_RENDERSYSTEM_DIRECT3D9}
    ${LIB_OIS}
    ${LIB_FREETYPE250}
    ${LIB_MYGUIENGINE}
    ${LIB_MYGUIOGREPLATFORM}
    ${LIB_PROTOBUF}
    ${LIB_PROTOC}
)

add_executable(DarkAgesOnline ${DAO_SOURCE})
target_link_libraries(DarkAgesOnline ${DAO_LIBS})

add_executable(Heightmap_Generator ${HEIGHTMAP_GENERATOR_SOURCE} ${LIBNOISE_SOURCE})
target_link_libraries(Heightmap_Generator ${HEIGHTMAP_GENERATOR_LIBS})

# This prints out ALL variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
