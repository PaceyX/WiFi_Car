#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"



#define OV9650_I2C_ADDR_W       0x60
#define OV9650_I2C_ADDR_R       0x61

#define SENSOR_GAIN_SCALE       16

LOCAL uint32 set_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 OV9650_Identify(uint32 param);
LOCAL uint32 OV9650_BeforeSnapshot(uint32 param);

LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);

LOCAL uint32 s_preview_mode;


#if 0

__align(4)const SENSOR_REG_T ov9650_RGB_COMMON[]=
{
    {0x12,0x80},
    {SENSOR_WRITE_DELAY,0x14},      // delay 20ms
    {0x37,0x00},                                            
    {0x0e,0x20},   
    {0x1e,0x04},//0x34   //this control flip mirror             
                                                    
    {0x01,0x80},                                            
    {0x02,0x80},                                            
    {0x00,0x00},                                            
    {0x10,0xf0},                                            
    //                                                  
    {0x04,0x00},                                            
    {0x0c,0x04},                                            
    {0x0d,0x80},                                            
    {0x11,0x80},//0x81//@20050713                                   
    {0x14,0x2e},                                            
    {0x15,0x00},                                            
    {0x32,0xad},                                            
                                        
    {0x3f,0xa6},                                            
    {0x41,0x02},                                            
    {0x42,0x08},                                            
    //                                                  
    {0x1b,0x00},                                            
    {0x16,0x06},                                            
    {0x33,0xc0},                                            
    {0x34,0xbf},                                            
    {0xA8,0x80},                                            
    {0x96,0x04},                                            
    {0x3a,0x0c},                                            
    {0x8e,0x00},                                            
    //                                                  
    {0x3c,0x77},                                            
    {0x8B,0x06},                                            
    {0x35,0x91},                                            
    {0x94,0x88},                                            
    {0x95,0x88},                                            
    {0x40,0xc1},                                            
    {0x29,0x3f},                                            
    {0x0f,0x42},                            

    // color matrix disable, xg feng                                                
    {0x3d,0x02},
                                                    
    {0x69,0x80},                                            
    {0x5C,0x96},                                            
    {0x5D,0x96},                                            
    {0x5E,0x10},                                            
    {0x59,0xeb},                                            
    {0x5A,0x9c},                                            
    {0x5B,0x55},                                            
    {0x43,0xf0},                                            
    {0x44,0x10},                                            
    {0x45,0x55},                                            
    {0x46,0x86},                                            
    {0x47,0x64},                                            
    {0x48,0x86},                                            
    {0x5F,0xF0},                                            
    {0x60,0x8c},                                            
    {0x61,0x20},                                            
    //                                                  
    {0xa5,0xd9},                                            
    {0xa4,0x74},                                            
    {0x8d,0x02},                                            
    //          
    // ae enable, awb disable, agc enable, xg feng                                      
    {0x13,0x00},
         
    //                                                  
    {0x4f,0x3a},                                            
    {0x50,0x3d},                                            
    {0x51,0x03},                                            
    {0x52,0x08},                                            
    {0x53,0x26},                                            
    {0x54,0x30},                                            
    {0x55,0x40},                                            
    {0x56,0x40},                                            
    {0x57,0x40},                                            
    {0x58,0x0d},                                            
    //
    // denoise disable, xg feng                                                 
    {0x8c,0x00},                                            
    {0x3E,0x02},                                            
    {0xa9,0xb8},                                            
    {0xaa,0x92},                                            
    {0xab,0x0a},                                            
    //                                                  
    {0x8f,0xdf},                                            
    {0x90,0x00},                                            
    {0x91,0x00},                                            
    {0x9f,0x00},
    // blc disable, xg feng                                             
    {0x3A,0x00},    
    //        
    // sensor ae, xg feng                                           
    {0x24,0x90},                                            
    {0x25,0x88},                                            
    {0x26,0xc6},
    
    {0x3a,0x08},                                        
    {0x3b,0x19},                                            
    //                                                  
    // 1:1 gamma curver, xg feng                                            
    {0x6c,0x40},                                               
    {0x6d,0x40},                                               
    {0x6e,0x40},                                               
    {0x6f,0x40},                                                
    {0x70,0x40},                                               
    {0x71,0x40},                                               
    {0x72,0x40},                                               
    {0x73,0x40},                                               
    {0x74,0x40},                                               
    {0x75,0x40},                                               
    {0x76,0x40},                                               
    {0x77,0x40},                                               
    {0x78,0x40},                                               
    {0x79,0x40},                                               
    {0x7a,0x40},                                               
    {0x7b,0x40},                                               
    {0x7c,0x04},                                               
    {0x7d,0x08},                                               
    {0x7e,0x10},                                               
    {0x7f,0x20},                                                
    {0x80,0x28},                                               
    {0x81,0x30},                                               
    {0x82,0x38},                                               
    {0x83,0x40},                                               
    {0x84,0x48},                                               
    {0x85,0x50},                                               
    {0x86,0x60},                                               
    {0x87,0x70},                                               
    {0x88,0x90},                                               
    {0x89,0xB0},                                              
    {0x8a,0xD0},
    
    {0x64,0x0d},                                            
    {0x65,0x30},                                            
    {0x66,0x00},
    
    {0x39,0x50},
    {0x38,0x92},    

        
    {0x3f,0xa6},                                            
    {0x41,0x02},
    {0x14,0x2e},    
     
    {0x11,0x80},
    
    {0x12,0x45},         
    {0x03,0x00},
    
    {0x2a,0x00},//10 for 50hz        
    {0x2b,0x00},//40 for 50hz        

        
    {0x03,0x00},
    {0x00,0x00},

    //initial ev=200 //@0713,lxw
    {0xA1,0x00},
    {0x10,0x32},
      

   
};
__align(4) const SENSOR_REG_T ov9650_RGB_1280X1024[]=
{
//OV9650
//SXGA RGB
//15fps at 12MHz input clock
//60Hz light frequency
//Device Address(Hex)/Register(Hex)/Value(Hex)
    {0x6a, 0x9d},
    {0x12, 0x05},
    {0x04, 0x00},
    {0x0c, 0x00},
    {0x0d, 0x00},
    {0x18, 0xbb},
    {0x17, 0x1b},
    {0x32, 0xa4},//added by xiaowei.luo,@2006.11.29
    
    {0x1a, 0x81},
    {0x03, 0x12},
};

__align(4)const SENSOR_REG_T ov9650_RGB_640X480[] =
{

//  OV9650
//  VGA RGB
//  30fps when 12MHz input clock
//  60Hz light frequency
//  Device Address(Hex)/Register(Hex)/Value(Hex)

    {0x6a, 0x3e},                                   
    {0x12, 0x45},                                   
    {0x04, 0x00},                                   
    {0x0c, 0x04},                                   
    {0x0d, 0x80},                                   
    //{0x18, 0xc7},                                 
    //{0x17, 0x27},
    {0x18, 0xc6},
    {0x17, 0x26},
    {0x32, 0xad}, //added by xiaowei.luo,@2006.11.29            
    {0x1a, 0x3d},                                   
    {0x03, 0x00},                                   



};

LOCAL SENSOR_REG_TAB_INFO_T s_OV9650_resolution_Tab_RAWRGB[]=
{
    // COMMON INIT  
    { ADDR_AND_LEN_OF_ARRAY(ov9650_RGB_COMMON),         0,      0,      12 },
        
    // RAWRGB PREVIEW 1
    { ADDR_AND_LEN_OF_ARRAY(ov9650_RGB_640X480),        640,    480,    24 },
    { ADDR_AND_LEN_OF_ARRAY(ov9650_RGB_1280X1024),      1280,   1024,   24 },   
    { PNULL,                    0,      0,  0,                          0 },
    { PNULL,                    0,      0,  0,                          0 },
    

    // RAWRGB PREVIEW 2 
    { PNULL,                    0,      0,  0,                          0 },
    { PNULL,                    0,      0,  0,                          0 },
    { PNULL,                    0,      0,  0,                          0 },
    { PNULL,                    0,      0,  0,                          0 }


};
#endif

__align(4)const SENSOR_REG_T ov9650_YUV_COMMON[]=
{
    {0x12, 0x80},
    {SENSOR_WRITE_DELAY,0x14},      // delay 20ms
    {0x11, 0x81},{0x6b, 0x0a},{0x6A, 0x4B},{0x3b, 0x19},
    {0x13, 0xe0},{0x01, 0x80},{0x02, 0x80},{0x00, 0x00},{0x10, 0x00},
    {0x13, 0xe5},{0x39, 0x43},{0x38, 0x12},{0x37, 0x00},{0x35, 0x91},
    {0x0e, 0xa0},{0x1e, 0x04},{0xA8, 0x80},{0x12, 0x40},{0x04, 0x00},
    {0x0c, 0x04},{0x0d, 0x80},{0x18, 0xc7},{0x17, 0x27},{0x32, 0xad},
    {0x03, 0x00},{0x1a, 0x3d},{0x19, 0x01},{0x14, 0x1e},{0x15, 0x00},
    {0x3f, 0xa6},{0x41, 0x02},{0x42, 0x08},{0x1b, 0x00},{0x16, 0x06},
    {0x33, 0xc0},{0x34, 0xbf},{0x96, 0x04},{0x3a, 0x08},{0x8e, 0x00},
    {0x3c, 0x77},{0x8B, 0x06},{0x94, 0x88},{0x95, 0x88},{0x40, 0xc1},
    {0x29, 0x3f},{0x0f, 0x42},{0x3d, 0x92},{0x69, 0x80},{0x5C, 0x96},
    {0x5D, 0x96},{0x5E, 0x10},{0x59, 0xeb},{0x5A, 0x9c},{0x5B, 0x55},
    {0x43, 0xf0},{0x44, 0x10},{0x45, 0x55},{0x46, 0x86},{0x47, 0x64},
    {0x48, 0x86},{0x5F, 0xe0},{0x60, 0x8c},{0x61, 0x20},{0xa5, 0xd9},
    {0xa4, 0x74},{0x8d, 0x02},{0x13, 0xe7},{0x4f, 0x34},{0x50, 0x36},
    {0x51, 0x02},{0x52, 0x10},{0x53, 0x22},{0x54, 0x32},{0x55, 0x40},
    {0x56, 0x40},{0x57, 0x40},{0x58, 0x0d},{0x8C, 0x03},{0x3E, 0x02},
    {0xa9, 0xb8},{0xaa, 0x92},{0xab, 0x0a},{0x8f, 0xdf},{0x90, 0x00},
    {0x91, 0x00},{0x9f, 0x00},{0x3A, 0x0d},{0x2a, 0x00},{0x2b, 0x00},
    {0x7C, 0x02},{0x7D, 0x06},{0x7E, 0x16},{0x7F, 0x3A},{0x80, 0x4C},
    {0x81, 0x5C},{0x82, 0x6A},{0x83, 0x78},{0x84, 0x84},{0x85, 0x8E},
    {0x86, 0x9E},{0x87, 0xAE},{0x88, 0xC6},{0x89, 0xDA},{0x8A, 0xEA},
    {0x6C, 0x20},{0x6D, 0x40},{0x6E, 0x80},{0x6F, 0x90},{0x70, 0x90},
    {0x71, 0x80},{0x72, 0x70},{0x73, 0x70},{0x74, 0x60},{0x75, 0x50},
    {0x76, 0x40},{0x77, 0x40},{0x78, 0x30},{0x79, 0x28},{0x7A, 0x20},
    {0x7B, 0x1c},{0x67, 0x80},{0x68, 0x80},{0x3a, 0x08},
    
};


__align(4) const SENSOR_REG_T ov9650_YUV_1280X1024[]=
{
    {0x11, 0x80}, {0x12, 0x00}, {0x2a, 0x00}, {0x2b, 0x00}, {0x18, 0xbd}, 
    {0x17, 0x1d}, {0x03, 0x12}, {0x1a, 0x81}, {0x6a, 0x3B},
    {0x0c, 0x00}, {0x0d, 0x00},
    //{SENSOR_WRITE_DELAY, 100}, //delay 100ms
};

__align(4)const SENSOR_REG_T ov9650_YUV_640X480[]=
{
#if 1    
    {0x11, 0x81},{0x12, 0x40},{0x2a, 0x00},{0x2b, 0x00},{0x18, 0xc7},
    {0x17, 0x27},{0x03, 0x00},{0x1a, 0x3d},{0x6A, 0x4b},
    {0x0c, 0x04},{0x0d, 0x80},  
    
    // delay about 1.5s
    {SENSOR_WRITE_DELAY,0xff},
    {SENSOR_WRITE_DELAY,0xff},
    {SENSOR_WRITE_DELAY,0xff},
   
#else
            { 0x12, 0x80},
        { 0x37, 0x00},                                          
        { 0x0e, 0x20},   
        { 0x1e, 0x34},//0x34     //this control flip mirror             
                                                    
        {0x01,0x80},                                            
        {0x02,0x80},                                            
        {0x00,0x00},                                            
        {0x10,0xf0},                                            
                                                        
        {0x04,0x00},                                            
        {0x0c,0x04},                                            
        {0x0d,0x80},                                            
        {0x11,0x80},//0x81//@20050713                                   
        {0x14,0x2e},                                            
        {0x15,0x00},                                            
        {0x32,0xad},                                            
        {0x19,0x01},                                            
        {0x3f,0xa6},                                            
        {0x41,0x02},                                            
        {0x42,0x08},                                            
                                                        
        {0x1b,0x00},                                            
        {0x16,0x06},                                            
        {0x33,0xc0},                                            
        {0x34,0xbf},                                            
        {0xA8,0x80},                                            
        {0x96,0x04},                                            
        {0x3a,0x0c},                                            
        {0x8e,0x00},                                            
                                                        
        {0x3c,0x77},                                            
        {0x8B,0x06},                                            
        {0x35,0x91},                                            
        {0x94,0x88},                                            
        {0x95,0x88},                                            
        {0x40,0xc1},                                            
        {0x29,0x3f},                                            
        {0x0f,0x42},                                            
                                                        
        {0x13,0xe5},                                            
                                                    
        {0x3d,0x92},                                            
        {0x69,0x80},                                            
        {0x5C,0x96},                                            
        {0x5D,0x96},                                            
        {0x5E,0x10},                                            
        {0x59,0xeb},                                            
        {0x5A,0x9c},                                            
        {0x5B,0x55},                                            
        {0x43,0xf0},                                            
        {0x44,0x10},                                            
        {0x45,0x55},                                            
        {0x46,0x86},                                            
        {0x47,0x64},                                            
        {0x48,0x86},                                            
        {0x5F,0xF0},                                            
        {0x60,0x8c},                                            
        {0x61,0x20},                                            
                                                        
        {0xa5,0xd9},                                            
        {0xa4,0x74},                                            
        {0x8d,0x02},                                            
                                                        
        {0x13,0xe7},                                            
        {0x4f,0x3a},                                            
        {0x50,0x3d},                                            
        {0x51,0x03},                                            
        {0x52,0x08},                                            
        {0x53,0x26},                                            
        {0x54,0x30},                                            
        {0x55,0x40},                                            
        {0x56,0x40},                                            
        {0x57,0x40},                                            
        {0x58,0x0d},                                            
                                                        
        {0x8C,0x23},                                            
        {0x3E,0x02},                                            
        {0xa9,0xb8},                                            
        {0xaa,0x92},                                            
        {0xab,0x0a},                                            
                                                        
        {0x8f,0xdf},                                            
        {0x90,0x00},                                            
        {0x91,0x00},                                            
        {0x9f,0x00},                                            
        {0x3A,0x01},  //yuv    uvy register   old is 0x0d       
                                                        
        {0x24,0x90},                                            
        {0x25,0x88},                                            
        {0x26,0xc6},                                            
        {0x3b,0x19},                                            
                                                        
                                                        
        {0x6c,0x40},                                            
        {0x6d,0x30},                                            
        {0x6e,0x4b},                                            
        {0x6f,0x60},                                            
        {0x70,0x70},                                            
        {0x71,0x70},                                            
        {0x72,0x70},                                            
        {0x73,0x70},                                            
        {0x74,0x60},                                            
        {0x75,0x60},                                            
        {0x76,0x50},                                            
        {0x77,0x48},                                            
        {0x78,0x3a},                                            
        {0x79,0x2e},                                            
        {0x7a,0x28},                                            
        {0x7b,0x22},                                            
        {0x7c,0x04},                                            
        {0x7d,0x07},                                            
        {0x7e,0x10},                                            
        {0x7f,0x28},                                            
        {0x80,0x36},                                            
        {0x81,0x44},                                            
        {0x82,0x52},                                            
        {0x83,0x60},                                            
        {0x84,0x6c},                                            
        {0x85,0x78},                                            
        {0x86,0x8c},                                            
        {0x87,0x9e},                                            
        {0x88,0xbb},                                            
        {0x89,0xd2},                                            
        {0x8a,0xe6},
        
        {0x64,0x0d},                                            
        {0x65,0x30},                                            
        {0x66,0x01},
        
        {0x39,0x50},
        {0x38,0x92},    
        {0x0c,0x04},                                            
        {0x0d,0x80},
        
        {0x3f,0xa6},                                            
        {0x41,0x02},
        {0x14,0x2e},    
         
        {0x11,0x80},
        
        {0x12,0x40},         
        {0x03,0x00},        
        {0x1a,0x3d},     
                    
        {0x6a,0x3e},
        {0x2a,0x00},//10 for 50hz        
        {0x2b,0x00},//40 for 50hz        
        {0x17,0x27},        
        {0x18,0xc7},
    
    
        { 0xff, 0xff}, //terminated symbol,end of the sequece!!!!
#endif
};    


LOCAL SENSOR_REG_TAB_INFO_T s_OV9650_resolution_Tab_YUV[]=
{      
    // COMMON INIT
    { ADDR_AND_LEN_OF_ARRAY(ov9650_YUV_COMMON),     	640,  480,  24,0   	}, 
    
    // YUV422 PREVIEW 1
    { ADDR_AND_LEN_OF_ARRAY(ov9650_YUV_640X480),        640,    480,    24,0 },
    { ADDR_AND_LEN_OF_ARRAY(ov9650_YUV_1280X1024),      1280,   1024,   24,0 },   
    { PNULL,                    0,      0,  0,       0,                   0 },
    { PNULL,                    0,      0,  0,       0,                    0 },    

    // YUV422 PREVIEW 2     
    { PNULL,                    0,      0,  0,       0,                   0 },
    { PNULL,                    0,      0,  0,       0,                   0 },
    { PNULL,                    0,      0,  0,       0,                    0 },
    { PNULL,                    0,      0,  0,       0,                   0 }
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_OV9650_ioctl_func_tab = 
{
// Internal 
    PNULL,
    PNULL,
    PNULL,
    OV9650_Identify,
    
    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

// External
    set_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,
    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,
    
    set_preview_mode,   
    set_image_effect,

    OV9650_BeforeSnapshot,
    PNULL,

    PNULL,
    
    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,
    
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL
    
};



PUBLIC SENSOR_INFO_T g_OV9650_yuv_info =
{
	OV9650_I2C_ADDR_W,				// salve i2c write address
	OV9650_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT|\
	SENSOR_ENVIROMENT_SUNNY,		
	
	// image effect
	SENSOR_IMAGE_EFFECT_NORMAL|\
	SENSOR_IMAGE_EFFECT_BLACKWHITE|\
	SENSOR_IMAGE_EFFECT_RED|\
	SENSOR_IMAGE_EFFECT_GREEN|\
	SENSOR_IMAGE_EFFECT_BLUE|\
	SENSOR_IMAGE_EFFECT_YELLOW|\
	SENSOR_IMAGE_EFFECT_NEGATIVE|\
	SENSOR_IMAGE_EFFECT_CANVAS,
	
	// while balance mode
	0,
		
	7,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved
	
	SENSOR_HIGH_PULSE_RESET,		// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	1,								// count of identify code
	0x0A, 0x96,						// supply two code to identify sensor.
	0x0B, 0x52,						// for Example: index = 0-> Device id, index = 1 -> version id											
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	1280,							// max width of source image
	1024,							// max height of source image
	"OV9652",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_VYUY,	// pattern of input image form sensor;			

	s_OV9650_resolution_Tab_YUV,	// point to resolution table information structure
	&s_OV9650_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview 
	3,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2,                       // deci frame num during video preview
	0,
	0,
	0,
	0,
	PNULL
	
};

LOCAL uint32 set_ae_enable(uint32 enable)
{
    unsigned char value;

	SCI_TRACE_LOW("set_ae_enable: enable = %d", enable);
	
    value = Sensor_ReadReg(0x13);
    value &= 0xFE;
    value |= enable;
    Sensor_WriteReg(0x13, value);

	return 0;
}

LOCAL uint32 set_hmirror_enable(uint32 enable)
{
    unsigned char value;

    SCI_TRACE_LOW("set_hmirror_enable: enable = %d", enable);
        
    value = Sensor_ReadReg(0x1E);
    value &= 0xDF;
    value |= enable<<5;
    Sensor_WriteReg(0x1E, value);

    return 0;
}

LOCAL uint32 set_vmirror_enable(uint32 enable)
{
    unsigned char value;

    SCI_TRACE_LOW("set_vmirror_enable: enable = %d", enable);
    
    value = Sensor_ReadReg(0x1E);
    value &= 0xEF;
    value |= enable<<4;
    Sensor_WriteReg(0x1E, value);

    return 0;
}

LOCAL uint32 read_ev_value(uint32 value)
{
    uint8 expo_low;
    uint8 expo_mid;
    uint8 expo_high;
    uint32 read_exposure = 0;
    
    expo_low  = Sensor_ReadReg(0x04);
    expo_mid  = Sensor_ReadReg(0x10);
    expo_high = Sensor_ReadReg(0xA1);
    
    read_exposure = (expo_low & 0x03) + (expo_mid << 0x02) + ((expo_high & 0x3f) << 0x0a);
    
    SCI_TRACE_LOW("read_ae_value: %x", read_exposure);
    return read_exposure;
}

LOCAL uint32 write_ev_value(uint32 exposure_value)
{
    uint8 expo_low;
    uint8 expo_mid;
    uint8 expo_high;
    
    SCI_TRACE_LOW("write_ae_value: %x", exposure_value);
    expo_low  = Sensor_ReadReg(0x04);   
    expo_high = Sensor_ReadReg(0xA1);
    
    expo_low = (expo_low&0xfc) + (exposure_value&0x03);
    expo_mid = (exposure_value>>0x02)&0xff;
    expo_high = (expo_high&0xc0) + (exposure_value>>0x0a)&0x3f;

    Sensor_WriteReg(0x04, expo_low);
    Sensor_WriteReg(0x10, expo_mid);
    Sensor_WriteReg(0xa1, expo_high);   
    
    return 0;   
}

LOCAL uint32 read_gain_value(uint32 value)
{
    uint32 read_gain;
    uint8 pre_gain00;
    uint8 pre_gain03;
    
    pre_gain00 = Sensor_ReadReg(0x00);
    pre_gain03 = Sensor_ReadReg(0x03);

    read_gain = (pre_gain00 & 0x0f) + 16;//pre_gain = (pre_gain00 & 0x0f) + 16;
    if(pre_gain00 & 0x10)
        read_gain = read_gain << 1;
    if (pre_gain00 & 0x20)
        read_gain = read_gain << 1;
    if (pre_gain00 & 0x40)
        read_gain = read_gain << 1;
    if (pre_gain00 & 0x80)
        read_gain = read_gain << 1;
        
    if (pre_gain03 & 0x40)
        read_gain = read_gain << 1;     
    if (pre_gain03 & 0x80)
        read_gain = read_gain << 1;
    
    SCI_TRACE_LOW("read_gain_value: %x", read_gain);
    
    return read_gain;
}

LOCAL uint32 write_gain_value(uint32 gain_value)
{
    uint8 write_gain00;
    uint8 write_gain03; 
    
    SCI_TRACE_LOW("write_gain_value: %x", gain_value);
    
    write_gain00 =0;
    write_gain03 = Sensor_ReadReg(0x03);
    
    write_gain03 &= 0x3f;
    if (gain_value > 31)
    {
        write_gain00 |= 0x10;
        gain_value = gain_value >> 1;
    }   
    if (gain_value > 31)
    {
        write_gain00 |= 0x20;
        gain_value = gain_value >> 1;
    }   
    if (gain_value > 31)
    {
        write_gain00 |= 0x40;
        gain_value = gain_value >> 1;
    }   
    if (gain_value > 31)
    {
        write_gain00 |= 0x80;
        gain_value = gain_value >> 1;
    }
    if (gain_value > 31)
    {
        write_gain03 |= 0x40;
        gain_value = gain_value >> 1;
    }   
    if (gain_value > 31)
    {
        write_gain03 |= 0x80;
        gain_value = gain_value >> 1;
    }       

    if (gain_value > 16) 
        write_gain00 |= ((gain_value -16) & 0x0f); 
            
    Sensor_WriteReg(0x00, write_gain00);
    Sensor_WriteReg(0x03, write_gain03);    
    
    return 0;

}

LOCAL uint32 read_gain_scale(uint32 value)
{
    return SENSOR_GAIN_SCALE;
}

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
    SCI_TRACE_LOW("set_preview_mode: preview_mode = %d", preview_mode);
    
    s_preview_mode = preview_mode;
    
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL:
            Sensor_WriteReg(0x11, 0x81);
            Sensor_WriteReg(0x14, 0x1e);
            Sensor_WriteReg(0x6A, 0x4B);
            Sensor_WriteReg(0x3b, 0x19);
            Sensor_WriteReg(0x39, 0x43);
            Sensor_WriteReg(0x38, 0x12);
            Sensor_WriteReg(0x35, 0x91);
            Sensor_WriteReg(0x24, 0x90);
            Sensor_WriteReg(0x25, 0x80);
            Sensor_WriteReg(0x26, 0xC6);
            Sensor_WriteReg(0x62, 0x1E);
            Sensor_WriteReg(0x63, 0x00);
            Sensor_WriteReg(0x64, 0x04);
            Sensor_WriteReg(0x65, 0x10);
            Sensor_WriteReg(0x66, 0x05);
            Sensor_WriteReg(0x9D, 0x04);
            Sensor_WriteReg(0x9E, 0x04);

            break;
        case DCAMERA_ENVIRONMENT_NIGHT:

            Sensor_WriteReg(0x11, 0x83);
            Sensor_WriteReg(0x14, 0x3e);
            Sensor_WriteReg(0x3b, 0x19); 
            Sensor_WriteReg(0x6A, 0x25);
            Sensor_WriteReg(0x39, 0x43);
            Sensor_WriteReg(0x38, 0x12);
            Sensor_WriteReg(0x35, 0x91);
            Sensor_WriteReg(0x24, 0xA0);
            Sensor_WriteReg(0x25, 0x90);
            Sensor_WriteReg(0x26, 0xe2);        
            Sensor_WriteReg(0x66, 0x00); 
            
            break;
/*            
        case DC_PRV_SUNNY_MODE:
        
            Sensor_WriteReg(0x11, 0x80);
            Sensor_WriteReg(0x14, 0x1e);
            Sensor_WriteReg(0x6A, 0x96);
            Sensor_WriteReg(0x3b, 0x19);
            Sensor_WriteReg(0x39, 0x50);
            Sensor_WriteReg(0x38, 0x92);
            Sensor_WriteReg(0x35, 0x81);
            Sensor_WriteReg(0x24, 0x70);
            Sensor_WriteReg(0x25, 0x60);
            Sensor_WriteReg(0x26, 0xe2); 
            Sensor_WriteReg(0x62, 0x1E);
            Sensor_WriteReg(0x63, 0x00);
            Sensor_WriteReg(0x64, 0x04);
            Sensor_WriteReg(0x65, 0x10);
            Sensor_WriteReg(0x66, 0x05);
            Sensor_WriteReg(0x9D, 0x04);
            Sensor_WriteReg(0x9E, 0x04);    
            break;
*/            
        default:
            break;
    }
    
    return 0;
}

__align(4) const SENSOR_REG_T ov9650_brightness_tab[][4]=
{
    {
        {0x24,0x1c},{0x25,0x14},{0x26,0x50},{0xFF,0xFF},
    },
    
    {
        {0x24,0x38},{0x25,0x30},{0x26,0x71},{0xFF,0xFF},
    },

    {
        {0x24,0x50},{0x25,0x48},{0x26,0x92},{0xFF,0xFF},
    },

    {
        {0x24,0x76},{0x25,0x69},{0x26,0xC4},{0xFF,0xFF},    
    },

    {
        {0x24,0x8a},{0x25,0x80},{0x26,0xC4},{0xFF,0xFF},
    },

    {
        {0x24,0xa4},{0x25,0x9a},{0x26,0xD6},{0xFF,0xFF},
    },

    {
        {0x24,0xE8},{0x25,0xDA},{0x26,0xF4},{0xFF,0xFF},
    }
};
__align(4) const SENSOR_REG_T ov9650_contrast_tab[][34]=
{
    {
        {0x3D, 0x12},{0x7c, 0x04},{0x7d, 0x09},{0x7e, 0x14},{0x7f, 0x28},
        {0x80, 0x32},{0x81, 0x3C},{0x82, 0x46},{0x83, 0x4F},{0x84, 0x58},
        {0x85, 0x61},{0x86, 0x73},{0x87, 0x85},{0x88, 0xA5},{0x89, 0xc5},
        {0x8a, 0xDD},{0x6c, 0x40},{0x6d, 0x50},{0x6e, 0x58},{0x6f, 0x50},
        {0x70, 0x50},{0x71, 0x50},{0x72, 0x50},{0x73, 0x48},{0x74, 0x48},
        {0x75, 0x48},{0x76, 0x48},{0x77, 0x48},{0x78, 0x40},{0x79, 0x40},
        {0x7a, 0x30},{0x7b, 0x2E},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x7C, 0x04},{0x7D, 0x09},{0x7E, 0x13},{0x7F, 0x29},
        {0x80, 0x35},{0x81, 0x41},{0x82, 0x4D},{0x83, 0x59},{0x84, 0x64},
        {0x85, 0x6F},{0x86, 0x85},{0x87, 0x97},{0x88, 0xB7},{0x89, 0xCF},
        {0x8A, 0xE3},{0x6C, 0x40},{0x6D, 0x50},{0x6E, 0x50},{0x6F, 0x58},
        {0x70, 0x60},{0x71, 0x60},{0x72, 0x60},{0x73, 0x60},{0x74, 0x58},
        {0x75, 0x58},{0x76, 0x58},{0x77, 0x48},{0x78, 0x40},{0x79, 0x30},
        {0x7A, 0x28},{0x7B, 0x26},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x7c, 0x05},{0x7d, 0x0b},{0x7e, 0x16},{0x7f, 0x2c},
        {0x80, 0x37},{0x81, 0x41},{0x82, 0x4b},{0x83, 0x55},{0x84, 0x5f},
        {0x85, 0x69},{0x86, 0x7c},{0x87, 0x8f},{0x88, 0xb1},{0x89, 0xcf},
        {0x8a, 0xe5},{0x6c, 0x50},{0x6d, 0x60},{0x6e, 0x58},{0x6f, 0x58},
        {0x70, 0x58},{0x71, 0x50},{0x72, 0x50},{0x73, 0x50},{0x74, 0x50},
        {0x75, 0x50},{0x76, 0x4c},{0x77, 0x4c},{0x78, 0x44},{0x79, 0x3c},
        {0x7a, 0x2c},{0x7b, 0x23},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x7C, 0x02},{0x7D, 0x06},{0x7E, 0x16},{0x7F, 0x3A},
        {0x80, 0x4C},{0x81, 0x5C},{0x82, 0x6A},{0x83, 0x78},{0x84, 0x84},
        {0x85, 0x8E},{0x86, 0x9E},{0x87, 0xAE},{0x88, 0xC6},{0x89, 0xDA},
        {0x8A, 0xEA},{0x6C, 0x20},{0x6D, 0x40},{0x6E, 0x80},{0x6F, 0x90},
        {0x70, 0x90},{0x71, 0x80},{0x72, 0x70},{0x73, 0x70},{0x74, 0x60},
        {0x75, 0x50},{0x76, 0x40},{0x77, 0x40},{0x78, 0x30},{0x79, 0x28},
        {0x7A, 0x20},{0x7B, 0x1c},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x6C, 0x20},{0x6D, 0x50},{0x6E, 0x80},{0x6F, 0xC0},
        {0x70, 0xC0},{0x71, 0xA0},{0x72, 0x90},{0x73, 0x78},{0x74, 0x78},
        {0x75, 0x78},{0x76, 0x40},{0x77, 0x20},{0x78, 0x20},{0x79, 0x20},
        {0x7A, 0x14},{0x7B, 0x0e},{0x7C, 0x02},{0x7D, 0x07},{0x7E, 0x17},
        {0x7F, 0x47},{0x80, 0x5F},{0x81, 0x73},{0x82, 0x85},{0x83, 0x94},
        {0x84, 0xA3},{0x85, 0xB2},{0x86, 0xC2},{0x87, 0xCA},{0x88, 0xDA},
        {0x89, 0xEA},{0x8A, 0xF4},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x7C, 0x02},{0x7D, 0x07},{0x7E, 0x1f},{0x7F, 0x49},
        {0x80, 0x5a},{0x81, 0x6a},{0x82, 0x79},{0x83, 0x87},{0x84, 0x94},
        {0x85, 0x9f},{0x86, 0xaf},{0x87, 0xbb},{0x88, 0xcf},{0x89, 0xdf},
        {0x8A, 0xee},{0x6C, 0x20},{0x6D, 0x50},{0x6E, 0xc0},{0x6F, 0xa8},
        {0x70, 0x88},{0x71, 0x80},{0x72, 0x78},{0x73, 0x70},{0x74, 0x68},
        {0x75, 0x58},{0x76, 0x40},{0x77, 0x30},{0x78, 0x28},{0x79, 0x20},
        {0x7A, 0x1e},{0x7B, 0x17},{0x3D, 0x92},{0xFF, 0xFF},
    },
    {
        {0x3D, 0x12},{0x6c, 0xf0},{0x6d, 0xf0},{0x6e, 0xf8},{0x6f, 0x74},
        {0x70, 0x58},{0x71, 0x48},{0x72, 0x48},{0x73, 0x48},{0x74, 0x38},
        {0x75, 0x40},{0x76, 0x34},{0x77, 0x34},{0x78, 0x2e},{0x79, 0x28},
        {0x7a, 0x24},{0x7b, 0x21},{0x7c, 0x0f},{0x7d, 0x1e},{0x7e, 0x3d},
        {0x7f, 0x5a},{0x80, 0x65},{0x81, 0x6e},{0x82, 0x77},{0x83, 0x80},
        {0x84, 0x87},{0x85, 0x8f},{0x86, 0x9c},{0x87, 0xa9},{0x88, 0xc0},
        {0x89, 0xd4},{0x8a, 0xe6},{0x3D, 0x92},{0xFF, 0xFF},
    }
};    
    
    
    


__align(4) const SENSOR_REG_T ov9650_image_effect_tab[][4]=
{
    // effect normal
    {
        {0x3a, 0x08},{0x67, 0x80},{0x68, 0x80}, {0xff, 0xff},
    },
    // effect BLACKWHITE
    {
        {0x3a, 0x18},{0x67, 0x80},{0x68, 0x80}, {0xff, 0xff},
    },
    // effect RED 
    {
        {0x3a, 0x18},{0x67, 0x80},{0x68, 0xc0}, {0xff, 0xff},
    },

    // effect GREEN 
    {
        {0x3a, 0x18},{0x67, 0x40},{0x68, 0x40}, {0xff, 0xff},
    },

    // effect  BLUE
    {
        {0x3a, 0x18},{0x67, 0xc0},{0x68, 0x80}, {0xff, 0xff},
    },
    // effect yellow
    {
        {0x3a, 0x18},{0x67, 0x40},{0x68, 0x80}, {0xff, 0xff},
    },
    // effect sepia
    {
        {0x3a, 0x18},{0x67, 0x40},{0x68, 0xa0}, {0xff, 0xff},
    },
    
    // effect NEGATIVE
    {
        {0x3a, 0x28},{0x67, 0x80},{0x68, 0x80}, {0xff, 0xff},
    },
/*
    // effect CANVAS ANTIQUE
    {
        {0x3a, 0x18},{0x67, 0x40},{0x68, 0xb0}, {0xff, 0xff},
    }
*/
};

__align(4) const SENSOR_REG_T ov9650_saturation_tab[][12]=
{
    {
        {0x4f, 0x0f}, {0x50, 0x0f}, {0x51, 0x01}, {0x52, 0x05}, {0x53, 0x0a},
        {0x54, 0x0e}, {0x55, 0x40}, {0x56, 0x40}, {0x57, 0x40}, {0x58, 0x0d},
        {0xff, 0xff}, {0xff, 0xff},
    },
    
    {
        {0x4f, 0x1d},{0x50, 0x1f},{0x51, 0x02},{0x52, 0x09},{0x53, 0x13},
        {0x54, 0x1c},{0x55, 0x40},{0x56, 0x40},{0x57, 0x40},{0x58, 0x0d},
        {0xff, 0xff},{0xff, 0xff},      
    },
    
    {
        {0x4f, 0x2c},{0x50, 0x2e},{0x51, 0x02},{0x52, 0x0e},{0x53, 0x1d},
        {0x54, 0x2a},{0x55, 0x40},{0x56, 0x40},{0x57, 0x40},{0x58, 0x0d},
        {0xff, 0xff},{0xff, 0xff},      
    },  
    
    {      
        {0x4f, 0x3a},{0x50, 0x3d},{0x51, 0x03},{0x52, 0x12},{0x53, 0x26},
        {0x54, 0x38},{0x55, 0x40},{0x56, 0x40},{0x57, 0x40},{0x58, 0x0d},
        {0xff, 0xff},{0xff, 0xff},      
    },
    
    {
        {0x4f, 0x49},{0x50, 0x4c},{0x51, 0x04},{0x52, 0x17},{0x53, 0x30},
        {0x54, 0x46},{0x55, 0x40},{0x56, 0x40},{0x57, 0x40},{0x58, 0x0d},
        {0xff, 0xff},{0xff, 0xff},      
    },
};

__align(4) const SENSOR_REG_T ov9650_sharpness_tab[][2]=
{
    {
            {0x3F,0xA7},{0xFF,0xFF},
    },          
        
    {
            {0x3F,0xA6},{0xFF,0xFF},
    },
    {
            {0x3F,0xA5},{0xFF,0xFF},
    },

    {
            {0x3F,0xA4},{0xFF,0xFF},
    },

    {
            {0x3F,0xA3},{0xFF,0xFF},
    },

    {
            {0x3F,0xA2},{0xFF,0xFF},
    },

    {
            {0x3F,0xA1},{0xFF,0xFF},
    },

    {
            {0x3F,0xA0},{0xFF,0xFF},
    },          
            
};


LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov9650_brightness_tab[level];

#line 944
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_TRACE_LOW("set_brightness: level = %d", level);
    
    return 0;
}

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov9650_contrast_tab[level];

#line 960
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_TRACE_LOW("set_contrast: level = %d", level);
    
    return 0;
}

LOCAL uint32 set_sharpness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov9650_sharpness_tab[level];

#line 976
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("set_sharpness: level = %d", level);
    
    return 0;
    
}


LOCAL uint32 set_saturation(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov9650_saturation_tab[level];

#line 994
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }   
    
    SCI_TRACE_LOW("set_saturation: level = %d", level);
    
    return 0;
}

LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov9650_image_effect_tab[effect_type];

#line 1011
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("-----------set_image_effect: effect_type = %d------------", effect_type);
    
    return 0;
}

LOCAL uint32 set_frame_rate(uint32 param)
{
    uint32 clkrc = 0;
    
    SCI_TRACE_LOW("set_frame_rate: param = %d", param);
    SCI_ASSERT((param>=1&&param<=64));
    
    clkrc = Sensor_ReadReg(0x11);
    clkrc = (clkrc&0xc0)+(param-1);
    
    Sensor_WriteReg(0x11,clkrc); 
    
    return 0;
}

LOCAL uint32 OV9650_Identify(uint32 param)
{
#define OV9650_PID_VALUE    0x96    
#define OV9650_PID_ADDR     0x0A
#define OV9650_VER_VALUE    0x50    
#define OV9650_VER_ADDR     0x0B
    
    uint8 pid_value = 0;
    uint8 ver_value = 0;
    BOOLEAN ret_value = 0XFF;
    uint8 i2c_cmd[2];
    
    i2c_cmd[0] = OV9650_PID_ADDR;
    I2C_WriteCmd(OV9650_I2C_ADDR_W, i2c_cmd[0], SCI_TRUE);
    I2C_ReadCmdArr(OV9650_I2C_ADDR_R, i2c_cmd, 2, SCI_TRUE);
    // Get Version Code
    pid_value = i2c_cmd[0];
    ver_value = i2c_cmd[1];

    if(OV9650_PID_VALUE == pid_value)
    {
        // Get Device Code
        
        if(OV9650_VER_VALUE == ver_value)
        {
            SCI_TRACE_LOW("That is OV9650 sensor !");       
        }
        else
        {
            SCI_TRACE_LOW("OV9650_Identify: That is OV%x%x sensor !", pid_value, ver_value);
        }
        ret_value = 0;
    }

    SCI_TRACE_LOW("OV9650_Identify: PID = %x, VER = %x",\
        pid_value, ver_value);
    
    return (uint32)ret_value;
}

LOCAL void caculate_expo( uint16 pre_expo,uint16 pre_gain ,uint16 *cap_expo,uint16 *cap_gain  )
{//preview as VGA 12.5fps,capture at SXGA 6.25fps  no auto nightmode
//  cap_expo =pre_expo;
//  cap_gain=pre_gain;
    


    uint32 Capture_Exposure_Gain ;
    uint16 Capture_Exposure =0;
    uint16 Capture_gain;
    uint16 pre_array1[3] = {105, 210, 53};//{126, 253, 63};
    //uint16 pre_array2[3] = {78, 78, 78};
    
    SCI_TRACE_LOW("SENSOR OV9650 : caculate_expo PREVIEW MODE %d", s_preview_mode);
    pre_expo =pre_expo*pre_array1[s_preview_mode]/100 ; //pre_expo*21*/40; 
    Capture_Exposure_Gain = pre_gain * pre_expo;
    if (Capture_Exposure_Gain < 1048*16)
    {
        Capture_Exposure = Capture_Exposure_Gain/16;
        
        if (Capture_Exposure > 78) 
        {                       
            Capture_Exposure = (Capture_Exposure+1)/78*78;
        }       
        //Capture_gain = 16;        
        Capture_gain = Capture_Exposure_Gain/Capture_Exposure;
    }    
    else
    {//if image is dark,//and use the correct exposure time to eliminate 50/60 hz line
        Capture_Exposure = 1048;
        Capture_gain = Capture_Exposure_Gain / 1048;
    }


    *cap_expo =Capture_Exposure;
    *cap_gain=Capture_gain;

}

LOCAL void Sensor_Set_ChangeMega_OV9650(void) 
{
      
    uint8 pre_gain00;   
    uint8 pre_expoA1;   
    uint8 pre_expo10;   
    uint8 pre_expo04;
    
    uint16  pre_expo;
    uint16 pre_gain ;
    uint16 cap_expo,cap_gain;


    uint8 data, cap_expo04, cap_expo10, cap_expoA1, cap_gain00;
    
    {
        uint8 reg_value = 0;
        reg_value = Sensor_ReadReg(0x13);
        reg_value &= 0xfa;
        Sensor_WriteReg(0x13, reg_value);
    }

    pre_gain00 = Sensor_ReadReg(0x00);
    pre_expo10 = Sensor_ReadReg(0x10);
    pre_expoA1 = Sensor_ReadReg(0xA1);
    pre_expo04 = Sensor_ReadReg(0x04);

    //caculate pre_exposure
    pre_expo= (pre_expoA1&0x3f);
    pre_expo= ((pre_expo<<8)+pre_expo10);
    pre_expo= ((pre_expo<<2)+(pre_expo04&0x03));
    //caculate pre_gain
    pre_gain = (pre_gain00 & 0x0f) + 16;//pre_gain = (pre_gain00 & 0x0f) + 16;
    if(pre_gain00 & 0x10)
        pre_gain = pre_gain << 1;
    if (pre_gain00 & 0x20)
        pre_gain = pre_gain << 1;
    if (pre_gain00 & 0x40)
        pre_gain = pre_gain << 1;
    if (pre_gain00 & 0x80)
        pre_gain = pre_gain << 1;
    
    caculate_expo( pre_expo,pre_gain ,&cap_expo,&cap_gain  );

    
    data = cap_expo & 0x03;
    cap_expo04=((pre_expo04 & 0xfc)|data);
     
    data = (cap_expo >> 2);
    cap_expo10 = data;  
        
    data = (uint8)(cap_expo >> 10); 
    cap_expoA1 = data;
      //exposure

    //write gain
    cap_gain00 =0;
    if (cap_gain > 31)
    {
        cap_gain00 |= 0x10;
        cap_gain = cap_gain >> 1;
    }   
    if (cap_gain > 31)
    {
        cap_gain00 |= 0x20;
        cap_gain = cap_gain >> 1;
    }   
    if (cap_gain > 31)
    {
        cap_gain00 |= 0x40;
        cap_gain = cap_gain >> 1;
    }   
    if (cap_gain > 31)
    {
        cap_gain00 |= 0x80;
        cap_gain = cap_gain >> 1;
    }   

    if (cap_gain > 16) 
        cap_gain00 |= ((cap_gain -16) & 0x0f); 
        
    //show reseult// write 
    Sensor_WriteReg(0x00, cap_gain00);
    Sensor_WriteReg(0xA1, cap_expoA1);
    Sensor_WriteReg(0x10, cap_expo10);
    Sensor_WriteReg(0x04, cap_expo04);

    Sensor_ReadReg(0x00);
    Sensor_ReadReg(0x10);
    Sensor_ReadReg(0xA1);
    Sensor_ReadReg(0x04);

    //OS_TickDelay(150);        //wait 1 frame need about 257ms(SXGA),the time decided by sensor
}

LOCAL uint32 OV9650_BeforeSnapshot(uint32 param)
{
    SENSOR_MODE_E sensor_mode = (SENSOR_MODE_E)param;
    
    SCI_TRACE_LOW("OV9650_BeforeSnapshot: param = %d", param);
    

    if(SENSOR_MODE_SNAPSHOT_ONE_FIRST == sensor_mode)
    {
        Sensor_Set_ChangeMega_OV9650();
    }

    return 0;
}




    







