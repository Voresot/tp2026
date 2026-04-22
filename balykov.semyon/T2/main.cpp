#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <complex>
#include <cmath>
#include <cctype>
struct DelimiterIO {
    char exp;
};

struct ULLBinIO {
    unsigned long long& ref;
    std::string& raw;
};

struct ComplexIO {
    std::complex<double>& ref;
};

struct StringIO {
    std::string& ref;
};

struct DataStruct {
    unsigned long long key1;
    std::string key1Raw;
    std::complex<double> key2;
    std::string key3;
};

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};
std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, ULLBinIO&& dest);
std::istream& operator>>(std::istream& in,
    ComplexIO&& dest);
std::istream& operator>>(std::istream& in,
    StringIO&& dest);
std::istream& operator>>(std::istream& in,
    DataStruct& dest);
std::ostream& operator<<(std::ostream& out,
    const DataStruct& dest);

iofmtguard::iofmtguard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags()) {
}

iofmtguard::~iofmtguard() {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, StringIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
std::istream& operator>>(std::istream& in, ULLBinIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = '0';
    in >> c;

    if (c != '0') {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> c;
    if (c != 'b' && c != 'B') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string binary_str;
    while (in.get(c) && (c == '0' || c == '1')) {
        binary_str += c;
    }
    in.unget();
    if (binary_str.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    dest.raw = binary_str;
    dest.ref = 0;
    for (char c : binary_str) {
        dest.ref = dest.ref * 2 + (c - '0');
    }
    return in;
}
std::istream& operator>>(std::istream& in, ComplexIO&& dest){
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    double real = 0.0;
    double image = 0.0;
    in >> DelimiterIO{ '#' };
    in >> DelimiterIO{ 'c' };
    in >> DelimiterIO{ '(' };
    in >> real;
    in >> image;
    in >> DelimiterIO{ ')' };
    if (in) {
        dest.ref = std::complex<double>(real, image);
    }
    else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    DataStruct input;
    bool key1_in = false;
    bool key2_in = false;
    bool key3_in = false;
    in >> DelimiterIO{ '(' };
    while (in && (!key1_in || !key2_in || !key3_in)) {
        in >> DelimiterIO{ ':' };
        std::string label;
        in >> label;
        if (!in) {
            in.setstate(std::ios::failbit);
            return in;
        }
        if (label == "key1" && key1_in == false) {
            in >> ULLBinIO{input.key1, input.key1Raw};
            key1_in = true;
        }
        else if (label == "key2" && key2_in == false) {
            in >> ComplexIO{ input.key2 };
            key2_in = true;
        }
        else if (label == "key3" && key3_in == false) {
            in >> StringIO{ input.key3 };
            key3_in = true;
        }
        else {
            in.setstate(std::ios::failbit);
            break;
        }
    }
    in >> DelimiterIO{ ':' };
    in >> DelimiterIO{ ')' };
    if (in && key1_in && key2_in && key3_in){
        dest = input;
    }
    else{
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard guard(out);
    out << "(:key1 0b" << src.key1Raw;
    out << ":key2 #c(";
    out << std::fixed << std::setprecision(1) << src.key2.real() << " ";
    out << std::fixed << std::setprecision(1) << src.key2.imag() << "):";

    out << "key3 \"" << src.key3 << "\":)";

    return out;
}
bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    const double eps = 1e-9;
    double mod_a = std::abs(a.key2);
    double mod_b = std::abs(b.key2);
    if (std::abs(mod_a - mod_b) > eps) {
        return mod_a < mod_b;
    }
    return a.key3.length() < b.key3.length();
}
int main(){
    std::vector<DataStruct> data;
    std::string line;
    while (std::getline(std::cin, line)){
        if (line.empty()){
            continue;
        }

        std::istringstream lineStream(line);
        std::copy(
            std::istream_iterator<DataStruct>(lineStream),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
