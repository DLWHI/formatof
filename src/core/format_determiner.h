#ifndef FORMATOF_CORE_FORMAT_DETERMINER_H_
#define FORMATOF_CORE_FORMAT_DETERMINER_H_

#include <filesystem>
#include <functional>
#include <set>

#include "format.h"

namespace formatof {
  class FormatDeterminer final {
   public:
    using FormatList = std::vector<std::reference_wrapper<const Format>>;
    FormatDeterminer();
    FormatDeterminer(const std::filesystem::path& format_file);

    FormatList formatOf(const std::filesystem::path& path);
   private:
    
    const Format kFormatBlock;
    const Format kFormatCharFile;
    const Format kFormatDirectory;
    const Format kFormatEmpty;
    const Format kFormatPipe;
    const Format kFormatSocket;
    const Format kFormatSymlink;
    
    void checkSystemTypes(const std::filesystem::path& filepath, FormatList& out);
    void parseView(const std::string_view& sv);

    std::multiset<Format, FormatDescendComparator> data_;
  };
}  // namespace formatof

#endif  // #define FORMATOF_CORE_FORMAT_DETERMINER_H_

