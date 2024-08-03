#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <set>
#include <vector>
#include <map>

#include "glutils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace flf{

	int _cell_n;
	int _seed_n;

	glu::shaderData sd;
	std::map<int, std::pair<int,int>> seeds;
	std::queue<std::tuple<int,int,int,int>> q;

	void init(int cell_n = 200, int seed_n = 255){
		_cell_n = cell_n;
		_seed_n = seed_n;

		sd.cell_count = cell_n;

		for(int i = 0; i < sd.cell_count; i++){
			for(int j = 0; j < sd.cell_count; j++){
				int idx = i * sd.cell_count + j;
				sd.state[idx] = (GLubyte)(0);
			}
		}

		for(int it = 1; it <= seed_n; it++){
			int i = rand() % sd.cell_count;
			int j = rand() % sd.cell_count;

			seeds[it] = {i, j};

			int idx = i * sd.cell_count + j;
			sd.state[idx] = (GLubyte)it;
			q.push({i,j,(sd.cell_count+1)/2, (GLubyte)it});
		}
		seeds[0] = {sd.cell_count*3, sd.cell_count*3};
	}

	void step(bool debug = 0){

		static int step_size_cur = 1000000000;

		if(q.empty()){
			return;
		}

		static int dir[8][2] = {
			{-1, -1},
			{-1, 0},
			{-1, 1},
			{1, -1},
			{1, 0},
			{1, 1},
			{0, -1},
			{0, 1}
		};

		int i, j, step, try_col;
		std::tie(i, j, step, try_col) = q.front();
		q.pop();

		int cur_idx = i * sd.cell_count + j;

		if(step < step_size_cur && debug){
			step_size_cur = step;
			std::cout << "current step size: " << step_size_cur << std::endl;
			std::cout << "queue size: " << q.size() << std::endl;
		}

		if(sd.state[cur_idx] != try_col) return;

		for(int d = 0; d < 8; d++){
			int iof = dir[d][0];
			int jof = dir[d][1];
			int ni = i + iof * step, nj = j + jof * step;
			
			if(ni < 0 || ni >= sd.cell_count || nj < 0 || nj >= sd.cell_count){
				continue;
			}

			double next_dist, my_dist;
			int next_idx = ni * sd.cell_count + nj;
			int next_col = sd.state[next_idx];

			next_dist = sqrt((seeds[next_col].first - ni)*(seeds[next_col].first - ni) + (seeds[next_col].second - nj)*(seeds[next_col].second - nj));
			my_dist = sqrt((seeds[try_col].first - ni)*(seeds[try_col].first - ni) + (seeds[try_col].second - nj)*(seeds[try_col].second - nj));

			if(try_col != sd.state[next_idx] && next_dist > my_dist){
				sd.state[next_idx] = try_col;
				q.push({ni, nj, (step+1)/2, try_col});
			}
		}
	}

}