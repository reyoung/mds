#pragma once
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include "mds/base/except.h"
namespace mds::base {

class FileDescriptor {
public:
  FileDescriptor(const char *filename, int opt) : fd_(open(filename, opt)) {
    MDS_ENFORCE(fd_ > 0) << "cannot open " << filename << " with option " << opt << " errno(" << errno
                         << ")";
  }

  ~FileDescriptor() {
    tryClose();
  }
  FileDescriptor(const FileDescriptor &) = delete;
  FileDescriptor &operator=(const FileDescriptor &) = delete;

  FileDescriptor(FileDescriptor &&o) noexcept: fd_(o.fd_) {
    o.fd_ = 0;
  }

  FileDescriptor &operator=(FileDescriptor &&o) noexcept {
    tryClose();
    fd_ = o.fd_;
    o.fd_ = 0;
    return *this;
  }

  [[nodiscard]] int FD() const {
    return fd_;
  }

  explicit operator int() const {
    return fd_;
  }

private:
  void tryClose() {
    if (fd_ > 0) {
      close(fd_);
    }
  }

  int fd_;
};
}