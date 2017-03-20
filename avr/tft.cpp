/* Arduino implementation for ili9341 */
#include <Arduino.h>
#include <SPI.h>
#include "tft.h"

// pin 4
#define TFT_RST_OFF {DDRD |= 0x10;PORTD |=  0x10;}
#define TFT_RST_ON  {DDRD |= 0x10;PORTD &=~ 0x10;}

// pin 5
#define TFT_CS_LOW  {DDRD |= 0x20;PORTD &=~ 0x20;}
#define TFT_CS_HIGH {DDRD |= 0x20;PORTD |=  0x20;}

// pin 6
#define TFT_DC_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
#define TFT_DC_HIGH {DDRD |= 0x40;PORTD |=  0x40;}

// pin 9
#define TFT_BL_OFF  {DDRB |= 0x02;PORTB &=~ 0x02;}
#define TFT_BL_ON   {DDRB |= 0x02;PORTB |=  0x02;}

static unsigned palette[256];

static void send_cmd(unsigned char c)
{
	TFT_DC_LOW;
	TFT_CS_LOW;
	SPI.transfer(c);
	TFT_CS_HIGH;
}

static void send_data8(unsigned char d)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI.transfer(d);
	TFT_CS_HIGH;
}

static void send_data16(unsigned d)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI.transfer(d >> 8);
	SPI.transfer(d & 0xff);
	TFT_CS_HIGH;
}

static void set_column(unsigned start, unsigned width)
{
	send_cmd(0x2A);                                         /* Column Command address       */
	send_data16(start);
	send_data16(start + width - 1);
}

static void set_page(unsigned start, unsigned height)
{
	send_cmd(0x2b);                                         /* Page Command address       */
	send_data16(start);
	send_data16(start + height - 1);
}

void tft_init(void)
{
	SPI.begin();
	TFT_CS_HIGH;
	TFT_DC_HIGH;

	TFT_RST_ON;
	delay(10);
	TFT_RST_OFF;

	send_cmd(0xCB);
	send_data8(0x39);
	send_data8(0x2C);
	send_data8(0x00);
	send_data8(0x34);
	send_data8(0x02);

	send_cmd(0xCF);
	send_data8(0x00);
	send_data8(0XC1);
	send_data8(0X30);

	send_cmd(0xE8);
	send_data8(0x85);
	send_data8(0x00);
	send_data8(0x78);

	send_cmd(0xEA);
	send_data8(0x00);
	send_data8(0x00);

	send_cmd(0xED);
	send_data8(0x64);
	send_data8(0x03);
	send_data8(0X12);
	send_data8(0X81);

	send_cmd(0xF7);
	send_data8(0x20);

	send_cmd(0xC0);    	//Power control
	send_data8(0x23);   	//VRH[5:0]

	send_cmd(0xC1);    	//Power control
	send_data8(0x10);   	//SAP[2:0];BT[3:0]

	send_cmd(0xC5);    	//VCM control
	send_data8(0x3e);   	//Contrast
	send_data8(0x28);

	send_cmd(0xC7);    	//VCM control2
	send_data8(0x86);  	//--

	send_cmd(0x36);    	// Memory Access Control

	//orientation
	send_data8(0xE8);  	//C8

	send_cmd(0x3A);
	send_data8(0x55); // 16 bit pixel

	send_cmd(0xB1);
	send_data8(0x00);
	send_data8(0x18);

	send_cmd(0xB6);    	// Display Function Control
	send_data8(0x08);
	send_data8(0x82);
	send_data8(0x27);

	send_cmd(0xF2);    	// 3Gamma Function Disable
	send_data8(0x00);

	send_cmd(0x26);    	//Gamma curve selected
	send_data8(0x01);

	send_cmd(0xE0);    	//Set Gamma
	send_data8(0x0F);
	send_data8(0x31);
	send_data8(0x2B);
	send_data8(0x0C);
	send_data8(0x0E);
	send_data8(0x08);
	send_data8(0x4E);
	send_data8(0xF1);
	send_data8(0x37);
	send_data8(0x07);
	send_data8(0x10);
	send_data8(0x03);
	send_data8(0x0E);
	send_data8(0x09);
	send_data8(0x00);

	send_cmd(0XE1);    	//Set Gamma
	send_data8(0x00);
	send_data8(0x0E);
	send_data8(0x14);
	send_data8(0x03);
	send_data8(0x11);
	send_data8(0x07);
	send_data8(0x31);
	send_data8(0xC1);
	send_data8(0x48);
	send_data8(0x08);
	send_data8(0x0F);
	send_data8(0x0C);
	send_data8(0x31);
	send_data8(0x36);
	send_data8(0x0F);

	send_cmd(0x11);    	//Exit Sleep
	delay(120);

	send_cmd(0x29);         //Display on
	send_cmd(0x2c);
}

void tft_fill(int x, int y, int w, int h, unsigned color)
{
	int i, j;

	w = min(w, WIDTH - x);
	h = min(h, HEIGHT - y);

	if (w && h) {
		set_column(x, w);
		set_page(y, h);

		send_cmd(0x2c);

		TFT_DC_HIGH;
		TFT_CS_LOW;

		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				SPI.transfer(color >> 8);
				SPI.transfer(color & 0xff);
			}
		}

		TFT_CS_HIGH;
	}
}

void tft_scroll(unsigned s) // 0..WIDTH-1
{
  send_cmd(0x37);
  send_data16(WIDTH - s);
}

void tft_cfg_scroll(unsigned top, unsigned bottom) // lines
{
  send_cmd(0x33);

  send_data16(top);
  send_data16(WIDTH - top - bottom);
  send_data16(bottom);
}

void tft_setpal(int idx, unsigned color)
{
	palette[idx] = color;
}

void tft_blit8(int x, int y, int w, int h, unsigned char *d)
{
	int i, j;

	set_column(x, w);
	set_page(y, h);

	send_cmd(0x2c);

	TFT_DC_HIGH;
	TFT_CS_LOW;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			unsigned c = palette[*d++];
			SPI.transfer(c >> 8);
			SPI.transfer(c & 0xff);
		}
	}

	TFT_CS_HIGH;
}

void tft_blit8x2(int x, int y, int w, int h, unsigned char *d)
{
	int i, j;

	set_column(x, w*2);
	set_page(y, h*2);

	send_cmd(0x2c);

	TFT_DC_HIGH;
	TFT_CS_LOW;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			unsigned c = palette[d[j]];
			SPI.transfer(c >> 8);
			SPI.transfer(c & 0xff);
			SPI.transfer(c >> 8);
			SPI.transfer(c & 0xff);
		}
		for (j = 0; j < w; j++) {
			unsigned c = palette[d[j]];
			SPI.transfer(c >> 8);
			SPI.transfer(c & 0xff);
			SPI.transfer(c >> 8);
			SPI.transfer(c & 0xff);
		}
		d += w;
	}

	TFT_CS_HIGH;
}

void tft_update(void)
{
}
