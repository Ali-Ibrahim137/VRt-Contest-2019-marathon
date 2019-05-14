#include <bits/stdc++.h>
#define ll long long
#define pb push_back
#define endl '\n'
#define Endl '\n'
#define fi first
#define ii pair<ll,ll>
#define se second
#define double long double
#define mod 1000000007
#define popcount __builtin_popcountll
#define PI acos(-1.0)
using namespace std;
int n, tot = 0;
struct location{
    int x, y, d, p, l, h, idx;
    location(){}
    location(int x,int y,int d,int p,int l, int h, int idx):x(x),y(y),d(d),p(p),l(l),h(h),idx(idx){};
};



struct command{
    string type;
    int st, en, pos;
    command(){}
    command(string type, int st, int en ,int pos):
        type(type),
        st(st),
        en(en),
        pos(pos){};
};

vector<command>amid;

void add_command(string type, int st = -1, int en = -1, int pos = -1){
    amid.pb(command(type, st, en, pos));
}
bool mark[2020];
bool skiped[2020];
void print_ans(int sz){
    for(int i=0;i<sz;i++){
        string type = amid[i].type;
        int st = amid[i].st, en = amid[i].en, pos = amid[i].pos;
        if(pos!=-1 && mark[pos]==0)continue;
        if(type=="work"){
            cout<<type<<" "<<amid[i].st<<" "<<amid[i].en<<" "<<amid[i].pos<<endl;
            continue;
        }
        if(type=="end"){
            cout<<type<<endl;
            continue;
        }
        cout<<type<<" "<<amid[i].st<<" "<<amid[i].pos<<endl;
    }
}

location a[2020];
location b[2020];
location sorted[2020][2020];
location tmp;
int rem;
bool comp(location a, location b){
    int d1 = abs(a.x-tmp.x) + abs(a.y-tmp.y);
    int d2 = abs(b.x-tmp.x) + abs(b.y-tmp.y);
    d1 = max(d1, a.l);
    d2 = max(d2, b.l);
    if(d1!=d2)return d1<d2;
    return a.h < b.h;
}

int mx;

int dis(int i, int j){
    if(i==0 || j==0)return 0;
    return abs(a[i].x-a[j].x) + abs(a[i].y-a[j].y);
}

bool vis[111][111];
location get_nxt(int j, int c, int mxp, int mxt, int nxi){
    location ret(-1,-1,0,0,0,0,0);
    int mn = 100000;
    int mx = 0;
    for(int i=1;i<=n;i++){
        if(a[i].p==0)continue;  // done from this place
        if(a[i].p<0){
            exit(15);
        }
        if(i==j)continue;       // move to next place
        if(a[i].p>mxp)continue;
        int x =a[i].x;
        int y =a[i].y;
        if(vis[x][y])continue;
        int d = dis(i, j);
        d+=c;
        d = max(d, a[i].l);
        if(d + dis(i, nxi) +a[i].d > mxt)continue;
        if(d + a[i].d>a[i].h)continue;
        if(mn>d){
            mn = d;
            ret = a[i];
            mx = a[i].p;
            continue;
        }
        if(mn==d){
            if(a[i].p>mx){
                mx = a[i].p;
                ret = a[i];
            }
        }
    }
    return ret;
}
int tim[111][111];

int validade_ans(){
    ll ret = 0, cur=0, cnt = 0, exp  = 0, income=0, lst=0;
    int sz = amid.size();
    for(int i=0;i<sz;i++){
        string str = amid[i].type;
        int st = amid[i].st;
        int en = amid[i].en;
        int pos = amid[i].pos;
        // cout<<str<<" "<<st<<" "<<en<<" "<<pos<<endl;
        if(str=="end"){
            exp+=(lst-cur);
            continue;
        }
        if(str=="start"){
            cnt++;
            exp+=240;
            cur = st;
            lst = pos;
            continue;
        }
        if(str=="arrive"){
            lst = st;
            continue;
        }
        lst = en;
    }
    for(int i=1;i<=n;i++){
        if(a[i].p==0){
            income+=(b[i].d * b[i].p * (b[i].p + 5));
        }
    }
    return income-exp;
    cout<<"used "<<cnt<<" workers"<<endl;
    cout<<"expenses "<<exp<<endl;
    cout<<"work time "<<exp-cnt*240<<endl;
    cout<<"income "<<income<<endl;
    cout<<"net profit "<<income-exp<<endl;
    return income-exp;
}



vector<location> fill_gaps(vector<location>v, int mx){
    vector<location>ret;
    int cur = 0;
    int sz = v.size();
    int lst_t = 0;
    int lst_idx = 0;
    if(sz==2)return ret;
    location pre = a[1];
    bool fst = 1;
    for(int i=1;i<sz-1;i++){
        location nx  = v[i];
        int j = nx.idx;
        nx = a[j];
        location md;
        while(1){
            // move from pre to mid if:
            // (dis (pre ===> mid) + (duration mid) + dis (mid ===> nx)) <= tim[nx]
            int x = nx.x;
            int y = nx.y;
            md = get_nxt(pre.idx, cur, mx, tim[x][y], nx.idx);
            if(md.x!=-1){
                // can fill this gap
                // move to mx
                // cout<<"HELLO"<<endl;
                int idx = md.idx;
                if(a[idx].p==0){
                    exit(15);
                }
                a[idx].p--;
                if(a[idx].p!=0)ret.pb(a[idx]);
                x = md.x;
                y = md.y;
                if(vis[x][y]){
                    exit(15);
                }
                vis[x][y]=1;
                int d = dis(pre.idx, md.idx) + cur;
                d = max(d, a[md.idx].l);
                tim[x][y] = d;
                pre = md;
                if(fst==1){
                    add_command("start", tim[x][y] - dis(1, idx), -1, 1);
                    // cout<<"start "<<tim[x][y] - dis(1, idx)<<" 1"<<endl;
                }
                fst = 0;
                add_command("arrive", tim[x][y] , -1, idx);
                add_command("work", tim[x][y] , tim[x][y]+a[idx].d, idx);
                // cout<<"arrive "<<tim[x][y]<<" "<<idx<<endl;
                // cout<<"work "<<tim[x][y]<<" "<<tim[x][y] + a[idx].d<<" "<<idx<<endl;
                cur = a[idx].d + tim[x][y];
                lst_idx = idx;
                lst_t = cur;
                continue;
            }
            // md cant be placed in the gap
            // move to nx
            x = nx.x;
            y = nx.y;
            pre = nx;
            int idx = nx.idx;
            cur = tim[x][y] + a[idx].d;
            lst_idx = idx;
            lst_t = cur;
            if(a[idx].p==0){
                exit(15);
            }
            a[idx].p--;
            if(fst==1){
                add_command("start", tim[x][y]-dis(1, idx) , -1, 1);
                // cout<<"start "<<tim[x][y] - dis(1, idx)<<" 1"<<endl;
            }
            fst = 0;
            add_command("arrive", tim[x][y] , -1, idx);
            // cout<<"arrive "<<tim[x][y]<<" "<<idx<<endl;
            add_command("work", tim[x][y] , tim[x][y]+a[idx].d, idx);
            // cout<<"work "<<tim[x][y]<<" "<<tim[x][y] + a[idx].d<<" "<<idx<<endl;
            if(a[idx].p!=0)ret.pb(a[idx]);
            break;
        }
    }
    add_command("arrive", lst_t + dis(1, lst_idx) , -1, 1);
    // cout<<"arrive "<<lst_t + dis(1, lst_idx)<<" 1"<<endl;
    add_command("end", -1 , -1, -1);
    // cout<<"end"<<endl;
    return ret;
}


int best =0, sizz = 0;
void go(){
    vector<location>v;
    v.pb(a[1]);
    int cur = 0;
    memset(vis,0,sizeof vis);
    tmp = get_nxt(1, 0, 10, 10000, 0);
    if(tmp.x==-1)return;
    v.pb(tmp);
    int x = tmp.x;
    int y = tmp.y;
    vis[x][y]=1;
    tim[x][y]=tmp.l;
    add_command("start", tmp.l-dis(1, tmp.idx) , -1, 1);
    // cout<<"start "<<tmp.l-dis(1, tmp.idx)<<" 1"<<endl;
    cur = tmp.l;
    mx = max(mx, tmp.p);
    a[tmp.idx].p--;
    while(1){
        add_command("arrive", cur , -1, tmp.idx);
        // cout<<"arrive "<<cur<<" "<<tmp.idx<<endl;
        add_command("work", cur , cur + tmp.d, tmp.idx);
        // cout<<"work "<<cur<<" "<<cur + tmp.d<<" "<<tmp.idx<<endl;
        location t = tmp;
        cur+=tmp.d;
        tmp = get_nxt(tmp.idx, cur, 10, 100000, 0);
        int d = dis(tmp.idx, t.idx) + cur;
        d = max(d, a[tmp.idx].l);
        if(d + a[tmp.idx].d > a[tmp.idx].h){
            add_command("arrive", cur+dis(t.idx, 1) , -1, 1);
            // cout<<"arrive "<<cur+dis(t.idx, 1)<<" 1"<<endl;
            add_command("end", -1 , -1, -1);
            // cout<<"end"<<endl;
            break;
        }
        int idx = tmp.idx;
        // if(a[idx].p!=1)v.pb(tmp);
        v.pb(tmp);
        cur = d;
        mx = max(mx, a[idx].p);
        a[idx].p--;
        int x = tmp.x;
        int y = tmp.y;
        vis[x][y]=1;
        tim[x][y]=cur;
    }
    mx--;
    int zz = validade_ans();
    if(zz>=best){
        best = zz;
        sizz = amid.size();
        memset(mark,0,sizeof mark);
        for(int i=1;i<=n;i++){
            if(a[i].p==0)mark[i]=1;
        }
    }
    v.pb(a[1]);
    int sz = v.size();
    for(int i=6;i>=1;i--){
        // retrace the route
        vector<location>vv;
        bool fst = 1;
        int lst_t = 0;
        int lst_i = 0;
        vv.pb(a[1]);
        sz = v.size();
        for(int j=0;j<sz;j++){
            int idx = v[j].idx;
            if(a[idx].p==0 && a[idx].idx!=0)continue;
            vv.pb(a[idx]);
        }
        int z;
        vv.pb(a[1]);
        vv = fill_gaps(vv, i);
        int zz = validade_ans();
        if(zz>=best){
            best = zz;
            sizz = amid.size();
            memset(mark,0,sizeof mark);
            for(int i=1;i<=n;i++){
                if(a[i].p==0)mark[i]=1;
            }
        }
        v.clear();
        for(int j=0;j<vv.size();j++){
            v.pb(vv[j]);
        }
    }
}


ii ans[2020];


void val(int sz){
    ll ret = 0, cur=0, cnt = 0, exp  = 0, income=0, lst=0;
    // cout<<sz<<endl;
    for(int i=0;i<sz;i++){
        string str = amid[i].type;
        int st = amid[i].st;
        int en = amid[i].en;
        int pos = amid[i].pos;
        if(pos!=-1 && mark[pos]==0)continue;
        // cout<<str<<" "<<st<<" "<<en<<" "<<pos<<endl;
        if(str=="end"){
            exp+=(lst-cur);
            continue;
        }
        if(str=="start"){
            cnt++;
            exp+=240;
            cur = st;
            lst = pos;
            continue;
        }
        if(str=="arrive"){
            lst = st;
            continue;
        }
        lst = en;
    }
    for(int i=1;i<=n;i++){
        if(mark[i]==0)continue;
        if(a[i].p==0){
            income+=(b[i].d * b[i].p * (b[i].p + 5));
        }
    }
    // cout<<"used "<<cnt<<" workers"<<endl;
    // cout<<"expenses "<<exp<<endl;
    // cout<<"work time "<<exp-cnt*240<<endl;
    // cout<<"income "<<income<<endl;
    cout<<"net profit "<<income-exp<<endl;

    return ;
}
void init(){
    amid.clear();
    for(int i=1;i<=n;i++){
        int x = a[i].x;
        int y = a[i].y;
        a[i].p=b[i].p;
        vis[x][y]=!mark[i];
        tim[x][y]=0;
        mark[i]=0;
    }
}
int main(){
    // return 0;
    // freopen("001.txt", "r", stdin);
    // ifstream fin;
    // fin.open("001.txt");
    cin>>n;
    int nn = n;
    for(int i=1;i<=n;i++){
        int x, y, d, p, l, h;
        //fin>>x>>y>>d>>p>>l>>h;
        scanf("%d%d%d%d%d%d", &x, &y, &d, &p, &l, &h);
        a[i]=location(x, y, d, p, l, h, i);
        b[i]=a[i];
    }
    int cnt = 0;
    int q = 35;
    while(q--){
        init();
        int cur_best = best;
        while(1){
            int f = 0;
            for(int i=1;i<=n;i++){
                if(a[i].p!=0){
                    f ++;
                    // break;
                }
            }
            if(f==0)break;
            mx = 0;
            go();
            if(sizz==0)sizz=amid.size();
            int zz = validade_ans();
            if(zz>=best){
                memset(mark,0,sizeof mark);
                best = zz;
                for(int i=1;i<=n;i++){
                    if(a[i].p==0)mark[i]=1;
                }
                sizz = amid.size();
            }
        }
        // cout<<"done "<<q<<endl;
    }
    print_ans(sizz);
    // cout<<best<<endl;
    return 0;
}
