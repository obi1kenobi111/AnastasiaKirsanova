
#include <iostream>
#include "Shadows.h"
#define PI 3.1415


void ComputeShadows::csComputeSelfShadowing( )
{
	int RaysCounter = 0;
	float Intencity, alpha;
	float SumIntencity = 0;
	//alpha -  упавший поток, 1-alpha - прошедший поток
	for (int i = 0; i < CellsRows.size(); i++) { 
		for (int j = 0; j < CellsRows[i].particleIndices.size(); j ++) { 
			
			for (int k = 0; k < CellsRows[i].rays.size(); k++) { // пересчитываем интенсивность лучей

				float distance = 0;
				if (mod(CellsRows[i].rays[k].LocalCoord - CellsRows[i].coord) > sbParticles[CellsRows[i].particleIndices[j]].radius) {

				}
				else {
				    alpha = sbParticles[CellsRows[i].particleIndices[j]].opacity;
				    Intencity = CellsRows[i].rays[k].Intencity; 

					SumIntencity += Intencity;//

					if (alpha <= 0.3) { //шар прозрачный не будет темным,часть света отразитс€ 
						alpha = 0.5;
					}
					sbShadows[CellsRows[i].particleIndices[j]] += Intencity * alpha;
					CellsRows[i].rays[k].Intencity *= (1 - alpha); 
					
					RaysCounter++;
				}
			   
			}
			//sbShadows[CellsRows[i].particleIndices[j]] += SumIntencity * alpha / RaysCounter;
			///SumIntencity = 0;
			//RaysCounter = 0;
		}

	}

	//RotateParticles();

}

void ComputeShadows::CalculateSphereCoordinates()
{

	float phi = 2.0*PI, theta = PI/2.0 ;
	float distance_between = 3.0f; 
	float r = 3.0;
	float n_phi, n_theta;
	
	sbParticles[0].pos = make_vec3r(0, 0, 0);
	int p = 1; 
	std::cout << sbParticles.size() << std::endl;
	while (p < sbParticles.size()) { 
		std::cout << p << std::endl;
		n_phi = 2.0 * PI * r / 3.6;
		n_theta = PI * r / 3.6;
		for (int i = 0; i < n_phi; i++) {
			for (int j = 0; j < n_theta; j ++) {
				
				if (p < sbParticles.size()) {
					sbParticles[p].pos.x = r * cos(2.0 * PI * i / n_phi) * sin(PI * j / (n_theta * 2.0)); 
					sbParticles[p].pos.y = r * sin(2.0 * PI * i / n_phi) * sin(PI * j / (n_theta * 2.0));
					sbParticles[p].pos.z = r * cos(PI * j / (n_theta * 2.0));
				}
				p++;

				
			}
		}

		r += distance_between;
	}

}


mat3r ComputeShadows::CalculateLight( )
{
	vec3r norm_light = normalize(sunDir);
	vec3r oz = make_vec3r(0, 0, 1);
	vec3r oy = make_vec3r(0, 1, 0);
	vec3r ox = make_vec3r(0, 1, 0);

	float angle = CalcAngle(norm_light, oz);
	vec3r axis = CrossProduct(norm_light, oz);
	axis = normalize(axis);
	vec4r Quat = make_vec4r(axis.x, axis.y, axis.z, angle);
	mat3r RotationMat = CalculateQuaternionMatrix(Quat);

	return RotationMat;
}

void ComputeShadows::RotateParticles()
{
	mat3r Matrix = CalculateLight();
	for (int i = 0; i < sbParticles.size();i++) {
		sbParticles[i].pos = Matrix*sbParticles[i].pos;

	}
}

void ComputeShadows::CreateGridCoordinates()
{

	int a,  b , c; 
	a = BoxSize;
	b = BoxSize;
	c = BoxSize;


	std::vector<float> oz; 
	oz.resize(c);

	
	 for (int i = 0; i <c; i++) {
		oz[i] = cell_size/2.0f + i * cell_size;
	}
	

	CellsRows.resize(a*b); 

	std::vector<Ray> _rays(RaysNum); 

	int p = 0;
	for (int i = 0; i < a; i++) {  
		for (int j = 0; j < b; j++) {
			
			CellsRows[p].coord = make_vec2r(cell_size/2.0f + i* cell_size, cell_size/2.0f + j* cell_size);  

			CellsRows[p].OzCoords = oz;
			
			for (int k = 0; k < RaysNum; k++) { //—оздаем лучи в каждой €чейке
				_rays[k].Intencity = 1; 
				_rays[k].LocalCoord = make_vec2r((float)rand_between(i* cell_size*10, cell_size * 5 + i* cell_size * 10) / 10.0f,
					              (float)rand_between(j* cell_size * 10, cell_size *5 + j* cell_size*10) / 10.0f);
			}
			
			CellsRows[p].rays = _rays;
			p++;
		}
	}
	
	//–аскладываем частицы по €чейкам:

	while (p < sbParticles.size()) {  

		for (int i = 0; i < CellsRows.size(); i++) {
			for (int j = 0; j < c; j++) {
				if(p < sbParticles.size()){

				sbParticles[p].radius = (float)rand_between(90, 120) / 100.0f;

				float delta = (cell_size - sbParticles[p].radius) / 2.0f;

				sbParticles[p].pos = make_vec3r(CellsRows[i].coord.x + (float)rand_between(-40 * delta, delta*40)/100.0f,
					                                CellsRows[i].coord.y + (float)rand_between(-40 * delta, delta * 40) / 100.0f,
					                                         CellsRows[i].OzCoords[j] + (float)rand_between(-40 *delta, delta * 40) / 100.0f);
				sbParticles[p].opacity = (float)rand_between(0, 100) / 100.0f;
				
				CellsRows[i].particleIndices.push_back(p);  
				}
				p++;
			}
			

		}	
	}
}

int ComputeShadows::rand_between(const int from, const int to)
{
		if (to == from)
			return to;
		if (to < from)
			return rand_between(to, from);
		return from + rand() % (to - from + 1);
}

	

