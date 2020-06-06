#ifndef _ESCAPER_HEAD_
#define _ESCAPER_HEAD_

#pragma once

#include <cstdio>
#include <string>

// `getch()` and some definitions are from tinycon
// https://sourceforge.net/projects/tinycon
// TinyCon - A tiny console library, written for C++
// --------------------------------------------------------------------------------
// License:  BSD
// Author:   Unix-Ninja | chris (at) unix-ninja (dot) com
// Version:  0.6
// Released: March 20, 2013
// --------------------------------------------------------------------------------

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif

#if IS_WINDOWS
#include <Windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#if IS_WINDOWS
#define KEY_SPEC 0
#define KEY_BACKSPACE 8
#define KEY_UP_ARROW 72
#define KEY_DOWN_ARROW 80
#define KEY_RIGHT_ARROW 77
#define KEY_LEFT_ARROW 75
#define KEY_ENTER 13
#else
#define KEY_SPEC 17
#define KEY_CONNECTER 91
#define KEY_BACKSPACE 127
#define KEY_UP_ARROW 65
#define KEY_DOWN_ARROW 66
#define KEY_RIGHT_ARROW 67
#define KEY_LEFT_ARROW 68
#define KEY_ENTER 10
#endif

#define KEY_TAB 9
#define KEY_ESC 27
#define KEY_DEL 51
#define KEY_SPACE 32

#if !IS_WINDOWS
int getch() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();
    if (ch == KEY_ESC) {
        ch = getchar();
        if (ch == KEY_CONNECTER) {
            ch = KEY_SPEC;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

namespace escaper {

    const std::string ESC = "\x1B";
    const std::string CSI = ESC + "[";

    namespace detail {

        std::string repeat(std::string str, int count) {
            if (count < 1)
                return "";
            if (count == 1)
                return str;
            if (count == 2)
                return str + str;

            std::string res;
            int max = str.length() * count;

            while (max > res.length() && count > 1) {
                if (count & 1) {
                    res += str;
                }
                count >>= 1;
                str += str;
            }
            res += str;

            return res;
        }

    }  // namespace detail

    namespace cursor {

        const std::string left = CSI + "G";
        const std::string hide = CSI + "?25l";
        const std::string show = CSI + "?25h";
        const std::string save = ESC + "7";
        const std::string restore = ESC + "8";

        std::string to(const int x, const int& y) {
            if (y <= 0) {
                return CSI + std::to_string(x + 1) + "G";
            }
            return CSI + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H";
        }

        std::string move(const int x, const int& y) {
            std::string result;

            if (x < 0) {
                result += CSI + std::to_string(-x) + "D";
            } else if (x > 0) {
                result += CSI + std::to_string(x) + "C";
            }

            if (y < 0) {
                result += CSI + std::to_string(-y) + "A";
            } else if (y > 0) {
                result += CSI + std::to_string(y) + "B";
            }

            return result;
        }

        std::string up(const int& count = 1) { return CSI + std::to_string(count) + "A"; }

        std::string down(const int& count = 1) {
            return CSI + std::to_string(count) + "B";
        }

        std::string forward(const int& count = 1) {
            return CSI + std::to_string(count) + "C";
        }

        std::string backward(const int& count = 1) {
            return CSI + std::to_string(count) + "D";
        }

        std::string next_line(const int& count = 1) {
            return detail::repeat(CSI + "E", count);
        }

        std::string prev_line(const int& count = 1) {
            return detail::repeat(CSI + "F", count);
        }

    }  // namespace cursor

    namespace scroll {

        std::string up(const int& count = 1) {
            return detail::repeat(CSI + "S", count);
        }

        std::string down(const int& count = 1) {
            return detail::repeat(CSI + "T", count);
        }

    }  // namespace scroll

    namespace erase {

        const std::string screen = CSI + "2J";
        const std::string line = CSI + "2K";
        const std::string line_start = CSI + "1K";
        const std::string line_end = CSI + "K";

        std::string up(const int& count = 1) {
            return detail::repeat(CSI + "1J", count);
        }

        std::string down(const int& count = 1) {
            return detail::repeat(CSI + "J", count);
        }

        std::string lines(const int& count = 1) {
            return detail::repeat(line + cursor::up(), count) + cursor::left;
        }

    }  // namespace erase

}  // namespace escaper

#endif
