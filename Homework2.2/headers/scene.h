#include <Angel.h>
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"
#include "camera.h"

// struct shapedata{
// 	vector<point4> AllVertices;
// 	vector<color4> AllColors;
// 	vector<norm4> AllNormals;
// };

class Scene{
	public:
		void init();
		mat4 GeneratePrespectiveMatrix();
		
		GLfloat incr=0.1;
		// For perspective camera
		GLfloat fovy=45;
		GLfloat aspect=1.0;
		GLfloat angle = 0.0;

		// Camera and view parameters
		GLfloat zNear = 0.1;
		GLfloat zFar = 300.0;
	

		bool rotatep=false;          // whether to rotate or not

		const GLfloat dr = 90.0 * DegreesToRadians;

		

		GLint  camera_view_loc;// camera-view matrix uniform shader variable location
		GLint  model_view_loc; // model-view matrix uniform shader variable locatio
		GLint  projection_loc; // projection matrix uniform shader variable location
		GLint  shade_loc;  // shade uniform shader variable location


				// Implementing a door that opens by swinging
		GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
		GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
						// door's rotation.
		GLfloat trans = 0.0;   // trans of 2nd cube.
		GLfloat transinc = 1.0;// trans increment of 2nd cube.

		/// end temp public
		
		cube* mycube2 ;
		cube_door* mycube;
		sphere* mysphere;

		Camera camera;
		
	private:
		vector<point4> AllVertices;
		vector<color4> AllColors;
		vector<norm4> AllNormals;

		


};

