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

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Stopwatch{
  public:
  inline Stopwatch(){ reset();  }
  inline void reset(){t = time(NULL);}
  inline int get_time(){  return ( time(NULL) - t);  }
  private:
  time_t t;
};

class Character : public Square{
  Character();

  private:
    int health;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game functions 
  void buffersetup();
  void setupmap();
  void clearmap();
  void processSelection(unsigned char PixelColor[], int btn);
  vec3 nextselectioncolor();

//

//Character Movement Functions
  bool can_move_up(Square character,int amount);
  bool can_move_down(Square character,int amount);
  bool can_move_left(Square character,int amount);
  bool can_move_right(Square character,int amount);
  //Idle animations
    void move_invaders(int amount);
    void drop_food();
    void spawn_food(int extrasize);
  bool will_character_hit_tree(Square character,Square tree,
  int amountup , int amountdown ,int amountleft, int amountright);
  bool did_character_get_food(Square character, Square food);

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
  //vector to hold all the characters
    vector<Square*> civilians;
    vector<Square*> invaders;
    vector<Square*> trees;
    vector<Square*> food;
  // Data storage for our geometry for the lines
    vec2 *points;
  // Used to keep track of generated selection colors
    vec3 selectioncolors = vec3(0.0,0.0,1.0);
  // Window Size 
    GLint windowSizeLoc;
    int win_h=900;
    int win_w=900;
    int Glut_win_id;
  // Crunch time hack to make characters able to be generated any time.
    // Offset of square  
      GLint offsetLoc;
    // Size of square
      GLint sizeLoc;
    // Color of square 
      GLint colorLoc;

  // Game Timer
    Stopwatch stopwatch;
  // Custom Game Variables Setup 
    //cammelCase - can be canged while game is running
    //THIS_NOTATION - cannot be changed so must be set before starting
    //Some may be easy to make editable.
    const int INITIAL_TREE_SIZE = 40; 
    const int INITIAL_CHARACTER_SIZE = 20;
    const int INITIAL_FOOD_SIZE = 5;
    const int NUM_BAD_GUYS = 5;
    const int NUM_GOOD_GUYS = 5;
    const int NUM_TREES = 2;
    const int CIVILIANS_STEP_SIZE = 5;
    const int INVADERS_STEP_SIZE = 1;
    const GLfloat CIVILIANS_SPEED = 0.09;
    const GLfloat INVADERS_SPEED = 0.09;
    const bool COLLISON_DETECTION_ON = true;
    const bool FOOD_IS_DROPPING = true;
    bool INVADERS_ARE_MOVING=true;
    int secsBetweenDrops = 5;
    vec3 foodColor = vec3(184/255.0,139/255.0,94/255.0);

  //
//


extern "C" void display(){
  if (clearscreen) {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  
  //Draw all the characters
  for (auto character : civilians){
    character->draw();
  }
  for(auto character : invaders){
    character->draw();
  }
  for(auto object : trees){
    object->draw();
  }
  for (auto object : food){
    object->draw();
  }

  glutSwapBuffers ();
}

extern "C" void idle(){
  if (INVADERS_ARE_MOVING) {
    move_invaders(INVADERS_STEP_SIZE); 
  }if(FOOD_IS_DROPPING){
    drop_food();
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
      character->draw(true);
    }
    for (auto character : invaders){
      character->draw(true);
    }
    for (auto object : trees){
      object->draw(true);
    }
    for (auto object : food){
      object->draw(true);
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
    INVADERS_ARE_MOVING = !INVADERS_ARE_MOVING; // switch 
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
  if(INVADERS_ARE_MOVING){
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
  }


  // control the selected civilian
  for (auto character: civilians){
    if(character->isSelected()){
      // setting goal to current position so moving is interpereted correctly
      character->goal_to_pos(); 
      switch (k){
      case 'w':
        if(can_move_up(*character,CIVILIANS_STEP_SIZE)){
          character->move_up(CIVILIANS_STEP_SIZE);
        }
        break;
      case 'a':
        if(can_move_left(*character,CIVILIANS_STEP_SIZE)){
          character->move_left(CIVILIANS_STEP_SIZE);
        }
        break;
      case 's':
        if(can_move_down(*character,CIVILIANS_STEP_SIZE)){
          character->move_down(CIVILIANS_STEP_SIZE);
        }
        break;
      case 'd':
        if(can_move_right(*character,CIVILIANS_STEP_SIZE)){
          character->move_right(CIVILIANS_STEP_SIZE);
        }
        break;
      }
      character->update();
      //cout << "character at " << character->get_pos().x << " " << character->get_pos().y << endl;
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
  const vec3 CANNOT_SELECT = vec3 (0.0,0.0,0.0);
  const vec3 TREE_COLOR = vec3(0.0,1.0,0.0);
  
  srand(time(NULL));// seed the random function with time
  
  GLfloat randomx, randomy;
  //set up the good characters 
  for (int i=0; i<NUM_GOOD_GUYS; i++){
    randomx = rand() % (win_w - INITIAL_CHARACTER_SIZE) + INITIAL_CHARACTER_SIZE ;
    randomy = rand() % win_h/3; //spawn good guys in the bottom half
    
    civilians.push_back(new Square(0,points,offsetLoc,sizeLoc,colorLoc));
    civilians[i]->change_size(INITIAL_CHARACTER_SIZE);
    civilians[i]->move(randomx,randomy+INITIAL_CHARACTER_SIZE);
    civilians[i]->setSpeed(CIVILIANS_SPEED);
    civilians[i]->selectColor(nextselectioncolor());
  }
  //set up the bad characters
  for (int i=0; i<NUM_BAD_GUYS; i++){
    randomx = rand() %  (win_w - INITIAL_CHARACTER_SIZE) + INITIAL_CHARACTER_SIZE;
    randomy = rand() % win_h/3 + win_h/2 ;// spawn bad characters in the top half
    
    invaders.push_back(new Square(0,points,offsetLoc,sizeLoc,colorLoc));
    invaders[i]->change_size(INITIAL_CHARACTER_SIZE);
    invaders[i]->move(randomx,randomy-INITIAL_CHARACTER_SIZE);
    invaders[i]->color(BAD_GUY_COLOR);
    invaders[i]->setSpeed(INVADERS_SPEED);
    invaders[i]->selectColor(nextselectioncolor());
  }
  // set up trees
  for (int i=0; i<NUM_TREES; i++){
    randomx = rand() %  (win_w - INITIAL_TREE_SIZE) + INITIAL_TREE_SIZE;
    randomy = rand() %  (win_h - INITIAL_TREE_SIZE) + INITIAL_TREE_SIZE;
    
    trees.push_back(new Square(0,points,offsetLoc,sizeLoc,colorLoc));
    trees[i]->change_size(INITIAL_CHARACTER_SIZE);
    trees[i]->move(randomx,randomy-INITIAL_CHARACTER_SIZE);
    trees[i]->color(TREE_COLOR);
    trees[i]->selectColor(nextselectioncolor());
  }


}

void clearmap(){
  for (auto character : invaders){
    delete character;
  }
  for (auto character : civilians){
    delete character;
  }
  for (auto object : trees){
    delete object;
  }
  for (auto object : food){
    delete object;
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
  if(COLLISON_DETECTION_ON){
    if (character.get_pos().y >= win_h - character.get_size() - amount ){ // character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(will_character_hit_tree(character,*tree,amount,0,0,0)){return false;}
    }
  }
  return true; 
}
//function to determine characters ability to move downward a given amount
bool can_move_down(Square character,int amount){
  if(COLLISON_DETECTION_ON){
    if (character.get_pos().y <= 0 + character.get_size() + amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(will_character_hit_tree(character,*tree,0,amount,0,0)){return false;}
    }
  }
  return true;
}
bool can_move_left(Square character,int amount){
  if(COLLISON_DETECTION_ON){  
    if( character.get_pos().x <= 0 + character.get_size() + amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(will_character_hit_tree(character,*tree,0,0,amount,0)){return false;}
    }
  }
  return true;
}
bool can_move_right(Square character,int amount){
  if(COLLISON_DETECTION_ON){
    if(character.get_pos().x >= win_w - character.get_size() - amount ){// character is at edge of window
      return false;
    }
    for (auto tree : trees){
      if(will_character_hit_tree(character,*tree,0,0,0,amount)){return false;}
    }
  }
  return true;
}

//moves invaders randomly TODO make them struggle toward the food 
void move_invaders(int amount){
   srand(time(NULL)); 
    for(auto character: invaders){
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

bool will_character_hit_tree(Square character,Square tree,
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
  if(stopwatch.get_time() > secsBetweenDrops){
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
  stopwatch.reset(); 
  }
}
// Will make food appear at random xes and ys  
// on the screen at the initail size + a given size 
void spawn_food(int size){
  srand(time(NULL));
  int randomx = rand() % (win_w-50) + 50 ;
  int randomy = rand() % (win_h-50) + 50;

  food.push_back(new Square(0,points,offsetLoc,sizeLoc,colorLoc));
  food[food.size()-1]->color(foodColor);
  food[food.size()-1]->change_size(INITIAL_FOOD_SIZE + size);
  food[food.size()-1]->move(randomx,randomy+INITIAL_FOOD_SIZE);
  food[food.size()-1]->selectColor(nextselectioncolor());
}


bool did_character_get_food(Square character, Square food){
  GLfloat tree_x = food.get_pos().x;
  GLfloat tree_y = food.get_pos().y;
  GLfloat character_x = character.get_pos().x;
  GLfloat character_y = character.get_pos().y;

  GLfloat treeleftmost_x = tree_x - food.get_size();
  GLfloat treerightmost_x = tree_x + food.get_size();
  GLfloat treebottommost_y = tree_y - food.get_size();
  GLfloat treetopmost_y = tree_y + food.get_size();

  GLfloat characterleftmost_x = character_x - character.get_size();
  GLfloat characterrightmost_x = character_x + character.get_size();
  GLfloat characterbottommost_y = character_y - character.get_size();
  GLfloat charactertopmost_y = character_y + character.get_size();

  bool characteronsame_xes = characterrightmost_x > treeleftmost_x && characterleftmost_x < treerightmost_x;
  bool characteronsame_ys = charactertopmost_y > treebottommost_y && characterbottommost_y < treetopmost_y;

  if(characteronsame_ys && characteronsame_xes){
    return true;
  }
  return false;
}


int main(int argc, char** argv){
  // Several people forgot to put in the following line.  This is an
  // error, even if it still works on your machine, a program is
  // incorrect without the following call to initialize GLUT.
  glutInit(&argc,argv);
  
  glutwindowinit();
  // Initialize the "magic" that glues all the code together.
  glewInit();

  buffersetup();   // set up shaders and display environment

  glutMainLoop();       // enter event loop
  
  return(EXIT_SUCCESS); // return successful exit code
}
