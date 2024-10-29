#include "Vector2D.hpp"

Vector2D::Vector2D(){
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(float x, float y){
    this->x = x;
    this->y = y;
}


Vector2D& Vector2D::Add(const Vector2D& v){
    x += v.x;
    y += v.y;
    return *this;
}
Vector2D& Vector2D::Subtract(const Vector2D& v){
    x -= v.x;
    y -= v.y;
    return *this;
}
Vector2D& Vector2D::Multiply(const Vector2D& v){
    x *= v.x;
    y *= v.y;
    return *this;
}
Vector2D& Vector2D::Divide(const Vector2D& v){
    x /= v.x;
    y /= v.y;
    return *this;
}


Vector2D operator+ (const Vector2D& v1, const Vector2D& v2){
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}
Vector2D operator- (const Vector2D& v1, const Vector2D& v2){
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}
Vector2D operator* (const Vector2D& v1, const Vector2D& v2){
    return Vector2D(v1.x * v2.x, v1.y * v2.y);
}
Vector2D operator/ (const Vector2D& v1, const Vector2D& v2){
    return Vector2D(v1.x / v2.x, v1.y / v2.y);
}

Vector2D& Vector2D::operator+= (const Vector2D& v){
    return this->Add(v);
}
Vector2D& Vector2D::operator-= (const Vector2D& v){
    return this->Subtract(v);
}
Vector2D& Vector2D::operator*= (const Vector2D& v){
    return this->Multiply(v);
}
Vector2D& Vector2D::operator/= (const Vector2D& v){
    return this->Divide(v);
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& v){
    stream << "(" << v.x << "," << v.y << ")";
    return stream;
}
