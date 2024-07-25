#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform {
public:
	vec3 position;
	quat rotation;
	vec3 scale;
	Transform() :
		position(vec3(0.0, 0.0, 0.0)),
		rotation(quat(1.0, vec3(0.0,0.0,0.0))),
		scale(vec3(1.0, 1.0, 1.0))
	{}
	Transform(vec3 p, quat r, vec3 s) :
		position(p),
		rotation(r),
		scale(s)
	{}

	static mat4 Matrix(
		float m00, float m01, float m02, float m03, // LINHA 1
		float m10, float m11, float m12, float m13, // LINHA 2
		float m20, float m21, float m22, float m23, // LINHA 3
		float m30, float m31, float m32, float m33  // LINHA 4
	)
	{
		return mat4(
			m00, m10, m20, m30, // COLUNA 1
			m01, m11, m21, m31, // COLUNA 2
			m02, m12, m22, m32, // COLUNA 3
			m03, m13, m23, m33  // COLUNA 4
		);
	}

	// Matriz identidade.
	mat4 Matrix_Identity()const
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f, // LINHA 1
			0.0f, 1.0f, 0.0f, 0.0f, // LINHA 2
			0.0f, 0.0f, 1.0f, 0.0f, // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f   // LINHA 4
		);
	}

	// Matriz de transla��o T. Seja p=[px,py,pz,pw] um ponto e t=[tx,ty,tz,0] um
	// vetor em coordenadas homog�neas, definidos em um sistema de coordenadas
	// Cartesiano. Ent�o, a matriz T � definida pela seguinte igualdade:
	//
	//     T*p = p+t.
	//
	static mat4 Matrix_Translate(vec3 t)
	{
		float tx = t.x;
		float ty = t.y;
		float tz = t.z;

		return Matrix(
			1.0f, 0.0f, 0.0f, tx,  // LINHA 1
			0.0f, 1.0f, 0.0f, ty,  // LINHA 2
			0.0f, 0.0f, 1.0f, tz,  // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f    // LINHA 4
		);
	}

	// Matriz S de "escalamento de um ponto" em rela��o � origem do sistema de
	// coordenadas. Seja p=[px,py,pz,pw] um ponto em coordenadas homog�neas.
	// Ent�o, a matriz S � definida pela seguinte igualdade:
	//
	//     S*p = [sx*px, sy*py, sz*pz, pw].
	//
	static mat4 Matrix_Scale(vec3 s)
	{
		float sx = s.x;
		float sy = s.y;
		float sz = s.z;

		return Matrix(
			sx, 0.0f, 0.0f, 0.0f,  // LINHA 1
			0.0f, sy, 0.0f, 0.0f,  // LINHA 2
			0.0f, 0.0f, sz, 0.0f,  // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f    // LINHA 4
		);
	}

	// Matriz R de "rota��o de um ponto" em rela��o � origem do sistema de
	// coordenadas e em torno do eixo X (primeiro vetor da base do sistema de
	// coordenadas). Seja p=[px,py,pz,pw] um ponto em coordenadas homog�neas.
	// Ent�o, a matriz R � definida pela seguinte igualdade:
	//
	//   R*p = [ px, c*py-s*pz, s*py+c*pz, pw ];
	//
	// onde 'c' e 's' s�o o cosseno e o seno do �ngulo de rota��o, respectivamente.
	static mat4 Matrix_Rotate_X(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,  // LINHA 1
			0.0f, c, -s, 0.0f,  // LINHA 2
			0.0f, s, c, 0.0f,  // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f    // LINHA 4
		);
	}

	// Matriz R de "rota��o de um ponto" em rela��o � origem do sistema de
	// coordenadas e em torno do eixo Y (segundo vetor da base do sistema de
	// coordenadas). Seja p=[px,py,pz,pw] um ponto em coordenadas homog�neas.
	// Ent�o, a matriz R � definida pela seguinte igualdade:
	//
	//   R*p = [ c*px+s*pz, py, -s*px+c*pz, pw ];
	//
	// onde 'c' e 's' s�o o cosseno e o seno do �ngulo de rota��o, respectivamente.
	static mat4 Matrix_Rotate_Y(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return Matrix(
			// PREENCHA AQUI A MATRIZ DE ROTA��O (3D) EM TORNO DO EIXO Y EM COORD.
			// HOMOG�NEAS, UTILIZANDO OS PAR�METROS c e s
			c, 0.0f, s, 0.0f,  // LINHA 1
			0.0f, 1.0f, 0.0f, 0.0f,  // LINHA 2
			-s, 0.0f, c, 0.0f,  // LINHA 3
			0.0f, 0.0f, 0.0f, 1    // LINHA 4
		);
	}

	// Matriz R de "rota��o de um ponto" em rela��o � origem do sistema de
	// coordenadas e em torno do eixo Z (terceiro vetor da base do sistema de
	// coordenadas). Seja p=[px,py,pz,pw] um ponto em coordenadas homog�neas.
	// Ent�o, a matriz R � definida pela seguinte igualdade:
	//
	//   R*p = [ c*px-s*py, s*px+c*py, pz, pw ];
	//
	// onde 'c' e 's' s�o o cosseno e o seno do �ngulo de rota��o, respectivamente.
	static mat4 Matrix_Rotate_Z(float angle) 
	{
		float c = cos(angle);
		float s = sin(angle);
		return Matrix(
			// PREENCHA AQUI A MATRIZ DE ROTA��O (3D) EM TORNO DO EIXO Z EM COORD.
			// HOMOG�NEAS, UTILIZANDO OS PAR�METROS c e s
			c, -s, 0.0f, 0.0f,  // LINHA 1
			s, c, 0.0f, 0.0f,  // LINHA 2
			0.0f, 0.0f, 1.0f, 0.0f,  // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f    // LINHA 4
		);
	}

	static mat4 Matrix_Rotate(float angle, vec4 axis)
	{
		float c = cos(angle);
		float s = sin(angle);

		vec4 v = axis / length(axis);

		float vx = v.x;
		float vy = v.y;
		float vz = v.z;

		return Matrix(
			vx * vx * (1 - c) + c, vx * vy * (1 - c) + vz * s, vx * vz * (1 - c) + vy * s, 0.0f,  // LINHA 1
			vx * vy * (1 - c) + vz * s, vy * vy * (1 - c) + c, vy * vz * (1 - c) - vx * s, 0.0f,  // LINHA 2
			vx * vz * (1 - c) - vy * s, vy * vz * (1 - c) + vx * s, vz * vz * (1 - c) + c, 0.0f,  // LINHA 3
			0.0f, 0.0f, 0.0f, 0.0f    // LINHA 4
		);
	}

	// Matriz de mudan�a de coordenadas para o sistema de coordenadas da C�mera.
	static mat4 Span_change(vec3 position_c, vec3 view_vector, vec3 up_vector)
	{
		vec3 w = -view_vector;
		vec3 u = cross(up_vector, w);

		// Normalizamos os vetores u e w
		w = w / length(w);
		u = u / length(u);

		vec3 v = cross(w, u);

		vec3 origin_o = vec3(0.0f, 0.0f, 0.0f);

		float ux = u.x;
		float uy = u.y;
		float uz = u.z;
		float vx = v.x;
		float vy = v.y;
		float vz = v.z;
		float wx = w.x;
		float wy = w.y;
		float wz = w.z;

		return Matrix(
			ux, uy, uz, dot(-u, position_c - origin_o),  // LINHA 1
			vx, vy, vz, dot(-v, position_c - origin_o),  // LINHA 2
			wx, wy, wz, dot(-w, position_c - origin_o),  // LINHA 3
			0.0f, 0.0f, 0.0f, 1.0f    // LINHA 4
		);
	}

	mat4 GetModelMatrix() const {
		mat4 I = Matrix_Identity();
		vec3 r = eulerAngles(rotation);
		return Matrix_Translate(position) * Matrix_Scale(scale) * Matrix_Rotate_Z(r.z) * Matrix_Rotate_Y(r.y) * Matrix_Rotate_X(r.x) * I;
	}

	Transform* lookAt(vec3 target, vec3 worldUp = vec3(0.0,1.0,0.0)) {
		target = normalize(target);

		vec3 front = getFront();
		vec3 up = getUp();
		vec3 axis = cross(front, target);
		float w = 1.0f + dot(front, target);

		rotation = normalize(quat(w, axis));


		vec3 realUp = normalize(cross(cross(front, worldUp), front));

		axis = cross(up, realUp);
		w = 1.0f + dot(up, realUp);

		rotation = normalize(quat(w, axis)) * rotation;
		
		return this;
	}

	Transform* resize(vec3 s) {
		this->scale *= s;
		return this;
	}

	vec3 getFront() {
		return rotation * vec3(0.0,0.0,1.0);
	}
	vec3 getUp() {
		return rotation * vec3(0.0, 1.0, 0.0);
	}
	vec3 getRight() {
		return rotation * vec3(1.0, 0.0, 0.0);
	}

	Transform operator + (const Transform& b) const {
		return Transform(position + b.position, rotation * b.rotation, scale * b.scale);
	}

	bool operator < (const Transform& b) const {
		return position.x < b.position.x;
	}
};