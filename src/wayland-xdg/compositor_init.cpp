#include "includeall.hpp"
#include "utils/smpointer.hpp"
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-shell-client-protocol.h"
#include <wayland-client-protocol.h>

class global_obj{
   public:
      fi_unique<wl_compositor, wl_compositor_destroy>comp;
      fi_unique<xdg_wm_base,xdg_wm_base_destroy>xdg_shell;
      fi_unique<wl_data_device_manager, wl_data_device_manager_destroy>data_device_manager;
      fi_unique<wl_subcompositor,wl_subcompositor_destroy> subcomp;
      fi_unique<zwlr_layer_shell_v1, zwlr_layer_shell_v1_destroy> layer_shell; 
   };
 

class wl_registry_listener_manager{
   private:
    

     // objectbinder initializer
     static void on_global(void* data, struct wl_registry* reg, uint32_t id, const char* intf, uint32_t ver) {
        auto* globjptr=static_cast<global_obj*>(data);
         
         if (!strcmp(intf, "wl_compositor")) globjptr->comp.reset((wl_compositor*)wl_registry_bind(reg, id, &wl_compositor_interface, ver));
         if (!strcmp(intf, "xdg_wm_base"))   globjptr->xdg_shell.reset(static_cast<xdg_wm_base*>(wl_registry_bind(reg, id, &xdg_wm_base_interface, ver)));
         if (!strcmp(intf, "wl_data_device_manager")) globjptr->data_device_manager.reset((wl_data_device_manager*)wl_registry_bind(reg, id, &wl_data_device_manager_interface, ver));
         if (!strcmp(intf, "wl_subcompositor")) globjptr->subcomp.reset((wl_subcompositor*)wl_registry_bind(reg, id, &wl_subcompositor_interface, ver));
         if (!strcmp(intf, "zwlr_layer_shell_v1"))   globjptr->layer_shell.reset((zwlr_layer_shell_v1*)wl_registry_bind(reg, id, &zwlr_layer_shell_v1_interface, ver));
         
      }
     static void on_global_remove(void *data, wl_registry *wl_registry, uint32_t name){
             
     }
     
     struct wl_registry_listener reg_list = { .global = on_global };


     
   public:

};
class xdg_registry_listener_manager{
   
   
     // ping pong for xdg app require to comfirm wayland that we are alive
     static void on_ping(void*, struct xdg_wm_base* s, uint32_t ser) { xdg_wm_base_pong(s, ser); }
     const struct xdg_wm_base_listener shell_list = { .ping = on_ping };
     

};