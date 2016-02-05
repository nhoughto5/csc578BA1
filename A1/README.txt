==== CSC 578B Assignment 1 README ====
Nick Houghton - V00151783

*There is a video demonstrating the working simulations which can be viewed
on youtube at https://youtu.be/9nybH4hXwyU


The solution provided for assignment 1 is comprised of a single project.

The SpringScene.cpp file provides and instantiation of both the linear spring and the angular spring solutions which are to be run 
one at a time via uncommenting the required lines in the RenderScene and UpdateScene functions.

Both solutions employ a class an ObjectGenerator which is used to create the desired spring-mass models.
The ObjectGenerator uses a class called 'ShapeData' which is used to store vertex, colour, indices and other important data and transport
them to the needed locations in a clean and concise manner. 

The linear spring simulation is begun by intializing the 'displacement' vector in the initializer list of the constructor in the Spring class.
This displacement causes the initial motion. The force of gravity and the force of the spring acting acting on the mass are used to calculate
not only the displacement of the mass and the nodes of the spring but they are also used to compute the amount the spring's nodes seperate
from each other under force as well as how they move in towards the central axis of the spring. These additional computations create
a far more realistic spring simulation. Once these values are computed the ObjectGenerator is used to recompute the vertex data which is then 
written over the previous data in the buffer.

The angular spring rotates at a constant rate. The period of rotation, length of the spring (radius) and weight of the mass are used
to calculate the resulting centrifugal force. The summation of the centrifugal force and the centripital force acting along the spring is used
to calculate the dispalcement, stretch and width of the spring as was done in the linear spring. The shorter the rotational period the 
greater the centrifugal force which further stretches the spring. 
	