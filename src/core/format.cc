#include "format.h"

#include <cstdio>

#include "assert.h"

namespace formatof {
  static std::string_view getLexem(std::string_view& str) {
    auto p = str.find(';');
    ASSERT(p != std::string_view::npos, std::runtime_error("malformed file type info"));
    std::string_view r = str.substr(0, p);
    str = str.substr(p + 1);
    return r;
  }

  static std::vector<char> parseSignature(const std::string_view& sg) {
    std::vector<char> r;
    const char* p = sg.data();
    const char* last = sg.data() + sg.size(); 
    int c = 0;
    int rd = 0;
    for (;p != last;) {
      ASSERT(std::sscanf(p, "%X %n", &c, &rd), std::runtime_error("malformed file type info"));
      r.push_back(c);
      p += rd;
    }
    return r;
  }

  Format::ExtensionList parseExtensions(const std::string_view& exts) {
    Format::ExtensionList res;
    if (exts.empty()) {
      return res;
    }
    const char* first = exts.data();
    const char* end = first + exts.size();
    const char* last = first + 1;
    for (;first != end; first = last) {
      first += (*first == '|');
      for (++last ;last != end && *last != '|'; ++last);  
      std::string_view ext(first, last - first);
      res.push_back(std::move(ext));
    }
    return res;
  }

  Format Format::parse(const std::string_view& exts) {
    Format f;
    std::string_view e = exts;
    f.name = getLexem(e);
    ASSERT(!e.empty(), std::runtime_error("malformed file type info: unexpected end after obtaining name"));
    f.signature = parseSignature(getLexem(e));
    ASSERT(!e.empty(), std::runtime_error("malformed file type info: unexpected end after obtaining signature"));
    f.extensions = parseExtensions(getLexem(e));
    ASSERT(!e.empty(), std::runtime_error("malformed file type info: unexpected end after obtainig extensions"));
    getLexem(e);
    ASSERT(!e.empty(), std::runtime_error("malformed file type info: unexpected end after obtaining type"));
    int rd = 0;
    ASSERT(std::sscanf(e.data(), "%td%n", &f.offset, &rd), std::runtime_error("malformed file type info: : unexpected end after obtaining offset"));
    e = e.substr(rd);
    ASSERT(e.empty(), std::runtime_error("malformed file type info: : unsupported format of type info"));
    return f;
  }

  std::string Format::toString() const {
    std::string s(name);
    s += ", ";
    int i = 0;
    for (; i < extensions.size() - 1; ++i) {
      s += extensions[i];
      s += '|';
    }
    s += extensions.back();
    return s;
    }
}  // namespace formatof
