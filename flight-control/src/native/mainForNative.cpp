#include "a8/Hal.hpp"
#include "a8/util/Text.hpp"
#include <iostream>
using namespace a8::util;
using namespace std;
using namespace a8::hal;
class MyText {
public:
    MyText() {
        this->text = new char[1]{'\0'};
        this->length = -1;
    }
    MyText(MyText &t2) {
        cout << "Copy t2" << endl;
        int len = t2.getLength();
        this->text = new char[len + 1];
        for (int i = 0; i < len + 1; i++) {
            this->text[i] = t2.text[i];
        }
        this->length = len;
    }
    int getLength() {
        if (length == -1) {
            for (int i = 0;; i++) {
                cout << i << ":" << this->text[i] << endl;
                if (this->text[i] == '\0') {
                    cout << "text's len:" << i << endl;
                    length = i;
                    break;
                }
            }
        }
        cout << "len:" << length << endl;
        return length;
    }

    ~MyText() {
        println("Text::~Text()");
        println(this->text);
        print("this->text == 0?:");
        // if (this->text == 0) {
        //    println("yes");
        //} else {
        println("no");
        delete[] text;
        this->text = 0;
        //}
        // delete[] this->text;

        // this->text = 0;
        // this->length = 0;
    }

    char *text = 0;
    int length = -1;
};
void log(MyText msg) {
    MyText s2 = msg;
    cout << s2.text << endl;
}
int main(int argc, char **argv) {
    cout << "Hello,main()";
    MyText s1;
    log(s1);
    return 0;
}