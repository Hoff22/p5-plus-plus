#include "Renderer.h"

GLsizei Renderer::FRAME_WIDTH = 800;
GLsizei Renderer::FRAME_HEIGHT = 600;
Shader Renderer::gizmoShader;
Shader Renderer::noShadingShader;
Shader Renderer::PBRShader;
Material Renderer::default_material;
DrawObject Renderer::circle_primitive;
DrawObject Renderer::sprite_primitive;
DrawObject Renderer::sphere_primitive;
DrawObject Renderer::line_primitive;
DrawObject Renderer::axisGizmo_primitive;
GLuint Renderer::numLoadedTextures = 0;
GLuint Renderer::loadedTextureId[10] = { 0,0,0,0,0,0,0,0,0,0 };
priority_queue<tuple<int, DrawObject*, Transform*, GLsizei>> Renderer::pq;

GLuint Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& normal_coefficients, const vector<GLuint>& indices)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO_model_coefficients_id;
	glGenBuffers(1, &VBO_model_coefficients_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!normal_coefficients.empty())
	{
		GLuint VBO_normal_coefficients_id;
		glGenBuffers(1, &VBO_normal_coefficients_id);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
		glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
		location = 1; // "(location = 1)" em "shader_vertex.glsl"
		number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
		glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//if (!texture_coefficients.empty())
	//{
	//	GLuint VBO_texture_coefficients_id;
	//	glGenBuffers(1, &VBO_texture_coefficients_id);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
	//	glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
	//	location = 2; // "(location = 1)" em "shader_vertex.glsl"
	//	number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
	//	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(location);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	GLuint indices_id;
	glGenBuffers(1, &indices_id);

	// "Ligamos" o buffer. Note que o tipo agora � GL_ELEMENT_ARRAY_BUFFER.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
	//

	// "Desligamos" o VAO, evitando assim que opera��es posteriores venham a
	// alterar o mesmo. Isso evita bugs.
	glBindVertexArray(0);

	// Retornamos o ID do VAO. Isso � tudo que ser� necess�rio para renderizar
	// os tri�ngulos definidos acima. Veja a chamada glDrawElements() em main().

	return VAO;
}
void Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& nromal_coefficients, const vector<float>& tangent_coefficients, const vector<float>& uv_coefficients, const vector<GLuint>& indices, DrawObject* obj)
{
#if 1
	if (obj->VAO == 0) {
		glGenVertexArrays(1, &obj->VAO);
	}
	glBindVertexArray(obj->VAO);

	if (obj->VBO_positions == 0) {
		glGenBuffers(1, &obj->VBO_positions);
	}
	if (obj->VBO_normals == 0) {
		glGenBuffers(1, &obj->VBO_normals);
	}
	if (obj->VBO_tangents == 0) {
		glGenBuffers(1, &obj->VBO_tangents);
	}
	if (obj->VBO_uv == 0) {
		glGenBuffers(1, &obj->VBO_uv);
	}
	if (obj->VBO_index == 0) {
		glGenBuffers(1, &obj->VBO_index);
	}

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, nromal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nromal_coefficients.size() * sizeof(float), nromal_coefficients.data());
	location = 1; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_tangents);
	glBufferData(GL_ARRAY_BUFFER, tangent_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, tangent_coefficients.size() * sizeof(float), tangent_coefficients.data());
	location = 2; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, uv_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uv_coefficients.size() * sizeof(float), uv_coefficients.data());
	location = 3; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices_id = obj->VBO_index;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);
#endif
}
void Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& nromal_coefficients, const vector<float>& uv_coefficients, const vector<GLuint>& indices, DrawObject* obj)
{
#if 1
	if (obj->VAO == 0) {
		glGenVertexArrays(1, &obj->VAO);
	}
	glBindVertexArray(obj->VAO);

	if (obj->VBO_positions == 0) {
		glGenBuffers(1, &obj->VBO_positions);
	}
	if (obj->VBO_normals == 0) {
		glGenBuffers(1, &obj->VBO_normals);
	}
	if (obj->VBO_uv == 0) {
		glGenBuffers(1, &obj->VBO_uv);
	}
	if (obj->VBO_index == 0) {
		glGenBuffers(1, &obj->VBO_index);
	}

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, nromal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nromal_coefficients.size() * sizeof(float), nromal_coefficients.data());
	location = 1; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, uv_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uv_coefficients.size() * sizeof(float), uv_coefficients.data());
	location = 3; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices_id = obj->VBO_index;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);
#endif
}
GLuint Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<GLuint>& indices)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO_model_coefficients_id;
	glGenBuffers(1, &VBO_model_coefficients_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices_id;
	glGenBuffers(1, &indices_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);

	return VAO;
}

void Renderer::BuildInstanceVAO(const vector<Transform>& transforms, DrawObject* obj) {
	
	vector<float> M0_coefficients;
	vector<float> M1_coefficients;
	vector<float> M2_coefficients;
	vector<float> M3_coefficients;

	for (const Transform& t : transforms) {
		mat4 model = t.GetModelMatrix();
		for (int i = 0; i < 4; i++) {
			M0_coefficients.push_back(model[0][i]);
		}
		for (int i = 0; i < 4; i++) {
			M1_coefficients.push_back(model[1][i]);
		}
		for (int i = 0; i < 4; i++) {
			M2_coefficients.push_back(model[2][i]);
		}
		for (int i = 0; i < 4; i++) {
			M3_coefficients.push_back(model[3][i]);
		}
	}
	
	if (obj->VAO == 0) {
		glGenVertexArrays(1, &obj->VAO);
	}
	glBindVertexArray(obj->VAO);

	if (obj->VBO_instancedM0 == 0) {
		glGenBuffers(1, &obj->VBO_instancedM0);
		glGenBuffers(1, &obj->VBO_instancedM1);
		glGenBuffers(1, &obj->VBO_instancedM2);
		glGenBuffers(1, &obj->VBO_instancedM3);
	}

	GLuint location = 4; // "(location = 4)" em "shader_vertex.glsl"
	GLint number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	
	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_instancedM0);
	glBufferData(GL_ARRAY_BUFFER, M0_coefficients.size() * sizeof(float), M0_coefficients.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(location + 0);
	glVertexAttribPointer(location + 0, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_instancedM1);
	glBufferData(GL_ARRAY_BUFFER, M1_coefficients.size() * sizeof(float), M1_coefficients.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(location + 1);
	glVertexAttribPointer(location + 1, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_instancedM2);
	glBufferData(GL_ARRAY_BUFFER, M2_coefficients.size() * sizeof(float), M2_coefficients.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(location + 2);
	glVertexAttribPointer(location + 2, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_instancedM3);
	glBufferData(GL_ARRAY_BUFFER, M3_coefficients.size() * sizeof(float), M3_coefficients.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(location + 3);
	glVertexAttribPointer(location + 3, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(location + 0, 1);
	glVertexAttribDivisor(location + 1, 1);
	glVertexAttribDivisor(location + 2, 1);
	glVertexAttribDivisor(location + 3, 1);
	
	glBindVertexArray(0);
}

void Renderer::SetMaterialParams(const Material& material, Shader* shader) {
	shader->setFloat("FloatParameter0", glfwGetTime());
	for (int i = 1; i < material.fparams.size(); i++) {
		string name = "FloatParameter";
		name += to_string(i);
		shader->setFloat(name, material.fparams[i]);
	}
	for (int i = 0; i < material.v4params.size(); i++) {
		string name = "VectorParameter";
		name += to_string(i);
		shader->setFloat4(name, material.v4params[i]);
	}
	for (int i = 0; i < material.textureIDs.size(); i++) {
		string name = "TextureImage";
		name += to_string(i);
		shader->setInt(name, material.textureIDs[i]);
	}
}

void Renderer::LoadTextureRGB(string filepath) {
	cout << "Carregando imagem " << filepath << "... " << "\n";

	if (numLoadedTextures == 10) {
		fprintf(stderr, "ERROR: too many textures loaded at once\n");
		std::exit(EXIT_FAILURE);
	}

	// Primeiro fazemos a leitura da imagem do disco
	stbi_set_flip_vertically_on_load(true);
	int width;
	int height;
	int channels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
	
	if (data == NULL)
	{
		data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		if (data == NULL) {
			fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filepath.c_str());
			std::exit(EXIT_FAILURE);
		}
	}

	printf("OK (%dx%d).\n", width, height);

	// Agora criamos objetos na GPU com OpenGL para armazenar a textura
	GLuint sampler_id;
	glGenTextures(1, &loadedTextureId[numLoadedTextures]);
	glGenSamplers(1, &sampler_id);

	// Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Par�metros de amostragem da textura.
	glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Agora enviamos a imagem lida do disco para a GPU
	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	GLuint textureunit = numLoadedTextures;
	glActiveTexture(GL_TEXTURE0 + textureunit);
	glBindTexture(GL_TEXTURE_2D, loadedTextureId[numLoadedTextures]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindSampler(textureunit, sampler_id);

	stbi_image_free(data);

	numLoadedTextures += 1;
}
void Renderer::UnloadTextures() {
	for (int i = 0; i < 10; i++) {
		glDeleteTextures(1, &loadedTextureId[i]);
	}
	numLoadedTextures = 0;
}

void Renderer::DrawEnv(Camera* camera) {
	
	
	noShadingShader.use();
	noShadingShader.setMatrix4("view_m", camera->GetViewMatrix());
	noShadingShader.setMatrix4("proj_m", camera->GetProjectionMatrix());
	noShadingShader.setMatrix4("mode_m", Transform(camera->Position, glm::quat(1.0, vec3(0.0, 0.0, 0.0)), glm::vec3(2000.0f)).GetModelMatrix());

	Material mat;
	mat.textureIDs = { 0 };
	SetMaterialParams(mat, &noShadingShader);
	noShadingShader.setBool("is_instance", 0);

	glBindVertexArray(sphere_primitive.VAO);

	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, sphere_primitive.indexes_size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::RenderTriangles(DrawObject* obj, Transform* tr, Camera* camera, int prio, Shader* shader = &PBRShader, GLsizei instance_qnt = 0) {
	shader->use();
	shader->setMatrix4("view_m", camera->GetViewMatrix());
	shader->setMatrix4("proj_m", camera->GetProjectionMatrix());
	shader->setMatrix4("mode_m", tr->GetModelMatrix());

	obj->material.v4params[1] = glm::vec4(camera->Position, 0.0f);
	obj->material.v4params[2] = glm::vec4(camera->Front, 0.0f);
	obj->material.v4params[3] = glm::vec4(tr->position, 0.0f);
	SetMaterialParams(obj->material, shader);

	glBindVertexArray(obj->VAO);

	if (instance_qnt > 0) {
		if (obj->VBO_instancedM0 == 0) {
			std::cout << "ERROR::INSTANCE CALL FOR UNINITIALIZED RENDER OBJECT" << std::endl;
			exit(1);
		}
		shader->setBool("is_instance", 1);
		glDrawElementsInstanced(GL_TRIANGLES, obj->indexes_size, GL_UNSIGNED_INT, 0, instance_qnt);
	}
	else {
		shader->setBool("is_instance", 0);
		glDrawElements(GL_TRIANGLES, obj->indexes_size, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void Renderer::RenderLines(DrawObject* obj, const Transform& tr, Camera* camera, int prio, Shader* shader = &noShadingShader) {
	shader->use();
	shader->setMatrix4("view_m", camera->GetViewMatrix());
	shader->setMatrix4("proj_m", camera->GetProjectionMatrix());
	shader->setMatrix4("mode_m", tr.GetModelMatrix());

	shader->setFloat4("solid_color", glm::vec4(0.8, 0.8, 0.8, 1.0));

	// cout << "rendering obj VAO: " << obj->VAO << endl;

	glBindVertexArray(obj->VAO);
	//glCullFace(GL_BACK);
	glLineWidth(5);
	glDrawElements(GL_LINES, obj->indexes_size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::InitFrame(glm::vec4 bg_color, bool clear_color) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (clear_color) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0);
	}
	else {
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void Renderer::DrawFrame(glm::vec4 bg_color, Camera* camera, RenderFlag renderFlags) {
	
	Renderer::FRAME_WIDTH = MainWindow::SCR_WIDTH;
	Renderer::FRAME_HEIGHT = MainWindow::SCR_HEIGHT;

	if (static_cast<int>(renderFlags) & static_cast<int>(RenderFlag::DrawEnvironmentFlag)) {
		DrawEnv(camera);
		InitFrame(bg_color);
	}
	else {
		InitFrame(bg_color, true);
	}

	RenderLines(&axisGizmo_primitive, Transform(), camera, 0, &gizmoShader);
	
	while (pq.size()) {
		int prio;
		DrawObject* obj;
		Transform* tr;
		GLsizei instancecount;
		tie(prio, obj, tr, instancecount) = pq.top();
		RenderTriangles(obj, tr, camera, prio, &PBRShader, instancecount);
		pq.pop();
	}

	glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

	assert(MainWindow::window != NULL);

	MainWindow::drawUI();

	glfwSwapBuffers(MainWindow::window);
}

void Renderer::SetupPrimitives() {
	Mesh circle_mesh("meshes/circle.obj");

	circle_primitive.indexes_size = (int)circle_mesh.indices.size();
	circle_primitive.VAO = BuildTrianglesVAO(
		circle_mesh.model_coefficients,
		circle_mesh.normal_coefficients,
		circle_mesh.indices
		);

	Mesh sphere_mesh("meshes/sphere.obj");
	LoadTextureRGB("textures/hdri_env2.png");

	sphere_primitive.indexes_size = (int)sphere_mesh.indices.size();
	BuildTrianglesVAO(
		sphere_mesh.model_coefficients,
		sphere_mesh.normal_coefficients,
		sphere_mesh.tangent_coefficients,
		sphere_mesh.texture_coefficients,
		sphere_mesh.indices,
		&sphere_primitive
	);

	vector<float> axisGizmo_model_coefficients = {
		0.0,0.0,0.0,1.0,
		1.0,0.0,0.0,1.0,
		0.0,1.0,0.0,1.0,
		0.0,0.0,1.0,1.0
	};
	vector<float> axisGizmo_normal_coefficients = {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0
	};
	vector<GLuint> axisGizmo_indices = {
		0,1,
		0,2,
		0,3
	};

	axisGizmo_primitive.indexes_size = (int)axisGizmo_indices.size();
	axisGizmo_primitive.VAO = BuildTrianglesVAO(
		axisGizmo_model_coefficients,
		//axisGizmo_normal_coefficients,
		axisGizmo_indices
	);

	vector<float> line_model_coefficients = {
		0.0,0.0,0.0,1.0,
		0.0,0.0,-1.0,1.0
	};
	vector<GLuint> line_indices= {
		0,1
	};

	line_primitive.indexes_size = (int)line_indices.size();
	line_primitive.VAO = BuildTrianglesVAO(
		line_model_coefficients,
		line_indices
	);

	vector<float> sprite_model_coefficients = {
		-1.0,-1.0, 0.0,1.0,
		 1.0,-1.0, 0.0,1.0,
		-1.0, 1.0, 0.0,1.0,
		 1.0, 1.0, 0.0,1.0
	};
	vector<float> sprite_normal_coefficients = {
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0
	};
	vector<float> sprite_texture_coefficients = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0
	};
	vector<GLuint> sprite_indices = {
		0,1,2,
		2,1,3
	};

	sprite_primitive.indexes_size = (int)sprite_indices.size();
	BuildTrianglesVAO(
		sprite_model_coefficients,
		sprite_normal_coefficients,
		sprite_texture_coefficients,
		sprite_indices,
		&sprite_primitive
	);
	
	default_material.v4params = { glm::vec4(25.0 / 255.0,25.0 / 255.0,25.0 / 255.0,1.0), glm::vec4(1.0), glm::vec4(1.0), glm::vec4(1.0), glm::vec4(1.0), glm::vec4(1.0)};
	default_material.fparams = vector<float>(5);
	default_material.textureIDs = { 1, 0, 2 };
	noShadingShader = Shader("shaders/default_vertex.glsl", "shaders/noShading_fragment.glsl");
	PBRShader = Shader("shaders/pbr_vertex.glsl", "shaders/pbr_fragment.glsl");
	gizmoShader = Shader("shaders/default_vertex.glsl", "shaders/gizmo_fragment.glsl");
}

int Renderer::Setup_GLAD_GLFW_OpenGL_Shard(string program_window_name) {
	glfwSetErrorCallback(MainWindow::glfw_error_callback);
	if (!glfwInit()) return 1;

	glfwWindowHint(GLFW_SAMPLES, 4);

	MainWindow::window = glfwCreateWindow(MainWindow::SCR_WIDTH, MainWindow::SCR_HEIGHT, program_window_name.c_str(), NULL, NULL);
	if (MainWindow::window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(MainWindow::window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(MainWindow::window, MainWindow::framebuffer_size_callback);
	glfwSetScrollCallback(MainWindow::window, MainWindow::scroll_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init ui
	MainWindow::initUI();
	SetupPrimitives();
}