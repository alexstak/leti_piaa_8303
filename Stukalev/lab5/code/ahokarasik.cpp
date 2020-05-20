#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


struct BohrVrtx
{
    std::map<char, int> next_v; // массив, необходимый для переходов по рёбрам в боре.
    std::map<char, int> auto_movement; //массив, необходимый для переходов в автомате
    int number; // номер обработанной подстроки
    int parent; //индекс родителя
    int suff_link = -1; //индекс перехода по суффиксальной ссылке
    int goodsufflink = -1; //индекс перехода по хорошей суффиксальной ссылке
    int deep; // длина подстроки
    bool isEnd; //- логическая переменная, отвечающая за то, является ли вершина терминальной
    char parentsymb; // символ родителя
};

struct Result
{
    int position; //позиция подстроки в тексте
    int number; //номер подстроки
};

bool compare(Result a1, Result a2)
{
    if(a1.position == a2.position)
        return a1.number < a2.number;
    return a1.position < a2.position;
}

class Bohr
{
    std::vector <BohrVrtx> bohrTree; // бор
    std::vector <Result> result;
    std::string text;
    std::vector <std::string> substrings; //  контейнер с подстроками
    std::vector <int> forcutstr; //контейнер для удаления подстрок из текста
    int stringNum; // общее количество подстрок
    int currcount; // текущее количество обработанных подстрок
public:
    std::vector <int> jokerpositions;//контейнер с позициями подстрок в исходной строке
    Bohr():stringNum(0), currcount(0)
    {
        bohrTree.push_back(make_bohr_vert(-1,-1));
    }

    BohrVrtx make_bohr_vert( int parent, char symbol, bool flag)//создание вершины
    {
        BohrVrtx tmp;//инициализация параметров
        tmp.parent = parent;
        tmp.parentsymb = symbol;
        tmp.isEnd = flag;
        return  tmp;
    }

    BohrVrtx make_bohr_vert( int parent, int link)
    {
        BohrVrtx tmp;
        tmp.parent = parent;
        tmp.suff_link = link;
        tmp.isEnd = false;
        return  tmp;
    }

    void input_data()//ввод данных необходимых для работы алгоритма
    {
        std::cin >> text;
        std::cin >> stringNum;
        std::string str;
        for(int i = 0; i < stringNum; i++)
        {
            std::cin >> str;
            substrings.push_back(str);
        }
        
    }

    void init_bohr()
    {
        for (int i = 0; i < stringNum; i++)
        {
            add_string_to_bohr(substrings[i]); //добавление строк поочередно в бор
        }
    }

    void add_string_to_bohr( std::string str)//добавление подстроки в бор
    {
        //std::cout << "\n-----------------------------------------------------\n";
        //std::cout << "Start add string to bohr " << str << ", number of string: " << currcount << '\n';
        int index = 0;
        char currSymb = ' ';
        for (int i = 0; i < str.size(); i++)
        {
            currSymb= str[i];
            //std::cout << "\nAdd vertex " << str[i] << " on position " << i << '\n';
            if (bohrTree[index].next_v.find(str[i]) == bohrTree[index].next_v.end())//если переход по символу отсутсвует, происходит добавление новой вершины
            {
                bohrTree.push_back(make_bohr_vert(index,str[i],false));
                bohrTree[index].next_v[str[i]] = bohrTree.size() - 1;
                //std::cout << "Added new vertex in tree: " <<  bohrTree[index].next_v[str[i]] << '\n';
            }
            index = bohrTree[index].next_v[str[i]];
            //std::cout << "Transition to vertex by symbol: " << str[i] << "\n";
        }
        if(str.length() > 0)
        {
            bohrTree[index].deep = str.size();//запоминаем длину текущей строки

            bohrTree[index].number =++currcount;// запоминаие номера текущей строки
            bohrTree[index].isEnd = true;//помечаем терминальную вершину
            //std::cout << "\nNew terminal vertex added: " << currSymb << "\n";
            //std::cout << "-----------------------------------------------------\n";
        }

    }

    int get_suff_link(int index) //получение суффиксальной ссылки для вершины
    {
        //std::cout << "Getting suffix link from vertex: " << index <<'\n';
        if (bohrTree[index].suff_link == -1) //если суффиксальная ссылка отсутствует
        {
            if (index == 0 || bohrTree[index].parent == 0)// если вершина корень или родитель вершины корень
            {
                //std::cout << "Suffix link leads to root!\n";
                bohrTree[index].suff_link = 0;
                //std::cout << "Suffix link from vertex " << index << " is 0\n\n";
                return bohrTree[index].suff_link;

            }
            bohrTree[index].suff_link = get_auto_movement(get_suff_link(bohrTree[index].parent), bohrTree[index].parentsymb);
        }
        //std::cout << "Suffix link from vertex " << index << " is " << bohrTree[index].suff_link << "\n\n";
        return bohrTree[index].suff_link;
    }

    int get_good_suff_link(int v_num)// получение хорошей суффиксальной ссылки для вершины
        {
            //std::cout << "Getting good suffix link from vertex: " << v_num <<'\n';
            if(bohrTree[v_num].goodsufflink == -1)//если хорошая суффиксальная ссылка отсутсвует
            {
                if(get_suff_link(v_num) == 0)//если вершина корень
                {
                    bohrTree[v_num].goodsufflink = 0;
                    //std::cout << "Suffix link leads to root!\n";
                }
                else
                {
                    int tmp = get_suff_link(v_num);
                    if(bohrTree[tmp].isEnd)//если ближайшая терминальная вершина найдена
                    {
                        //std::cout << "Nearest terminal vertex found: ";
                        bohrTree[v_num].goodsufflink = tmp;
                        //std::cout << "good suffix link from vertex " << v_num << " is " << bohrTree[v_num].goodsufflink << "\n\n";
                        return bohrTree[v_num].goodsufflink;
                    }
                    bohrTree[v_num].goodsufflink = get_good_suff_link(tmp);//рекурсинвый поиск
                }
            }
            //std::cout << "Good suffix link from vertex " << v_num << " is " << bohrTree[v_num].goodsufflink << "\n\n";
            return bohrTree[v_num].goodsufflink;
        }

    int get_auto_movement(int index, char symb) //функция перехода из вершины по символу
    {
         //std::cout << "Try to find transition from " << index << " by symbol " << symb << "\n";
        if(bohrTree[index].auto_movement.find(symb) == bohrTree[index].auto_movement.end()) // если путь в массиве переходов найден не был
        {
            if (bohrTree[index].next_v.find(symb) != bohrTree[index].next_v.end())// если переход по текущему символу существует в боре
            {
                bohrTree[index].auto_movement[symb] = bohrTree[index].next_v[symb];// добавляем этот переход
                //std::cout << "Go to vertex: " << bohrTree[index].auto_movement[symb] << " ,using symbol: " <<  symb << "\n"
                          //<< "Transition added\n";
            }
            else
            {
                if(index == 0)
                {
                    //std::cout << "Suffix link leads to root!\n";
                    bohrTree[index].auto_movement[symb] = 0;
                    //std::cout << "Go to vertex: " << bohrTree[index].auto_movement[symb] << " ,using symbol: " <<  symb << "\n"
                             // << "Transition added\n";
                    return  bohrTree[index].auto_movement[symb];
                }
                bohrTree[index].auto_movement[symb] =  get_auto_movement(get_suff_link(index), symb);
                //std::cout << "Go to vertex: " << bohrTree[index].auto_movement[symb] << " ,using symbol: " <<  symb << "\n"
                          //<< "Transition added\n";
            }
        }
        //std::cout << "Transition to vertex: " << bohrTree[index].auto_movement[symb] << " ,using symbol: " << symb << "\n\n";
        return bohrTree[index].auto_movement[symb];
    }

    void Aho()
    {
        //std::cout << "\n-----------------------------------------------------\n";
        //std::cout << "Search for substrings started\n";
        int curr = 0;
        for (int i = 0; i < text.size(); i++)
        {
            //std::cout << "Current symbol: " << text[i] << '(' << i << ")\n";
            curr = get_auto_movement(curr, text[i]);
            check_for_substring(curr,i);

        }
        //std::cout << "End of search\n";
        //std::cout << "-----------------------------------------------------\n";
    }

    void check_for_substring(int v_num, int letter_position)
    {
        for (int tmp = v_num; tmp != 0; tmp = get_good_suff_link(tmp))
        {
            if (bohrTree[tmp].isEnd)//если вершина терминальная
            {
                Result buff;
                buff.position = letter_position + 2 - bohrTree[tmp].deep;
                buff.number = bohrTree[tmp].number;
                
                result.push_back(buff);
                //std::cout << "The entrance founded, index = " <<
                          //letter_position + 2 - bohrTree[tmp].deep << " and pattern is " << substrings[bohrTree[tmp].number - 1] << "\n";
            }
        }
    }

    void print_result()
    {
        std::sort(result.begin(),result.end(), compare);
        //std::cout << "Result of programm:\n";
        //std::vector <int> stringforedit(text.size());
        for( int i = 0; i < result.size(); i++)
        {
            std::cout << result[i].position << " " << result[i].number << '\n';
        }
        /*std::cout << "\nMachine:\n";
        int maxcountoftrans = 0;
        int currcountoftrans;
        for (int i = 0; i < bohrTree.size(); i++)
        {
            auto iter = bohrTree[i].auto_movement.begin();
            std::cout << "Ways from vertex " << i << ":\n";
            currcountoftrans = bohrTree[i].next_v.size();
            for (int j = 0; j < bohrTree[i].auto_movement.size(); j++)//вывод автомата
            {
                std::cout << "\tsymbol: " << iter->first << ", index: " << iter->second << "\n";
                iter++;
            }
            if( currcountoftrans > maxcountoftrans)
                maxcountoftrans = currcountoftrans;

        }
        std::cout << "\nMaximum number of outgoing arcs from one vertex of bor is:  " << maxcountoftrans << '\n';
        std::cout << "\nRemoving substrings from text:\n";
        std::string editedtext = "";

        for(int i = 0; i < result.size(); i++)
        {
            for(int j = 0; j < substrings[result[i].number - 1].length(); j++)//отмечатеся вхождение подстроки в текст
            {
                forcutstr[result[i].position +j -1]++;
            }
        }
        for(int i = 0; i < forcutstr.size(); i++)
        {
            if(forcutstr[i] == 0)
                editedtext += text[i];
        }
        std::cout << "Source text: " << text << '\n';
        std::cout << "Edited text: " << editedtext << '\n';*/
    }



};


int main()
{

    Bohr* tmp = new Bohr();
    tmp->input_data();
    tmp->init_bohr();
    tmp->Aho();
    tmp->print_result();

    return 0;
}