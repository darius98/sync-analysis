#ifndef SYNC_ANALYSIS_EXE_LIB_COMPAT_H_
#define SYNC_ANALYSIS_EXE_LIB_COMPAT_H_

#include <lib/src/dump_file_header.h>
#include <lib/src/event.h>
#include <lib/src/event_type.h>

namespace syan {

using DumpFileHeader = ::SyanDumpFileHeader;

using EventType = ::SyanEventType;
using ObjectId = decltype(::SyanEvent::addr);

using RawBacktrace = intptr_t (&)[12];

} // namespace syan

#endif
