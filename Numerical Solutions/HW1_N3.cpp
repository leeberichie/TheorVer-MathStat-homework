// task3_part1.cpp
#include <bits/stdc++.h>
using namespace std;
using u128 = unsigned __int128;
using ull = unsigned long long;

long double comb_ld(int n, int k){
    if (k < 0 || k > n) return 0.0L;
    long double res = 1.0L;
    for (int i=1;i<=k;i++){
        res *= (long double)(n - k + i);
        res /= (long double)i;
    }
    return res;
}

unsigned long long falling(unsigned n, int k){
    if (k < 0) return 0;
    unsigned long long res = 1;
    for (int i=0;i<k;i++){
        res *= (unsigned long long)(n - i);
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // data
    int R[3] = {11,10,7};
    int G[3] = {9,8,6};
    int B[3] = {5,10,10};
    int T[3];
    for(int i=0;i<3;i++) T[i] = R[i] + G[i] + B[i];

    // multinomial coefficient 5!/(2!2!1!) = 30
    const long double Mcoef = 30.0L;

    long double P1_sum = 0.0L; // part1 sum over baskets
    long double P2_sum = 0.0L; // part2
    long double P3_sum = 0.0L; // part3
    long double P4_sum = 0.0L; // part4

    for (int i=0;i<3;i++){
        long double pR = (long double)R[i] / (long double)T[i];
        long double pG = (long double)G[i] / (long double)T[i];
        long double pB = (long double)B[i] / (long double)T[i];

        // Part 1 (with replacement, multinomial)
        long double P1i = Mcoef * pow(pR,2) * pow(pG,2) * pow(pB,1);
        P1_sum += P1i;

        // Part 2 (without replacement, multivariate hypergeometric)
        long double num2 = comb_ld(R[i],2) * comb_ld(G[i],2) * comb_ld(B[i],1);
        long double den2 = comb_ld(T[i],5);
        long double P2i = num2 / den2;
        P2_sum += P2i;

        // Part 3 (with replacement, contiguous blocks) = 6 * pR^2 pG^2 pB
        long double P3i = 6.0L * pow(pR,2) * pow(pG,2) * pow(pB,1);
        P3_sum += P3i;

        // Part 4 (without replacement, contiguous blocks)
        // P4i = 6 * (R)_2 * (G)_2 * (B)_1 / (T)_5
        unsigned long long num4_small = 6ULL * falling(R[i],2) * falling(G[i],2) * falling(B[i],1);
        // compute falling factorial (T)_5 as unsigned long long may hold it (values here are < 1e8)
        unsigned long long den4_small = falling(T[i],5);
        long double P4i = (long double)num4_small / (long double)den4_small;
        P4_sum += P4i;

        cout.setf(std::ios::fixed); cout<<setprecision(5);
        cout << "Basket " << (i+1) << " (T="<<T[i]<<"):\n";
        cout << " P1 (with repl, multinomial) = " << (double)P1i << "\n";
        cout << " P2 (no repl, hypergeo)      = " << (double)P2i << "\n";
        cout << " P3 (with repl, blocks)      = " << (double)P3i << "\n";
        cout << " P4 (no repl, blocks)        = " << (double)P4i << "\n\n";
    }

    long double P1 = P1_sum / 3.0L;
    long double P2 = P2_sum / 3.0L;
    long double P3 = P3_sum / 3.0L;
    long double P4 = P4_sum / 3.0L;

    cout << "======= AVERAGED over 3 baskets =======\n";
    cout << "Part 1 (with repl, unordered) : " << (double)P1 << "\n";
    cout << "Part 2 (no repl, unordered)   : " << (double)P2 << "\n";
    cout << "Part 3 (with repl, blocks)    : " << (double)P3 << "\n";
    cout << "Part 4 (no repl, blocks)      : " << (double)P4 << "\n\n";

    // --- optional Monte Carlo checks (fast) ---
    // (omitted heavy Monte Carlo in printed run; uncomment if you want simulation)
    return 0;
}
