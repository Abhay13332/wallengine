

#include "glsl/font.hpp"
#include "ext/import-font.h"
#include "glsl/texture.hpp"
#include <includeall.hpp>
using namespace msdf_atlas;

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
    FcPattern* matched_pattern = FcFontMatch(nullptr, pattern, &result);
    if(!matched_pattern) std::cout << "no scalable font installed err \n",exit(0);
    FcChar8* font_path;
    if(FcResultMatch != FcPatternGetString(matched_pattern, FC_FILE, 0, &font_path)) std::cout << "can't get ont path err\n",exit(0);
    this->font_path.assign((char*)(font_path));
     
     FcFini();
    }
     
void fi_font::load_font(fi_settings cur_settings){
    
    // std::cout << "sdf" <<cur_settings.contains("sdf") << "wer" << std::endl;

    
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
    // double glyphScale = 64.0; // Increase this (e.g., to 48 or 64) for higher resolution
    double pixelRange = fi_get_else(cur_settings, "PixelRange", double,2.0);  // Standard range
    for(GlyphGeometry &glyph:glyphs){

        glyph.edgeColoring(msdfgen::edgeColoringInkTrap, maxCornerAngle,0);
        // glyph.wrapBox(glyphScale, pixelRange / glyphScale, 1.0);
        

    }
    TightAtlasPacker Tpacker;
    Tpacker.setDimensionsConstraint(fi_get_else(cur_settings,"DimensionConstraint",DimensionsConstraint,DimensionsConstraint::SQUARE));   
    
    Tpacker.setMinimumScale(fi_get_else(cur_settings, "glyphPixelSizeMin", double,  48.0));
    Tpacker.setPixelRange(pixelRange);
    Tpacker.setMiterLimit(fi_get_else(cur_settings, "MiterLimit", double,1.0));
    Tpacker.pack(glyphs.data(), glyphs.size());
    int width, height;
    Tpacker.getDimensions(width, height);
    bitmapStorage=ImmediateAtlasGenerator<float, 3, msdfGenerator, BitmapAtlasStorage<byte, 3>>(width,height);
    channels=3;
    GeneratorAttributes generatorattr;
    bitmapStorage.setAttributes(generatorattr);
    bitmapStorage.setThreadCount(4.0);
    bitmapStorage.generate(glyphs.data(),glyphs.size());
    std::cout << font_path << std::endl;

    glyphsgeo=glyphs;
    // msdfgen::BitmapSection<unsigned char, 3> bitmapd << end;
   
  
    // std::cout <<(bitmap.pixels==nullptr) << std::endl; 
     bitmap = (bitmapStorage.atlasStorage());
    
    stbi_flip_vertically_on_write(true);
   
    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(msdf_ft);



   


}