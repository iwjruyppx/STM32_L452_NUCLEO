#ifndef TM_SH1106_H
#define TM_SH1106_H

#ifdef USE_OLED_DRIVER_SH1106

/* I2C address */
#ifndef SH1106_I2C_ADDR
// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)
#define SH1106_I2C_ADDR         (0x3C <<1)
#endif

#ifndef SH1106_WIDTH
#define SH1106_WIDTH            128
#endif
/* SH1106 LCD height in pixels */
#ifndef SH1106_HEIGHT
#define SH1106_HEIGHT           64
#endif

#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

typedef enum {
	SH1106_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SH1106_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SH1106_COLOR_t;
    
typedef struct {
    int (*cbfunc_SingleRegWrite)(uint16_t, uint8_t, uint8_t);
    int (*cbfunc_RegWrite)(uint16_t, uint8_t, uint8_t *, uint16_t);
} SH1106_callback_t;

uint8_t TM_SH1106_Init(SH1106_callback_t *cb_funcs);

void TM_SH1106_UpdateScreen(void);

void TM_SH1106_ToggleInvert(void);

void TM_SH1106_Fill(SH1106_COLOR_t Color);

void TM_SH1106_DrawPixel(uint16_t x, uint16_t y, SH1106_COLOR_t color); 

void TM_SH1106_GotoXY(uint16_t x, uint16_t y);

char TM_SH1106_Putc(char ch, TM_FontDef_t* Font, SH1106_COLOR_t color);

char TM_SH1106_Puts(char* str, TM_FontDef_t* Font, SH1106_COLOR_t color);

void TM_SH1106_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1106_COLOR_t c);

void TM_SH1106_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);

void TM_SH1106_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);

void TM_SH1106_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color);

void TM_SH1106_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);

void TM_SH1106_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);

void TM_SH1106_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color);

char CWM_SH1106_Puts_Auto_newLine(char* str, TM_FontDef_t* Font, SH1106_COLOR_t color) ;
void TM_SH1106_ScreenClean(void);

void SH1106_ON(void);
void SH1106_OFF(void);

#endif /*USE_OLED_DRIVER_SH1106*/

#endif /* TM_SH1106_H */
