varying vec3 lightDir,normal;
uniform sampler2D Texture0;

void main()
{

	float intensity;
	
	
	vec4 color =  texture2D( Texture0 , vec2( gl_TexCoord[0] ) );
	// normalizing the lights position to be on the safe side
	
	vec3 n = normalize(normal);
	
	intensity = dot(lightDir,n);
	
	if (intensity > 0.5)
		color *= vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.15)
		color *= vec4(0.5,0.5,0.5,1.0);
	else
		color *= vec4(0.2,0.2,0.2,1.0);
	
	gl_FragColor = color;
} 