#ifndef string__
#define string__

namespace a8 {
namespace util {

class string {
public:
    string();
    //string(char text[]);
    string(char *text);
    string(string &str); // copy constructor
    string(float fValue);
    string(int iValue);
    ~string();
    int getLength() ;

    char *getText() ;

    string operator +(string const &str2) {
        string ret;   

        return  ret;
    }
    static int const UNKNOWN = -1;
private:
    int length = UNKNOWN;
    char *text;
    void init(char *text, int length);
};

} // namespace util

} // namespace a8

#endif