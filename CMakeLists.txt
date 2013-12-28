
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
    src/client/CameraControlSystem.cpp
    src/client/Character.cpp
    src/client/DAO_Application.cpp
    src/client/DAO_FrameListener.cpp
    src/client/DAO_ResourceManager.cpp
    src/client/GameContext.cpp
    src/client/GlobalValueStore.cpp
    src/client/InputManager.cpp
    src/client/LogManager.cpp
    src/client/main.cpp
    src/client/PlayerCharacter.cpp
    src/client/RenderManager.cpp
    src/client/Resource.cpp
    src/client/ResourceGroup.cpp
    
    src/client/Functions/md5.cpp

  	#Terrain
  	src/client/Terrain/TerrainManager.cpp
  	src/client/Terrain/TerrainPageProvider.cpp
  	src/client/Terrain/TerrainDefiner.cpp
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

    set(MSVC_RUNTIME "static")
    foreach(variable ${variables})
      if(${variable} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${variable} "${${variable}}")
      endif()
      if(${variable} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
      endif()
    endforeach()
    
    #BOOST
    
    set(Boost_USE_STATIC_LIBS ON) 
    set(Boost_USE_MULTITHREADED ON)  
    set(Boost_USE_STATIC_RUNTIME ON) 
    find_package(Boost 1.55.0 COMPONENTS thread system) 
    
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS}) 
        set(DAO_LIBS ${DAO_LIBS} ${Boost_LIBRARIES})   
        add_definitions( -DBOOST_ALL_NO_LIB )                                                                                                                                                                                                                       
    endif()
    
    
    SET(OGRE_DIR "" CACHE PATH "")
    include_directories(${OGRE_DIR}/include)
    include_directories(${OGRE_DIR}/include/OGRE)
    include_directories(${OGRE_DIR}/include/OGRE/Paging)
    include_directories(${OGRE_DIR}/include/OIS)
    
    link_directories(${OGRE_DIR}/lib/RelWithDebInfo)
    link_directories(${OGRE_DIR}/lib/RelWithDebInfo/opt)
    
    set(DAO_LIBS ${DAO_LIBS} OgreMain.lib)
    set(DAO_LIBS ${DAO_LIBS} OgreOverlay.lib)
    set(DAO_LIBS ${DAO_LIBS} OgrePaging.lib)
    set(DAO_LIBS ${DAO_LIBS} OgreProperty.lib)
    set(DAO_LIBS ${DAO_LIBS} OgreRTShaderSystem.lib)
    set(DAO_LIBS ${DAO_LIBS} OgreTerrain.lib)
    set(DAO_LIBS ${DAO_LIBS} OgreVolume.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_BSPSceneManager.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_CgProgramManager.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_OctreeSceneManager.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_OctreeZone.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_ParticleFX.lib)
    set(DAO_LIBS ${DAO_LIBS} Plugin_PCZSceneManager.lib)
    set(DAO_LIBS ${DAO_LIBS} RenderSystem_Direct3D9.lib)
    
    SET(OGRE_DEPENDENCY_DIR "" CACHE PATH "")
    include_directories(${OGRE_DEPENDENCY_DIR}/include)
    link_directories(${OGRE_DEPENDENCY_DIR}/lib/Release)
    
    SET(LIBNOISE_DIR "" CACHE PATH "")
    include_directories(${LIBNOISE_DIR}/include)
    link_directories(${LIBNOISE_DIR}/bin)
    
    set(HEIGHTMAP_GENERATOR_LIBS ${HEIGHTMAP_GENERATOR_LIBS} libnoise.lib)
    
    set(DAO_LIBS ${DAO_LIBS} OIS.lib)
    
    SET(OGRE_PLUGIN_DIR "" CACHE PATH "")

ENDIF(MSVC)

add_executable(DarkAgesOnline ${DAO_SOURCE})
target_link_libraries(DarkAgesOnline ${DAO_LIBS})

add_executable(Heightmap_Generator ${HEIGHTMAP_GENERATOR_SOURCE} ${LIBNOISE_SOURCE})       
target_link_libraries(Heightmap_Generator ${HEIGHTMAP_GENERATOR_LIBS})

# This prints out ALL variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
