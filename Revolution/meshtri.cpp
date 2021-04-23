#include "meshtri.h"
#include "matrices.h"



void MeshTri::clear()
{
	m_points.clear();
	m_normals.clear();
	m_line_indices.clear();
	m_tri_indices.clear();
}


int MeshTri::add_vertex(const Vec3& P)
{
    // ajouter P à m_points
     m_points.push_back(P) ;

    // renvoyer l'indice du point
     return (m_points.size() - 1) ;
}


int MeshTri::add_normal(const Vec3& N)
{
    // ajouter P à m_normals
     m_normals.push_back(N);

    // renvoyer l'indice du point
     return (m_normals.size());
}

void MeshTri::add_line(int i1, int i2)
{
    m_line_indices.push_back(i1);
    m_line_indices.push_back(i2);
}


void MeshTri::add_tri(int i1, int i2, int i3)
{
    m_tri_indices.push_back(i1);
    m_tri_indices.push_back(i2);
    m_tri_indices.push_back(i3);
}

void MeshTri::add_quad(int i1, int i2, int i3, int i4)
{
    add_tri(i1,i2,i4);
    add_tri(i2,i3,i4);
}


void MeshTri::create_pyramide()
{
    clear();
    // ajouter les 5 points
    Vec3 A = {0,0,1};
    Vec3 B = {1,0,0};
    Vec3 C = {0,1,0};
    Vec3 D = {-1,0,0};
    Vec3 F = {0,-1,0};

   int a = add_vertex(A);
   int b = add_vertex(B);
   int c = add_vertex(C);
   int d = add_vertex(D);
   int f = add_vertex(F);

    // ajouter les 8 lignes (segments)
    add_line(a,b);
    add_line(a,c);
    add_line(b,c);
    add_line(a,d);
    add_line(a,f);
    add_line(d,c);
    add_line(f,b);
    add_line(f,d);


    // ajouter les 5 faces
    add_tri(a,b,c); //haut1
    add_tri(a,d,f); // haut2
    add_tri(a,c,d); // haut3
    add_tri(a,f,b); //haut4

    add_quad(b,c,d,f); //base

    gl_update();
}


void MeshTri::anneau()
{
    clear();

    // ajouter les points
    Mat4 mat1 = translate(0,0,0);

    Vec4 v4 = Vec4(1,1,1,1);
    std::vector<int> c1;
    std::vector<int> c2;

    for(int i = 0;i < 360;i+=30){
        c1.push_back(add_vertex(Vec3(scale(1,1,0)*rotateZ(i)*v4)));
    }

    for(int i = 0;i < 360;i+=30){
        c2.push_back(add_vertex(Vec3(scale(1.5,1.5,0)*rotateZ(i)*v4)));
    }

    // ajouter les lignes
    for(int i = 0 ;i < c1.size(); i++ ){
        add_line(c1[i],c1[(i+1)%c1.size()]);
    }

    for(int i = 0 ;i < c2.size(); i++ ){
        add_line(c2[i],c2[(i+1)%c2.size()]);
    }

    // ajouter les faces
    for(int i = 0; i < c1.size();i++){
        add_quad(c1[i], c1[(i+1)%c1.size()], c2[i], c2[(i+1)%c2.size()]);
    }

    gl_update();
}

void MeshTri::spirale()
{
    clear();
    // ajouter les points

    float x1,x2,x3,x4,z1,z2,z3,z4;
    int i;
/*
    for(int i=1; i<360; i=i+0.5){
        Vec3 P1 = {x1,i/10,z1};
        Vec3 P2 = {x2,i/10,z2};
        Vec3 P3 = {x3,i/10+0.01,z3};
        Vec3 P4 = {x4,i/10+0.01,z4};
    // ajouter les lignes

    // ajouter les faces
    for(int y = 0; i < 360;y++){
        add_quad(x1, x2, x3, x4);
    }

    int i1=add_vertex(P1);
    int i2=add_vertex(P2);
    int i3=add_vertex(P3);
    int i4=add_vertex(P4);

    }
*/
    gl_update();
}


void MeshTri::revolution(const std::vector<Vec3>& poly)
{
    clear();
    // ajouter les points
    // ajouter les normales au sommets
    // ajouter les lignes
    // ajouter les faces

    // Faire varier angle 0 -> 360 par pas de D degre
    // Faire tourner les sommets du polygon -> nouveau points

    // on obtient une grille de M x N (360/D x poly.nb) points

    // pour la creation des quads qui relient ces points,
    // attention la derniere rangee doit etre reliee a la premiere
    float rot;
    int i=0;
    for(rot = 0; rot < 360 ; rot += 0.1){
        for(i=0;i < (int) m_points.size()-1; i++){


            add_line(i,i+1);

        }

    }


    gl_update();
}


MeshTri::MeshTri()
{}



void MeshTri::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_phong = new ShaderProgramPhong();
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo2);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//VAO2
	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_phong->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_phong->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glEnableVertexAttribArray(m_shader_phong->idOfNormalAttribute);
	glVertexAttribPointer(m_shader_phong->idOfNormalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	//EBO indices
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_ebo2);
}

void MeshTri::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!m_normals.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
		glBufferData(GL_ARRAY_BUFFER, 3 * m_normals.size() * sizeof(GLfloat), &(m_normals[0][0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//EBO indices
	if (!m_tri_indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_tri_indices.size() * sizeof(int), &(m_tri_indices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (!m_line_indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_line_indices.size() * sizeof(int), &(m_line_indices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}



void MeshTri::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void MeshTri::draw_lines(const Vec3& color)
{
	m_shader_color->startUseProgram();

	m_shader_color->sendViewMatrix(viewMatrix);
	m_shader_color->sendProjectionMatrix(projectionMatrix);

	glBindVertexArray(m_vao);

	if (has_faces())
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
		glUniform3f(m_shader_color->idOfColorUniform, 0,0,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
		glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo2);
	glDrawElements(GL_LINES, m_line_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_color->stopUseProgram();
}


void MeshTri::draw_flat(const Vec3& color)
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}


void MeshTri::draw_smooth(const Vec3& color)
{
	m_shader_phong->startUseProgram();

	m_shader_phong->sendViewMatrix(viewMatrix);
	m_shader_phong->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_phong->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_phong->stopUseProgram();
}




