#version 150
in vec4 vPosition;	
in vec4 vColor;	
out float fRotation;   
out vec4 Color;			
out float dColor;		

uniform float changeColor;    /* key=3 is pressed?  */
uniform float rotation;			/* how much it will be rotated  */
uniform float rotate;			/* key=2 is pressed?  */
void main()
{
			/* rotate x and y direction with sin and cos value*/
			if(rotate==1){
				float x = (vPosition.x * cos(rotation)) - (vPosition.y * sin(rotation));
				float y = (vPosition.x * sin(rotation)) + (vPosition.y * cos(rotation));
				gl_Position.x=x;
				gl_Position.y=y;
				gl_Position.z = vPosition.z;
				gl_Position.w = vPosition.w;
			}
			else{
			gl_Position=vPosition;
			}
			dColor = changeColor;
			Color = vColor;
			fRotation = rotation;
}