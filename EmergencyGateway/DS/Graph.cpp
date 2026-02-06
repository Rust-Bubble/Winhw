#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <queue>
#include <unordered_map>
// 1. 基本操作实现

// 添加节点
void Graph::addVertex(const Vertex& vertex) {
    // 检查是否已存在
    if (findVertexIndex(vertex.id) != -1) {
        std::cout << "顶点ID " << vertex.id << " 已存在！" << std::endl;
        return;
    }
    vertices.push_back(vertex);
    idToIndexMap[vertex.id] = vertices.size() - 1;   // 添加映射：新顶点的索引是 size()-1
    adjacencyList[vertex.id] = std::vector<Edge>();       // 在邻接表中初始化这个点，下标是该点的ID
    std::cout << "添加顶点: " << vertex.name << " (" << vertex.id << ")" << std::endl;

    return;
}
// 添加边
void Graph::addEdge(const Edge& edge) {
    // 检查顶点是否存在
    if (findVertexIndex(edge.from) == -1 || findVertexIndex(edge.to) == -1) {
        std::cout << "边端点不存在！" << std::endl;
        return;
    }

    // 检查是否在添加自环边（from == to）
    if (edge.from == edge.to) {
        std::cout << "错误：不允许自环边！" << std::endl;
        return;
    }

    // 检查边是否已存在（包括反向）
    bool edgeExists = false;
    Edge existingEdge;

    // 检查正向边
    if (adjacencyList.find(edge.from) != adjacencyList.end()) {
        for (const Edge& e : adjacencyList[edge.from]) {
            if (e.to == edge.to) {
                edgeExists = true;
                existingEdge = e;
                break;
            }
        }
    }

    if (edgeExists) {
        // 边已存在，打印警告并更新
        std::cout << "警告：边 " << edge.from << " <-> " << edge.to
            << " 已存在（距离: " << existingEdge.distance << "m）"
            << "，更新为 " << edge.distance << "m" << std::endl;

        // 删除旧边（正向）
        for (auto it = adjacencyList[edge.from].begin();
            it != adjacencyList[edge.from].end(); ++it) {
            if (it->to == edge.to) {
                adjacencyList[edge.from].erase(it);
                break;
            }
        }

        // 删除旧边（反向）
        for (auto it = adjacencyList[edge.to].begin();
            it != adjacencyList[edge.to].end(); ++it) {
            if (it->to == edge.from) {
                adjacencyList[edge.to].erase(it);
                break;
            }
        }
    }

    // 创建反向边
    Edge reverseEdge = edge;
    reverseEdge.from = edge.to;
    reverseEdge.to = edge.from;

    // 添加到邻接表
    adjacencyList[edge.from].push_back(edge);
    adjacencyList[edge.to].push_back(reverseEdge);

    if (!edgeExists) {
        std::cout << "添加边: " << getVertex(edge.from)->name << " <-> " << getVertex(edge.to)->name
            << " (距离: " << edge.distance << "m)" << std::endl;
    }
}
// 删除节点
void Graph::removeVertex(int id) {
    int index = findVertexIndex(id);
    if (index == -1) {
        std::cout << "顶点ID " << id << " 不存在！" << std::endl;
        return;
    }

    std::string vertexName = vertices[index].name;
    std::cout << "删除顶点: " << vertexName << " (" << id << ")" << std::endl;

    // 1. 先删除所有与该顶点相连的边
    std::vector<int> neighbors;
    if (adjacencyList.find(id) != adjacencyList.end()) {
        // 收集所有邻居ID
        for (const Edge& edge : adjacencyList[id]) {
            neighbors.push_back(edge.to);
        }
    }

    // 删除与邻居的连接
    for (int neighborId : neighbors) {
        removeEdge(id, neighborId);
    }

    // 2. 从邻接表中删除该顶点
    adjacencyList.erase(id);

    // 3. 从顶点列表中删除该顶点
    vertices.erase(vertices.begin() + index);

    // 4. 重新构建idToIndexMap（简单可靠的方法）
    idToIndexMap.clear();
    for (size_t i = 0; i < vertices.size(); i++) {
        idToIndexMap[vertices[i].id] = static_cast<int>(i);
    }

    std::cout << "顶点 " << vertexName << " 删除完成" << std::endl;
    return;
}
// 删除边
void Graph::removeEdge(int from, int to) {
    // 检查两个点是否存在
    int index1 = findVertexIndex(from);
    int index2 = findVertexIndex(to);
    if (index1 == -1 || index2 == -1) {
        if (index1 == -1) {
            std::cout << "顶点ID " << from << " 不存在！" << std::endl;
        }
        if (index2 == -1) {
            std::cout << "顶点ID " << to << " 不存在！" << std::endl;
        }
        return;
    }

    bool removed = false;
    std::cout << "删除从" << vertices[index1].name << "到" << vertices[index2].name << "的边" << std::endl;

    if (adjacencyList.find(from) != adjacencyList.end()) {
        for (size_t i = 0; i < adjacencyList[from].size(); i++) {
            if (adjacencyList[from][i].to == to) {
                adjacencyList[from].erase(adjacencyList[from].begin() + i);
                removed = true;
                i--; // 因为删除此处的边记录后，后面的元素会前移，因此标记也要前移
            }
        }
    }

    if (adjacencyList.find(to) != adjacencyList.end()) {
        for (size_t i = 0; i < adjacencyList[to].size(); i++) {
            if (adjacencyList[to][i].to == from) {
                adjacencyList[to].erase(adjacencyList[to].begin() + i);
                removed = true;
                i--; // 因为删除此处的边记录后，后面的元素会前移，因此标记也要前移
            }
        }
    }

    if (removed) {
        std::cout << "已删除从" << vertices[index1].name << "(" << from
            << ")到" << vertices[index2].name << "(" << to << ")的边" << std::endl;
    }
    else {
        std::cout << "边不存在：从" << vertices[index1].name << "(" << from
            << ")到" << vertices[index2].name << "(" << to << ")" << std::endl;
    }

    return;
}

bool Graph::generateList(const char* path){
    std::cout << "[Graph::generateList]生成地图，自" << path << std::endl;
    using namespace std;
    const char*& filepath = path;
    //读取点数据
        //读取文件，分离出各个属性
    ifstream file(filepath);
    if(!file){
        std::cout << "[Graph::generateList]打开点文件失败，请检查点地图文件位置" << std::endl;
        return false;
    }
    int size;
    string line, rsize, head;

    //读取点数
    getline(file, rsize);
    size = stoi(rsize);
    //读取表头
    getline(file, head);
    //读取点数据
    for(int i = 0; i < size; i++)
    {
        getline(file, line);
        string name, rtype, rid, rx, ry, rib;
        int id, x, y;
        bool isburning;
        VertexType type;
        stringstream ss(line);
        getline(ss, rid, ',');
        id = stoi(rid.c_str());
        getline(ss, name, ',');
        getline(ss, rtype, ',');
        if(rtype == "TEACHING_BUILDING") type = VertexType::TEACHING_BUILDING;
        else if(rtype == "DORMITORY") type = VertexType::DORMITORY;
        else if(rtype == "EXIT")type = VertexType::EXIT;
        else type = VertexType::FLOOR_NODE;
        getline(ss, rx, ' ');
        getline(ss, ry, ',');
        x = stoi(rx);
        y = stoi(ry);
        Vec2Int pos(x, y);
        getline(ss, rib, '\n');
        if(rib == "0")isburning = false;
        else isburning = true;
        Vertex target = Vertex(id, name, type, pos, isburning);//使用属性创点
        this->addVertex(target); //把点加入图
    }

    //读取边数据
    int edgenum;
    //读取点数
    getline(file, rsize);
    edgenum = stoi(rsize);
    //读取表头
    getline(file, head);
    for(int i = 0; i < edgenum; i++)
    {
        getline(file, line);
        string edgetype, rbeg, rend, rcongestion, rrisklevel, rdistance;
        int beg_id, end_id, risklevel;
        double congestion, distance;
        stringstream ss(line);
        getline(ss, rbeg, ',');
        getline(ss, rend, ',');
        beg_id = stoi(rbeg);
        end_id = stoi(rend);
        getline(ss, edgetype, ',');
        EdgeType tp;
        if(edgetype == "road") tp = EdgeType::ROAD;
        else if(edgetype == "stair") tp = EdgeType::STAIR;
        else tp = EdgeType::CORRIDOR;
        getline(ss, rcongestion, ',');
        congestion = stod(rcongestion);
        getline(ss, rdistance, ',');
        distance = stod(rdistance);
        getline(ss, rrisklevel, '\n');
        risklevel = stoi(rrisklevel);
        addEdge(Edge(beg_id, end_id, distance, tp, risklevel, congestion));
    }
    std::cout << "[Graph::generateList]地图生成成功" << std::endl;
    return true;
}

// 2.查询操作实现

// 根据id，获取节点的指针
Vertex* Graph::getVertex(int id) {
    int index = findVertexIndex(id);
    if (index == -1) return nullptr;
    return &vertices[index];
}
const Vertex* Graph::getVertex(int id) const {
    int index = findVertexIndex(id);
    return (index != -1) ? &vertices[index] : nullptr;
}

// 根据id获取顶点索引
int Graph::findVertexIndex(int id) const {
    auto it = idToIndexMap.find(id);
    return (it != idToIndexMap.end()) ? it->second : -1;
}

// 根据id获取顶点名称
std::string Graph::getVertexName(int id) const {
    const Vertex* vertex = getVertex(id);
    return vertex ? vertex->name : "";
}

// 根据id获取顶点坐标
Vec2Int Graph::getVertexPosition(int id) const {
    const Vertex* vertex = getVertex(id);
    return vertex ? vertex->position : Vec2Int(-1, -1);
}

// 根据顶点索引，获取从某个顶点出发的所有边，返回边的列表（vector）
std::vector<Edge> Graph::getEdgesFrom(int vertexId) const {
    auto it = adjacencyList.find(vertexId);
    return (it != adjacencyList.end()) ? it->second : std::vector<Edge>();
}

// 获取所有顶点的ID列表（vector）
std::vector<int> Graph::getAllVertexIds() const {
    std::vector<int> ids;
    for(const auto& vertex : vertices){
        ids.push_back(vertex.id);
    }
    return ids;
}

// 根据顶点类型，获取对应的顶点ID列表
std::vector<int> Graph::getVerticesByType(VertexType type) const {
    std::vector<int> result;
    for (const auto& vertex : vertices) {
        if (vertex.type == type) {
            result.push_back(vertex.id);
        }
    }
    return result;
}

// 获取顶点数量
int Graph::getVertexCount() const {
    return vertices.size();
}
// 获取边数量
int Graph::getEdgeCount() const {
    int count = 0;
    for (const auto& pair : adjacencyList) {
        count += pair.second.size();
    }
    return (count / 2);  // 每条边被记录了两次
}
// 根据顶点类型，获取对应的顶点数量
int Graph::countVerticesByType(VertexType type) const {
    int count = 0;
    for (const auto& vertex : vertices) {
        if (vertex.type == type) {
            count++;
        }
    }
    return count;
}

// 获取顶点类型的字符串表示
std::string Graph::getVertexTypeString(VertexType type) {
    switch (type) {
    case VertexType::TEACHING_BUILDING: return "教学楼";
    case VertexType::DORMITORY: return "宿舍";
    case VertexType::EXIT: return "出口";
    case VertexType::FLOOR_NODE: return "楼层节点";
    default: return "未知类型";
    }
}

// 计算路径权重
double Graph::calculatePathWeight(const std::vector<int>& path, bool emergencyMode) const {
    if (path.size() < 2) return 0.0;

    double totalWeight = 0.0;
    for (size_t i = 0; i < path.size() - 1; i++) {
        int from = path[i];
        int to = path[i + 1];

        // 查找边并计算权重
        bool edgeFound = false;
        if (adjacencyList.find(from) != adjacencyList.end()) {
            for (const Edge& edge : adjacencyList.at(from)) {
                if (edge.to == to) {
                    totalWeight += edge.getWeight(emergencyMode);
                    edgeFound = true;
                    break;
                }
            }
        }

        if (!edgeFound) {
            std::cout << "警告：路径中顶点 " << from << " 到 " << to << " 的边不存在！" << std::endl;
            return std::numeric_limits<double>::max();  // 返回极大值表示路径无效
        }
    }

    return totalWeight;
}

// 打印图的基本信息
void Graph::printGraph() const {
    std::cout << "========== 图结构信息 ==========" << std::endl;
    std::cout << "顶点数量: " << getVertexCount() << std::endl;
    std::cout << "边数量: " << getEdgeCount() << std::endl;

    // 按类型统计顶点
    std::cout << "\n顶点类型统计:" << std::endl;
    for (int i = 0; i < 4; i++) {
        VertexType type = static_cast<VertexType>(i);
        int count = countVerticesByType(type);
        if (count > 0) {
            std::cout << "  " << getVertexTypeString(type) << ": " << count << "个" << std::endl;
        }
    }

    std::cout << "\n顶点列表:" << std::endl;
    for (const auto& vertex : vertices) {
        std::cout << "  [ID:" << vertex.id << "] " << vertex.name
            << " (类型:" << getVertexTypeString(vertex.type)
            << ", 坐标:" << vertex.position.to_string()
            << ", 着火:" << (vertex.isBurning ? "是" : "否") << ")" << std::endl;
    }

    std::cout << "\n邻接表:" << std::endl;
    for (const auto& pair : adjacencyList) {
        if (!pair.second.empty()) {
            std::cout << "  顶点 [" << pair.first << "]:" << getVertexName(pair.first) << " -> ";
            for (const Edge& edge : pair.second) {
                std::cout << edge.to << "(" << edge.distance << "m";
                if (edge.riskLevel > 0) std::cout << ", 风险:" << edge.riskLevel;
                if (edge.congestion > 0) std::cout << ", 拥堵:" << edge.congestion;
                std::cout << ") ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "=================================" << std::endl;
}

// 3.核心算法

using ELP = Graph::ExitID_Length_Path;
// 使用Dijkstra算法查找最短路径
ELP Graph::findShortestPath(int startId, int endId, bool emergencyMode) const{
    using ELP = Graph::ExitID_Length_Path;
    // 1. 参数验证
    if(findVertexIndex(startId) == -1) {
        std::cout << "起点ID " << startId << " 不存在！" << std::endl;
        return ELP();
    }
    if(findVertexIndex(endId) == -1) {
        std::cout << "终点ID " << endId << " 不存在！" << std::endl;
        return ELP();
    }

    // 2. 起点和终点相同的情况
    if(startId == endId) {
        std::cout << "起点和终点相同！" << std::endl;
        return ELP();
    }

    // 3. 特殊检查：如果终点着火且是紧急模式，直接返回不可达
    if(emergencyMode) {
        const Vertex* endVertex = getVertex(endId);
        if(endVertex && endVertex->isBurning) {
            std::cout << "终点着火，不可达！" << std::endl;
            return ELP();
        }
    }

    // 4. 预检查连通性
    if(!isConnected(startId, endId, emergencyMode)) {
        std::cout << "从 " << getVertex(startId)->name << " 到 "
            << getVertex(endId)->name << " 不连通！" << std::endl;
        return ELP();
    }

    // 5. 初始化数据结构
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> prev;
    std::unordered_map<int, bool> visited;
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> pq;

    // 初始化所有节点
    for(const auto& vertex : vertices) {
        dist[vertex.id] = std::numeric_limits<double>::max();
        prev[vertex.id] = -1;
        visited[vertex.id] = false;
    }

    // 设置起点
    dist[startId] = 0.0;
    pq.push({startId, 0.0});

    // 6. Dijkstra主循环
    while(!pq.empty()) {
        DijkstraNode current = pq.top();
        pq.pop();

        int u = current.id;

        // 跳过已访问的节点
        if(visited[u]) continue;
        visited[u] = true;

        // 如果当前节点就是终点，提前结束
        if(u == endId) {
            break;
        }

        // 遍历所有邻接边
        if(adjacencyList.find(u) != adjacencyList.end()) {
            for(const Edge& edge : adjacencyList.at(u)) {
                //边终点坐标
                int v = edge.to;

                // 如果邻接节点已访问，跳过
                if(visited[v]) continue;

                // 在紧急模式下，如果邻接节点着火，跳过该边
                if(emergencyMode) {
                    const Vertex* neighbor = getVertex(v);
                    if(neighbor && neighbor->isBurning) {
                        continue;
                    }
                }

                // 计算边的实际权重
                double weight = edge.getWeight(emergencyMode);
                double newDist = dist[u] + weight;

                // 如果找到更短路径
                if(newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    pq.push({v, newDist});
                }
            }
        }
    }

    // 7. 检查是否可达（理论上应该可达，因为预检查过，但保留作为防护）
    if(dist[endId] == std::numeric_limits<double>::max()) {
        std::cout << "从 " << startId << " 到 " << endId << " 不可达" << std::endl;
        return ELP();
    }

    // 8. 重建路径（从终点到起点）
    std::vector<int> path;
    for(int at = endId; at != -1; at = prev[at]) {
        path.push_back(at);

        // 安全防护：防止无限循环
        if(path.size() > vertices.size() + 1) {
            std::cout << "路径重建出现循环！" << std::endl;
            return ELP();
        }
    }

    // 反转路径（起点->终点）
    reverse(path.begin(), path.end());

    // 路径重构完成后，根据path向量重建边集合
    std::vector<Edge> finalEdges;
    for(size_t i = 0; i < path.size() - 1; i++) {
        int from = path[i];
        int to = path[i + 1];

        // 在邻接表中查找对应的边
        for(const Edge& edge : adjacencyList.at(from)) {
            if(edge.to == to) {
                finalEdges.push_back(edge);
                break;
            }
        }
    }


    // 9. 验证路径有效性
    if(path.empty() || path[0] != startId) {
        std::cout << "路径重建错误：起点不匹配" << std::endl;
        return ELP();
    }

    //输出点解决方案
    std::cout << "点解决方案： ";
    for(int i = 0; i < path.size(); i++)
    {
        std::cout << path[i];
        if(i != path.size() - 1) std::cout << "->";
        else std::cout << std::endl;
    }
    //输出边解决方案
    std::cout << "边解决方案： ";
    for(int i = 0; i < finalEdges.size(); i++)
    {
        std::cout << "(" << finalEdges[i].from << "," << finalEdges[i].to << ")";
        if(i != finalEdges.size() - 1) std::cout << "->";
        else std::cout << std::endl;
    }
    double length = calculatePathWeight(path, emergencyMode);
    ELP solution = ELP(endId, length, move(path), move(finalEdges));
    return solution;//直接输出elp解决方案
}

// 多出口疏散方案
std::vector<Graph::ExitID_Length_Path> Graph::findMultipleExits(
    int startId, bool emergencyMode
)const{
    using ELP = Graph::ExitID_Length_Path;
    std::vector<ELP> results;

    // 获取所有出口
    std::vector<int> exitIds = getVerticesByType(VertexType::EXIT);
    if(exitIds.empty()) {
        std::cout << "系统中没有出口！" << std::endl;
        return results;
    }

    // 计算到每个出口的路径
    for(int exitId : exitIds) {
        /*std::vector<int> path = findShortestPath(startId, exitId, emergencyMode);
        results.reserve(exitIds.size());
        if (!path.empty()) {
            double totalWeight = calculatePathWeight(path, emergencyMode);
            results.emplace_back(exitId, totalWeight, std::move(path));
        }*/
        results.emplace_back(findShortestPath(startId, exitId, emergencyMode));
    }

    // 按权重排序
    std::sort(
        results.begin(), results.end(),
        [](const ELP& a, const ELP& b){return a.length < b.length; }
    );

    return results;
}

// 可达性检测
bool Graph::isAreaReachable(int areaId, bool emergencyMode) {
    // 1. 检查目标区域是否存在
    Vertex* target = getVertex(areaId);
    if (!target) {
        std::cout << "区域ID " << areaId << " 不存在！" << std::endl;
        return false;
    }

    // 2. 获取所有出口
    std::vector<int> exitIds = getVerticesByType(VertexType::EXIT);
    if (exitIds.empty()) {
        std::cout << "系统中没有出口！" << std::endl;
        return false;
    }

    // 3. 使用 isConnected 检查连通性（更高效）
    for (int exitId : exitIds) {
        // 紧急模式下检查出口是否着火
        if (emergencyMode) {
            Vertex* exitVertex = getVertex(exitId);
            if (exitVertex && exitVertex->isBurning) {
                continue;  // 这个出口着火，跳过
            }
        }

        // 使用 BFS 检查连通性（比 Dijkstra 更快）
        if (isConnected(areaId, exitId, emergencyMode)) {
            std::cout << "区域 " << target->name << " 可到达出口 "
                << getVertex(exitId)->name << std::endl;
            return true;
        }
    }

    // 4. 所有出口都无法到达
    std::cout << "区域 " << target->name << " 无法到达任何出口" << std::endl;
    return false;
}

// 使用BFS检测两个顶点是否连通（不考虑路径长度）
bool Graph::isConnected(int startId, int endId, bool emergencyMode) const {
    // 1. 参数验证
    if (findVertexIndex(startId) == -1 || findVertexIndex(endId) == -1) {
        return false;
    }

    // 2. 起点和终点相同
    if (startId == endId) {
        return true;
    }

    // 3. 紧急模式特殊检查
    if (emergencyMode) {
        const Vertex* endVertex = getVertex(endId);
        if (endVertex && endVertex->isBurning) {
            return false;  // 终点着火不可达
        }
    }

    // 4. BFS遍历
    std::unordered_map<int, bool> visited;
    std::queue<int> q;

    visited[startId] = true;
    q.push(startId);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 如果到达终点
        if (current == endId) {
            return true;
        }

        // 遍历所有邻居
        auto it = adjacencyList.find(current);
        if (it != adjacencyList.end()) {
            for (const Edge& edge : it->second) {
                int neighbor = edge.to;

                // 跳过已访问的节点
                if (visited[neighbor]) continue;

                // 紧急模式下，跳过着火邻居
                if (emergencyMode) {
                    const Vertex* neighborVertex = getVertex(neighbor);
                    if (neighborVertex && neighborVertex->isBurning) {
                        continue;
                    }
                }

                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    // 5. BFS结束未找到终点
    return false;
}

Graph::Graph(Graph&& that)noexcept
    : vertices(std::move(that.vertices))
    , adjacencyList(std::move(that.adjacencyList))
    , idToIndexMap(std::move(that.idToIndexMap)){}

Graph& Graph::operator=(Graph&& that)noexcept{
    if(this != &that){
        this->vertices      = std::move(that.vertices);
        this->adjacencyList = std::move(that.adjacencyList);
        this->idToIndexMap  = std::move(that.idToIndexMap);
    }
    return *this;
}

