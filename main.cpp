#include "wayland-xdg/eventqueue.hpp"
#include <filesystem>
#include <glsl/texture.hpp>
#include <wayland-xdg/fi_keyboard_handler.hpp>
#include <waylandinterrupt.hpp>
#include <xdg-shell-client-protocol.h>
#include <utils/smpointer.hpp>

#include <wayland-client-protocol.h>
#include <wayland-xdg/fi_pointer_handler.hpp>
#include "glsl/font.hpp"
using namespace std;
 
void err(const string& str){
cout << "err:"<<str;

exit(EXIT_SUCCESS);
}
string readfile(const string &path){
    cout << std::filesystem::absolute(path);
    std:ifstream openfile(path);
    if(!openfile){
        
        err("readfile"+path);

    }
    stringstream buff;
    buff << openfile.rdbuf() ;
    return buff.str();


} 
GLuint createShader(GLenum type,const char* src){
    GLuint sid=glCreateShader(type);
    glShaderSource(sid,1,&src,nullptr);
    glCompileShader(sid);
    GLint isCompiled ;
    glGetShaderiv(sid,GL_COMPILE_STATUS,&isCompiled);
    if(!isCompiled){
          std::array<char,512> buf{}; glGetShaderInfoLog(sid, 512, nullptr, buf.data());
        std::cerr << "GLSL Error: " << buf.data() << "\n";
        err("shaderissue");
    }
 
    
    return sid;
}
GLuint attachshaderandgetprogram(){
  
    string  frag_src_str=readfile(std::string(SOURCE_DIR)+"src_sh/fragment.glsl");
    string  vert_src_str=readfile(std::string(SOURCE_DIR)+"src_sh/vertex.glsl");
    const char* vert_src=vert_src_str.c_str();
    const char* frag_src=frag_src_str.c_str();
    const GLuint fragshadId = createShader(GL_FRAGMENT_SHADER,frag_src);
    cout << "fragment compiled" <<"\n";
    const GLuint vertshadId=createShader(GL_VERTEX_SHADER,vert_src);
    cout << "vertex compiled" <<"\n";
    
    GLuint program =glCreateProgram();
    
    glAttachShader(program,vertshadId);
    glAttachShader(program,fragshadId);
    glLinkProgram(program);
    glUseProgram(program);
    glDeleteShader(fragshadId);
    glDeleteShader(vertshadId);
    return program;
    


}
void glUnbindVertexArray(){
    glBindVertexArray(0);
}
struct globalRemove{
void operator()(wl_compositor *comp) const {    if(comp)wl_compositor_destroy(comp);}
void operator()(xdg_wm_base   *shell) const { if(shell) xdg_wm_base_destroy(shell);}
};
 unique_ptr<wl_compositor,globalRemove> comp = nullptr;
unique_ptr<xdg_wm_base,globalRemove>shell = nullptr;

struct seatData{
    uint32_t global_id;
    string name;
    fi_unique<wl_seat, wl_seat_release> seat;
    fi_unique<wl_keyboard, wl_keyboard_release> keyboard;
    fi_unique<wl_pointer,wl_pointer_release> pointer;
     
    
} __attribute__((aligned(64))) ;
static const struct wl_keyboard_listener keyboard_listener={
   
};
static const struct wl_pointer_listener pointer_listener ={
    
};
static  vector<unique_ptr<seatData>> vtseat;
static fi_keyboard_handler keyb(new fi_EventQueue());
static fi_pointer_listener pointH;
static void seat_handle_cap(void *data, struct wl_seat *curr_seat, uint32_t caps) {
    auto* curr_seat_info=static_cast<seatData *>(data);
    if((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !curr_seat_info->keyboard){
        curr_seat_info->keyboard.reset(wl_seat_get_keyboard(curr_seat));
        keyb.setup(curr_seat_info->keyboard.get());   
    }
    if((caps & WL_SEAT_CAPABILITY_POINTER) && !curr_seat_info->pointer){
       curr_seat_info->pointer.reset(wl_seat_get_pointer(curr_seat));
       pointH.setup(curr_seat_info->pointer.get());
     }

}
void set_seat_name(void *data, wl_seat *curr_seat, const char *name){
 auto *curr_seat_info=static_cast<seatData*>(data);
 curr_seat_info->name=string(name);

 
}
static const struct  wl_seat_listener seat_listener ={
    .capabilities=seat_handle_cap,
    .name=set_seat_name
};
static void on_global(void* data, struct wl_registry* reg, uint32_t id, const char* intf, uint32_t ver) {
    if (!strcmp(intf, "wl_compositor")) comp =unique_ptr<wl_compositor,globalRemove>( static_cast<wl_compositor *>(wl_registry_bind(reg, id, &wl_compositor_interface, ver)));
    if (!strcmp(intf, "xdg_wm_base"))   shell =unique_ptr<xdg_wm_base,globalRemove>( static_cast<xdg_wm_base *>(wl_registry_bind(reg, id, &xdg_wm_base_interface, ver)));

    if(!strcmp(intf,wl_seat_interface.name)){
            unique_ptr<seatData> currst=make_unique<seatData>(seatData({
             .global_id=id,
                
            }));
         
            currst->seat.reset((static_cast<wl_seat*>(wl_registry_bind(reg,id,&wl_seat_interface,ver))));
            wl_seat_add_listener(currst->seat.get(),&seat_listener,currst.get());
            vtseat.push_back(std::move(currst));
      }
}

static const struct wl_registry_listener reg_list = { 
    .global = on_global
};

static const struct xdg_toplevel_listener xdg_list{
.configure=[](void * data, struct xdg_toplevel * , int32_t, int32_t, struct wl_array *){

}
};
static void on_ping(void* data, struct xdg_wm_base* wm_base, uint32_t ser) { xdg_wm_base_pong(wm_base, ser); }
static const struct xdg_wm_base_listener shell_list = { .ping = on_ping };

int main() {
   
    signals_bind();
    auto *disp= wl_display_connect(nullptr);
    auto *reg  = wl_display_get_registry(disp);
    wl_registry_add_listener(reg, &reg_list, nullptr);
    wl_display_roundtrip(disp);
    if (!comp || !shell) return 1;
    xdg_wm_base_add_listener(shell.get(), &shell_list, nullptr);
    
    
    EGLDisplay edpy = eglGetDisplay((EGLNativeDisplayType)disp);
    eglInitialize(edpy, nullptr, nullptr);
     
    EGLint attr[] = { EGL_RED_SIZE, 8,
         EGL_GREEN_SIZE, 8,
         EGL_BLUE_SIZE, 8, 
         EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
         EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES, 4, 
        EGL_NONE };
        eglBindAPI(EGL_OPENGL_API);
        EGLConfig cfg; EGLint n;

        eglChooseConfig(edpy, attr, &cfg, 1, &n);
        
        EGLint cattr[] = { 
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION,6,
            EGL_CONTEXT_OPENGL_PROFILE_MASK,
            EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_NONE 
        };
        EGLContext ctx = eglCreateContext(edpy, cfg, EGL_NO_CONTEXT, cattr);
        
        
        auto *surf = wl_compositor_create_surface(comp.get());
        auto *xsurf = xdg_wm_base_get_xdg_surface(shell.get(), surf);
        auto *top = xdg_surface_get_toplevel(xsurf);
        xdg_toplevel_set_title(top, "GPU Animated Window");
        
        
        int width = 1920/2, height =1080/2;
        struct wl_egl_window *ewin = wl_egl_window_create(surf, width, height);
        EGLSurface esur = eglCreateWindowSurface(edpy, cfg, ewin, nullptr);
        //setting current window for drawing context
        eglMakeCurrent(edpy, esur, esur, ctx);
        wl_surface_commit(surf);
        GLuint programId=attachshaderandgetprogram();
         
        
 
     GLfloat verts[]={
        -1,-1,    -1,-1,   
         1,1,      1,1,
        -1,1,     -1,1,
         1,-1,     1,-1
    };  
    GLuint indices[]={0,1,2,0,1,3};
    GLuint vbo,vao,ebo;
    glGenVertexArrays(1,&vao);
    
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*(sizeof(GLfloat)),nullptr);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glUnbindVertexArray();
    GLint time_loc=glGetUniformLocation(programId,"time");
    GLuint pointerX_loc=glGetUniformLocation(programId, "programX");
    GLuint pointerY_loc=glGetUniformLocation(programId, "programY");
    GLint txt1_loc=glGetUniformLocation(programId,"txt1");
    fi_font font("/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf");
    font.load_font();
    GLuint txt1Id=fi_glsl_set_texture("/home/abhay/Pictures/Wallpapers/wallhaven-1qq9w1.jpg").textureId;
    GLint txt2_loc=glGetUniformLocation(programId,"txt2");
    
    GLuint txt2Id=fi_glsl_set_texture((std::string(SOURCE_DIR)+"new.png")).textureId;
    cout << txt1Id << " " << txt2Id<<"\n";
    // cout << (font.bitmap.pixels==nullptr) << endl;
     
    cout << font.channels<< endl;
    // stbi_write_png((std::string(SOURCE_DIR)+"new.png").c_str(),font.bitmap.width,font.bitmap.height,3,font.ptr,font.bitmap.width*3);

    GLuint txtfontbitmapId =fi_glsl_set_texture(font.bitmap.height,font.bitmap.width,font.channels,font.bitmap.pixels).textureId;
    glBindVertexArray(vao);
    float time=0;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,txtfontbitmapId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,txt2Id);
    check_glsl_error();
    glUniform1i(txt1_loc,0);
    glUniform1i(txt2_loc,1);
//  fi_font textFont;
//     textFont.load_font();
//     auto piel=textFont.bitmap.pixels;
   float addition=0.01F;
    
   while (!exitflag_sig && wl_display_dispatch(disp) != -1) {
           
            time+=addition;
            //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUniform1f(time_loc,time);
            glUniform1f(pointerX_loc, pointH.x);
            glUniform1f(pointerY_loc, pointH.y);
            glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_INT,nullptr);
            eglSwapBuffers(edpy, esur);
            if(time>2.1){
               addition=-0.01F;
            }
            if(time<=0){
                addition=0.01F;
            }

       
    }

    
    eglDestroyContext(edpy, ctx);
    eglDestroySurface(edpy, surf);
    eglTerminate(edpy);
   
   
    return 0;
}