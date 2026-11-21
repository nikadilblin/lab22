#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// Определение типа указателя на функцию
typedef double (*TFun)(double);

// Прототипы математических функций
double Y(double x);
double V(double x);
double S(double x);

// Прототипы вспомогательных функций
void print_table(TFun f, double xn, double xk, double h, const char* name);
void plot_graph(TFun f, double x0, double x1, const char* name);
void save_to_file(TFun f, double xn, double xk, double h, const char* filename);
void read_from_file(TFun f, const char* filename);
void find_max_golden_section(TFun f, double a, double b);
void describe_function(TFun f, double a, double b);

// Меню
void menu() {
    printf("\n--- МЕНЮ ---\n");
    printf("1. Вычислить значение функции (выбор функции, задание значения x)\n");
    printf("2. Табулировать (выбор функции, интервал, шаг)\n");
    printf("3. Выполнить операцию (выбор функции и действие)\n");
    printf("4. Выход\n");
    printf("Выберите пункт: ");
}

int main() {
    int choice, func_choice;
    double x, xn, xk, h;
    TFun current_func = NULL;
    const char* func_name = "";

    while (1) {
        menu();
        scanf("%d", &choice);

        if (choice == 4) break;

        // Выбор функции для пунктов 1 и 2 (для пункта 3 выбор внутри)
        if (choice == 1 || choice == 2) {
            printf("Выберите функцию:\n1. Y(x)\n2. V(x)\n3. S(x)\nВаш выбор: ");
            scanf("%d", &func_choice);
            switch (func_choice) {
            case 1: current_func = Y; func_name = "Y(x)"; break;
            case 2: current_func = V; func_name = "V(x)"; break;
            case 3: current_func = S; func_name = "S(x)"; break;
            default: printf("Неверный выбор.\n"); continue;
            }
        }

        switch (choice) {
        case 1:
            printf("Введите значение x: ");
            scanf("%lf", &x);
            printf("%s = %lf\n", func_name, current_func(x));
            break;

        case 2:
            printf("Введите начало интервала (xn): "); scanf("%lf", &xn);
            printf("Введите конец интервала (xk): "); scanf("%lf", &xk);
            printf("Введите шаг (h): "); scanf("%lf", &h);
            print_table(current_func, xn, xk, h, func_name);
            break;

        case 3:
            printf("Выберите операцию:\n");
            printf("1. Записать значения в файл dat.txt\n");
            printf("2. Рассчитать значения из файла dat.txt\n");
            printf("3. Построить график\n");
            printf("4. Найти максимум методом золотого сечения\n");
            printf("5. Описать функцию на интервале\n");
            printf("Ваш выбор: ");
            int op;
            scanf("%d", &op);

            // Для операций нужно выбрать функцию
            printf("Выберите функцию:\n1. Y(x)\n2. V(x)\n3. S(x)\nВаш выбор: ");
            scanf("%d", &func_choice);
            switch (func_choice) {
            case 1: current_func = Y; func_name = "Y(x)"; break;
            case 2: current_func = V; func_name = "V(x)"; break;
            case 3: current_func = S; func_name = "S(x)"; break;
            default: printf("Неверный выбор.\n"); continue;
            }

            if (op == 1) {
                printf("Введите xn, xk, h: ");
                scanf("%lf %lf %lf", &xn, &xk, &h);
                save_to_file(current_func, xn, xk, h, "dat.txt");
            }
            else if (op == 2) {
                read_from_file(current_func, "dat.txt");
            }
            else if (op == 3) {
                printf("Введите x0 и x1 для графика: ");
                scanf("%lf %lf", &xn, &xk);
                plot_graph(current_func, xn, xk, func_name);
            }
            else if (op == 4) {
                printf("Введите a и b для поиска максимума: ");
                scanf("%lf %lf", &xn, &xk);
                find_max_golden_section(current_func, xn, xk);
            }
            else if (op == 5) {
                printf("Введите a и b для описания: ");
                scanf("%lf %lf", &xn, &xk);
                describe_function(current_func, xn, xk);
            }
            break;
        default:
            printf("Неверный пункт меню.\n");
        }
    }
    return 0;
}

// --- Реализация математических функций (Вариант 4) ---

// Y(x) = ln(sqrt(x^3)), при x > 0
double Y(double x) {
    if (x <= 0) return NAN;
    return log(sqrt(pow(x, 3)));
}

// V(x) - кусочная функция
double V(double x) {
    if (x > 1) {
        // sqrt(tg(x^2 - 1))
        double val = tan(x * x - 1);
        if (val < 0) return NAN;
        return sqrt(val);
    }
    else if (x >= 0 && x <= 1) {
        return -2 * x;
    }
    else {
        // e * cos(x)
        return exp(1) * cos(x); // exp(1) = e
    }
}

// S(x) = (x-1)/(x+1) + 1/3 * ((x-1)/(x+1))^3 + 1/5 * ((x-1)/(x+1))^5 + 1/7 * ((x-1)/(x+1))^7
double S(double x) {
    if (x == -1) return NAN; // Деление на ноль
    double t = (x - 1) / (x + 1);
    return t + (1.0 / 3.0) * pow(t, 3) + (1.0 / 5.0) * pow(t, 5) + (1.0 / 7.0) * pow(t, 7);
}

// --- Вспомогательные функции ---

void print_table(TFun f, double xn, double xk, double h, const char* name) {
    printf("\nТабуляция %s:\n", name);
    printf("----------------------\n");
    printf("|   x   |   %s   |\n", name);
    printf("----------------------\n");
    for (double x = xn; x <= xk + 1e-9; x += h) {
        double y = f(x);
        if (isnan(y)) printf("| %5.2f |  ERROR  |\n", x);
        else printf("| %5.2f | %7.4f |\n", x, y);
    }
    printf("----------------------\n");
}

void save_to_file(TFun f, double xn, double xk, double h, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) { printf("Ошибка открытия файла!\n"); return; }
    for (double x = xn; x <= xk + 1e-9; x += h) {
        fprintf(file, "%lf,%lf\n", x, f(x));
    }
    fclose(file);
    printf("Данные сохранены в %s\n", filename);
}

void read_from_file(TFun f, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) { printf("Ошибка открытия файла!\n"); return; }
    double x, y;
    printf("\nДанные из файла (x, f(x)):\n");
    while (fscanf(file, "%lf,%lf", &x, &y) == 2) {
        printf("x = %lf, f(x) = %lf (Проверка: %lf)\n", x, y, f(x));
    }
    fclose(file);
}

// Поиск максимума методом золотого сечения
void find_max_golden_section(TFun f, double a, double b) {
    const double phi = (1 + sqrt(5)) / 2;
    double x1, x2, y1, y2;
    double eps = 1e-5;

    while (fabs(b - a) > eps) {
        x1 = b - (b - a) / phi;
        x2 = a + (b - a) / phi;
        y1 = f(x1);
        y2 = f(x2);

        if (y1 < y2) a = x1;
        else b = x2;
    }
    double x_max = (a + b) / 2;
    printf("Максимум на интервале: x = %lf, f(x) = %lf\n", x_max, f(x_max));
}

// Описание функции (возрастание/убывание)
void describe_function(TFun f, double a, double b) {
    double step = (b - a) / 100;
    int increasing = 0, decreasing = 0;
    double prev = f(a);

    for (double x = a + step; x <= b; x += step) {
        double curr = f(x);
        if (curr > prev) increasing++;
        else if (curr < prev) decreasing++;
        prev = curr;
    }

    if (increasing > 0 && decreasing == 0) printf("Функция монотонно возрастает на [%lf, %lf]\n", a, b);
    else if (decreasing > 0 && increasing == 0) printf("Функция монотонно убывает на [%lf, %lf]\n", a, b);
    else printf("Функция немонотонна на [%lf, %lf] (есть участки возрастания и убывания)\n", a, b);
}

// Построение графика в консоли (ASCII-графика)
void plot_graph(TFun f, double x0, double x1, const char* name) {
    int width = 60, height = 20;
    double y_min = 1e9, y_max = -1e9;
    double step = (x1 - x0) / (width - 1);

    // Находим минимум и максимум для масштабирования
    for (int i = 0; i < width; i++) {
        double x = x0 + i * step;
        double y = f(x);
        if (isnan(y)) continue;
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;
    }

    if (y_max == y_min) { printf("Функция постоянна, график не построить.\n"); return; }

    // Инициализация поля
    char screen[20][60];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) screen[i][j] = ' ';

    // Отрисовка осей
    int zero_y = (int)((y_max - 0) / (y_max - y_min) * (height - 1));
    if (zero_y >= 0 && zero_y < height) {
        for (int j = 0; j < width; j++) screen[zero_y][j] = '-';
    }
    // Ось X (если x=0 в диапазоне)
    int zero_x = (int)((0 - x0) / (x1 - x0) * (width - 1));
    if (zero_x >= 0 && zero_x < width) {
        for (int i = 0; i < height; i++) screen[i][zero_x] = '|';
    }

    // Отрисовка точек
    for (int j = 0; j < width; j++) {
        double x = x0 + j * step;
        double y = f(x);
        if (isnan(y)) continue;
        int i = (int)((y_max - y) / (y_max - y_min) * (height - 1));
        if (i >= 0 && i < height) screen[i][j] = '*';
    }

    printf("\nГрафик %s на [%lf, %lf]:\n", name, x0, x1);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) putchar(screen[i][j]);
        putchar('\n');
    }
}