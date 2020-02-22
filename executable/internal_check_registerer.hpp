#ifndef SYNC_ANALYSIS_EXE_INTERNAL_CHECK_REGISTERER_H_
#define SYNC_ANALYSIS_EXE_INTERNAL_CHECK_REGISTERER_H_

namespace syan {

class Check;

}

namespace syan::internal {

struct RegisteredCheck {
  Check* check;
  RegisteredCheck* next_check;

  static RegisteredCheck*& get_head() noexcept {
    static RegisteredCheck* head{nullptr};
    return head;
  }
};

template<class DerivedCheck>
class Registerer {
  DerivedCheck check{};

public:
  Registerer() noexcept {
    auto& head = RegisteredCheck::get_head();
    static RegisteredCheck registered_check{&check, head};
    head = &registered_check;
  }
};

}  // namespace syan::internal

#endif
