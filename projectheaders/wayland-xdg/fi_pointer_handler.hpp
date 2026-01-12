#ifndef FI_POINTER_H
#define FI_POINTER_H
#include "wayland-xdg/eventqueue.hpp"
#include <includeall.hpp>
#include <wayland-client-protocol.h>
class fi_pointer_listener{
    wl_pointer_listener pointer_listener;
    fi_EventQueue *queue;
    public:
	int x, y;
     void setup(wl_pointer *wl_pointer){
      
        pointer_listener.motion=[](void * data, struct wl_pointer *wl_pointer, uint32_t timestamp, wl_fixed_t x_cord , wl_fixed_t y_cord)->void{
            fi_pointer_listener* instance=static_cast<fi_pointer_listener*>(data);
			std::cout <<"pointer "<< x_cord <<" "<< y_cord << "\n"; 
			instance->x=x_cord;
			instance->y=y_cord;
        };
        pointer_listener.axis=[](void *, struct wl_pointer *, uint32_t, uint32_t, wl_fixed_t)->void{};
        pointer_listener.axis_relative_direction=[](void *data,
					struct wl_pointer *wl_pointer,
					uint32_t axis,
					uint32_t direction)->void{};
        pointer_listener.axis_discrete =[](void *data,
			      struct wl_pointer *wl_pointer,
			      uint32_t axis,
			      int32_t discrete)->void{};
        pointer_listener.axis_source =[](void *data,
			    struct wl_pointer *wl_pointer,
			    uint32_t axis_source) ->void{}; 
        pointer_listener.axis_stop=[](void *data,
			  struct wl_pointer *wl_pointer,
			  uint32_t time,
			  uint32_t axis)->void{};
        pointer_listener.button=[](void *data,
		       struct wl_pointer *wl_pointer,
		       uint32_t serial,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state)->void{};
        pointer_listener.axis_value120= [](void *data,
			      struct wl_pointer *wl_pointer,
			      uint32_t axis,
			      int32_t value120)->void{};
        pointer_listener.enter=[](void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface,
		      wl_fixed_t surface_x,
		      wl_fixed_t surface_y)->void{};
        pointer_listener.frame=[](void *data,
		      struct wl_pointer *wl_pointer)->void{};   
         pointer_listener.leave=[](void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface) ->void{};                          
        wl_pointer_add_listener(wl_pointer, &pointer_listener, this);
        

     }
};
#endif