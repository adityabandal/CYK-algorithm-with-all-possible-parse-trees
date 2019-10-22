#include <bits/stdc++.h>
using namespace std;
#define MAX 100
string CFG[MAX][MAX];
string temp_prod[MAX];
int np;
string cyk[MAX][MAX];
vector<pair<int,pair<int,int>>> right_tree[MAX][MAX];
vector<pair<int,pair<int,int>>> left_tree[MAX][MAX];
void getCFG()
{
    cout<<"CFG =>\n";
    ifstream fin;
    fin.open("input.txt");
    string line;
    vector<string> token;
    int pos_t,p,j;
    np=0;
    while (!fin.eof())
    {
        token.clear();
        getline(fin,line);
        cout<<line<<endl;
        pos_t=line.find("->");
        CFG[np][0]=line.substr(0,pos_t);
        line=line.substr(pos_t+2,line.length());

        p=0;
        while(line.length())
        {
            j=line.find("|");
            if(j>line.length())
            {
                temp_prod[p++] = line;
                line="";
            }
            else
            {
                temp_prod[p++] = line.substr(0,j);
                line=line.substr(j+1,line.length());
            }
        }
        for(int i=0;i<p;i++)
        {
            CFG[np][i+1]=temp_prod[i];
        }
        np++;
    }
//    for(int i=0;i<np;i++)
//    {
//        cout<<CFG[i][0]<<" -> ";
//        j=1;
//        while(!CFG[i][j].empty())
//        {
//            cout<<CFG[i][j]<<" ";
//            j++;
//        }
//        cout<<endl;
//    }
    fin.close();
}
void search_and_add_to_cyk(string& lit,int lefti,int leftj,int righti,int rightj,int lindex,int rindex,int row,int col)
{

    int j,k;
    for(j=0;j<np;j++)
    {
        k=1;
        while (!CFG[j][k].empty())
        {
            if(CFG[j][k]==lit)
            {
                cyk[row][col].append(CFG[j][0]);
                left_tree[row][col].emplace_back(make_pair(lindex,make_pair(lefti,leftj)));
                right_tree[row][col].emplace_back(make_pair(rindex,make_pair(righti,rightj)));
            }
            k++;
        }
    }
}
void printcyk(int n)
{
    cout<<"\nCYK matrix :\n";
    for(int i=n-1;i>=0;i--)
    {
        for(int j=0;j<n-i;j++)
        {
            if(cyk[i][j].empty())
            {
                cout<<"@\t";
            }
            else{
                cout<<cyk[i][j]<<"\t";
            }
        }
        cout<<endl;
    }
}
void printtree(int n)
{
    cout<<"\ntree_left :\n";
    for(int i=n-1;i>=0;i--)
    {
        for(int j=0;j<n-i;j++)
        {
            cout<<left_tree[i][j].size()<<"\t";
        }
        cout<<endl;
    }
    cout<<"tree_right :\n";
    for(int i=n-1;i>=0;i--)
    {
        for(int j=0;j<n-i;j++)
        {
            cout<<right_tree[i][j].size()<<"\t";
        }
        cout<<endl;
    }
}
void print_parse_tree(int row,int col,int index,string input)
{
    queue<pair<int,pair<int,int>>> q;
    pair<int,pair<int,int>> t1,t2,l,r;
    int i,j,ind,number;
    t1=make_pair(index,make_pair(row,col));
    q.push(t1);
    while(!q.empty())
    {
        number=q.size();
        while (number)
        {
            t1=q.front();
            q.pop();
            i=t1.second.first;
            j=t1.second.second;
            ind=t1.first;
            l=left_tree[i][j][ind];
            r=right_tree[i][j][ind];

            if(l.second.first!=-1)
            {
                q.push(l);
                q.push(r);
                cout<<cyk[i][j][ind]<<"->"<<cyk[l.second.first][l.second.second][l.first]<<cyk[r.second.first][r.second.second][r.first]<<" | ";
            }
            else{
                cout<<cyk[i][j][ind]<<"->"<<input[l.first]<<" | ";
            }
            number--;
        }
        cout<<endl;
    }
    cout<<endl;
}
void print_parse_trees(int n,string input)
{
    string str=cyk[n-1][0];
    int len=str.length();
    int i=0,x=1;
    while(i<len)
    {
        if(str[i]=='S')
        {
            cout<<"Parse tree numner "<<x++<<" :\n";
            print_parse_tree(n-1,0,i,input);
        }
        i++;
    }
}
void cyk_algo()
{
    int i,j,k;
    string str;
    cout<<"enter a string: ";
    cin>>str;
    int l=str.length();


    string temp;
    string ch;
    for(i=0;i<l;i++)
    {
        temp="";
        ch="";
        ch+=str[i];
        for(j=0;j<np;j++)
        {
            k=1;
            while (!CFG[j][k].empty())
            {
                if(CFG[j][k]==ch)
                {
                    temp.append(CFG[j][0]);
                    left_tree[0][i].emplace_back(make_pair(i,make_pair(-1,-1)));                //make_pair(,make_pair())
                    right_tree[0][i].emplace_back(make_pair(-1,make_pair(-1,-1)));
                }
                k++;
            }
        }
        cyk[0][i]=temp;
    }
    string a,b;
    int y,z,lefti,leftj,righti,rightj;
    string literal;
    for(i=1;i<l;i++)                                    // loops to build cyk matrix
    {
        for(j=0;j<l-i;j++)
        {
            temp="";                                    // variable that would be stored in cyk[i][j]
            for(k=0;k<i;k++)                            // loop to iterate through various combination of lengths
            {
                lefti=k;
                leftj=j;
                righti=i-1-k;
                rightj=j+k+1;
                a=cyk[k][j];
                b=cyk[i-1-k][j+k+1];
                for(y=0;y<a.length();y++)               // loops to generate combinations of two productions
                {
                    for(z=0;z<b.length();z++)
                    {
                        literal="";                     // combintion to be searched
                        literal=literal+a[y]+b[z];
                        search_and_add_to_cyk(literal,lefti,leftj,righti,rightj,y,z,i,j);
                    }
                }
            }
        }
    }
    printcyk(l);
    //printtree(l);
    print_parse_trees(l,str);
}
int main() {
    std::ofstream out("output.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    getCFG();
    cyk_algo();
    std::cout.rdbuf(coutbuf); //reset to standard output again
    return 0;
}