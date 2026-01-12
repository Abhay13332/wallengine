#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include "queue"
#include <cstdint>

#include "includeall.hpp"
enum fi_event_type:uint8_t{
  FI_KEYBOARD_PRESSED,
  FI_KEYBOARD_RELEASED,
  FI_MOUSE,
} ;
struct fi_event_data{};
struct fi_event_modifier_state{
   bool alt;
   bool ctrl;
   bool shift;
   bool super;
   bool caps_lock;
} __attribute__((aligned(8))) ;
struct fi_event_keyboard_data:fi_event_data{
   std::string keypressed;
   fi_event_modifier_state modifier_states;

} __attribute__((aligned(64)));
struct fi_event{
  fi_event_type eventtype;
 std::unique_ptr<fi_event_data> Data;


} __attribute__((aligned(16)));
class fi_EventQueue{
  private:
    std::queue<fi_event> qu;

  public:
    void addEvent(fi_event event);
    fi_event doEvent();
      
};
#endif