#include <iostream>
#include "Rectangle.h"
#include "Square.h"
#include "Ring.h"
#include "Composite_shape.h"

int main()
{
    std::cout << "\n\n=== RECTANGLE ===\n";
    Rectangle rec(Point{ 1, 1 }, Point{ 4, 3 });
    std::cout << "A (x, y) = " << rec.getX_min() << "\t" << rec.getY_min() << std::endl;
    std::cout << "B (x, y) = " << rec.getX_max() << "\t" << rec.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << rec.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    Point center_rec = rec.getCenter();
    std::cout << "center (x) = " << center_rec.x << std::endl;
    std::cout << "center (y) = " << center_rec.y << std::endl;
    std::cout << "______________________________________________\n";
    rec.move(2, 2);
    std::cout << "A (x, y) = " << rec.getX_min() << "\t" << rec.getY_min() << std::endl;
    std::cout << "B (x, y) = " << rec.getX_max() << "\t" << rec.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    rec.scale(2);
    std::cout << "A (x, y) = " << rec.getX_min() << "\t" << rec.getY_min() << std::endl;
    std::cout << "B (x, y) = " << rec.getX_max() << "\t" << rec.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << rec.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    center_rec = rec.getCenter();
    std::cout << "center (x) = " << center_rec.x << std::endl;
    std::cout << "center (y) = " << center_rec.y << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << rec.getName();
   
    std::cout << "\n\n=== SQUARE ===\n";
    Square sq(Point{ 1, 1 }, 3);
    std::cout << "Left bottom (x, y) = " << sq.getX_min() << "\t" << sq.getY_min() << std::endl;
    std::cout << "Right top (x, y) = " << sq.getX_max() << "\t" << sq.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << sq.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    Point center_sq = sq.getCenter();
    std::cout << "center (x) = " << center_sq.x << std::endl;
    std::cout << "center (y) = " << center_sq.y << std::endl;
    std::cout << "______________________________________________\n";
    sq.move(2, 2);
    std::cout << "After move(2,2):\n";
    std::cout << "Left bottom (x, y) = " << sq.getX_min() << "\t" << sq.getY_min() << std::endl;
    std::cout << "Right top (x, y) = " << sq.getX_max() << "\t" << sq.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    sq.scale(2);
    std::cout << "After scale(2):\n";
    std::cout << "Left bottom (x, y) = " << sq.getX_min() << "\t" << sq.getY_min() << std::endl;
    std::cout << "Right top (x, y) = " << sq.getX_max() << "\t" << sq.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << sq.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    center_sq = sq.getCenter();
    std::cout << "center (x) = " << center_sq.x << std::endl;
    std::cout << "center (y) = " << center_sq.y << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << sq.getName();

    std::cout << "\n\n=== RING ===\n";
    Ring ring(Point{ 2, 2 }, 5, 3);
    std::cout << "Bounding box:\n";
    std::cout << "min X = " << ring.getX_min() << "\tmin Y = " << ring.getY_min() << std::endl;
    std::cout << "max X = " << ring.getX_max() << "\tmax Y = " << ring.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << ring.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    Point center_ring = ring.getCenter();
    std::cout << "center (x) = " << center_ring.x << std::endl;
    std::cout << "center (y) = " << center_ring.y << std::endl;
    std::cout << "______________________________________________\n";
    ring.move(2, 2);
    std::cout << "After move(2,2):\n";
    std::cout << "center (x) = " << ring.getCenter().x << std::endl;
    std::cout << "center (y) = " << ring.getCenter().y << std::endl;
    std::cout << "Bounding box:\n";
    std::cout << "min X = " << ring.getX_min() << "\tmin Y = " << ring.getY_min() << std::endl;
    std::cout << "max X = " << ring.getX_max() << "\tmax Y = " << ring.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    ring.scale(2);
    std::cout << "After scale(2):\n";
    std::cout << "center (x) = " << ring.getCenter().x << std::endl;
    std::cout << "center (y) = " << ring.getCenter().y << std::endl;
    std::cout << "Bounding box:\n";
    std::cout << "min X = " << ring.getX_min() << "\tmin Y = " << ring.getY_min() << std::endl;
    std::cout << "max X = " << ring.getX_max() << "\tmax Y = " << ring.getY_max() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << "Area = " << ring.getArea() << std::endl;
    std::cout << "______________________________________________\n";
    std::cout << ring.getName();
    std::cout << "\n______________________________________________\n\n";

    CompositeShape comp;
    comp.addShape(std::make_unique<Rectangle>(Point{ 1, 1 }, Point{ 4, 3 }));
    comp.addShape(std::make_unique<Square>(Point{ 2, 2 }, 3));
    comp.addShape(std::make_unique<Ring>(Point{ 0, 0 }, 5, 3));
    comp.addShape(std::make_unique<Rectangle>(Point{ 0, 0 }, Point{ 2, 2 }));
    comp.addShape(std::make_unique<Square>(Point{ 3, 3 }, 2));
    comp.print();

    return 0;
}
