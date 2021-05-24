#pragma once

#include <cstring>
#include <cstdint>
#include <iostream>
#include "mds/base/except.h"
#include <string_view>

namespace mds::base {

template<size_t Sel>
struct Simple8BTable {
};

template<>
struct Simple8BTable<0> {
  enum {
    N = 240,
    NBits = 0,
  };
  template<typename ConstIterator>
  static uint64_t Pack(ConstIterator begin, ConstIterator end) {
    return 0;
  }

  template<typename Container>
  static void Unpack(Container *dst, uint64_t val) {
    for (int i = 0; i < N; ++i) {
      dst->push_back(1UL);
    }
  }
};

template<>
struct Simple8BTable<1> {
  enum {
    N = 120,
    NBits = 0,
  };
  template<typename ConstIterator>
  static uint64_t Pack(ConstIterator container, ConstIterator end) {
    return 1UL << 60;
  }

  template<typename Container>
  static void Unpack(Container *dst, uint64_t val) {
    for (int i = 0; i < N; ++i) {
      dst->push_back(1UL);
    }
  }
};

template<size_t Sel, size_t N_, size_t NBits_>
struct Simple8BTableBase {
  enum {
    N = N_,
    NBits = NBits_,
  };
  template<typename ConstIterator>
  static uint64_t Pack(ConstIterator beg, ConstIterator end) {
    uint64_t val = Sel << 60;

    for (size_t i = 0; i < N_ && beg != end; ++i) {
      val |= (*beg) << (i * NBits);
      ++beg;
    }
    return val;
  }

  template<typename Container>
  static void Unpack(Container *dst, uint64_t val) {
    uint64_t mask = (0x01UL << NBits_) - 1;
    for (int i = 0; i < N; ++i) {
      dst->push_back(val & mask);
      val >>= NBits_;
    }
  }

};

#define ImplementSimple8BTable(sel, n, nbits) \
  template<>\
  struct Simple8BTable<sel> : public Simple8BTableBase<sel, n, nbits> {}

ImplementSimple8BTable(2, 60, 1);
ImplementSimple8BTable(3, 30, 2);
ImplementSimple8BTable(4, 20, 3);
ImplementSimple8BTable(5, 15, 4);
ImplementSimple8BTable(6, 12, 5);
ImplementSimple8BTable(7, 10, 6);
ImplementSimple8BTable(8, 8, 7);
ImplementSimple8BTable(9, 7, 8);
ImplementSimple8BTable(10, 6, 10);
ImplementSimple8BTable(11, 5, 12);
ImplementSimple8BTable(12, 4, 15);
ImplementSimple8BTable(13, 3, 20);
ImplementSimple8BTable(14, 2, 30);
ImplementSimple8BTable(15, 1, 60);

#undef ImplementSimple8BTable

template<size_t I, typename ConstIterator>
struct Simple8BEncodeLooper {
  Simple8BEncodeLooper(std::ostream &os, ConstIterator begin, ConstIterator end) : os_(os), begin_(begin), end_(end) {}

  ConstIterator operator()() {
    if (!canPack()) {
      return Simple8BEncodeLooper<I + 1, ConstIterator>(os_, begin_, end_)();
    }

    using table = Simple8BTable<I>;
    uint64_t val = table::Pack(begin_, end_);
    os_.write(reinterpret_cast<const char *>(&val), sizeof(val));
    begin_ += table::N;
    if (begin_ >= end_) {
      return end_;
    } else {
      return begin_;
    }
  }

private:
  [[nodiscard]] bool canPack() const {
    size_t size = end_ - begin_;
    using table = Simple8BTable<I>;
    if (size < table::N) {
      return false;
    }

    auto end = end_;
    if (size > table::N) {
      end = begin_ + table::N;
    }

    if (table::NBits == 0) {
      return std::all_of(begin_, end, [](uint64_t v) -> bool {
        return v == 1;
      });
    }
    uint64_t max = (1UL << table::NBits) - 1;
    return std::all_of(begin_, end, [&](uint64_t v) -> bool {
      return v <= max;
    });
  }

  std::ostream &os_;
  ConstIterator begin_;
  ConstIterator end_;
};

template<typename ConstIterator>
struct Simple8BEncodeLooper<16, ConstIterator> {
  Simple8BEncodeLooper(std::ostream &os, ConstIterator begin, ConstIterator end) : begin_(begin) {}
  ConstIterator operator()() {
    MDS_THROW() << "the value in src is too large, which is " << *begin_;
  }
  ConstIterator begin_;
};

template<typename Container>
inline void EncodeSimple8B(std::ostream &os, const Container &container) {
  auto begin = container.begin();
  auto end = container.end();
  while (begin != end) {
    begin = Simple8BEncodeLooper<0, decltype(begin)>(os, begin, end)();
  }
}

template<size_t I, typename Container>
struct Simple8BDecoderLooper {
  Simple8BDecoderLooper(Container *container, uint64_t val) : container_(container), val_(val) {
  }

  void operator()() {
    uint64_t sel = val_ >> 60;
    if (sel != I) {
      Simple8BDecoderLooper<I + 1, Container>(container_, val_)();
      return;
    }

    using table = Simple8BTable<I>;
    table::Unpack(container_, val_);
  }

  Container *container_;
  uint64_t val_;
};

template<typename Container>
struct Simple8BDecoderLooper<16, Container> {
  Simple8BDecoderLooper(Container *container, uint64_t val) {
  }
  void operator()() {
    MDS_THROW() << "invalid selector value 16";
  }
};

template<typename Container>
inline void DecodeSimple8B(Container *container, std::string_view buf) {
  MDS_ENFORCE(buf.size() % sizeof(uint64_t) == 0) << "buffer should contains uint64_t";
  while (!buf.empty()) {
    uint64_t tmp = *reinterpret_cast<const uint64_t *>(buf.data());
    Simple8BDecoderLooper<0, Container>(container, tmp)();
    buf = buf.substr(sizeof(uint64_t));
  }
}

}