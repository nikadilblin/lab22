#include <stdio.h>
#include <math.h>

// Определение типа указателя на функцию
typedef double (*TFun)(double);

// Прототипы математических функций
double Y(double x);
double V(double x);
double S(double x);

// ==========================================
// ЗАДАНИЕ 3: Вариант 4
// Вычисляет количество всех значений функции у, 
// для которых выполняются неравенства y > 0,4 или y < -3
// ==========================================
int calculate(TFun prtf, double* x, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        double y = prtf(x[i]);

        // Проверяем, не является ли значение недопустимым (например, корень из отрицательного или логарифм)
        if (isnan(y)) continue;

        if (y > 0.4 || y < -3.0) {
            count++;
        }
    }
    return count;
}

// --- Пример использования ---
int main() {
    // Задаем массив значений x для проверки
    double x_arr[] = { -2.5, -1.0, 0.0, 0.5, 1.0, 1.5, 2.0, 3.0 };
    int n = sizeof(x_arr) / sizeof(x_arr[0]);

    printf("Проверяемый массив x: ");
    for (int i = 0; i < n; i++) printf("%.2f ", x_arr[i]);
    printf("\n\n");

    // Вызов функции calculate для каждой из функций
    int count_Y = calculate(Y, x_arr, n);
    printf("Количество значений Y(x), удовлетворяющих условию (y > 0.4 или y < -3): %d\n", count_Y);

    int count_V = calculate(V, x_arr, n);
    printf("Количество значений V(x), удовлетворяющих условию (y > 0.4 или y < -3): %d\n", count_V);

    int count_S = calculate(S, x_arr, n);
    printf("Количество значений S(x), удовлетворяющих условию (y > 0.4 или y < -3): %d\n", count_S);

    return 0;
}

// ==========================================
// Математические функции (Вариант 4 из предыдущего задания)
// ==========================================

// Y(x) = ln(sqrt(x^3)), при x > 0
double Y(double x) {
    if (x <= 0) return NAN;
    return log(sqrt(pow(x, 3)));
}

// V(x) - кусочная функция
double V(double x) {
    if (x > 1) {
        double val = tan(x * x - 1);
        if (val < 0) return NAN;
        return sqrt(val);
    }
    else if (x >= 0 && x <= 1) {
        return -2 * x;
    }
    else {
        return exp(1) * cos(x); // e * cos(x)
    }
}

// S(x) = t + (1/3)*t^3 + (1/5)*t^5 + (1/7)*t^7, где t = (x-1)/(x+1)
double S(double x) {
    if (x == -1) return NAN;
    double t = (x - 1) / (x + 1);
    return t + (1.0 / 3.0) * pow(t, 3) + (1.0 / 5.0) * pow(t, 5) + (1.0 / 7.0) * pow(t, 7);
}