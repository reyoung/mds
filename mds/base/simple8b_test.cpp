#include "mds/base/simple8b.h"
#include "catch2/catch_all.hpp"
#include <sstream>
namespace mds::base {
TEST_CASE("simple8b") {
  std::vector<uint64_t> vec{1, 2, 3, 4, 5, 17, 81};
  std::ostringstream oss;
  EncodeSimple8B(oss, vec);
  auto result = oss.str();
  REQUIRE(result.size() == sizeof(uint64_t));

  std::vector<uint64_t> tmp;
  DecodeSimple8B(&tmp, result);
  REQUIRE(vec == tmp);
}
}