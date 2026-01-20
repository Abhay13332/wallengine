#include "includeall.hpp"
#include <glsl/texture.hpp>


using namespace std;
void fi_glsl_set_to_texture(fi_glsl_texture &textureinfo,const unsigned char* texture_data,GLint data_format=GL_RGBA){

glGenTextures(1,&textureinfo.textureId);
glBindTexture(GL_TEXTURE_2D,textureinfo.textureId);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

// 2. Anisotropic Filtering (Prevents blur at angles)
// This is a core feature in 4.6
float maxAnisotropy = 0.1f;
glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);

check_glsl_error();
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,textureinfo.width,textureinfo.height,0,data_format,GL_UNSIGNED_BYTE,texture_data);
glGenerateMipmap(GL_TEXTURE_2D);
glGetError();
}
fi_glsl_texture fi_glsl_set_texture(string path){
fi_glsl_texture textureinfo;

stbi_set_flip_vertically_on_load(true);  

unsigned char* texture_data = stbi_load(path.c_str(),&textureinfo.width,&textureinfo.height,&textureinfo.channels,STBI_rgb_alpha);
cout <<"hjk"<< textureinfo.textureId<< endl;
fi_glsl_set_to_texture(textureinfo, texture_data);
cout << "hj"<<textureinfo.textureId<< endl;

stbi_image_free(texture_data);
return textureinfo;
}
fi_glsl_texture fi_glsl_set_texture(int height,int width,int channels,const unsigned char* texture_data){
    fi_glsl_texture textureinfo;
    textureinfo.height=height;
    textureinfo.width=width;
    textureinfo.channels=channels;
    GLint dataformat=0;
    if(channels==1){dataformat=GL_R;}
    else if(channels==2){dataformat=GL_RG;}
    else if(channels==3){dataformat=GL_RGB;}
    else if(channels==4){dataformat=GL_RGBA;}
    cout << dataformat << endl;
    fi_glsl_set_to_texture(textureinfo,texture_data,dataformat);
    return textureinfo;
    

}

