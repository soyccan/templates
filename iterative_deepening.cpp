// Egyptian fraction
#include <bits/stdc++.h>
#define int long long
#define MS(a,v) memset(a,v,sizeof a)
#define FOR(i,l,r) for (int i=l; i<=r; i++)
#define RFOR(i,l,r) for (int i=l; i>=r; i--)
using namespace std;
int sol[1000000];
int ans[1000000];
void update(int d) {
    bool better=0;
    FOR(i,0,d)
        if (ans[i]==-1 || ans[i]!=sol[i])
            better=1;
    if (better)
        memcpy(ans,sol,sizeof ans);
}
bool bt(int a,int b,int d,int maxd,int st) {
    cout<<"a:"<<a<<" b:"<<b<<endl;
    if (d>=maxd) {
        if (a!=0 && a!=1) return 0;
        sol[d]=b;
        update(d);
        return 1;
    }
    bool ok=0;
    for (int i=st; ; i++) {
        // if even (maxd-d+1) * (1/i) <= a/b, no solution
        if (b*(maxd-d+1) <= a*i) break;

        // a/b - 1/i
        int na=a*i-b;
        int nb=b*i;
        int g=__gcd(na,nb);
        if (na<0) continue;

        sol[d]=i;
        if (bt(na/g,nb/g,d+1,maxd,i+1)) ok=1;
    }
    return ok;
}
main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int a,b;
    cin>>a>>b;
    int g=__gcd(a,b);
    a/=g, b/=g;
    for (int maxd=1; ; maxd++) {
        MS(ans,-1);
        if (bt(a,b,0,maxd,1)) {
            FOR(i,0,maxd) cout<<ans[i]<<' ';
            cout<<"yes\n";
            break;
        }
    }
    return 0;
}
