#include <iostream>
#include <cstring>
#include <type_traits>

#include "../core/format_determiner.h"

#define STDEXT_ARGV_PARSER_WERROR
#include "argparse.h"
#include "thread_pool.h"

using DeterminedFiles = std::unordered_map<std::string_view, formatof::FormatDeterminer::FormatList>;

static constexpr char help[] = 
  "Usage: formatof [options] FILES\n"
  "Display file type data based on file signature (aka \"magic numbers\")\n";
  /*"Options:\n"
  "    --help\n"
  "       Display this message\n"
  "    --rename, -r\n"
  "       Automatically rename file to match one of the possible extentions\n"
  "\n"
  "    --no-rename, -n\n"
  "       Do not display promt to rename file\n"
  "\n"
  "    --format-list, -f\n"
  "       Specify file with additional file format info\n";
  "\n"
  "    --threads, -t\n"
  "       Number of working threads processing files\n";*/

// i dont care
static int should_rename = 0;
static int thread_count = 1;
static const char* format_list_file = nullptr;

DeterminedFiles determine(formatof::FormatDeterminer& formats, char* path) {
  DeterminedFiles res;
  std::string_view pathvw(path);
  std::cout << pathvw;
  std::filesystem::path filepath(pathvw.begin(), pathvw.end());
  try {
    formatof::FormatDeterminer::FormatList fmts = formats.formatOf(filepath);
    if (fmts.size()) {
      std::cout << ": ";
      std::size_t tab = pathvw.size() + 2;
      auto f = fmts.begin();
      std::cout << f->get().toString() << std::endl;
      for (++f; f != fmts.end(); ++f) {
        for (std::size_t i = 0; i < tab; ++i) {
          std::cout << ' '; 
        }
        std::cout << f->get().toString() << std::endl;
      }
      res.emplace(pathvw, std::move(fmts));
      if (should_rename > 0 && fmts.size() == 1 && !fmts[0].get().extensions.empty()) {
        const std::string_view& ext = fmts[0].get().extensions[0];
        std::filesystem::path newname(filepath);
        newname.replace_extension(std::filesystem::path(ext.begin(), ext.end()));
        std::filesystem::rename(filepath, newname);
      }
    } else {
      std::cout << ": unknown file format" << std::endl;
    }      
  } catch (std::runtime_error& e) {
    std::cout << ": " << e.what() << std::endl;
  }
  return res;
}

void promtRename(const DeterminedFiles& determined) {
  std::size_t selected_ind = -1;
  std::size_t i = 0;
  for (const auto& file : determined) {
    if (file.second[0].get().extensions.empty()) {
      std::cout << "No extensions for " << file.first << std::endl;
      continue;
    }
    if (file.second.size() > 1) {
      std::cout << file.first << std::endl;
      for (selected_ind = -1; 0 > selected_ind || selected_ind >= file.second.size();) {
        std::cout << "Select format:" << std::endl;
        for (i = 0; i < file.second.size(); ++i) {
          std::cout << (i + 1) << ". " << file.second[i].get().toString() << std::endl;
        }
        std::cin >> selected_ind;
      }
    } else {
      selected_ind = 1;
    }
    const formatof::Format& selected_fmt = file.second[selected_ind - 1];
    if (selected_fmt.extensions.size() > 1) {
      for (selected_ind = -1; 0 > selected_ind || selected_ind >= selected_fmt.extensions.size();) {
        std::cout << "Select extensions:" << std::endl;
        for (i = 0; i < selected_fmt.extensions.size(); ++i) {
          std::cout << (i + 1) << ". " << selected_fmt.extensions[i] << std::endl;
        }
        std::cin >> selected_ind;
      }
    } else {
      selected_ind = 1;
    }
    const std::string_view ext = selected_fmt.extensions[selected_ind - 1];
    std::filesystem::path newname(file.first);
    newname.replace_extension(std::filesystem::path(ext.begin(), ext.end()));
    std::filesystem::rename(file.first, newname);
  }
}

int main(int argc, char** argv) {
  if (argc < 2) { 
    std::cout << help << std::endl;
    return 1;
  }

  struct argparse argp = argparse_create(argc, argv);

  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'r', "rename"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'n', "no-rename"},
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'f', "format-list"},
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 't', "threads"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "help"},
  };
  static constexpr int optn = sizeof(opts)/sizeof(opts[0]);

  int ind = argparse_long(&argp, opts, optn);
  for (; ind != ARGPARSE_END; ind = argparse_long(&argp, opts, optn)) {
    if (opts[ind].short_name == 'r') {
      should_rename = 1;
    } else if (opts[ind].short_name == 'n') {
      should_rename = -1;
    } else if (opts[ind].short_name == 't') {
      thread_count = std::stoi(std::string(opts[ind].val));
    } else if (!strcmp(opts[ind].long_name, "help")) {
      std::cout << help << std::endl;
      return 2;
    } else if (ind == ARGPARSE_MISSING_VALUE) {
      return ARGPARSE_MISSING_VALUE;
    }
  }
  
  if (argp.optind == argc) {
    std::cout << "formatof: No files specified" << std::endl;
    return 1;
  }

  formatof::FormatDeterminer formats;
  DeterminedFiles determined;
  for (; argp.optind < argc; ++argp.optind) {
    determined.merge(determine(formats, argv[argp.optind]));
  }
  char ans = 0;
  for (; ans != 'y' && ans != 'n' && should_rename == 0;) {
    std::cout << "Rename files?(y/n)";
    std::cin >> ans;
    if (ans == 'y') {
      promtRename(determined);
    }
  }
  return 0;
}