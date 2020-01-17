#ifndef SYNC_ANALYSIS_EXE_CHECK_H_
#define SYNC_ANALYSIS_EXE_CHECK_H_

#include <lib/src/event.h>

#include "environment.hpp"
#include "internal_check_registerer.hpp"

namespace syan {

class Check {
public:
  virtual ~Check() = default;

  virtual void on_event(const Environment& environment, const Event& event) = 0;
};

} // namespace syan

#define INTERNAL_SYAN_REGISTER_CHECK_ID2(ID1, ID2) ID1##ID2
#define INTERNAL_SYAN_REGISTER_CHECK_ID(ID1, ID2)                              \
  INTERNAL_SYAN_REGISTER_CHECK_ID2(ID1, ID2)
#define SYAN_REGISTER_CHECK(DerivedCheck)                                      \
  ::syan::internal::Registerer<DerivedCheck> INTERNAL_SYAN_REGISTER_CHECK_ID(  \
      DerivedCheck, _registerer) {}

#endif
