// var 5
// ULL OCT CMP LSP

#include <algorithm>
#include <cctype>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <complex>
#include <iostream>
#include <vector>

// (:key1 #c(1.0 -1.0):key2 (:keyX 076:):key3 "data":)
// (:key2 (:keyX 01001:):key3 "with : inside":key1 #c(2.0 -3.0):)
struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO {
    char exp;
};

struct OctLiteralIO {
    unsigned long long& ref;
};

struct ComplexIO {
    std::complex<double>& ref;
};

struct StringIO {
    std::string& ref;
};

struct LabelIO {
    std::string exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, OctLiteralIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

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

std::istream& operator>>(std::istream& in, OctLiteralIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    unsigned long long num;

    in >> DelimiterIO{'0'} >> num;
    if (!in) {
        return in;
    }

    std::istringstream str_view(std::to_string(num));

    char digit;
    while (str_view >> digit) {
        if (((digit - '0') < 0) || ((digit - '0') > 9)) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    dest.ref = num;

    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    double re;
    double im;

    in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('} >> re;
    if (!in) {
        return in;
    }

    if (!std::isspace(in.peek())) {
        in.setstate(std::ios::failbit);
        return in;
    }
    in.get();

    in >> im >> DelimiterIO{')'};
    if (in) {
        dest.ref = std::complex<double>(re, im);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string data = "";
    if ((in >> StringIO{data}) && (data != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

void cleanup_whitespaces(std::istream& in) {
    while (std::isspace(in.peek())) {
        in.get();
    }
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    cleanup_whitespaces(in);

    // if (in.peek() != '(') {
    //     in.setstate(std::ios::failbit);
    //     return in;
    // }

    DataStruct object;
    struct check_keys_status {
        bool key1 = false;
        bool key2 = false;
        bool key3 = false;

        bool is_set1() {
            return key1;
        }

        void set1() {
            key1 = true;
        }

        bool is_set2() {
            return key2;
        }

        void set2() {
            key2 = true;
        }

        bool is_set3() {
            return key3;
        }

        void set3() {
            key3 = true;
        }

        bool all_set() {
            return key1 && key2 && key3;
        }
    };

    in >> DelimiterIO{'('};
    if (!in) {
        return in;
    }

    check_keys_status keys;
    while (in && in.peek() != ')') {
        in >> DelimiterIO{':'};

        std::string field_type;
        in >> LabelIO{field_type};
        if (!in) {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> DelimiterIO{' '};
        if (!in) {
            return in;
        }

        if (field_type == "key1" && !keys.is_set1()) {
            in >> OctLiteralIO{object.key1};
            keys.set1();
        } else if (field_type == "key2" && !keys.is_set2()) {
            in >> ComplexIO{object.key2};
            keys.set2();
        } else if (field_type == "key3" && !keys.is_set3()) {
            in >> StringIO{object.key3};
            keys.set3();
        } else {
            in.setstate(std::ios::failbit);
            return in;
        }

        // if (!in) {
        //     return in;
        // }

        in >> DelimiterIO{':'};
        if (!in) {
            return in;
        }
    }

    in >> DelimiterIO{')'};

    if (in && keys.all_set()) {
        dest = std::move(object);
    } else {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    out << "(:key1 0" << src.key1
        << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")"
        << ":key3 \"" << src.key3 << "\":)";

    return out;
}

bool compare_keys(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    if (a.key2 != b.key2) {
        return std::abs(a.key2) < std::abs(b.key2);
    }

    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> objects;
    std::string raw;

    while (std::getline(std::cin, raw)) {
        if (raw.empty()) {
            continue;
        }

        std::istringstream iss(raw);
        std::copy(
            std::istream_iterator<DataStruct>(iss),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(objects)
        );
    }

    std::sort(
        objects.begin(),
        objects.end(),
        compare_keys
    );

    std::copy(
        objects.begin(),
        objects.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
