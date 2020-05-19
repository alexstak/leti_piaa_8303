#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iostream>

struct BohrVrtx
{
    int next_v[5] = {-1, -1, -1 , -1, -1}; // массив, необходимый для переходов по рёбрам в боре.
    int auto_movement[5] = {-1, -1, -1, -1, -1}; //массив, необходимый для переходов в автомате
    int num = 0; //номер шаблона терминальной вершины
    int par = 0; //индекс родительской вершины
    int indsymbtopar = - 1; //индекс символа родительской вершины
    int sufflink = -1; //индекс перехода по суффиксальной ссылке
    int goodsufflink = -1; //индекс перехода по хорошей суффиксальной ссылке
    bool isEnd = false; //- логическая переменная, отвечающая за то, является ли вершина терминальной
};

struct Result
{
    int position; //позиция подстроки в тексте
    int number; //номер подстроки
};

class Bohr
{
    std::vector <BohrVrtx> bohrTree; //бор
    std::vector <Result> result; //контейнер для хранения результата
    std::vector <int> forcutstr; //контейнер для удаления подстрок из текста
    std::string text; //строка с текстом
    int stringNum; //количество подстрок
    char alphabet[5] = {'A', 'C', 'G', 'T', 'N'}; //алфавит
    std::vector <std::string> substrings; //контейнер для хранения подстрок
public:
    Bohr()
    {
        bohrTree.push_back(make_bohr_vert(0,0,-1,false));
    }

    BohrVrtx make_bohr_vert( int number, int index, int symbol, bool flag)
    {
        BohrVrtx tmp;
        tmp.num = number;
        tmp.par = index;
        tmp.indsymbtopar = symbol;
        tmp.isEnd = flag;
        return  tmp;
    }

    void input_data()
    {
        std::cin >> text;
        std::cin >> stringNum;
        for (int i = 0; i < stringNum; i++)//считывание подстрок
        {
            std::string tmp;
            std::cin >> tmp;
            substrings.push_back(tmp);
        }
        std::vector <int> tmp(text.size());
        forcutstr = tmp; //инициализация массива для удаления подстрок количеством нулей равным длине текста
    }

    void init_bohr()
    {
        for (int i = 0; i < stringNum; i++)
        {
            add_string_to_bohr(substrings[i], i+1);//добавление подстрок в бор
        }
    }

    void add_string_to_bohr(std::string str, int number)
    {
        int index = 0;
        int symbol = -1;
        std::cout << "\n-----------------------------------------------------\n";
        std::cout << "Start add string to bohr " << str << ", number of string: " << number << '\n';
        for(int i = 0; i < str.length(); i++)
        {
            if(str[i] == 'A')
                symbol = 0;
            else if(str[i] == 'C')
                symbol = 1;
            else if(str[i] == 'G')
                symbol = 2;
            else if(str[i] == 'T')
                symbol = 3;
            else if(str[i] == 'N')
                symbol = 4;
            std::cout << "\nAdd vertex " << symbol << "(" << str[i] << ") on position " << i << '\n';
            if(bohrTree[index].next_v[symbol] == -1)//если перехода нет
            {
                if(i == str.length()-1)//если конец строки, то вершина терминальная
                    bohrTree.push_back(make_bohr_vert(number, index, symbol, true));
                else
                    bohrTree.push_back(make_bohr_vert(number, index, symbol, false));
                bohrTree[index].next_v[symbol] = bohrTree.size() - 1;
                std::cout << "Added new vertex in tree: " <<  bohrTree[index].next_v[symbol] << '\n'
                          << "Symbol path to vertex: " << alphabet[symbol] << '(' << symbol << ")\n";
            }
            index = bohrTree[index].next_v[symbol];//если переход обнаружен, осуществляется переход
            std::cout << "Transition to vertex by symbol: " << alphabet[symbol] << '(' << symbol << ")\n";
        }
        std::cout << "\nNew terminal vertex added: " << alphabet[symbol] << '(' << symbol << ")\n"
                  << "Number of this vertex: " << bohrTree[index].num << '\n';
        std::cout << "Add string completed\n";
    }

    int get_auto_move(int v_num, int symb)//функция перехода из вершины по символу
    {
        std::cout << "Try to find transition form " << v_num << " by symbol " << alphabet[symb] << '(' << symb << ")\n";
        if(bohrTree[v_num].auto_movement[symb] == -1) // если путь в массиве переходов найден не был
        {
            if(bohrTree[v_num].next_v[symb] == -1) // если перехода по текущему символу в боре не существует
            {
                if(v_num == 0)
                {
                    std::cout << "Link leads to root\n";
                    bohrTree[v_num].auto_movement[symb] = 0;
                }
                else
                {
                    bohrTree[v_num].auto_movement[symb] = get_auto_move(get_suff_link(v_num), symb);
                }
                return bohrTree[v_num].auto_movement[symb];
            }
            bohrTree[v_num].auto_movement[symb] = bohrTree[v_num].next_v[symb];
            std::cout << "Go to vertex: " << bohrTree[v_num].auto_movement[symb] << " ,using symbol: " << alphabet[symb] << '(' << symb << ")\n"
                      << "Transition added\n";
        }
        std::cout << "Transition to vertex: " << bohrTree[v_num].auto_movement[symb] << " ,using symbol: " << alphabet[symb] << '(' << symb << ")\n\n";
        return bohrTree[v_num].auto_movement[symb];
    }

    int get_suff_link(int v_num)//получение суффиксальной ссылки для вершины
    {
        std::cout << "Getting suffix link from vertex: " << v_num <<'\n';
        if(bohrTree[v_num].sufflink == -1)//если суффиксальной ссылка отсутствует
        {
            if(v_num != 0 && bohrTree[v_num].par != 0)// если вершина не корень и не сын корня
            {
                bohrTree[v_num].sufflink = get_auto_move(get_suff_link(bohrTree[v_num].par), bohrTree[v_num].indsymbtopar);//рекурсивный поиск
                return bohrTree[v_num].sufflink;
            }
            bohrTree[v_num].sufflink = 0;
            std::cout << "Suffix link leads to root!\n";

        }
        std::cout << "Suffix link from vertex " << v_num << " is " << bohrTree[v_num].sufflink << "\n\n";
        return bohrTree[v_num].sufflink;
    }

    int get_good_suff_link(int v_num)// получение хорошей суффиксальной ссылки для вершины
        {
            std::cout << "Getting good suffix link from vertex: " << v_num <<'\n';
            if(bohrTree[v_num].goodsufflink == -1)//если хорошая суффиксальная ссылка отсутсвует
            {
                if(get_suff_link(v_num) == 0)//если вершина корень
                {
                    bohrTree[v_num].goodsufflink = 0;
                    std::cout << "Suffix link leads to root!\n";
                }
                else
                {
                    int tmp = get_suff_link(v_num);
                    if(bohrTree[tmp].isEnd)//если ближайшая терминальная вершина найдена
                    {
                        std::cout << "Nearest terminal vertex found: ";
                        bohrTree[v_num].goodsufflink = tmp;
                        std::cout << "good suffix link from vertex " << v_num << " is " << bohrTree[v_num].goodsufflink << "\n\n";
                        return bohrTree[v_num].goodsufflink;
                    }
                    bohrTree[v_num].goodsufflink = get_good_suff_link(tmp);//рекурсинвый поиск
                }
            }
            std::cout << "Good suffix link from vertex " << v_num << " is " << bohrTree[v_num].goodsufflink << "\n\n";
            return bohrTree[v_num].goodsufflink;
        }


    void check_for_substring(int v_num, int letter_position)
    {
        for(int i = v_num; i != 0; i = get_good_suff_link(i))
        {
            if(bohrTree[i].isEnd)//если найдена терминальная вершина
            {
                int shift = 0;
                int tmp = i;
                while(bohrTree[tmp].par != 0)//вычисляем начало вхождения подстроки
                {
                    tmp = bohrTree[tmp].par;
                    shift++;
                }
                result.push_back({letter_position-shift,bohrTree[i].num});
                std::cout << "Substring founded: " << letter_position - shift << " " << bohrTree[i].num << '\n';
            }
        }
    }

    void print_result()
    {
        for( int i = 0; i < result.size(); i++)
        {
            std::cout << result[i].position << " " << result[i].number << '\n';
        }
        std::cout << "\nMachine:\n";
        int maxarcs = 0;
        for (int i = 0; i < bohrTree.size(); i++)
        {
            int currmaxarcs = 0;
            std::cout << "Way from vertex " << i << ":\n";
            for (int j = 0; j < 5; j++)
            {
                if(bohrTree[i].auto_movement[j] != -1)
                {
                    currmaxarcs++;
                    std::cout << "\tVertex " << bohrTree[i].auto_movement[j] << " ->  " << alphabet[j] << "(" << j << ")\n";
                }
            }
            if(currmaxarcs > maxarcs)//запоминаем наибольшее число исходящих дуг
                maxarcs = currmaxarcs;
        }
        std::cout << "\nMaximum number of outgoing arcs from one vertex of bor is:  " << maxarcs << '\n';
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
        std::cout << "Edited text: " << editedtext << '\n';

    }

    void Aho()
    {
        std::cout << "\n-----------------------------------------------------\n";
        std::cout << "Search for substrings started\n";
        int symb = -1;
        int v_num = 0;
        for(int i = 0; i < this->text.length(); i++)
        {
            if(text[i] == 'A')
            {
                symb = 0;
            }
            else if(text[i] == 'C')
            {
                symb = 1;
            }
            else if(text[i] == 'G')
            {
                symb = 2;
            }
            else if(text[i] == 'T')
            {
                symb = 3;
            }
            else if(text[i] == 'N')
            {
                symb = 4;
            }
            else
            {
                std::cout << "Wrong letter!\n";
                break;
            }
            std::cout << "Current symbol: " << text[i] << '(' << i << ")\n";
            v_num = get_auto_move(v_num, symb);
            check_for_substring(v_num, i + 1);
        }
        std::cout << "Search for substrings ended\n";
        std::cout << "-----------------------------------------------------\n";
    }
};

int main()
{
    Bohr* tmp = new Bohr();
    tmp->input_data();
    tmp->init_bohr();
    tmp->Aho();
    tmp->print_result();
}


/*
CCCA
1
CC

AACCGGTTNN
4
ACC
GGT
TT
CC
 */
