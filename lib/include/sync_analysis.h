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

// Mutex
void syan_mutex_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_before_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_after_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_mutex_on_destroy(void* addr) SYAN_CPP_NOEXCEPT;

// Read-write lock
void syan_rwlock_on_create(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_try_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_before_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_after_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_w_try_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_before_w_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_after_w_lock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_w_unlock(void* addr) SYAN_CPP_NOEXCEPT;
void syan_rwlock_on_destroy(void* addr) SYAN_CPP_NOEXCEPT;

#ifdef __cplusplus
}
#endif

#undef SYAN_CPP_NOEXCEPT

#endif
