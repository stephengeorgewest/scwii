#include <stdlib.h>
#include <time.h>
#include <wiiuse/wpad.h>

#include <grrlib.h>

#include <noise/noise.h>
#include "noiseutils.h"
using namespace noise;


// Font
#include "FreeMonoBold_ttf.h"

bool quitapp=false;

int main(void)
{
	// Initialise the Graphics & Video subsystem
	GRRLIB_Init();

	// Initialise the Wii Remotes
	WPAD_Init();

	// Load the font from memory
	GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
	// Create an empty texture to store a copy of the screen
	GRRLIB_texImg *CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

	// To have a cool effect anti-aliasing is turned on
	GRRLIB_Settings.antialias = true;

	// Black background
	GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
	

	int image_height= rmode->efbHeight;//256;//
	int image_width = rmode->fbWidth;//256;//
	srand(time(0));
	const float RAND_RANGE = 10.0;
	
	float r = (float)rand()/(float)RAND_MAX*(RAND_RANGE);
	module::Perlin myModule;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (myModule);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize (image_width, image_height);
	heightMapBuilder.SetBounds (r+0.0, r+4.8, r+0.0, r+6.4);
	//heightMapBuilder.EnableSeamless (true);
	heightMapBuilder.Build ();
	
	utils::RendererImage renderer;
	utils::Image image;
	
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	//http://www.allscoop.com/tools/Web-Colors/web-colors.php?abc=%23bbbbff
	renderer.AddGradientPoint (-1.0000, utils::Color (0x11, 0x00, 0x77, 255)); // deeps
	renderer.AddGradientPoint (-0.3000, utils::Color (0x11, 0x00, 0x77, 255)); // deeps
	renderer.AddGradientPoint (-0.1500, utils::Color (0x99, 0xcc, 0xff, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000, utils::Color (0x00, 0x7f, 0xff, 255)); // shore
	renderer.AddGradientPoint ( 0.0625, utils::Color (0xff, 0xff, 0xdd, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color (0x33, 0x99, 0x00, 255)); // grass
	renderer.AddGradientPoint ( 0.4250, utils::Color (0x00, 0x77, 0x00, 255)); // forest
	renderer.AddGradientPoint ( 1.0000, utils::Color (0x44, 0x44, 0x00, 255)); // dirt
	renderer.AddGradientPoint ( 1.4000, utils::Color (0xaa, 0xaa, 0xaa, 255)); // rock
	renderer.AddGradientPoint ( 1.6000, utils::Color (0xff, 0xff, 0xff, 255)); // snow
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	//renderer.EnableWrap (true);
	renderer.SetLightBrightness (2.0); // Double the brightness
	//renderer.SetLightAzimuth();
	//renderer.SetLightElevation();
	//renderer.SetLightColor();
	renderer.Render ();
	float angle = 0.0;
	float mag = 0.0;

	while(!quitapp)
	{
		WPAD_ScanPads();  // Scan the Wii Remotes
		
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
		{
			//puts("Quitting");
			quitapp=true;
			break;
		}
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)
		{//Render A new Map
			float r = (float)rand()/(float)RAND_MAX*(RAND_RANGE);
			heightMapBuilder.SetBounds (r+0.0, r+4.8, r+0.0, r+6.4);
			//heightMapBuilder.EnableSeamless (true);
			heightMapBuilder.Build ();
			
			renderer.SetLightBrightness (1.0+mag); // Double the brightness
			renderer.SetLightAzimuth(angle-90.0);
			renderer.SetLightElev((1-mag)*85.0);
			utils::Color c ((noise::uint8)0xFF,
							(noise::uint8)((float)(0xFF)*(1.0-mag/4)),
							(noise::uint8)((float)(0xFF)*(1.0-mag/2)),
							(noise::uint8)0xFF);
			renderer.SetLightColor(c);
			renderer.Render ();
		}
		
		WPADData* wpd =  WPAD_Data(0);
		u32 exp_type;
        if ( WPAD_Probe(WPAD_CHAN_0, &exp_type) != 0 )
                exp_type = WPAD_EXP_NONE;
		if(exp_type == WPAD_EXP_NUNCHUK)
		{
			if(WPAD_ButtonsHeld(0) & WPAD_NUNCHUK_BUTTON_C)
			{//Adjust The Lighting
				mag = wpd->exp.nunchuk.js.mag;
                angle = wpd->exp.nunchuk.js.ang;
				renderer.SetLightBrightness (1.0+mag); // Double the brightness
				renderer.SetLightAzimuth(angle-90.0);
				renderer.SetLightElev((1-mag)*85.0);
				utils::Color c ((noise::uint8)0xFF,
								(noise::uint8)((float)(0xFF)*(1.0-mag/4)),
								(noise::uint8)((float)(0xFF)*(1.0-mag/2)),
								(noise::uint8)0xFF);
				renderer.SetLightColor(c);
				renderer.Render ();
			}
		}
		for(int y=0; y<rmode->efbHeight;y++)
		for(int x=0; x<rmode->fbWidth;x++)
		{
	#define GRRLIB_RED     0xFF000000
	#define GRRLIB_GREEN   0x00FF0000
	#define GRRLIB_BLUE    0x0000FF00
	#define GRRLIB_ALPHA   0x000000FF
			utils::Color c = image.GetValue(x%image_width,y%image_height);
			GRRLIB_Plot(x, y, c.red<<24 | c.green<<16 | c.blue<<8 | c.alpha);
		}
		//GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
		if(WPAD_ButtonsHeld(0) & WPAD_NUNCHUK_BUTTON_C)
		{
			char str[128];
			sprintf(str, "Azimuth = %f, Elevation = %f", angle-90.0, (1.0-mag)*85 );
			GRRLIB_PrintfTTF(50,
						rmode->efbHeight - 50,
						myFont,
						str,
						20,
						0xFFFFFFFF);
		}
		GRRLIB_Render();  // Render the frame buffer to the TV
	}


	GRRLIB_FreeTexture(CopiedImg);
	GRRLIB_FreeTTF(myFont);
	GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB


	return(0);
}