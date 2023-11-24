#pragma once

// Debug Options
#define DEBUG_ENABLED 1

// Windows Settings
#define WINDOW_X_RESOLUTION 1366
#define WINDOW_Y_RESOLUTION 768
#define V_SYNC 1
#define FULLSCREEN_MODE 0 // If set, window resolution is maximized to your screen resolution

// render settings
#define FORWARD_RENDER 0

// AA Settings
#define SUPERSAMPLING_FACTOR 1 //Should be a multiple of 2 (1 means window resolution will be the render resolution)
#define MSAA_SAMPLE_AMOUNT 4  //only used in forward rendering

//Texture Filtering settings
#define ANISOTROPIC_FILTERING_LEVEL 8.0f

// IBL Settings
#define LIGHT_PROBE_RESOLUTION 32
#define REFLECTION_PROBE_MIP_COUNT 5
#define REFLECTION_PROBE_RESOLUTION 128
//should always be greater than the light and reflection probe resolution
#define IBL_CAPTURE_RESOLUTION 256 
#define BRDF_LUT_RESOLUTION 512

// Frustum Options
#define NEAR_PLANE 0.1f
#define FAR_PLANE 2000.0f

//Shadowmap options
#define  SHADOWMAP_RESOLUTION_X 2048
#define  SHADOWMAP_RESOLUTION_Y 2048
#define  SHADOWMAP_NEAR_PLANE 0.1f
#define  SHADOWMAP_FAR_PLANE 400.0f

//SSAO Options
// Maximum amount is restricted by the shader. Only supports a maximum of 64
#define SSAO_KERNEL_SIZE 32

#define SAFE_DELETE(p) if(p){delete p;p = nullptr;} void(0)
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p;p = nullptr;}
#define MACRO_STRING(p) #p

