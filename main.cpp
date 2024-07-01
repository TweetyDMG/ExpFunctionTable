#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <locale>
#include <windows.h>

#define E 2.7182818284

using namespace std;

double factorial(int n);
double series(double x, double epsilon, int& numTerms);
double func(double x);
void clearConsole();

bool isValidInput(double xn, double xk, double dx) {
    return (xn < xk) && (dx > 0) && ((xk - xn) / dx >= 1);
}

void clearConsole() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int main() {
    // Установка локали для корректного вывода
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char choice;
    do {
        clearConsole();

        double xn, xk, dx;
        cout << "Привет! Это программа выведет значения функции e^(-x) в виде таблицы\n" << endl;
        while (true) {
            cout << "Введите начальное значение x (xn): ";
            cin >> xn;
            cout << "Введите конечное значение x (xk): ";
            cin >> xk;
            cout << "Введите шаг изменения x (dx): ";
            cin >> dx;

            if (isValidInput(xn, xk, dx)) {
                break;
            }
            else {
                clearConsole();
                cout << "Ошибка: неверные значения xn, xk или dx. Убедитесь, что xn < xk и dx > 0.\n";
            }
        }

        double totalRate1 = 0, totalRate2 = 0;
        int numPoints1 = 0, numPoints2 = 0;

        cout << " _______________________________________________________________________________\n";
        cout << " |    x   |  eps1 |   f1   |  rate1  |    eps2    |   f2   |  rate2  |   f(x)  |\n";
        cout << " |        |  0.1  |        |         |  0.000001  |        |         |         |\n";
        cout << " |--------|-------|--------|---------|------------|--------|---------|---------|\n";

        for (double x = xn; x <= xk; x += dx) {
            int numTerms1 = 0, numTerms2 = 0;
            double fx = func(x);
            double result1 = series(x, 0.1, numTerms1);
            double result2 = series(x, 0.000001, numTerms2);
            double rate1 = (result1 != 0) ? fabs(log(fabs(result1)) / log(numTerms1)) : 0;
            double rate2 = (result2 != 0) ? fabs(log(fabs(result2)) / log(numTerms2)) : 0;

            if (isnan(result1) || result1 <= 0) {
                result1 = numeric_limits<double>::quiet_NaN();
                rate1 = numeric_limits<double>::quiet_NaN();
            }
            if (isnan(result2) || result2 <= 0) {
                result2 = numeric_limits<double>::quiet_NaN();
                rate2 = numeric_limits<double>::quiet_NaN();
            }

            if (!isnan(rate1)) {
                totalRate1 += rate1;
                numPoints1++;
            }
            if (!isnan(rate2)) {
                totalRate2 += rate2;
                numPoints2++;
            }

            cout << fixed << setprecision(4);
            cout << " | " << setw(4) << x
            << " |       | " << setw(6) << result1
            << " | " << setw(7) << setprecision(5) << rate1
            << " |            | " << setw(6) << setprecision(4) << result2
            << " | " << setw(7) << setprecision(5) << rate2
            << " | " << setw(6) << fx
            << " |\n";
        }
        cout << " |--------|-------|--------|---------|------------|--------|---------|---------|\n";
        cout << " | Total  |       |        | " << setw(7) << setprecision(5) << totalRate1 / numPoints1
        << " |            |        | " << setw(7) << setprecision(5) << totalRate2 / numPoints2
        << " |         |\n";
        cout << " -------------------------------------------------------------------------------\n";

        cout << "Хотите повторить ввод значений? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

double factorial(int n) {
    int r;
    for (r = 1; n > 1; r *= (n--));
    return r;
}

double series(double x, double epsilon, int& numTerms) {
    double result = 0.0;
    double term = 1.0;
    int k = 0;
    numTerms = 0;

    while (fabs(term) > epsilon) {
        result += term;
        term *= (-x) / (k + 1);
        k++;
        numTerms++;
    }
    return result;
}

double func(double x) {
    return pow(E, -x);
}
