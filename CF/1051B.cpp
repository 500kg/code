#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main()
{
	ll l, r;
	cin >> l >> r;
	cout << "YES" << endl;
	for (ll i = l; i <= r; i += 2)
	{
		printf("%lld %lld\n", i, i + 1);
	}
}