cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(bomberman)

set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/libs/include")

if (MSVC)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
      set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/libs/msvc/x64/SDL2.lib;${CMAKE_SOURCE_DIR}/libs/msvc/x64/SDL2main.lib")
      configure_file("${CMAKE_SOURCE_DIR}/libs/msvc/x64/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    else ()
      set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/libs/msvc/x86/SDL2.lib;${CMAKE_SOURCE_DIR}/libs/msvc/x86/SDL2main.lib")
      configure_file("${CMAKE_SOURCE_DIR}/libs/msvc/x86/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    endif ()

    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
elseif(MINGW)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "-L${CMAKE_SOURCE_DIR}/libs/mingw/x64  -lmingw32 -lSDL2main -lSDL2  -mwindows")
        configure_file("${CMAKE_SOURCE_DIR}/libs/mingw/x64/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    else ()
        set(SDL2_LIBRARIES "-L${CMAKE_SOURCE_DIR}/libs/mingw/x86  -lmingw32 -lSDL2main -lSDL2  -mwindows")
        configure_file("${CMAKE_SOURCE_DIR}/libs/mingw/x86/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
    endif ()

    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
else()
    message(FATAL_ERROR "-- No supported compiler found")
endif()

# Configure compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  # using regular Clang or AppleClang or GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror -pedantic -Wold-style-cast")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "/std:c++14 /permissive- /W4 /EHsc")
endif()

# TODO find a better way of copying files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/brick_gray0.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/stone2_gray0.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/human.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/troll.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/delayed_fireball.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/conjure_flame.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/sticky_flame.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/potion_bubbly.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/gameOver.png ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/maps/classic.txt ${CMAKE_CURRENT_BINARY_DIR}/maps/classic.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/maps/default.txt ${CMAKE_CURRENT_BINARY_DIR}/maps/default.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/maps/empty.txt ${CMAKE_CURRENT_BINARY_DIR}/maps/empty.txt COPYONLY)

add_library(bomber
	./Player.cpp
	./Bomb.cpp
	./Flame.cpp
	./Wall.cpp
	./MapParser.cpp
	./BonusItem.cpp
	./GameObjectFactory.cpp
	./Playground.cpp
	./Geometry.cpp
	./Config.cpp
	./Dimensions.cpp
	./Bomberman.cpp
)

target_include_directories(bomber PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(bomberman
	./main.cpp
	./SDLWrapper.cpp
	./Grid.cpp
	./Textures.cpp
	./PlayerControl.cpp
)

target_include_directories(bomberman SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/include)

target_link_libraries(bomberman
        bomber
        ${SDL2_LIBRARIES}
)

target_include_directories(bomberman PRIVATE
        ${SDL2_INCLUDE_DIRS}
)


add_executable(testbomb
	./test/main.cpp
	./test/student_tests.cpp
)

target_link_libraries( testbomb
	bomber
)

