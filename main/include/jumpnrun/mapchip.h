#pragma once
struct LIST_OF_MAPCHIP
{
	char type;//n: ÈµAf: °Ah: ¼°
	int x, y;
	unsigned long int alpha;
	bool foreground, waterproof;
};

#define NUM_MAPCHIP 10*15
// n: no collision / f: floor, full collision 
// l: leaf, upper-half collision / h: half floor, lower half collision 
// w: water
static const unsigned int water_alpha = 0x55ffffff;
const LIST_OF_MAPCHIP list_of_mapchips[NUM_MAPCHIP] =
{
	{'f', 0, 0},
	{'f', 0, 64},
	{'f', 0, 64*2},
	{'h', 0, 64*3},
	{ 'n', 0, 64 * 4 },
	{ 'f', 0, 64 * 8 },
	{ 'f', 0, 64 * 10 },
	{'f', 0, 64*11},

	{'f', 64, 0},
	{'f', 64, 64},
	{'f', 64, 64*2},
	{'n', 64, 64*3},
	{'f', 0, 64*10},
	{'n', 64, 64*4},
	{ 'f', 64, 64 * 9 },
	{ 'f', 64, 64 * 10 },
	{ 'f', 64, 64 * 11 },

	{'f', 64*2, 0},
	{'f', 64*2, 64},
	{'f', 64*2, 64*2},
	{'h', 64*2, 64*3},
	{'f', 64*2, 64*4},
	{'f', 64*2, 64 * 10 },
	{ 'f', 64 * 2, 64 * 12 },
	{'f', 64*2, 64 * 13 },

	{'n', 64*3, 0},
	{'l', 64*3, 64},
	{'f', 64*3, 64*2},
	{'w', 64*3, 64*3, water_alpha, true },
	{ 'w', 64 * 3, 64 * 4, water_alpha, true},
	{ 'n', 64 * 3, 64 * 5 },
	{ 'h', 64 * 3, 64 * 6 },
{ 'n', 64 * 3, 64 * 7, 0xffffffff, true },
{ 'n', 64 * 3, 64 * 8, 0xffffffff, true },
{ 'h', 64 * 3, 64 * 9 },
{ 'l', 64 * 3, 64 * 10 },


	{'f', 64*4, 0, 0xffffffff, false, true },
	{'f', 64*4, 64, 0xffffffff, false, true},
	{'n', 64*4, 64*2},
	{'n', 64*4, 64*3},
	{'n', 64*4, 64*4},
{ 'h', 64 * 4, 64 * 6 },
{ 'l', 64 * 4, 64 * 7 },
{ 'l', 64 * 4, 64 * 10 },


	{'f', 64*5, 0},
	{'l', 64*5, 64},
	{'l', 64*5, 64*2},
	{'l', 64*5, 64*3},
	{ 'n', 64 * 5, 64 * 4 },
	{ 'f', 64 * 5, 64 * 6 },
	{ 'h', 64 * 5, 64 * 7 },
{ 'f', 64 * 5, 64 * 9 },
{ 'h', 64 * 5, 64 * 12 },

	{'f', 64*6, 0},
	{'h', 64*6, 64},
	{'h', 64*6, 64*2},
	{'h', 64*6, 64*3},
	{ 'n', 64 * 6, 64 * 4 },
	{ 'h', 64 * 6, 64 * 6 },

	{'f', 64*7, 0},
	{'f', 64*7, 64},
	{'f', 64*7, 64*2},
	{'h', 64*7, 64*3},
	{'n', 64*7, 64*4},

	{'f', 64*8, 0},
	{'f', 64*8, 64},
	{'f', 64*8, 64*2},
	{'h', 64*8, 64*3},
	{'n', 64*8, 64*4},

	{'f', 64*9, 0, true },
	{'f', 64*9, 64},
	{'f', 64*9, 64*2},
	{'h', 64*9, 64*3},
	{'n', 64 * 9, 64 * 4 },
	{'n', 64 * 9, 64 * 8, 0xffffffff, true },
	{'n', 64*9, 64*9, 0xffffffff, true },
	{ 'f', 64 * 9, 64 * 11 },
	{'f', 64 * 9, 64 * 12 },


	{'f', 64*10, 0, 0xffffffff, false, true },
	{'f', 64*10, 64},
	{'f', 64*10, 64*2},
	{'l', 64*10, 64*3},
	{ 'l', 64 * 10, 64 * 4 },
	{ 'l', 64 * 10, 64 * 6 },
	{ 'h', 64 * 10, 64 * 7 },
	{ 'f', 64 * 10, 64 * 8 },
{ 'h', 64 * 10, 64 * 9 },
	{ 'f', 64 * 10, 64 * 10 },
	{ 'f', 64 * 10, 64 * 14 },

	{'n', 64*11, 0},
	{'n', 64*11, 64},
	{'h', 64*11, 64*2},
	{'l', 64*11, 64*3},
	{ 'n', 64 * 11, 64 * 4 },
	{ 'f', 64 * 11, 64 * 6 },
	{ 'f', 64 * 11, 64 * 7 },
	{ 'n', 64 * 11, 64 * 8 },
	{ 'n', 64 * 11, 64 * 9 },
	{ 'h', 64 * 11, 64 * 11 },

	{ 'h', 64 * 12, 64 * 3 },
	{ 'n', 64 * 12, 64 * 6 },
	{ 'n', 64 * 12, 64 * 7 },
	{ 'n', 64 * 12, 64 * 8 },

	{ 'f', 64 * 13, 0 },
	{ 'f', 64 * 13, 64 },
	{ 'h', 64 * 13, 64 * 2 },
	{ 'h', 64 * 13, 64 * 3 },
	{ 'n', 64 * 13, 64 * 4 },
	{ 'f', 64 * 13, 64 * 5 },
	//{ 'f', 64 * 13, 64 * 6 },
	{ 'n', 64 * 13, 64 * 6 },
	{ 'n', 64 * 13, 64 * 7 },
	{ 'n', 64 * 13, 64 * 8 },

	{ 'f', 64 * 14, 0 },
	{ 'f', 64 * 14, 64 },
	{ 'f', 64 * 14, 64 * 2 },
	{ 'n', 64 * 14, 64 * 3 },
	{ 'n', 64 * 14, 64 * 4 },
	{ 'n', 64 * 14, 64 * 5 },
	{ 'n', 64 * 14, 64 * 6 },
	{ 'n', 64 * 14, 64 * 7 },
	{ 'n', 64 * 14, 64 * 8 },
	{ 'l', 64 * 14, 64 * 9 },
};