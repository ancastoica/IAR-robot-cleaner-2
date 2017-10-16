#include "../include/state.hpp"

namespace cleaner{
  state::state(std::vector<bool>const grid, bool base, size battery, size pose): grid(grid), base(base), battery(battery), pose(pose){}

  state::~state(){}

  std::vector<bool>const& state::getGrid() const{
    return grid;
  }

  bool state::getBase() const{
    return base;
  }

  size state::getBattery() const{
    return battery;
  }

  size state::getPose() const{
    return pose;
  }

  int state::nbDirtyCells() const{
    int n = 0;
    for(int i=0; i<grid.size(); i++){
      if(grid[i] == true){
        n += 1;
      }
    }
    return n;
  }

  int state::nearestDirtyCell(int height, int width) const{
    int rows = height, cols = width;
    int index = 0;
    bool** matrix = new bool*[rows];
    for (int i = 0; i < rows; ++i){
      matrix[i] = new bool[cols];
      for (int j = 0; j < cols; ++j){
        matrix[i][j] = grid[index];
        index += 1;
      }
    }



    for (int i = 0; i < rows; ++i)
      delete [] matrix[i];
    delete [] matrix;
  }
}
