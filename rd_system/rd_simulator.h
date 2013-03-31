#include <cmath>

class rdmap{
private:
	int mapsize;
	float f, k;
	float ru, rv;
	float **u, **u_next;
	float **v, **v_next;
public:
	rdmap(int _mapsize) : mapsize(_mapsize), f(0.029), k(0.056), ru(0.09), rv(0.056){
		u = static_cast<float**>(malloc(sizeof(float*)*mapsize));
		v = static_cast<float**>(malloc(sizeof(float*)*mapsize));
		u[0] = static_cast<float*>(malloc(sizeof(float)*mapsize*mapsize));
		v[0] = static_cast<float*>(malloc(sizeof(float)*mapsize*mapsize));
		u_next = static_cast<float**>(malloc(sizeof(float*)*mapsize));
		v_next = static_cast<float**>(malloc(sizeof(float*)*mapsize));
		u_next[0] = static_cast<float*>(malloc(sizeof(float)*mapsize*mapsize));
		v_next[0] = static_cast<float*>(malloc(sizeof(float)*mapsize*mapsize));
		for(int i = 1; i < mapsize; i++){
			u[i] = u[i-1] + mapsize;
			v[i] = v[i-1] + mapsize;
			u_next[i] = u_next[i-1] + mapsize;
			v_next[i] = v_next[i-1] + mapsize;
		}
		map_reset();
	}
	~rdmap(){
		delete u[0];
		delete v[0];
		delete u;
		delete v;

		delete u_next[0];
		delete v_next[0];
		delete u_next;
		delete v_next;

	}

	void map_reset(){
		for(int i = 0; i < mapsize; i++){
			for(int j = 0; j < mapsize; j++){
				u[i][j] = 0.0;
				v[i][j] = 0.0;
				u_next[i][j] = 0.0;
				v_next[i][j] = 0.0;
			}
		}

		for(int i = 1; i < mapsize-1; i++){
			for(int j = 1; j < mapsize-1; j++){
				u[i][j] = 0.9;
			}
		}
		const float init_param = 0.3;
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
				u_next[i][j] = static_cast<float>(u[i][j] + 2.0 * (ru * ((u[i][j-1] - 2*u[i][j] + u[i][j+1]) + (u[i-1][j] - 2*u[i][j] + u[i+1][j])) - u[i][j]*v[i][j]*v[i][j] + f*(1-u[i][j])));
				v_next[i][j] = static_cast<float>(v[i][j] + 2.0 * (rv * ((v[i][j-1] - 2*v[i][j] + v[i][j+1]) + (v[i-1][j] - 2*v[i][j] + v[i+1][j])) + u[i][j]*v[i][j]*v[i][j] - (f+k)*v[i][j]));
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

		float** tmp;
		tmp = u;
		u = u_next;
		u_next = tmp;
		tmp = v;
		v = v_next;
		v_next = tmp;
	}

	void set_f(float _f){f = _f;}
	void set_k(float _k){k = _k;}
	void set_ru(float _ru){ru = _ru;}
	void set_rv(float _rv){rv = _rv;}
	float get_f(){return f;}
	float get_k(){return k;}
	float get_ru(){return ru;}
	float get_rv(){return rv;}
	float** get_v(){
		return v;
	}
};