#ifndef SYNC_ANALYSIS_LIB_SRC_INIT_H_
#define SYNC_ANALYSIS_LIB_SRC_INIT_H_

#ifdef __cplusplus
#define SYAN_CPP_NOEXCEPT noexcept
#else
#define SYAN_CPP_NOEXCEPT
#endif

#ifdef __cplusplus
extern "C" {
#endif

void syan_init() SYAN_CPP_NOEXCEPT;

void syan_shutdown() SYAN_CPP_NOEXCEPT;

#ifdef __cplusplus
}
#endif

#undef SYAN_CPP_NOEXCEPT

#endif
