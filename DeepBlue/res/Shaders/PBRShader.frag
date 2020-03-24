#version 420 core
out vec4 colour;

in vec2 TexCoord0;
in vec3 Normal;
in vec3 VecPosition;

#define USE_UNREAL_MODEL
#define USE_GAMMA_CORRECTION
#define HIGH_SCENE
//#define MEDIUM_SCENE
//#define LOW_SCENE

struct PointLight{
	vec3 LightColour;
	float LightIntensity;
	vec3 LightPos;
	float LightConstant;
	float LightLinear;
	float LightExp;
};

struct DirectionalLight{
	vec3 LightColour;
	float LightIntensity;
	vec3 LightDir;
};

layout(binding=0) uniform sampler2D u_AlbedoTexture;
layout(binding=1) uniform sampler2D u_RoughnessTexture;
layout(binding=2) uniform sampler2D u_MetalnessTexture;
layout(binding=3) uniform sampler2D u_NormalMap;

#ifdef LOW_SCENE
	uniform PointLight u_pointLights[1];
	const unsigned int NUM_POINT_LIGHT = 1;
	const unsigned int NUM_DIRECTIONAL_LIGHT = 0;

#elif defined(MEDIUM_SCENE)
	uniform PointLight u_pointLights[2];
	//uniform DirectionalLight u_directionalLights[1];
	const unsigned int NUM_POINT_LIGHT = 2;
	const unsigned int NUM_DIRECTIONAL_LIGHT = 0;

#elif defined(HIGH_SCENE)
	uniform PointLight u_pointLights[3];
	uniform DirectionalLight u_directionalLights[1];
	const unsigned int NUM_POINT_LIGHT = 3;
	const unsigned int NUM_DIRECTIONAL_LIGHT = 1;

#else
	uniform PointLight u_pointLights[1];
	const unsigned int NUM_POINT_LIGHT = 1;
	const unsigned int NUM_DIRECTIONAL_LIGHT = 0;
#endif	

  /*========================================================
							Constants
  ========================================================*/
	float PI = 3.141592;
	float epsilon = 0.0001;

  /*========================================================
							Diffuse
  ========================================================*/

float DIFF_Lambert(){
	return (1.0/PI);
}

float DIFF_Oren_Nayar(float R, vec3 N, vec3 L, vec3 V){
	R = R * 90; //Remapping
	R = (R*PI)/180.0; //Angle to rads
	float Rsqr = R * R;
	float A = 1.0 - 0.5 * (Rsqr / (Rsqr + 0.33));
	float B = 0.45 * (Rsqr/(Rsqr + 0.09));
	
	float sigmaL = acos(dot(N, L));
	float sigmaV = acos(dot(N, V));

	float alpha = max(sigmaL, sigmaV);
	float beta = min(sigmaL, sigmaV);

	vec3 phiVecV = normalize(V - N * dot(N, V));
	vec3 phiVecL = normalize(L - N * dot(N, L));

	return (1/PI) * (A + B * max(dot(phiVecL, phiVecV), 0.0) * sin(alpha) * tan(beta)); // * dot(N, L)
}

  /*========================================================
				  Normal Distribution Functions
  ========================================================*/

float D_GGX(vec3 N, vec3 H, float r){

	float rsqr = r * r;

	float NdotHsqr =  dot(N, H) * dot(N, H);

	float c = (NdotHsqr * (rsqr - 1) + 1);

	return (rsqr / (PI * c * c));
}

float D_Beckmann(vec3 N, vec3 H, float r){
	
	r += epsilon;

	float rsqr = r * r;

	float NdotHsqr = dot(N, H) * dot(N,H);

	float c = NdotHsqr * NdotHsqr;

	float denom = (PI * rsqr * c);

	float nom = exp((NdotHsqr - 1) / (rsqr * NdotHsqr));

	return (nom / (denom));
}

  /*========================================================
					  Fresnel Functions
  ========================================================*/

vec3 F_SchlickSGA(vec3 V, vec3 H, vec3 F0){

	float VdotH = dot(V,H);

	float sga = pow(2.0, (-5.55473 * VdotH - 6.98316) * VdotH);

	return F0 + (1- F0) * sga;
}

vec3 F_Schlick(vec3 V, vec3 H, vec3 F0){

	float VdotH = dot(V,H);

	float ex = pow((1.0 - VdotH), 5.0);

	return F0 + (1- F0) * ex;
}

  /*========================================================
					Geometry Shadowing Function
  ========================================================*/

float G1_SchlickGGX(vec3 N, vec3 v, float k){

	float NdotV = dot(N,v);
	
	float denom = NdotV * (1 - k) + k;
	
	return NdotV / denom;
}

float G_SchlickGGX(float R, vec3 N, vec3 L, vec3 V){

	float k = pow((R + 1), 2.0) / 8;
	
	float G1 = G1_SchlickGGX(N, L, k);
	
	float G2 = G1_SchlickGGX(N, V, k);
	
	return G1*G2;
}

float G1_SchlickBeckmann(vec3 N, vec3 v, float k){ //Same as G1_SchlickGGX
	
	float NdotV = dot(N,v);
	
	float denom = NdotV * (1 - k) + k;
	
	return NdotV / denom;
}

float G_SchlickBeckmann(float R, vec3 N, vec3 L, vec3 V){

	float k  = R * sqrt(2/PI);
	
	float G1 = G1_SchlickBeckmann(N, L, k);
	
	float G2 = G1_SchlickBeckmann(N, V, k);
	
	return G1*G2;
}

void main(){
	
	/*========================================================
							PBR Variables
	========================================================*/
	vec3 N = normalize(Normal);

	vec3 V = normalize(VecPosition * -1);
	
	//Albedo
	vec4 albedo = texture2D(u_AlbedoTexture, TexCoord0.st);
	
	//Roughness
	float roughness = texture2D(u_RoughnessTexture, TexCoord0.st).x;
	
	//Metalness
	float metalness = texture2D(u_MetalnessTexture, TexCoord0.st).x;
	
	//Alpha remapping a la Disney and Unreal 
	float alpha = roughness * roughness;
	
	//Gamma value
	float gamma = 2.2;
	
	//Normal Distribution Function value
	vec3 D = vec3(1.0, 1.0, 1.0);
	
	//Fresnel value
	vec3 F = vec3(1.0, 1.0, 1.0);
	vec3 F0 = vec3(0.04);
	
	//Geometric shadowing value
	vec3 G = vec3(1.0, 1.0, 1.0);
	
	//Debugging Variables
	float r = 0.3;
	float alpha2 = r * r;
	float metalness2 = 0.0;
		

	/*========================================================
						PBR CALCULATIONS
	========================================================*/
	vec3 radiance = vec3(0.0);
	
	//Go through all lights and calculate contribution/colour
	for(int i = 0; i < NUM_POINT_LIGHT; i++){
		vec3 L = normalize(u_pointLights[i].LightPos - VecPosition);
		//vec3 L = normalize(lightPos);

		vec3 H = normalize( L + V );
		
		//Light intensity
		float intensity = u_pointLights[i].LightIntensity; //100.0

		float d = length(u_pointLights[i].LightPos - VecPosition); //DONT USE L, it's always normalized, meaning length is always one.............FML
		intensity *= 1/(u_pointLights[i].LightConstant + u_pointLights[i].LightLinear * d  + u_pointLights[i].LightExp * (d * d));
		
		//Check if light is in front of surface
		float visible = clamp(dot(N, L), 0.0, 1.0);

		//NDF
		#ifdef USE_UNREAL_MODEL
			D *= D_GGX(N, H, alpha);
		#else
			D *= D_Beckmann(N, H, alpha);
		#endif
		
		//F0 using constant colour, for textures ues mix(F0, albedo.rgb , metalness2);
		//For colour: F0 = mix(F0, vec3(0.98, 0.75294, 0.11765) , metalness);
		F0 = mix(F0, albedo.rgb , metalness); //vec3(0.9, 0.6, 0.05)

		//Fresnel
		#ifdef USE_UNREAL_MODEL
			F *= F_SchlickSGA(V, H, F0);
		#else
			F *= F_Schlick(V, H, F0);
		#endif

		//Geometric Shadowing
		#ifdef USE_UNREAL_MODEL
			G *= G_SchlickGGX(alpha, N, L, V);
		#else
			G *= G_SchlickBeckmann(alpha, N, L, V);
		#endif

		//Diffuse fraction
		// (1-F) gives us the light that is not reflected,
		// (1- metalness), acts like a switch between dielectric and metal surfaces.
		// Metal surfaces have no or very small diffuse values
		vec3 kd = (vec3(1.0) - F) * (1.0 - metalness);

		//Specular component using Cook-Torrance BRDF
		vec3 spec = (D * F * G) / (4 * dot(N, V) * dot(N, L));


		//Diffuse component
		#ifdef USE_UNREAL_MODEL
			vec3 diff = kd * albedo.rgb * DIFF_Lambert(); //(alpha2, N, L, V); // vec3(0.9, 0.6, 0.05) albedo.rgb
		#else
			vec3 diff = kd * albedo.rgb * DIFF_Oren_Nayar(alpha, N, L, V);
		#endif

		//Complete shading model
		vec3 model = (diff + spec) * (u_pointLights[i].LightColour);

		//Colour only if the light can be seen from the position
		model *= visible;

		//vec3 light_colour = vec3(0.99216, 0.98431, 0.82745);
		radiance += model * intensity;
	}

	// Calculate final colour including gamma correction and light intensity
	#ifdef USE_GAMMA_CORRECTION
		colour = vec4(pow(radiance, vec3(1/gamma)), albedo.a); //albedo.a alpha = 1.0
	#else
		colour = vec4(radiance, 1.0);
	#endif
	
	// !Uncomment to debug singular components!
	//colour = vec4( pow(vec3(1.0, 1.0, 1.0) * intensity * visible * D_Beckmann(N,H,r), vec3(1.0/gamma)), 1.0);
}