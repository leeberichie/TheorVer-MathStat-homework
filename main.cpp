// prob_service_plot.cpp
#include <matplot/matplot.h>
#include <vector>
#include <algorithm>
using namespace matplot;

int main() {
    double T = 80.0;
    double t1 = 15.0;
    double t2 = 20.0;

    int N = 400; // сетка для x и y
    std::vector<double> xs(N), ys(N);
    for(int i=0;i<N;i++){
        xs[i] = i*T/(N-1);
        ys[i] = i*T/(N-1);
    }

    // Функция для проверки обслуживания обеих заявок
    auto both_served = [&](double x, double y){
        if(x<y){
            return (x <= T - t1) && (y >= x + t1) && (y <= T - t2);
        } else if(y < x){
            return (y <= T - t2) && (x >= y + t2) && (x <= T - t1);
        }
        return false;
    };

    // Функция для проверки обслуживания первой
    auto first_served = [&](double x, double y){
        double end = 0.0;
        bool s1=false, s2=false;
        if(x<y){
            if(x <= T-t1){ s1=true; end = x+t1; }
            if(y <= T-t2 && y >= end) s2=true;
        } else if(y<x){
            if(y <= T-t2){ s2=true; end=y+t2; }
            if(x <= T-t1 && x >= end) s1=true;
        } else { // x==y
            if(x <= T-t1) s1=true;
        }
        return s1;
    };

    // Функция для проверки обслуживания второй
    auto second_served = [&](double x, double y){
        double end = 0.0;
        bool s1=false, s2=false;
        if(x<y){
            if(x <= T-t1){ s1=true; end=x+t1; }
            if(y <= T-t2 && y >= end) s2=true;
        } else if(y<x){
            if(y <= T-t2){ s2=true; end=y+t2; }
            if(x <= T-t1 && x >= end) s1=true;
        } else { // x==y
            if(y <= T-t2) s2=true;
        }
        return s2;
    };

    // Создаем сетку Z для визуализации
    matrix<double> Z_both(N, N), Z_first(N,N), Z_second(N,N), Z_only_one(N,N);

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            double x=xs[j], y=ys[i];
            bool b1=first_served(x,y), b2=second_served(x,y);
            Z_both(i,j) = both_served(x,y)?1.0:0.0;
            Z_first(i,j) = b1?1.0:0.0;
            Z_second(i,j) = b2?1.0:0.0;
            Z_only_one(i,j) = (b1^b2)?1.0:0.0; // только одна
        }
    }

    // --- Построение графиков ---
    auto f1 = figure(true);
    imagesc(xs, ys, Z_both);
    colorbar();
    title("Обслужены обе заявки");
    xlabel("x (поступление первой)");
    ylabel("y (поступление второй)");
    save("both_served.png");

    auto f2 = figure(true);
    imagesc(xs, ys, Z_only_one);
    colorbar();
    title("Обслужена только одна заявка");
    xlabel("x");
    ylabel("y");
    save("only_one_served.png");

    auto f3 = figure(true);
    imagesc(xs, ys, Z_first);
    colorbar();
    title("Обслужена первая заявка");
    xlabel("x");
    ylabel("y");
    save("first_served.png");

    auto f4 = figure(true);
    imagesc(xs, ys, Z_second);
    colorbar();
    title("Обслужена вторая заявка");
    xlabel("x");
    ylabel("y");
    save("second_served.png");

    // график условной вероятности: первая раньше второй среди обслуженных обеих
    matrix<double> Z_first_before_both(N,N,0.0);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            double x=xs[j], y=ys[i];
            if(both_served(x,y)){
                Z_first_before_both(i,j) = (x<y)?1.0:0.0;
            }
        }
    }
    auto f5 = figure(true);
    imagesc(xs, ys, Z_first_before_both);
    colorbar();
    title("Первая обслужена раньше второй (среди обеих)");
    xlabel("x");
    ylabel("y");
    save("first_before_second.png");

    show();
    return 0;
}
