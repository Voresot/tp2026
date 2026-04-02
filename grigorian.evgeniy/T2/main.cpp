#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>

struct DataStruct {
    unsigned long long key1;
    char key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    bool k1 = false, k2 = false, k3 = false;

    char c1, c2;
    in >> c1 >> c2;

    if (c1 != '(' || c2 != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < 3; ++i) {
        char k, e, y, num;

        in >> k >> e >> y >> num;
        if (k != 'k' || e != 'e' || y != 'y') {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (num == '1' && !k1) {
            in >> std::oct >> temp.key1;

            int next = in.peek();
            if (next == 'u' || next == 'U') {
                in.get();
                next = in.peek();
                if (next == 'l' || next == 'L') in.get();
                next = in.peek();
                if (next == 'l' || next == 'L') in.get();
            }
            k1 = true;
        }
        else if (num == '2' && !k2) {
            char q1, q2;
            in >> std::ws >> q1 >> std::noskipws >> temp.key2 >> std::skipws >> q2;
            if (q1 != '\'' || q2 != '\'') in.setstate(std::ios::failbit);
            k2 = true;
        }
        else if (num == '3' && !k3) {
            char q;
            in >> std::ws >> q;
            if (q != '"') {
                in.setstate(std::ios::failbit);
            } else {
                std::getline(in, temp.key3, '"');
            }
            k3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (i < 2) {
            char colon;
            in >> colon;
            if (colon != ':') in.setstate(std::ios::failbit);
        }
    }

    char c3, c4;
    in >> c3 >> c4;

    if (c3 != ':' || c4 != ')') {
        in.setstate(std::ios::failbit);
    }

    if (in && k1 && k2 && k3) {
        dest = temp;
    } else {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& src) {
    std::ostream::sentry sentry(os);
    if (!sentry) return os;

    std::ios_base::fmtflags f(os.flags());
    os << "(:key1 0" << std::oct << src.key1 << "ull"
       << ":key2 '" << src.key2 << "'"
       << ":key3 \"" << src.key3 << "\":)";
    os.flags(f);

    return os;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        std::istringstream iss(line);
        std::copy(
            std::istream_iterator<DataStruct>(iss),
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
