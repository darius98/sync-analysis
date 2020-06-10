#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_EVENT_TYPE_H_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_EVENT_TYPE_H_

#include <sync_analysis.h>

namespace sync {

enum SyanEventTypeFlags {
  SA_EV_THREAD = 1u << 0,
  SA_EV_MUTEX = 1u << 1,
  SA_EV_REC_MUTEX = 1u << 2,
  SA_EV_RWLOCK = 1u << 3,

  SA_EV_CREATE = 1u << 4,
  SA_EV_TRY_WR_LOCK = 1u << 5,
  SA_EV_BEFORE_WR_LOCK = 1u << 6,
  SA_EV_AFTER_WR_LOCK = 1u << 7,
  SA_EV_WR_UNLOCK = 1u << 8,
  SA_EV_TRY_RD_LOCK = 1u << 9,
  SA_EV_BEFORE_RD_LOCK = 1u << 10,
  SA_EV_AFTER_RD_LOCK = 1u << 11,
  SA_EV_RD_UNLOCK = 1u << 12,
  SA_EV_DESTROY = 1u << 13,
  SA_EV_JOIN = 1u << 14,
  SA_EV_DETACH = 1u << 15,
};

enum SyanEventType {
  SA_EV_THREAD_ON_CREATE = SA_EV_THREAD | SA_EV_CREATE,
  SA_EV_THREAD_ON_JOIN = SA_EV_THREAD | SA_EV_JOIN,
  SA_EV_THREAD_ON_DETACH = SA_EV_THREAD | SA_EV_DETACH,

  SA_EV_MUTEX_ON_CREATE = SA_EV_MUTEX | SA_EV_CREATE,
  SA_EV_MUTEX_ON_TRY_LOCK = SA_EV_MUTEX | SA_EV_TRY_WR_LOCK,
  SA_EV_MUTEX_BEFORE_LOCK = SA_EV_MUTEX | SA_EV_BEFORE_WR_LOCK,
  SA_EV_MUTEX_AFTER_LOCK = SA_EV_MUTEX | SA_EV_AFTER_WR_LOCK,
  SA_EV_MUTEX_ON_UNLOCK = SA_EV_MUTEX | SA_EV_WR_UNLOCK,
  SA_EV_MUTEX_ON_DESTROY = SA_EV_MUTEX | SA_EV_DESTROY,

  SA_EV_REC_MUTEX_ON_CREATE = SA_EV_REC_MUTEX | SA_EV_CREATE,
  SA_EV_REC_MUTEX_ON_TRY_LOCK = SA_EV_REC_MUTEX | SA_EV_TRY_WR_LOCK,
  SA_EV_REC_MUTEX_BEFORE_LOCK = SA_EV_REC_MUTEX | SA_EV_BEFORE_WR_LOCK,
  SA_EV_REC_MUTEX_AFTER_LOCK = SA_EV_REC_MUTEX | SA_EV_AFTER_WR_LOCK,
  SA_EV_REC_MUTEX_ON_UNLOCK = SA_EV_REC_MUTEX | SA_EV_WR_UNLOCK,
  SA_EV_REC_MUTEX_ON_DESTROY = SA_EV_REC_MUTEX | SA_EV_DESTROY,

  SA_EV_RWLOCK_ON_CREATE = SA_EV_RWLOCK | SA_EV_CREATE,
  SA_EV_RWLOCK_ON_TRY_RD_LOCK = SA_EV_RWLOCK | SA_EV_TRY_RD_LOCK,
  SA_EV_RWLOCK_BEFORE_RD_LOCK = SA_EV_RWLOCK | SA_EV_BEFORE_RD_LOCK,
  SA_EV_RWLOCK_AFTER_RD_LOCK = SA_EV_RWLOCK | SA_EV_AFTER_RD_LOCK,
  SA_EV_RWLOCK_ON_RD_UNLOCK = SA_EV_RWLOCK | SA_EV_RD_UNLOCK,
  SA_EV_RWLOCK_ON_TRY_WR_LOCK = SA_EV_RWLOCK | SA_EV_TRY_WR_LOCK,
  SA_EV_RWLOCK_BEFORE_WR_LOCK = SA_EV_RWLOCK | SA_EV_BEFORE_WR_LOCK,
  SA_EV_RWLOCK_AFTER_WR_LOCK = SA_EV_RWLOCK | SA_EV_AFTER_WR_LOCK,
  SA_EV_RWLOCK_ON_WR_UNLOCK = SA_EV_RWLOCK | SA_EV_WR_UNLOCK,
  SA_EV_RWLOCK_ON_DESTROY = SA_EV_RWLOCK | SA_EV_DESTROY,
};

static_assert(SA_EV_THREAD_ON_CREATE == SYAN_THREAD_CREATE_EVENT);

// Thread

inline void* syan_thread_on_create_init() {
  return syan_initialize_event(SA_EV_THREAD_ON_CREATE);
}

inline void syan_thread_on_create_finalize(void* event, void* addr) {
  syan_finalize_event(event, addr);
}

inline void syan_thread_on_create(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_CREATE, addr);
}

inline void syan_thread_on_join(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_JOIN, addr);
}

inline void syan_thread_on_detach(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_DETACH, addr);
}

// Mutex

inline void syan_mutex_on_create(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_CREATE, addr);
}

inline void syan_mutex_on_try_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_TRY_LOCK, addr);
}

inline void syan_mutex_before_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_BEFORE_LOCK, addr);
}

inline void syan_mutex_after_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, addr);
}

inline void syan_mutex_on_unlock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_UNLOCK, addr);
}

inline void syan_mutex_on_destroy(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_DESTROY, addr);
}

// Recursive mutex

inline void syan_rec_mutex_on_create(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_CREATE, addr);
}

inline void syan_rec_mutex_on_try_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_TRY_LOCK, addr);
}

inline void syan_rec_mutex_before_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_BEFORE_LOCK, addr);
}

inline void syan_rec_mutex_after_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_AFTER_LOCK, addr);
}

inline void syan_rec_mutex_on_unlock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_UNLOCK, addr);
}

inline void syan_rec_mutex_on_destroy(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_DESTROY, addr);
}

// Read-write lock

inline void syan_rwlock_on_create(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_CREATE, addr);
}

inline void syan_rwlock_on_try_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, addr);
}

inline void syan_rwlock_before_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, addr);
}

inline void syan_rwlock_after_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, addr);
}

inline void syan_rwlock_on_rd_unlock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_RD_UNLOCK, addr);
}

inline void syan_rwlock_on_try_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, addr);
}

inline void syan_rwlock_before_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, addr);
}

inline void syan_rwlock_after_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, addr);
}

inline void syan_rwlock_on_wr_unlock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_WR_UNLOCK, addr);
}

inline void syan_rwlock_on_destroy(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_DESTROY, addr);
}

}  // namespace sync

#endif
