#pragma once
#include "Renderer.h"

class Ocean
{
	DrawObject	ocean_drawObject;
	Transform	ocean_transform;
	float*		p_f;

public:
	Ocean();
	void update();
	void draw();
};

