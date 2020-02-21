#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int dr[] = {1,-1,0,0};
int dc[] = {0,0,1,-1};
enum State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};


struct NodesParams {
  int x,y,h,g;
};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    for(int i=0;i<line.size();i++){
      if(line[i] == '0'){
        row.push_back(State::kEmpty);
      }else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}

vector<vector<State> > ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State> > board;
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

// Calculate the manhattan distance 
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

// this is an strucure which implements the operator overlading 
struct Compare { 
    bool operator()(NodesParams const& p1, NodesParams const& p2) 
    { 
        // return "true" if "p1" is ordered  
        // before "p2", for example: 
        return p1.h > p2.h; 
    } 
}; 

/**
 * add nodes availble to the vector and make the grid at this point as closed 
 */
void ExpandNeighbors(priority_queue<NodesParams, vector<NodesParams>, Compare> &nodes,int x,int y,int h,int g,vector<vector<State> > &visited){
  NodesParams N;
  N.x = x,N.y = y,N.h = h, N.g = g;
  nodes.push(N);
  //cout<<N.x<<endl;
  visited[x][y] = State::kObstacle;
}

/**
 * check if node is valid or not 
 */
bool CheckValidCell(int x, int y,vector<vector<State> > &grid,vector<vector<State> > &visited){
  if(x<0 || y < 0 || x >=grid.size() || y>=grid[0].size() || grid[x][y] == State::kObstacle || visited[x][y] == State::kObstacle)return false ;
  else return true ; 
}

/** 
 * Implementation of search algorithm
 */
vector<vector<State> > Search(vector<vector<State> > grid,vector<vector<State> > visited ,int init[2], int goal[2]) {
  priority_queue<NodesParams, vector<NodesParams>, Compare> nodes;
  vector<vector<State> > emptyGrid;
  int x = init[0];
  int y = init[1];
  int h = Heuristic(x,y,goal[0],goal[1]);
  int g = 0;
  ExpandNeighbors(nodes,x,y,h,g,visited);
  while(!nodes.empty()){
    NodesParams front = nodes.top(); nodes.pop();
    cout<<front.x<<" ";
    grid[front.x][front.y] = State::kPath;
    for(int i=0;i<4;i++){
      bool check = CheckValidCell(front.x + dr[i],front.y + dc[i],grid,visited) ; 
      if(check){
        int g = front.g + 1;
        int h = Heuristic(front.x + dr[i],front.y + dc[i], goal[0],goal[1]);

         // Check if we're done, if we reach the target.
         if (front.x + dr[i] == goal[0] && front.y + dc[i] == goal[1]) {
           grid[ init[0]][ init[1] ] = State::kStart;
           grid[ goal[0] ][ goal[1] ] = State::kFinish;
           return grid;
          }
        // if it isn't the target then we still add neighbors to the queue   
        ExpandNeighbors(nodes,front.x + dr[i], front.y + dc[i],h,g,visited);
      }
    }
  }

  cout<<"No Path Found"<<endl;
  return emptyGrid;
}



string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}

void PrintBoard(const vector<vector<State> > board) {
  for (int i = 0; i < (int)board.size(); i++) {
    for (int j = 0; j < (int)board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

int main() {
  
  int init[2] = {0, 0};
  int goal[2] = {4, 5};
  auto board = ReadBoardFile("/Users/khaledelshamy/Desktop/board.text");
  auto visited = board;
  auto solution = Search(board,visited ,init, goal);
  cout<<endl;
  PrintBoard(solution);
  return 0;
}