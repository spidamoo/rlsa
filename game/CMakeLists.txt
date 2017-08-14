project(client)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/manual-bin)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/manual-bin)
set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_SOURCE_DIR}/manual-bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/manual-bin)

include_directories(include)
include_directories(../rnslib/include)
include_directories(../rnslib/linux_include)
include_directories(SYSTEM ../Eigen)
include_directories(../hge181/include)

SET(CMAKE_SYSTEM_NAME      Windows)
SET(CMAKE_SYSTEM_PROCESSOR i586)

if (WIN32)
    SET(CMAKE_C_COMPILER       mingw32-gcc )
    SET(CMAKE_CXX_COMPILER     mingw32-g++ )
    SET(CMAKE_ASM-ATT_COMPILER mingw32-as  )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()

    if(EXISTS /usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin)
        SET(CMAKE_C_COMPILER       /usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc )
        SET(CMAKE_CXX_COMPILER     /usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-g++ )
        SET(CMAKE_ASM-ATT_COMPILER /usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-as  )

            SET(CMAKE_VERBOSE_MAKEFILE 1)
        message(this:)
        message(${CMAKE_CXX_COMPILE_OBJECT})
        message(${CMAKE_CXX_LINK_EXECUTABLE} )
            set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER>  <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
            #set(CMAKE_CXX_FLAGS_DEBUG -c)
            set(CMAKE_OSX_SYSROOT "")
            SET(CMAKE_RANLIB echo)
        SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rcs <TARGET> <LINK_FLAGS> <OBJECTS>")

        set(CMAKE_AR /usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-ar)
    endif()

endif()

add_library(hge STATIC IMPORTED)
set_property(TARGET hge PROPERTY
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/hge181/lib/gcc/libhge.a)

set(SOURCE_FILES main.cpp
        src/Exception.cpp
        src/GUIInput.cpp
        src/GUIItem.cpp
        src/GUIWindow.cpp
        src/GUIWindowLogin.cpp
        src/VisualCharacter.cpp
        src/VisualEffect.cpp
        src/VisualItem.cpp
        src/VisualGame.cpp
        src/VisualLoot.cpp
        ../hge181/src/helpers/hgeanim.cpp
        ../hge181/src/helpers/hgefont.cpp
        ../hge181/src/helpers/hgerect.cpp
        ../hge181/src/helpers/hgesprite.cpp
        ../tinyxml/tinystr.cpp
        ../tinyxml/tinyxml.cpp
        ../tinyxml/tinyxmlerror.cpp
        ../tinyxml/tinyxmlparser.cpp
        src/services/alert.cpp)


if (WIN32)
    add_executable(rns ${SOURCE_FILES})
else()
    add_executable(rns.exe ${SOURCE_FILES})
endif()


set(SOURCE_LIB ../rnslib/src/Game.cpp
        ../rnslib/src/Character.cpp
        ../rnslib/src/Effect.cpp
        ../rnslib/src/Item.cpp
        ../rnslib/src/Loot.cpp
        ../rnslib/src/NetworkPacket.cpp)

add_library(rnslib_win STATIC ${SOURCE_LIB})

if (WIN32)
    target_link_libraries(rns rnslib_win ws2_32 hge)
else()
    target_link_libraries(rns.exe rnslib_win ws2_32 hge)
endif()