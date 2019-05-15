#include <iostream>
#include <GL/freeglut.h>
#include "src/SOIL.h"
#include <string.h>
#include <stdio.h>

#define UP 1
#define DOWN 0
#define MAX 10
#define SIZE_MIS_X 55   //determines the size of missiles
#define SIZE_MIS_y 30   //determines the size of missiles
#define MAX_MISSILES 3  //maximum number of missiles in game
#define SPEED 1
using namespace std;
int frames=60;
//  The number of frames
int frameCount = 0;
//  Number of frames per second
float fps = 0;
//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;
//for choose scene
char scene[50]="res/war01_scene.jpg";
//my dup char holder
char imageloc[50];
//high scores
int max_dist, max_miss;
//flag to check if you were hit by a missile
int hit_missile=0;
//flad to determint the plane chosen by the user
int plane_choice=2;
//flag for setting
int setting=0;
void* currentfont;
float x_step=-171.0;    //for loading bar movement (in pg=0)
float y_cre=0;            //for credits text moveemnt
float y_pos=0;          //y axis position of plane
float theta=0;          //angle of the plane
bool state;             //state of plane (either going up or down)
bool local_screen = true;
int update_mis;
GLfloat x = 0.0f;       //background screen position
GLfloat fuel=98;        //fuel left in plane
GLfloat dist, missiles; //dustance travelled and missiled douged
GLfloat missile_x=250,missile_y[MAX_MISSILES]= {0};//position of missiles
int no_of_missiles=3;   //determines number of missiles in the game
int full = 1;
int i_bck,i_mis1,i_mis2,i_mis3,i_plane,i_inst21;
int i_cre22,i_sel31,i_sel32,i_0=0;
int j[10]={0};
int i_s,i;
GLfloat windowWidth;
GLfloat windowHeight;
GLuint tex_2d, tex_2d_mis[4], tex_2d_plane;
GLuint tex_2d_0, tex_2d_1, tex_2d_2, tex_2d_21, tex_2d_22, tex_2d_31, tex_2d_4, tex_2d_23;
//determines the current screen
int page=0;

void drawBitmap(float x,float y,void *f,const unsigned char* s) {
	int i;
	glRasterPos2f(x,y);
    glutBitmapString(f,s);
	printf("Bitmap drawing at %f,%f\n %s\r\n",x,y,s);
}

void drawStroke(const unsigned char* s,float tx,float ty,float tz,float sx,float sy,float sz) {
	glPushMatrix();
	glTranslatef(tx,ty,tz);
	glScalef(sx,sy,sz);
    glColor3f(1.0,1.0,1.0);
    glutStrokeString(GLUT_STROKE_ROMAN,s);
	glPopMatrix();
}

//class to draw the plane
class plane {
public:
	float x[MAX], y[MAX], i;
	float ymax, ymin;
	int button;
	plane()
	{
		x[1]=-30, x[2]=30, x[3]=30, x[4]=-30;
		y[1]=15, y[2]=15, y[3]=-15, y[4]=-15;
	}
	void draw_plane()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(plane_choice==2)
		{
			if (i_plane == 0)
			{
				tex_2d_plane = SOIL_load_OGL_texture
					       (
						       "res/plane2.png",
						       SOIL_LOAD_AUTO,
						       SOIL_CREATE_NEW_ID,
						       SOIL_FLAG_MULTIPLY_ALPHA
					       );
				i_plane = 1;
			}
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_plane);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(x[1], y[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(x[2], y[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(x[3], y[3]);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(x[4], y[4]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
} plane1;

//Navigation for esc and fullscreen
void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//27 is the ASCII value of the ESC key
	case 27:
		x=0.0;
		if(page!=2)
		{
			i_bck=0,i_mis1=0,i_mis2=0,i_mis3=0,i_plane=0,i_sel31=0,i_sel32=0,i_0=0;
			j[0]=j[1]=j[2]=j[3]=j[4]=j[5]=j[6]=j[7]=0;
			missiles=0;
			dist=0;
			setting=0;
			hit_missile=0;
			y_pos=0;
			missile_x=250;
			fuel=98;
			y_cre=0;
			x=0.0;
			cout<<full<<endl;
			cout<<x<<endl;
			page=2;
		}
		else
			//exit game
			exit(0);
		break;

	case 'f':       //full screen
		if (full == 0)
		{
			glutFullScreen();
			full = 1;
		}
		else
		{
			glutReshapeWindow(800, 450);
			glutPositionWindow(320,150);
			full = 0;
		}
	}
}

//Determines the action on mouse click event
void Mouse(int button, int m_state, int m_x, int m_y) {
	if(page==1) {
		if(m_x>720 && m_y>260 && m_state==GLUT_UP)
		{
			cout<<m_x<<" "<<m_y<<endl;
			if(local_screen==1)
				local_screen=0;
			else
				page=2;
		}
		
	}
	else if(page==2) {
		if(full==0) {
			if(m_state==GLUT_UP)
				cout<<m_x<<" "<<m_y<<endl;
			if(m_y>92 && m_y<116 && m_state==GLUT_UP) {
				cout<<"play"<<endl;
				//page=31;
				page=3;
			}
			if(m_y>158 && m_y<182 && m_state==GLUT_UP) {
				cout<<"Instructions"<<endl;
				page=21;
			}
			if(m_y>226 && m_y<256 && m_state==GLUT_UP) {
				cout<<"High"<<endl;
				page=23;
			}
			if(m_y>260 && m_state==GLUT_UP) {
				cout<<"exit"<<endl;
				exit(0);
			}
		}
		else {
			if(state==GLUT_UP)
				cout<<m_x<<" "<<m_y<<endl;
			if(m_y>154 && m_y<195 && m_state==GLUT_UP) {
				cout<<"play"<<endl;
				//page=31;
				page=3;
			}
			if(m_y>213 && m_y<251 && m_state==GLUT_UP) {
				cout<<"Settings"<<endl;
				page=31;
				setting=1;
			}
			if(m_y>269 && m_y<310 && m_state==GLUT_UP) {
				cout<<"Instructions"<<endl;
				page=21;
			}
			if(m_y>329 && m_y<366 && m_state==GLUT_UP) {
				cout<<"Credit"<<endl;
				page=22;
			}
			if(m_y>387 && m_y<425 && m_state==GLUT_UP) {
				cout<<"High"<<endl;
				page=23;
			}
			if(m_y>430 && m_state==GLUT_UP) {
				cout<<"exit"<<endl;
				exit(0);
			}

		}
	}
	else if(page==3) {
		if(button==GLUT_LEFT_BUTTON && m_state==GLUT_DOWN) {
			state=UP;
			cout<<"Going Up!"<<endl;
		}
		else if(button==GLUT_LEFT_BUTTON && m_state==GLUT_UP) {
			state=DOWN;
			cout<<"Going Down"<<endl;
		}
	}
}

//draw text in finish screen (page=4)
void draw_fin_text() {
	char string[6][40];
	int i,lengthOfString;

	strcpy(string[0],"WOW!!");
	sprintf(string[1],"You just travelled %d meters",(int)dist);
	sprintf(string[2],"and dodged %d missiles",(int)missiles);
	sprintf(string[3],"before you ran out of fuel!!");
	sprintf(string[4],"before you were hit by one!!");

	//update high scores;
	if((int)dist>max_dist)
	{
		max_dist=dist;
		cout<<max_dist<<" "<<dist<<endl;
	}
	if(missiles>max_miss)
	{
		max_miss=missiles;
		//cout<<max_miss<<endl;
	}
    
	glLineWidth(4);
    drawStroke(reinterpret_cast<const unsigned char *>(string[0]),-105,55,0,0.3,0.3,0.3);
	glLineWidth(3);
    drawStroke(reinterpret_cast<const unsigned char *>(string[1]),-155,35,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(string[2]),-155,20,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(hit_missile==0?string[3]:string[4]),-155,5,0,0.1,0.1,0.1);
}

void draw_high_text() {
	char string_high[10][20];
	int lengthOfString,i;

	strcpy(string_high[0],"High Scores: ");
	sprintf(string_high[1],"Distance: %d",max_dist);
	sprintf(string_high[2],"Missiles: %d",max_miss);

	glLineWidth(3);
    drawStroke(reinterpret_cast<const unsigned char *>(string_high[0]),0,30,0,0.2,0.2,0.2);

	glLineWidth(2);
    drawStroke(reinterpret_cast<const unsigned char *>(string_high[1]),70,0,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(string_high[2]),70,-15,0,0.1,0.1,0.1);
}

//draw text in page 2 (menu screen)
void draw_menu_text() {
	char string_menu[5][20];
	int lengthOfString,i;

	strcpy(string_menu[0],"Play!");
	strcpy(string_menu[1],"Instructions");
	strcpy(string_menu[2],"High Scores");
	strcpy(string_menu[3],"Exit..");

	glLineWidth(1);
    drawStroke(reinterpret_cast<const unsigned char *>(string_menu[0]),-120,50,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(string_menu[1]),-120,20,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(string_menu[2]),-120,-10,0,0.1,0.1,0.1);
    drawStroke(reinterpret_cast<const unsigned char *>(string_menu[3]),-120,-45,0,0.1,0.1,0.1);
}

//draw text in page 21 (instruction screen)
void draw_inst_text() {
	char string[5][120];
	int i,lengthOfString;

	strcpy(string[0],"Instructions:\n");
	strcpy(string[1],"The main objective of this game is to go as far\nas possible in your plane, without hitting the\nmissiles.\n\nPress right mouse button to increase altitude!\nLeaving it will automatically take you down.\nKeep a close eye on the fuel guage though :P\nHave FUN!!!");

	glLineWidth(3);
    drawStroke(reinterpret_cast<const unsigned char *>(string[0]),-40,50,0,0.3,0.3,0.3);
	glLineWidth(1);
	int y_pos_21=20;
	drawStroke(reinterpret_cast<const unsigned char *>(string[1]),-40,y_pos_21-10,0,0.06,0.06,0.06);
}

//print the score in page 3 (prints the game scores during game play)
void draw_score() {
	int length;
	char score_text[15];
	strcpy(score_text,"Distance: ");
	glLineWidth(1);
    drawStroke(reinterpret_cast<const unsigned char *>(score_text),85,82,0,0.08,0.08,0.08);

	char dist_text_val[15];
	sprintf(dist_text_val,"%d",(int)dist);
    drawStroke(reinterpret_cast<const unsigned char *>(dist_text_val),130,82,0,0.08,0.08,0.08);

	char missiles_text[15];
	strcpy(missiles_text,"Missiles: ");
    drawStroke(reinterpret_cast<const unsigned char *>(missiles_text),85,72,0,0.08,0.08,0.08);

	char mis_text_val[15];
	sprintf(mis_text_val,"%d",(int)missiles);
    drawStroke(reinterpret_cast<const unsigned char *>(mis_text_val),130,72,0,0.08,0.08,0.08);

	char fuel_text[15];
	strcpy(fuel_text,"Fuel:  %");
    drawStroke(reinterpret_cast<const unsigned char *>(fuel_text),-149,82,0,0.08,0.08,0.08);

	char fuel_text_val[15];
	sprintf(fuel_text_val,"%d",(int)fuel);
    drawStroke(reinterpret_cast<const unsigned char *>(fuel_text_val),-125,82,0,0.08,0.08,0.08);
}

void draw_intro(int i) {
	glLineWidth(1);
	drawStroke(reinterpret_cast<const unsigned char *>("SkyWars"),-20,75,0,0.08,0.08,0.08);
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	if(i==0){
	drawBitmap(-90,40,GLUT_BITMAP_HELVETICA_18,
		reinterpret_cast<const unsigned char *>("hello? This is 2D game inspired from the great balakot airstrike.\n\r\nThe 2019 Balakot airstrike was conducted by India in the early morning hours of\nFebruary 26 when Indian warplanes crossed the de facto border in the disputed \nregion of Kashmir, and dropped bombs in the vicinity of the town of Balakot in \nKhyber Pakhtunkhwa province in Pakistan."));
	drawBitmap(-45,-20,GLUT_BITMAP_HELVETICA_18,
		reinterpret_cast<const unsigned char *>("You will be briefed about the mission\nClick Next Button"));
	drawBitmap(-90,-60,GLUT_BITMAP_HELVETICA_18,
		reinterpret_cast<const unsigned char *>("NAME: S LOKESHWAR\nUSN: 1BI16CS187"));
	}
	else if(i==1) {
		drawStroke(reinterpret_cast<const unsigned char *>("Mission-1"),-20,35,0,0.08,0.08,0.08);
		drawBitmap(-60,20,GLUT_BITMAP_HELVETICA_18,
		reinterpret_cast<const unsigned char *>("Cross the border to reach targetted site of balakot\nDo not be seen by enemy planes. Dodge them. \nYou passing the border shouldn't be known to them.\nAfter reaching the location wait for further instructions."));
	} 
	glPopMatrix();
}

void draw_intro_boxes() {
	printf("Narration::\n");
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0,0.0,0.0,0.9);
		glBegin(GL_POLYGON);
			glVertex2d(120,90);
			printf("Drawing ??\n");
			glVertex2d(120,70);
			glVertex2d(-120,70);
			glVertex2d(-120,90);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2d(-100,-90);
			printf("Drawing ??\n");
			glVertex2d(-100,50);
			glVertex2d(100,50);
			glVertex2d(100,-90);
		glEnd();
		glColor4f(0.1,0.1,0.9,0.9);
		glBegin(GL_POLYGON);
			glVertex2d(120,-28);
			glVertex2d(120,-40);
			glVertex2d(145,-40);
			glVertex2d(145,-28);
		glEnd();
		glDisable(GL_BLEND);
		glColor3f(1.0,1.0,1.0);
		glClearColor(1.0,1.0,1.0,1.0);
		drawBitmap(125.0,-35.0,GLUT_BITMAP_9_BY_15,reinterpret_cast<const unsigned char *>("NEXT=>>"));
		glPopMatrix();
		glFlush();
}

void rocket(int x_cor, int y_cor,int r_no) {
    //char rocket[3][16];
    //rocket[0]="res/rocket2.png";rocket[1]="res/rocket3.png";rocket[2]="res/rocket4.png";
    // const unsigned char* rocket[3][];
    // rocket[0] = reinterpret_cast<const unsigned char *>("res/rocket2.png");
    // rocket[1] = reinterpret_cast<const unsigned char *>("res/rocket3.png");
    // rocket[2] = reinterpret_cast<const unsigned char *>("res/rocket4.png");
    const unsigned char* t = reinterpret_cast<const unsigned char *>("res/rocket2.png");
    
    
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (i_mis1 == 0){
		tex_2d_mis[1] = SOIL_load_OGL_texture
				(
					"res/rocket2.png",
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MULTIPLY_ALPHA
				);

		i_mis1 = 1;
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d_mis[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(x_cor, y_cor);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(x_cor+SIZE_MIS_X, y_cor+SIZE_MIS_y-10);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(x_cor, y_cor+SIZE_MIS_y-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void draw_rockets() {
	if(missile_x>200)
		no_of_missiles=rand()%MAX_MISSILES+1;
	if(missile_x>=195 && missile_x<=200)
		for(int k=1; k<=no_of_missiles; k++)
			missile_y[k]=-101+rand()%165;
	switch(no_of_missiles) {
	case 1:
		rocket(missile_x,missile_y[1],1);
		break;
	case 2:
		rocket(missile_x,missile_y[1],1);
		rocket(missile_x,missile_y[2],2);
		break;
	case 3:
		rocket(missile_x,missile_y[1],1);
		rocket(missile_x,missile_y[2],2);
		rocket(missile_x,missile_y[3],3);
		break;
	case 4:
		rocket(missile_x,missile_y[1],1);
		rocket(missile_x,missile_y[2],3);
		rocket(missile_x,missile_y[3],2);
		rocket(missile_x,missile_y[4],3);
		break;
	default:
	case 5:
		rocket(missile_x,missile_y[1],1);
		rocket(missile_x,missile_y[2],3);
		rocket(missile_x,missile_y[3],2);
		rocket(missile_x,missile_y[4],3);
		rocket(missile_x,missile_y[5],1);
		break;

	}
}

void draw_chosen_plane() {
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-150,-40);
	glVertex2f(-30,-40);
	glVertex2f(-30,40);
	glVertex2f(-150,40);
	glEnd();

	glPushMatrix();
	glTranslatef(-90,0,0);
	glScalef(1.6,1.6,0);
	plane1.draw_plane();
	glPopMatrix();
}

void drawfps() {
	int length;
	char d_fps[15];
	sprintf(d_fps,"FPS: %.1f",fps);
	glLineWidth(1);
    drawStroke(reinterpret_cast<const unsigned char *>(d_fps),-160,-85,0,0.04,0.04,0.04);
}

void page1_dup(const char t[50],float x,float y, int index) {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	if(j[index]==0) {
		tex_2d_1 = SOIL_load_OGL_texture
			(
				t,
				SOIL_LOAD_RGBA,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_NTSC_SAFE_RGB
			);
		j[index]=1;
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d_1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-x, -y);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(x, -y);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(x, y);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-x, y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void RenderScene() {
	if(page==0) { //do not shorten this.
		glClear(GL_COLOR_BUFFER_BIT);
		//load .png image
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(i_0==0) {
			tex_2d_0 = SOIL_load_OGL_texture
				   (
					   "res/loading.png",
					   SOIL_LOAD_RGBA,
					   SOIL_CREATE_NEW_ID,
					   SOIL_FLAG_NTSC_SAFE_RGB
				   );
			i_0=1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d_0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-190.0f, -190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(190.0f, -190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(190.0f, 190.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-190.0f, 190.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//draw loading bar
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
			glVertex2f(-171.0f, -4.4f);
			glVertex2f(0.0f+x_step, -4.4f);
			glVertex2f(0.0f+x_step, 1.3f);
			glVertex2f(-171.0f, 1.3f);
		glEnd();

		drawfps();
		glutSwapBuffers();
		glFlush();
		//local_screen=true;
	}
	if(page==1) {
		if(local_screen==1){
			page1_dup("res/intro.jpg",178.0,100.0,0);
			glColor3f(1.0,1.0,1.0);
			draw_intro_boxes();
			draw_intro(0);
			glutSwapBuffers();
			glFlush();
		}
		else if(local_screen==0){
			page1_dup("res/intro.jpg",178.0,100.0,1);
			glColor3f(1.0,1.0,1.0);
			draw_intro_boxes();
			draw_intro(1);
			// glPushMatrix();
			// page1_dup("res/map01.png",100,-25,2);
			// glPopMatrix();
			glutSwapBuffers();
			glFlush();
		}
	}
	//game menu
	if(page==2) {
		page1_dup("res/rtr271ii.jpg",178.0,100.0,3);
		draw_menu_text();
		drawfps();
		glutSwapBuffers();
	}
	//instruction
	if(page==21) {
		page1_dup("res/instructions2.png",178.0,100.0,5);
		draw_inst_text();
		glutSwapBuffers();
	}
	//highscores
	if(page==23) {
		page1_dup("res/scene2.png",178.0,100.0,4);
		draw_high_text();
		glutSwapBuffers();
	}
	//game play
	if(page==3) { //do not shorten this.
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		if (i_bck == 0)
		{
			tex_2d = SOIL_load_OGL_texture
				 (
					 scene,
					 SOIL_LOAD_RGBA,
					 SOIL_CREATE_NEW_ID,
					 SOIL_FLAG_NTSC_SAFE_RGB
				 );
			i_bck = 1;
		}
		glBindTexture(GL_TEXTURE_2D, tex_2d);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-190.0f+x, -100.0f);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(890.0f+x, -100.0f);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(890.0f+x, 100.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-190.0f+x, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//fuel indicator outline
		glColor3f(0,0,0);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-50-100,80);
		glVertex2f(50-100,80);
		glVertex2f(50-100,70);
		glVertex2f(-50-100,70);
		glEnd();

		//fuel indicator
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(-49-100,79);
		glVertex2f(-49+fuel-100,79);
		glVertex2f(-49+fuel-100,71);
		glVertex2f(-49-100,71);
		glEnd();

		//seprator--to seprate score and game screen
		glLineWidth(1);
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glVertex2f(-200,64);
		glVertex2f(200,64);
		glEnd();

		draw_score();
		draw_rockets();

		glPushMatrix();
		glTranslatef(-130,y_pos,0);
		glRotatef(theta,0,0,1);
		plane1.draw_plane();
		glPopMatrix();
		drawfps();
		glutSwapBuffers();
	}
	//finish
	if(page==4) { //add next game here.
		page1_dup("res/finish.png",178.0,100.0,7);
		draw_fin_text();
		glutSwapBuffers();
	}
}

void calculateFPS() {
	//  Increase frame count
	frameCount++;
	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	//  Calculate time passed
	int timeInterval = currentTime - previousTime;

	if(timeInterval > 1000)
	{
		//  calculate the number of frames per second
		fps = frameCount / (timeInterval / 1000.0f);
		//  Set time
		previousTime = currentTime;
		//  Reset frame count
		frameCount = 0;
	}
}

void TimerFunction(int v) {
	calculateFPS();
	if(page==0)
	{
		if(x_step<171.0)
			x_step+=6.0*SPEED;
		else
			page=1;
	}
	if(page==22)
	{
		if(y_cre<80)
			y_cre+=0.5;
	}
	if(page==3)
	{
		if(fuel==98||x<-700)
			x=0;
        x-=0.3*SPEED;
		cout<<x<<endl;
		if(missile_x<-210)
			missile_x=250;
		missile_x-=1*SPEED;
		if(state==UP)
		{
			if(fuel>0)
				fuel-=.1*SPEED;
		}

		dist+=0.1*SPEED;

		//update number of missiles douged
		{
			if(missile_x<-90)
			{
				if(update_mis==0)
				{
					missiles+=no_of_missiles;
					update_mis=1;
				}
			}
			else
				update_mis=0;
		}

		//plane position
		if(state==UP)
		{
			if(fuel>0)
			{
				if(y_pos<=90)
					SPEED*y_pos++;
				if(theta<0)
					theta+=.3*SPEED;
				else
					theta+=.1*SPEED;
			}
			else
				y_pos--*SPEED;
		}
		else
		{
			if(y_pos>=-100)
				SPEED*y_pos--;
			else
			{
				y_pos=0;
				page=4;
			}
			if(theta>0)
				theta-=.3*SPEED;
			else
				theta-=.1*SPEED;

		}

		//check for collision
		if(missile_x<-110)
		{
			cout<<"Possibility of crash"<<endl;
			for(int m=1; m<=no_of_missiles; m++)
			{
				if(missile_y[m]>y_pos-10 && missile_y[m]<y_pos+10)
				{
					cout<<"Crash"<<endl;
					hit_missile=1;
					for(int m1=1; m1<=no_of_missiles; m1++)
						missile_x=200;
					y_pos=0;
					x=0;
					page=4;
					break;
				}
			}

		}
	}

	glutPostRedisplay();
	glutTimerFunc(1000/v,TimerFunction, v);
}

void myinit(void) {
	glClearColor(0.0f, 0.8f, 0.0f, 1.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

void Resize(int w, int h) {
	GLfloat aspectRatio;
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h) {
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else {
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,450);
	glutCreateWindow("Airplane!");
	//glutPositionWindow(320,150);
	glutFullScreen();
	myinit();
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc(SpecialKeys);
	//glutMouseFunc(Mouse);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(Mouse);
	glutTimerFunc(100, TimerFunction, frames);
	glutReshapeFunc(Resize);
	glutMainLoop();
	return 0;
}
