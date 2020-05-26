#include <iostream>
#include <vector>
#include <algorithm>
#include "math.h"


struct edge
{
    char first;
    char second;
    double weight;
};

struct ways
{
    char start;
    char finish;
}way;

struct maps
{
    std::string curr_path;
    double length;
};



double func_h(char one, char two)
{
    std::cout << "\tcalculated prefix function for vertices:" << one << " and " << two << " is " << abs((int)(one)-(int)(two)) << "\n";
    return abs((int)(one)-(int)(two));
}
class Graph
{
    std::vector <edge> edges;
    std::vector <char> path;
    std::vector <char> close;
    std::vector <maps> map_path;
public:

    Graph(){}

    void input_data()
    {
        std::cin >> way.start >> way.finish;
        path.push_back(way.start);
        char tmp;
        edge rib;
        std::cin >> tmp;
        while (tmp != '!')//добавление рёбер в контейнер
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

    void work()
    {
        //std::cout << "\nalgorithm start\n";
        //sort(edges.begin(), edges.end(), compare);
        std::string buf = "";
        buf += way.start;
        //std::cout << "adding possible paths from the starting vertex:\n";
        for (size_t i = 0; i < edges.size(); i++)
        {
            //std::cout << "\tcurrent rib:" << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
            if (edges[i].first == way.start)//добавление возможных путей из начальной вершины
            {
                //std::cout << "\tadding new rib to map path:" << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                buf += edges[i].second;
                map_path.push_back({ buf, edges[i].weight });
                buf.resize(1);
            }
        }
        path.push_back(way.start);
        while (!map_path.empty())
        {
            //std::cout << "\nstart search for a new path\n";
            size_t index = min_search();
            if (map_path[index].curr_path.back() == way.finish)//завершение работы, если конечная вершина достигнта
            {
                std::cout << "final vertex is reached!\nresult of programm:\n";
                print_reuslt(index);
                return;
            }
            //std::cout << "search for paths from the current vertex: " << map_path[index].curr_path.back() << "\n";
            for (size_t i = 0; i < edges.size(); i++)
            {
                //std::cout << "\tcurrent rib:" << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                if (edges[i].first == map_path[index].curr_path.back())
                {
                    //std::cout << "\tadding new rib to map path:" << edges[i].first << " -> " << edges[i].second << " with weight: " << edges[i].weight << "\n";
                    map_path.push_back({ map_path[index].curr_path + edges[i].second, edges[i].weight + map_path[index].length });//добавляем минимальную вершину
                }
            }
            path.push_back(map_path[index].curr_path.back());
            //std::cout << "remove current path: " << map_path[index].curr_path << "\n";
            map_path.erase(map_path.begin() + index);//удаляем предыдущий путь
        }
        //std::cout << "\nno path founded\n";
    }

    bool is_view(char value)
    {
        //std::cout << "\tcheck: if vertex is viewed " << value << "\n";
        for (size_t i = 0; i < path.size(); i++)
        {
            if(path[i] == value)//проверка на то, просмотрена ли вершина
            {
                //std::cout << "\tthis vertex is viewed!\n";
                return true;
            }
        }
        //std::cout << "\tthis vertex is not viewed!\n";
        return false;
    }

    size_t min_search()
    {
        //std::cout << "\nsearch for a new min index:\n";
        double min = 0;
        for (size_t i = 0; i < edges.size(); i++)
            min += edges[i].weight;
        min += 31;//инициализирование начального значения для сравнения
        size_t tmp;
        for (size_t i = 0; i < map_path.size(); i++)
        {
            int tmp = map_path[i].length + func_h(way.finish, map_path[i].curr_path.back());
            //std::cout << "comparing the vertex index with the current minimum: current is "
                      //<< tmp;
            //std::cout << " and current min is " << min << "\n";
            if (tmp < min)//сравнение индекса вершины с текущим минимальным
            {
                if (is_view(map_path[i].curr_path.back()))
                {
                    map_path.erase(map_path.begin() + i);//удаление уже просмтренных вершин
                }
                else
                {
                    min = map_path[i].length + func_h(way.finish, map_path[i].curr_path.back());//запоминание индекса минимальной вершины
                    //std::cout << "a new current min is " << min << "\n";
                    tmp = i;
                }
            }
        }
        //std::cout << "a new min index founded: " << tmp << '\n';
        return tmp;
    }
    void print_reuslt(size_t index)
    {
        std::cout << map_path[index].curr_path;//вывод минимального пути
    }
};
int main()
{
    Graph tmp;
    tmp.input_data();
    tmp.work();
}
