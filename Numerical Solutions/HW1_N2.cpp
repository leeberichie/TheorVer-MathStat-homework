// task2_prob.cpp
#include <bits/stdc++.h>
using namespace std;

/*
  Рассчитывает вероятности для задачи 2:
  - аналитические/численные интегралы делаются посредством сеточной интеграции на прямоугольнике R = [0,k1] x [1-k2,1]
  - дополнительно Monte Carlo для проверки
*/

int main(){
    // параметры (L=1 нормировано)
    double k1 = 0.9;
    double k2 = 0.75;
    double k = 0.15;

    double x_min = 0.0, x_max = k1;
    double n_min = 1.0 - k2, n_max = 1.0;

    // сеточная интеграция: N_x * N_n точек
    int Nx = 2000; // можно увеличить для большей точности
    int Nn = 2000;
    double dx = (x_max - x_min)/Nx;
    double dn = (n_max - n_min)/Nn;

    double area_total = k1 * k2;

    double A1=0, A2=0, A3=0, A4=0, A5=0; // площади событий

    for(int i=0;i<Nx;i++){
        double x = x_min + (i+0.5)*dx; // центр ячейки
        for(int j=0;j<Nn;j++){
            double n = n_min + (j+0.5)*dn;
            double a = x;
            double c = 1.0 - n;
            double b = fabs(n - x);
            // 1) x < n
            if (x < n) A1 += dx*dn;
            // 2) MN < k
            if (b < k) A2 += dx*dn;
            // 3) triangle: max < 0.5
            if (max(a, max(b, c)) < 0.5) A3 += dx*dn;
            // 4) min <= 0.5 * max
            double mn = min(a, min(b, c));
            double mx = max(a, max(b, c));
            if (mn <= 0.5 * mx) A4 += dx*dn;
            // 5) short*long < middle^2
            vector<double> L = {a,b,c};
            sort(L.begin(), L.end());
            if (L[0]*L[2] < L[1]*L[1]) A5 += dx*dn;
        }
    }

    cout.setf(ios::fixed);
    cout<<setprecision(5);
    cout << "Parameters: k1="<<k1<<", k2="<<k2<<", k="<<k<<"\n";
    cout << "Area total = " << area_total << "\n\n";

    cout << "Grid integration (Nx="<<Nx<<", Nn="<<Nn<<"):\n";
    cout << " P(AM<AN)       ~ " << (A1/area_total) << "  (area="<<A1<<")\n";
    cout << " P(MN<kL)       ~ " << (A2/area_total) << "  (area="<<A2<<")\n";
    cout << " P(triangle)    ~ " << (A3/area_total) << "  (area="<<A3<<")\n";
    cout << " P(min<=0.5max) ~ " << (A4/area_total) << "  (area="<<A4<<")\n";
    cout << " P(short*long < mid^2) ~ " << (A5/area_total) << "  (area="<<A5<<")\n\n";

    // Monte Carlo for cross-check
    std::mt19937_64 rng(123456789);
    std::uniform_real_distribution<double> Ux(x_min, x_max);
    std::uniform_real_distribution<double> Un(n_min, n_max);

    int Nsim = 2000000;
    int c1=0,c2=0,c3=0,c4=0,c5=0;
    for(int t=0;t<Nsim;t++){
        double x = Ux(rng), n = Un(rng);
        double a = x, c = 1.0 - n, b = fabs(n-x);
        if (x < n) c1++;
        if (b < k) c2++;
        if (max(a,max(b,c)) < 0.5) c3++;
        double mn = min(a,min(b,c)), mx = max(a,max(b,c));
        if (mn <= 0.5*mx) c4++;
        vector<double> L = {a,b,c}; sort(L.begin(), L.end());
        if (L[0]*L[2] < L[1]*L[1]) c5++;
    }

    cout << "MonteCarlo (N="<<Nsim<<"):\n";
    cout << " P(AM<AN)       ~ " << double(c1)/Nsim << "\n";
    cout << " P(MN<kL)       ~ " << double(c2)/Nsim << "\n";
    cout << " P(triangle)    ~ " << double(c3)/Nsim << "\n";
    cout << " P(min<=0.5max) ~ " << double(c4)/Nsim << "\n";
    cout << " P(short*long < mid^2) ~ " << double(c5)/Nsim << "\n";

    return 0;
}
