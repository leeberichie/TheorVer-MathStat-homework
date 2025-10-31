// task3_part3.cpp
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

ld fact(int n){
    ld r = 1.0L;
    for(int i=2;i<=n;i++) r *= i;
    return r;
}

ld multinomial_coef(int r,int g,int b){
    return fact(5) / (fact(r)*fact(g)*fact(b));
}

unsigned long long choose_ull(int n,int k){
    if (k<0 || k>n) return 0ULL;
    unsigned long long num = 1;
    for(int i=1;i<=k;i++){
        num = num * (unsigned long long)(n - k + i) / (unsigned long long)i;
    }
    return num;
}

ld comb_ld(int n,int k){
    if (k<0 || k>n) return 0.0L;
    ld r = 1.0L;
    for(int i=1;i<=k;i++){
        r *= (ld)(n - k + i);
        r /= (ld)i;
    }
    return r;
}

int main(){
    cout.setf(std::ios::fixed);
    cout<<setprecision(4);

    // data
    int Rv[3] = {11,10,7};
    int Gv[3] = {9,8,6};
    int Bv[3] = {5,10,10};
    int Tv[3];
    for(int i=0;i<3;i++) Tv[i] = Rv[i] + Gv[i] + Bv[i];

    // precompute probabilities for with-replacement per basket
    ld pR[3], pG[3], pB[3];
    for(int i=0;i<3;i++){
        pR[i] = (ld)Rv[i] / (ld)Tv[i];
        pG[i] = (ld)Gv[i] / (ld)Tv[i];
        pB[i] = (ld)Bv[i] / (ld)Tv[i];
    }

    // enumerate all triples (r,g,b) with r+g+b = 5
    vector<array<int,3>> states;
    for(int r=0;r<=5;r++){
        for(int g=0; g<=5-r; g++){
            int b = 5 - r - g;
            states.push_back({r,g,b});
        }
    }
    // should be 21 states
    // cout << "states: " << states.size() << "\n";

    // accumulators
    ld prob9_wr = 0.0L, prob10_wr = 0.0L, prob11_wr = 0.0L;
    ld prob9_nwr = 0.0L, prob10_nwr = 0.0L, prob11_nwr = 0.0L;

    // precompute per-basket probabilities for each state
    vector<vector<ld>> P_wr(3, vector<ld>(states.size(), 0.0L));
    vector<vector<ld>> P_nwr(3, vector<ld>(states.size(), 0.0L));

    for(int i=0;i<3;i++){
        for(size_t s=0;s<states.size();s++){
            int r = states[s][0], g = states[s][1], b = states[s][2];
            // with replacement: multinomial
            ld coef = multinomial_coef(r,g,b);
            P_wr[i][s] = coef * pow(pR[i], r) * pow(pG[i], g) * pow(pB[i], b);
            // without replacement: multivariate hypergeometric
            ld num = comb_ld(Rv[i], r) * comb_ld(Gv[i], g) * comb_ld(Bv[i], b);
            ld den = comb_ld(Tv[i], 5);
            P_nwr[i][s] = num / den;
        }
    }

    // triple sum over states for 3 baskets
    for(size_t s1=0;s1<states.size();s1++){
        for(size_t s2=0;s2<states.size();s2++){
            for(size_t s3=0;s3<states.size();s3++){
                int total_r = states[s1][0] + states[s2][0] + states[s3][0];
                int total_g = states[s1][1] + states[s2][1] + states[s3][1];
                int total_b = states[s1][2] + states[s2][2] + states[s3][2];

                // with replacement:
                ld prod_wr = P_wr[0][s1] * P_wr[1][s2] * P_wr[2][s3];
                if (total_r == 4) prob9_wr += prod_wr;
                if (total_g <= 4) prob10_wr += prod_wr;
                if (total_b > 1) prob11_wr += prod_wr;

                // without replacement:
                ld prod_nwr = P_nwr[0][s1] * P_nwr[1][s2] * P_nwr[2][s3];
                if (total_r == 4) prob9_nwr += prod_nwr;
                if (total_g <= 4) prob10_nwr += prod_nwr;
                if (total_b > 1) prob11_nwr += prod_nwr;
            }
        }
    }

    cout << "RESULTS (n=5 from EACH basket)\n\n";
    cout << "9) P(R = 4):\n";
    cout << "  with replacement   : " << prob9_wr << "\n";
    cout << "  without replacement: " << prob9_nwr << "\n\n";

    cout << "10) P(G <= 4):\n";
    cout << "  with replacement   : " << prob10_wr << "\n";
    cout << "  without replacement: " << prob10_nwr << "\n\n";

    cout << "11) P(B > 1):\n";
    cout << "  11.1 with replacement   : " << prob11_wr << "\n";
    cout << "  11.2 without replacement: " << prob11_nwr << "\n\n";

    return 0;
}
