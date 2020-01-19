#ifndef SYNC_ANALYSIS_EXE_EVENT_H_
#define SYNC_ANALYSIS_EXE_EVENT_H_

#include <lib/src/event.h>
#include <lib/src/event_type.h>

namespace syan {

using EventType = ::EventType;
using Event = ::Event;
using ObjectId = decltype(Event::addr);
using ThreadId = decltype(Event::thread_id);

}

#endif
