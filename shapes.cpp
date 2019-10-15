#include <iostream>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "shapes.hpp"


Polygon::Polygon(int vCount): vertexCount(vCount) {}

int Polygon::getVertexCount() const {
  return vertexCount;
}

std::string Polygon::toString() {
  std::ostringstream str;
  str << "Polygon [ vertexCount " << vertexCount << " ]";
  return str.str();
}

Circle::Circle(Vector center, float radius): Polygon(-1), center(center), radius(radius) {}

Vector& Circle::getCenter() {
  return center;
}

float Circle::getRadius() {
  return radius;
}

double Circle::getArea() {
  return M_PI * M_PI * radius;
}

double Circle::getPerimeter() {
  return 2 * M_PI * radius;
}

bool Circle::isIntersects(Polygon *other) {
  Circle *circle;
  if ((circle = dynamic_cast<Circle *>(other)) != nullptr) {
    auto centerDelta = sqrt(center.sq_dist(circle->center));
    return centerDelta <= circle->getRadius() + getRadius();
  }
  throw std::logic_error("Intersection with other shapes not implemented");
}

std::string Circle::toString() {
  std::ostringstream str;
  str << "Circle [ center x=" << center.x << " y=" << center.y << " radius " << radius << "]";
  return str.str();
}

RegularPolygon::RegularPolygon(int vCount, float angle, float edgeLength): Polygon(vCount), angle(angle), edgeLength(edgeLength) {}

double RegularPolygon::getPerimeter() {
  return edgeLength * vertexCount;
}

double RegularPolygon::getArea() {
  auto a = edgeLength / (2 * tan(M_PI / vertexCount));
  return getPerimeter() * a / 2;
}

std::string RegularPolygon::toString() {
  std::ostringstream str;
  str << "Regular Polygon [ vertexCount " << vertexCount << " edgeLength " << edgeLength << " angle " << angle << "]";
  return str.str();
}

Square::Square(Vector position, float edgeLength): RegularPolygon(4, M_PI_2, edgeLength), position(position) {}

bool Square::isIntersects(Polygon *other) {
  Square *square;
  if ((square = dynamic_cast<Square *>(other)) != nullptr) {
    auto dx = abs(square->position.x - position.x);
    auto dy = abs(square->position.y - position.y);
    return dx <= edgeLength && dy <= edgeLength;
  }
  
  Circle *circle;
  if((circle = dynamic_cast<Circle *>(other)) != nullptr) {
    auto dx = circle->getCenter().x - std::max((float) position.x, (float) std::min((float) circle->getCenter().x, position.x + edgeLength));
    auto dy = circle->getCenter().y - std::max((float) position.y, (float) std::min((float) circle->getCenter().y, position.y + edgeLength));
    return (dx * dx + dy * dy) < (circle->getRadius() * circle->getRadius());
  }
  
  throw std::logic_error("Intersection with other shapes not implemented");
}

std::string Square::toString() {
  std::ostringstream str;
  str << "Square [ position x=" << position.x << " y=" << position.y << " edgeLength " << edgeLength << "]";
  return str.str();
}
