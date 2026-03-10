#include "composite_shape.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Cannot add null shape to CompositeShape");
    }
    shapes.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double totalArea = 0;
    for (const auto& shape : shapes) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const {
    if (shapes.empty()) {
        return Point(0, 0);
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }

    return Point((minX + maxX) / 2, (minY + maxY) / 2);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point compositeCenter = getCenter();

    for (auto& shape : shapes) {
        Point shapeCenter = shape->getCenter();
        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;

        shape->move(dx * (factor - 1), dy * (factor - 1));
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::printInfo() const {
    Point center = getCenter();
    printf("[COMPOSITE, (%.2f, %.2f), %.2f:\n", center.x, center.y, getArea());

    for (size_t i = 0; i < shapes.size(); ++i) {
        Point shapeCenter = shapes[i]->getCenter();
        printf("  %s, (%.2f, %.2f), %.2f",
            shapes[i]->getName().c_str(),
            shapeCenter.x, shapeCenter.y,
            shapes[i]->getArea());

        if (i < shapes.size() - 1) {
            printf(",\n");
        } else {
            printf("\n");
        }
    }
    printf("]\n");
}
