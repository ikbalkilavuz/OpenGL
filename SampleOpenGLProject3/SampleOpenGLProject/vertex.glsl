#version 150
in vec4 vPosition;	
in vec4 vColor;	  
out vec4 Color;				


uniform float spiralDirection;  /* if spiral direction will be negatif,it will change the direction of spiral movement */
uniform float rotation;			/* how much it will be rotated  */
uniform float rotate;			/* rotation start button is pressed?  */
uniform float Scale;			/* is scaling start? */
uniform float ScaleNumber;		/* how much it will be scaled */
uniform float Spiral;			/* is spiral move start?  */
uniform float AngleSpiral;		/* how much it will be spiralling */
			  
void main()
{


    		mat4 scaling = mat4( 1.0, 0.0,  0.0, 0.0,
		    					 0.0, 1.0,  0.0, 0.0,
		    					 0.0, 0.0,  1.0, 0.0,
		   						 0.0, 0.0,  0.0, 1.0 );

		   	mat4 translation=scaling;
		   	mat4 rz=scaling;

			/* scale star like beating heart */
			if(Scale==1){

			/* scaling matris is creating with number of coming from application */
		    	scaling = mat4( ScaleNumber, 0.0,  0.0, 0.0,
		    						 0.0, ScaleNumber,  0.0, 0.0,
		    						 0.0, 0.0,  1.0, 0.0,
		   							 0.0, 0.0,  0.0, 1.0 );


			}


			/* move the star like spiral */
			if(Spiral==1){
	
				float angles= radians( AngleSpiral );
      			 /* sin and cos values calculating with angle value  */
    			float c = cos( angles );
    			float s = sin( angles );

				/* translation matris is creating */
		   		float r = 0.00095 * AngleSpiral;   /* it is radian of spiral movement.it wont touch the window edges  */
		   		float x ;	
		   		float y;
		   		if(spiralDirection>0){
		   			x = spiralDirection * r * s;
		   			y = spiralDirection * r * c;
		   		}else{
		   			 x = spiralDirection * r * c;
		   			 y = spiralDirection * r * s;	
		   		}

		   		translation = mat4( 1.0, 0.0,  0.0, 0.0,
		    						 0.0, 1.0,  0.0, 0.0,
		    						 0.0, 0.0,  1.0, 0.0,
		   							 x, y,  0.0, 1.0 );

			}


			/* rotate x and y direction with sin and cos value*/
			if(rotate==1){
				float angles= radians( rotation );
      			 /* sin and cos values calculating with angle value  */
    			float c = cos( angles );
    			float s = sin( angles );
			   	/* rotation z matris is creating */
			   	rz = mat4( c, -s, 0.0, 0.0,
		   						s,  c, 0.0, 0.0,
		  				 	    0.0, 0.0, 1.0, 0.0,
						  	    0.0, 0.0, 0.0, 1.0 );
	

			}

			gl_Position = translation * rz * scaling * vPosition;
			Color = vColor;
}