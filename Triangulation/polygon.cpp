#include "polygon.h"

// Calcul si il y a intersection entre deux segments AB et CD
bool intersecte(const Vec3 &A, const Vec3 &B, const Vec3 &C, const Vec3 &D) {
  return true;
}

void PolygonEditor::add_vertex(float x, float y) {
  // On ajoute le point si le polygone reste simple
}

void PolygonEditor::remove_last() {
  if (!m_points.empty())
    m_points.pop_back();
}

void PolygonEditor::clear() { m_points.clear(); }

void PolygonEditor::lisse() {
  uint64_t n = m_points.size();
  std::vector<Vec3> subd;
  subd.reserve(n * 2);

  // bonus : comme pour le TP2 en n'oubliant pas le premier et le dernier point

  m_points.swap(subd);
}

void PolygonEditor::polygon_to_fan_triangle(
    std::vector<int> &triangle_indices) {
  // ajout de trous les triangles formés par le premier sommet du polygone et deux
  // points consécutifs
}

Vec3 PolygonEditor::compute_normale() {
  Vec3 n(0, 0, 0);
  // ajout à n de la normale des triangles formé par les triplets de points
  // consécutifs du polygone
  return n;
}

// Vérifie si un point p est à l'intérieur d'un triangle p0p1p2
bool point_inside_triangle(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p) {

  // Le point est à l'intérieur du triangle si il est toujour à gauche/droite de
  // tout les cotés du triangle
}

PolygonEditor::PolyLine PolygonEditor::to_poly_line() {
  PolyLine result(0);
  uint i = m_points.size() - 1;
  while (i > 0) {
    result.add(i);
    --i;
  }
  return result;
}

void PolygonEditor::polygon_ear_triangle(std::vector<int> &triangle_indices) {
  PolyLine pl = to_poly_line();
  uint size = m_points.size();

  PolyLine *it = &pl;

  // pour chaque angle du polygone on vérifie qu'il est inférieur à pi

  // On vérifie que aucun point du polygone n'est à l'intérieur du triangle
  // formé par les trois points

  // Quand on trouve une oreille on ajoute ses indices de sommets
  // puis on l'enlève du polygone
}

void triangles_to_edges(const std::vector<int> &triangles_indices,
                        std::vector<int> &edges_indices) {
  for (uint32_t i = 0; i < triangles_indices.size(); i += 3) {
    int it1 = triangles_indices[i];
    int it2 = triangles_indices[i + 1];
    int it3 = triangles_indices[i + 2];
    edges_indices.push_back(it1);
    edges_indices.push_back(it2);
    edges_indices.push_back(it2);
    edges_indices.push_back(it3);
    edges_indices.push_back(it1);
    edges_indices.push_back(it3);
  }
}

PolygonEditor::PolygonEditor()
    : enable_triangulation(false), use_ear_triangulation(false),
      draw_polygon(true) {}

void PolygonEditor::gl_init() {
  // SHADER
  m_shader_color = new ShaderProgramColor();

  //	//VBO
  glGenBuffers(1, &m_vbo);

  // genere 1 VAO
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
  glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT,
                        GL_FALSE, 0, 0);
  glBindVertexArray(0);

  // EBO indices
  glGenBuffers(1, &m_ebo);
}

void PolygonEditor::draw(const Vec3 &color) {
  if (m_points.empty())
    return;

  Mat4 id;

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(Vec3), m_points.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_shader_color->startUseProgram();
  m_shader_color->sendViewMatrix(id);
  m_shader_color->sendProjectionMatrix(id);

  if (enable_triangulation && m_points.size() >= 3) {
    std::vector<int> triangles;
    std::vector<int> edges;
    if (use_ear_triangulation)
      polygon_ear_triangle(triangles);
    else
      polygon_to_fan_triangle(triangles);
    triangles_to_edges(triangles, edges);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(int),
                 &(triangles[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Vec3 tmp(0, 1, 0);
    glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(tmp));
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges.size() * sizeof(int),
                 &(edges[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_LINES, edges.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  if (draw_polygon) {
    Vec3 tmp(1, 0, 0);
    glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(tmp));
    glBindVertexArray(m_vao);
    glPointSize(4.0);
    glDrawArrays(GL_POINTS, 0, m_points.size());
    glDrawArrays(GL_LINE_LOOP, 0, m_points.size());
    glBindVertexArray(0);
  }

  m_shader_color->stopUseProgram();
}
