#include<bits/stdc++.h>
#include<conio.h>
#define Inf 999999
#define NInf -999999
using namespace std;

string player_color;
string enemy_color;


int minVal(string color, string grid[8][8], int depth, int breadth);
int maxVal(string color, string grid[8][8], int depth, int breadth);


void changeMap(string grid[8][8], int x, int y, string color)
{
    int blast = 1;
    if(x == 0 || x == 7) blast++;
    if(y == 0 || y == 7) blast++;
    int c = 0;
    if(grid[x][y] == "No") {
        grid[x][y] = color + "1";
    } else if(grid[x][y][0] == color[0]){
        c = (int)grid[x][y][1] - 48;
        blast =  c + blast;
        if(blast == 4) {
            grid[x][y] = "No";
            if(x>0) {
                if(grid[x-1][y] != "No") grid[x-1][y][0] = color[0];
                changeMap(grid, x-1, y, color);
            }
            if(x<7) {
                if(grid[x+1][y] != "No") grid[x+1][y][0] = color[0];
                changeMap(grid, x+1, y, color);
            }
            if(y>0) {
                if(grid[x][y-1] != "No") grid[x][y-1][0] = color[0];
                changeMap(grid, x, y-1, color);
            }
            if(y<7) {
                if(grid[x][y+1] != "No") grid[x][y+1][0] = color[0];
                changeMap(grid, x, y+1, color);
            }
        } else {
            grid[x][y][1] = grid[x][y][1] + 1;
        }
    } else {
        return;
    }
}

bool is_Wining(string grid[8][8], int x, int y, string color)
{
    string grid1[8][8];
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            grid1[j][k] = grid[j][k];
        }
    }
    changeMap(grid1, x, y, color);
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(grid1[i][j][0] != color[0]) return false;
        }
    }
    return true;
}

bool has_won(string grid[8][8], string color)
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(grid[i][j][0] != color[0]) return false;
        }
    }
    return true;
}

int hero(string grid[8][8], int x, int y, string color)
{
    int value=0;
    bool flag = false;
    if(is_Wining(grid, x, y, color) && color == player_color) value += 10000;
    if(is_Wining(grid, x, y, color) && color == enemy_color) value -= 10000;
    if(grid[x][y][0] == player_color[0]) value++;
    if(x>0 && grid[x-1][y][0]==enemy_color[0] && grid[x-1][y][1] == '3') {value -= 5; flag = true;}
    if(x<7 && grid[x+1][y][0]==enemy_color[0] && grid[x+1][y][1] == '3') {value -= 5; flag = true;}
    if(y>0 && grid[x][y-1][0]==enemy_color[0] && grid[x][y-1][1] == '3') {value -= 5; flag = true;}
    if(y<7 && grid[x][y+1][0]==enemy_color[0] && grid[x][y+1][1] == '3') {value -= 5; flag = true;}
    if(flag == false)
    {
        value++;
        if(x==0)
        {
            value++;
            if(y==0 || y==7) value++;
        }
        else if(x==7)
        {
            value++;
            if(y==0 || y==7) value++;
        }
        if(y==0)
        {
            value++;
        }
        else if(y==7)
        {
            value++;
        }
        if(grid[x][y][1] == '3') {
            value += 2;
        }
        int i=x-1, j=y-1;
        while(i>=0) {
            if(grid[i][y][1] == '3' && grid[i][y][0]==player_color[0]) {
                value += 2*(x-i);
                i--;
            } else {
                break;
            }
        }
        while(j>=0) {
            if(grid[x][j][1] == '3' && grid[x][j][0]==player_color[0]) {
                value += 2*(y-j);
                j--;
            } else {
                break;
            }
        }
        i=x+1, j=y+1;
        while(i<8) {
            if(grid[i][y][1] == '3' && grid[i][y][0]==player_color[0]) {
                value += 2*(i-x);
                i++;
            } else {
                break;
            }
        }
        while(j<8) {
            if(grid[x][j][1] == '3' && grid[x][j][0]==player_color[0]) {
                value += 2*(j-y);
                j++;
            } else {
                break;
            }
        }
    }
    return value;
}

int maxVal(string color, string grid[8][8], int depth, int breadth)
{
    if(has_won(grid, color)) return 10000;
    priority_queue<pair<int, pair<int, int> > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                int a = hero(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }
    pair<int, pair<int, int> > branch;
    if(depth == 1) {
        branch = que.top();
        return branch.first;
    }
    int mx = NInf;
    for(int i=0; i<breadth; i++)
    {
        if(!que.empty())
        {
            branch = que.top();
            que.pop();
            string grid1[8][8];
            for(int j=0; j<8; j++)
            {
                for(int k=0; k<8; k++)
                {
                    grid1[j][k] = grid[j][k];
                }
            }
            changeMap(grid1, branch.second.first, branch.second.second, color);
            int a = minVal(enemy_color, grid1, depth-1, breadth);
            mx = max(mx, a);
        }
    }
    return mx;
}

int minVal(string color, string grid[8][8], int depth, int breadth)
{
    if(has_won(grid, color)) return -10000;
    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                int a = hero(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }
    pair<int, pair<int, int> > branch;
    if(depth == 1) {
        branch = que.top();
        return branch.first;
    }
    int mn = Inf;
    for(int i=0; i<breadth; i++)
    {
        branch = que.top();
        que.pop();
        string grid1[8][8];
        for(int j=0; j<8; j++)
        {
            for(int k=0; k<8; k++)
            {
                grid1[j][k] = grid[j][k];
            }
        }
        changeMap(grid1, branch.second.first, branch.second.second, color);
        int a = maxVal(player_color, grid1, depth-1, breadth);
        mn = min(mn, a);
    }
    return mn;
}

pair<int, int> minMax(string color, string grid[8][8], int depth, int breadth)
{
    //cout << "i am in minmax" << endl;
    //getch();
    priority_queue<pair<int, pair<int, int> > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                //cout << "$" << endl;
                int a = hero(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }

    pair<int, pair<int, int> > branch;
    int mx = NInf, x=-1, y=-1;
    for(int i=0; i<breadth; i++)
    {
        if(!que.empty())
        {
            branch = que.top();
            //cout << branch.first << " " << branch.second.first << " " << branch.second.second << endl;
            que.pop();
            string grid1[8][8];
            for(int j=0; j<8; j++)
            {
                for(int k=0; k<8; k++)
                {
                    grid1[j][k] = grid[j][k];
                }
            }
            changeMap(grid1, branch.second.first, branch.second.second, color);
            int a = minVal(enemy_color, grid1, depth-1, breadth);
            if(a>mx) {
                mx = a;
                x = branch.second.first;
                y = branch.second.second;
            }
        }
    }
    return make_pair(x, y);
}


int main()
{
    string color;
    player_color = "R";
    string grid[8][8];
    if(player_color == "G") enemy_color = "R";
    else enemy_color = "G";

    while(true)
    {
        ifstream iFile;
        iFile.open("shared_file.txt", ios::in);
        iFile >> color;
        if(player_color == color) {
            for(int i=0; i<8; i++) {
                for(int j=0; j<8; j++) {
                    iFile >> grid[i][j];
                }
                cout<<endl;
            }
            pair<int , int> co_ordinate = minMax(player_color, grid, 3, 5);
            ofstream oFile;
            oFile.open("shared_file.txt", ios::out | ios::trunc);
            oFile << 0 << endl;
            oFile << co_ordinate.first << " " << co_ordinate.second << endl;
        }
    }

    return 0;
}
