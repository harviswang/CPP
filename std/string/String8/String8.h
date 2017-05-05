#ifndef STRING8_H
#define STRING8_H

class String16;
//! This is a string holding UTF-8 characters. Does not allow the value more
// than 0x10FFFF, which is not valid unicode codepoint.
class String8
{
public:
             String8();
             String8(const String8& o);
    explicit String8(const char* o);
    explicit String8(const char* o, size_t numChars);
    
    explicit String8(const String16& o);
    explicit String8(const char16_t* o);
    explicit String8(const char16_t* o, size_t numChars);
    explicit String8(const char32_t* o);
    explicit String8(const char32_t* o, size_t numChars);
             ~String8();

    static inline const String8 empty();
    
    static String8 format(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
    static String8 formatV(const char* fmt, va_list args);
};

#endif //STRING8_H