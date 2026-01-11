#include "includeall.hpp"
class global_obj{
   public:
      wl_compositor *comp;
      xdg_wm_base   *xdg_shell;
      wl_data_device_manager* data_device_manager;
      wl_subcompositor* subcomp;
      zwlr_layer_shell_v1* layer_shell; 
   };
 

class wl_registry_listener_manager{
   private:
    

     // objectbinder initializer
     static void on_global(void* data, struct wl_registry* reg, uint32_t id, const char* intf, uint32_t ver) {
        global_obj* globjptr=static_cast<global_obj*>(data);
         
         if (!strcmp(intf, "wl_compositor")) globjptr->comp = (wl_compositor*)wl_registry_bind(reg, id, &wl_compositor_interface, ver);
         if (!strcmp(intf, "xdg_wm_base"))   globjptr->xdg_shell = (xdg_wm_base*)wl_registry_bind(reg, id, &xdg_wm_base_interface, ver);
         if (!strcmp(intf, "wl_data_device_manager")) globjptr->data_device_manager = (wl_data_device_manager*)wl_registry_bind(reg, id, &wl_data_device_manager_interface, ver);
         if (!strcmp(intf, "wl_subcompositor")) globjptr->subcomp = (wl_subcompositor*)wl_registry_bind(reg, id, &wl_subcompositor_interface, ver);
         if (!strcmp(intf, "zwlr_layer_shell_v1"))   globjptr->layer_shell = (zwlr_layer_shell_v1*)wl_registry_bind(reg, id, &zwlr_layer_shell_v1_interface, ver);
         
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