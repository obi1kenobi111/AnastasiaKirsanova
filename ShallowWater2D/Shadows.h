#pragma once
#include <vector>;
#include "Vectors.h"


struct Particle {
	vec3r pos;
	float radius;
	float opacity;
};

struct Ray {
	float Intencity;
	vec2r LocalCoord;
};


struct RowCells {
	std::vector<float> OzCoords;
	vec2r coord;
	vector<Ray> rays;
	vector<int> particleIndices; 
};


class ComputeShadows {

public:
	ComputeShadows(int NumParticles, int BoxSize, int  RaysInCell) {

		sbParticles.resize(NumParticles); 
		sbShadows.resize(NumParticles, 0);
		//this->sunDir = sunDir;
		this->BoxSize = BoxSize;
		RaysNum = RaysInCell;
		CreateGridCoordinates();
	};

	
	void csComputeSelfShadowing( );
	mat3r CalculateLight();
	void RotateParticles();
	void CreateGridCoordinates();

	void CalculateSphereCoordinates();

	vec3r sunDir;
	std::vector<Particle> sbParticles;
	std::vector<float> sbShadows;
	int BoxSize;
	int rand_between(const int from, const int to); 


private:
	std::vector<RowCells> CellsRows;
	float RaysNum = 20; // чисто лучей в €чейке
	float cell_size = 3.6; //размер €чейки
};

