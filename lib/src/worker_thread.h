#ifndef SYNC_ANALYSIS_LIB_SRC_WORKER_THREAD_H_
#define SYNC_ANALYSIS_LIB_SRC_WORKER_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

int syan_start_worker_thread(void* output_fd_thread_param);

int syan_stop_worker_thread();

#ifdef __cplusplus
}
#endif

#endif
