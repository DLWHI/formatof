#include <gtest/gtest.h>

#include "../src/core/format_determiner.h"

struct FormatOfTestEnv : ::testing::Test {
  FormatOfTestEnv() : Test() {}

  void SetUp() {
    list = new formatof::FormatDeterminer();

    std::filesystem::path resoruce_dir(FORMATOF_RESOURCE_DIR);

    for (const auto& en : std::filesystem::directory_iterator(resoruce_dir)) {
      files.push_back(en.path());
    }
  }

  void TearDown() {
    delete list;
  }

  formatof::FormatDeterminer* list;
  std::vector<std::filesystem::path> files;
};

template <typename LhString, typename RhString>
bool compareStrings(const LhString& lhs, const RhString& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (std::size_t i = 0; i < lhs.size(); ++i) {
    if (static_cast<char>(lhs[i]) != static_cast<char>(rhs[i])) {
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
