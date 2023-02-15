#include "array"
#include <algorithm>
#include "vector"
#include <iostream>

using namespace std;


static inline string ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
    return s;
}

static inline string rtrim(string s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
    return s;
}

static inline string trim(string &s) {
    return ltrim(rtrim(s));
}


/// <summary>
/// Род
/// </summary>
enum class Gender {
    /// <summary>
    /// Средний
    /// </summary>
    Neuter = 0,
    /// <summary>
    /// Мужской
    /// </summary>
    Masculine = 1,
    /// <summary>
    /// Женский
    /// </summary>
    Feminine = 2
};


/// <summary>
/// Число прописью
/// </summary>
class Number2Words {
    /// <summary>
    /// Число прописью
    /// </summary>
    /// <param name="number"> Число </param>
    /// <param name="gender">Род</param>
    /// <param name="titles">Три формы измерения. [один] "кит", [два] "кита", [пять] "китов" </param>
    /// <exception cref="ArgumentOutOfRangeException"></exception>
    /// <example>
    ///     2.toWords(Gender::Musculine, new[] { "яблоко", "яблока", "яблок" }) // два яблока
    /// </example>

private:


public:
    static string toWords(long long number, Gender gender, const vector<string> &titles = {}) {

        if (!titles.empty() && titles.size() != 3) {
            throw invalid_argument("The array should contain three forms");
        }
        std::array<long long, 5> powers =
                {
                        1000ll, // 10^3
                        1000ll * 1000, // 10^6
                        1000ll * 1000 * 1000, // 10^9
                        1000ll * 1000 * 1000 * 1000, // 10^12
                        1000ll * 1000 * 1000 * 1000 * 1000, // 10^15
                };
        array<vector<string>, 5> powerTitles =
                {
                        {
                                {"тысяча", "тысячи", "тысяч"},
                                {"миллион", "миллиона", "миллионов"},
                                {"миллиард", "миллиарда", "миллиардов"},
                                {"триллион", "триллиона", "триллионов"},
                                {"квадриллион", "квадриллиона", "квадриллионов"}
                        }
                };
        string result;
        long long tmpValue;
        if (number >= 0) {
            result = "";
            tmpValue = number;
        } else {
            result = "минус";
            tmpValue = -number;
        }

        for (int i = powers.size() - 1; i >= 0; i--) {
            if (tmpValue < powers.at(i)) continue;
            if (!result.empty()) {
                result += " ";
            }
            result += numberLess1000ToWord(tmpValue / powers.at(i), i == 0 ? Gender::Feminine : Gender::Masculine,
                                           powerTitles.at(i));
            tmpValue %= powers.at(i);
        }
        if (!result.empty()) {
            result += " ";
        }
        result += numberLess1000ToWord(tmpValue, gender, titles, number != 0);
        return trim(result);

    }


    static string
    numberLess1000ToWord(long long number, Gender gender, const vector<string> &titles, bool ignoreZero = true) {

        string result;
        int titleIndex;
        if (number > 99) {
            array<string, 10> hundreds = {"", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот",
                                          "восемьсот", "девятьсот"};
            result += hundreds.at(number / 100);
            number %= 100;
        }
        if (number > 19) {
            array<string, 10> tens = {"", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят",
                                      "восемьдесят", "девяносто"};
            if (!result.empty()) {
                result += " ";
            }
            result += tens.at(number / 10);
            number %= 10;
        }
        if (number == 0) {
            if (!ignoreZero) {
                if (!result.empty()) {
                    result += " ";
                }
                result += "ноль";
            }
        } else {
            if (!result.empty()) {
                result += " ";
            }

            if (number == 1) {
                switch (gender) {
                    case Gender::Neuter:
                        result += "одно";
                        break;
                    case Gender::Masculine:
                        result += "один";
                        break;
                    case Gender::Feminine:
                        result += "одна";
                        break;
                    default:
                        throw invalid_argument("Unknown gender: " + std::to_string(static_cast<int>(gender)));
                }
            } else if (number == 2) {
                switch (gender) {
                    case Gender::Neuter:
                    case Gender::Masculine:
                        result += "два";
                        break;
                    case Gender::Feminine:
                        result += "две";
                        break;
                    default:
                        throw invalid_argument("Unknown gender: " + to_string(static_cast<int>(gender)));
                }
            } else {
                array<string, 20> numbers =
                        {
                                "0", "1", "2", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять",
                                "десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать",
                                "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"
                        };
                result += numbers.at(number);
            }
        }
        // Добавить слово после числа
        if (number == 1)
            titleIndex = 0;
        else if (number >= 2 && number <= 4)
            titleIndex = 1;
        else
            titleIndex = 2;

        if (!titles.empty()) {
            if (!result.empty()) result += " ";
            result += titles.at(titleIndex);
        }
        return result;
    }
};


int main() {
    auto number_words = Number2Words::toWords(12345678901, Gender::Neuter, {"яблоко", "яблока", "яблок"});
    cout << "Результат - " << number_words << endl;
    return 0;
}
