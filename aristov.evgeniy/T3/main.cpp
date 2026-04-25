#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <functional>

struct Point {
    int x;
    int y;
};

struct Polygon {
    std::vector<Point> points;
};

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b) {
    return a.points.size() == b.points.size() &&
           std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

std::istream& operator>>(std::istream& in, Point& pt) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c1 = 0;
    char c2 = 0;
    char c3 = 0;
    int x = 0;
    int y = 0;
    in >> c1 >> x >> c2 >> y >> c3;
    if (in && c1 == '(' && c2 == ';' && c3 == ')') {
        pt.x = x;
        pt.y = y;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& p) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    size_t count = 0;
    if (!(in >> count) || count < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::vector<Point> pts;
    std::copy_n(std::istream_iterator<Point>(in), count, std::back_inserter(pts));
    if (in) {
        p.points = std::move(pts);
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

struct AreaAccumulator {
    Point first;
    Point prev;
    double sum;
};

AreaAccumulator accumulateAreaFunc(AreaAccumulator acc, const Point& curr) {
    acc.sum += static_cast<double>(acc.prev.x) * curr.y -
               static_cast<double>(curr.x) * acc.prev.y;
    acc.prev = curr;
    return acc;
}

double getArea(const Polygon& p) {
    if (p.points.size() < 3) {
        return 0.0;
    }
    AreaAccumulator initial{p.points.front(), p.points.front(), 0.0};
    AreaAccumulator result = std::accumulate(
        std::next(p.points.begin()), p.points.end(), initial, accumulateAreaFunc);
    result.sum += static_cast<double>(result.prev.x) * result.first.y -
                  static_cast<double>(result.first.x) * result.prev.y;
    return std::abs(result.sum) / 2.0;
}

double addAreaIfEven(double acc, const Polygon& p) {
    return (p.points.size() % 2 == 0) ? acc + getArea(p) : acc;
}

double addAreaIfOdd(double acc, const Polygon& p) {
    return (p.points.size() % 2 != 0) ? acc + getArea(p) : acc;
}

double addAreaIfEqual(double acc, const Polygon& p, size_t count) {
    return (p.points.size() == count) ? acc + getArea(p) : acc;
}

double addArea(double acc, const Polygon& p) {
    return acc + getArea(p);
}

bool compareArea(const Polygon& a, const Polygon& b) {
    return getArea(a) < getArea(b);
}

bool compareVertexes(const Polygon& a, const Polygon& b) {
    return a.points.size() < b.points.size();
}

bool isEven(const Polygon& p) {
    return p.points.size() % 2 == 0;
}

bool isOdd(const Polygon& p) {
    return p.points.size() % 2 != 0;
}

bool isEqualSize(const Polygon& p, size_t count) {
    return p.points.size() == count;
}

struct BBox {
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
};

BBox expandBBoxWithPoint(BBox box, const Point& pt) {
    return BBox{
        std::min(box.minX, pt.x),
        std::max(box.maxX, pt.x),
        std::min(box.minY, pt.y),
        std::max(box.maxY, pt.y)
    };
}

BBox expandBBoxWithPolygon(BBox box, const Polygon& poly) {
    return std::accumulate(
        poly.points.begin(), poly.points.end(), box, expandBBoxWithPoint);
}

struct MaxSeqState {
    size_t current;
    size_t max;
};

MaxSeqState seqAccumulator(MaxSeqState acc, const Polygon& p, const Polygon& target) {
    if (p == target) {
        return MaxSeqState{acc.current + 1, std::max(acc.max, acc.current + 1)};
    }
    return MaxSeqState{0, acc.max};
}

size_t parseStrictSizeT(const std::string& str) {
    size_t pos = 0;
    size_t val = std::stoull(str, &pos);
    if (pos != str.length()) {
        throw std::invalid_argument("");
    }
    return val;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Missing filename\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    while (!file.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(file),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );
        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cout << std::fixed << std::setprecision(1);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        try {
            if (cmd == "AREA") {
                std::string subcmd;
                iss >> subcmd;
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                if (subcmd == "EVEN") {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, addAreaIfEven);
                    std::cout << sum << '\n';
                } else if (subcmd == "ODD") {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, addAreaIfOdd);
                    std::cout << sum << '\n';
                } else if (subcmd == "MEAN") {
                    if (polygons.empty()) {
                        throw std::invalid_argument("");
                    }
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, addArea);
                    std::cout << (sum / polygons.size()) << '\n';
                } else {
                    size_t count = parseStrictSizeT(subcmd);
                    if (count < 3) {
                        throw std::invalid_argument("");
                    }
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        std::bind(addAreaIfEqual, std::placeholders::_1, std::placeholders::_2, count));
                    std::cout << sum << '\n';
                }
            } else if (cmd == "MAX") {
                std::string subcmd;
                iss >> subcmd;
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                if (polygons.empty()) {
                    throw std::invalid_argument("");
                }
                if (subcmd == "AREA") {
                    auto it = std::max_element(polygons.begin(), polygons.end(), compareArea);
                    std::cout << getArea(*it) << '\n';
                } else if (subcmd == "VERTEXES") {
                    auto it = std::max_element(polygons.begin(), polygons.end(), compareVertexes);
                    std::cout << it->points.size() << '\n';
                } else {
                    throw std::invalid_argument("");
                }
            } else if (cmd == "MIN") {
                std::string subcmd;
                iss >> subcmd;
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                if (polygons.empty()) {
                    throw std::invalid_argument("");
                }
                if (subcmd == "AREA") {
                    auto it = std::min_element(polygons.begin(), polygons.end(), compareArea);
                    std::cout << getArea(*it) << '\n';
                } else if (subcmd == "VERTEXES") {
                    auto it = std::min_element(polygons.begin(), polygons.end(), compareVertexes);
                    std::cout << it->points.size() << '\n';
                } else {
                    throw std::invalid_argument("");
                }
            } else if (cmd == "COUNT") {
                std::string subcmd;
                iss >> subcmd;
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                if (subcmd == "EVEN") {
                    size_t res = std::count_if(polygons.begin(), polygons.end(), isEven);
                    std::cout << res << '\n';
                } else if (subcmd == "ODD") {
                    size_t res = std::count_if(polygons.begin(), polygons.end(), isOdd);
                    std::cout << res << '\n';
                } else {
                    size_t num = parseStrictSizeT(subcmd);
                    if (num < 3) {
                        throw std::invalid_argument("");
                    }
                    size_t res = std::count_if(polygons.begin(), polygons.end(),
                        std::bind(isEqualSize, std::placeholders::_1, num));
                    std::cout << res << '\n';
                }
            } else if (cmd == "INFRAME") {
                Polygon target;
                if (!(iss >> target)) {
                    throw std::invalid_argument("");
                }
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                BBox globalBox = std::accumulate(
                    polygons.begin(), polygons.end(), BBox{}, expandBBoxWithPolygon);
                BBox targetBox = std::accumulate(
                    target.points.begin(), target.points.end(), BBox{}, expandBBoxWithPoint);
                bool isInFrame = false;
                if (!polygons.empty()) {
                    isInFrame = (targetBox.minX >= globalBox.minX &&
                                 targetBox.maxX <= globalBox.maxX &&
                                 targetBox.minY >= globalBox.minY &&
                                 targetBox.maxY <= globalBox.maxY);
                }
                std::cout << (isInFrame ? "<TRUE>" : "<FALSE>") << '\n';
            } else if (cmd == "MAXSEQ") {
                Polygon target;
                if (!(iss >> target)) {
                    throw std::invalid_argument("");
                }
                if (iss >> std::ws && !iss.eof()) {
                    throw std::invalid_argument("");
                }
                MaxSeqState res = std::accumulate(
                    polygons.begin(), polygons.end(), MaxSeqState{0, 0},
                    std::bind(seqAccumulator, std::placeholders::_1, std::placeholders::_2, target));
                std::cout << res.max << '\n';
            } else {
                throw std::invalid_argument("");
            }
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
