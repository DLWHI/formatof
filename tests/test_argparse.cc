#include <gtest/gtest.h>

#include <cstring>

#include "../src/cli/argparse.h"

TEST(ArgparseShortTest, no_options) {
  char* argv[] = {"program", NULL};
  optind = 0;
  int result = argparse_short(1, argv, "a");
  ASSERT_EQ(result, -1);
  ASSERT_EQ(optind, 1);
}

TEST(ArgparseShortTest, single_option_no_arg) {
  char* argv[] = {"program", "-a", NULL};
  optind = 0;
  int result = argparse_short(2, argv, "a");
  ASSERT_EQ(result, 'a');
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, option_with_required_arg) {
  char* argv[] = {"program", "-a", "value", NULL};
  optind = 0;
  int result = argparse_short(3, argv, "a:");
  ASSERT_EQ(result, 'a');
  ASSERT_STREQ(optarg, "value");
  ASSERT_EQ(optind, 3);
}

TEST(ArgparseShortTest, option_with_arg_adjacent) {
  char* argv[] = {"program", "-avalue", NULL};
  optind = 0;
  int result = argparse_short(2, argv, "a:");
  ASSERT_EQ(result, 'a');
  ASSERT_STREQ(optarg, "value");
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, combined_options) {
  char* argv[] = {"program", "-abc", NULL};
  optind = 0;
  int result1 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result1, 'a');
  int result2 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result2, 'b');
  int result3 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result3, 'c');
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, combined_options_with_unknown) {
  char* argv[] = {"program", "-aubc", NULL};
  optind = 0;
  int result1 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result1, 'a');
  int result2 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result2, '?');
  int result3 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result3, 'b');
  int result4 = argparse_short(2, argv, "abc");
  ASSERT_EQ(result4, 'c');
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, combined_options_separate) {
  char* argv[] = {"program", "-a", "-b", "-c", NULL};
  optind = 0;
  int result1 = argparse_short(4, argv, "abc");
  ASSERT_EQ(result1, 'a');
  int result2 = argparse_short(4, argv, "abc");
  ASSERT_EQ(result2, 'b');
  int result3 = argparse_short(4, argv, "abc");
  ASSERT_EQ(result3, 'c');
  ASSERT_EQ(optind, 4);
}

TEST(ArgparseShortTest, unknown_option) {
  char* argv[] = {"program", "-z", NULL};
  optind = 0;
  int result = argparse_short(2, argv, "a");
  ASSERT_EQ(result, '?');
  ASSERT_EQ(optopt, 'z');
}

TEST(ArgparseShortTest, missing_required_arg) {
  char* argv[] = {"program", "-a", NULL};
  optind = 0;
  int result = argparse_short(2, argv, "a:");
  ASSERT_EQ(result, '?');
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, end_of_options) {
  char* argv[] = {"program", "--", "-a", "file", NULL};
  optind = 0;
  int result = argparse_short(4, argv, "a");
  ASSERT_EQ(result, -1);
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, non_option_stops) {
  char* argv[] = {"program", "file", "-a", NULL};
  optind = 0;
  int result = argparse_short(3, argv, "a");
  ASSERT_EQ(result, -1);
  ASSERT_EQ(optind, 1);
}

TEST(ArgparseShortTest, combined_with_arg) {
  char* argv[] = {"program", "-bvalue", NULL};
  optind = 0;
  int result = argparse_short(2, argv, "b:");
  ASSERT_EQ(result, 'b');
  ASSERT_STREQ(optarg, "value");
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseShortTest, multiple_options_and_non_args) {
  char* argv[] = {"program", "-a", "value", "-b", "file2", "file1", NULL};
  optind = 0;
  int result1 = argparse_short(6, argv, "a:b:");
  ASSERT_EQ(result1, 'a');
  ASSERT_STREQ(optarg, "value");
  ASSERT_EQ(optind, 3);
  int result2 = argparse_short(6, argv, "a:b:");
  ASSERT_EQ(result2, 'b');
  ASSERT_STREQ(optarg, "file2");
  ASSERT_EQ(optind, 5);
}

TEST(ArgparseShortTest, multiple_options_stop_on_non_arg) {
  char* argv[] = {"program", "-a", "value", "file1", "-b", "file2", NULL};
  optind = 0;
  int result1 = argparse_short(6, argv, "ab:");
  ASSERT_EQ(result1, 'a');
  ASSERT_EQ(optind, 2);
  int result2 = argparse_short(6, argv, "ab:");
  ASSERT_EQ(result2, -1);
  ASSERT_EQ(optind, 2);
}

TEST(ArgparseTest, no_options) {
  char* argv[] = {"program", NULL};
  int argc = 1;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
  ASSERT_EQ(argobj.optind, 1);
}

TEST(ArgparseTest, single_long_option_no_arg) {
  char* argv[] = {"program", "--arg", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, long_option_with_required_arg) {
  char* argv[] = {"program", "--arg", "value", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_STREQ(opts[result].val, "value");
  ASSERT_EQ(argobj.optind, 3);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}


TEST(ArgparseTest, long_option_with_arg_equal) {
  char* argv[] = {"program", "--arg=value", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_STREQ(opts[result].val, "value");
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, combined_multiple_long_options) {
  char* argv[] = {"program", "--arg1", "--arg2", "--arg3", NULL};
  int argc = 4;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg1"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg2"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg3"}
  };
  int result = 0;
  for (int i = 0; i < 3; ++i) {
    result = argparse_long(&argobj, opts, 3);
    ASSERT_EQ(result, i);
    ASSERT_EQ(argobj.optind, i + 2);
  }
  result = argparse_long(&argobj, opts, 3);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, unknown_long_option) {
  char* argv[] = {"program", "--unknown", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"},
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_UNKNOWN_OPTION);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, long_option_missing_required_arg) {
  char* argv[] = {"program", "--arg", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_MISSING_VALUE);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, end_of_options) {
  char* argv[] = {"program", "--", "--arg", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
  ASSERT_EQ(argobj.optind, 1);
}

TEST(ArgparseTest, non_option_stops) {
  char* argv[] = {"program", "file", "--arg", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
  ASSERT_EQ(argobj.optind, 1);
}

TEST(ArgparseTest, multiple_long_options_and_non_args) {
  char* argv[] = {"program", "--arg", "value", "--barg", "file2", "file1", NULL};
  int argc = 6;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "barg"},
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 1);
  ASSERT_EQ(argobj.optind, 3);
  ASSERT_STREQ(opts[result].val, "value");
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 5);
  ASSERT_STREQ(opts[result].val, "file2");
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(argobj.optind, 5);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, multiple_long_options_stop_on_non_arg) {
  char* argv[] = {"program", "--arg", "value", "file1", "--barg", "file2", NULL};
  int argc = 6;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "barg"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 1);
  ASSERT_EQ(argobj.optind, 2);
  ASSERT_EQ(opts[result].val, nullptr);
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, ARGPARSE_END);
  ASSERT_EQ(argobj.optind, 2);
}

TEST(ArgparseTest, long_option_optional_argument_present) {
  char* argv[] = {"program", "--arg", "value", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_OPTIONAL_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 3);
  ASSERT_STREQ(opts[result].val, "value");
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, long_option_optional_argument_not_present) {
  char* argv[] = {"program", "--arg", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_OPTIONAL_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 2);
  ASSERT_EQ(opts[result].val, nullptr);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, single_short_option_no_arg) {
  char* argv[] = {"program", "-a", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, short_option_with_required_arg) {
  char* argv[] = {"program", "-a", "value", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_STREQ(opts[result].val, "value");
  ASSERT_EQ(argobj.optind, 3);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseShortTest, short_option_with_arg_adjacent) {
  char* argv[] = {"program", "-avalue", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_STREQ(opts[result].val, "value");
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, multiple_short_options) {
  char* argv[] = {"program", "-a", "-b", "-c", NULL};
  int argc = 4;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'b', ARGPARSE_NO_LONG_NAME},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'c', ARGPARSE_NO_LONG_NAME}
  };
  int result = 0;
  for (int i = 0; i < 3; ++i) {
    result = argparse_long(&argobj, opts, 3);
    ASSERT_EQ(result, i);
    ASSERT_EQ(argobj.optind, i + 2);
  }
  result = argparse_long(&argobj, opts, 3);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseShortTest, combined_short_options) {
  char* argv[] = {"program", "-abc", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'b', ARGPARSE_NO_LONG_NAME},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'c', ARGPARSE_NO_LONG_NAME}
  };
  int result = 0;
  for (int i = 0; i < 3; ++i) {
    result = argparse_long(&argobj, opts, 3);
    ASSERT_EQ(result, i);
  }
  result = argparse_long(&argobj, opts, 3);
  ASSERT_EQ(argobj.optind, 2);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, unknown_short_option) {
  char* argv[] = {"program", "-u", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME},
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_UNKNOWN_OPTION);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, short_option_missing_required_arg) {
  char* argv[] = {"program", "-a", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_MISSING_VALUE);
  ASSERT_EQ(argobj.optind, 2);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, multiple_short_options_and_non_args) {
  char* argv[] = {"program", "-a", "value", "-bfile2", "file1", NULL};
  int argc = 5;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'b', ARGPARSE_NO_LONG_NAME},
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, 'a', ARGPARSE_NO_LONG_NAME}
  };
  int result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 1);
  ASSERT_EQ(argobj.optind, 3);
  ASSERT_STREQ(opts[result].val, "value");
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 4);
  ASSERT_STREQ(opts[result].val, "file2");
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(argobj.optind, 4);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, multiple_short_options_stop_on_non_arg) {
  char* argv[] = {"program", "-a", "value", "file1", "--barg", "file2", NULL};
  int argc = 6;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_REQUIRED_ARGUMENT, ARGPARSE_NO_SHORT_NAME, "barg"},
    argparse_option{NULL, ARGPARSE_NO_ARGUMENT, 'a', "arg"}
  };
  int result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, 1);
  ASSERT_EQ(argobj.optind, 2);
  ASSERT_EQ(opts[result].val, nullptr);
  result = argparse_long(&argobj, opts, 2);
  ASSERT_EQ(result, ARGPARSE_END);
  ASSERT_EQ(argobj.optind, 2);
}

TEST(ArgparseTest, short_option_optional_argument_present) {
  char* argv[] = {"program", "-a", "value", NULL};
  int argc = 3;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_OPTIONAL_ARGUMENT, 'a', "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 3);
  ASSERT_STREQ(opts[result].val, "value");
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}

TEST(ArgparseTest, short_option_optional_argument_not_present) {
  char* argv[] = {"program", "-a", NULL};
  int argc = 2;
  struct argparse argobj = argparse_create(argc ,argv);
  struct argparse_option opts[] = {
    argparse_option{NULL, ARGPARSE_OPTIONAL_ARGUMENT, 'a', "arg"}
  };
  int result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, 0);
  ASSERT_EQ(argobj.optind, 2);
  ASSERT_EQ(opts[result].val, nullptr);
  result = argparse_long(&argobj, opts, 1);
  ASSERT_EQ(result, ARGPARSE_END);
}
