#pragma once
#include <vector>
#include <fstream>
#include "mds/base/except.h"
namespace mds::strlist {

class Dumper {
public:
  explicit Dumper(const char *filename) : f_(filename, std::ios::trunc) {
    MDS_ENFORCE(f_.is_open()) << "cannot open " << filename << "to write";
    pos_.emplace_back(0);
  }

  void Append(std::string_view buf) {
    uint32_t bufSize = buf.size();
    f_.write(buf.data(), bufSize);
    pos_.emplace_back(pos_.back() + bufSize);
  }

  void Complete() {
    for (auto &pos: pos_) {
      f_.write(reinterpret_cast<const char *>(&pos), sizeof(pos));
    }
    uint32_t nPos = pos_.size();
    f_.write(reinterpret_cast<const char *>(&nPos), sizeof(nPos));
    f_.close();
  }

private:
  std::ofstream f_;
  std::vector<uint64_t> pos_;
};

}