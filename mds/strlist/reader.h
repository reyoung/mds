#pragma once
#include "mds/base/mmap.h"
#include "mds/base/simple8b.h"
#include <vector>
namespace mds::strlist {

class Reader {
public:
  explicit Reader(const char *filename) : reader_(filename) {
    uint32_t endOffset = *reinterpret_cast<const uint32_t *>(reader_.end() - sizeof(uint32_t));
    pos_.emplace_back(0);
    std::string_view buf(reinterpret_cast<const char *>(reader_.end() - endOffset - sizeof(uint32_t)), endOffset);
    base::DecodeSimple8B(&pos_, buf);
    for (size_t i = 0; i < pos_.size() - 1; i++) {
      pos_[i + 1] += pos_[i];
    }
  }

  [[nodiscard]] size_t size() const {
    return pos_.size() - 1;
  }

  [[nodiscard]] std::string_view operator[](size_t i) const {
    const char *beg = reinterpret_cast<const char *>(reader_.begin() + pos_[i]);
    size_t len = pos_[i + 1] - pos_[i];
    return std::string_view(beg, len);
  }

private:
  base::MMapReader reader_;
  std::vector<uint64_t> pos_;

};

}