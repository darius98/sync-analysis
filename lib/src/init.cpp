#include "init.h"

namespace {

struct StaticInitializer {
  explicit StaticInitializer() noexcept {
    syan_init();
  }

  ~StaticInitializer() noexcept {
    syan_shutdown();
  }
};

volatile StaticInitializer initializer;

}
