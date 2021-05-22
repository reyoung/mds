#pragma once
#include "mds/base/mmap.h"
#include <vector>
namespace mds::strlist {

class Reader {
public:
  explicit Reader(const char *filename) : reader_(filename) {
    nPos_ = *reinterpret_cast<const uint32_t *>(reader_.end() - sizeof(uint32_t));
    pos_ = reinterpret_cast<const uint64_t * >(reader_.end() - sizeof(uint32_t) - nPos_ * sizeof(uint64_t));
  }

  [[nodiscard]] size_t size() const {
    return nPos_ - 1;
  }

  [[nodiscard]] std::string_view operator[](size_t i) const {
    const char *beg = reinterpret_cast<const char *>(reader_.begin() + pos_[i]);
    size_t len = pos_[i + 1] - pos_[i];
    return std::string_view(beg, len);
  }

private:
  base::MMapReader reader_;
  const uint64_t *pos_;
  uint32_t nPos_;

};

}