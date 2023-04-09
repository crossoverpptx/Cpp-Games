#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point() {}
    Point(const int x, const int y) : x(x), y(y) {}
    void Print();
    void Clear();
    int GetX() { return this->x; }
    int GetY() { return this->y; }
    void ChangePosition(const int x, const int y);
    void PrintCircular();
    bool operator== (const Point& point) { return (point.x == this->x) && (point.y == this->y); }
private:
    int x, y;
};

#endif // POINT_H