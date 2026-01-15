#ifndef GLSL_FONT_H
#define GLSL_FONT_H

#include "ext/import-font.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include <cstdlib>
#include <fontconfig/fontconfig.h>
#include "includeall.hpp"
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <vector>

void load_font();
#endif