// Functions to parse command line arguments. Functions inside are ugly,
// i don't want extra things in global namespace 
#ifndef ARGPARSE_H_
#define ARGPARSE_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

// Enumerations for argparse_option.has_val field
#define	ARGPARSE_NO_ARGUMENT		0
#define ARGPARSE_REQUIRED_ARGUMENT	1
#define ARGPARSE_OPTIONAL_ARGUMENT	2

// Some default values
#define ARGPARSE_NO_SHORT_NAME -1
#define ARGPARSE_NO_LONG_NAME NULL

/// Return codes
#define ARGPARSE_END -1
#define ARGPARSE_MISSING_VALUE -2
#define ARGPARSE_UNKNOWN_OPTION -3


struct argparse {
  int optind;
  int optpos;
  int argc;
  char* const* argv;
};

struct argparse_option
{
  char* val;
  int has_val;
  int short_name;
  const char* long_name;
};

static char *optarg;
static int opterr, optind, optopt;

struct argparse argparse_create(int argc, char** argv) {
  struct argparse def{1, 1, argc, argv};
  return def;
}

void argparse_reset(struct argparse* aps) {
  aps->optind = 1;
  aps->optpos = 1;
}

int argparse_short (int argc, char * const argv[], const char *optstring) {
  static int optpos = 1;
  const char *arg;
  if (optind == 0) {
    optind = 1;
    optpos = 1;
  } 
  if (*optstring == '+') {
    ++optstring;
  }
  arg = argv[optind];
  if (!arg) {
    return -1;
  } else if (strcmp(arg, "--") == 0) {
    ++optind;
    return -1;
  } else if (arg[0] == '-' && isalnum(arg[1])) {
    const char* opt = strchr(optstring, arg[optpos]);
    optopt = arg[optpos];
    int ret = optopt;
    if (!opt) {
      if (opterr && optstring[0] != ':') {
        fprintf(stderr, "%s: illegal option: %c\n", argv[0], optopt);
      }
      ret = '?';
    } else if (opt[1] == ':') {
      if (arg[optpos + 1]) {
        optarg = (char*)arg + optpos + 1;
        ++optind;
      } else if (argv[optind + 1] && argv[optind + 1][0] != '-') {
        optarg = argv[++optind];
      } else {
        if (opterr && *optstring != ':') {
          fprintf(stderr, "%s: option requires an argument: %c\n", argv[0], optopt);
        }
        ret = (*optstring == ':') ? ':' : '?';
      }
      optpos = 1;
    } 
    if (!arg[++optpos]) {
      ++optind;
      optpos = 1;
    }
    return ret;
  }
  return -1;
}

int getopt(int argc, char * const argv[], const char *optstring) {
  return argparse_short(argc, argv, optstring);
}

int argparse_long (struct argparse* args, struct argparse_option *options, int n) {
  char* arg = args->argv[args->optind];
  int opt = ARGPARSE_END;
  if (!arg) {
    return ARGPARSE_END;
  } else if (memcmp(arg, "--", 2) == 0) {
    if (!arg[2]) {
      return ARGPARSE_END;
    } else if (isalnum(arg[2])) {
      for (opt = 0; opt < n && strncmp(options[opt].long_name, arg + 2, strlen(options[opt].long_name)); ++ opt) {}
      if (opt >= n) {
        fprintf(stderr, "%s: illegal option: %s\n", args->argv[0], arg);
        opt = ARGPARSE_UNKNOWN_OPTION;
      } else if (options[opt].has_val != ARGPARSE_NO_ARGUMENT) {
        char* argval = strchr(arg, '=');
        if (argval) {
          options[opt].val = argval + 1;
        } else if (args->argv[args->optind + 1]) {
          options[opt].val = args->argv[++args->optind];
        } else if (options[opt].has_val == ARGPARSE_REQUIRED_ARGUMENT) {
          fprintf(stderr, "%s: option requires an argument: %s\n", args->argv[0], arg);
          opt = ARGPARSE_MISSING_VALUE;
        }
      }
      ++args->optind;
    }
  } else if (arg[0] == '-' && isalnum(arg[1])) {
    for (opt = 0; opt < n && options[opt].short_name != arg[args->optpos]; ++opt) {}
    if (opt >= n) {
        fprintf(stderr, "%s: illegal option: -%c\n", args->argv[0], arg[args->optpos]);
        opt = ARGPARSE_UNKNOWN_OPTION;
    } else if (options[opt].has_val != ARGPARSE_NO_ARGUMENT) {
      if (arg[args->optpos + 1]) {
        options[opt].val = (char*)arg + args->optpos + 1;
        ++args->optind;
      } else if (args->argv[args->optind + 1]) {
        options[opt].val = args->argv[++args->optind];
      } else if (options[opt].has_val == ARGPARSE_REQUIRED_ARGUMENT) {
        fprintf(stderr, "%s: option requires an argument: -%c\n", args->argv[0], arg[args->optpos]);
        opt = ARGPARSE_MISSING_VALUE;
      }
      args->optpos = 1;
    } 
    if (!arg[++args->optpos]) {
      args->optpos = 1;
      ++args->optind;
    }
  }
  return opt;
}
#ifdef	__cplusplus
}
#endif  // extern "C"

#endif  // ARGPARSE_H_
