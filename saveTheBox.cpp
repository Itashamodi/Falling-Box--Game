#include <GL/glut.h>
#include <bits/stdc++.h>
#include <unistd.h>
#define width	600
#define height  400

using namespace std;

vector<int> xval, yval;
int x,y;
int ballSize=20;
int fallRate=1;
int score = 0;
int lastfall =0;
int interval = 130;
bool play = true;

void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
}

void reset();

void keyboard(unsigned char key, int mousex, int mousey)
{
	switch(key)
	{
		case	'f': x = max(0,x-20);	break;
		case	'j': x = min(width, x+20); 	break;
		case	'F': x = max(0,x-20);	break;
		case	'J': x = min(width,x+20);	break;
		case	'N': reset();	break;
		case	'n': reset();	break;
		case	'e':	exit(0);	break;
		case	'E':	exit(0);	break;
		default:	break;	
	}
	glutPostRedisplay();
}

void drawball(int x,int y)
{
	glColor3f(1.0,0.0,0.0);
	glRecti(x,y,x+ballSize,y+ballSize);
}
	
bool LostGame(int x,int y)
{
	if(y > height || y+20<0)	return true;
	return false;
}

void draw()
{
	glClearColor(255.0/255,255.0/255,0/255,0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,1.0,0.0);

	for(int i=0; i<yval.size(); i++)
	{
		glRecti(xval[i],yval[i],xval[i]+200,yval[i]+20);
	}

	drawball(x,y);
	glFlush();
}

void displayText(int x, int y, string text)
{
	glRasterPos2f(x,y);
	for(auto c:text)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,c);
	glFlush();
}

void displayScore()
{
	string text = "Score: "+to_string(score);
	glColor3f(0.0,0.0,1.0);
	displayText(40,40,text);
	displayText(5,5,"Press 'E' to exit");
}

void display()
{
	if(xval.empty())
		reset();
	if(play)
	{
		glClearColor(1.0,1.0,1.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT);	

		displayScore();
		draw();
		displayScore();
		int n= height/interval;

		bool land = false;
		//	detect if the ball landed somewhere
		for(int i=0; i<n; i++)
		{
			if((x+ballSize>=xval[i]) && (x<=xval[i]+200) && (y==yval[i]+20))
			{
				y = (y+1)%height;
				if(lastfall)	score++;
				lastfall = 0;
				land = true;	break;
			}
		}
		
		//increase the coordinates of the platforms

		for(int i=0; i<n; i++)
		{
			yval[i] = (yval[i]+1)%height;
			if(yval[i] == 0)
				xval[i] = rand()%(width-300);
		}
		
		if(land == false)
		{
			y-=fallRate;
			lastfall = 1;
		}
	
//		usleep(10);
		glutPostRedisplay();
//		display();
	}
	if(LostGame(x,y))
	{
		string text = "GAME OVER";
		displayText(width/2-100, 2*height/3, text);
		text= "Your Score: "+to_string(score);
		displayText(width/2-100, 2*height/3-50, text);
		text ="Press 'N' to start a new game\n";
		displayText(width/2-100, 2*height/3-100, text);
		play=false;
	}
	glutSwapBuffers();
}

void reset()
{	
	xval.clear();
	yval.clear();
	int n = height/interval;
	for(int i=0; i<n; i++)
	{
		xval.push_back(rand()%(width-300));
		yval.push_back(interval*i);
	}
	x = xval[0]+98;
	y = yval[0]+20;
	score = 0;
	play = true;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Save The Box");
	glutDisplayFunc(display);
	gluOrtho2D(0,600,0,400);
	init();
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
