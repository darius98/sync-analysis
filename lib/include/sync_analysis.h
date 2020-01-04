#ifndef SYNC_ANALYSIS_H_
#define SYNC_ANALYSIS_H_

#ifdef __cplusplus
#define SYAN_CPP_NOEXCEPT noexcept
#else
#define SYAN_CPP_NOEXCEPT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SYNC_ANALYSIS_ENABLED) && SYNC_ANALYSIS_ENABLED

// Thread
void syan_thread_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_thread_on_join(void* addr) SYAN_CPP_NOEXCEPT;
void syan_thread_on_detach(void* addr) SYAN_CPP_NOEXCEPT;

// Mutex
void syan_mutex_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_before_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_after_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_destroy(void* addr) SYAN_CPP_NOEXCEPT;

// Recursive mutex
void syan_rec_mutex_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rec_mutex_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rec_mutex_before_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rec_mutex_after_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rec_mutex_on_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rec_mutex_on_destroy(void* addr) SYAN_CPP_NOEXCEPT;

// Read-write lock
void syan_rwlock_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_try_rd_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_before_rd_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_after_rd_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_rd_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_try_wr_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_before_wr_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_after_wr_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_wr_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_destroy(void* addr) SYAN_CPP_NOEXCEPT;

#else

// Thread
inline void syan_thread_on_create(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_thread_on_join(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_thread_on_detach(void* addr) SYAN_CPP_NOEXCEPT {}

// Mutex
inline void syan_mutex_on_create(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_mutex_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_mutex_before_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_mutex_after_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_mutex_on_unlock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_mutex_on_destroy(void* addr) SYAN_CPP_NOEXCEPT {}

// Recursive mutex
inline void syan_rec_mutex_on_create(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rec_mutex_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rec_mutex_before_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rec_mutex_after_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rec_mutex_on_unlock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rec_mutex_on_destroy(void* addr) SYAN_CPP_NOEXCEPT {}

// Read-write lock
inline void syan_rwlock_on_create(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_on_try_rd_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_before_rd_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_after_rd_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_on_rd_unlock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_on_try_wr_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_before_wr_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_after_wr_lock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_on_wr_unlock(void* addr) SYAN_CPP_NOEXCEPT {}
inline void syan_rwlock_on_destroy(void* addr) SYAN_CPP_NOEXCEPT {}

#endif

#ifdef __cplusplus
}
#endif

#undef SYAN_CPP_NOEXCEPT

#endif
