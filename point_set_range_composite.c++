//https://old.yosupo.jp/problem/point_set_range_composite
// coded by zeffy
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zeroupper") //Enable AVX
#pragma GCC target("avx","popcnt")  //Enable AVX
#include <x86intrin.h> //SSE Extensions
#include <bits/stdc++.h> 
using namespace std;
#define eb emplace_back
#define mp make_pair
#define hello cout<<"hello"<<"\n"
#define forr(i,a,b) for(int i=a;i<b;i++)
#define it(s)	for(auto itr:s)
#define dvg(s) 	for(auto itr:s)	cout<<itr<<" ";cout<<endl;
#define dbg(s)	cout<<#s<<"= "<<s<<endl;
typedef long long int lli;
typedef unsigned long long int ulli;
const lli INF=(lli)1e17+5;
const ulli MOD=998244353;
int n,q;
struct node
{
	lli a,b;
};
node ans;
bool flag=false;
vector<node> seg;
vector<node> v;

void combine(node &x,node &y,node &z)
{
	x.a=y.a*z.a;x.a=x.a%MOD;
	x.b=y.b*z.a+z.b;x.b=x.b%MOD;
}

void comb(node &x,node &y)
{
	x.a*=y.a;
	x.b=x.b*y.a+y.b;
	x.a=x.a%MOD;
	x.b=x.b%MOD;
}

void build(int l,int r,int pos)
{
	if(l==r)
	{
		seg[pos]=v[l];
		return;
	}
	int mid=l+((r-l)>>1);
	build(l,mid,pos<<1);
	build(mid+1,r,pos<<1|1);
	combine(seg[pos],seg[pos<<1],seg[pos<<1|1]);
}

void update(int l,int r,int pos,int qpos,node k)
{
	if(l==r)
	{
		seg[pos]=k;
		return;
	}
	int mid=l+((r-l)>>1);
	if(mid<qpos)	update(mid+1,r,pos<<1|1,qpos,k);
	else 		update(l,mid,pos<<1,qpos,k);
	combine(seg[pos],seg[pos<<1],seg[pos<<1|1]);
}

lli evaluate(node c,lli x)
{
	lli k=c.a*x+c.b;
	return k%MOD;
}


void  query(int l,int r,int pos,int ql,int qr)
{
	if(l>qr || r<ql) return;
	if(l>=ql && r<=qr)
	{
		if(!flag) {flag=true;ans=seg[pos];return;}
		if(flag) comb(ans,seg[pos]);
		return;
	}
	int mid=l+((r-l)>>1);
	query(l,mid,pos<<1,ql,qr);
	query(mid+1,r,pos<<1|1,ql,qr);
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    std::cout<< std::fixed;
    std::cout.precision(6);
    
	cin>>n>>q;
	seg=vector<node> (4*n);
	v=vector<node> (n+1);
	forr(i,0,n)
	{
		int a,b;cin>>a>>b;v[i].a=a;v[i].b=b;
	}
	build(0,n-1,1);
	while(q--)
	{
		int ch;
		cin>>ch;
		if(ch)
		{
			int l,r;lli x;
			cin>>l>>r>>x;
			r--;
			flag=false;
			ans.a=1;ans.b=1;
			query(0,n-1,1,l,r);
			lli k=0;
			k=ans.a*x+ans.b;
			cout<<k%MOD<<"\n";
		}
		else
		{
			int pos;lli a,b;
			cin>>pos>>a>>b;
			node k;k.a=a;k.b=b;
			update(0,n-1,1,pos,k);
		}
	}



	 
		
	
	
    
    
    

    return 0;
}
