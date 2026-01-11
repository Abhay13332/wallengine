#include "includeall.hpp"

using namespace std;

GLuint texture(string path){
int height,width,channels;
stbi_set_flip_vertically_on_load(true);  
unsigned char* texture_data = stbi_load(path.c_str(),&width,&height,&channels,STBI_rgb_alpha);

GLuint textureId;
glGenTextures(1,&textureId);
glBindTexture(GL_TEXTURE_2D,textureId);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texture_data);
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(texture_data);
return textureId;
}