cmake_minimum_required(VERSION 3.8.2)

project(kme-smb3)

find_package(PkgConfig REQUIRED)
find_package(PhysFS REQUIRED)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics network system window)
pkg_check_modules(GME REQUIRED IMPORTED_TARGET libgme)
pkg_check_modules(JSONCPP REQUIRED IMPORTED_TARGET jsoncpp)

add_executable(
  kme-smb3
  src/states/basestate.cpp
  src/states/basegame/ecs/entitydata.cpp
  src/states/basegame/tiledefs.cpp
  src/states/basegame/tilemap.cpp
  src/states/basegame/world.cpp
  src/states/basegame.cpp
  src/states/gameplay.cpp
  src/states/intro.cpp
  src/util/base64.cpp
  src/util/file.cpp
  src/util/string.cpp
  src/util/util.cpp
  src/assetmanager.cpp
  src/engine.cpp
  src/maploader.cpp
  src/music.cpp
  src/sound.cpp
  src/renderstates.cpp
  src/main.cpp
)

target_include_directories(
  kme-smb3
  PUBLIC include
)

target_link_libraries(
  kme-smb3
  jsoncpp
  physfs
  sfml-audio sfml-graphics sfml-network sfml-system sfml-window
  PkgConfig::GME
  PkgConfig::JSONCPP
)

set_target_properties(
  kme-smb3
  PROPERTIES
  CXX_STANDARD 17
)
