cmake_minimum_required(VERSION 3.8.2)

project(kme-smb3)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)

find_package(PhysFS REQUIRED)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics network system window)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/json/include)

add_executable(
  kme-smb3
  src/states/basestate.cpp
  src/states/basegame/entities.cpp
  src/states/basegame/tiledefs.cpp
  src/states/basegame/tilemap.cpp
  src/states/basegame/world.cpp
  src/states/basegame.cpp
  src/states/gameplay.cpp
  src/states/intro.cpp
  src/util/file.cpp
  src/assetmanager.cpp
  src/engine.cpp
  src/input.cpp
  src/renderstates.cpp
  src/main.cpp
)

target_link_libraries(
  kme-smb3
  physfs
  sfml-audio sfml-graphics sfml-network sfml-system sfml-window
)
