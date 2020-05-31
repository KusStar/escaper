#include <iostream>

#include "escaper.hpp"

#if IS_WINDOWS
const char *RADIO_ON = "(*)";
const char *RADIO_OFF = "( )";
#else
const char *RADIO_ON = "◉";
const char *RADIO_OFF = "◯";
#endif

using namespace std;
using namespace escaper;

void run() {
    auto mark = [&](int current, int index) -> string {
        return current == index ? RADIO_ON : RADIO_OFF;
    };

    int index = 0;

    for (;;) {
        string s1 = mark(index, 0) + " " + "Apple";
        string s2 = mark(index, 1) + " " + "Banana";
        string s3 = mark(index, 2) + " " + "Orange";

        cout << s1 << "\n";
        cout << s2 << "\n";
        cout << s3 << "\n";
        cout << "  Use arrow keys (↕) to move cursor" << "\n";

        int c = getch();
        switch (c) {
            case KEY_ENTER:
                cout << "Your choice is: " << index + 1 << "\n";
                return;
            case KEY_SPEC:
                switch (c = getch()) {
                    case KEY_UP_ARROW:
                    case KEY_LEFT_ARROW:
                        index -= 1;
                        if (index < 0) index = 2;
                        break;
                    case KEY_DOWN_ARROW:
                    case KEY_RIGHT_ARROW:
                        index += 1;
                        if (index > 2) index = 0;
                        break;
                }
                break;
        }
        cout << cursor::up(4);
    }
}

int main() { run(); }
