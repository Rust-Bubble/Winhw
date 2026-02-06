#pragma once
#include"head.h"
enum class EdgeType {
    STAIR,              // 楼梯
    CORRIDOR,           // 走廊
    ROAD 			    // 道路
};

struct Edge {
    int from;              // 起点ID
    int to;                // 终点ID
    double distance;       // 距离（米）
    EdgeType type;         // 边的类型
    int riskLevel;         // 安全风险等级：0-10, 0安全
    double congestion;     // 拥堵系数：0.0-1.0，0不拥堵
    

    // 构建边的函数。注意：对于每一个id，不要使用负数。
    // @param from 起点ID，-1表示无效
    // @param to 终点ID，-1
    // @param distance 边长度（米）
    // @param type 边的类型
    // @param riskLevel 安全风险等级。取值范围是[0（安全）,10（危险）]。
    // @param congestion 拥堵系数。取值范围是[0.0（通畅无阻）,1.0（极其拥堵）]。
    Edge(int from = -1, int to = -1, double distance = 0.0, EdgeType type = EdgeType::ROAD, int riskLevel = 0, double congestion = 0.0) :
        from(from), to(to), distance(distance), type(type), riskLevel(riskLevel), congestion(congestion){
        
    }


    // 计算边的权重（暂时都只用“距离”作为权重）
    // @param isEmergency 是否处于紧急模式
    // @return 边的权重值。目前，是否紧急情况，距离都是一样的。
    double getWeight(bool isEmergency = false) const {
        if (isEmergency) return 1e5;//若为紧急情况，道路不可通行
        else return riskLevel + congestion + distance;//若不紧急，采用三属性和为最终权值
    }

};