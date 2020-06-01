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
#define KEY_SPEC -32
#define KEY_BACKSPACE 8
#define KEY_UP_ARROW 72
#define KEY_DOWN_ARROW 80
#define KEY_RIGHT_ARROW 77
#define KEY_LEFT_ARROW 75
#else
#define KEY_SPEC 17
#define KEY_CONNECTER 91
#define KEY_BACKSPACE 127
#define KEY_UP_ARROW 65
#define KEY_DOWN_ARROW 66
#define KEY_RIGHT_ARROW 67
#define KEY_LEFT_ARROW 68
#endif

#define KEY_TAB 9
#define KEY_ESC 27
#define KEY_DEL 51
#define KEY_ENTER 10

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

    using namespace std;
    const string ESC = "\x1B";
    const string CSI = ESC + "[";

    namespace detail {

        string repeat(string str, int count) {
            if (count < 1)
                return "";
            if (count == 1)
                return str;
            if (count == 2)
                return str + str;

            string res;
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

        const string left = CSI + "G";
        const string hide = CSI + "?25l";
        const string show = CSI + "?25h";
        const string save = ESC + "7";
        const string restore = ESC + "8";

        string up(const int& count = 1) { return CSI + to_string(count) + "A"; }

        string down(const int& count = 1) {
            return CSI + to_string(count) + "B";
        }

        string forward(const int& count = 1) {
            return CSI + to_string(count) + "C";
        }

        string backward(const int& count = 1) {
            return CSI + to_string(count) + "D";
        }

        string next_line(const int& count = 1) {
            return detail::repeat(CSI + "E", count);
        }

        string prev_line(const int& count = 1) {
            return detail::repeat(CSI + "F", count);
        }

    }  // namespace cursor

    namespace scroll {

        string up(const int& count = 1) {
            return detail::repeat(CSI + "S", count);
        }

        string down(const int& count = 1) {
            return detail::repeat(CSI + "T", count);
        }

    }  // namespace scroll

    namespace erase {

        const string screen = CSI + "2J";
        const string line = CSI + "2K";
        const string line_start = CSI + "1K";
        const string line_end = CSI + "K";

        string up(const int& count = 1) {
            return detail::repeat(CSI + "1J", count);
        }

        string down(const int& count = 1) {
            return detail::repeat(CSI + "J", count);
        }

        string lines(const int& count = 1) {
            return detail::repeat(line + cursor::up(), count);
        }

    }  // namespace erase

}  // namespace escaper

#endif
