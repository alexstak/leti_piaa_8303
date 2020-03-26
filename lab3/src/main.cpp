#include <iostream>
#include <vector>
#include <algorithm>

#define INT_MAX    2147483647


struct edge
{
    char first;
    char second;
    int weight;//первоначальный вес
    int forward;//вперёд
    int back;//назад
    bool not_oriented;
};


bool compare(edge first, edge second)
{
    if(first.first == second.first)
        return (int)first.second < (int)second.second;
    return (int)first.first < (int)second.first;
}

struct ways
{
    char start;
    char finish;
    int res;
}way;

class Graph
{
private:
    std::vector <edge> graph;
    std::vector <char> viewedpoints;
    std::vector <char> curr_path;

public:
    Graph()
    {
        int N;
        way.res = 0;
        std::cin >> N >> way.start >> way.finish;
        for(int i = 0; i < N; i++)//заполнения контейнера с рёбрами
        {
            edge element;
            std::cin >> element.first >> element.second >> element.weight;
            element.forward = element.weight;
            element.back = 0;
            element.not_oriented = false;
            graph.push_back(element);
        }
        for(int i = 0; i < graph.size(); i++)
        {
            for(int j = 0; j < graph.size(); j++)
            {
                if(graph[i].first == graph[j].second )//проверка на двунаправленные рёбра
                {
                    if (graph[i].second == graph[j].first)
                    {
                        graph[i].back += graph[j].forward;
                        graph[i].not_oriented = true;
                        graph.erase(graph.begin()+j);
                    }
                }
            }
        }
    }

    bool isViewing(char value)
    {
        for(size_t i = 0; i < viewedpoints.size(); i++)
            if(viewedpoints[i] == value)
                return true;
        return false;
    }

    ~Graph()
        {
            graph.clear();
            viewedpoints.clear();
            curr_path.clear();
        }

    bool Search(char value, int& min)
    {
        if(value == way.finish)//сток достигнут
        {
            curr_path.push_back(value);
            return true;
        }
        viewedpoints.push_back(value);
        for(size_t i = 0; i < graph.size(); i++)
        {
            if((isViewing(graph[i].second) || graph[i].forward <= 0) && (isViewing(graph[i].first) || graph[i].back <= 0))//пропуск если вершина уже просмотрена или ребро имеет нулевой вес
                continue;
            if(value == graph[i].first)
            {

                //std::cout << "add to curr_path: " << graph[i].first << "\n";
                curr_path.push_back(graph[i].first);
                if(Search(graph[i].second, min))//рекурсивное продолжение поиска
                {
                    min = (graph[i].forward < min) ? graph[i].forward : min;//минимальный вес
                    //std::cout << "curr min: " << min << "\n";
                   return true;
                }
                //std::cout << "rem from curr_path: " << curr_path.back() << "\n";
                curr_path.pop_back();
            }
            if(value == graph[i].second)
            {
                //std::cout << "add to curr_path: " << graph[i].second << "\n";
                curr_path.push_back(graph[i].second);
                if(Search(graph[i].first, min))//рекурсивное продолжение поиска
                {
                    min = (graph[i].back < min) ? graph[i].back : min;//минимальный вес
                    //std::cout << "curr min: " << min << "\n";
                   return true;
                }
                //std::cout << "rem from curr_path: " << curr_path.back() << "\n";
                curr_path.pop_back();
            }
        }
        return false;
    }


    void FordFalk()
    {
        int min = INT_MAX;
        while(Search(way.start, min))
        {
            for(int i = 1; i < curr_path.size(); i++)
            {
                for(int j = 0; j < graph.size(); j++)
                {
                    if(graph[j].first == curr_path[i-1] && graph[j].second == curr_path[i])//пересчёт потока через ребро
                    {
                        //std::cout << "Edge change weight: " << graph[j].first << " " << graph[j].second;
                        //std::cout << "\nforward: " << graph[j].forward << " to " << graph[j].forward - min;
                        //std::cout << "\nback: " << graph[j].back << " to " << graph[j].back + min << "\n";
                        graph[j].forward -= min;
                        graph[j].back += min;
                    }
                    if(graph[j].second == curr_path[i-1] && graph[j].first == curr_path[i])//пересчёт потока через ребро
                    {
                        //std::cout << "Edge change weight: " << graph[j].first << " " << graph[j].second;
                        //std::cout << "\nforward: " << graph[j].forward << " to " << graph[j].forward + min;
                        //std::cout << "\nback: " << graph[j].back << " to " << graph[j].back - min << "\n";
                        graph[j].forward += min;
                        graph[j].back -= min;
                    }
                }
            }
            way.res += min;
            viewedpoints.clear();
            curr_path.clear();
            min = INT_MAX;
        }
    }
    void print_result()
    {
        sort(graph.begin(), graph.end(), compare);//сортировка в лексикографическом порядке по первой вершине, потом по второй
        std::cout << way.res << std::endl;
        for(int i = 0; i < graph.size(); i++)
        {
            int value = graph[i].weight - graph[i].forward;
            if(value <  0 - graph[i].back)
            {
                value =  0 - graph[i].back;
            }

            if(graph[i].not_oriented == false)
            {
                std::cout << graph[i].first << " " << graph[i].second << " " << value << std::endl;

            }
            else
            {
                if(value < 0)
                    value = 0;
                std::cout << graph[i].first << " " << graph[i].second << " " << value << std::endl;
                std::swap(graph[i].first, graph[i].second);
                std::swap(graph[i].back, graph[i].forward);
                graph.at(i).not_oriented = false;
                sort(graph.begin(), graph.end(), compare);
                i--;
            }
        }
    }
};

int main()
{
    Graph* tmp = new Graph();
    tmp->FordFalk();
    tmp->print_result();
    delete tmp;
    return 0;
}
