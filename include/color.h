
#ifndef _COLOR_H
#define _COLOR_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>

inline std::ostream& blue(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& red(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& green(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& magenta(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& yellow(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}


inline std::ostream& reset(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return s;
}

struct color {
    color(WORD attribute) :m_color(attribute){};
    WORD m_color;
};

template <class _Elem, class _Traits> std::basic_ostream<_Elem, _Traits>&
operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, c.m_color);
    return i;
}

#else

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

inline std::ostream& green(std::ostream &s) {
    s << "\x1b[38;2;85;255;85m";
    return s;
}

inline std::ostream& reset(std::ostream &s) {
    s << "\x1b[0m";
    return s;
}

#endif

inline std::ostream& done(std::ostream &s) {
    s << green;
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
