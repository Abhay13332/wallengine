cmake_minimum_required(VERSION 4.2)
project(wallengine LANGUAGES C CXX)


#include package
include(FetchContent)

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(WLR REQUIRED wlroots-0.19)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
pkg_check_modules(WAYLAND_SCANNER REQUIRED wayland-scanner)
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
pkg_check_modules(GLESV2 REQUIRED glesv2)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
# Protocol Generation logic
pkg_get_variable(WAYLAND_SCANNER_PATH wayland-scanner wayland_scanner)
pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)

#font

pkg_check_modules(FCCONFIG REQUIRED fontconfig)
pkg_check_modules(FREETYPE REQUIRED freetype2)
set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_SKIA OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_MSDFGEN_EXTERNAL OFF CACHE BOOL "" FORCE) # Ensures internal msdfgen is used

# Optional: Configuration for the internal msdfgen
set(MSDFGEN_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_VCPKG OFF CACHE BOOL "" FORCE)

# --- 2. Fetch ONLY msdf-atlas-gen ---
FetchContent_Declare(
    msdf-atlas-gen
    GIT_REPOSITORY https://github.com/Chlumsky/msdf-atlas-gen.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(msdf-atlas-gen)
set("MSDF_LIBRARIES" msdfgen::msdfgen-ext msdf-atlas-gen::msdf-atlas-gen)

function(generate_wayland_protocol protocol_file)
    get_filename_component(protocol_name ${protocol_file} NAME_WE)
    
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-protocol.c"
        COMMAND ${WAYLAND_SCANNER_PATH} private-code ${protocol_file} "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-protocol.c"
        DEPENDS ${protocol_file}
    )
    
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-client-protocol.h"
        COMMAND ${WAYLAND_SCANNER_PATH} client-header ${protocol_file} "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-client-protocol.h"
        DEPENDS ${protocol_file}
    )
    
    set(PROTOCOL_SOURCES ${PROTOCOL_SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-protocol.c" PARENT_SCOPE)
    set(PROTOCOL_HEADERS ${PROTOCOL_HEADERS} "${CMAKE_CURRENT_BINARY_DIR}/${protocol_name}-client-protocol.h" PARENT_SCOPE)
endfunction()

# Protocols to generate
generate_wayland_protocol("${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml")
generate_wayland_protocol("/home/abhay/tmptasks/wlroots/protocol/wlr-layer-shell-unstable-v1.xml")

#include path
set(STB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externalheaders/stb") 
set(SRC_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/projectheaders")

#include srcfiles
set(SRC_FILES "src/texture.cpp" "src/stb.cpp" "src/waylandinterrupt.cpp"
        "src/wayland-xdg/fi_keyboard_handler.cpp" "src/wayland-xdg/eventqueue.cpp"
         "src/glsl/font.cpp" 
         "src/utils/setmap.cpp" )
# Add your executable
add_executable(wallengine main.cpp  ${PROTOCOL_SOURCES} ${PROTOCOL_HEADERS} ${SRC_FILES})

#add module targets




# Include directories
target_include_directories(wallengine PRIVATE 
    ${WLR_INCLUDE_DIRS} 
    ${WAYLAND_CLIENT_INCLUDE_DIRS}
    ${WAYLAND_EGL_INCLUDE_DIRS}
    ${GLESV2_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${STB_INCLUDE_DIR}
    ${SRC_INCLUDE_DIR}
    ${XKBCOMMON_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${FCCONFIG_INCLUDE_DIRS}
    ${msdfgen_SOURCE_DIR}
    ${msdfgen-atlas_SOURCE_DIR}
)

# Link libraries
target_link_libraries(wallengine PRIVATE 
    ${WLR_LIBRARIES} 
    ${WAYLAND_CLIENT_LIBRARIES}
    ${WAYLAND_EGL_LIBRARIES}
    ${GLESV2_LIBRARIES}
    ${EGL_LIBRARIES}
    ${XKBCOMMON_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${MSDF_LIBRARIES}
    ${FCCONFIG_LIBRARIES}
)

#cmake flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
 
# Specify C++ standard
set_target_properties(wallengine PROPERTIES 
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)
# We need -DWLR_USE_UNSTABLE for many wlroots features
add_definitions(-DWLR_USE_UNSTABLE)

