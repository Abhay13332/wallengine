
#include "core/edge-coloring.h"
#include "ext/import-font.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include "utils/setmap.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fontconfig/fontconfig.h>
#include <includeall.hpp>
#include <msdf-atlas-gen/TightAtlasPacker.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdf-atlas-gen/types.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace msdf_atlas;
class fi_font{
    std::string font_path;

   public:
    fi_font(std::string path);
    fi_font();
    void set_default_font();
   
    void load_font(fi_settings cur_settings=fi_setting_set());

};
fi_font::fi_font(std::string path){
        font_path=path;
       

}          
fi_font::fi_font(){
    set_default_font();
}

void fi_font::set_default_font(){
        FcPattern *pattern;
    if(!FcInit()||(pattern=FcPatternCreate())==nullptr ){
        std::cout << "font error" << "\n";
        exit(0);
    }
    FcPatternAddString(pattern, FC_FAMILY, reinterpret_cast<FcChar8*>(const_cast<char8_t*>(u8"sans-serif")));
    FcPatternAddBool(pattern,FC_SCALABLE,FcTrue);
    FcResult result;
    FcPattern* matched_pattern = FcFontMatch(NULL, pattern, &result);
    if(!matched_pattern) std::cout << "no scalable font installed err \n",exit(0);
    FcChar8* font_path;
    if(FcResultMatch != FcPatternGetString(matched_pattern, FC_FILE, 0, &font_path)) std::cout << "can't get ont path err\n",exit(0);
    this->font_path.assign((char*)(font_path));
     delete font_path;
     FcFini();
    }
     
void fi_font::load_font(fi_settings cur_settings){
    

    
    msdfgen::FreetypeHandle *msdf_ft;
    msdfgen::FontHandle *font;
    if(!(msdf_ft = msdfgen::initializeFreetype())|| !(font =msdfgen::loadFont(msdf_ft, font_path.c_str()))) std::cout << "font load msdf error\n",exit(0);
    std::vector<GlyphGeometry> glyphs;
    FontGeometry fontgeometry(&glyphs);
    if(cur_settings.contains("charSet")){
        Charset curr_charSet;
        auto charSetStr=cur_settings.get<const char*>("charSet");
        curr_charSet.parse(charSetStr, strlen(charSetStr));
        fontgeometry.loadCharset(font, fi_get_else(cur_settings,"fontScale",double,1.0),curr_charSet );

    }else{
        fontgeometry.loadCharset(font, fi_get_else(cur_settings,"fontScale",double,1.0), Charset::ASCII);}
    const double maxCornerAngle = fi_get_else(cur_settings,"maxCornerAngle",double,0);
    for(GlyphGeometry &glyph:glyphs)
        glyph.edgeColoring(msdfgen::edgeColoringInkTrap, maxCornerAngle,0);
    TightAtlasPacker Tpacker;
    Tpacker.setDimensionsConstraint(fi_get_else(cur_settings,"DimensionConstraint",DimensionsConstraint,DimensionsConstraint::SQUARE));   
    
    Tpacker.setMinimumScale(fi_get_else(cur_settings, "glyphPixelSizeMin", double,24.0));
    Tpacker.setPixelRange(fi_get_else(cur_settings, "PixelRange", double,2.0));
    Tpacker.setMiterLimit(fi_get_else(cur_settings, "PixelRange", double,1.0));

    Tpacker.pack(glyphs.data(), glyphs.size());
    int width = 0, height = 0;
    Tpacker.getDimensions(width, height);
    ImmediateAtlasGenerator<float, 3, msdfGenerator, BitmapAtlasStorage<byte, 3>> generator(width,height);
    GeneratorAttributes generatorattr;
    generator.setAttributes(generatorattr);
    generator.setThreadCount(4);
    generator.generate(glyphs.data(),glyphs.size());
    



   


}