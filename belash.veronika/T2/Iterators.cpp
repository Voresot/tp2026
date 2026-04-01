#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cctype>

struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
};

std::string extractValue(const std::string& line, const std::string& key) {
    std::string search = ":" + key + " ";
    size_t pos = line.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    size_t end = line.find(':', pos);
    if (end == std::string::npos) return "";
    return line.substr(pos, end - pos);
}

bool parseDataStruct(const std::string& line, DataStruct& ds) {
    if (line.size() < 4 || line[0] != '(' || line[1] != ':' ||
        line[line.size() - 2] != ':' || line[line.size() - 1] != ')')
        return false;

    std::string key1_val = extractValue(line, "key1");
    std::string key2_val = extractValue(line, "key2");
    std::string key3_val = extractValue(line, "key3");
    if (key1_val.empty() || key2_val.empty() || key3_val.empty())
        return false;

    std::istringstream iss1(key1_val);
    double d;
    char suffix;
    iss1 >> d;
    if (iss1.fail()) return false;
    iss1 >> suffix;
    if (suffix != 'd' && suffix != 'D') return false;
    std::string leftover1;
    iss1 >> leftover1;
    if (!leftover1.empty()) return false;
    ds.key1 = d;

    std::istringstream iss2(key2_val);
    unsigned long long ull;
    iss2 >> ull;
    if (iss2.fail()) return false;
    std::string suffix2;
    iss2 >> suffix2;
    if (suffix2.size() == 3) {
        for (char& c : suffix2) c = std::tolower(c);
        if (suffix2 != "u11" && suffix2 != "ull") return false;
    }
    else {
        return false;
    }
    std::string leftover2;
    iss2 >> leftover2;
    if (!leftover2.empty()) return false;
    ds.key2 = ull;

    if (key3_val.size() < 2 || key3_val.front() != '"' || key3_val.back() != '"')
        return false;
    ds.key3 = key3_val.substr(1, key3_val.size() - 2);

    return true;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << ds.key1;
    os << "(:key1 " << oss.str() << "d:key2 " << ds.key2 << "u11:key3 \""
        << ds.key3 << "\":)";
    return os;
}

std::istream& operator>>(std::istream& is, DataStruct& ds) {
    std::string line;
    if (!std::getline(is, line)) {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (!parseDataStruct(line, ds)) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

int main() {
    std::vector<DataStruct> vec;
    std::copy(std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(vec));

    std::sort(vec.begin(), vec.end(),
        [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            if (a.key2 != b.key2) return a.key2 < b.key2;
            return a.key3.size() < b.key3.size();
        });

    std::copy(vec.begin(), vec.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
