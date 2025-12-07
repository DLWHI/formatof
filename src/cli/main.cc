#include <iostream>
#include <cstring>
#include <type_traits>

#include "../core/format_list.h"

static constexpr char help[] = 
  "Usage: formatof [options] FILES\n"
  "Display file type data based on file signature (aka \"magic numbers\")\n";
  /*"Options:\n"
  "    --rename, -r\n"
  "       Automatically rename file to match one of the possible extentions\n"
  "\n"
  "    --no-rename, -n\n"
  "       Do not display promt to rename file\n"
  "\n"
  "    --format-list, -f\n"
  "       Specify file with additional file format info\n";*/


void determine(formatof::FormatList& formats, char* path) {
  std::filesystem::path filepath(path);
  if (std::filesystem::exists(  filepath)) {
    if (!std::filesystem::is_directory(filepath)) {
      std::vector<std::reference_wrapper<const formatof::Format>> fmts = formats.formatOf(filepath);
      std::cout << path;
      if (fmts.size()) {
        std::cout << ": ";
        std::size_t tab = std::strlen(path) + 2;
        auto f = fmts.begin();
        std::cout << f->get().toString() << std::endl;
        for (++f; f != fmts.end(); ++f) {
          for (std::size_t i = 0; i < tab; ++i) {
            std::cout << ' '; 
          }
          std::cout << f->get().toString() << std::endl;
        }
      } else {
        std::cout << ": unknown file format" << std::endl;
      }
    } else {
      std::cout << path << ": is a directory" << std::endl;
    }
  } else {
    std::cout << path << ": no such file or directory" << std::endl;
  }
}

int main(int argc, char** argv) {
    if (argc < 2) {
    std::cout << help << std::endl;
    return 1;
  }

  formatof::FormatList formats;
  
  for (int i = 1; i < argc; ++i) {
    determine(formats, argv[i]);
  }

  return 0;
}