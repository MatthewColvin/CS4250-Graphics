#include <Angel.h>
#include "common.h"
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"
#include "camera.h"

class Scene{
	public:
		void init();

		//// temp public 


		GLfloat incr=0.1;
		// For perspective camera
		GLfloat fovy=45;
		GLfloat aspect=1.0;

		GLfloat radius=15.0;
		GLfloat angle = 0.0;
		GLfloat cameraangle = 0.0;


		GLfloat left = -1.0, right = 1.0;
		GLfloat bottom = -1.0, top = 1.0;

		// Camera and view parameters
		GLfloat zNear = 0.1;
		GLfloat zFar = 300.0;

		GLfloat mvx=0.0;
		GLfloat mvy=0.0;
		GLfloat mvz=0.0;

		bool rotatep=false;          // whether to rotate or not

		const GLfloat dr = 90.0 * DegreesToRadians;



		GLint  camera_view;// camera-view matrix uniform shader variable location
		GLint  model_view; // model-view matrix uniform shader variable locatio
		GLint  projection; // projection matrix uniform shader variable location
		GLint  shade_loc;  // shade uniform shader variable location


				// Implementing a door that opens by swinging
		GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
		GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
						// door's rotation.
		GLfloat trans = 0.0;   // trans of 2nd cube.
		GLfloat transinc = 1.0;// trans increment of 2nd cube.

		/// end temp public

		cube_door mycube;
		cube mycube2;
		sphere mysphere;

		Camera camera(){return scenecamera;}
	private:
		
		Camera scenecamera;


};

