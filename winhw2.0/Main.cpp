#include "Graph.h"

int main() {
    Graph G;
    string map_id = "0";
    G.generateList(map_id);
    G.printGraph();
    int begin_id = 4;//设置起点
    vector<pair<int, double>> resault = G.findMultipleExits(begin_id);
    for (int i = 0; i < resault.size(); i++)
    {
        pair<int, double> solution = resault.back();
        cout << "从指定起点到id为" << solution.first << "的出口路径权值为" << solution.second << endl;
        cout << "详细路径为";
        vector<int>path = G.findShortestPath(begin_id, solution.first);
        for (int j = 0; j < path.size(); j++)
        {
            cout << path[j];
            if (j!=path.size()-1)
            {
                cout << "->";
            }
        }
        cout << endl;
        resault.pop_back();
    }
    return 0;
}