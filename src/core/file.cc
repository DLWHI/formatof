#include "file.h"

namespace formatof {
  File::File(const std::filesystem::path& path)
    : hndl_(path, std::ifstream::binary),
      offs_(0),
      path_(path),
      ext_(path.extension()) {
    hndl_.seekg(0, std::ifstream::end);
    lim_ = hndl_.tellg();
    hndl_.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  }

  const std::filesystem::path::string_type& File::path() const noexcept {
    return path_.native();
  } 

  const std::filesystem::path::string_type File::extension() const noexcept {
    return ext_.native();
  }

  bool File::signatureMatches(const Format& format) {
    if (lim_ < format.offset + format.signature.size()) {
      return false;
    }
    if (format.signature.size() > sig_.size() || format.offset != offs_) {
      resizeSignature(format.signature.size(), format.offset);
    }
    return std::equal(format.signature.begin(), format.signature.end(), sig_.begin());
  }

  void File::resizeSignature(std::size_t size, std::ptrdiff_t offs) {
    hndl_.seekg(offs, std::ifstream::beg);
    sig_.resize(size);
    hndl_.read(sig_.data(), sig_.size());
    offs_ = offs;
  }
}  // namespace formatof
