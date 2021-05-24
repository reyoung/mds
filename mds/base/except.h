#pragma once
#include "exceptxx/BaseExceptionImpl.h"
#include "exceptxx/ThrowHelper.h"
#include "exceptxx/AssertException.h"
#include <string>

namespace mds::base {

class BaseException : public exceptxx::BaseExceptionImpl<BaseException> {
public:
  using Error = std::string;

  BaseException(Error error, const char *func,
                size_t line, std::string &&message) : BaseExceptionImpl(func,
                                                                        line,
                                                                        std::move(message)),
                                                      error_(std::move(error)) {
  }

  const char *tag() const override {
    return "ENFORCE";
  }

  std::string error() const override {
    return error_;
  }

  std::string description() const override {
    return "mds error";
  }

private:
  std::string error_;
};

}

#define MDS_ENFORCE(cond) \
  if (!(cond)) exceptxx::ThrowHelper<mds::base::BaseException>(#cond, EXCEPTXX_GET_FUNC(), __LINE__) <<= std::ostringstream()

#define MDS_THROW() \
  exceptxx::ThrowHelper<mds::base::BaseException>("throw", EXCEPTXX_GET_FUNC(), __LINE__) <<= std::ostringstream()