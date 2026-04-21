#include "datastruct.h"
#include <cctype>
#include <cmath>
#include <sstream>

std::istream& operator>>(std::istream& in, DelimiterIO&& d) {
    std::istream::sentry guard(in);
    if (!guard) return in;
    char sym = '\0';
    in >> sym;
    if (in && sym != d.exp)
        in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, DoubleIO&& val) {
    std::istream::sentry guard(in);
    if (!guard) return in;
    in >> std::ws;
    std::string mantPart;
    char expSign = '+';
    int expVal = 0;
    char ch;
    bool seenDot = false;
    bool digitBefore = false;
    bool digitAfter = false;
    if (in.peek() == '-') {
        in.get(ch);
        mantPart.push_back('-');
    } else if (in.peek() == '+') {
        in.get(ch);
    }
    while (in.get(ch)) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            mantPart.push_back(ch);
            if (seenDot) digitAfter = true;
            else digitBefore = true;
        } else if (ch == '.' && !seenDot) {
            if (!digitBefore) {
                in.setstate(std::ios::failbit);
                return in;
            }
            mantPart.push_back(ch);
            seenDot = true;
        } else if (ch == 'e' || ch == 'E') {
            break;
        } else {
            in.putback(ch);
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    if (!digitBefore || !seenDot || !digitAfter) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (!in.get(expSign) || (expSign != '+' && expSign != '-')) {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> expVal;
    if (!in) {
        in.setstate(std::ios::failbit);
        return in;
    }
    double mantVal = std::stod(mantPart);
    int finalExp = (expSign == '-') ? -expVal : expVal;
    val.ref = mantVal * std::pow(10.0, finalExp);
    return in;
}

std::istream& operator>>(std::istream& in, LongLongIO&& val) {
    std::istream::sentry guard(in);
    if (!guard) return in;
    in >> std::ws;
    std::string token;
    char ch;
    while (in.get(ch) && ch != ':')
        token.push_back(ch);
    in.putback(':');
    if (token.length() < 2) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (token.length() == 3 && (token[0] == '-' || token[0] == '+')) {
        in.setstate(std::ios::failbit);
        return in;
    }
    size_t len = token.length();
    char c1 = std::tolower(token[len - 2]);
    char c2 = std::tolower(token[len - 1]);
    if (c1 != 'l' || c2 != 'l') {
        in.setstate(std::ios::failbit);
        return in;
    }
    token.pop_back();
    token.pop_back();
    if (token.empty() || token == "-" || token == "+") {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::stringstream ss(token);
    long long num = 0;
    if (ss >> num) {
        val.ref = num;
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& str) {
    std::istream::sentry guard(in);
    if (!guard) return in;
    return std::getline(in >> DelimiterIO{'"'}, str.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& ds) {
    std::istream::sentry guard(in);
    if (!guard) return in;
    DataStruct tmp;
    bool k1 = false, k2 = false, k3 = false;
    if (!(in >> DelimiterIO{'('} >> DelimiterIO{':'})) {
        in.setstate(std::ios::failbit);
        return in;
    }
    for (int i = 0; i < 3; ++i) {
        std::string field;
        if (!(in >> field)) break;
        if (field == "key1" && !k1) {
            if (!(in >> DoubleIO{tmp.key1})) break;
            k1 = true;
        } else if (field == "key2" && !k2) {
            if (!(in >> LongLongIO{tmp.key2})) break;
            k2 = true;
        } else if (field == "key3" && !k3) {
            if (!(in >> StringIO{tmp.key3})) break;
            k3 = true;
        } else {
            in.setstate(std::ios::failbit);
            break;
        }
        if (i < 2 && !(in >> DelimiterIO{':'})) {
            in.setstate(std::ios::failbit);
            break;
        }
    }
    if (!(in >> DelimiterIO{':'} >> DelimiterIO{')'}) || !k1 || !k2 || !k3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    ds = std::move(tmp);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& ds) {
    std::ostream::sentry guard(out);
    if (!guard) return out;
    iofmtguard fmt(out);
    double m = ds.key1;
    int e = 0;
    if (ds.key1 == 0.0) {
        m = 0.0;
        e = 0;
    } else {
        double a = std::abs(m);
        while (a < 1.0 && a != 0.0) {
            a *= 10.0;
            --e;
        }
        while (a >= 10.0) {
            a /= 10.0;
            ++e;
        }
        m = (m < 0) ? -a : a;
    }
    out << "(:key1 ";
    out << std::fixed << std::setprecision(1) << m;
    out << ((e >= 0) ? "e+" : "e") << e;
    out << ":key2 " << ds.key2 << "ll";
    out << ":key3 \"" << ds.key3 << "\":)";
    return out;
}

iofmtguard::iofmtguard(std::basic_ios<char>& s)
    : stream_(s)
    , w_(s.width())
    , fill_(s.fill())
    , prec_(s.precision())
    , flags_(s.flags())
{}

iofmtguard::~iofmtguard() {
    stream_.width(w_);
    stream_.fill(fill_);
    stream_.precision(prec_);
    stream_.flags(flags_);
}
