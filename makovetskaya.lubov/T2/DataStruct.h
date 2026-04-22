#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <iomanip>

struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
};

struct DelimiterIO {
    char exp;
};

struct DoubleIO {
    double& ref;
};

struct ULongLongIO {
    unsigned long long& ref;
};

struct StringIO {
    std::string& ref;
};

class iofmtguard {
public:
    explicit iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();

private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::ios::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleIO&& dest);
std::istream& operator>>(std::istream& in, ULongLongIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);

#endif
