#pragma once
#include <string>
#include <iostream>

// 一个二维向量结构体，用于表示坐标
struct Vec2 {
    // 这里的联合体类似于引用，这些变量竖排上使用了同一个内存。
    union {
        // 有效数据
        double data[2];
        // 坐标系变量别名
        struct { double x, y; };
        // 宽高变量别名
        struct { double w, h; };
        // 宽高变量别名2
        struct { double width, height; };
        struct { double left, right; };
    };
    Vec2(double x = -1, double y = -1) : x(x), y(y) {}


    // 用于输出Vec2的函数
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    std::string to_string() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }
};

// 一个二维向量结构体，用于表示坐标
struct Vec2Int {
    // 这里的联合体类似于引用，这些变量竖排上使用了同一个内存。
    union {
        // 有效数据
        int data[2];
        // 坐标系变量别名
        struct { int x, y; };
        // 宽高变量别名
        struct { int w, h; };
        // 宽高变量别名2
        struct { int width, height; };
        struct { int left , right; };
    };
    Vec2Int(int x = -1, int y = -1) : x(x), y(y) {}

    // 用于输出Vec2Int的函数
    friend std::ostream& operator<<(std::ostream& os, const Vec2Int& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    std::string to_string() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    Vec2Int operator+(const Vec2Int& other) const {
        return Vec2Int(x + other.x, y + other.y);
    }
    Vec2Int operator-(const Vec2Int& other) const {
        return Vec2Int(x - other.x, y - other.y);
    }
    bool operator==(const Vec2Int& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2Int& other) const {
        return !(*this==other);
    }
};