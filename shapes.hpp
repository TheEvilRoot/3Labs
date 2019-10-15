#pragma once

struct Vector {
  int x;
  int y;
  
  double sq_dist(Vector &other) {
    auto a = abs(other.x - x);
    auto b = abs(other.y - y);
    return (a * a) + (b * b);
  }
};

class Polygon {
protected:
  int vertexCount;
public:
  Polygon(int vCount);
  
  int getVertexCount() const;
  
  virtual double getArea() = 0;
  
  virtual double getPerimeter() = 0;
  
  virtual bool isIntersects(Polygon *other) = 0;
  
  virtual std::string toString();
};

class Circle: public Polygon {
protected:
  Vector center;
  float radius;
public:
  Circle(Vector center, float radius);
  
  Vector& getCenter();
  
  float getRadius();
  
  double getArea() override;
  
  double getPerimeter() override;
  
  bool isIntersects(Polygon *other) override;
  
  std::string toString() override;
};

class RegularPolygon: public Polygon {
protected:
  float angle;
  float edgeLength;
public:
  RegularPolygon(int vCount, float angle, float edgeLength);
  double getPerimeter() override;
  
  double getArea() override;
  
  virtual bool isIntersects(Polygon *other) override = 0;
  
  std::string toString() override;
};

class Square: public RegularPolygon {
private:
  Vector position;
public:
  Square(Vector position, float edgeLength);
  
  bool isIntersects(Polygon *other) override;
  
  std::string toString() override;
};
