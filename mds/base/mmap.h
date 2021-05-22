#pragma once
#include "mds/base/file_descriptor.h"
#include <sys/mman.h>
#include <sys/stat.h>
namespace mds::base {

class MMapReader {
public:
  explicit MMapReader(const char *filename) : fd_(filename, O_RDONLY) {
    struct stat s;
    memset(&s, 0, sizeof(s));
    MDS_ENFORCE(fstat(fd_.FD(), &s) == 0) << "cannot stat file " << filename << " errno " << errno;
    len_ = s.st_size;
    beg_ = reinterpret_cast<uint8_t *>(mmap(nullptr, len_, PROT_READ, MAP_SHARED, fd_.FD(), 0));
    MDS_ENFORCE(beg_ != nullptr) << "cannot mmap file " << filename;
  }
  ~MMapReader() {
    if (beg_ != nullptr) {
      munmap(beg_, len_);
    }
  }

  MMapReader(const MMapReader &) = delete;
  MMapReader &operator=(const MMapReader &) = delete;

  [[nodiscard]] const uint8_t *begin() const {
    return beg_;
  }
  [[nodiscard]] const uint8_t *end() const {
    return begin() + len_;
  }

  [[nodiscard]] size_t size() const {
    return end() - begin();
  }

private:
  FileDescriptor fd_;
  uint8_t *beg_;
  uint64_t len_;
};

}