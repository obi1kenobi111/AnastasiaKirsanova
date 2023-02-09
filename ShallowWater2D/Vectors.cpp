#include "Vectors.h"



vec2r operator - (vec2r a, vec2r b) {
	vec2r c = make_vec2r(a.x - b.x, a.y - b.y);
	return c;
}
