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
#include <time.h>
#include "windows.h"



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

char currentColor = 1;
long score = 0;
char *scoreText = (char*) calloc(1,sizeof(char));
char gameOver = 0;

int N = 10;
int M = 24;
int tabTop = 4;

int width = 800;
int height = 600;

Figure *cfig = new figure;

void *font = GLUT_BITMAP_TIMES_ROMAN_24;

//char map[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,5,0}};
char **map;

float oldTimeSinceStart = 0;
float timer = 0;
float deltaTime = 0;

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
    //printf("- %d:%d\n",newVec->x,newVec->y);
    return newVec;
}

int moveFigure(Figure *fig, Vec2 addVec) {

    addVec.x += fig->x;
    addVec.y += fig->y;

    if (((addVec.x) >=N)
        || ((addVec.x) < 0)
        || ((addVec.y) >= M)) {
           // printf("N|%d:%d",addVec.x,addVec.y);
            return 0;
    }

    for (int i = 0; i < (fig->n); i++) {
        Vec2 cube = fig->cube[i];
        if (((addVec.x+cube.x) >=N)
        || ((addVec.x+cube.x) < 0)
        || ((addVec.y-cube.y) >= M)) {
            //printf("Nc|%d:%d",addVec.x,addVec.y);
        return 0;
        }
        if (map[addVec.y-cube.y][addVec.x+cube.x] > 0) {
            //printf("NC|%d:%d",addVec.x,addVec.y);
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

    if (fig->y < tabTop)
        return;

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

void createFigure(Figure *fig) {
    char c = rand() % 4;
    currentColor = 1 + rand() % 4;
    if (c == 0)
        memcpy(fig,&line,sizeof(Figure));
    if (c == 1)
        memcpy(fig,&square,sizeof(Figure));
    if (c == 2)
        memcpy(fig,&triangle,sizeof(Figure));
    if (c == 3)
        memcpy(fig,&trigon,sizeof(Figure));
    fig->x = N / 2;
    fig->y = 2;

}

void doneFigure(Figure *fig) {
    if (fig->y < tabTop) {
        gameOver = 1;
        return;
    }
    for (int i = 0; i < (fig->n); i++)
        map[fig->y-fig->cube[i].y][fig->x+fig->cube[i].x]=currentColor;
    createFigure(fig);
}

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
            score++;
            dl = i;
            break;
        }
        if (cz == N)
            return;
    }
     for (int i = dl-1; i >= 0; i--) {
         memcpy(map[i+1],map[i],N*sizeof(char));
    }
    moveMap();
}

//-------Bitmap----///
void draw_string_bitmap(void *font, const char* string)
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}
void draw_string(void *font, const char* string)
{
  while(*string)
    glutStrokeCharacter(font, *string++);
}


char *addStr(char str1[], char str2[]) {
    char *str = (char *) malloc(256);
    int i,j;
    for (i = 0; i < 128; i++) {
        if (str1[i] == '\0') break;
        str[i] = str1[i];
    }
    for (j = 0; j < 128; j++) {
        if (str2[j] == '\0') break;
        str[i+j] = str2[j];
    }
    str[i+j] = '\0';
    realloc(str,(i+j+1));
    return str;
}

void printfMe(float x, float y, char* string) {
glLineWidth(1.0f);

  glPushMatrix();
  glTranslatef(x,y, 0);
  glScalef(0.3f, 0.3f, 1.0f);
  draw_string(GLUT_STROKE_ROMAN, string);
  glPopMatrix();
}

void printfBmp(float x, float y, char* string) {
glLineWidth(1.0f);

glRasterPos2f(x, y);
  draw_string_bitmap(GLUT_BITMAP_HELVETICA_18, string);
  glFinish();

}

void getScoreText() {
    int d = 0;
    long sc = score;
    while (sc != 0) {
        realloc(scoreText,(d+1)*sizeof(char));
        scoreText[d] = '0' + (sc % 10);
        sc /= 10;
        d++;
    }
    for (int i = 0; i < (d/2); i++) {
        char buf = scoreText[i];
        scoreText[i] = scoreText[d-1-i];
        scoreText[d-1-i] = buf;
    }
}

void drawMap() {

    float quadSize = ((float)(height-75-30-15) / (float)M);

  float sX = width/2 - quadSize*(N/2);
  float sY = height - quadSize - 75-30;

  glColor4f(1.0, 1.0, 1.0, 0.7);
  printfMe(sX-75,sY+45, "Welcome to Tetris CC!");

  getScoreText();
  char *zero = "0";
  printfMe(sX+quadSize*N+30,sY - 30, addStr("SCORE: ",(score>0?scoreText:zero)));
  if (gameOver) {
  printfMe(sX-75,sY-45-45, "GAME OVER!");
    return;
  }

    /*printf("..::TetrisCC::..::ver..::..alpha::..\n");
    printf("::::::::::::::::::::::::::::::::::::\n");
    printf("SCORE: %d\n");
    printf("::::::::::::::::::::::::::::::::::::\n");
    printf("|----------------------------------|\n");*/
    char **dm = (char**) malloc(M*sizeof(char*));
    for (int i = 0; i < M; i++) {
        dm[i] = (char*) malloc(N*sizeof(char));
        memcpy(dm[i],map[i],N*sizeof(char));
    }

    for (int i = 0; i < (cfig->n); i++)
        dm[cfig->y-cfig->cube[i].y][cfig->x+cfig->cube[i].x]= currentColor;

            glColor4f(1.0, 1.0, 1.0, 0.1);
            glBegin(GL_QUADS);
            glVertex2f(sX,sY );
            glVertex2f(sX+(N)*quadSize,sY);
            glVertex2f(sX+(N)*quadSize, sY-((M-tabTop)*quadSize));
            glVertex2f(sX, sY-((M-tabTop)*quadSize));
            glEnd();

    for (int i = tabTop; i < M; i++) {
        for (int j = 0; j < N; j++) {
            //printf("%c",dm[i][j]);

            if (dm[i][j] > 0) {

            if (dm[i][j] == 1)
            glColor4f(1.0, 1.0, 1.0, 0.3);
            if (dm[i][j] == 2)
            glColor4f(1.0, 0.0, 0.0, 0.3);
            if (dm[i][j] == 3)
            glColor4f(0.0, 1.0, 0.0, 0.3);
            if (dm[i][j] == 4)
            glColor4f(0.0, 0.0, 1.0, 0.3);
            glBegin(GL_QUADS);
            glVertex2f(sX+j*quadSize,sY-((i-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize,sY-((i-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize, sY-((i+1-tabTop)*quadSize));
            glVertex2f(sX+j*quadSize, sY-((i+1-tabTop)*quadSize));
            glEnd();

            glColor4f(1.0, 1.0, 1.0, 0.5);
            glBegin(GL_LINES);
            glVertex2f(sX+j*quadSize,sY-((i-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize,sY-((i-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize,sY-((i-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize, sY-((i+1-tabTop)*quadSize));
            glVertex2f(sX+(j+1)*quadSize, sY-((i+1-tabTop)*quadSize));
            glVertex2f(sX+j*quadSize, sY-((i+1-tabTop)*quadSize));
            glVertex2f(sX+j*quadSize, sY-((i+1-tabTop)*quadSize));
            glVertex2f(sX+j*quadSize,sY-((i-tabTop)*quadSize));
            glEnd();
            }
        }
        //printf("|\n");
    }
    //printf("|----------------------------------|\n");
    delete dm;
}

void replay() {
for (int i = 0; i < M;i++) {
for (int j = 0; j < N; j++)
map[i][j] = 0;
}
score = 0;
gameOver = 0;
createFigure(cfig);
}

void initGame() {
    srand(time(NULL));
    map = (char**)malloc(M*sizeof(char*));
    for (int i = 0; i < M;i++) {
        map[i] = (char*)malloc(N*sizeof(char));
        for (int j = 0; j < N; j++)
            map[i][j] = 0;
    }
    memcpy(cfig,&triangle,sizeof(Figure));
    createFigure(cfig);
}




/* GLUT callback Handlers */
//------Imput handler----//
static void key(unsigned char key, int x, int y)
{
    //----ESC Handler---//
    if (key == 27) {

    }
    //----BACKSPACE Handler---//

    //----ENTER Handler---//
    if (key == 13) {
       replay();
    }

    if (gameOver) {
            glutPostRedisplay();
        return;
    }
    /*if (key == 'w')
            moveFigure(cfig,{0,-1});*/
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
        if (key == 'v')
            createFigure(cfig);

        if (!moveFigure(cfig,{0,1})) {
            doneFigure(cfig);
            moveMap();
        }
        else {
            moveFigure(cfig,{0,-1});
        }


    //----QUIT Handler----//
    if (key == 'q') {
        exit(0);
    }
    glutPostRedisplay();
}

static void specialKeys(int key, int x, int y ) {
//----Cursor Handler----//
    /*if (key == GLUT_KEY_LEFT) {
       if (checkStep(-step,0))
        cursor->x -= step;
    }
    if (key == GLUT_KEY_UP) {
       if (checkStep(0,step))
        cursor->y += step;
    }
    if (key == GLUT_KEY_RIGHT) {
       if (checkStep(step,0))
        cursor->x += step;
    }
    if (key == GLUT_KEY_DOWN) {
       if (checkStep(0,-step))
        cursor->y -= step;
    }*/
glutPostRedisplay();
}

static void idle(void)
{

    float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
deltaTime = timeSinceStart - oldTimeSinceStart;
oldTimeSinceStart = timeSinceStart;

deltaTime /= 1000;

if (!gameOver) {
timer += deltaTime;
if (timer >= 0.25) {
    timer = 0;
    moveFigure(cfig,{0,1});
    if (!moveFigure(cfig,{0,1})) {
            doneFigure(cfig);
            moveMap();
        }
        else {
            moveFigure(cfig,{0,-1});
        }
}
}
    glutPostRedisplay();
}

//-----Draw-----//
static void display(void)
{


glClearColor(0.0,0.0,0.0,1.0);
glClear(GL_COLOR_BUFFER_BIT);
        //-----Draw ----//
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        drawMap();


    glutSwapBuffers();
}

//-----Camera settings---//
void reshape(int w, int h)
{
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        width = w;
        height = h;
}

//----Init OpenGL-----//
void initGL(int argc, char *argv[])
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(width, height);
        glutCreateWindow("Tetris CC");
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutSpecialFunc(specialKeys);
        glutKeyboardFunc(key);
        glutIdleFunc(idle);
        glEnable(GL_POINT_SMOOTH);
        glutMainLoop();
}


int main(int argc, char *argv[])
{
    initGame();
    //rotateFigure(cfig);
    //drawMap();

   //---Init Graphics core-----//
    initGL(argc, argv);
    return 0;
}
