#include <OGLRender/grid2d.h>
#include <QApplication>
#include <QGLViewer/simple_viewer.h>
#include <matrices.h>
#include <polygon.h>

const Vec3 ROUGE = {1, 0, 0};
const Vec3 VERT = {0, 1, 0};
const Vec3 BLEU = {0, 0, 1};
const Vec3 JAUNE = {1, 1, 0};
const Vec3 CYAN = {0, 1, 1};
const Vec3 MAGENTA = {1, 0, 1};
const Vec3 BLANC = {1, 1, 1};
const Vec3 GRIS = {0.5, 0.5, 0.5};
const Vec3 NOIR = {0, 0, 0};

int main(int argc, char *argv[]) {
  // init du viewer
  QApplication a(argc, argv);
  SimpleViewer::init_gl();

  // polygone avec methode d'édition
  PolygonEditor polyg;

  // Fenetre de saisie du polygone
  SimpleViewer viewer2D({0, 0, 0}, 1);
  // croix
  Grid2d grid;

  viewer2D.f_init = [&]() {
    glDisable(GL_DEPTH_TEST);
    grid.gl_init();
    polyg.gl_init();
  };

  // drawing
  viewer2D.f_draw = [&]() {
    grid.draw(GRIS);
    polyg.draw(JAUNE);
  };

  // x,y coordonnées souris [-1, 1]
  viewer2D.f_mousePress = [&](Qt::MouseButton b, float x, float y) {
    switch (b) {
    case Qt::LeftButton:
      polyg.add_vertex(x, y);
      break;
    case Qt::RightButton:
      polyg.remove_last();
      break;
    default:
      break;
    }
  };

  viewer2D.f_keyPress = [&](int key, Qt::KeyboardModifiers /*mod*/) {
    switch (key) {
    case Qt::Key_L: // touche 'l'
      polyg.lisse();
      break;
    case Qt::Key_C: // touche 'c'
      polyg.clear();
      break;
    case Qt::Key_T: // touche 't'
      polyg.enable_triangulation = !polyg.enable_triangulation;
      break;
    case Qt::Key_E: // touche 'e'
      polyg.use_ear_triangulation = !polyg.use_ear_triangulation;
      break;
    case Qt::Key_D: // touche 'd'
      polyg.draw_polygon = !polyg.draw_polygon;
      break;
    default:
      break;
    }
    viewer2D.update();
  };

  viewer2D.setMinimumSize(500, 500);
  viewer2D.setMaximumSize(500, 500);
  viewer2D.setWindowTitle("Triangulation");
  viewer2D.show();

  return a.exec();
}
