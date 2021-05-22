#include "mds/base/file_descriptor.h"
#include "catch2/catch_all.hpp"

namespace mds::base {
TEST_CASE("mds_base_fd") {
  {
    FileDescriptor fd("a.txt", O_WRONLY | O_CREAT);
  }
  {
    FileDescriptor fd("a.txt", O_RDONLY);
    FileDescriptor fd2(std::move(fd));
    fd = std::move(fd2);
  }
}
}