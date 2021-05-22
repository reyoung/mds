#include "mds/strlist/reader.h"
#include "mds/strlist/dumper.h"
#include "catch2/catch_all.hpp"

namespace mds::strlist {
TEST_CASE("mds_strlist") {
  {
    Dumper dumper("a.strlist");
    dumper.Append("ok");
    dumper.Append("ok2");
    dumper.Complete();
  }
  {
    Reader reader("a.strlist");
    REQUIRE(reader.size() == 2);
    REQUIRE(reader[0] == "ok");
    REQUIRE(reader[1] == "ok2");
  }
}
}