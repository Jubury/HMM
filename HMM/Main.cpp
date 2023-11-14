#include<iomanip>
#include<iostream>

using namespace std;
const float OBSTACLETRUE = .9;
const float OBSTACLEFALSE = .1;
const float OPENTRUE = .95;
const float OPENFALSE = .05;
const float STRAIGHT = .75;
const float RIGHT = .10;
const float LEFT = .15;

class Point {

public:
	int x;
	int y;
	float probability;
	bool obstacle;
	Point() {
		this->x = 0;
		this->y = 0;
		this->probability = 0;
		this->obstacle = false;
	}
	Point(int x, int y, float probability, bool obstacle) {
		this->x = x;
		this->y = y;
		this->probability = probability;
		this->obstacle = obstacle;
	}

};

class Plane {

public:
	Point Map[6][7];
	void createMap() {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				Map[i][j] = Point(i, j, 5.0, false);
			}
		}
		createObstacles();
	}

	void createObstacles() {
		Map[0][2] = Point(0, 2, 0, true);
		Map[0][3] = Point(0, 3, 0, true);
		Map[0][4] = Point(0, 4, 0, true);
		Map[0][5] = Point(0, 5, 0, true);
		Map[1][3] = Point(1, 3, 0, true);
		Map[1][4] = Point(1, 4, 0, true);
		Map[1][5] = Point(1, 5, 0, true);
		Map[1][6] = Point(1, 6, 0, true);
		Map[2][0] = Point(2, 0, 0, true);
		Map[2][5] = Point(2, 5, 0, true);
		Map[2][6] = Point(2, 6, 0, true);
		Map[3][0] = Point(3, 0, 0, true);
		Map[3][1] = Point(3, 1, 0, true);
		Map[3][5] = Point(3, 5, 0, true);
		Map[3][6] = Point(3, 6, 0, true);
		Map[4][0] = Point(4, 0, 0, true);
		Map[4][1] = Point(4, 1, 0, true);
		Map[4][2] = Point(4, 2, 0, true);
		Map[4][6] = Point(4, 5, 0, true);
		Map[5][1] = Point(5, 1, 0, true);
		Map[5][2] = Point(5, 2, 0, true);
		Map[5][3] = Point(5, 3, 0, true);
	}

	float findPercentage(int i, int j, bool sensor) {
		bool outOfBounds = false;
		//checking if out of bounds
		if (j < 0 || j >= 7 || i < 0 || i >= 6) {
			bool outOfBounds = true;
			if (outOfBounds && sensor) {
				return OBSTACLETRUE;
			}
			else if (outOfBounds && !sensor) {
				return OBSTACLEFALSE;
			}
		}
		//continue if in bounds
		if ((sensor) && (Map[i][j].obstacle == true)) {
			return OBSTACLETRUE;
		}
		if ((sensor) && (Map[i][j].obstacle == false)) {
			return OBSTACLEFALSE;
		}
		if ((!sensor) && (Map[i][j].obstacle == true)) {
			return OPENFALSE;
		}
		if ((!sensor) && (Map[i][j].obstacle == false)) {
			return OPENTRUE;
		}
		else {
			return 0;
		}
	}

	void sensor(bool w, bool n, bool e, bool s) {
		float CURRPROB, NEWPROB, NORMAL = 0.0;
		float W = 0.0;
		float N = 0.0;
		float E = 0.0;
		float S = 0.0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (Map[i][j].obstacle == false) {
					//Calculate for West
					W = findPercentage(i, j - 1, w);
					//Calculate for North
					N = findPercentage(i - 1, j, n);
					//Calculate for East
					E = findPercentage(i, j + 1, e);
					//Calculate for South
					S = findPercentage(i + 1, j, s);
					//Calculating Probabilty
					CURRPROB = Map[i][j].probability;
					NEWPROB = W * N * E * S * CURRPROB;
					Map[i][j] = Point(i, j, NEWPROB, false);
				}
			}
		}

		//Finding normalizer
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (Map[i][j].obstacle == false) {
					CURRPROB = Map[i][j].probability;
					NORMAL = NORMAL + CURRPROB;
				}
			}
		}

		//Looping through map and updating probability values using normalizer
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (Map[i][j].obstacle == false) {
					CURRPROB = Map[i][j].probability;
					NEWPROB = (CURRPROB / NORMAL);
					Map[i][j] = Point(i, j, NEWPROB, false);
				}
			}
		}
	}

	void move(char x) {
		float totalProb = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (Map[i][j].obstacle == false) {
					//if moving north
					if (x == 'N') {
						//if left block is in bounds and a open square
						if ((j - 1 >= 0 && j - 1 < 7) && (Map[i][j - 1].obstacle == false)) {
							totalProb += Map[i][j - 1].probability * RIGHT;
						}
						//if it goes straight and bounces
						if ((i - 1 < 0) || (Map[i - 1][j].obstacle == true)) {
							totalProb += Map[i][j].probability * STRAIGHT;
						}
						//if right square is in bounds and a open square
						if ((j + 1 >= 0 && j + 1 < 7) && (Map[i][j + 1].obstacle == false)) {
							totalProb += Map[i][j + 1].probability * LEFT;
						}
						//if south square is in bounds and a open square
						if ((i + 1 >= 0 && i + 1 < 6) && (Map[i + 1][j].obstacle == false)) {
							totalProb += Map[i + 1][j].probability * STRAIGHT;
						}
						Map[i][j] = Point(i, j, totalProb, false);
						totalProb = 0;
					}
					if (x == 'W') {
						//if right square is in bounds and a open square
						if ((j + 1 >= 0 && j + 1 < 7) && (Map[i][j + 1].obstacle == false)) {
							totalProb += Map[i][j + 1].probability * STRAIGHT;
						}
						//if it drifts left from top sqaure
						if (((i - 1 >= 0) && (i - 1 < 6)) && (Map[i - 1][j].obstacle == false)) {
							totalProb += Map[i - 1][j].probability * LEFT;
						}
						//if it drifts right from bottom square
						if ((i + 1 >= 0 && i + 1 < 6) && (Map[i + 1][j].obstacle == false)) {
							totalProb += Map[i + 1][j].probability * RIGHT;
						}
						//if left square is out of bounds or an obstacle
						if ((j - 1 < 0) || (Map[i][j - 1].obstacle == true)) {
							totalProb += Map[i][j].probability * STRAIGHT;
						}
						Map[i][j] = Point(i, j, totalProb, false);
						totalProb = 0;
					}
					if (x == 'S') {
						//if right square is in bounds and a open square
						if ((j + 1 >= 0 && j + 1 < 7) && (Map[i][j + 1].obstacle == false)) {
							totalProb += Map[i][j + 1].probability * RIGHT;
						}
						//if left block is in bounds and a open square
						if ((j - 1 >= 0 && j - 1 < 7) && (Map[i][j - 1].obstacle == false)) {
							totalProb += Map[i][j - 1].probability * LEFT;
						}
						//if top square is in bounds and a open square
						if (((i - 1 >= 0) && (i - 1 < 6)) && (Map[i - 1][j].obstacle == false)) {
							totalProb += Map[i - 1][j].probability * STRAIGHT;
						}
						//if bottom square is out of bounds or an obstacle
						if ((i + 1 > 6) || (Map[i + 1][j].obstacle == true)) {
							totalProb += Map[i][j].probability * STRAIGHT;
						}
						Map[i][j] = Point(i, j, totalProb, false);
						totalProb = 0;
					}
				}
			}
		}
	}

	void displayMap() {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (Map[i][j].obstacle) {
					cout << "#### ";
				}
				else {
					cout << setprecision(2) << fixed << Map[i][j].probability * 100 << " ";
				}
			}
			cout << endl;
		}
		cout << endl << endl;
	}
};

int main() {

	Plane plane;
	plane.createMap();
	plane.displayMap();
	plane.sensor(0, 0, 0, 0);
	plane.displayMap();
	plane.move('N');
	plane.displayMap();
	plane.sensor(0, 0, 1, 0);
	plane.displayMap();
	plane.move('N');
	plane.displayMap();
	plane.sensor(0, 1, 1, 0);
	plane.displayMap();
	plane.move('W');
	plane.displayMap();
	plane.sensor(0, 1, 0, 0);
	plane.displayMap();
	plane.move('S');
	plane.displayMap();
	plane.sensor(0, 0, 0, 0);
	plane.displayMap();
	return 0;

}