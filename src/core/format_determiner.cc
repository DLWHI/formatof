#include "format_determiner.h"

#include <algorithm>

#include "assert.h"
#include "file.h"
#include "signature_data.h"

namespace formatof {
  FormatDeterminer::FormatDeterminer() 
      : kFormatBlock({"Block device", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatCharFile({"Character device", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatDirectory({"Directory", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatEmpty({"Empty file", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatPipe({"Named pipe", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatSocket({"Socket", std::vector<char>(), 0, Format::ExtensionList()}),
        kFormatSymlink({"Symbolic link", std::vector<char>(), 0, Format::ExtensionList()}) {
    parseView(std::string_view(sig_data, sig_data_size - 1));
  }

  FormatDeterminer::FormatDeterminer(const std::filesystem::path& format_file) : FormatDeterminer() {
    std::ifstream ff(format_file, std::ifstream::binary);
    static std::string contents;
    ff.seekg(0, std::ifstream::end);
    contents.resize(ff.tellg());
    ff.seekg(0, std::ifstream::beg);
    ff.get(contents.data(), contents.size());
    parseView(std::string_view(contents.data(), contents.size()));
  }

  void FormatDeterminer::parseView(const std::string_view& sv) {
    std::string_view str(sv);
    for (;!str.empty(); ) {
      std::size_t off = 0;
      for(; off < str.size() && str[off] != '\n'; ++off) {}
      Format fl = Format::parse(str.substr(0, off));
      data_.emplace(std::move(fl));
      str = str.substr(off + 1);
    } 
  }

  FormatDeterminer::FormatList FormatDeterminer::formatOf(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
      throw std::runtime_error("file does not exist");
    }
    FormatList res;
    checkSystemTypes(path, res);
    if (!res.empty()) {
      return res;
    }
    File f(path);
    for (const auto& fdat : data_) {
      if (f.signatureMatches(fdat)) {
        res.push_back(std::ref(fdat));
      }
    }
    return res;
  }
  
  void FormatDeterminer::checkSystemTypes(const std::filesystem::path& filepath, FormatList& out) {
    if (std::filesystem::is_block_file(filepath)) {
      out.push_back(kFormatBlock);
    } else if (std::filesystem::is_character_file(filepath)) {
      out.push_back(kFormatCharFile);
    } else if (std::filesystem::is_directory(filepath)) {
      out.push_back(kFormatDirectory);
    } else if (std::filesystem::is_empty(filepath)) {
      out.push_back(kFormatEmpty);
    } else if (std::filesystem::is_fifo(filepath)) {
      out.push_back(kFormatPipe);
    } else if (std::filesystem::is_socket(filepath)) {
      out.push_back(kFormatSocket);
    } else if (std::filesystem::is_symlink(filepath)) {
      out.push_back(kFormatSymlink);
    }
  }
  
}  // namespace formatof
