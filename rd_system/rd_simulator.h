#include <cmath>
#include <utility>
#include <memory>

class rdmap{
private:
	int mapsize;
	float f, k;
	float ru, rv;
	std::unique_ptr<double[]> uData, vData, u_nextData, v_nextData;	
	std::unique_ptr<double*[]> u, v, u_next, v_next;
public:
	rdmap(int _mapsize) : mapsize(_mapsize), f(0.03f), k(0.056f), ru(0.09f), rv(0.056f),
		u(new double*[mapsize]), v(new double*[mapsize]), u_next(new double*[mapsize]), v_next(new double*[mapsize]),
		uData(new double[mapsize*mapsize]), vData(new double[mapsize*mapsize]), u_nextData(new double[mapsize*mapsize]), v_nextData(new double[mapsize*mapsize])
	{
		for(int i = 0; i < mapsize; i++){
			u[i] = uData.get()+i*mapsize;
			v[i] = vData.get()+i*mapsize;
			u_next[i] = u_nextData.get()+i*mapsize;
			v_next[i] = v_nextData.get()+i*mapsize;
		}
		map_reset();
	}
	~rdmap(){}

	void map_reset(){
		for(int i = 0; i < mapsize; i++){
			for(int j = 0; j < mapsize; j++){
				u[i][j] = 0.9;
				v[i][j] = 0.0;
				u_next[i][j] = 0.9;
				v_next[i][j] = 0.0;
			}
		}

		const double init_param = 0.3;
		for(int i = 100; i < 140; i++){
			for(int j = 50; j < 80; j++){
				u[i][j] = 1.0;
				v[i][j] = init_param;
			}
		}
	}

	void point_delete(int x, int y, int range){
		int x_min = std::max(x-range, 0);
		int x_max = std::min(x+range, mapsize);
		int y_min = std::max(y-range, 0);
		int y_max = std::min(y+range, mapsize);

		for(int i = x_min; i < x_max; i++){
			for(int j = y_min; j < y_max; j++){
				u[i][j] = 0.0;
				v[i][j] = 0.0;
			}
		}
	}

	void move(){
		for(int i = 1; i < mapsize-1; i++){
			for(int j = 1; j < mapsize-1; j++){
				u_next[i][j] = u[i][j] + 1.0 * (ru * ((u[i][j-1] - 2*u[i][j] + u[i][j+1]) + (u[i-1][j] - 2*u[i][j] + u[i+1][j])) - u[i][j]*v[i][j]*v[i][j] + f*(1-u[i][j]));
				v_next[i][j] = v[i][j] + 1.0 * (rv * ((v[i][j-1] - 2*v[i][j] + v[i][j+1]) + (v[i-1][j] - 2*v[i][j] + v[i+1][j])) + u[i][j]*v[i][j]*v[i][j] - (f+k)*v[i][j]);
			}
		}

		for(int i = 1; i < mapsize-1; i++){
			/*
				u_next[i][0] = u_next[i][mapsize-2];
				u_next[i][mapsize-1] = u_next[i][1];
				u_next[0][i] = u_next[mapsize-2][i];
				u_next[mapsize-1][i] = u_next[1][i];
				v_next[i][0] = v_next[i][mapsize-2];
				v_next[i][mapsize-1] = v_next[i][1];
				v_next[0][i] = v_next[mapsize-2][i];
				v_next[mapsize-1][i] = v_next[1][i];
				*/
				u_next[i][0] = 0;
				u_next[i][mapsize-1] = 0;
				u_next[0][i] = 0;
				u_next[mapsize-1][i] = 0;
				v_next[i][0] = 0;
				v_next[i][mapsize-1] = 0;
				v_next[0][i] = 0;
				v_next[mapsize-1][i] = 0;
		}

		std::swap(u, u_next);
		std::swap(v, v_next);
	}

	void set_f(float _f){f = _f;}
	void set_k(float _k){k = _k;}
	void set_ru(float _ru){ru = _ru;}
	void set_rv(float _rv){rv = _rv;}
	float get_f(){return f;}
	float get_k(){return k;}
	float get_ru(){return ru;}
	float get_rv(){return rv;}
	double** get_v(){
		return v.get();
	}
};