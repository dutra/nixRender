#include <iostream>

/* using intro = magenta; */
/* using error = red; */
/* using warning = yellow; */
/* using info = blue; */

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

std::ostream& (&intro)(std::ostream&) = magenta;
std::ostream& (&error)(std::ostream&) = red;
std::ostream& (&warning)(std::ostream&) = yellow;
std::ostream& (&info)(std::ostream&) = blue;
