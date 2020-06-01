#include <iostream>
#include <vector>

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

string select() {
    auto mark = [&](int current, int index) -> string {
        return current == index ? RADIO_ON : RADIO_OFF;
    };

    int index = 0;

    vector<string> fruits = {"Apple", "Banana", "Orange"};

    for (;;) {
        for (int i = 0; i < fruits.size(); i++) {
            string s = mark(index, i) + " " + fruits.at(i);
            cout << s << "\n";
        }

        int c = getch();

        switch (c) {
            case KEY_ENTER:
                return fruits.at(index);
            case KEY_SPEC:
                switch (c = getch()) {
                    case KEY_UP_ARROW:
                    case KEY_LEFT_ARROW:
                        index -= 1;
                        if (index < 0)
                            index = 2;
                        break;
                    case KEY_DOWN_ARROW:
                    case KEY_RIGHT_ARROW:
                        index += 1;
                        if (index > 2)
                            index = 0;
                        break;
                }
                break;
        }
        cout << erase::lines(fruits.size());
    }
}

int main() {
    cout << "? What's your favorite?" << endl << flush;
    auto choice = select();
    cout << "Your choice is: " << choice << "\n";
}
