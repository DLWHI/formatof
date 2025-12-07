#ifndef FORMATOF_CORE_FORMAT_H_
#define FORMATOF_CORE_FORMAT_H_

#include <string>
#include <string_view>
#include <vector>


namespace formatof {
  struct Format final {
    using ExtensionsList = std::vector<std::string_view>;

    static Format parse(const std::string_view& exts);

    std::string toString() const;    

    std::string_view name;
    std::vector<char> signature;
    ptrdiff_t offset;
    ExtensionsList extensions;
  };

  struct FormatDescendComparator final {
    bool operator()(const Format& lhs, const Format& rhs) const noexcept {
      return lhs.signature.size() > rhs.signature.size();
    }
  };
}  // namespace formatof

#endif  // FORMATOF_CORE_FORMAT_H_
