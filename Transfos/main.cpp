
#include <QApplication>
#include <QGLViewer/simple_viewer.h>
#include <matrices.h>
#include <primitives.h>


const Vec3 ROUGE   = {1,0,0};
const Vec3 VERT    = {0,1,0};
const Vec3 BLEU    = {0,0,1};
const Vec3 JAUNE   = {1,1,0};
const Vec3 CYAN    = {0,1,1};
const Vec3 MAGENTA = {1,0,1};
const Vec3 BLANC   = {1,1,1};
const Vec3 GRIS    = {0.5,0.5,0.5};
const Vec3 NOIR    = {0,0,0};

/// primitives(sphere/cube/cylindre/cone)
Primitives prim;

/// compteur (incremente dans animate)
int compteur = 0;

int finger_mid =1;
int finger_top = 1;

void flecheZ(const Mat4& tr, Vec3 coul)
{
    prim.draw_cylinder(tr*translate(0,0,1.5)*scale(0.5,0.5,1.95), coul);
    prim.draw_cone(tr*translate(0,0,3), coul);
}


void draw_repere(const Mat4& tr)
{
    prim.draw_sphere(tr, BLANC);
    flecheZ(tr,BLEU);
    flecheZ(tr*rotateY(90), ROUGE);
    flecheZ(tr*rotateX(-90),VERT);
}


void finger(const Mat4& tr){

    prim.draw_sphere(tr,BLANC);
    prim.draw_cube(tr*scale(1.7,0.5,0.5)*translate(0.5,0,0),ROUGE);

    prim.draw_sphere(tr*translate(1.9,0,0),BLANC);
    prim.draw_cube(tr*scale(1.3,0.5,0.5)*translate(2.1,0,0),VERT);

    prim.draw_sphere(tr*translate(3.7,0,0),BLANC);
    prim.draw_cube(tr*scale(1.1,0.5,0.5)*translate(3.9,0,0),BLEU);

}

void palm(const Mat4& tr){

    prim.draw_sphere(tr,BLANC);
    prim.draw_cube(tr*scale(3,0.5,3)*translate(0.6,0,0),CYAN);

    finger(tr*translate(1.7,0,-1.5)*rotateY(25));
    finger(tr*translate(3,0,-1.2));
    finger(tr*translate(3,0,0));
    finger(tr*translate(3,0,1.2));
}

void bras(const Mat4& tr){

   prim.draw_sphere(tr*scale(2,2,2),BLANC);
   prim.draw_cube(tr*scale(5,2,2)*translate(0.65,0,0),CYAN);

   prim.draw_sphere(tr*scale(2,2,2)*translate(3.3,0,0),BLANC);
   prim.draw_cube(tr*scale(6,2,2)*translate(1.7,0,0),CYAN);

   palm(tr*translate(13.5,0,0));
}


void articulations(const Mat4& transfo)
{
    bras(transfo);
}

void doight(Mat4 mat, float a, float s)
{
    prim.draw_sphere(mat,BLANC);

}


int main(int argc, char *argv[])
{
    // init du viewer
    QApplication a(argc, argv);
    SimpleViewer::init_gl();
    // ouvre une fenetre QGLViewer avec fond noir et une scene de rayon 10
    SimpleViewer viewer(NOIR,10);

    // GL init
    viewer.f_init = [&] ()
    {
        prim.gl_init();
    };

    // drawing

    viewer.f_draw = [&] ()
    {
        prim.set_matrices(viewer.getCurrentModelViewMatrix(), viewer.getCurrentProjectionMatrix());

        //prim.draw_sphere(Mat4(), BLANC);
        //prim.draw_cube(translate(2,0,0), ROUGE);
        //prim.draw_cone(translate(0,2,0), VERT);
        //prim.draw_cylinder(translate(0,0,2), BLEU);

        //draw_repere(Mat4());

        /*
         * EXO 3,2,1 *
         *
        for(int a = 0; a < 360; a+=20){
            Mat4 tr = Mat4()*rotateY(compteur+a)*translate(10,0,0)*rotateX(compteur);
            compteur++;
            draw_repere(tr);
        }

        */


    articulations(Mat4()*translate(-6,0,0)*rotateZ(24)*rotateY(24)*rotateX(24));
    };


    // to do when key pressed
    viewer.f_keyPress = [&] (int key, Qt::KeyboardModifiers /*mod*/)
    {
        switch(key)
        {
            case Qt::Key_A: // touche 'a'
                if (viewer.animationIsStarted())
                    viewer.stopAnimation();
                else
                    viewer.startAnimation();
                break;
            default:
                break;
        }
    };

    // to do every 50th of sec
    viewer.f_animate = [&] ()
    {
        compteur++;
        std::cout << "animate :" << compteur<< std::endl;
    };


    viewer.show();
    return a.exec();
}
