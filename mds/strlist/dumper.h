#pragma once
#include <vector>
#include <fstream>
#include "mds/base/except.h"
#include "mds/base/simple8b.h"
namespace mds::strlist {

class Dumper {
public:
  explicit Dumper(const char *filename) : f_(filename, std::ios::trunc) {
    MDS_ENFORCE(f_.is_open()) << "cannot open " << filename << "to write";
  }

  void Append(std::string_view buf) {
    uint32_t bufSize = buf.size();
    f_.write(buf.data(), bufSize);
    pos_.emplace_back(bufSize);
  }

  void Complete() {
    auto before = f_.tellp();
    base::EncodeSimple8B(f_, pos_);
    auto after = f_.tellp();
    uint32_t bufSize = after - before;
    f_.write(reinterpret_cast<const char *>(&bufSize), sizeof(bufSize));
    f_.close();
  }

private:
  std::ofstream f_;
  std::vector<uint64_t> pos_;
};

}