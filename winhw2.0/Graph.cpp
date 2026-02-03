#include "Graph.h"

// 1. 基本操作实现

// 添加节点
void Graph::addVertex(const Vertex& vertex) {
    // 检查是否已存在
    if (findVertexIndex(vertex.id) != -1) {
        cerr << "顶点ID " << vertex.id << " 已存在！" << endl;
        return;
    }
    vertices.push_back(vertex);
    idToIndexMap[vertex.id] = vertices.size() - 1;   // 添加映射：新顶点的索引是 size()-1
    adjacencyList[vertex.id] = vector<Edge>();       // 在邻接表中初始化这个点，下标是该点的ID
    cout << "添加顶点: " << vertex.name << " (" << vertex.id << ")" << endl;

    return;
}
// 添加边
void Graph::addEdge(const Edge& edge) {
    // 检查顶点是否存在
    if (findVertexIndex(edge.from) == -1 || findVertexIndex(edge.to) == -1) {
        cerr << "边端点不存在！" << endl;
        return;
    }

    // 检查是否在添加自环边（from == to）
    if (edge.from == edge.to) {
        cerr << "错误：不允许自环边！" << endl;
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
        cout << "警告：边 " << edge.from << " <-> " << edge.to
            << " 已存在（距离: " << existingEdge.distance << "m）"
            << "，更新为 " << edge.distance << "m" << endl;

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
        cout << "添加边: " << getVertex(edge.from)->name << " <-> " << getVertex(edge.to)->name
            << " (距离: " << edge.distance << "m)" << endl;
    }
}
// 删除节点
void Graph::removeVertex(int id) {
    int index = findVertexIndex(id);
    if (index == -1) {
        cerr << "顶点ID " << id << " 不存在！" << endl;
        return;
    }

    string vertexName = vertices[index].name;
    cout << "删除顶点: " << vertexName << " (" << id << ")" << endl;

    // 1. 先删除所有与该顶点相连的边
    vector<int> neighbors;
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

    cout << "顶点 " << vertexName << " 删除完成" << endl;
    return;
}
// 删除边
void Graph::removeEdge(int from, int to) {
    // 检查两个点是否存在
    int index1 = findVertexIndex(from);
    int index2 = findVertexIndex(to);
    if (index1 == -1 || index2 == -1) {
        if (index1 == -1) {
            cerr << "顶点ID " << from << " 不存在！" << endl;
        }
        if (index2 == -1) {
            cerr << "顶点ID " << to << " 不存在！" << endl;
        }
        return;
    }

    bool removed = false;
    cout << "删除从" << vertices[index1].name << "到" << vertices[index2].name << "的边" << endl;

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
        cout << "已删除从" << vertices[index1].name << "(" << from
            << ")到" << vertices[index2].name << "(" << to << ")的边" << endl;
    }
    else {
        cout << "边不存在：从" << vertices[index1].name << "(" << from
            << ")到" << vertices[index2].name << "(" << to << ")" << endl;
    }

    return;
}

//读取地图，创建邻接表
void Graph::generateList(string mapID)
{
    string dirpath = "Maps/map" + mapID + "/";
    //读取点数据
        //读取文件，分离出各个属性
    string srcpath = dirpath + "srcmap.csv";
    ifstream file(srcpath);
    if (!file)
    {
        cerr << "打开点文件失败，请检查点地图文件位置" << endl;
        return;
    }
    string line;
    bool ishead = true;
    while (getline(file, line)) {
        if (ishead)
        {
            ishead = false;
            continue;
        }
        string name, rtype, rid, rx, ry, rib;
        int id, x, y;
        bool isburning;
        VertexType type;
        stringstream ss(line);
        getline(ss, rid, ',');
        id = stoi(rid.c_str());
        getline(ss, name, ',');
        getline(ss, rtype, ',');
        if (rtype == "TEACHING_BUILDING") type = VertexType::TEACHING_BUILDING;
        else if (rtype == "DORMITORY") type = VertexType::DORMITORY;
        else if (rtype == "EXIT")type = VertexType::EXIT;
        else type = VertexType::FLOOR_NODE;
        getline(ss, rx, ' ');
        getline(ss, ry, ',');
        x = stod(rx);
        y = stod(ry);
        Vec2 pos = Vec2(x, y);
        getline(ss, rib, '\n');
        if (rib == "0")isburning = false;
        else isburning = true;
        //cout << id << " " << name << " " << pos << " " << rtype << ' ' << isburning << endl; 
        Vertex target = Vertex(id, name, type, pos, isburning);//使用属性创点
        addVertex(target); //把点加入图
    }
    int size = vertices.back().id;
    //读取各边距离
    string Disfpath = dirpath + "DisArr.txt";
    ifstream Disfile(Disfpath, ios::in);
    if (!Disfile)
    {
        cerr << "读取边距离出错，请检查文件位置！" << endl;
        return;
    }
    //读取各边拥堵程度
    string Jamfpath = dirpath + "JamArr.txt";
    ifstream Jamfile(Jamfpath, ios::in);
    if (!Jamfile)
    {
        cerr << "读取拥堵程度出错，请检查文件位置！" << endl;
        return;
    }
    //读取安全等级
    string Savefpath = dirpath + "SaveArr.txt";
    ifstream Savefile(Savefpath, ios::in);
    if (!Savefile)
    {
        cerr << "读取安全等级出错，请检查文件位置！" << endl;
        return;
    }
    //读取边类型
    string Typefpath = dirpath + "TypeArr.txt";
    ifstream Typefile(Typefpath, ios::in);
    if (!Typefile)
    {
        cerr << "读取边类型出错，请检查文件位置！" << endl;
        return;
    }
    //根据四个数据整合成邻接表
    for (int i = 0; i < size; i++)
    {
        string dline,sline,jline,tline;
        getline(Disfile, dline);
        getline(Savefile, sline);
        getline(Jamfile, jline);
        getline(Typefile, tline);
        stringstream distance(dline),save(sline),jam(jline),type(tline);
        for (int j = 0; j < size; j++)
        {
            string d, s, a, t;
            getline(distance, d, ' ');
            getline(save, s, ' ');
            getline(jam, a, ' ');
            getline(type, t, ' ');
            double disnum = stod(d), jamnum = stod(a);
            int savenum = stoi(s);
            EdgeType tp;
            if (t == "road") tp = EdgeType::ROAD;
            else if (t == "stair") tp = EdgeType::STAIR;
            else tp = EdgeType::CORRIDOR;
            if (disnum == -1 || i > j)  continue;//忽略无效边
            addEdge(Edge(i + 1, j + 1, disnum, tp, savenum, jamnum));
        }
    }
   
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
string Graph::getVertexName(int id) const {
    const Vertex* vertex = getVertex(id);
    return vertex ? vertex->name : "";
}

// 根据id获取顶点坐标
Vec2 Graph::getVertexPosition(int id) const {
    const Vertex* vertex = getVertex(id);
    return vertex ? vertex->position : Vec2(-1, -1);
}

// 根据顶点索引，获取从某个顶点出发的所有边，返回边的列表（vector）
vector<Edge> Graph::getEdgesFrom(int vertexId) const {
    auto it = adjacencyList.find(vertexId);
    return (it != adjacencyList.end()) ? it->second : vector<Edge>();
}

// 获取所有顶点的ID列表（vector）
vector<int> Graph::getAllVertexIds() const {
    vector<int> ids;
    for (const auto& vertex : vertices) {
        ids.push_back(vertex.id);
    }
    return ids;
}

// 根据顶点类型，获取对应的顶点ID列表
vector<int> Graph::getVerticesByType(VertexType type) const {
    vector<int> result;
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
string Graph::getVertexTypeString(VertexType type) {
    switch (type) {
    case VertexType::TEACHING_BUILDING: return "教学楼";
    case VertexType::DORMITORY: return "宿舍";
    case VertexType::EXIT: return "出口";
    case VertexType::FLOOR_NODE: return "楼层节点";
    default: return "未知类型";
    }
}

// 计算路径权重
double Graph::calculatePathWeight(const vector<int>& path, bool emergencyMode) const {
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
            cerr << "警告：路径中顶点 " << from << " 到 " << to << " 的边不存在！" << endl;
            return numeric_limits<double>::max();  // 返回极大值表示路径无效
        }
    }

    return totalWeight;
}

// 打印图的基本信息
void Graph::printGraph() const {
    cout << "========== 图结构信息 ==========" << endl;
    cout << "顶点数量: " << getVertexCount() << endl;
    cout << "边数量: " << getEdgeCount() << endl;

    // 按类型统计顶点
    cout << "\n顶点类型统计:" << endl;
    for (int i = 0; i < 4; i++) {
        VertexType type = static_cast<VertexType>(i);
        int count = countVerticesByType(type);
        if (count > 0) {
            cout << "  " << getVertexTypeString(type) << ": " << count << "个" << endl;
        }
    }

    cout << "\n顶点列表:" << endl;
    for (const auto& vertex : vertices) {
        cout << "  [ID:" << vertex.id << "] " << vertex.name
            << " (类型:" << getVertexTypeString(vertex.type)
            << ", 坐标:" << vertex.position.to_string()
            << ", 着火:" << (vertex.isBurning ? "是" : "否") << ")" << endl;
    }

    cout << "\n邻接表:" << endl;
    for (const auto& pair : adjacencyList) {
        if (!pair.second.empty()) {
            cout << "  顶点 [" << pair.first << "]:" << getVertexName(pair.first) << " -> ";
            for (const Edge& edge : pair.second) {
                cout << edge.to << "(" << edge.distance << "m";
                if (edge.riskLevel > 0) cout << ", 风险:" << edge.riskLevel;
                if (edge.congestion > 0) cout << ", 拥堵:" << edge.congestion;
                cout << ") ";
            }
            cout << endl;
        }
    }
    cout << "=================================" << endl;
}





// 3.核心算法

// 使用Dijkstra算法查找最短路径
vector<int> Graph::findShortestPath(int startId, int endId, bool emergencyMode) {
    // 1. 参数验证
    if (findVertexIndex(startId) == -1) {
        cerr << "起点ID " << startId << " 不存在！" << endl;
        return {};
    }
    if (findVertexIndex(endId) == -1) {
        cerr << "终点ID " << endId << " 不存在！" << endl;
        return {};
    }

    // 2. 起点和终点相同的情况
    if (startId == endId) {
        cout << "起点和终点相同！" << endl;
        return { startId };
    }

    // 3. 特殊检查：如果终点着火且是紧急模式，直接返回不可达
    if (emergencyMode) {
        Vertex* endVertex = getVertex(endId);
        if (endVertex && endVertex->isBurning) {
            cout << "终点着火，不可达！" << endl;
            return {};
        }
    }

    // 4. 预检查连通性
    if (!isConnected(startId, endId, emergencyMode)) {
        cout << "从 " << getVertex(startId)->name << " 到 "
            << getVertex(endId)->name << " 不连通！" << endl;
        return {};
    }

    // 5. 初始化数据结构
    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    unordered_map<int, bool> visited;
    priority_queue<DijkstraNode, vector<DijkstraNode>, greater<DijkstraNode>> pq;

    // 初始化所有节点
    for (const auto& vertex : vertices) {
        dist[vertex.id] = numeric_limits<double>::max();
        prev[vertex.id] = -1;
        visited[vertex.id] = false;
    }

    // 设置起点
    dist[startId] = 0.0;
    pq.push({ startId, 0.0 });

    // 6. Dijkstra主循环
    while (!pq.empty()) {
        DijkstraNode current = pq.top();
        pq.pop();

        int u = current.id;

        // 跳过已访问的节点
        if (visited[u]) continue;
        visited[u] = true;

        // 如果当前节点就是终点，提前结束
        if (u == endId) {
            break;
        }

        // 遍历所有邻接边
        if (adjacencyList.find(u) != adjacencyList.end()) {
            for (const Edge& edge : adjacencyList[u]) {
                int v = edge.to;

                // 如果邻接节点已访问，跳过
                if (visited[v]) continue;

                // 在紧急模式下，如果邻接节点着火，跳过该边
                if (emergencyMode) {
                    Vertex* neighbor = getVertex(v);
                    if (neighbor && neighbor->isBurning) {
                        continue;
                    }
                }

                // 计算边的实际权重
                double weight = edge.getWeight(emergencyMode);
                double newDist = dist[u] + weight;

                // 如果找到更短路径
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    pq.push({ v, newDist });
                }
            }
        }
    }

    // 7. 检查是否可达（理论上应该可达，因为预检查过，但保留作为防护）
    if (dist[endId] == numeric_limits<double>::max()) {
        cout << "从 " << startId << " 到 " << endId << " 不可达" << endl;
        return {};
    }

    // 8. 重建路径（从终点到起点）
    vector<int> path;
    for (int at = endId; at != -1; at = prev[at]) {
        path.push_back(at);

        // 安全防护：防止无限循环
        if (path.size() > vertices.size() + 1) {
            cerr << "路径重建出现循环！" << endl;
            return {};
        }
    }

    // 反转路径（起点->终点）
    reverse(path.begin(), path.end());

    // 9. 验证路径有效性
    if (path.empty() || path[0] != startId) {
        cerr << "路径重建错误：起点不匹配" << endl;
        return {};
    }

    return path;
}

// 多出口疏散方案
vector<pair<int, double>> Graph::findMultipleExits(int startId, bool emergencyMode) {
    vector<pair<int, double>> results;

    // 获取所有出口
    vector<int> exitIds = getVerticesByType(VertexType::EXIT);
    if (exitIds.empty()) {
        cout << "系统中没有出口！" << endl;
        return results;
    }

    // 计算到每个出口的路径
    for (int exitId : exitIds) {
        vector<int> path = findShortestPath(startId, exitId, emergencyMode);
        if (!path.empty()) {
            double totalWeight = calculatePathWeight(path, emergencyMode);
            results.push_back({ exitId, totalWeight });
        }
    }

    // 按权重排序
    sort(results.begin(), results.end(), [](const pair<int, double>& a, const pair<int, double>& b) {return a.second < b.second;});

    return results;
}

// 可达性检测
bool Graph::isAreaReachable(int areaId, bool emergencyMode) {
    // 1. 检查目标区域是否存在
    Vertex* target = getVertex(areaId);
    if (!target) {
        cout << "区域ID " << areaId << " 不存在！" << endl;
        return false;
    }

    // 2. 获取所有出口
    vector<int> exitIds = getVerticesByType(VertexType::EXIT);
    if (exitIds.empty()) {
        cout << "系统中没有出口！" << endl;
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
            cout << "区域 " << target->name << " 可到达出口 "
                << getVertex(exitId)->name << endl;
            return true;
        }
    }

    // 4. 所有出口都无法到达
    cout << "区域 " << target->name << " 无法到达任何出口" << endl;
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
    unordered_map<int, bool> visited;
    queue<int> q;

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