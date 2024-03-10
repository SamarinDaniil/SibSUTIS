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


void print(Fraction arr[][M], int n, int m)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (arr[i][j].denominator == 1 || arr[i][j].numerator == 0) {
				cout << arr[i][j].numerator << " ";
			}
			else
				cout << arr[i][j].numerator << "/" << arr[i][j].denominator << " ";
		}
		cout << "| ";
		if (arr[i][n - 1].denominator == 1 || arr[i][n - 1].numerator == 0) {
			cout << arr[i][n - 1].numerator << endl;
		}
		else
			cout << arr[i][n - 1].numerator << "/" << arr[i][n - 1].denominator << endl;
	}
	cout << endl;
}

long long NOD(long long a, long long b)
{
	if (b == 0) return a;
	else return NOD(b, a % b);
}

void divide(Fraction arr[][M], int n, int m) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (arr[i][j].denominator != 0)
				if (arr[i][j].numerator % arr[i][j].denominator == 0) {
					arr[i][j].numerator /= arr[i][j].denominator;
					arr[i][j].denominator = 1;
				}
				else {
					long long d = NOD(arr[i][j].denominator, arr[i][j].numerator);
					if (d != 0) {
						arr[i][j].denominator /= d;
						arr[i][j].numerator /= d;
					}
				}
		}
	}
}

long long fact(long long n)
{
	if (n <= 1)
		return 1;

	return n * fact(n - 1);
}

int CountRang(Fraction arr[][M], int n, int m) {
	int rang = 0;
	for (int i = 0; i < m; i++)
	{
		int count = 0;
		for (int j = 0; j < n; j++) {
			if (arr[i][j].numerator != 0) {
				break;
			}
			count++;
		}
		if (count != n)
			rang++;
	}
	return rang;
}


long long CombinNum(int rang, int X_count)
{
	if (rang == X_count)
		return 1;

	return fact(X_count) / (fact(X_count - rang) * fact(rang));
}

bool NextSequence(int* a, int X_count, int rang)
{
	for (int i = rang - 1; i >= 0; --i)
		if (a[i] < X_count - rang + i + 1)
		{
			++a[i];
			for (int j = i + 1; j < rang; ++j)
				a[j] = a[j - 1] + 1;
			return true;
		}
	return false;
}

void bazis (Fraction arr[][M], int n, int m) {
	int rang = CountRang(arr, n, m);
	int combination_num = CombinNum(rang, n - 1);
	cout << "######################" << endl << "Rang: \t" << rang << endl << "Kol-vo X: \t" << n - 1 << endl << "C: \t" << combination_num << endl << "######################" << endl;

	//генерируем сочетания
	int* buf_comb = new int[rang];
	int* comb = new int[rang * combination_num];
	int combin_index = rang;
	for (int i = 0; i < rang; i++)
	{
		buf_comb[i] = i + 1;
		comb[i] = i + 1;
	}

	while (NextSequence(buf_comb, n - 1, rang))
	{
		for (int i = 0; i < rang; i++)
		{
			comb[combin_index] = buf_comb[i];
			combin_index++;
		}
	}
	cout << endl << "Combin sequence:" << endl;
	for (int i = 0; i < rang * combination_num; i++) {
		cout << comb[i] << " ";
		if (i % rang == rang - 1)
			cout << " | ";
	}
	cout << endl << endl << endl << endl;

	//поиск решений
	for (int i = 0; i < combination_num; i++) {
		int break_flag = 0;
		cout << "\t###" << i + 1 << " [";
		for (int j = 0; j < rang; j++) {
			cout << " " << comb[i * rang + j];
		}
		cout << " ]###" << endl << endl;

		Fraction buf[M][M];
		for (int v = 0; v < m; v++)
		{
			for (int j = 0; j < n; j++)
			{
				buf[v][j] = arr[v][j];
			}
		}
		print(buf, n, m);

		int v_memmory = -1;
		for (int v = 0; v < rang; v++) {
			//если элемент комбинации равен 0, то проверяем может ли этот базис вообще существовать
			if (buf[v][comb[i * rang + v] - 1].numerator == 0) {
				//если элементы в каждом столбце равны 0, то базиса нет
				int count = 0;
				for (int j = 0; j < rang; j++) {
					if (buf[v][comb[i * rang + j] - 1].numerator == 0) {
						count++;
					}
				}
				if (count == rang) {
					cout << "NO" << endl;
					break_flag = 1;
					break;
				}
				//иначе вернёмся к этой строке потом
				else {
					//не запоминаем строку, если была пропущена ещё одна ранеее
					if (v_memmory != -1)
						continue;
					v_memmory = v;
					continue;
				}
			}
			//если элемент - отрицательное число
			if (buf[v][comb[i * rang + v] - 1].numerator / buf[v][comb[i * rang + v] - 1].denominator < 0) {
				for (int k = 0; k < n; k++) {
					buf[v][k].numerator *= -1;
				}
			}
			//если ведущий элемент не = 1
			if (buf[v][comb[i * rang + v] - 1].numerator / buf[v][comb[i * rang + v] - 1].denominator != 1 || buf[v][comb[i * rang + v] - 1].numerator % buf[v][comb[i * rang + v] - 1].denominator != 0) {
				long long num = buf[v][comb[i * rang + v] - 1].numerator;
				long long den = buf[v][comb[i * rang + v] - 1].denominator;
				for (int k = 0; k < n; k++) {
					buf[v][k].numerator *= den;
					buf[v][k].denominator *= num;
				}
				divide(buf, n, m);
				print(buf, n, m);
			}
			// 0 элементы выше и ниже ведущего элемента
			for (int j = 0; j < m; j++) {
				if (v == j) {
					continue;
				}
				//если нашли строку с не нулевым элементом в ведущем столбце
				if (buf[j][comb[i * rang + v] - 1].numerator != 0) {
					for (int k = 0; k < n; k++) {
						if (k == comb[i * rang + v] - 1) {
							continue;
						}
						long long num = buf[j][comb[i * rang + v] - 1].numerator * buf[v][k].numerator;
						long long den = buf[j][comb[i * rang + v] - 1].denominator * buf[v][k].denominator;
						//если у уменьшаемого и вычитаемого разные знаменатели - домнажаем на знаменатели друг друга
						if (den != buf[j][k].denominator && buf[j][k].denominator != 0 && buf[j][k].numerator != 0 && den != 0) {
							long long buffer = den;
							num *= buf[j][k].denominator;
							den *= buf[j][k].denominator;

							buf[j][k].numerator *= buffer;
							buf[j][k].denominator *= buffer;
						}
						buf[j][k].numerator -= num;
						if (buf[j][k].denominator == 0)
							buf[j][k].denominator = den;
					}
					buf[j][comb[i * rang + v] - 1].numerator = 0;
					buf[j][comb[i * rang + v] - 1].denominator = 0;
					divide(buf, n, m);
					print(buf, n, m);
				}
			}
			//если мы пропустили строку, к которой потом вернёмся
			if (v_memmory != -1) {
				//если элемент не поменялся, то продолжаем дальше преобразовать, к строке вернёмся позже
				if (buf[v][comb[i * rang + v_memmory] - 1].numerator != 0) {
					v = v_memmory - 1;
					v_memmory = -1;
				}
			}
			//проверяем всё ли преобразовалось
			if (v + 1 == rang) {
				int schet = 0;
				for (int j = 0; j < rang; j++) {
					if (buf[j][comb[i * rang + j] - 1].numerator == 1) {
						schet++;
					}
				}
				if (schet != rang) {
					v = -1;
				}
			}
		}
		divide(buf, n, m);
		// вывод
		if (break_flag) {
			cout << "######################" << endl << endl;
			continue;
		}

		cout << "X" << i + 1 << " = (";
		int index = 0;
		for (int j = 0; j < n - 1; j++) {
			cout << " ";
			if (j == comb[i * rang + index] - 1) {
				if (buf[index][n - 1].numerator == 0) {
					cout << "0";
				}
				else {
					cout << buf[index][n - 1].numerator;
					if (buf[index][n - 1].denominator != 1) {
						cout << "/" << buf[index][n - 1].denominator;
					}
				}
				index++;
			}
			else {
				cout << "0";
			}
			cout << ";";
		}
		cout << ")" << endl;
		cout << "######################S " << endl << endl;
	}
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Fraction a[M][M];
    std::ifstream inputFile("77.txt");
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
    bazis(a, _column, _row);
    flag = CheckConsistency(a, n, flag,_column);
    PrintResult(a,_row,_column,flag);
    return 0;
}
