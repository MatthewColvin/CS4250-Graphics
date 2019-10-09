// Matthew Colvin
// 
//

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include "square.h"
#include "circle.h"
#include <vector>
#include <random>
#include <time.h>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

class Stopwatch{
  public:
  inline Stopwatch(){ reset();  }
  inline void reset(){t = time(NULL);}
  inline int get_time(){  return ( time(NULL) - t);  }
  private:
  time_t t;
};

class Character : public Square{
  public:
    Character():Square(){
    };
    Character(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc,int maxhealth) :
    Square(nindex,npoints,noffsetLoc,nsizeLoc,ncolorLoc){
      max_health = maxhealth;
      currentheath = max_health;
      isAlive=true;
    };


    void kill(){ isAlive = false; };
    bool is_alive(){return isAlive;} 

    void sub_health (int damage){
      currentheath-=damage;
      if(currentheath <= 0){
        kill();
      }
    }
    void add_heath (int amount){
      currentheath+=amount;
    }

  protected:
    bool isAlive;
    int max_health;
    int currentheath; 
};

class Civilian : public Character{
  public:
    Civilian(int max_health):Character(){
    };
    Civilian(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc,int maxhealth) :
    Character(nindex,npoints,noffsetLoc,nsizeLoc,ncolorLoc,maxhealth){
    };

    void update(){ 
      if(isAlive){
        color(healthtocolor());
        // selection color is health plus some blue
        GLfloat selr = healthtocolor().x;
        GLfloat selg = healthtocolor().y;
        GLfloat selb = (healthtocolor().z * 255 + 50) / 255;
        colorwhenselected = vec3(selr,selg,selb);
      }
      Square::update();   
    }

  private:
    
    vec3 healthtocolor(){
      
      if(currentheath >= max_health){
        currentheath = max_health;
        return(vec3(0.0,1.0,0.0)); 
      }
      
      GLfloat healthpercentage = currentheath/(GLfloat)max_health;
      
      GLfloat rvalue, gvalue;
      
      // This health is just a number between 0 and 510 
      // for scaling into a color
      GLfloat RGBhealth = healthpercentage * 510; 

      if (RGBhealth <= 255.0){
        rvalue = 255.0;
        gvalue = RGBhealth;
      }else{
        gvalue = 255.0;
        rvalue = 510.0 - RGBhealth;
      }
      return (vec3(rvalue/255.0,gvalue/255.0,0));
    };
};

class Invader: public Character{
  public:
    Invader(int max_health):Character(){
    };
    Invader(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc,int maxhealth) :
    Character(nindex,npoints,noffsetLoc,nsizeLoc,ncolorLoc,maxhealth){
    };

    void update(){ 
      if(isAlive){
        color(healthtocolor());
      }   
      Square::update();
    }


  private:
    vec3 healthtocolor(){
      
      if(currentheath >= max_health){
        currentheath = max_health;
        return(vec3(0.0,0.0,1.0)); 
      }
      
      GLfloat healthpercentage = currentheath/(GLfloat)max_health;
      
      GLfloat rvalue, bvalue;
      
      // This health is just a number between 0 and 510 
      // for scaling into a color
      GLfloat RGBhealth = healthpercentage * 510; 

      if (RGBhealth <= 255.0){
        rvalue = 255.0;
        bvalue = RGBhealth;
      }else{
        bvalue = 255.0;
        rvalue = 510.0 - RGBhealth;
      }
      return (vec3(rvalue/255.0,0,bvalue/255.0));
    };
};

class Food : public Square{
  public:
    
    Food() : Square(){};

    Food(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc) :
    Square(nindex,npoints,noffsetLoc,nsizeLoc,ncolorLoc){};
    
    void eat(){ eaten = true; };
    bool has_been_eaten(){ return (eaten);}
  private :
    bool eaten = false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game functions 
  void buffersetup();
  void setupmap();
  void processSelection(unsigned char PixelColor[], int btn);
  vec3 nextselectioncolor();
  string current_title();
  bool is_game_over();
//

//Character Movement Functions
  bool can_move_up(Square character,int amount);
  bool can_move_down(Square character,int amount);
  bool can_move_left(Square character,int amount);
  bool can_move_right(Square character,int amount);
  bool can_spawn_there(Food object);
  //Idle animations
    void move_invaders_randomly(int amount);
    void move_invaders_toward_food(int amount);
    void move_civilians();
    void drop_food();
    void spawn_food(int extrasize);
    void characters_eat();
    void slowly_starve_characters();
  bool objects_will_collide(Square character,Square tree,
  int amountup , int amountdown ,int amountleft, int amountright);
  bool character_did_get_food(Square *character, Square *food);
  

//

//Glut setup and Callbackfuncitons 
  void glutwindowinit();
  extern "C" void display();
  extern "C" void idle();
  extern "C" void mouse(int btn, int state, int x, int y);
  extern "C" void menustatus(int status, int x, int y);
  extern "C" void motion(int x, int y);
  extern "C" void myReshape(int w, int h);
  extern "C" void myMenu(int value);
    void setupMenu();
  extern "C" void key(unsigned char k, int xx, int yy);
  extern "C" void special(int k, int xx, int yy);
//

//Global Variables 
  bool clearscreen=true;
  bool gameStarted = false;
  bool gameOver = false;
  bool civiliansWin = false;
  // Character and object vectors ////////////
    vector<Civilian*> civilians;
    vector<Invader*> invaders;
    vector<Square*> trees;
    vector<Food*> food;
  // Data storage for our geometry for the lines
    vec2 *points;
  // Used to keep track of generated selection colors
    vec3 selectioncolors = vec3(0.0,0.0,1.0);
  // Window Size 
    GLint windowSizeLoc;
    int win_h=900;
    int win_w=900;
    int Glut_win_id;
  // Offsets to uniforms
    GLint offsetLoc;
    GLint sizeLoc;
    GLint colorLoc;

  // Game Timers
    Stopwatch foodtimer;
    Stopwatch civilanstarver;
    Stopwatch invaderstarver;
    Stopwatch gametimer;
  // Custom Game Variables Setup 
    //cammelCase - can be canged while game is running
    //THIS_NOTATION - cannot be changed so must be set before starting
    //Some may be easy to make editable.
      // TREE ATTRIBUTES
        const int INITIAL_TREE_SIZE = 80; 
        const vec3 treeColor = vec3(50/255.0,77/255.0,43/255.0);
        const int NUM_TREES = 6;

      // INVADER ATTRIBUTES
        int NUM_BAD_GUYS = 5;
        int invadersStepSize = 1;
        GLfloat invadersSpeed = 0.09;
        bool invadersAreMoving=true;
        int INVADERS_MAX_HEALTH = 1000;
        int invaderHungerInterval = 2; //sec 
        int invaderHealthlossperinterval = 100; 
        bool invadersAreStarving =true;
      // CIVILIANS ATTRIBUTES
        const int INITIAL_CHARACTER_SIZE = 20;
        const int NUM_GOOD_GUYS = 5;
        const int civiliansStepSize = 10;
        const GLfloat civiliansSpeed = 0.40;
        bool civiliansAreMoving=true; 
        int CIVILIANS_MAX_HEALTH = 1000;
        bool civiliansAreStarving = true;
        int civilianHungerInterval = 2; //sec
        int civilianHealthLossPerInterval = 100; 

      // FOOD ATTRIBUTES
        int INITIAL_FOOD_SIZE = 5;
        bool foodIsDropping = true;
        int secsBetweenDrops = 5;
        vec3 foodColor = vec3(184/255.0,139/255.0,94/255.0);

      // GAME ATTRIBUTES
        const bool collisonDetectionIsOn = true;
        bool charactersCanEat = true;

  //
//


extern "C" void display(){
  if (clearscreen) {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  if(gameStarted){
    //Draw all the characters
    for (auto character : civilians){
      if(character->is_alive()){
        character->draw();
      }
    }
    for(auto character : invaders){
      if(character->is_alive()){
        character->draw();
      }
    }
    for(auto object : trees){
      object->draw();
    }
    for (auto object : food){
      if(!object->has_been_eaten()){
      object->draw();
      }
    }

    glutSwapBuffers ();
  }
}

extern "C" void idle(){  
  glutSetWindowTitle(current_title().c_str());
  
  if(!is_game_over()){
    if (invadersAreMoving) {
      switch(rand()%2){
        case(0):
          move_invaders_randomly(invadersStepSize); 
        if(rand() % 2==0) break;
        
        case(1):
          move_invaders_toward_food(invadersStepSize);
        break;    
      }
    }
    if(civiliansAreMoving){
      move_civilians();
    }
    if(foodIsDropping){
      drop_food();
    }
    if(charactersCanEat){
      characters_eat();
    }
    if (civiliansAreStarving || invadersAreStarving){
      slowly_starve_characters();
    }
  }
  glutPostRedisplay();
}

void processSelection(unsigned char PixelColor[], int btn){
  // std::cout << PixelColor.x << " " << PixelColor.y << " " << PixelColor.z << std::endl;
  setupMenu();// menu possibilities update.
  for (auto character : civilians){
    if (cmpcolor(PixelColor,character->getSelectColor())){
      character->Selected();
      cout << "Civilian at:" << character->get_pos().x << " " << character->get_pos().y << endl;
    }else{
      character->notSelected();
    }
  } 
  for (auto character : invaders){
    if (cmpcolor(PixelColor,character->getSelectColor())){
      cout << "Invader at:" << character->get_pos().x << " " << character->get_pos().y << endl;
    }
  }
  for (auto object : trees){
    if (cmpcolor(PixelColor,object->getSelectColor())){
      cout << "Tree at:" << object->get_pos().x << " " << object->get_pos().y << endl;
    }
  }
  for (auto object : food){
    if (cmpcolor(PixelColor,object->getSelectColor())){
      cout << "Food at:" << object->get_pos().x << " " << object->get_pos().y << endl;
    }
  }
}

// Mouse callback, implements selection by using colors in the back buffer.
extern "C" void mouse(int btn, int state, int x, int y){
  if (state == GLUT_DOWN) {
    // Draw the scene with identifying colors
    // Ensure the clear color isn't the same as any of your objects
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw the character in the selection color in the back buffer
    for (auto character : civilians){
      if(character->is_alive()){
        character->draw(true);
      }
    }
    for (auto character : invaders){
      if(character->is_alive()){
        character->draw(true);
      }
    }
    for (auto object : trees){
      object->draw(true);
    }
    for (auto object : food){
      if(!object->has_been_eaten()){
        object->draw(true);
      }
    }

    

    // Flush ensures all commands have drawn
    glFlush();

    // Read the value at the location of the cursor
    // In the back buffer (not the one visible on-screen)
    glReadBuffer(GL_BACK);
    unsigned char PixelColor[3];
    glReadPixels(x, win_h-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &PixelColor);
    std::cout << int(PixelColor[0]) << " "
	      << int(PixelColor[1]) << " "
	      << int(PixelColor[2]) << std::endl;
    processSelection(PixelColor, btn);


    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
  }
}

// Allows you to still get location of mouse clicks when using menus
extern "C" void menustatus(int status, int x, int y){
  // Pass the new location to the object.

  glutPostRedisplay();
}  

// Called whenever mouse moves, after being pressed
extern "C" void motion(int x, int y){

 
  glutPostRedisplay();
}


// Maintains the mapping from screen to world coordinates.
extern "C" void myReshape(int w, int h){
  glViewport(0,0,w,h);
  win_h = h;
  win_w = w;
  glUniform2f(windowSizeLoc, win_w, win_h);       // Pass the window size
						  // size
  glutPostRedisplay();
}

// Example menu code.
extern "C" void myMenu(int value){
  switch (value) {
  case 1:
    invadersAreMoving = !invadersAreMoving; // switch 
    break;
  case 2: // Black background
    glClearColor (0.0, 0.0, 0.0, 1.0);
    break;
  //case ?: // reset game?
  default:
    break;
  }
  glutPostRedisplay(); 
}


// Create menu and items.
void setupMenu(){
  glutCreateMenu(myMenu);
  if(invadersAreMoving){
    glutAddMenuEntry("Stop Enemies", 1);
  }else{
    glutAddMenuEntry("Start Enemies",1);
  }
  glutAddMenuEntry("black background", 2);
  glutAddMenuEntry("reset game",3);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Keypress events.
extern "C" void key(unsigned char k, int xx, int yy){
  switch (k) {
  case 'q':
  case 'Q': 
    exit(0);
    break;
  case 'c':
    clearscreen = !clearscreen;
  case 'b':
  case 'B': 
    gameStarted = true;
  }


  // control the selected civilian
  for (auto character: civilians){
    if(character->isSelected()){
      // setting goal to current position so moving is interpereted correctly
      character->goal_to_pos(); 
      switch (k){
      case 'w':
        if(can_move_up(*character,civiliansStepSize)){
          character->move_up(civiliansStepSize);
        }
        break;
      case 'a':
        if(can_move_left(*character,civiliansStepSize)){
          character->move_left(civiliansStepSize);
        }
        break;
      case 's':
        if(can_move_down(*character,civiliansStepSize)){
          character->move_down(civiliansStepSize);
        }
        break;
      case 'd':
        if(can_move_right(*character,civiliansStepSize)){
          character->move_right(civiliansStepSize);
        }
        break;
      }
    }
    
  }
  glutPostRedisplay();
}

// Special Keys events.
// This one only responds to the up arrow key.
extern "C" void special(int k, int xx, int yy){
  switch (k) {
  case GLUT_KEY_UP:
  break;
  case GLUT_KEY_DOWN:
  break;
  case GLUT_KEY_LEFT:
    break;
  case GLUT_KEY_RIGHT:
    break;
  default:
    // do nothing
    break;
  }
  glutPostRedisplay();
}

// Initialize all OpenGL callbacks, create window.
void glutwindowinit(){
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(win_w,win_h);
  glutInitWindowPosition(20,20);

  Glut_win_id = glutCreateWindow("Homework 1");

  // Color initializations
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  // Can you do this?  Attach a menu to a button already
  // used for something else?  
  //setupMenu();

  // Callbacks
  glutDisplayFunc(display); 
  glutIdleFunc(idle); 
  glutReshapeFunc (myReshape);
  glutMouseFunc (mouse);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutMotionFunc (motion);

  // NOTE: This is not in book. The following sets up a callback
  // whenever a menu is in use.  It gives mouse click location as well
  // as the status of the use of the menu.
  glutMenuStatusFunc(menustatus);
}

// This function initializes the buffers and shaders
void buffersetup(){
  // Locations of variables in shaders.

  // Create a vertex array object - this will hold the buffers necessary for the VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");

  // Initialize the vertex position attribute from the vertex shader
  windowSizeLoc = glGetUniformLocation(program, "windowSize");

  if (windowSizeLoc==-1) {
    std::cerr << "Unable to find windowSize parameter" << std::endl;
  }
  offsetLoc  = glGetUniformLocation(program, "offset");
  if (offsetLoc==-1) {
    std::cerr << "Unable to find offsetLoc parameter" << std::endl;
  }
  sizeLoc  = glGetUniformLocation(program, "size");
  if (sizeLoc==-1) {
    std::cerr << "Unable to find sizeLoc parameter" << std::endl;
  }
  colorLoc  = glGetUniformLocation(program, "vcolor");
  if (colorLoc==-1) {
    std::cerr << "Unable to find colorLoc parameter" << std::endl;
  }
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform2f(offsetLoc, 0.0, 0.0);               // Initial offset
						  // for mouse loc.

  // We need just one square and circle to draw any number of them.
  points = new vec2[Square::NumPoints+Circle::NumPoints];
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  setupmap();
  // Send the data to the graphics card, after it has been generated
  // by creating the objects in the world (above).
  glBufferData(GL_ARRAY_BUFFER, (Square::NumPoints+Circle::NumPoints)*sizeof(vec2), points, GL_STATIC_DRAW);
}

void setupmap(){
  const vec3 BAD_GUY_COLOR= vec3 (1.0,0.0,0.0);
  
  srand(time(NULL));// seed the random function with time
  
  GLfloat randomx, randomy;

  // set up trees
  for (int i=0; i<NUM_TREES; i++){
    randomx = rand() %  (win_w - INITIAL_TREE_SIZE) + INITIAL_TREE_SIZE;
    randomy = rand() %  (win_h - INITIAL_TREE_SIZE) + INITIAL_TREE_SIZE;
    
    trees.push_back(new Square(0,points,offsetLoc,sizeLoc,colorLoc));
    trees[i]->change_size(INITIAL_TREE_SIZE);
    trees[i]->move(randomx,randomy);
    trees[i]->color(treeColor);
    trees[i]->selectColor(nextselectioncolor());
  }

  //set up the good characters 
  for (int i=0; i<NUM_GOOD_GUYS; i++){
    Civilian* potential_citizen = new Civilian(0,points,offsetLoc,sizeLoc,colorLoc,CIVILIANS_MAX_HEALTH);
    potential_citizen->change_size(INITIAL_CHARACTER_SIZE);
    potential_citizen->selectColor(nextselectioncolor());
    potential_citizen->setSpeed(civiliansSpeed);
    
    do{
      randomx = rand() % (win_w - INITIAL_CHARACTER_SIZE) + INITIAL_CHARACTER_SIZE ;
      randomy = rand() % win_h/3; //spawn good guys in the bottom half
      potential_citizen->move(randomx,randomy+INITIAL_CHARACTER_SIZE);
      potential_citizen->goal_to_pos(); 
    }while (!(
    can_move_left(*potential_citizen,0) &&
    can_move_right(*potential_citizen,0) &&
    can_move_up(*potential_citizen,0) &&
    can_move_down(*potential_citizen,0)
    ));
     
    civilians.push_back(potential_citizen);
  }
  //set up the bad characters
  for (int i=0; i<NUM_BAD_GUYS; i++){
  Invader* potential_invader = new Invader(0,points,offsetLoc,sizeLoc,colorLoc,CIVILIANS_MAX_HEALTH);
  do{
      randomx = rand() %  (win_w - INITIAL_CHARACTER_SIZE) + INITIAL_CHARACTER_SIZE;
      randomy = rand() % win_h/3 + win_h/2 ;// spawn bad characters in the top half
      potential_invader->move(randomx,randomy+INITIAL_CHARACTER_SIZE);
      potential_invader->change_size(INITIAL_CHARACTER_SIZE);
      potential_invader->goal_to_pos(); 
    }while (!(
    can_move_left(*potential_invader,0) &&
    can_move_right(*potential_invader,0) &&
    can_move_up(*potential_invader,0) &&
    can_move_down(*potential_invader,0)
    ));
    invaders.push_back(potential_invader);
  }
}



// generates a selection color different from all others
vec3 nextselectioncolor(){
  if (selectioncolors.x < 255.0){
    if (selectioncolors.y >= 255.0){
      selectioncolors.x++;
      selectioncolors.y = 0;
    }
    if(selectioncolors.y < 255.0){
      if (selectioncolors.z >= 255.0){
        selectioncolors.y++;
        selectioncolors.z = 0;
      }
      if(selectioncolors.z < 255.0){
        selectioncolors.z++;
      }
    }
  }
  return selectioncolors/255;
}



//function to determine characters ability to move upward a given amount
bool can_move_up(Square character,int amount){
  if(collisonDetectionIsOn){
    if (character.get_pos().y >= win_h - character.get_size() - amount ){ // character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(objects_will_collide(character,*tree,amount,0,0,0)){return false;}
    }
  }
  return true; 
}
//function to determine characters ability to move downward a given amount
bool can_move_down(Square character,int amount){
  if(collisonDetectionIsOn){
    if (character.get_pos().y <= 0 + character.get_size() + amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(objects_will_collide(character,*tree,0,amount,0,0)){return false;}
    }
  }
  return true;
}
bool can_move_left(Square character,int amount){
  if(collisonDetectionIsOn){  
    if( character.get_pos().x <= 0 + character.get_size() + amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(objects_will_collide(character,*tree,0,0,amount,0)){return false;}
    }
  }
  return true;
}
bool can_move_right(Square character,int amount){
  if(collisonDetectionIsOn){
    if(character.get_pos().x >= win_w - character.get_size() - amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(objects_will_collide(character,*tree,0,0,0,amount)){return false;}
    }
  }
  return true;
}

//moves invaders randomly TODO make them struggle toward the food 
void move_invaders_randomly(int amount){
   srand(time(NULL)); 
    for(auto character: invaders){
      if(character->is_alive()){
        character->goal_to_pos();
        switch(rand()%4){
          case 0:
            if(can_move_up(*character,amount)){
              character->move_up(amount);
            }
            break;
          case 1:
            if(can_move_left(*character,amount)){
              character->move_left(amount);
            }
            break;
          case 2:
            if(can_move_down(*character,amount)){
              character->move_down(amount);
            }
          case 3:
            if(can_move_right(*character,amount)){
              character->move_right(amount);
            }
            break;
        }
        character->update();
    }
    }
}


int food_lenth_from_character(Character* c,Food* f){  
  if(c->is_alive() && !f->has_been_eaten()){
    vec2 foodpos = f->get_pos();
    vec2 charpos = c->get_pos();
    return(sqrt((foodpos.x-charpos.x)*(foodpos.x-charpos.x) + (foodpos.y-charpos.y)*(foodpos.y-charpos.y)));
  }
  return -1;
}

void move_invaders_toward_food(int amount){
    for(auto character: invaders){
      if(character->is_alive()){
        int min_distance_from_food = 100000000;
        vec2 closest_food_coordinates = vec2(0,0);
        for (auto object : food){
          if(min_distance_from_food < food_lenth_from_character(character,object));
          closest_food_coordinates = object->get_pos();
        }

        bool needToGoRight = character->get_pos().x < closest_food_coordinates.x;
        bool needToGoUp = character->get_pos().y < closest_food_coordinates.y;
        
        switch (rand()%2){
          case 0:
            if(needToGoUp && can_move_up(*character,amount)){
              character->move_up(amount);
            }else if(can_move_down(*character,amount)){
              character->move_down(amount);
            }
          if(rand() % 2 == 0){break;}       
          
          case 1: 
            if(needToGoRight && can_move_right(*character,amount)){
              character->move_right(amount);
            }else if(can_move_left(*character,amount)){
              character->move_left(amount);
            }
        } 
      character->update();
    }
  }
}



void move_civilians(){
  for (auto character : civilians){
    character->update();
  }
}

bool objects_will_collide(Square character,Square tree,
int amountup,int amountdown,int amountleft,int amountright){ 
    GLfloat tree_x = tree.get_pos().x;
    GLfloat tree_y = tree.get_pos().y;
    GLfloat character_x = character.get_pos().x;
    GLfloat character_y = character.get_pos().y;


    GLfloat treeleftmost_x = tree_x - tree.get_size();
    GLfloat treerightmost_x = tree_x + tree.get_size();
    GLfloat treebottommost_y = tree_y - tree.get_size();
    GLfloat treetopmost_y = tree_y + tree.get_size();

    GLfloat characterleftmost_x = character_x - character.get_size() - amountleft ;
    GLfloat characterrightmost_x = character_x + character.get_size() + amountright ;
    GLfloat characterbottommost_y = character_y - character.get_size() - amountdown ;
    GLfloat charactertopmost_y = character_y + character.get_size() + amountup;

    bool characteronsame_xes = characterrightmost_x > treeleftmost_x && characterleftmost_x < treerightmost_x;
    bool characteronsame_ys = charactertopmost_y > treebottommost_y && characterbottommost_y < treetopmost_y;

    if(characteronsame_ys && characteronsame_xes){
      return true;
    }
  return false;
}
// This function will call the function spawn_food to create food on the
// screen randomly
void drop_food(){
  if(foodtimer.get_time() > secsBetweenDrops){
    switch (rand() % 4){
      case 0: // generate a small bit of food 
        spawn_food(0);
      case 1:
        if(rand() % 2 == 0)
        spawn_food(5);
      break;
      case 2:
        spawn_food(10);
      break;
      case 3:
        if (rand()%2 == 0 && rand()%2 == 0 ) 
        spawn_food(25);
      break;
      default:
      break;
    }
  foodtimer.reset(); 
  }
}
// Will make food appear at random xes and ys  
// on the screen at the initail size + a given size 
void spawn_food(int size){
  if(rand()%2 == 0){ // attempt to make food a bit more random
    srand(foodtimer.get_time());
  }else{
    srand(time(NULL));
  }

  Food* potential_food = new Food(0,points,offsetLoc,sizeLoc,colorLoc);
  potential_food->color(foodColor);
  potential_food->change_size(INITIAL_FOOD_SIZE + size);
  potential_food->selectColor(nextselectioncolor());

  do {
  int randomx = rand() % (win_w - INITIAL_FOOD_SIZE - size) + INITIAL_FOOD_SIZE + size;
  int randomy = rand() % (win_h - INITIAL_FOOD_SIZE - size) + INITIAL_FOOD_SIZE + size;
  
  potential_food->move(randomx,randomy+INITIAL_FOOD_SIZE);
  potential_food->goal_to_pos();

  }while(!can_spawn_there(*potential_food));

  food.push_back(potential_food);

}


bool character_did_get_food(Square* character, Square* food){
  GLfloat tree_x = food->get_pos().x;
  GLfloat tree_y = food->get_pos().y;
  GLfloat character_x = character->get_pos().x;
  GLfloat character_y = character->get_pos().y;

  GLfloat treeleftmost_x = tree_x - food->get_size();
  GLfloat treerightmost_x = tree_x + food->get_size();
  GLfloat treebottommost_y = tree_y - food->get_size();
  GLfloat treetopmost_y = tree_y + food->get_size();

  GLfloat characterleftmost_x = character_x - character->get_size();
  GLfloat characterrightmost_x = character_x + character->get_size();
  GLfloat characterbottommost_y = character_y - character->get_size();
  GLfloat charactertopmost_y = character_y + character->get_size();

  bool characteronsame_xes = characterrightmost_x > treeleftmost_x && characterleftmost_x < treerightmost_x;
  bool characteronsame_ys = charactertopmost_y > treebottommost_y && characterbottommost_y < treetopmost_y;

  if(characteronsame_ys && characteronsame_xes){
    return true;
  }
  return false;
}

void characters_eat(){
  for(auto object: food){
    for (auto character : civilians){
      if(character_did_get_food(character,object)){
        character->add_heath((int)(object->get_size())); // character gets as much heath as food size 
        object->eat();
      }
    }
    for (auto character : invaders){
      if(character_did_get_food(character,object)){
        character->add_heath((int)(object->get_size())); // character gets as much heath as food size 
        object->eat();
      }
    }
  }
}

void slowly_starve_characters(){
  
  if(invadersAreStarving && invaderstarver.get_time() > invaderHungerInterval){
    for (auto character : invaders){
      if(character->is_alive()){
        character->sub_health(invaderHealthlossperinterval);
      }
    }
  invaderstarver.reset();  
  }

  if(civiliansAreStarving && civilanstarver.get_time() > civilianHungerInterval){
    for (auto character : civilians){
      if(character->is_alive()){
        character->sub_health(civilianHealthLossPerInterval);
      }
    }
  civilanstarver.reset();  
  }


}

bool can_spawn_there(Food food){
 for(auto character : civilians){
  if(character->is_alive()){
    if(objects_will_collide(*character,food,10,10,10,10)){ return false;}
  }
 }
 for(auto character : invaders){
  if(character->is_alive()){
    if(objects_will_collide(*character,food,10,10,10,10)){ return false;}
  }
 }
 for(auto object : trees){
  if(objects_will_collide(*object,food,0,0,0,0)){return false;}
 }
 return true;
}

string current_title(){
  
  if(!gameStarted){
    return("Use:AWSD keys to move|Select character to move with mouse|Press b to start");
  }
  if(!gameOver){
    return ("You will be Green and starve to red  PlayTime: " + std::to_string(gametimer.get_time()));
  }else{
    static int winning_time = gametimer.get_time();
    if(civiliansWin){
      return("You won in " + std::to_string(winning_time)  + " Seconds");
    }else{
      return("You lost in " + std::to_string(winning_time) + " Seconds" );
    }
    
    
  }


}
bool is_game_over(){
  int living_invaders=0;
  int living_civilians=0;
  for(auto character : invaders){
    if (character->is_alive()){
      civiliansWin = false;
      living_invaders++;
    }
  }
 for(auto character : civilians){
    if (character->is_alive()){
      civiliansWin = true;
      living_civilians++;
    }
  }
  if(living_civilians == 0 || living_invaders == 0){
    gameOver=true;
  }

  return gameOver;
}

int main(int argc, char** argv){
  // Several people forgot to put in the following line.  This is an
  // error, even if it still works on your machine, a program is
  // incorrect without the following call to initialize GLUT.
  glutInit(&argc,argv);
  
  glutwindowinit();
  // Initialize the "magic" that glues all the code together.
  glewInit();

  gametimer.reset();

  
  buffersetup();   // set up shaders and display environment

  glutMainLoop();       // enter event loop
  
  return(EXIT_SUCCESS); // return successful exit code
}
