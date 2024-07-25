#include "Ocean.h"

Ocean::Ocean() {
	Mesh ms("meshes/planeHIGH.obj");
	ocean_drawObject.indexes_size = ms.indices.size();
	Renderer::BuildTrianglesVAO(ms.model_coefficients, ms.normal_coefficients, ms.tangent_coefficients, ms.texture_coefficients, ms.indices, &ocean_drawObject);
	ocean_drawObject.material = Renderer::default_material;
	ocean_transform.scale = glm::vec3(10.0f);
}
void Ocean::update() {
	p_f = Scene::paramsf;
	ocean_drawObject.material.v4params[4] = vec4(p_f[6], p_f[7], p_f[8], p_f[9]);
	ocean_drawObject.material.fparams[0] = p_f[10];
	ocean_drawObject.material.fparams[1] = p_f[11];
	ocean_drawObject.material.fparams[2] = p_f[12];
	ocean_drawObject.material.fparams[3] = p_f[13];
	ocean_drawObject.material.fparams[4] = p_f[4];
}
void Ocean::draw() {
	Renderer::pq.push(make_tuple(
		0,
		&ocean_drawObject,
		&ocean_transform,
		0
	));
}