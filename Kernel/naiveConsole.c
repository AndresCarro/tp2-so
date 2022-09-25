#include <naiveConsole.h>

#define WIDTH 80
#define HEIGHT 25
#define VIDEOSTART 0xB8000
#define WINDOWS 2

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *)VIDEOSTART;
static uint8_t *currentVideo = (uint8_t *)VIDEOSTART;
static const uint32_t width = WIDTH;
static const uint32_t height = HEIGHT;

//Window currently writing to.
static uint8_t currentWindow = 0;

//Amount of windows
static uint8_t windows = 1;

//Starting point for windows
static uint8_t *videoWindow[2] = {(uint8_t*)VIDEOSTART, (uint8_t*)VIDEOSTART + WIDTH};

//Pointer to windows
static uint8_t *currentVideoW[2] = {(uint8_t*)VIDEOSTART, (uint8_t*)VIDEOSTART + WIDTH};

//Default format color
static const uint8_t defaultFormat = 0x07;

//Select amount of windows
uint8_t ncWindows(uint8_t amount){
	if (amount >= 2){
		if (windows != 2)
			ncClear();
		return windows = 2;
	}
	ncCurrentWindow(0);
	if (windows != 1){
		windows = 1;
		currentVideo = (currentVideoW[0] > currentVideoW[1]) ? currentVideoW[0] : currentVideoW[1];
		for (int i = 0; i < WINDOWS; i++)
		{
			currentVideoW[i] = videoWindow[i];
		}
		ncNewline();
		ncNewline();
	}
	return windows;
}

//Select windows to write to
uint8_t ncCurrentWindow(uint8_t select){
	if (select > 0)
		return currentWindow = 1;
	return currentWindow = 0;
}

void ncDeleteChar(){
	if (windows == 1){
		if (currentVideo-2 >= video){
			currentVideo -= 2;
			*currentVideo = ' ';
			*(currentVideo+1) = defaultFormat;
		}
		return;
	} else {
		if (currentVideoW[currentWindow]-2 >= videoWindow[currentWindow]){
			currentVideoW[currentWindow] -= 2;
			*(currentVideoW[currentWindow]) = ' ';
			*(currentVideoW[currentWindow]+1) = defaultFormat;

		}
	}
}

void ncPrintFormat(const char* string,uint8_t format){
	for(int i=0;string[i]!='\0';i++){
		ncPrintCharFormat(string[i],format);
	}
}

void ncPrintCharFormat(char character,uint8_t format){
	switch (character)
	{
	case '\n':
		ncNewline();
		return;
		break;
	case '\b':
		ncDeleteChar();
		return;
		break;
	default:
		break;
	}
	if (windows == 1){
		*(currentVideo++) = character;
		*(currentVideo++) = format;
		if ((uint64_t)(currentVideo - video) / (2*width) >= height)
			scrollUp();
	} else {
		*(currentVideoW[currentWindow]++) = character;
		*(currentVideoW[currentWindow]++) = format;
		if (currentWindow && ((currentVideoW[currentWindow] - video) % (2*width) <= width)){
			currentVideoW[currentWindow] += width;
		} else if (!currentWindow && ((currentVideoW[currentWindow] - video) % (2*width) >= width))
			currentVideoW[currentWindow] += width;
		if ((uint64_t)(currentVideoW[currentWindow] - video) / (width*2) >= height)
			scrollUp();
	}
}

void ncPrint(const char *string)
{
	int i; 

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	ncPrintCharFormat(character, defaultFormat);
}

void ncNewline()
{
	if (windows == 1){
		do
		{
			ncPrintChar(' ');
		} while ((uint64_t)(currentVideo - video) % (width * 2) != 0);
	} else {
		do
		{
			ncPrintChar(' ');
		} while ((uint64_t)(currentVideoW[currentWindow] - video) % (width * 2) != ( currentWindow ? width : 0 ));
		
	}
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncClear()
{
	
	if (windows==1){
		int i;

		for (i = 0; i < height * width; i++){
			video[i * 2] = ' ';
			video[i*2+1] = defaultFormat;
		}
		
		currentVideo = video;
	} else {
		ncClearWindow(currentWindow);
	}
}

void ncClearWindow(uint8_t windowToCLear){
	int i;
	uint32_t widthW;
	if (windowToCLear==0){
		widthW=width/2;
		i=0;
	}
	else{
		widthW=width;
		i=width/2 ;
	}
		
	while(i < height*width){
		video[i*2]=' ';
		video[i*2+1] = defaultFormat;
		if (i && (i+1)%widthW==0)
			i+=width/2;
		i++;
	}
	currentVideoW[windowToCLear]=videoWindow[windowToCLear];
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void scrollUp(){
	// Scroll all
	if (windows == 1){
		uint64_t i;
		for (i = 0; i <= width * (height-1); i++)
		{
			video[i*2] = video[(i+width)*2];
			video[i*2+1] = video[(i+width)*2+1];	// Copy format too.
		}
		for (uint64_t j = i; j < width * height; j++)
		{
			video[j*2] = ' ';
			video[j*2+1] = defaultFormat;
		}
		currentVideo -= width*2;
	} else {	// Scroll a window
		uint64_t i = 0;
		uint32_t widthW;
		if (currentWindow==0){
			widthW=width/2;
			i=0;
		}
		else{
			widthW=width;
			i=width/2 ;
		}
			
		while(i < (HEIGHT-1)*WIDTH){
			video[i*2]=video[(i+WIDTH)*2];
			video[i*2+1]=video[(i+WIDTH)*2+1];	// Copy format
			if (i && (i+1)%widthW==0)
				i+=WIDTH/2;
			i++;
		}
		while (i < height*width)
		{
			video[i*2]=' ';
			video[i*2]=defaultFormat;
			if (i && (i+1)%widthW==0)
				i+=width/2;
			i++;
		}
		currentVideoW[currentWindow]=videoWindow[currentWindow] + width*2*(height-1);
		// Hardcoded memory directions.
		// currentVideoW[currentWindow]=(currentWindow) ? (0xb8f00 + width) : (0xb8f00);
	}
}
