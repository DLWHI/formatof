#ifndef FORMATOF_CORE_ASSERT_H_
#define FORMATOF_CORE_ASSERT_H_

#include <stdexcept>

#define ASSERT(x, e)   \
  if (!(x)) {  \
    throw e;         \
  }

#endif  // FORMATOF_CORE_ASSERT_H_
