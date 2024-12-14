#include <freeglut.h>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

vector<Point> currentCircle;

bool isJustified = false;
bool isboundaryFillSelected = false;
bool isfloodFillSelected = false;
bool iscircle1Selected = false;
bool iscircle2Selected = false;


struct Checkbox {
    int x, y, width, height;
    bool checked;
    const char* label;

    Checkbox(int x, int y, int width, int height, const char* label)
        : x(x), y(y), width(width), height(height), checked(false), label(label) {}
};

vector<Checkbox> checkboxes;


void renderBitmapString(float x, float y, void* font, const char* string) {
    glColor3f(0.0, 0.0, 0.0);
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void drawCheckbox(const Checkbox& checkbox) {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);


    glVertex2i(checkbox.x, checkbox.y);
    glVertex2i(checkbox.x + checkbox.width, checkbox.y);
    glVertex2i(checkbox.x + checkbox.width, checkbox.y + checkbox.height);
    glVertex2i(checkbox.x, checkbox.y + checkbox.height);

    glEnd();

    if (checkbox.checked) {

        glBegin(GL_QUADS);
        glColor3f(0.0, 0.3, 1.0);  
        glVertex2i(checkbox.x, checkbox.y);
        glVertex2i(checkbox.x + checkbox.width, checkbox.y);
        glVertex2i(checkbox.x + checkbox.width, checkbox.y + checkbox.height);
        glVertex2i(checkbox.x, checkbox.y + checkbox.height);
        glEnd();

    }

    renderBitmapString(checkbox.x + checkbox.width + 10, checkbox.y + 5, GLUT_BITMAP_HELVETICA_12, checkbox.label);
}


bool isPointInsideCheckbox(int x, int y, const Checkbox& checkbox) {

    return (x >= checkbox.x && x <= checkbox.x + checkbox.width && y >= checkbox.y && y <= checkbox.y + checkbox.height);

}

void setPixel(int x, int y, float r, float g, float b) {
    glBegin(GL_POINTS);
    glColor3f(r, g, b);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float* color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void boundaryFill(int x, int y, float* fillColor, float* boundaryColor) {
    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if ((color[0] != boundaryColor[0] || color[1] != boundaryColor[1] || color[2] != boundaryColor[2]) &&
            (color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {



            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);


            points.push(Point(p.x + 1, p.y));
            points.push(Point(p.x - 1, p.y));
            points.push(Point(p.x, p.y - 1));
            points.push(Point(p.x, p.y + 1));

        }
        
        
    }
}

void floodFill1(int x, int y, float* fillColor, float* defaultColor) {
    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if (color[0] == defaultColor[0] && color[1] == defaultColor[1] && color[2] == defaultColor[2]) {
            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);
            points.push(Point(p.x + 1, p.y));
            points.push(Point(p.x - 1, p.y));
            points.push(Point(p.x, p.y + 1));
            points.push(Point(p.x, p.y - 1));
        }
    }
}

void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        setPixel(xc + x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc + x, yc - y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc - y, 1.0, 0.0, 0.0);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void drawCircle2(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        setPixel(xc + x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc + x, 0.0, 0.0, 0.0);
        setPixel(xc + y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc - x, 0.0, 0.0, 0.0);
        setPixel(xc + x, yc - y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc - y, 1.0, 0.0, 0.0);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    
    for (const auto& checkbox : checkboxes) {
        drawCheckbox(checkbox);
    }

    
    if (iscircle1Selected) {
        
        drawCircle(200, 400, 50);

        float fillColor[3] = { 1.0, 1.0, 0.0 };
        float boundaryColor[3] = { 1.0, 0.0, 0.0 };

        if (isboundaryFillSelected) {
            boundaryFill(200, 400, fillColor, boundaryColor);
        }
        if (isfloodFillSelected) {
            float defaultColor[3] = { 1.0, 1.0, 1.0 };


            floodFill1(200, 400, fillColor, defaultColor);
        }
    }

    if (iscircle2Selected) {

        drawCircle2(600, 400, 50);

        float fillColor[3] = { 1.0, 1.0, 0.0 };
        float boundaryColor[3] = { 1.0, 0.0, 0.0 };

        if (isboundaryFillSelected) {


            boundaryFill(600, 400, fillColor, boundaryColor);
        }
        if (isfloodFillSelected) {
            
            float defaultColor[3] = { 1.0, 1.0, 1.0 };

            floodFill1(600, 400, fillColor, defaultColor);
        }
    }

    renderBitmapString(340, 770, GLUT_BITMAP_HELVETICA_18, "Filling Algorithms");
    renderBitmapString(550, 20, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");


    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int correctedY = windowHeight - y;

        
        for (auto& checkbox : checkboxes) {
            if (isPointInsideCheckbox(x, correctedY, checkbox)) {

                checkbox.checked = !checkbox.checked;
                if (checkbox.label == "Boundary Filled") isboundaryFillSelected = checkbox.checked;
                else if (checkbox.label == "Flood Filled") isfloodFillSelected = checkbox.checked;
                else if (checkbox.label == "Circle 1") iscircle1Selected = checkbox.checked;
                else if (checkbox.label == "Circle 2") iscircle2Selected = checkbox.checked;


            }
        }
    }
    glutPostRedisplay();
}




void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);

    
    checkboxes.push_back(Checkbox(660, 750, 15, 15, "Boundary Filled"));
    checkboxes.push_back(Checkbox(660, 725, 15, 15, "Flood Filled"));
    checkboxes.push_back(Checkbox(660, 700, 15, 15, "Circle 1"));
    checkboxes.push_back(Checkbox(660, 675, 15, 15, "Circle 2"));

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Filling Algorithms");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}










/*
#include <freeglut.h>
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

vector<Point> currentCircle;

bool isJustified = false;
bool isboundaryFillSelected = false;
bool isfloodFillSelected = false;
bool iscircle1Selected = false;
bool iscircle2Selected = false;

bool isCheckedClipped1 = false;
bool isCheckedViewport1 = false;
bool isCheckedViewport2 = false;

vector<Point> currentLine2;
vector<vector<Point>> lines2;

bool isCheckedClipped2 = false;

void renderBitmapString(float x, float y, void* font, const char* string) {
    glColor3f(0.0, 0.0, 0.0);
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void setPixel(int x, int y, float r, float g, float b) {
    glBegin(GL_POINTS);
    glColor3f(r, g, b);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float* color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void boundaryFill(int x, int y, float* fillColor, float* boundaryColor) {
    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if ((color[0] != boundaryColor[0] || color[1] != boundaryColor[1] || color[2] != boundaryColor[2]) &&
            (color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {
            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);
            points.push(Point(p.x + 1, p.y));
            points.push(Point(p.x - 1, p.y));
            points.push(Point(p.x, p.y - 1));
            points.push(Point(p.x, p.y + 1));
        }
    }
}

void floodFill1(int x, int y, float* fillColor, float* defaultColor) {
    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if (color[0] == defaultColor[0] && color[1] == defaultColor[1] && color[2] == defaultColor[2]) {
            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);
            points.push(Point(p.x + 1, p.y));
            points.push(Point(p.x - 1, p.y));
            points.push(Point(p.x, p.y + 1));
            points.push(Point(p.x, p.y - 1));
        }
    }
}

void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        setPixel(xc + x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc + x, yc - y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc - y, 1.0, 0.0, 0.0);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void drawCircle2(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        setPixel(xc + x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc + y, 1.0, 0.0, 0.0);
        setPixel(xc + y, yc + x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc + x, 0.0, 0.0, 0.0);
        setPixel(xc + y, yc - x, 1.0, 0.0, 0.0);
        setPixel(xc - y, yc - x, 0.0, 0.0, 0.0);
        setPixel(xc + x, yc - y, 1.0, 0.0, 0.0);
        setPixel(xc - x, yc - y, 1.0, 0.0, 0.0);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glFlush();
}

void leftSubDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);


    
    // Draw checkboxes
    for (int i = 0; i < 3; ++i) {
        float y = 195.0f - i * 10.0f;
        bool isChecked = (i == 0) ? isboundaryFillSelected :
            (i == 1) ? iscircle1Selected :
            iscircle2Selected;

        glBegin(GL_QUADS);
        glColor3f(isChecked ? 0.0 : 1.0, isChecked ? 1.0 : 0.0, 0.0);
        glVertex2f(102.0f, y);
        glVertex2f(107.0f, y);
        glVertex2f(107.0f, y - 5.0f);
        glVertex2f(102.0f, y - 5.0f);
        glEnd();
    }
    
    if (iscircle1Selected) {
        drawCircle(0, 0, 50);

        float fillColor[3] = { 1.0, 1.0, 0.0 };
        float boundaryColor[3] = { 1.0, 0.0, 0.0 };

        if (isboundaryFillSelected) {
            boundaryFill(0, 0, fillColor, boundaryColor);
        }
    }

    if (iscircle2Selected) {
        drawCircle2(600, 400, 50);

        float fillColor[3] = { 1.0, 1.0, 0.0 };
        float boundaryColor[3] = { 1.0, 0.0, 0.0 };

        if (isboundaryFillSelected) {
            boundaryFill(600, 400, fillColor, boundaryColor);
        }
    }

    renderBitmapString(110, 190, GLUT_BITMAP_HELVETICA_12, "Boundary Fill");
    renderBitmapString(110, 180, GLUT_BITMAP_HELVETICA_12, "Circle 1");
    renderBitmapString(110, 170, GLUT_BITMAP_HELVETICA_12, "Circle 2");

    renderBitmapString(-45.0f, 190.0f, GLUT_BITMAP_HELVETICA_18, "Boundary Fill Algorithm");
    renderBitmapString(60.0f, -190.0f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");

    glFlush();
}

void rightSubDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw checkboxes
    for (int i = 0; i < 3; ++i) {
        float y = 195.0f - i * 10.0f;
        bool isChecked = (i == 0) ? isfloodFillSelected :
            (i == 1) ? iscircle1Selected :
            iscircle2Selected;

        glBegin(GL_QUADS);
        glColor3f(isChecked ? 0.0 : 1.0, isChecked ? 1.0 : 0.0, 0.0);
        glVertex2f(102.0f, y);
        glVertex2f(107.0f, y);
        glVertex2f(107.0f, y - 5.0f);
        glVertex2f(102.0f, y - 5.0f);
        glEnd();
    }

    renderBitmapString(110, 190, GLUT_BITMAP_HELVETICA_12, "Flood Fill");
    renderBitmapString(110, 180, GLUT_BITMAP_HELVETICA_12, "Circle 1");
    renderBitmapString(110, 170, GLUT_BITMAP_HELVETICA_12, "Circle 2");

    renderBitmapString(-30.0f, 190.0f, GLUT_BITMAP_HELVETICA_18, "Flood Fill Algorithm");
    renderBitmapString(60.0f, -190.0f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");

    glFlush();
}

void leftSubWindowMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        float x_pos = (x / (float)width) * 400.0f - 200.0f;
        float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;

        if (x_pos >= 102.0f && x_pos <= 107.0f) {
            if (y_pos >= 190.0f && y_pos <= 195.0f) {
                isboundaryFillSelected = !isboundaryFillSelected;
            }
            else if (y_pos >= 180.0f && y_pos <= 185.0f) {
                iscircle1Selected = !iscircle1Selected;
            }
            else if (y_pos >= 170.0f && y_pos <= 175.0f) {
                iscircle2Selected = !iscircle2Selected;
            }
        }

        glutPostRedisplay();
    }
}

void rightSubWindowMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        float x_pos = (x / (float)width) * 400.0f - 200.0f;
        float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;

        if (x_pos >= 102.0f && x_pos <= 107.0f) {
            if (y_pos >= 190.0f && y_pos <= 195.0f) {
                isfloodFillSelected = !isfloodFillSelected;
            }
            else if (y_pos >= 180.0f && y_pos <= 185.0f) {
                iscircle1Selected = !iscircle1Selected;
            }
            else if (y_pos >= 170.0f && y_pos <= 175.0f) {
                iscircle2Selected = !iscircle2Selected;
            }
        }

        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
    glMatrixMode(GL_MODELVIEW);
}

void init1() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
}

void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    glutInitWindowPosition(100, 100);
    int mainWindow = glutCreateWindow("Line Clipping");
    glutDisplayFunc(display);
    init();

    int leftSubWindow = glutCreateSubWindow(mainWindow, 0, 0, 800, 800);
    init1();
    glutDisplayFunc(leftSubDisplay);
    glutReshapeFunc(reshape);
    glutMouseFunc(leftSubWindowMouse);

    int rightSubWindow = glutCreateSubWindow(mainWindow, 800, 0, 800, 800);
    init();
    glutDisplayFunc(rightSubDisplay);
    glutReshapeFunc(reshape);
    glutMouseFunc(rightSubWindowMouse);

    glutMainLoop();
    return 0;
}

*/
























