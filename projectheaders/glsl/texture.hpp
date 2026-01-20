#ifndef TEXTURE_H
#define TEXTURE_H
#include "includeall.hpp"

struct fi_glsl_texture{
  GLuint textureId;
  int height;
  int width;
  int channels;
};
fi_glsl_texture fi_glsl_set_texture(std::string path);
fi_glsl_texture fi_glsl_set_texture(int height,int width,int channels,const unsigned char* texture_data);


#endif