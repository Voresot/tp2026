#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <limits>

struct Datastruct {
    double key1 = 0;
    unsigned long long key2 = 0;
    std::string key3;
};

std::istream& operator>>(std::istream& in, Datastruct& ds)
{
    char ch, c;
    double val;
    std::string s;

    if (!(in >> ch) || ch != '(') {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return in;
    }

    if (!(in >> ch) || ch != ':') {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return in;
    }

    while (in.peek() != EOF && in.peek() != ')')
    {
        std::string key;

        while (in.peek() != ' ' && in.peek() != EOF) {
            c = in.get();
            if (c == ':') continue;
            key += c;
        }

        if (in.peek() == ' ') in.get();
        if (key == "key1") {
            in >> val;

            if (!in) {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), ')');
                break;
            }

            ds.key1 = val;
        }
        else if (key == "key2") {
            in >> s;

            if (!in) {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), ')');
                break;
            }
            if (in.peek() == '0') {
            in.ignore(2);
            }
          in >> val;
          ds.key2 = val;
        }

        else if (key == "key3") {
            if (in.get() != '"') {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), ')');
                break;
            }

            std::getline(in, ds.key3, '"');

            if (!in) {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), ')');
                break;
            }
        }

        if (in.peek() == ':') in.get();
    }

    if (in.peek() == ')') in.get();

    return in;
}

std::ostream& operator<<(std::ostream& out, const Datastruct& ds)
{
    out << std::nouppercase;

    out << "(:key1 "
        << std::fixed << std::setprecision(1) << ds.key1
        << ":key2 0b" << ds.key2
        << ":key3 \"" << ds.key3 << "\":)";

    return out;
}

bool compare_data(const Datastruct& a, const Datastruct& b)
{
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<Datastruct> vec;
    std::copy(std::istream_iterator<Datastruct>(std::cin),
              std::istream_iterator<Datastruct>(),
              std::back_inserter(vec));

    std::sort(vec.begin(), vec.end(), compare_data);

    for (const auto& x : vec) {
        std::cout << x << "\n";
    }
}
