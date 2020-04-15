#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

class SubStr
{
    std::vector <int> ind_lenghts;
    std::string input_string;
public:
    SubStr(){}
    void prefix_func()
    {
        //std::cout << "Value function prefix: 0 ";
        for (size_t i=1; i<input_string.length(); ++i)
            {
                //ищем, какой префикс можно расширить
                size_t j = ind_lenghts[i-1]; //длина предыдущего префикса, может быть нулевой
                while ((j > 0) && (input_string[i] != input_string[j])) //если нельзя расширить
                    j = ind_lenghts[j-1];   //уменьшаем значение префикса

                if (input_string[i] == input_string[j])
                    ++j;  // расширяем найденный префикс
                ind_lenghts[i] = j;
                //std::cout << j << ' ';

             }
        //std::cout << '\n';

    }
    void KMP()
    {
        std::cin >> input_string;
        std::vector <int> tmp(input_string.length());//инициализируем нулями вектор длин префиксов
        ind_lenghts = tmp;
        char c;
        int j = 0;
        int i = 0;
        bool no_one_digit = false;
        prefix_func();//считается префикс функция для образца
        std::cin >> c;
        while(!std::cin.fail() && c != '!')//пока возможно считать символ
        {
            if(input_string[j] == c)//соответсвие найдено
            {
                //std::cout << "Equel elements:     " << input_string[j] << "==" << c <<\n";
                j++;
                i++;
                std::cin >> c;
            }
            if(j == input_string.length())//подстрока найдена
            {
                //std::cout << "Sub string founded: " << input_string[j] << "::" << c << "\n";
                if(no_one_digit)//если более одной цифры, то между ними ставится запятая
                    std::cout << ',';
                std::cout << i - j;
                j = ind_lenghts[j-1];
                no_one_digit = true;
            }
            else if(input_string[j] != c && !std::cin.fail())//совпадения не обноружено или конец ввода
            {
                //std::cout << "Non equel:          " << input_string[j] << "!=" << c << "\n";
                if(j != 0)
                    j = ind_lenghts[j - 1];
                else
                {
                    i++;
                    std::cin >> c;
                }
            }

        }
        if(!no_one_digit)//если посдтроки найдено не было
            std::cout << -1;
    }

    ~SubStr(){}
};


int main()
{
    SubStr tmp;
    tmp.KMP();
}

/*
ab
ababababababbabaa
!
ab
ababbab
!
 */

