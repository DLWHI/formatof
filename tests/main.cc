#include <gtest/gtest.h>

#include "format_list.h"

struct FormatOfTestEnv : ::testing::Test {
  FormatOfTestEnv() : Test() {}

  void SetUp() {
    list = new formatof::FormatList();

    std::filesystem::path resoruce_dir(FORMATOF_RESOURCE_DIR);

    for (const auto& en : std::filesystem::directory_iterator(resoruce_dir)) {
      files.push_back(en.path());
    }
  }

  void TearDown() {
    delete list;
  }

  formatof::FormatList* list;
  std::vector<std::filesystem::path> files;
};

bool compareStrings(const std::string_view& str, const std::wstring wstr) {
  if (str.size() != wstr.size()) {
    return false;
  }
  for (std::size_t i = 0; i < str.size(); ++i) {
    if (str[i] != static_cast<char>(wstr[i])) {
      return false;
    }
  }
  return true;
}

TEST_F(FormatOfTestEnv, test) {
  for (const auto& f : files) {
    std::vector<std::reference_wrapper<const formatof::Format>> fmts = list->formatOf(f);
    ASSERT_FALSE(fmts.empty());
    auto rhs = f.stem().native();
    bool found = false;
    for (auto fmt = fmts.begin(); !found && fmt != fmts.end(); ++fmt) {
      const std::string_view& lhs = fmt->get().name;
      std::cout << "Asserting: " << lhs << " == ";
      for (auto c : rhs) {
        std::cout << static_cast<char>(c);
      }
      std::cout << ": ";
      found |= compareStrings(lhs, rhs);
      if (found) {
       std::cout << "Success" << std::endl;
      } else {
        std::cout << "Fail" << std::endl;   
      }
    }
    if (!found) {
      FAIL();
    }
  }
  SUCCEED(); 
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
