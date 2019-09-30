During this first openGl programming assignment many things went wrong. I 
believe I still ended up with something cool in the end. My initial idea was 
to make a teater totter given lines.cc as the starting code. I began by learing
more about how the buffers are managed in openGl and after that began to draw
some simple primatives. In the beginning I was trying to use the glbuffersubdata
function in order to retain some orginazition in my code by using multiple arrays.
The problem seemed to be that this function was not sending the verts to be drawn by 
the GlDrawArrays function inside of the callback. In order to fix this issue i decided 
to go with a simple array with all of the information in it. 
    The bulk of my time after figuring out how to get multiple primatives on the screen 
was spent trying to find out how I could do something cool. I initially thought it 
may be a cool idea to change the shaders inbetween the callback to redraw. But I dont 
believe this is a supported ability. So after some tikering i decided to make my shapes 
disappear and reappear depending on how often the screen was redrawn. At first this 
anamation(if you can even call it that) was very fast and sometimes you couldnt even 
see the different shapes appearing but I added logic so that it would only draw new shapes 
on every 10 callbacks. So now when you drag the window to make it larger or smaller my shapes 
pop up and disappear.
    The assignment was very interesting and I had a good time coding what i did but I am much more
excited to see what 3D is like in comparison. 
