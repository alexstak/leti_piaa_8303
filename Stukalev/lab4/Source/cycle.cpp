#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

class Cycle
{
    std::vector <int> ind_lenghts;
    std::string first_string;
    std::string second_string;
public:
    Cycle(){}
    void prefix_func()
    {
        //std::cout << "Value function prefix: 0 ";
        for (size_t i=1; i<second_string.length(); ++i)
            {
               // ищем, какой префикс можно расширить
                size_t j = ind_lenghts[i-1]; //длина предыдущего префикса, может быть нулевой
                while ((j > 0) && (second_string[i] != second_string[j])) //если нельзя расширить,
                    j = ind_lenghts[j-1];   //уменьшаем значение префикса

                if (second_string[i] == second_string[j])
                    ++j;  //расширяем найденный префик
                ind_lenghts[i] = j;
                //std::cout << j << ' ';
             }
        //std::cout << '\n';
    }

    void cycle()
    {
        std::cin >> first_string;
        std::cin >> second_string;

        if(first_string.length() != second_string.length())//если длины строк не равны - выход
        {
            //std::cout << "Non-equel lengths: " << first_string.length() << "!=" << second_string.length() << '\n';
            std::cout << -1;
            return;
        }
        std::vector <int> tmp(second_string.length());
        ind_lenghts = tmp;
        this->prefix_func();// вычисление префикс функции для second_string
        int laps = 0;
        for (int ind_f = 0, ind_s = 0;;)
        {
            if(first_string[ind_f] == second_string[ind_s])
            {
                //std::cout << "Equel elements:    " << first_string[ind_f] << "==" << second_string[ind_s] <<" index: "<< ind_f << " " << ind_s <<'\n';
                ind_f++;
                ind_s++;
            }
            if(ind_f == first_string.length())
            {
                ind_f = 0;
                laps++;
            }
            if(ind_s == first_string.length())
            {
                //std::cout << "Cycle founded: ";
                std::cout << ind_f;
                return;
            }
            else if(first_string[ind_f] != second_string[ind_s] && ind_s < first_string.length())
            {
                //std::cout << "Nonequel elements: " << first_string[ind_f] << "!=" << second_string[ind_s] <<" index: "<< ind_f << " " << ind_s <<'\n';
                if(ind_s == 0)
                    ind_f++;
                else
                    ind_s = ind_lenghts[ind_s - 1];
            }
            if(laps > 1)
                break;
        }
        std::cout << -1;
    }

    ~Cycle(){}
};


int main()
{
    Cycle tmp;
    tmp.cycle();
}
