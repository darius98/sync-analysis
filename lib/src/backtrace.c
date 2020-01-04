#include "backtrace.h"

#include <libunwind.h>

void syan_get_backtrace(intptr_t* backtrace) {
  int frames = 0;
  unw_cursor_t unwind_cursor;
  unw_context_t unwind_context;

  unw_getcontext(&unwind_context);
  unw_init_local(&unwind_cursor, &unwind_context);
  while (frames < 12 && unw_step(&unwind_cursor) > 0) {
    intptr_t ip;
    if (unw_get_reg(&unwind_cursor, UNW_REG_IP, (unw_word_t*)&ip) < 0) {
      break;
    }
    backtrace[frames++] = ip;
  }
}
