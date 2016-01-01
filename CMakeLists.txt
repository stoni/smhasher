project(SMHasher)

cmake_minimum_required(VERSION 2.4)

set(CMAKE_BUILD_TYPE Release)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 11")
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

add_library(
  SMHasherSupport
  AvalancheTest.cpp
  Bitslice.cpp
  Bitvec.cpp
  CityTest.cpp
  City.cpp
  crc.cpp
  DifferentialTest.cpp
  Hashes.cpp
  KeysetTest.cpp
  lookup3.cpp
  md5.cpp
  MurmurHash1.cpp
  MurmurHash2.cpp
  MurmurHash3.cpp
  Platform.cpp
  Random.cpp
  sha1.cpp
  SpeedTest.cpp
  Spooky.cpp
  SpookyTest.cpp
  Stats.cpp
  SuperFastHash.cpp
  Types.cpp
  PMurHash.c
  sanmayce.cpp  
  xxhash.c
)

add_executable(
  SMHasher
  main.cpp
)

target_link_libraries(
  SMHasher
  SMHasherSupport
  "${CMAKE_SOURCE_DIR}/memhash.lib"
)
