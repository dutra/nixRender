
#ifndef _COLOR_H
#define _COLOR_H

#include <iostream>

inline std::ostream& blue(std::ostream &s) {
  s << "\x1b[38;2;85;85;255m";
  return s;
}

inline std::ostream& yellow(std::ostream &s) {
  s << "\x1b[38;2;255;255;85m";
  return s;
}

inline std::ostream& red(std::ostream &s) {
  s << "\x1b[38;2;255;85;85m";
  return s;
}

inline std::ostream& magenta(std::ostream &s) {
  s << "\x1b[38;2;255;85;255m";
  return s;
}

inline std::ostream& reset(std::ostream &s) {
  s << "\x1b[0m";
  return s;
}

inline std::ostream& error(std::ostream &s) {
  s << red;
  return s;
}

inline std::ostream& warning(std::ostream &s) {
  s << yellow;
  return s;
}

inline std::ostream& info(std::ostream &s) {
  s << blue;
  return s;
}

inline std::ostream& intro(std::ostream &s) {
  s << magenta;
  return s;
}


#endif
