
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "tm_stm32f4_SH1106.h"

#ifdef USE_OLED_DRIVER_SH1106

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* Private SH1106 structure */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
    uint8_t SH1106_Buffer[SH1106_WIDTH * SH1106_HEIGHT / 8]; /* SH1106 data buffer */
    SH1106_callback_t *CWMcbfuncs ;
} SH1106_t;

/* Private variable */
static SH1106_t SH1106;

/* Write command */
static void SH1106_WRITECOMMAND(uint8_t cmd)
{
    if(SH1106.CWMcbfuncs->cbfunc_SingleRegWrite != NULL)
        SH1106.CWMcbfuncs->cbfunc_SingleRegWrite(SH1106_I2C_ADDR, 0x00, (cmd));
}

/* Write command */
static void SH1106_REG_WRITE(uint8_t Reg, uint8_t *pData, uint16_t Size)
{
    if(SH1106.CWMcbfuncs->cbfunc_RegWrite != NULL)
        SH1106.CWMcbfuncs->cbfunc_RegWrite(SH1106_I2C_ADDR, Reg, pData, Size);
}

uint8_t TM_SH1106_Init(SH1106_callback_t *cb_funcs) {
    SH1106.CWMcbfuncs = cb_funcs;
    int vccstate = 0;
    
    /* Init LCD */
    // Init sequence for 128x64 OLED module
    SH1106_WRITECOMMAND(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_WRITECOMMAND(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_WRITECOMMAND(0x80);                                  // the suggested ratio 0x80
    SH1106_WRITECOMMAND(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_WRITECOMMAND(0x3F);
    SH1106_WRITECOMMAND(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_WRITECOMMAND(0x00);                                   // no offset
	
    SH1106_WRITECOMMAND(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
    SH1106_WRITECOMMAND(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC) 
      { SH1106_WRITECOMMAND(0x10); }
    else 
      { SH1106_WRITECOMMAND(0x14); }
    SH1106_WRITECOMMAND(SH1106_MEMORYMODE);                    // 0x20
    SH1106_WRITECOMMAND(0x00);                                  // 0x0 act like ks0108
    SH1106_WRITECOMMAND(SH1106_SEGREMAP | 0x1);
    SH1106_WRITECOMMAND(SH1106_COMSCANDEC);
    SH1106_WRITECOMMAND(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_WRITECOMMAND(0x12);
    SH1106_WRITECOMMAND(SH1106_SETCONTRAST);                   // 0x81
    if (vccstate == SH1106_EXTERNALVCC) 
      { SH1106_WRITECOMMAND(0x9F); }
    else 
      { SH1106_WRITECOMMAND(0xCF); }
    SH1106_WRITECOMMAND(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC) 
      { SH1106_WRITECOMMAND(0x22); }
    else 
      { SH1106_WRITECOMMAND(0xF1); }
    SH1106_WRITECOMMAND(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_WRITECOMMAND(0x40);
    SH1106_WRITECOMMAND(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_WRITECOMMAND(SH1106_NORMALDISPLAY);                 // 0xA6
    
    SH1106_WRITECOMMAND(SH1106_DISPLAYON);//--turn on oled panel

    /* Clear screen */
    TM_SH1106_Fill(SH1106_COLOR_BLACK);

    /* Update screen */
    TM_SH1106_UpdateScreen();

    /* Set default values */
    SH1106.CurrentX = 0;
    SH1106.CurrentY = 0;

    /* Initialized OK */
    SH1106.Initialized = 1;

    /* Return OK */
    return 1;
}

void TM_SH1106_UpdateScreen(void) {
    uint8_t m;
    uint8_t m_col = 2;

    for (m = 0; m < 8; m++) {
        SH1106_WRITECOMMAND(0xB0 + m);
        SH1106_WRITECOMMAND(m_col & 0xf);
        SH1106_WRITECOMMAND(0x10| (m_col >> 4));

        /* Write multi data */
        SH1106_REG_WRITE( 0x40, &SH1106.SH1106_Buffer[SH1106_WIDTH * m], SH1106_WIDTH);
    }
}

void TM_SH1106_ToggleInvert(void) {
	uint16_t i;
	
	/* Toggle invert */
	SH1106.Inverted = !SH1106.Inverted;
	
	/* Do memory toggle */
	for (i = 0; i < sizeof(SH1106.SH1106_Buffer); i++) {
		SH1106.SH1106_Buffer[i] = ~SH1106.SH1106_Buffer[i];
	}
}

void TM_SH1106_Fill(SH1106_COLOR_t color) {
	/* Set memory */
	memset(SH1106.SH1106_Buffer, (color == SH1106_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SH1106.SH1106_Buffer));
}

void TM_SH1106_DrawPixel(uint16_t x, uint16_t y, SH1106_COLOR_t color) {
	if (
		x >= SH1106_WIDTH ||
		y >= SH1106_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SH1106.Inverted) {
		color = (SH1106_COLOR_t)!color;
	}
	
	/* Set color */
	if (color == SH1106_COLOR_WHITE) {
		SH1106.SH1106_Buffer[x + (y / 8) * SH1106_WIDTH] |= 1 << (y % 8);
	} else {
		SH1106.SH1106_Buffer[x + (y / 8) * SH1106_WIDTH] &= ~(1 << (y % 8));
	}
}

void TM_SH1106_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SH1106.CurrentX = x;
	SH1106.CurrentY = y;
}

char TM_SH1106_Putc(char ch, TM_FontDef_t* Font, SH1106_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SH1106_WIDTH < (SH1106.CurrentX + Font->FontWidth) ||
		SH1106_HEIGHT < (SH1106.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				TM_SH1106_DrawPixel(SH1106.CurrentX + j, (SH1106.CurrentY + i), (SH1106_COLOR_t) color);
			} else {
				TM_SH1106_DrawPixel(SH1106.CurrentX + j, (SH1106.CurrentY + i), (SH1106_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	SH1106.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

char TM_SH1106_Puts(char* str, TM_FontDef_t* Font, SH1106_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (TM_SH1106_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

void TM_SH1106_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1106_COLOR_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SH1106_WIDTH) {
		x0 = SH1106_WIDTH - 1;
	}
	if (x1 >= SH1106_WIDTH) {
		x1 = SH1106_WIDTH - 1;
	}
	if (y0 >= SH1106_HEIGHT) {
		y0 = SH1106_HEIGHT - 1;
	}
	if (y1 >= SH1106_HEIGHT) {
		y1 = SH1106_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			TM_SH1106_DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			TM_SH1106_DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		TM_SH1106_DrawPixel(x0, y0, c); 
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void TM_SH1106_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c) {
	/* Check input parameters */
	if (
		x >= SH1106_WIDTH ||
		y >= SH1106_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SH1106_WIDTH) {
		w = SH1106_WIDTH - x;
	}
	if ((y + h) >= SH1106_HEIGHT) {
		h = SH1106_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	TM_SH1106_DrawLine(x, y, x + w, y, c);         /* Top line */
	TM_SH1106_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	TM_SH1106_DrawLine(x, y, x, y + h, c);         /* Left line */
	TM_SH1106_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void TM_SH1106_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c) {
	uint8_t i;
	
	/* Check input parameters */
	if (
		x >= SH1106_WIDTH ||
		y >= SH1106_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SH1106_WIDTH) {
		w = SH1106_WIDTH - x;
	}
	if ((y + h) >= SH1106_HEIGHT) {
		h = SH1106_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		TM_SH1106_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void TM_SH1106_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color) {
	/* Draw lines */
	TM_SH1106_DrawLine(x1, y1, x2, y2, color);
	TM_SH1106_DrawLine(x2, y2, x3, y3, color);
	TM_SH1106_DrawLine(x3, y3, x1, y1, color);
}


void TM_SH1106_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		TM_SH1106_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void TM_SH1106_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_SH1106_DrawPixel(x0, y0 + r, c);
    TM_SH1106_DrawPixel(x0, y0 - r, c);
    TM_SH1106_DrawPixel(x0 + r, y0, c);
    TM_SH1106_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_SH1106_DrawPixel(x0 + x, y0 + y, c);
        TM_SH1106_DrawPixel(x0 - x, y0 + y, c);
        TM_SH1106_DrawPixel(x0 + x, y0 - y, c);
        TM_SH1106_DrawPixel(x0 - x, y0 - y, c);

        TM_SH1106_DrawPixel(x0 + y, y0 + x, c);
        TM_SH1106_DrawPixel(x0 - y, y0 + x, c);
        TM_SH1106_DrawPixel(x0 + y, y0 - x, c);
        TM_SH1106_DrawPixel(x0 - y, y0 - x, c);
    }
}

void TM_SH1106_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_SH1106_DrawPixel(x0, y0 + r, c);
    TM_SH1106_DrawPixel(x0, y0 - r, c);
    TM_SH1106_DrawPixel(x0 + r, y0, c);
    TM_SH1106_DrawPixel(x0 - r, y0, c);
    TM_SH1106_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_SH1106_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        TM_SH1106_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        TM_SH1106_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        TM_SH1106_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}


static uint8_t CWM_BUFF_SHIFT(uint8_t hight, uint8_t low, uint8_t shift)
{
    uint16_t data;

    data = ((uint16_t)hight << 8) | (uint16_t)low;
    return (uint8_t)(data>> shift);
}

static void CWM_SET_SH1106_SCREEN_MEMORY(int x, int y8, uint8_t data)
{
    SH1106.SH1106_Buffer[x + (y8 * SH1106_WIDTH)] = data;
}

static void CWM_SH1106_SHIFT_PIXEL(int shift_y){
    uint8_t data;
    int shift_byte = shift_y / 8;
    int shift_bit = shift_y % 8;
    int x = 0, y = 0;

    for(y=0;y<((SH1106_HEIGHT/8) -shift_byte); y++){
        for(x=0;x<SH1106_WIDTH;x++){
            data = CWM_BUFF_SHIFT( SH1106.SH1106_Buffer[x + ((y + 1 + shift_byte) * SH1106_WIDTH)] ,SH1106.SH1106_Buffer[x + ((y + shift_byte) * SH1106_WIDTH)], shift_bit);
            CWM_SET_SH1106_SCREEN_MEMORY( x, y, data);
        }
    }
    if(shift_byte){
        for(y = (SH1106_HEIGHT/8)-1;y >((SH1106_HEIGHT/8) -shift_byte -1);y-- ){
            for(x=0;x<SH1106_WIDTH;x++){
                data = 0x00;
                CWM_SET_SH1106_SCREEN_MEMORY( x, y, data);
            }
        }
    }
    if(shift_bit){
        y = ((SH1106_HEIGHT/8) -shift_byte -1);
        for(x=0;x<SH1106_WIDTH;x++){
            data = SH1106.SH1106_Buffer[x + ((y) * SH1106_WIDTH)] & (0xff>>shift_bit);
            CWM_SET_SH1106_SCREEN_MEMORY( x, y, data);
        }
        
    }
}

char CWM_SH1106_Puts_Auto_newLine(char* str, TM_FontDef_t* Font, SH1106_COLOR_t color) {
    char temp_c;
    /* Write characters */
    while (*str) {
        if((SH1106.CurrentX + Font->FontWidth) >= SH1106_WIDTH){
            SH1106.CurrentX = 0;
            SH1106.CurrentY += Font->FontHeight;
        }else if ((SH1106.CurrentY + Font->FontHeight) > SH1106_HEIGHT){
            CWM_SH1106_SHIFT_PIXEL(Font->FontHeight - (SH1106_HEIGHT - SH1106.CurrentY));
            SH1106.CurrentY -= (Font->FontHeight - (SH1106_HEIGHT - SH1106.CurrentY) );
        }
    /* Write character by character */
        temp_c = TM_SH1106_Putc(*str, Font, color);
        if (temp_c != *str) {
        } else {
		/* Increase string pointer */
		str++;
        }
        if(str[0] == 0x0A){ // \n == ascii 0x0A  newline
            str++;
            SH1106.CurrentX = 0;
            SH1106.CurrentY += Font->FontHeight;            
        }
    }
    /* Everything OK, zero should be returned */
    return *str;
}

void TM_SH1106_ScreenClean(void){
    TM_SH1106_Fill(SH1106_COLOR_BLACK);
    TM_SH1106_GotoXY(0, 0);
}
 
void SH1106_ON(void) {
    SH1106_WRITECOMMAND(SH1106_DISPLAYON);//--turn on oled panel
}
void SH1106_OFF(void) {
    SH1106_WRITECOMMAND(SH1106_DISPLAYOFF);                    // 0xAE
}

#endif /*USE_OLED_DRIVER_SH1106*/
