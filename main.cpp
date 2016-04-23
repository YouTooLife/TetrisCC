#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "windows.h"
/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

typedef struct vec2 {
    int x;
    int y;
} Vec2;

typedef struct figure {
    int x;
    int y;
    int n;
    Vec2 cube[10];
} Figure;

#define M 20
#define N 20

//char map[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,5,0}};
int **map;


Figure line = {0,0,4,{
    {0,0},{1,0},{2,0},{3,0}}};
Figure triangle = {3,3,4,{
       {0,0},{0,1},{-1,0},{1,0}}};
Figure square = {0,0,4,{
       {0,0},{1,0},{0,1},{1,1}}};
Figure trigon = {3,3,3,{
       {0,0},{0,1},{1,0}}};


int isCollision(Figure *fig) {
   /* for (i = 0; i < (fig->n); i++) {
        if ((map[fig->y][])
    }*/
    return 0;
}


Vec2 *addVec2(Vec2 vec, Vec2 addVec) {
    Vec2 *newVec = new Vec2;
    printf("- %d:%d\n",vec.x,vec.y);
    newVec->x = vec.x + addVec.x;
    newVec->y = vec.y + addVec.y;
    printf("- %d:%d\n",newVec->x,newVec->y);
    return newVec;
}

int moveFigure(Figure *fig, Vec2 addVec) {

    addVec.x += fig->x;
    addVec.y += fig->y;

    if (((addVec.x) >=N)
        || ((addVec.x) < 0)
        || ((addVec.y) >= M)) {
            printf("N|%d:%d",addVec.x,addVec.y);
            return 0;
    }

    for (int i = 0; i < (fig->n); i++) {
        Vec2 cube = fig->cube[i];
        if (((addVec.x+cube.x) >=N)
        || ((addVec.x+cube.x) < 0)
        || ((addVec.y-cube.y) >= M)) {
            printf("Nc|%d:%d",addVec.x,addVec.y);
        return 0;
        }
        if (map[addVec.y-cube.y][addVec.x+cube.x] > 0) {
            printf("NC|%d:%d",addVec.x,addVec.y);
            return 0;
        }
    }
    fig->x = addVec.x;
    fig->y = addVec.y;
    return 1;
}

Vec2 *rotateVec2(Vec2 vec) {
    Vec2 *newVec = new Vec2;
    //printf("- %d:%d\n",vec.x,vec.y);
    newVec->x = -vec.y;
    newVec->y = vec.x;
    //printf("- %d:%d\n",newVec->x,newVec->y);
    return newVec;
}

void rotateFigure(Figure *fig) {
    Vec2 buf[4];
    for (int i = 0; i < (fig->n); i++) {
        memcpy(&buf[i],&(fig->cube[i]),sizeof(Vec2));
        //printf("%d %d:%d\n",i,buf[i].x,buf[i].y);
        memcpy(&buf[i],rotateVec2(buf[i]),sizeof(Vec2));
        //printf("%d %d:%d\n",i,fig->x+buf[i].x,fig->y-buf[i].y);
        if (((fig->x+buf[i].x) >=N)
        || ((fig->x+buf[i].x) < 0)
        || ((fig->y-buf[i].y) >= M)) {
            delete buf;
        return ;
        }
        if (map[fig->y-buf[i].y][fig->x+buf[i].x] > 0) {
            delete buf;
            return;
        }
    }
    memcpy(&(fig->cube),&buf,fig->n*sizeof(Vec2));
    delete buf;
}

void doneFigure(Figure *fig) {
    for (int i = 0; i < (fig->n); i++)
        map[fig->y-fig->cube[i].y][fig->x+fig->cube[i].x]=5;
    memcpy(fig,&trigon,sizeof(Figure));
}


Figure *cfig = new figure;


void moveMap() {

int dl;
  for (int i = M-1; i >= 0; i--) {
        int cj = 0;
        int cz = 0;
        for (int j = 0; j < N; j++)
            if (map[i][j] == 0)
                cz++;
            else
                cj++;
        if (cj == N) {
            dl = i;
            break;
        }
        if (cz == N)
            return;
    }
     for (int i = dl-1; i >= 0; i--) {
         memcpy(map[i+1],map[i],N*sizeof(int));
    }

}

void drawMap() {

    int **dm = (int**) malloc(M*sizeof(int*));
    for (int i = 0; i < M; i++) {
        dm[i] = (int*) malloc(N*sizeof(int));
        memcpy(dm[i],map[i],N*sizeof(int));
    }

    for (int i = 0; i < (cfig->n); i++)
        dm[cfig->y-cfig->cube[i].y][cfig->x+cfig->cube[i].x]=1;
    for (int i = 2; i < M; i++) {
        for (int j = 0; j < N; j++)
            printf("%d",dm[i][j]);
        printf("\n");
    }
    printf("---------\n");
    delete dm;
}



int main(int argc, char *argv[])
{
    map = (int**)malloc(M*sizeof(int*));
    for (int i = 0; i < M;i++) {
        map[i] = (int*)malloc(N*sizeof(int));
        for (int j = 0; j < N; j++)
            map[i][j] = 0;
    }

    memcpy(cfig,&triangle,sizeof(Figure));
    cfig->y = 1;
    //rotateFigure(cfig);
    //drawMap();
    char key;
    do {
        //Sleep(10);
    key = getch();
        if (key == 'w')
            moveFigure(cfig,{0,-1});
        if (key == 's')
            moveFigure(cfig,{0,1});
        if (key == 'a')
            moveFigure(cfig,{-1,0});
        if (key == 'd')
            moveFigure(cfig,{1,0});
        if (key == 'e')
            rotateFigure(cfig);
        if (key == 'b')
            doneFigure(cfig);
        system("cls");
        drawMap();
        if (!moveFigure(cfig,{0,1})) {
            doneFigure(cfig);
            moveMap();
        }
        else {
            moveFigure(cfig,{0,-1});
        }
    } while (key!='0');
    return 0;
    /*glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glutMainLoop();

    return EXIT_SUCCESS;*/
}
