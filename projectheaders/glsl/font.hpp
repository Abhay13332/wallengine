#ifndef GLSL_FONT_H
#define GLSL_FONT_H
#include "core/edge-coloring.h"
#include "ext/import-font.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include <GL/gl.h>
#include <fontconfig/fontconfig.h>
#include <includeall.hpp>
#include <memory>
#include <msdf-atlas-gen/TightAtlasPacker.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdf-atlas-gen/types.h>
#include <utils/setmap.hpp>



class fi_font{
    std::string font_path; 
    msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<unsigned char, 3>> bitmapStorage;
    public:
    int channels=3;
    std::vector<msdf_atlas::GlyphGeometry> glyphsgeo;
  
     std::unique_ptr<unsigned char> ptr;
    // msdf_atlas::BitmapAtlasStorage<unsigned char, 3> bitmapStorage;
    msdfgen::BitmapConstRef<unsigned char, 3> bitmap;
   
    fi_font(std::string path);
    fi_font();
    void set_default_font();
   
    void load_font(fi_settings cur_settings=fi_setting_set());

};
#endif