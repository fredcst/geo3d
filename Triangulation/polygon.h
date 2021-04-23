#ifndef POLYGON_EDITOR_H
#define POLYGON_EDITOR_H

#include <GL/glew.h>
#include <OGLRender/shaderprogramcolor.h>
#include <vector>

#include <matrices.h>

class PolygonEditor {
  // Structure pour représenter un polygone pour une liste doublement chaînées
  // de sommet
  struct PolyLine {
    // Le prochain sommet
    PolyLine *next_;
    // Le précédent sommet
    PolyLine *prev_;
    // Indice du sommet dans m_point
    int index_point_;
    PolyLine(int index_point) : index_point_(index_point) {
      next_ = this;
      prev_ = this;
    }
    void remove() {
      if (next_ == this) {
        index_point_ = -1;
        next_ = nullptr;
        prev_ = nullptr;
      }
      next_->prev_ = prev_;
      prev_->next_ = next_;
    }
    void add(int index_point) {
      PolyLine *tmp = next_;
      next_ = new PolyLine(index_point);
      next_->prev_ = this;
      tmp->prev_ = next_;
      next_->next_ = tmp;
    }
    bool operator==(const PolyLine &p) {
      return p.index_point_ == index_point_;
    }
    bool operator!=(const PolyLine &p) {
      return p.index_point_ != index_point_;
    }
  };

  std::vector<Vec3> m_points;
  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_ebo;
  ShaderProgramColor *m_shader_color;

  /**
   * @brief Calcul la normal du polygone
   * @return La normale du polygone
   */
  Vec3 compute_normale();
  PolyLine to_poly_line();

public:
  bool enable_triangulation;

  bool use_ear_triangulation;

  bool draw_polygon;

  PolygonEditor();

  void draw(const Vec3 &color);

  /**
   * @brief ajoute un sommet au tableau de sommet
   * @param x coordonnée x du sommet
   * @param y coordonnée y du sommet
   */
  void add_vertex(float x, float y);

  /**
   * @brief retire le dernier sommet ajouté dans le tableau de sommet
   */
  void remove_last();

  /**
   * @brief vide le tableau de sommet
   */
  void clear();

  void gl_init();

  /**
   * @brief lisse le polygone
   */
  void lisse();

  /**
   * @brief triangulation éventail du polygone
   * @param triangle_indices tableau d'indices des triangle de la triangulation
   */
  void polygon_to_fan_triangle(std::vector<int> &triangle_indices);

  /**
   * @brief triangulation du polygone par la méthode des oreilles
   * @param triangle_indices tableau d'indices des triangle de la triangulation
   */
  void polygon_ear_triangle(std::vector<int> &triangle_indices);

  inline const std::vector<Vec3> &vertices() { return m_points; }
};

#endif // POLYGON_EDITOR_H
