#include "format_list.h"

#include <algorithm>

#include "assert.h"
#include "file.h"
#include "signature_data.h"

namespace formatof {
  FormatList::FormatList() {
    parseView(std::string_view(sig_data, sig_data_size - 1));
  }

  FormatList::FormatList(const std::filesystem::path& format_file) : FormatList() {
    std::ifstream ff(format_file, std::ifstream::binary);
    static std::string contents;
    ff.seekg(0, std::ifstream::end);
    contents.resize(ff.tellg());
    ff.seekg(0, std::ifstream::beg);
    ff.get(contents.data(), contents.size());
    parseView(std::string_view(contents.data(), contents.size()));
  }

  void FormatList::parseView(const std::string_view& sv) {
    std::string_view str(sv);
    for (;!str.empty(); ) {
      std::size_t off = 0;
      for(; off < str.size() && str[off] != '\n'; ++off) {}
      Format fl = Format::parse(str.substr(0, off));
      data_.emplace(std::move(fl));
      str = str.substr(off + 1);
    } 
  }

  std::vector<std::reference_wrapper<const Format>> FormatList::formatOf(const std::filesystem::path& path) {
    File f(path);
    std::vector<std::reference_wrapper<const Format>> res;
    for (const auto& fdat : data_) {
      if (f.signatureMatches(fdat)) {
        res.push_back(std::ref(fdat));
      }
    }
    return res;
  }
}  // namespace formatof
