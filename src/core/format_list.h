#ifndef FORMATOF_CORE_FORMAT_LIST_H_
#define FORMATOF_CORE_FORMAT_LIST_H_

#include <filesystem>
#include <functional>
#include <set>

#include "format.h"

namespace formatof {
  class FormatList final {
   public:
    FormatList();
    FormatList(const std::filesystem::path& format_file);

    std::vector<std::reference_wrapper<const Format>> formatOf(const std::filesystem::path& path);
   private:
    void parseView(const std::string_view& sv);

    std::multiset<Format, FormatDescendComparator> data_;
  };
}  // namespace formatof

#endif  // FORMATOF_CORE_FORMAT_LIST_H_
