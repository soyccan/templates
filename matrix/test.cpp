#include <bits/stdc++.h>
#include "matrix.h"
#define int long long
#define MS(s,v) memset(s,v,sizeof(s))
#define FOR(i,start,end) for (int i=start; i<end; ++i)
using namespace std;
using namespace matrix;
SquareMatrix<double> randmat() {
	SquareMatrix<double> m(5);
	FOR(i,0,5) FOR(j,0,5) m[i][j] = rand()%100-50;
	return m;
}
main() {
	Matrix<double> A { {1}, {2} };
	SquareMatrix<double> P { {1,2}, {1,0} };
	SquareMatrix<double> Q { {2,1}, {1,-1} };
	Matrix<double> R = inverse(Q) * P * Q;
	cout<<R<<endl;

	int dp[100];
	dp[1] = 1, dp[2] = 2;
	for (int i=3; i<100; i++)
		cout << i << ' ' << (1LL<<(i-1)) << ' ' << (dp[i] = dp[i-1]+2*dp[i-2]) << endl;

	// FOR(i,0,10) {
	// 	SquareMatrix<double> m = randmat();

	// 	SquareMatrix<double> a(3);
	// 	Matrix<double> b(3,1);
	// 	for (int i=0; i<3; i++)
	// 		for (int j=0; j<3; j++)
	// 			cin>>a[i][j];
	// 	for (int i=0; i<3; i++)
	// 		cin>>b[i][0];
	// 	SquareMatrix<double> a1 = inverse(a);

	// 	cout<<fixed<<setprecision(2);
	// 	cout<<a1*b<<endl;
	// 	cout<<endl;
	// }

#ifdef soytw
    system("pause");
#endif
    return 0;
}
