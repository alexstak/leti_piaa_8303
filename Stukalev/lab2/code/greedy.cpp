#include <iostream>
#include <vector>
#include <algorithm>



struct edge
{
    char first;
    char second;
    double weight;
};
bool compare(edge one, edge two)
{
    return one.weight < two.weight;
}
class Graph
{
    char start;
    char finish;
    std::vector <edge> edges;
    std::vector <char> path;
public:
    Graph(){}

    void input_data()
    {
        std::cin >> start >> finish;
        path.push_back(start);
        char tmp;
        edge rib;
        std::cin >> tmp;
        while (tmp != '!')//запись рёбер в контейнер
        {
            rib.first = tmp;
            if (!(std::cin >> rib.second))
                break;
            if (!(std::cin >> rib.weight))
                break;
            edges.push_back(rib);
            std::cin >> tmp;
        }
    }

    void greedy()
    {
        int count = 0;
        bool log = false;
        /*std::cout << "Ribs before sorting:\n";
        for(int i = 0; i < edges.size(); i ++)
        {
            std::cout << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
        }*/
        sort(edges.begin(), edges.end(), compare);//сортировка рёбер по возратанию
        /*std::cout << "Ribs after sorting:\n";
        for(int i = 0; i < edges.size(); i ++)
        {
            std::cout << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
        }
        std::cout << "\nalgorithm start\n";*/
        while (path.back() != finish)
        {
            for (size_t i = 0; i < edges.size(); i++)
            {
                //std::cout << "current rib:" << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                for (size_t i = 0; i < path.size(); i++)
                {
                    if (path[i] == start)
                        count++;
                    if (count == 2)
                    {
                        path.clear();//удаление петель
                        path.push_back(start);
                        //std::cout << "insert vertex to the path:" << start << "\n";
                    }
                }
                count = 0;
                if (edges[i].first == path.back())
                {
                    path.push_back(edges[i].second);
                    //std::cout << "insert vertex to the path: " << edges[i].second << "\n";
                    //std::cout << "removing an unnecessary edge: " << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                    edges.erase(edges.begin() + i);//удаление ребра из контейнера, потому что далее его использовать нет необходимости
                    log = true;
                    break;
                }
                else
                    log = false;

            }
            if (log == false)
            {
                //std::cout << "return to the privious vertex from " << path.back();
                for (size_t i = 0; i < edges.size(); i++)
                {
                    if (edges[i].second == path.back())
                    {
                        //std::cout << "delete rib without a true path: " << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                        edges.erase(edges.begin() + i);//удаление вершины, из которой нет пути
                    }
                }
                path.pop_back();
                //std:: cout << " to " << path.back() << "\n";
            }
        }
        //std::cout << "path founded!\n";
    }
    void print_reuslt()
    {
        for (size_t i = 0; i < path.size(); i++)
            std::cout << path[i];
    }
};
int main()
{
    Graph tmp;
    tmp.input_data();
    tmp.greedy();
    tmp.print_reuslt();
}

