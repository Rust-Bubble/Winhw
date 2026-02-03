#pragma once
#include"Vec2.h"

// 点的类型
enum class VertexType {
    TEACHING_BUILDING,  // 教学楼
    DORMITORY,          // 宿舍
    EXIT,               // 出口
    FLOOR_NODE,         // 楼层节点
};

// 点结构
struct Vertex {
    int id;                     // 唯一标识
    string name;                // 名称（如"一教101"）
    VertexType type;            // 点的类型
    Vec2 position;              // 用于图形显示的坐标。
    bool isBurning;             // 是否着火


    // 构造函数，创造一个节点信息。
    // @param id: 节点编号。-1表示无效。
    // @param name: 节点名称。默认情况下为空
    // @param type: 节点类型。
    // @param pos : 用于GUI的节点坐标。
    // @param isburning: 当前节点是否着火。
    Vertex(int id = -1, string name = "", VertexType type = VertexType::DORMITORY, Vec2 pos = Vec2(), bool isburning = false) :
        id(id), name(name), type(type), position(pos), isBurning(isburning) {
    }

};