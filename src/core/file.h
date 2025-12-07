#ifndef FORMATOF_CORE_FILE_H_
#define FORMATOF_CORE_FILE_H_

#include <fstream>
#include <filesystem>
#include <mutex>

#include "format.h"

namespace formatof {
  class File final {
   public:
    File(const std::filesystem::path& path);

    const std::filesystem::path::string_type& path() const noexcept;    
    const std::filesystem::path::string_type extension() const noexcept;    
    bool signatureMatches(const Format& format);
   private:

    void resizeSignature(std::size_t size, std::ptrdiff_t offs);

    std::ifstream hndl_;
    std::size_t lim_;
    std::vector<char> sig_;
    std::ptrdiff_t offs_;
    std::filesystem::path path_;
    std::filesystem::path ext_;
  };
}  // namespace formatof

#endif  // FORMATOF_CORE_FILE_H_
