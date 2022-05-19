#include<bits/stdc++.h>
#include<conio.h>
#define Inf 999999
#define NInf -999999
using namespace std;

string player_color;
string enemy_color;
int p_cnt=0, o_cnt=0;


int minVal(string color, string grid[8][8], int depth, int breadth, int alpha, int beta);
int maxVal(string color, string grid[8][8], int depth, int breadth, int alpha, int beta);


void changeMap(string grid[8][8], int x, int y, string color)
{
    if(p_cnt <= 0 || o_cnt <= 0) return;
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
                if(grid[x-1][y] != "No")
                {
                    if(grid[x-1][y][0] != color[0])
                    {
                        if(color == player_color) o_cnt--;
                        else p_cnt--;
                    }
                    grid[x-1][y][0] = color[0];
                }
                changeMap(grid, x-1, y, color);
            }
            if(x<7) {
                if(grid[x+1][y] != "No")
                {
                    if(grid[x+1][y][0] != color[0])
                    {
                        if(color == player_color) o_cnt--;
                        else p_cnt--;
                    }
                    grid[x+1][y][0] = color[0];
                }
                changeMap(grid, x+1, y, color);
            }
            if(y>0) {
                if(grid[x][y-1] != "No")
                {
                    if(grid[x][y-1][0] != color[0])
                    {
                        if(color == player_color) o_cnt--;
                        else p_cnt--;
                    }
                    grid[x][y-1][0] = color[0];
                }
                changeMap(grid, x, y-1, color);
            }
            if(y<7) {
                if(grid[x][y+1] != "No")
                {
                    if(grid[x][y+1][0] != color[0])
                    {
                        if(color == player_color) o_cnt--;
                        else p_cnt--;
                    }
                    grid[x][y+1][0] = color[0];
                }
                changeMap(grid, x, y+1, color);
            }
        } else {
            grid[x][y][1] = grid[x][y][1] + 1;
        }
    } else {
        return;
    }
}


bool is_critical(string grid[8][8], int x, int y, string color)
{
    if(grid[x][y][0]==color[0])
    {
        if(grid[x][y][1] == '3') return true;
        if((x==0 || x==7 || y==0 || y==7) && grid[x][y][1] == '2') return true;
        if(((x==0 && y==0) || (x==0 && y==7) || (x==7 && y==0) || (x==7 && y==7)) && grid[x][y][1] == '1') return true;
    }
    return false;
}


int how_many_tobe_critical(string grid[8][8], int x, int y, string color)
{
    if(grid[x][y] == "No") {
        int count = 3;
        if(x==0) count--;
        if(x==7) count--;
        if(y==0) count--;
        if(y==7) count--;
        return count;
    }
    else if(grid[x][y][0]==color[0])
    {
        int count = 3;
        int a = (int)grid[x][y][1] - 48;
        if(x==0) count--;
        if(x==7) count--;
        if(y==0) count--;
        if(y==7) count--;
        return count-a;
    }
    return Inf;
}


int how_many_blast(string grid[8][8], int x, int y, string color)
{
    int count = 0;
    if(p_cnt <= 0 || o_cnt <= 0) return 0;
    int blast = 1;
    if(x == 0 || x == 7) blast++;
    if(y == 0 || y == 7) blast++;
    int c = 0;
    if(grid[x][y] == "No") {
        grid[x][y] = color + "1";
        return 0;
    } else if(grid[x][y][0] == color[0]){
        c = (int)grid[x][y][1] - 48;
        blast =  c + blast;
        if(blast == 4) {
            grid[x][y] = "No";
            if(x>0) {
                if(grid[x-1][y] != "No")
                {
                    if(grid[x-1][y][0] != color[0])
                    {
                        if(color == player_color)
                        {
                            o_cnt--;
                            count += (4-how_many_tobe_critical(grid, x-1, y, enemy_color));
                        }
                        else
                        {
                            p_cnt--;
                            count += (4-how_many_tobe_critical(grid, x-1, y, enemy_color));
                        }
                        count += 12;
                    }
                    grid[x-1][y][0] = color[0];
                }
                count += how_many_blast(grid, x-1, y, color);
            }
            if(x<7) {
                if(grid[x+1][y] != "No")
                {
                    if(grid[x+1][y][0] != color[0])
                    {
                        if(color == player_color)
                        {
                            o_cnt--;
                            count += (4-how_many_tobe_critical(grid, x+1, y, enemy_color));
                        }
                        else
                        {
                            p_cnt--;
                            count += (4-how_many_tobe_critical(grid, x+1, y, enemy_color));
                        }
                        count += 12;
                    }
                    grid[x+1][y][0] = color[0];
                }
                count += how_many_blast(grid, x+1, y, color);
            }
            if(y>0) {
                if(grid[x][y-1] != "No")
                {
                    if(grid[x][y-1][0] != color[0])
                    {
                        if(color == player_color)
                        {
                            o_cnt--;
                            count += (4-how_many_tobe_critical(grid, x, y-1, enemy_color));
                        }
                        else
                        {
                            p_cnt--;
                            count += (4-how_many_tobe_critical(grid, x, y-1, enemy_color));
                        }
                        count += 12;
                    }
                    grid[x][y-1][0] = color[0];
                }
                count += how_many_blast(grid, x, y-1, color);
            }
            if(y<7) {
                if(grid[x][y+1] != "No")
                {
                    if(grid[x][y+1][0] != color[0])
                    {
                        if(color == player_color)
                        {
                            o_cnt--;
                            count += (4-how_many_tobe_critical(grid, x, y+1, enemy_color));
                        }
                        else
                        {
                            p_cnt--;
                            count += (4-how_many_tobe_critical(grid, x, y+1, enemy_color));
                        }
                        count += 12;
                    }
                    grid[x][y+1][0] = color[0];
                }
                count += how_many_blast(grid, x, y+1, color);
            }
        } else {
            grid[x][y][1] = grid[x][y][1] + 1;
            return 0;
        }
    } else {
        return 0;
    }
    return count;
}


bool is_Wining(string grid[8][8], int x, int y, string color)
{
    string grid1[8][8];
    p_cnt = 0, o_cnt = 0;
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            grid1[j][k] = grid[j][k];
            if(grid1[j][k][0] == player_color[0]) p_cnt++;
            else if(grid1[j][k][0] == enemy_color[0]) o_cnt++;
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

int hero_max(string grid[8][8], int x, int y, string color)
{
    int value=0;
    bool flag = false, flag2 = false;
    string grid1[8][8];
    p_cnt = 0, o_cnt = 0;
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            grid1[j][k] = grid[j][k];
            if(grid1[j][k][0] == player_color[0]) p_cnt++;
            else if(grid1[j][k][0] == enemy_color[0]) o_cnt++;
        }
    }
    if(is_Wining(grid, x, y, color) && color == player_color) return 200000;
    if(grid[x][y][0] == player_color[0]) value++;
    if(is_critical(grid, x, y, player_color)) {
        value += 2;
        flag2 = true;
    }
    if(flag2)
    {
        value += how_many_blast(grid1,x,y,color);
    }
    else
    {
        if(x>0 && is_critical(grid, x-1, y, enemy_color)) {value -= 5; flag = true;}
        if(x<7 && is_critical(grid, x+1, y, enemy_color)) {value -= 5; flag = true;}
        if(y>0 && is_critical(grid, x, y-1, enemy_color)) {value -= 5; flag = true;}
        if(y<7 && is_critical(grid, x, y+1, enemy_color)) {value -= 5; flag = true;}
        if(flag == false)
        {
            value++;
            if(x==0)
            {
                value += 2;
            }
            else if(x==7)
            {
                value += 2;
            }
            if(y==0)
            {
                value += 2;
            }
            else if(y==7)
            {
                value += 2;
            }
            int v = 4, min_1;
            min_1 = how_many_tobe_critical(grid, x, y, player_color);
            if(x>0 && how_many_tobe_critical(grid, x-1, y, enemy_color)<min_1) {v -= 4;}
            if(x<7 && how_many_tobe_critical(grid, x+1, y, enemy_color)<min_1) {v -= 4;}
            if(y>0 && how_many_tobe_critical(grid, x, y-1, enemy_color)<min_1) {v -= 4;}
            if(y<7 && how_many_tobe_critical(grid, x, y+1, enemy_color)<min_1) {v -= 4;}
            value += v;
        }
    }
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            if(is_critical(grid1, j, k, enemy_color))
            {
                if(j>0 ) {value -= 6*(4-how_many_tobe_critical(grid1,j-1, k, player_color));}
                if(j<7 ) {value -= 6*(4-how_many_tobe_critical(grid1,j+1, k, player_color));}
                if(k>0 ) {value -= 6*(4-how_many_tobe_critical(grid1,j, k-1, player_color));}
                if(k<7 ) {value -= 6*(4-how_many_tobe_critical(grid1,j, k+1, player_color));}
            }
        }
    }

    return value;
}

int hero_min(string grid[8][8], int x, int y, string color)
{
    int value=0;
    bool flag = false, flag2 = false;
    string grid1[8][8];
    p_cnt = 0, o_cnt = 0;
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            grid1[j][k] = grid[j][k];
            if(grid1[j][k][0] == player_color[0]) p_cnt++;
            else if(grid1[j][k][0] == enemy_color[0]) o_cnt++;
        }
    }

    if(is_Wining(grid, x, y, color) && color == enemy_color) return -200000;
    if(grid[x][y][0] == enemy_color[0]) value++;
    if(is_critical(grid, x, y, enemy_color)) {
        value += 2;
        flag2 = true;
    }
    if(flag2)
    {
        value += how_many_blast(grid1,x,y,color);
    }
    else
    {
        if(x>0 && is_critical(grid, x-1, y, player_color)) {value -= 5; flag = true;}
        if(x<7 && is_critical(grid, x+1, y, player_color)) {value -= 5; flag = true;}
        if(y>0 && is_critical(grid, x, y-1, player_color)) {value -= 5; flag = true;}
        if(y<7 && is_critical(grid, x, y+1, player_color)) {value -= 5; flag = true;}
        if(flag == false)
        {
            value++;
            if(x==0)
            {
                value += 2;
            }
            else if(x==7)
            {
                value += 2;
            }
            if(y==0)
            {
                value += 2;
            }
            else if(y==7)
            {
                value += 2;
            }
            int v = 4, min_1;
            min_1 = how_many_tobe_critical(grid, x, y, enemy_color);
            if(x>0 && how_many_tobe_critical(grid, x-1, y, player_color)<min_1) {v -= 4;}
            if(x<7 && how_many_tobe_critical(grid, x+1, y, player_color)<min_1) {v -= 4;}
            if(y>0 && how_many_tobe_critical(grid, x, y-1, player_color)<min_1) {v -= 4;}
            if(y<7 && how_many_tobe_critical(grid, x, y+1, player_color)<min_1) {v -= 4;}
            value += v;
        }
    }
    for(int j=0; j<8; j++)
    {
        for(int k=0; k<8; k++)
        {
            if(is_critical(grid1, j, k, player_color))
            {
                if(j>0 ) {value -= 6*(4-how_many_tobe_critical(grid1,j-1, k, enemy_color));}
                if(j<7 ) {value -= 6*(4-how_many_tobe_critical(grid1,j+1, k, enemy_color));}
                if(k>0 ) {value -= 6*(4-how_many_tobe_critical(grid1,j, k-1, enemy_color));}
                if(k<7 ) {value -= 6*(4-how_many_tobe_critical(grid1,j, k+1, enemy_color));}
            }
        }
    }

    return -value;
}


int maxVal(string color, string grid[8][8], int depth, int breadth, int alpha, int beta)
{
    if(has_won(grid, enemy_color)) return -200000;
    priority_queue<pair<int, pair<int, int> > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                int a = hero_max(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }
    pair<int, pair<int, int> > branch;
    if(depth == 1) {
        branch = que.top();

        return branch.first;
    }
    for(int i=0; i<breadth; i++)
    {
        if(!que.empty())
        {
            branch = que.top();
            que.pop();
            string grid1[8][8];
            p_cnt = 0, o_cnt = 0;
            for(int j=0; j<8; j++)
            {
                for(int k=0; k<8; k++)
                {
                    grid1[j][k] = grid[j][k];
                    if(grid1[j][k][0] == player_color[0]) p_cnt++;
                    else if(grid1[j][k][0] == enemy_color[0]) o_cnt++;
                }
            }
            changeMap(grid1, branch.second.first, branch.second.second, color);
            int a = minVal(enemy_color, grid1, depth-1, breadth, alpha, beta);
            if(a>alpha)
            {
                alpha = a;
                if(alpha>=beta)break;
            }
        }
    }
    return alpha;
}

int minVal(string color, string grid[8][8], int depth, int breadth, int alpha, int beta)
{
    if(has_won(grid, player_color)) return 200000;
    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                int a = hero_min(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }
    pair<int, pair<int, int> > branch;
    if(depth == 1) {
        branch = que.top();
        return branch.first;
    }
    for(int i=0; i<breadth; i++)
    {
        if(!que.empty())
        {
            branch = que.top();
            que.pop();
            string grid1[8][8];
            p_cnt = 0, o_cnt = 0;
            for(int j=0; j<8; j++)
            {
                for(int k=0; k<8; k++)
                {
                    grid1[j][k] = grid[j][k];
                    if(grid1[j][k][0] == player_color[0]) p_cnt++;
                    else if(grid1[j][k][0] == enemy_color[0]) o_cnt++;
                }
            }
            changeMap(grid1, branch.second.first, branch.second.second, color);
            int a = maxVal(player_color, grid1, depth-1, breadth, alpha, beta);
            if(a<beta)
            {
                beta = a;
                if(alpha>=beta) break;
            }
        }
    }
    return beta;
}

pair<int, int> alphaBeta(string color, string grid[8][8], int depth, int breadth)
{
    int alpha = NInf;
    int beta = Inf;
    priority_queue<pair<int, pair<int, int> > > que;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(grid[i][j][0] == color[0] || grid[i][j] == "No")
            {
                int a = hero_max(grid, i, j, color);
                que.push(make_pair(a, make_pair(i, j)));
            }
        }
    }
    pair<int, pair<int, int> > branch;
    int x=-1, y=-1;
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
            int a = minVal(enemy_color, grid1, depth-1, breadth, alpha, beta);
            if(a>alpha) {
                alpha = a;
                if(alpha>=beta) break;
                x = branch.second.first;
                y = branch.second.second;
            }
        }
    }
    return make_pair(x, y);
}


int main(int argc, char* argv[])
{
    player_color = argv[1];
    string grid[8][8];
    if(player_color == "G") enemy_color = "R";
    else enemy_color = "G";
    string command;
    cin >> command;
    cerr << "started" << endl;

    while(command == "start")
    {
        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                cin >> grid[i][j];
            }
        }
        pair<int , int> co_ordinate = alphaBeta(player_color, grid, 8, 5);
        cout << co_ordinate.first << " " << co_ordinate.second << endl;
    }
    return 0;
}


