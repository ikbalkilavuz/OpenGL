#version 150
out vec4 fColor;
in vec4 Color;
in float fRotation;
in float dColor;
float Rotation;

void main()
{ 
	/* if change color is  active and if it is on the middle change darker*/
	if(dColor==1){
		/* if it is color of ninja star, set color darker  and lighter*/
		Rotation=fRotation+0.3;
		if(fRotation<=0){
			Rotation=(-1*fRotation)+0.3;
		}
		if(Color.x==0.5 && Color.y==0.5 && Color.z == 0.5){
			fColor = vec4(Color.x*Rotation,Color.y*Rotation,Color.z*Rotation,1.0);
		}else{
			fColor = Color;
		}
		}
	else{
		fColor = Color;
	}

	
}