#ifndef SYNC_ANALYSIS_EXE_CHECK_H_
#define SYNC_ANALYSIS_EXE_CHECK_H_

#include <type_traits>

#include "environment.hpp"
#include "event.hpp"
#include "internal_check_registerer.hpp"

namespace syan {

class Check {
public:
  virtual ~Check() = default;

  virtual void on_start(const Environment&) {}

  virtual void on_event(const Environment& env, Event event) = 0;

  virtual void on_end(const Environment&) {}
};

} // namespace syan

#define INTERNAL_SYAN_REGISTER_CHECK_ID2(ID1, ID2) ID1##ID2
#define INTERNAL_SYAN_REGISTER_CHECK_ID(ID1, ID2)                              \
  INTERNAL_SYAN_REGISTER_CHECK_ID2(ID1, ID2)
#define SYAN_REGISTER_CHECK(DerivedCheck)                                      \
  static_assert(::std::is_base_of_v<::syan::Check, DerivedCheck>,              \
                "Cannot register " #DerivedCheck                               \
                " as a check because it is not a subclass of ::syan::Check."); \
  static_assert(!::std::is_abstract_v<DerivedCheck>,                           \
                "Cannot register " #DerivedCheck                               \
                " as a check because it does not implement "                   \
                "::syan::Check::on_event() method.");                          \
  ::syan::internal::Registerer<DerivedCheck> INTERNAL_SYAN_REGISTER_CHECK_ID(  \
      DerivedCheck, _registerer) {}

#endif
