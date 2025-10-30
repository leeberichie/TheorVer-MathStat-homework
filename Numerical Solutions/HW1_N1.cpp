// prob_service.cpp
#include <bits/stdc++.h>
using namespace std;

int main(){
    double T = 80.0;
    double t1 = 15.0;
    double t2 = 20.0;

    auto sq = [](double x){ return x*x; };
    double area_total = T*T;

    // area where both served: (T - t1 - t2)^2 if positive
    double D = T - t1 - t2;
    double area_both = (D>0) ? sq(D) : 0.0;

    // area first served (general formula derived in solution)
    double A1_part1 = 0.0; // integral_{0}^{T-t1} (T - x) dx
    if (T - t1 > 0) {
        double u = T - t1;
        A1_part1 = u * T - 0.5 * u * u;
    }
    double A1_part2 = 0.0; // (t2 - t1)_+^2 / 2
    if (t2 > t1) A1_part2 = 0.5 * sq(t2 - t1);
    double A1_part3 = (D>0) ? 0.5 * sq(D) : 0.0;
    double area_first = A1_part1 + A1_part2 + A1_part3;

    // area second served (swap roles)
    double A2_part1 = 0.0;
    if (T - t2 > 0) {
        double u = T - t2;
        A2_part1 = u * T - 0.5 * u * u;
    }
    double A2_part2 = 0.0;
    if (t1 > t2) A2_part2 = 0.5 * sq(t1 - t2);
    double A2_part3 = (D>0) ? 0.5 * sq(D) : 0.0;
    double area_second = A2_part1 + A2_part2 + A2_part3;

    // probabilities
    double p_both = area_both / area_total;
    double p_first = area_first / area_total;
    double p_second = area_second / area_total;
    double p_only_one = p_first + p_second - 2.0 * p_both;

    // conditional probabilities
    double p_first_given_second = (p_second>0) ? (p_both / p_second) : 0.0;
    double p_second_given_first = (p_first>0) ? (p_both / p_first) : 0.0;
    double p_first_before_second_given_both = (area_both>0) ? 0.5 : 0.0;

    cout.setf(ios::fixed); cout<<setprecision(5);
    cout << "T="<<T<<", t1="<<t1<<", t2="<<t2<<"\n\n";
    cout << "P(both) = " << p_both << "  (area="<<area_both<<")\n";
    cout << "P(first) = " << p_first << "  (area="<<area_first<<")\n";
    cout << "P(second)= " << p_second << "  (area="<<area_second<<")\n";
    cout << "P(only one) = " << p_only_one << "\n\n";
    cout << "P(first | second) = " << p_first_given_second << "\n";
    cout << "P(second | first) = " << p_second_given_first << "\n";
    cout << "P(first before second | both) = " << p_first_before_second_given_both << "\n\n";

    // --- Monte Carlo check ---
    int N = 2000000;
    std::mt19937_64 rng(1234567);
    std::uniform_real_distribution<double> U(0.0, T);
    int cnt_both = 0, cnt_first = 0, cnt_second = 0, cnt_only_one = 0, cnt_both_first_before = 0;
    for (int i=0;i<N;i++){
        double x = U(rng), y = U(rng);
        // process events in time order
        double end = 0.0;
        bool s1=false, s2=false;
        if (x < y){
            // first arrives first
            if (x <= T - t1) { s1 = true; end = x + t1; }
            if (y <= T - t2 && y >= end) s2 = true;
        } else if (y < x){
            if (y <= T - t2) { s2 = true; end = y + t2; }
            if (x <= T - t1 && x >= end) s1 = true;
        } else { // x==y (probability 0 for continuous, ignore)
            if (x <= T - t1) s1 = true;
            // tie-break not important
        }
        if (s1) cnt_first++;
        if (s2) cnt_second++;
        if (s1 && s2) {
            cnt_both++;
            // who starts earlier?
            if (x < y) cnt_both_first_before++;
            else if (y < x) ; // second before first
        }
        if ((s1 && !s2) || (!s1 && s2)) cnt_only_one++;
    }
    cout << "MonteCarlo (N="<<N<<"):\n";
    cout << " P(both) ~ " << double(cnt_both)/N << "\n";
    cout << " P(first) ~ " << double(cnt_first)/N << "\n";
    cout << " P(second) ~ " << double(cnt_second)/N << "\n";
    cout << " P(only one) ~ " << double(cnt_only_one)/N << "\n";
    cout << " P(first before second | both) ~ " << (double(cnt_both_first_before)/max(1,cnt_both)) << "\n";

    return 0;
}
