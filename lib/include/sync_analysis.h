#ifndef SYNC_ANALYSIS_H_
#define SYNC_ANALYSIS_H_

#ifdef __cplusplus
extern "C" {
#endif

// Core
void* syan_allocate_event(int event_type);
void syan_capture_allocated_event(void* event, void* addr);
void syan_capture_event(int event_type, void* addr);

// Thread
void* syan_thread_on_create_init();
void syan_thread_on_create_finalize(void* event, void* addr);
void syan_thread_on_create(void* addr);

void syan_thread_on_join(void* addr);
void syan_thread_on_detach(void* addr);

// Mutex
void syan_mutex_on_create(void* addr);
void syan_mutex_on_try_lock(void* addr);
void syan_mutex_before_lock(void* addr);
void syan_mutex_after_lock(void* addr);
void syan_mutex_on_unlock(void* addr);
void syan_mutex_on_destroy(void* addr);

// Recursive mutex
void syan_rec_mutex_on_create(void* addr);
void syan_rec_mutex_on_try_lock(void* addr);
void syan_rec_mutex_before_lock(void* addr);
void syan_rec_mutex_after_lock(void* addr);
void syan_rec_mutex_on_unlock(void* addr);
void syan_rec_mutex_on_destroy(void* addr);

// Read-write lock
void syan_rwlock_on_create(void* addr);
void syan_rwlock_on_try_rd_lock(void* addr);
void syan_rwlock_before_rd_lock(void* addr);
void syan_rwlock_after_rd_lock(void* addr);
void syan_rwlock_on_rd_unlock(void* addr);
void syan_rwlock_on_try_wr_lock(void* addr);
void syan_rwlock_before_wr_lock(void* addr);
void syan_rwlock_after_wr_lock(void* addr);
void syan_rwlock_on_wr_unlock(void* addr);
void syan_rwlock_on_destroy(void* addr);

#ifdef __cplusplus
}
#endif

#endif
