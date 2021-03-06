
#include<cstdio>
#include<cmath>
#include<vector>
#include<list>
#include<cstring>
#include<algorithm>
using namespace std;
const int N = 50005;
const double pi = acos(-1.0);
typedef long long LL;
struct edge {
	int to, next;
}e[N << 1];
struct Complex {
	double r, i;
	Complex(double r = 0.0, double i = 0.0) :r(r), i(i) {}
	Complex operator + (const Complex &a)const {
		return Complex(r + a.r, i + a.i);
	}
	Complex operator - (const Complex &a)const {
		return Complex(r - a.r, i - a.i);
	}
	Complex operator * (const Complex &a)const {
		return Complex(r * a.r - i * a.i, r * a.i + i * a.r);
	}
	Complex operator / (const double &a)const {
		return Complex(r / a, i / a);
	}
};
int head[N], sz;
int mx[N], SZ[N];
int root, nSZ;
bool done[N];
int cnt[N];
int dis[N], dcnt;
bool nprime[N];
int prime[N], pcnt;
Complex x1[N << 2], tmp[N << 2];
int tn;
LL tt[N << 2], res;
void getprime() {
	pcnt = 0;
	nprime[0] = nprime[1] = 1;
	prime[pcnt++] = 2;
	for (int i = 4; i < N; i += 2) nprime[i] = 1;
	for (int i = 3; i < N; i += 2) {
		if (nprime[i]) continue;
		prime[pcnt++] = i;
		for (int j = i + i; j < N; j += i)
			nprime[j] = 1;
	}
}
int rev(int x) {
	int res = 0;
	for (int i = 0; i < tn; i++) {
		if (x & 1) res += 1 << tn - i - 1;
		x >>= 1;
	}
	return res;
}
void fft(Complex A[], int n, int op) {
	for (int i = 0; i < n; i++) tmp[rev(i)] = A[i];
	for (int i = 0; i < n; i++) A[i] = tmp[i];
	for (int i = 1; (1 << i) <= n; i++) {
		int m = 1 << i;
		Complex wn(cos(op * 2.0 * pi / m), sin(op * 2.0 * pi / m));
		for (int k = 0; k < n; k += m) {
			Complex w(1, 0), u, t;
			for (int j = 0; j < m / 2; j++) {
				u = A[k + j];
				t = w * A[k + j + m / 2];
				A[k + j] = u + t;
				A[k + j + m / 2] = u - t;
				w = w * wn;
			}
		}
	}
	if (op == -1)
		for (int i = 0; i < n; i++) A[i] = A[i] / n;
}
void addedge(int u, int v) {
	e[sz].to = v;
	e[sz].next = head[u];
	head[u] = sz++;
}
void getroot(int u, int f) {
	SZ[u] = 1; mx[u] = 0;
	for (int i = head[u]; i != -1; i = e[i].next) {
		int v = e[i].to;
		if (v == f || done[v]) continue;
		getroot(v, u);
		SZ[u] += SZ[v];
		mx[u] = max(mx[u], SZ[v]);
	}
	mx[u] = max(mx[u], nSZ - SZ[u]);
	if (mx[u] < mx[root]) root = u;
}
void getdis(int u, int f, int c) {
	dis[dcnt++] = c;
	for (int i = head[u]; i != -1; i = e[i].next) {
		int v = e[i].to;
		if (v == f || done[v]) continue;
		getdis(v, u, c + 1);
	}
}
LL calc(int u, int c) {
	dcnt = 0;
	getdis(u, 0, c);
	int Max = -1;
	for (int i = 0; i < dcnt; i++) {
		cnt[dis[i]] ++;
		Max = max(Max, dis[i]);
	}
	LL tres = 0;
	Max++;
	int len = 1; tn = 0;
	while (len < 2 * Max) len <<= 1, tn++;
	for (int i = 0; i < Max; i++) x1[i] = Complex(cnt[i], 0);
	for (int i = Max; i < len; i++) x1[i] = Complex(0, 0);
	fft(x1, len, 1);
	for (int i = 0; i < len; i++) x1[i] = x1[i] * x1[i];
	fft(x1, len, -1);
	int lim = Max * 2;
	for (int i = 0; i < lim; i++) tt[i] = LL(x1[i].r + 0.5);
	for (int i = 0; i < dcnt; i++) tt[dis[i] * 2] --;
	for (int i = 0; i < lim; i++) tt[i] /= 2;
	for (int i = 0; i < pcnt && prime[i] < lim; i++) tres += tt[prime[i]];
	for (int i = 0; i < dcnt; i++) cnt[dis[i]] --;
	return tres;
}
void deal(int u) {
	res += calc(u, 0);
	done[u] = 1;
	for (int i = head[u]; i != -1; i = e[i].next) {
		int v = e[i].to;
		if (done[v]) continue;
		res -= calc(v, 1);
		nSZ = mx[0] = SZ[v];
		getroot(v, root = 0);
		deal(root);
	}
}
void init() {
	memset(head, -1, sizeof(head));
	memset(done, 0, sizeof(done));
	sz = 0;
}
void solve(int n) {
	int u, v;
	init();
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &u, &v);
		addedge(u, v);
		addedge(v, u);
	}
	res = 0;
	mx[0] = nSZ = n;
	getroot(1, root = 0);
	deal(root);
	printf("%.10f\n", res * 2.0 / n / (n - 1));
}
int main()
{
//	freopen("1.in", "r", stdin);
	getprime();
	int n;
	while (~scanf("%d", &n)) {
		solve(n);
	}
	return 0;
}