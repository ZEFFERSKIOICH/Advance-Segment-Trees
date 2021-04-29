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
const ulli MOD=1e9+7;
#define lson l,mid,pos<<1
#define rson mid+1,r,pos<<1|1
#define midi int mid=l+((r-l)>>1)

int n,q;
lli ans=0;
struct node
{
	lli M1,M2,m1,m2,cM,cm,lazy,sum;
};

vector<lli> v;
vector<node> seg;


void pushup(int pos)
{
	int l=pos<<1,r=l|1;
	seg[pos].sum=seg[l].sum+seg[r].sum;
	
	//maxi
	if(seg[l].M1>seg[r].M1)
	{
		seg[pos].M1=seg[l].M1;
		seg[pos].cM=seg[l].cM;
		seg[pos].M2=max(seg[r].M1,seg[l].M2);
	}
	else if(seg[l].M1<seg[r].M1)
	{
		seg[pos].M1=seg[r].M1;
		seg[pos].cM=seg[r].cM;
		seg[pos].M2=max(seg[l].M1,seg[r].M2);
	}
	else 
	{
		seg[pos].M1=seg[r].M1;
		seg[pos].cM=seg[l].cM+seg[r].cM;
		seg[pos].M2=max(seg[l].M2,seg[r].M2);
	}

	//mini
	if(seg[l].m1<seg[r].m1)
	{
		seg[pos].m1=seg[l].m1;
		seg[pos].cm=seg[l].cm;
		seg[pos].m2=min(seg[r].m1,seg[l].m2);
	}
	else if(seg[l].m1>seg[r].m1)
	{
		seg[pos].m1=seg[r].m1;
		seg[pos].cm=seg[r].cm;
		seg[pos].m2=min(seg[l].m1,seg[r].m2);
	}
	else 
	{
		seg[pos].m1=seg[r].m1;
		seg[pos].cm=seg[l].cm+seg[r].cm;
		seg[pos].m2=min(seg[l].m2,seg[r].m2);
	}
}

void update_max(int pos,lli val)
{
	seg[pos].sum+=(val-seg[pos].M1)*seg[pos].cM;
	if(seg[pos].M1==seg[pos].m1)		{seg[pos].M1=seg[pos].m1=val;seg[pos].M2=-INF,seg[pos].m2=INF;}
	else if(seg[pos].M1==seg[pos].m2)	seg[pos].M1=seg[pos].m2=val;
	seg[pos].M1=val;
	return ;
}

void update_min(int pos,lli val)
{
	seg[pos].sum+=(val-seg[pos].m1)*seg[pos].cm;
	if(seg[pos].m1==seg[pos].M1) 		{seg[pos].M1=seg[pos].m1=val;seg[pos].M2=-INF,seg[pos].m2=INF;}
	else if(seg[pos].m1==seg[pos].M2) seg[pos].m1=seg[pos].M2=val;
	seg[pos].m1=val;
	return ;
}

void update_add(int l,int r,int pos,lli val)
{
	seg[pos].sum+=val*(r-l+1);
	seg[pos].M1+=val;
	if(seg[pos].M2!=-INF) seg[pos].M2+=val;
	seg[pos].m1+=val;
	if(seg[pos].m2!=INF)  seg[pos].m2+=val;
	seg[pos].lazy+=val;
	return ;
}



void pushdown(int l,int r,int pos)
{
	if(l==r)	return;//leadf node;
	int lc=pos<<1,rc=lc|1;
	midi;
	if(seg[pos].lazy!=0)
	{
		update_add(lson,seg[pos].lazy);
		update_add(rson,seg[pos].lazy);
		seg[pos].lazy=0;
	}
	if(seg[pos].M1<seg[lc].M1)	update_max(lc,seg[pos].M1);
	if(seg[pos].M1<seg[rc].M1)	update_max(rc,seg[pos].M1);

	if(seg[pos].m1>seg[lc].m1) 	update_min(lc,seg[pos].m1);
	if(seg[pos].m1>seg[rc].m1)	update_min(rc,seg[pos].m1);
}



void build(int l,int r,int pos)
{
	if(l==r)
	{
		seg[pos].M1=v[l];
		seg[pos].m1=v[l];
		seg[pos].sum=v[l];
		seg[pos].cM=1;
		seg[pos].cm=1;
		return;
	}
	midi;
	build(lson);
	build(rson);
	pushup(pos);
}






void minimise(int l,int r,int pos,int ql,int qr,lli val)
{
	if(seg[pos].M1<=val ||  l>r) return;
	if(l>=ql && r<=qr && seg[pos].M2<val)
	{
		update_max(pos,val);
		return;
	}
	pushdown(l,r,pos);
	midi;
	if(l>qr || r<ql) return;
	minimise(lson,ql,qr,val);
	minimise(rson,ql,qr,val);
	pushup(pos);
}

void maximise(int l,int r,int pos,int ql,int qr,lli val)
{
	if(seg[pos].m1>=val || l>r) return;
	if(l>=ql && r<=qr && seg[pos].m2>val)
	{
		update_min(pos,val);
		return;
	}
	pushdown(l,r,pos);
	midi;
	if(l>qr || r<ql) return;
	maximise(lson,ql,qr,val);
	maximise(rson,ql,qr,val);
	pushup(pos);
}



void add(int l,int r,int pos,int ql,int qr,lli val)
{
	if(l>=ql && r<=qr)
	{
		update_add(l,r,pos,val);
		return ;
	}
	pushdown(l,r,pos);
	if(l>qr || r<ql) return;
	midi;
	add(lson,ql,qr,val);
	add(rson,ql,qr,val);
	pushup(pos);
}


lli query(int l,int r,int pos,int ql,int qr)
{
	if(l>=ql && r<=qr)
	{
		return seg[pos].sum;
	}
	pushdown(l,r,pos);
	if(l>qr || r<ql) return 0;
	midi;
	return query(lson,ql,qr) + query(rson,ql,qr);
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    std::cout<< std::fixed;
    std::cout.precision(6);
    
	cin>>n>>q;
	v=vector<lli> (n,0);
	seg=vector<node> (n*4,{-INF,-INF,INF,INF,0,0,0,0});
	forr(i,0,n) cin>>v[i];
	
	build(0,n-1,1);
	while(q--)
	{
		ans=0;
		int ch,l,r;
		cin>>ch>>l>>r;r--;
		lli x;
		if(ch<3)   cin>>x;

		if(ch==0) {minimise(0,n-1,1,l,r,x);}
		if(ch==1) {maximise(0,n-1,1,l,r,x);}
		if(ch==2) {add(0,n-1,1,l,r,x);}
		if(ch==3) 
		{
			cout<<query(0,n-1,1,l,r)<<"\n";
		}


	}
    

    return 0;
}





















