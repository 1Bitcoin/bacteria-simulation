#include <glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>

const int OFFSET = 15;
const int TABLE = 20;
const float eatCoef = 1.2;
const float healthCoef = 0.1;
const float percentSave = 0.2;
const int damageCoeff = 1;

int numberHarmlessbacterium = 1;
int numberAgressivebacterium = 1;

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
	int genome; //to upgrade
	int age = 0;
};

struct UnitField
{
	float x;
	float y;
	int food;
	int fraction;
	int numberFiled;
};

Bacterium arrrayHarmless[400];
Bacterium arrrayAgressive[400];
UnitField arrayFoodfield[TABLE * TABLE];

void initField()
{
	int numberFiled = 0;
	UnitField cell;

	srand(time(NULL)); // seed for rand ()

	for (int y = 0; y < TABLE; y++)
	{
		for (int x = 0; x < TABLE; x++)
		{
			cell.x = (x + 1) * (OFFSET);
			cell.y = (y + 1) * (OFFSET);
			cell.food = rand() % 9;
			cell.numberFiled = numberFiled;

			if (!x && !y) // first bacterium
				cell.fraction = 1;
			else
				cell.fraction = 0;

			arrayFoodfield[numberFiled] = cell;
			numberFiled++;
		}
	}
}

int randomChangegenom(Bacterium &bacterium, int index)
{
	int randInt = rand() % 100;
	int signChange = 0;

	if (randInt > 98 && bacterium.fraction != 2)
	{
		bacterium.damage += 0.5 * bacterium.food;
		bacterium.food = 0;
		bacterium.health = 1;
		bacterium.genome = 1;
		bacterium.fraction = 2;

		int numberFiled = bacterium.numberFiled;

		arrayFoodfield[numberFiled].fraction = bacterium.fraction;
		arrrayAgressive[numberAgressivebacterium] = bacterium;
		arrrayHarmless[index].health = 0;

		numberAgressivebacterium++;

		signChange = 1;
	}
	else if (randInt < 20 && bacterium.fraction != 1)
	{
		bacterium.damage = 1;
		bacterium.food = 0;
		bacterium.health = 1;
		bacterium.genome = 0;
		bacterium.fraction = 1;

		int numberFiled = bacterium.numberFiled;

		arrayFoodfield[numberFiled].fraction = bacterium.fraction;
		arrrayHarmless[numberHarmlessbacterium] = bacterium;
		arrrayAgressive[index].health = 0;

		numberHarmlessbacterium++;

		signChange = 1;
	}
	return signChange;
}

void toAttack(Bacterium &agressiveBacterium)
{
	for (int i = 0; i < numberHarmlessbacterium; i++)
	{
		if (arrrayHarmless[i].x == agressiveBacterium.x + OFFSET && arrrayHarmless[i].y == agressiveBacterium.y && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
		{
			arrrayHarmless[i].health -= agressiveBacterium.damage;
			if (arrrayHarmless[i].health <= 0)
			{
				agressiveBacterium.food += arrrayHarmless[i].food;
				arrayFoodfield[arrrayHarmless[i].numberFiled].fraction = 0;
			}
		}
		else if (arrrayHarmless[i].x == agressiveBacterium.x - OFFSET && arrrayHarmless[i].y == agressiveBacterium.y && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
		{
			arrrayHarmless[i].health -= agressiveBacterium.damage;
			if (arrrayHarmless[i].health <= 0)
			{
				agressiveBacterium.food += arrrayHarmless[i].food;
				arrayFoodfield[arrrayHarmless[i].numberFiled].fraction = 0;
			}
		}
		else if (arrrayHarmless[i].x == agressiveBacterium.x && arrrayHarmless[i].y == agressiveBacterium.y + OFFSET && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
		{
			arrrayHarmless[i].health -= agressiveBacterium.damage;
			if (arrrayHarmless[i].health <= 0)
			{
				agressiveBacterium.food += arrrayHarmless[i].food;
				arrayFoodfield[arrrayHarmless[i].numberFiled].fraction = 0;
			}
		}
		else if (arrrayHarmless[i].x == agressiveBacterium.x && arrrayHarmless[i].y == agressiveBacterium.y - OFFSET && agressiveBacterium.fraction != arrrayHarmless[i].fraction)
		{
			arrrayHarmless[i].health -= agressiveBacterium.damage;
			if (arrrayHarmless[i].health <= 0)
			{
				agressiveBacterium.food += arrrayHarmless[i].food;
				arrayFoodfield[arrrayHarmless[i].numberFiled].fraction = 0;
			}
		}
	}
}

void upgradeDamage(Bacterium &bacterium)
{
	if (bacterium.health > 0) // if bacterium live
		bacterium.damage += bacterium.food * damageCoeff;
}

void dead(Bacterium &bacterium)
{
	bacterium.age += 1;

	if (bacterium.age == 50)
	{
		arrayFoodfield[bacterium.numberFiled].fraction = 0;
		arrayFoodfield[bacterium.numberFiled].food += bacterium.food * percentSave;
		bacterium.health = 0;
	}
}

void changeFood(Bacterium &bacterium, UnitField produceCell)
{
	if (bacterium.food < 5)
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

	produceCell.food = 0;
	produceCell.x = 0;
	produceCell.y = 0;

	for (int i = 0; i < TABLE * TABLE && condition != 4; i++) // optim lower code
	{
		if (arrayFoodfield[i].x == parentBacterium.x + OFFSET && arrayFoodfield[i].y == parentBacterium.y && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
		{
			produceCell = arrayFoodfield[i];
			condition++;
		}
		else if (arrayFoodfield[i].x == parentBacterium.x - OFFSET && arrayFoodfield[i].y == parentBacterium.y && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
		{
			produceCell = arrayFoodfield[i];
			condition++;;
		}
		else if (arrayFoodfield[i].x == parentBacterium.x && arrayFoodfield[i].y == parentBacterium.y + OFFSET && arrayFoodfield[i].food >= produceCell.food && arrayFoodfield[i].fraction == 0)
		{
			produceCell = arrayFoodfield[i];
			condition++;
		}
		else if (arrayFoodfield[i].x == parentBacterium.x && arrayFoodfield[i].y == parentBacterium.y - OFFSET && arrayFoodfield[i].food >= produceCell.food&& arrayFoodfield[i].fraction == 0)
		{
			produceCell = arrayFoodfield[i];
			condition++;
		}
	}

	if (condition)
	{
		childBacterium.x = produceCell.x; // in fucn
		childBacterium.y = produceCell.y;

		childBacterium.damage = parentBacterium.damage;
		childBacterium.food = 1;
		childBacterium.health = 1;

		childBacterium.numberFiled = produceCell.numberFiled;
		childBacterium.genome = parentBacterium.genome;
		childBacterium.fraction = parentBacterium.fraction;

		arrayFoodfield[childBacterium.numberFiled].fraction = childBacterium.fraction;

		if (!childBacterium.genome)
		{
			arrrayHarmless[numberHarmlessbacterium] = childBacterium;
			numberHarmlessbacterium++;
		}
		else if (childBacterium.genome == 1)
		{
			arrrayAgressive[numberAgressivebacterium] = childBacterium;
			numberAgressivebacterium++;
		}
	}
	return condition;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < numberHarmlessbacterium; i++)
	{
		changeFood(arrrayHarmless[i], arrayFoodfield[arrrayHarmless[i].numberFiled]);
		repairHealth(arrrayHarmless[i]);
		//int condition = randomChangegenom(arrrayHarmless[i], i);
		dead(arrrayHarmless[i]);
		upgradeDamage(arrrayHarmless[i]);

		if (arrrayHarmless[i].health > 0) // make try delete from vector or change this on array
		{
			glColor3f(0, arrrayHarmless[i].health * 0.15, 0);

			if (0)
			{
				glColor3f(arrrayHarmless[i].health * 0.15, arrrayHarmless[i].health * 0.15, 0);
			}

			glRectf(arrrayHarmless[i].x - sqrt(200) / 2, arrrayHarmless[i].y + sqrt(200) / 2,
				arrrayHarmless[i].x + sqrt(200) / 2, arrrayHarmless[i].y - sqrt(200) / 2); // optim offset
		}
	}

	for (int i = 0; i < numberAgressivebacterium; i++)
	{
		changeFood(arrrayAgressive[i], arrayFoodfield[arrrayAgressive[i].numberFiled]);
		repairHealth(arrrayAgressive[i]);
		//int condition = randomChangegenom(arrrayAgressive[i], i);
		dead(arrrayAgressive[i]);
		upgradeDamage(arrrayAgressive[i]);

		if (arrrayAgressive[i].health > 0) // make try delete from vector or change this on array
		{
			glColor3f(arrrayAgressive[i].health * 0.15, 0, 0);

			if (0)
			{
				glColor3f(arrrayAgressive[i].health * 0.15, arrrayAgressive[i].health * 0.15, 0);
			}

			glRectf(arrrayAgressive[i].x - sqrt(200) / 2, arrrayAgressive[i].y + sqrt(200) / 2,
				arrrayAgressive[i].x + sqrt(200) / 2, arrrayAgressive[i].y - sqrt(200) / 2); // optim offset

			toAttack(arrrayAgressive[i]); // here problem
		}
	}

	int flagHarmless = 0;
	int flagAgressive = 0;

	//produce agressive
	if (!produceBacterium(arrrayAgressive[numberAgressivebacterium - 1]) && !flagAgressive)
	{
		produceBacterium(arrrayAgressive[rand() % numberAgressivebacterium]);
		flagAgressive = 1;
	}
	else
		produceBacterium(arrrayAgressive[rand() % numberAgressivebacterium]);

	//produce harmless
	if (!produceBacterium(arrrayHarmless[numberHarmlessbacterium - 1]) && !flagHarmless)
	{
		produceBacterium(arrrayHarmless[rand() % numberHarmlessbacterium]);
		flagHarmless = 1;
	}
	else
		produceBacterium(arrrayHarmless[rand() % numberHarmlessbacterium]);

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
	Bacterium agressive;

	harmless.x = OFFSET;
	harmless.y = OFFSET;

	harmless.food = 1;
	harmless.damage = 1;
	harmless.health = 1;

	harmless.fraction = 1;
	harmless.numberFiled = 0;
	harmless.genome = 0;

	agressive.x = 20 * OFFSET;
	agressive.y = 20 * OFFSET;

	agressive.food = 1;
	agressive.damage = 15;
	agressive.health = 1;

	agressive.fraction = 2;
	agressive.numberFiled = TABLE * TABLE - 1; // ксотыль для поля 20 на 20
	agressive.genome = 1;

	arrrayAgressive[numberAgressivebacterium - 1] = agressive;
	arrrayHarmless[numberHarmlessbacterium - 1] = harmless;
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