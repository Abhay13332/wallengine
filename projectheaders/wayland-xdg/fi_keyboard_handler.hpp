#ifndef FI_KEYBOARD_HANDLER_H
#define FI_KEYBOARD_HANDLER_H
#include "includeall.hpp"


class fi_keyboard_handler{
    xkb_state *keystate;
    wl_keyboard_listener keyboard_listener;
    public:
    int time;
    
    static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
                                  uint32_t format, int32_t fd, uint32_t size){
        fi_keyboard_handler* instance=static_cast<fi_keyboard_handler*>(data);                            
        xkb_context *ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        char* memmap=(char*)mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
        xkb_keymap* kmp= xkb_keymap_new_from_string(ctx,memmap,
                XKB_KEYMAP_FORMAT_TEXT_V1,XKB_KEYMAP_COMPILE_NO_FLAGS);
        munmap((void*)mmap,size);
        close(fd);
        
        instance->keystate=xkb_state_new(kmp);        
    }
    static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
       uint32_t serial, uint32_t time, 
       uint32_t key, uint32_t state) {
        fi_keyboard_handler* instance=static_cast<fi_keyboard_handler*>(data);                            

        uint32_t xkbkey=8+key;
        if(state == WL_KEYBOARD_KEY_STATE_PRESSED){
           char buf[128];
           xkb_state_key_get_utf8(instance->keystate,xkbkey,buf,sizeof(buf));
            instance->time=1;
          // keyhandling
      }
      if(state == WL_KEYBOARD_KEY_STATE_RELEASED){
         // keyhandling
      }
    }
    void setup(wl_keyboard *wl_keyboard){
      keyboard_listener.key=keyboard_handle_key;
      keyboard_listener.keymap=keyboard_handle_keymap;
      keyboard_listener.modifiers=keyboard_handle_modifiers;
      keyboard_listener.enter=keyboard_handle_enter;
      keyboard_listener.leave=keyboard_handle_leave;
      keyboard_listener.repeat_info=keyboard_handle_repeat_info;
      //remain
      wl_keyboard_add_listener(wl_keyboard,&keyboard_listener,this);
    }
    static void  keyboard_handle_modifiers(void *data, wl_keyboard *wl_keyboard,
                   uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, 
                      uint32_t mods_locked, uint32_t group){
         //keyhandling
    }
    static void keyboard_handle_enter(void *data, wl_keyboard *wl_keyboard, uint32_t serial, wl_surface *surface, wl_array *keys){}
    static void keyboard_handle_leave(void *data, wl_keyboard *wl_keyboard, uint32_t serial, wl_surface *surface){}
    static void keyboard_handle_repeat_info(void *data, wl_keyboard *wl_keyboard, int32_t rate, int32_t delay){}
    
 
    
    
    
    
 
};

#endif