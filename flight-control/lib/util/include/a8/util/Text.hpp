#ifndef string__
#define string__

namespace a8 {
namespace util {

class Text {
public:
    Text();
    //string(char text[]);
    Text(char *text);
    //Text(Text &str); // copy constructor
    Text(float fValue);
    Text(int iValue);
    ~Text();
    int getLength() ;

    char *getText() ;

    // Text operator +(Text const &str2) {
    //     Text ret;   

    //     return  ret;
    // }
    static int const UNKNOWN = -1;
private:
    int length = UNKNOWN;
    char *text;
    void init(char *text, int length);
};

} // namespace util

} // namespace a8

#endif