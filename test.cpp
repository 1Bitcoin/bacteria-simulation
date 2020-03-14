#include <glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>

const int OFFSET = 15;
const int TABLE = 20;
const float eatCoef = 0.2;
const float healthCoef = 0.1;

int numberBacterium = 1;

using namespace std;

struct Bacterium
{
    float x = 0;
    float y = 0;
    float food;
    float damage;
    int health;
    int fraction;
    int numberFiled;
    int genome = 0; //to upgrade
};

struct UnitField
{
    float x;
    float y;
    int food;
    int fraction;
    int numberFiled;
};

vector<Bacterium> arrrayHarmless;
vector<UnitField> arrayFoodfield;

void initField()
{
    UnitField cell;

    for (int y = 0; y < TABLE; y++)
    {
        for (int x = 0; x < TABLE; x++)
        {
            cell.x = (x + 1) * (OFFSET);
            cell.y = (y + 1) * (OFFSET);
            cell.food = rand() % 9;

            if (!x && !y) // first bacterium
                cell.fraction = 1;
            else
                cell.fraction = 0;

            arrayFoodfield.push_back(cell); // optim
        }
    }
}

void randomChangegenom(Bacterium &bacterium)
{
    int randInt = rand() % 100;

    if (randInt > 98)
    {
        bacterium.damage += 0.5 * bacterium.food;
        bacterium.food = 0;
        bacterium.genome = 1;
        bacterium.fraction = 2;
    }

}

void toAttack(Bacterium &agressiveBacterium)
{
    for (int i = 0; i < numberBacterium; i++)
    {
        if (arrrayHarmless[i].x == agressiveBacterium.x + OFFSET && arrrayHarmless[i].y == agressiveBacterium.y && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
        {
            arrrayHarmless[i].health -= agressiveBacterium.damage;
            if (arrrayHarmless[i].health <= 0)
            {
                arrayFoodfield[arrrayHarmless[i].numberFiled].numberFiled = 0;
            }
        }
        else if (arrrayHarmless[i].x == agressiveBacterium.x - OFFSET && arrrayHarmless[i].y == agressiveBacterium.y && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
        {
            arrrayHarmless[i].health -= agressiveBacterium.damage;
            if (arrrayHarmless[i].health <= 0)
            {
                arrayFoodfield[arrrayHarmless[i].numberFiled].numberFiled = 0;
            }
        }
        else if (arrrayHarmless[i].x == agressiveBacterium.x && arrrayHarmless[i].y == agressiveBacterium.y + OFFSET && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
        {
            arrrayHarmless[i].health -= agressiveBacterium.damage;
            if (arrrayHarmless[i].health <= 0)
            {
                arrayFoodfield[arrrayHarmless[i].numberFiled].numberFiled = 0;
            }
        }
        else if (arrrayHarmless[i].x == agressiveBacterium.x && arrrayHarmless[i].y == agressiveBacterium.y - OFFSET && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
        {
            arrrayHarmless[i].health -= agressiveBacterium.damage;
            if (arrrayHarmless[i].health <= 0)
            {
                arrayFoodfield[arrrayHarmless[i].numberFiled].numberFiled = 0;
            }
        }
    }
}

void changeFood(Bacterium &bacterium, UnitField produceCell)
{
    bacterium.food += produceCell.food * eatCoef;
}

void repairHealth(Bacterium &bacterium)
{
    if (bacterium.health < 100)
        bacterium.health += bacterium.food * healthCoef;
}

int produceBacterium(Bacterium parentBacterium)
{
    Bacterium childBacterium;
    UnitField produceCell;

    int condition = 0;
    int numberFiled = 0;

    produceCell.food = 0;
    produceCell.x = 0;
    produceCell.y = 0;

    for (int i = 0; i < TABLE * TABLE && condition != 4; i++) // optim lower code
    {
        if (arrayFoodfield[i].x == parentBacterium.x + OFFSET && arrayFoodfield[i].y == parentBacterium.y && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
        {
            produceCell = arrayFoodfield[i];
            condition++;
            numberFiled = i;
        }
        else if (arrayFoodfield[i].x == parentBacterium.x - OFFSET && arrayFoodfield[i].y == parentBacterium.y && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
        {
            produceCell = arrayFoodfield[i];
            condition++;
            numberFiled = i;
        }
        else if (arrayFoodfield[i].x == parentBacterium.x && arrayFoodfield[i].y == parentBacterium.y + OFFSET && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
        {
            produceCell = arrayFoodfield[i];
            condition++;
            numberFiled = i;
        }
        else if (arrayFoodfield[i].x == parentBacterium.x && arrayFoodfield[i].y == parentBacterium.y - OFFSET && arrayFoodfield[i].food >= produceCell.food&& arrayFoodfield[i].fraction == 0)
        {
            produceCell = arrayFoodfield[i];
            condition++;
            numberFiled = i;
        }
    }

    if (condition)
    {
        childBacterium.x = produceCell.x; // in fucn
        childBacterium.y = produceCell.y;
        childBacterium.damage = 1;
        childBacterium.food = 1;
        childBacterium.health = 1;
        childBacterium.numberFiled = numberFiled;
        childBacterium.genome = parentBacterium.genome;
        childBacterium.fraction = parentBacterium.fraction;

        arrayFoodfield[numberFiled].fraction = parentBacterium.fraction;
        numberBacterium++;

        arrrayHarmless.push_back(childBacterium);
    }
    return condition;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < arrrayHarmless.size(); i++)
    {
        changeFood(arrrayHarmless[i], arrayFoodfield[arrrayHarmless[i].numberFiled]);
        repairHealth(arrrayHarmless[i]);
        randomChangegenom(arrrayHarmless[i]);

        if (arrrayHarmless[i].health > 0) // make try delete from vector or change this on array
        {
            if (!arrrayHarmless[i].genome)
                glColor3f(0, arrrayHarmless[i].health * 0.15, 0);
            else
            {
                glColor3f(arrrayHarmless[i].health * 0.2, 0, 0);
                if (!arrrayHarmless[i].food)
                    glColor3f(arrrayHarmless[i].health * 0.15, arrrayHarmless[i].health * 0.15, 0);
            }

            glRectf(arrrayHarmless[i].x - sqrt(200) / 2, arrrayHarmless[i].y + sqrt(200) / 2,
                arrrayHarmless[i].x + sqrt(200) / 2, arrrayHarmless[i].y - sqrt(200) / 2); // optim offset
        }
        if (arrrayHarmless[i].genome)
            toAttack(arrrayHarmless[i]);
    }

    int flag = 0;

    if (!produceBacterium(arrrayHarmless.back()) && !flag)
    {
        produceBacterium(arrrayHarmless[rand() % numberBacterium]);
        flag = 1;
    }
    else
        produceBacterium(arrrayHarmless[rand() % numberBacterium]);


    glEnd();
    glutSwapBuffers();
}

void timer(int = 0)
{
    display();
    glutTimerFunc(500, timer, 0);
}

void initFirstBacterium(void)
{
    Bacterium harmless;

    harmless.x = OFFSET;
    harmless.y = OFFSET;
    harmless.fraction = 1;
    harmless.food = 1;
    harmless.damage = 1;
    harmless.health = 1;
    harmless.numberFiled = 0;

    arrrayHarmless.push_back(harmless);
}

int main(int argc, char** argv)
{
    initField();
    initFirstBacterium();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Biology");
    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 300, 300, 0, -1, 1);
    glutDisplayFunc(display);
    timer();
    glutMainLoop();
}
