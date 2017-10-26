#include <cmath>
#include <iostream>
#include "../include/state.hpp"

namespace cleaner {
	state::state(std::vector<bool> const grid, bool base, size battery, size pose) : grid(grid), base(base),
																					 battery(battery), pose(pose) {}

	state::~state() {}

	std::vector<bool> const &state::getGrid() const {
		return grid;
	}

	bool state::getBase() const {
		return base;
	}

	size state::getBattery() const {
		return battery;
	}

	size state::getPose() const {
		return pose;
	}

	int state::nbDirtyCells() const {
		int n = 0;
		for (int i = 0; i < grid.size(); i++) {
			if (grid[i] == true) {
				n += 1;
			}
		}
		return n;
	}

	int state::nearestDirtyCell(int height, int width) const {
		double minDist = height * width;
		double tempDist = 0;
		int indexMin = -1;

		for (int i = 0; i < height * width; i++) {
			if(grid[i] == 1) {
				tempDist = getDistance(i % width, static_cast<int>(floor(i / height)), pose % width, static_cast<int>(floor(pose / height)));
				if(tempDist < minDist){
					minDist = tempDist;
					indexMin = i;
				}
			}
		}
		return indexMin;
	}

	double state::getDistance(int cell_x, int cell_y, int robot_x, int robot_y) const{
		return sqrt(pow(robot_x - cell_x, 2) + pow(robot_y - cell_y, 2));
	}
}

