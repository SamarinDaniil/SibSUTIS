#include <bits/stdc++.h>

using namespace std;

#define M 10

class Fraction
{

public:
    long long numerator;
    long long denominator;

    Fraction() : numerator(0), denominator(1) {}
    Fraction(long long num, long long denom = 1) : numerator(num), denominator(denom) {}
    //
    bool operator==(const Fraction& other) const
    {
        return numerator * other.denominator == denominator * other.numerator;
    }
    bool operator==(long long value) const
    {
        return numerator == value * denominator;
    }
     bool operator<(const Fraction& other) const {
        return (numerator * other.denominator) < (other.numerator * denominator);
    }
    bool operator>(const Fraction& other) const {
        return (numerator * other.denominator) > (other.numerator * denominator);
    }

    bool operator<=(const Fraction& other) const {
        return (numerator * other.denominator) <= (other.numerator * denominator);
    }
    bool operator>=(const Fraction& other) const {
        return (numerator * other.denominator) >= (other.numerator * denominator);
    }
    bool operator!=(const Fraction& other) const {
        return (numerator != other.numerator) || (denominator != other.denominator);
    }
    //
    friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
        if (fraction.numerator == 0 || fraction.denominator == 1)
            os << fraction.numerator;
        else if (fraction.denominator == -1)
            os << -1*fraction.numerator;
        else
            os << fraction.numerator << "/" << fraction.denominator;
        return os;
    }
    Fraction operator/(const Fraction& other) const
    {
        if (other.numerator == 0)
        {
            std::cout << "Ошибка: деление на ноль" << std::endl;
            return Fraction();
        }

        long long num = numerator * other.denominator;
        long long den = denominator * other.numerator;
        long long k = std::__gcd(num,den);
        num/=k;
        den/=k;
        if (num == 0)
            den = 1;
        return Fraction(num, den);
    }
    Fraction operator*(const Fraction& other)
    {
        long long num = numerator * other.numerator;
        long long denom = denominator * other.denominator;
        long long k = std::__gcd(num,denom);
        num/=k;
        denom/=k;
        if (num == 0)
            denom = 1;
        return Fraction(num, denom);
    }
    Fraction operator+(const Fraction& other)
    {
        long long num = numerator * other.denominator + other.numerator * denominator;
        long long denom = denominator * other.denominator;
        long long k = std::__gcd(num,denom);
        num/=k;
        denom/=k;
        if (num == 0)
            denom = 1;
        return Fraction(num, denom);
    }
    Fraction operator-(const Fraction& other)
    {
        long long num = numerator * other.denominator - other.numerator * denominator;
        long long denom = denominator * other.denominator;
        long long k = std::__gcd(num,denom);
        num/=k;
        denom/=k;
        if (num == 0)
            denom = 1;
        return Fraction(num, denom);
    }
    Fraction& operator=(const Fraction& other)
    {
        numerator = other.numerator;
        denominator = other.denominator;
        long long k = std::__gcd(numerator,denominator);
        numerator/=k;
        denominator/=k;
        if (numerator == 0)
            denominator = 1;
        return *this;
    }
};

void PrintMatrix(Fraction a[][M], long long n,long long _column)
{
    for (long long i = 0; i < n; i++)
    {
        for (long long j = 0; j < _column; j++)
        {
            cout << a[i][j] <<" ";
        }
        cout << endl;
    }
}

int findMaxindex(Fraction a[][M], long long row, long long _row, long long indexColumn)
{
    int maxIndex = row;
    for (int i = row; i < _row; i++)
    {
        if (a[maxIndex][indexColumn] < a[i][indexColumn])
        {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void swapRow(Fraction a[][M], long long row, long long maxRow ,long long _column)
{
    for (int j = 0; j < _column; j++)
    {
        Fraction swp = a[row][j];
        a[row][j] = a[maxRow][j];
        a[maxRow][j] = swp;
    }
}

void divideRow(Fraction a[][M], long long row,long long _column, long long indexColumn)
{
    Fraction div = a[row][indexColumn];
    for (int j = 0; j < _column; j++)
    {
         a[row][j] = a[row][j]/div;
    }
}

void PerformOperation(Fraction a[][M], long long _row, long long _column)
{
    long long k = 0, c, flag = 0, m = 0;
    int indexRow = 0, indexColumn = 0, maxRow = 0;
    cout << " Индексы :" << _row << "x" << _column << endl;
    for (indexRow = 0; indexRow < _row; indexRow++)
    {
        maxRow = findMaxindex(a, indexRow, _row , indexColumn);
        if (a[maxRow][indexColumn] != 0 && maxRow != indexRow)
        {
            cout << a[indexRow][indexColumn] << ' ' << a[maxRow][indexColumn] << endl;
            swapRow(a, indexRow, maxRow, _column);
            cout << "<Свап: " << indexRow << " " << maxRow << " >" << endl;
            divideRow(a, indexRow, _column, indexColumn);
        }
        if (a[indexRow][indexColumn] != 0)
            divideRow(a, indexRow, _column, indexColumn);
        if (a[maxRow][indexColumn] == 0)
        {
            while(a[findMaxindex(a, indexRow, _row, indexColumn)][indexColumn] == 0)
            {
                indexColumn++;
                if (indexColumn > _column-1)
                    break;
            }
            int maxRow2 = findMaxindex(a, indexRow, _row, indexColumn);
            if (a[maxRow2][indexColumn] != 0 && maxRow2 != indexRow)
            {
                swapRow(a, indexRow, maxRow2, _column);
                cout << "<!Свап: " << indexRow << " " << maxRow2 << " >" << endl;
                divideRow(a, indexRow, _column, indexColumn);
            }
        }
        /*
        for (long long i = 0; i < _row; i++)
        {
            if (i == indexRow)
                continue;
            for(long long j = indexColumn+1; j < _column; j++)
            {
                a[i][j+1] = a[indexRow][indexColumn]*a[i][j+1] - a[indexRow][j+1]*a[i][indexColumn];
            }
             a[i][indexColumn] = Fraction(0);
        }
        indexColumn++;
        PrintMatrix(a, _row,_column);
        cout << string(50, '#') << endl;
        /*/
        if ( a[indexRow][indexColumn] == 0)
            continue;
        for (long long j = 0; j < _row; j++)
        {
            if (indexRow != j)
            {

                Fraction pro = a[j][indexColumn] / a[indexRow][indexColumn];

                for (k = 0; k < _column; k++)
                    a[j][k] = a[j][k] - (a[indexRow][k]) * pro;
            }
            PrintMatrix(a, _row, _column);
            cout << string(50, '#') << endl;
        }
        indexColumn++;
    }
}

void PrintResult(Fraction a[][M], long long _row, long long _column, long long flag)
{
    cout << "Ответ: ";

    if (flag == 2)
    {
        cout << "Бесконечное множество." << endl;
        for (long long i = 0; i < _row; i++)
        {
            bool kek = false;
            for(long long j = 0 ; j < _column-1; j++)
            {
                if (a[i][j].numerator != 0)
                {
                    if (kek == false)
                    {
                        cout << " [x" << j+1 << "] = " << a[i][_column-1] << " ";
                        kek = true;
                        continue;
                    }
                    a[i][j].numerator *=-1;
                    if (a[i][j].numerator > 0)
                        cout << "+" << a[i][j] <<"[x" << j+1 << "] ";
                    else
                        cout << a[i][j] <<"[x" << j+1 << "] ";
                }

            }
        }

    }
    else if (flag == 3)
        cout << "Нет решений!" << endl;
    else
    {
        for (long long i = 0; i < _row; i++)
        {
            cout << " x" << i+1 << " = " << a[i][_column-1] << " ";
        }
    }
}

long long CheckConsistency(Fraction a[][M], long long n, long long flag,long long _column)
{
    long long i, j;
    Fraction sum;

    // flag == 2 for infinite solution
    // flag == 3 for No solution
    flag = 1;
    for (i = 0; i < n; i++)
    {
        sum = Fraction(0);
        int check = 0;
        for (j = 0; j < _column; j++)
        {
            if (a[i][j].numerator != 0)
                check++;
        }
        if (check > 2)
            return 2;
        if (check == 1 && a[i][_column].numerator == 0)
            return 2;
        if (a[i][_column].numerator != 0 && check == 1)
            return 3;
    }
    return flag;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    Fraction a[M][M];
    std::ifstream inputFile("13.txt");
    if (!inputFile.is_open())
    {
        std::cout << "<Не удалось открыть файл!>" << std::endl;
        return 1;
    }
    // Считываем числа из файла
    long long num, _row = 0, _column = 0;
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        _column = 0;
        while (iss >> num)
        {
            a[_row][_column] = Fraction(num);
            _column++;
        }
        _row++;
    }
    inputFile.close();
    //
    PrintMatrix(a, _row,_column);

    long long flag = 0, n = _row;

    PerformOperation(a, _row,_column);
    flag = CheckConsistency(a, n, flag,_column);
    PrintResult(a,_row,_column,flag);
    return 0;
}
