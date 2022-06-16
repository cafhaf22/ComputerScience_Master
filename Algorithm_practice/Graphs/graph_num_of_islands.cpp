/*
Problem:
Given an m x n 2D binary grid grid which represents a map of '1's (land) and '0's (water),
return the number of islands.

An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. 
You may assume all four edges of the grid are all surrounded by water.

Constraints:

m == grid.length
n == grid[i].length
1 <= m, n <= 300
grid[i][j] is '0' or '1'.

Example 1:

Input: grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
Output: 1

Example 2:

Input: grid = [
  ["1","1","0","0","0"],
  ["1","1","0","0","0"],
  ["0","0","1","0","0"],
  ["0","0","0","1","1"]
]
Output: 3

*/

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>
#include <deque>

using namespace std;

void bfs(int r, int c, set<pair<int,int>>& visited_nodes, deque<pair<int,int>>& q, const vector<vector<char>>& grid) {
    //breath first search es iterativo
    q.push_back(pair<int,int>(r,c));
    visited_nodes.insert(pair<int,int>(r,c));
    while (!q.empty()) {
        //mientras la queue no este vacia vammos a seguir expandiendo nuestra lisa
        pair<int,int> row_col = q.front();
        q.pop_front();
        vector<vector<int>> directions = {{1,0},{-1,0},{0,1},{0,-1}};
        
        for(int di = 0; di < directions.size() ; di++) {
            r = row_col.first + directions[di][0];
            c = row_col.second + directions[di][1];
            auto it = visited_nodes.find(pair<int, int>(r,c));
            if( r < grid.size()     &&
                c < grid[0].size()  &&
                grid[r][c] == '1'   &&
                it == visited_nodes.end() ) {
                    q.push_back(make_pair(r,c));
                    visited_nodes.insert(make_pair(r,c));
            }
        }

    }
}

int numIslands(vector<vector<char>>& grid) {
    //Este probema seria un algoritmo de grafos 
    //BFS - Breath first search
    //Tenemos que visitar cada celda y verificar los ndos adyacentes para ver si son 1 o 0
    //Por cada que visitemos lo marcamos como visitado
    //
    if(grid.size() == 0) {
        return 0;
    }
    int rows = grid.size(); 
    int cols = grid[0].size();
    //set<
    int num_of_islands = 0;
    set<pair<int,int>> visited_nodes;
    deque<pair<int,int>> q;
    
    for(int i = 0; i < rows ; i++) {
        for (int j = 0; j < cols ; j++) {
            //Si encontramos un 1 tenemos que marcar ese nodo como visitado
            auto it = visited_nodes.find(pair<int,int>(i,j));
            if(grid[i][j] == '1' && it == visited_nodes.end() ) {
                //ahora tenemos que recorres nuestro traversal
                //eso seria bfs
                bfs(i,j,visited_nodes,q,grid);
                num_of_islands++;
            }
        }
    }
    return num_of_islands;
}

int main () {
    /*set<pair<int,int>> my_set;
    my_set.insert(make_pair(1,1));
    my_set.insert(make_pair(1,2));
    for(auto it = my_set.begin(); it != my_set.end(); it++){
        cout << it->first << ", " << it->second << endl;
    }
    auto it = my_set.find(pair<int,int>(1,3)); 
    if(it != my_set.end()) {
        cout << "Pair is in the set" << endl;
    }else if(it == my_set.end()) {
        cout << "Pair is Not in the set" << endl;
    }

    //Queues are FIFO tyoe of containers
    queue<int> my_queue;
    my_queue.push(12);
    my_queue.push(15);
    my_queue.push(20);
    my_queue.push(28);
    my_queue.push(30);

    cout << "Size of queue " << my_queue.size() << endl;
    cout << "Queue first element " << my_queue.front() << endl;
    cout << "Queue last element " << my_queue.back() << endl;
    cout << "Queue Pop" << endl;
    my_queue.pop();
    cout << "Size of queue " << my_queue.size() << endl;
    cout << "Queue first element " << my_queue.front() << endl;
    cout << "Queue last element " << my_queue.back() << endl;*/
    vector<vector<char>> grid = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };

    cout << "Number of islands " << numIslands(grid) << endl;
    
    vector<vector<char>> grid2 = {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };
    cout << "Number of islands " << numIslands(grid2) << endl;

    return 0;
}