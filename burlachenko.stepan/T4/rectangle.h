#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
private:
    Point bottomLeft_;
    Point topRight_;
public:
    Rectangle(const Point& bottomLeft, const Point& topRight);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& newCenter) override;
    void scale(double ratio) override;
    std::string getName() const override;    
};

#endif