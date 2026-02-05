#pragma once
#include <vector>
#include <unordered_map>
#include "Vertex.h"
#include "Edge.h"

#define Maxsize 50

class Graph {
private:
    std::vector<Vertex> vertices;  // 所有顶点，用vector存储
    std::unordered_map<int, std::vector<Edge>> adjacencyList;  // 所有边，邻接表，用unordered_map存储
    std::unordered_map<int, int> idToIndexMap;   // 顶点ID到索引的映射，方便快速查找
    
public:
    struct ExitID_Length_Path{
        int exitID;
        double length;
        std::vector<int> path;
        ExitID_Length_Path(int _exitid, double _length, std::vector<int>&& _path)
            : exitID(_exitid), length(_length), path(std::move(_path)){};
    };
    /////////////////////////////
    // 1.编辑操作

        // 添加一个节点。
        // 在具体的使用过程中，可以如此编写：
        // 0 |Vertex vert(...); // 先设置好节点的属性
        // 1 |graph.addVertex(vert);
        // @param vertex 添加的节点。
    void addVertex(const Vertex& vertex);      // 添加节点

    // 添加一条边
    // 在具体使用的过程中，可以如此编写：
    // 0 |Edge edge(...); // 先设置好边的属性
    // 1 |graph.addEdge(edge);
    // @param edge 添加的边。
    void addEdge(const Edge& edge);            // 添加边

    // 根据节点id，删除节点。
    // 为保证不悬挂引用，本函数会顺便删除连接的边。
    // @param id 节点id。
    void removeVertex(int id);                 // 删除节点

    // 根据两个节点的id，删除一条边。必须确保节点与边是存在的
    // @param from 边的起始节点id。
    // @param to 边的结束节点id。
    void removeEdge(int from, int to);         // 删除边

    //读取地图，并把地图为邻接表,参数是选择的地图的id
    bool generateList(const char* path);

    ///////////////////////////////
    // 2.查询操作

        // 根据id，获取顶点。可以用来检验这个边是否存在的。
        // @param id 节点id。
        // @return 节点指针。如果节点不存在，则返回nullptr。
    Vertex* getVertex(int id);                        // 根据id，获取节点的指针
    const Vertex* getVertex(int id) const;

    // 根据id，获取顶点索引。
    // @param id 节点id。
    // @return 节点索引。如果节点不存在，则返回-1。
    // @deprecated 。
    int findVertexIndex(int id) const;                // 根据id，获取顶点索引

    // 根据id，获取顶点名称。
    // @param id 节点id。
    // @return 节点名称。如果节点不存在，则返回""。
    std::string getVertexName(int id) const;               // 根据id，获取顶点名称

    // 根据id，获取顶点坐标。
    // @param id 节点id。
    // @return 节点坐标。如果节点不存在，则返回Vec2(-1, -1)。
    Vec2Int getVertexPosition(int id) const;             // 根据id，获取顶点坐标

    // 根据id，获取其触发所链接的列表。
    // @param vertexId 顶点索引。
    // @return 边列表。如果顶点不存在，则返回空列表。
    std::vector<Edge> getEdgesFrom(int vertexId) const;    // 根据顶点索引，获取从某个顶点出发的所有边，返回边的列表（vector）

    // 获取所有顶点的ID列表
    // @return 顶点ID列表。
    std::vector<int> getAllVertexIds() const;              // 获取所有顶点的ID列表（vector）

    // 根据顶点类型，获取对应的顶点ID列表
    // @param type 顶点类型。
    // @return 顶点ID列表。
    std::vector<int> getVerticesByType(VertexType type) const;   // 根据顶点类型，获取对应的顶点ID列表

    int getVertexCount() const;                // 获取顶点数量
    int getEdgeCount() const;                  // 获取边数量
    int countVerticesByType(VertexType type) const;   // 根据顶点类型，获取对应的顶点数量

    // 获取顶点类型的字符串表示（用于输出）
    static std::string getVertexTypeString(VertexType type);

    // 计算路径的权重.
    // @param path 最后导出的路径。
    // @param isEmergency 是否为紧急疏散模式。
    // @return 路径的权重。
    double calculatePathWeight(const std::vector<int>& path, bool emergencyMode = false) const;   // 计算路径权重

    void printGraph() const;                   // 打印图的基本信息

    ///////////////////////////////
    // 3.核心算法
    std::vector<int> findShortestPath(int startId, int endId, bool emergencyMode = false)const;// 使用Dijkstra算法查找最短路径
    // 多出口疏散方案
    std::vector<ExitID_Length_Path> findMultipleExits(int startId, bool emergencyMode = false)const;
    bool isAreaReachable(int areaId, bool emergencyMode = false);                           // 出口可达性检测（判断“当前点”能否到达某个“出口”）
    bool isConnected(int startId, int endId, bool emergencyMode = false) const;             // 连通性检测（判断“两个点之间”是否连通）

    // Dijkstra算法辅助结构
    struct DijkstraNode {
        int id;
        double distance;
        bool operator>(const DijkstraNode& other) const {
            return distance > other.distance;
        }
    };


    // 默认构造函数
    Graph() = default;
    // 拷贝构造函数
    Graph(const Graph& other) {
        vertices = other.vertices;
        adjacencyList = other.adjacencyList;
        idToIndexMap = other.idToIndexMap;  // 需要复制映射表
    }
    // 拷贝赋值运算符
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            vertices = other.vertices;
            adjacencyList = other.adjacencyList;
            idToIndexMap = other.idToIndexMap;
        }
        return *this;
    }
    // 默认析构函数
    ~Graph() = default;

    Graph(Graph&&)noexcept;
    Graph& operator=(Graph&&)noexcept;

};