// task3_part2.cpp
#include <bits/stdc++.h>
using namespace std;
long double C(int n, int k){
    if (k<0 || k>n) return 0.0L;
    long double r = 1.0L;
    for (int i=1;i<=k;i++){
        r *= (long double)(n - k + i);
        r /= (long double)i;
    }
    return r;
}
unsigned long long falling(int n, int k){
    if (k<0) return 0;
    unsigned long long r=1;
    for (int i=0;i<k;i++) r *= (unsigned long long)(n - i);
    return r;
}

int main(){
    cout.setf(std::ios::fixed);
    cout<<setprecision(9);

    // data
    int R[3] = {11,10,7};
    int G[3] = {9,8,6};
    int B[3] = {5,10,10};
    int T[3];
    for (int i=0;i<3;i++) T[i] = R[i]+G[i]+B[i];

    // precompute per-basket ingredients
    long double pR[3], pG[3], pB[3];
    for (int i=0;i<3;i++){
        pR[i] = (long double)R[i]/(long double)T[i];
        pG[i] = (long double)G[i]/(long double)T[i];
        pB[i] = (long double)B[i]/(long double)T[i];
    }

    // likelihoods for scenarios A,B,C,D
    long double P_1[3], P_2[3], P_3[3], P_4[3];

    // A: multinomial coefficient 30 * pR^2 pG^2 pB
    for (int i=0;i<3;i++){
        P_1[i] = 30.0L * pow(pR[i],2) * pow(pG[i],2) * pow(pB[i],1);
    }

    // B: multivariate hypergeometric
    for (int i=0;i<3;i++){
        long double num = C(R[i],2) * C(G[i],2) * C(B[i],1);
        long double den = C(T[i],5);
        P_2[i] = num / den;
    }

    // C: 6 * pR^2 pG^2 pB
    for (int i=0;i<3;i++){
        P_3[i] = 6.0L * pow(pR[i],2) * pow(pG[i],2) * pow(pB[i],1);
    }

    // D: 6 * (R)_2 (G)_2 (B)_1 / (T)_5
    for (int i=0;i<3;i++){
        unsigned long long num = 6ULL * falling(R[i],2) * falling(G[i],2) * falling(B[i],1);
        unsigned long long den = falling(T[i],5);
        P_4[i] = (long double)num / (long double)den;
    }

    auto compute_post_and_next_red = [&](const long double L[3], bool with_replacement){
        long double S = L[0]+L[1]+L[2];
        long double post[3];
        for (int i=0;i<3;i++) post[i] = (S>0.0L) ? L[i]/S : 0.0L;
        long double next_red = 0.0L;
        for (int i=0;i<3;i++){
            long double pr;
            if (with_replacement){
                pr = pR[i]; // independent
            } else {
                pr = (long double)(R[i]-2) / (long double)(T[i]-5); // removed 2R,2G,1B already
            }
            next_red += post[i] * pr;
        }
        cout << "Posteriors: ["<<post[0]<<", "<<post[1]<<", "<<post[2]<<"]\n";
        cout << "P(next is red | this scenario) = "<< next_red << "\n\n";
    };

    cout << "*** 1): with replacement, unordered (multinomial) ***\n";
    for(int i=0;i<3;i++) cout << "P1 Basket["<<i+1<<"] = "<< P_1[i] << "\n";
    compute_post_and_next_red(P_1, true);

    cout << "*** 2): without replacement, unordered (multivariate hypergeometric) ***\n";
    for(int i=0;i<3;i++) cout << "P2 Basket ["<<i+1<<"] = "<< P_2[i] << "\n";
    compute_post_and_next_red(P_2, false);

    cout << "*** 3): with replacement, contiguous blocks ***\n";
    for(int i=0;i<3;i++) cout << "P3 Basket ["<<i+1<<"] = "<< P_3[i] << "\n";
    compute_post_and_next_red(P_3, true);

    cout << "*** 4): without replacement, contiguous blocks ***\n";
    for(int i=0;i<3;i++) cout << "P4 Basket ["<<i+1<<"] = "<< P_4[i] << "\n";
    compute_post_and_next_red(P_4, false);

    return 0;
}
