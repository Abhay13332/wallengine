#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <queue>
#include <cstring>
#include <memory>
#include <unistd.h>           
#include <csignal>
#include <sys/mman.h>
#include <xkbcommon/xkbcommon.h>
#include <linux/input-event-codes.h>
#include <sys/syscall.h>
#include <wayland-client.h>
#include <wayland-egl.h> // The bridge between Wayland and EGL
#include <EGL/egl.h>
#include <GLES3/gl32.h> 
#include <GL/gl.h>
#include <xdg-shell-client-protocol.h>
#include <utils/helpermacros.hpp>
#define namespace protocol_namespace
#include <wlr-layer-shell-unstable-v1-client-protocol.h>
#undef namespace
#include "stb_image.h"
#include <stb_image_write.h>

#endif

