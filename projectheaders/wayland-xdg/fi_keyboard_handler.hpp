#ifndef FI_KEYBOARD_HANDLER_H
#define FI_KEYBOARD_HANDLER_H
#include "wayland-xdg/eventqueue.hpp"
#include <includeall.hpp>
#include <memory>
#include <xkbcommon/xkbcommon-compat.h>
#include <xkbcommon/xkbcommon.h>
struct fi_xkb_indices {
  xkb_mod_index_t alt, super, caps, shift, ctrl;
} __attribute__((aligned(32)));

struct repeatinfo {};
class fi_keyboard_handler {
  xkb_state *keystate;
  wl_keyboard_listener keyboard_listener;
  fi_xkb_indices indices;
  fi_EventQueue *queue;
  
public:
   fi_keyboard_handler(fi_EventQueue *queue){
      this->queue=queue;
    }
  static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
                                     uint32_t format, int32_t fd,
                                     uint32_t size) {
    auto *instance = static_cast<fi_keyboard_handler *>(data);
    xkb_context *ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    char *memmap =
        static_cast<char *>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));
    xkb_keymap *kmp = xkb_keymap_new_from_string(
        ctx, memmap, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    munmap(reinterpret_cast<void *>(mmap), size);
    close(fd);

    // setting indices
    instance->indices.alt = xkb_keymap_mod_get_index(kmp, XKB_MOD_NAME_ALT);
    instance->indices.caps = xkb_keymap_mod_get_index(kmp, XKB_MOD_NAME_CAPS);
    instance->indices.ctrl = xkb_keymap_mod_get_index(kmp, XKB_MOD_NAME_CTRL);
    instance->indices.shift = xkb_keymap_mod_get_index(kmp, XKB_MOD_NAME_SHIFT);
    instance->indices.super = xkb_keymap_mod_get_index(kmp, XKB_MOD_NAME_LOGO);

    // setting keystate for instance
    instance->keystate = xkb_state_new(kmp);
  }
  static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
                                  uint32_t serial, uint32_t time, uint32_t key,
                                  uint32_t state) {
    auto *instance = static_cast<fi_keyboard_handler *>(data);

    uint32_t xkbkey = 8 + key;
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED ||
        state == WL_KEYBOARD_KEY_STATE_RELEASED) {
      char buf[128];
      
      auto sym=xkb_state_key_get_one_sym(instance->keystate,xkbkey);
      xkb_keysym_get_name(sym,buf, sizeof(buf));
      std::cout << buf<< std::endl;

      fi_event keyboard_event = {
          .eventtype = state == WL_KEYBOARD_KEY_STATE_PRESSED
                           ? FI_KEYBOARD_PRESSED
                           : FI_KEYBOARD_RELEASED,

      };
      keyboard_event.Data =
          std::make_unique<fi_event_keyboard_data>((fi_event_keyboard_data{
              .keypressed = std::string(buf),
              .modifier_states{
                  .alt = (bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.alt,
                      XKB_STATE_MODS_EFFECTIVE),
                  .ctrl = (bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.ctrl,
                      XKB_STATE_MODS_EFFECTIVE),
                  .shift = (bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.shift,
                      XKB_STATE_MODS_EFFECTIVE),
                  .super = (bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.super,
                      XKB_STATE_MODS_EFFECTIVE),
                  .caps_lock = (bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.caps,
                      XKB_STATE_MODS_EFFECTIVE),

              }}));
              if((bool)xkb_state_mod_index_is_active(
                      instance->keystate, instance->indices.ctrl,
                      XKB_STATE_MODS_EFFECTIVE) && std::string(buf)=="c" ){
                        std::exit(3);
                        std::cout << "yes" << std::endl; 
                      }else{
                        std::cout << "no" << std::endl; 
                      }

      instance->queue->addEvent(std::move(keyboard_event));
    }
  }
  void setup(wl_keyboard *wl_keyboard) {
    keyboard_listener.key = keyboard_handle_key;
    keyboard_listener.keymap = keyboard_handle_keymap;
    keyboard_listener.modifiers = keyboard_handle_modifiers;
    keyboard_listener.enter = keyboard_handle_enter;
    keyboard_listener.leave = keyboard_handle_leave;
    keyboard_listener.repeat_info = keyboard_handle_repeat_info;
    // remain
    wl_keyboard_add_listener(wl_keyboard, &keyboard_listener, this);
  }
  static void keyboard_handle_modifiers(void *data, wl_keyboard *wl_keyboard,
                                        uint32_t serial,
                                        uint32_t mods_depressed,
                                        uint32_t mods_latched,
                                        uint32_t mods_locked, uint32_t group) {
    xkb_state_update_mask(static_cast<fi_keyboard_handler*>(data)->keystate, mods_depressed,mods_latched, mods_locked, 0,0,group);
  }
  static void keyboard_handle_enter(void *data, wl_keyboard *wl_keyboard,
                                    uint32_t serial, wl_surface *surface,
                                    wl_array *keys) {}
  static void keyboard_handle_leave(void *data, wl_keyboard *wl_keyboard,
                                    uint32_t serial, wl_surface *surface) {}
  static void keyboard_handle_repeat_info(void *data, wl_keyboard *wl_keyboard,
                                          int32_t rate, int32_t delay) {}
};

#endif