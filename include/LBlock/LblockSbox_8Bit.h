/*************************************
@berif  : this file is the 8-bit S-box of Lblock.
		  It include (s1,s0),(s3,s2),(s5,s4),(s7,s6),(s9,s8).
		  It is writed to test the speed of LBlock80.
@author : Shusheng Liu
@time   : 11.4 2011
@Copyright (c) 2010, south china normal university
**************************************/


#ifndef _LBLOCKSBOX_8BIT_H_
#define _LBLOCKSBOX_8BIT_H_

typedef unsigned char uint8;

uint8 sbox0[256]={0x4e,0x49,0x4f,0x40,0x4d,0x44,0x4a,0x4b,0x41,0x42,0x48,0x43,0x47,0x46,0x4c,0x45,
0xbe,0xb9,0xbf,0xb0,0xbd,0xb4,0xba,0xbb,0xb1,0xb2,0xb8,0xb3,0xb7,0xb6,0xbc,0xb5,
0xee,0xe9,0xef,0xe0,0xed,0xe4,0xea,0xeb,0xe1,0xe2,0xe8,0xe3,0xe7,0xe6,0xec,0xe5,
0x9e,0x99,0x9f,0x90,0x9d,0x94,0x9a,0x9b,0x91,0x92,0x98,0x93,0x97,0x96,0x9c,0x95,
0xfe,0xf9,0xff,0xf0,0xfd,0xf4,0xfa,0xfb,0xf1,0xf2,0xf8,0xf3,0xf7,0xf6,0xfc,0xf5,
0xde,0xd9,0xdf,0xd0,0xdd,0xd4,0xda,0xdb,0xd1,0xd2,0xd8,0xd3,0xd7,0xd6,0xdc,0xd5,
0x0e,0x09,0x0f,0x00,0x0d,0x04,0x0a,0x0b,0x01,0x02,0x08,0x03,0x07,0x06,0x0c,0x05,
0xae,0xa9,0xaf,0xa0,0xad,0xa4,0xaa,0xab,0xa1,0xa2,0xa8,0xa3,0xa7,0xa6,0xac,0xa5,
0x7e,0x79,0x7f,0x70,0x7d,0x74,0x7a,0x7b,0x71,0x72,0x78,0x73,0x77,0x76,0x7c,0x75,
0xce,0xc9,0xcf,0xc0,0xcd,0xc4,0xca,0xcb,0xc1,0xc2,0xc8,0xc3,0xc7,0xc6,0xcc,0xc5,
0x5e,0x59,0x5f,0x50,0x5d,0x54,0x5a,0x5b,0x51,0x52,0x58,0x53,0x57,0x56,0x5c,0x55,
0x6e,0x69,0x6f,0x60,0x6d,0x64,0x6a,0x6b,0x61,0x62,0x68,0x63,0x67,0x66,0x6c,0x65,
0x2e,0x29,0x2f,0x20,0x2d,0x24,0x2a,0x2b,0x21,0x22,0x28,0x23,0x27,0x26,0x2c,0x25,
0x8e,0x89,0x8f,0x80,0x8d,0x84,0x8a,0x8b,0x81,0x82,0x88,0x83,0x87,0x86,0x8c,0x85,
0x1e,0x19,0x1f,0x10,0x1d,0x14,0x1a,0x1b,0x11,0x12,0x18,0x13,0x17,0x16,0x1c,0x15,
0x3e,0x39,0x3f,0x30,0x3d,0x34,0x3a,0x3b,0x31,0x32,0x38,0x33,0x37,0x36,0x3c,0x35
};

uint8 sbox1[256]={0x71,0x7e,0x77,0x7c,0x7f,0x7d,0x70,0x76,0x7b,0x75,0x79,0x73,0x72,0x74,0x78,0x7a,
0x61,0x6e,0x67,0x6c,0x6f,0x6d,0x60,0x66,0x6b,0x65,0x69,0x63,0x62,0x64,0x68,0x6a,
0x81,0x8e,0x87,0x8c,0x8f,0x8d,0x80,0x86,0x8b,0x85,0x89,0x83,0x82,0x84,0x88,0x8a,
0xb1,0xbe,0xb7,0xbc,0xbf,0xbd,0xb0,0xb6,0xbb,0xb5,0xb9,0xb3,0xb2,0xb4,0xb8,0xba,
0x01,0x0e,0x07,0x0c,0x0f,0x0d,0x00,0x06,0x0b,0x05,0x09,0x03,0x02,0x04,0x08,0x0a,
0xf1,0xfe,0xf7,0xfc,0xff,0xfd,0xf0,0xf6,0xfb,0xf5,0xf9,0xf3,0xf2,0xf4,0xf8,0xfa,
0x31,0x3e,0x37,0x3c,0x3f,0x3d,0x30,0x36,0x3b,0x35,0x39,0x33,0x32,0x34,0x38,0x3a,
0xe1,0xee,0xe7,0xec,0xef,0xed,0xe0,0xe6,0xeb,0xe5,0xe9,0xe3,0xe2,0xe4,0xe8,0xea,
0x91,0x9e,0x97,0x9c,0x9f,0x9d,0x90,0x96,0x9b,0x95,0x99,0x93,0x92,0x94,0x98,0x9a,
0xa1,0xae,0xa7,0xac,0xaf,0xad,0xa0,0xa6,0xab,0xa5,0xa9,0xa3,0xa2,0xa4,0xa8,0xaa,
0xc1,0xce,0xc7,0xcc,0xcf,0xcd,0xc0,0xc6,0xcb,0xc5,0xc9,0xc3,0xc2,0xc4,0xc8,0xca,
0xd1,0xde,0xd7,0xdc,0xdf,0xdd,0xd0,0xd6,0xdb,0xd5,0xd9,0xd3,0xd2,0xd4,0xd8,0xda,
0x51,0x5e,0x57,0x5c,0x5f,0x5d,0x50,0x56,0x5b,0x55,0x59,0x53,0x52,0x54,0x58,0x5a,
0x21,0x2e,0x27,0x2c,0x2f,0x2d,0x20,0x26,0x2b,0x25,0x29,0x23,0x22,0x24,0x28,0x2a,
0x41,0x4e,0x47,0x4c,0x4f,0x4d,0x40,0x46,0x4b,0x45,0x49,0x43,0x42,0x44,0x48,0x4a,
0x11,0x1e,0x17,0x1c,0x1f,0x1d,0x10,0x16,0x1b,0x15,0x19,0x13,0x12,0x14,0x18,0x1a
};

uint8 sbox2[256]={0x2e,0x25,0x2f,0x20,0x27,0x22,0x2c,0x2d,0x21,0x28,0x24,0x29,0x2b,0x2a,0x26,0x23,
0xde,0xd5,0xdf,0xd0,0xd7,0xd2,0xdc,0xdd,0xd1,0xd8,0xd4,0xd9,0xdb,0xda,0xd6,0xd3,
0xbe,0xb5,0xbf,0xb0,0xb7,0xb2,0xbc,0xbd,0xb1,0xb8,0xb4,0xb9,0xbb,0xba,0xb6,0xb3,
0xce,0xc5,0xcf,0xc0,0xc7,0xc2,0xcc,0xcd,0xc1,0xc8,0xc4,0xc9,0xcb,0xca,0xc6,0xc3,
0xfe,0xf5,0xff,0xf0,0xf7,0xf2,0xfc,0xfd,0xf1,0xf8,0xf4,0xf9,0xfb,0xfa,0xf6,0xf3,
0xee,0xe5,0xef,0xe0,0xe7,0xe2,0xec,0xed,0xe1,0xe8,0xe4,0xe9,0xeb,0xea,0xe6,0xe3,
0x0e,0x05,0x0f,0x00,0x07,0x02,0x0c,0x0d,0x01,0x08,0x04,0x09,0x0b,0x0a,0x06,0x03,
0x9e,0x95,0x9f,0x90,0x97,0x92,0x9c,0x9d,0x91,0x98,0x94,0x99,0x9b,0x9a,0x96,0x93,
0x7e,0x75,0x7f,0x70,0x77,0x72,0x7c,0x7d,0x71,0x78,0x74,0x79,0x7b,0x7a,0x76,0x73,
0xae,0xa5,0xaf,0xa0,0xa7,0xa2,0xac,0xad,0xa1,0xa8,0xa4,0xa9,0xab,0xaa,0xa6,0xa3,
0x6e,0x65,0x6f,0x60,0x67,0x62,0x6c,0x6d,0x61,0x68,0x64,0x69,0x6b,0x6a,0x66,0x63,
0x3e,0x35,0x3f,0x30,0x37,0x32,0x3c,0x3d,0x31,0x38,0x34,0x39,0x3b,0x3a,0x36,0x33,
0x1e,0x15,0x1f,0x10,0x17,0x12,0x1c,0x1d,0x11,0x18,0x14,0x19,0x1b,0x1a,0x16,0x13,
0x8e,0x85,0x8f,0x80,0x87,0x82,0x8c,0x8d,0x81,0x88,0x84,0x89,0x8b,0x8a,0x86,0x83,
0x4e,0x45,0x4f,0x40,0x47,0x42,0x4c,0x4d,0x41,0x48,0x44,0x49,0x4b,0x4a,0x46,0x43,
0x5e,0x55,0x5f,0x50,0x57,0x52,0x5c,0x5d,0x51,0x58,0x54,0x59,0x5b,0x5a,0x56,0x53
};

uint8 sbox3[256]={0xdb,0xd9,0xd4,0xde,0xd0,0xdf,0xda,0xdd,0xd6,0xdc,0xd5,0xd7,0xd3,0xd8,0xd1,0xd2,
0xab,0xa9,0xa4,0xae,0xa0,0xaf,0xaa,0xad,0xa6,0xac,0xa5,0xa7,0xa3,0xa8,0xa1,0xa2,
0xfb,0xf9,0xf4,0xfe,0xf0,0xff,0xfa,0xfd,0xf6,0xfc,0xf5,0xf7,0xf3,0xf8,0xf1,0xf2,
0x0b,0x09,0x04,0x0e,0x00,0x0f,0x0a,0x0d,0x06,0x0c,0x05,0x07,0x03,0x08,0x01,0x02,
0xeb,0xe9,0xe4,0xee,0xe0,0xef,0xea,0xed,0xe6,0xec,0xe5,0xe7,0xe3,0xe8,0xe1,0xe2,
0x4b,0x49,0x44,0x4e,0x40,0x4f,0x4a,0x4d,0x46,0x4c,0x45,0x47,0x43,0x48,0x41,0x42,
0x9b,0x99,0x94,0x9e,0x90,0x9f,0x9a,0x9d,0x96,0x9c,0x95,0x97,0x93,0x98,0x91,0x92,
0xbb,0xb9,0xb4,0xbe,0xb0,0xbf,0xba,0xbd,0xb6,0xbc,0xb5,0xb7,0xb3,0xb8,0xb1,0xb2,
0x2b,0x29,0x24,0x2e,0x20,0x2f,0x2a,0x2d,0x26,0x2c,0x25,0x27,0x23,0x28,0x21,0x22,
0x1b,0x19,0x14,0x1e,0x10,0x1f,0x1a,0x1d,0x16,0x1c,0x15,0x17,0x13,0x18,0x11,0x12,
0x8b,0x89,0x84,0x8e,0x80,0x8f,0x8a,0x8d,0x86,0x8c,0x85,0x87,0x83,0x88,0x81,0x82,
0x3b,0x39,0x34,0x3e,0x30,0x3f,0x3a,0x3d,0x36,0x3c,0x35,0x37,0x33,0x38,0x31,0x32,
0x7b,0x79,0x74,0x7e,0x70,0x7f,0x7a,0x7d,0x76,0x7c,0x75,0x77,0x73,0x78,0x71,0x72,
0x5b,0x59,0x54,0x5e,0x50,0x5f,0x5a,0x5d,0x56,0x5c,0x55,0x57,0x53,0x58,0x51,0x52,
0xcb,0xc9,0xc4,0xce,0xc0,0xcf,0xca,0xcd,0xc6,0xcc,0xc5,0xc7,0xc3,0xc8,0xc1,0xc2,
0x6b,0x69,0x64,0x6e,0x60,0x6f,0x6a,0x6d,0x66,0x6c,0x65,0x67,0x63,0x68,0x61,0x62
};

uint8 sboxk[256]={0xb8,0xb7,0xbe,0xb5,0xbf,0xbd,0xb0,0xb6,0xbb,0xbc,0xb9,0xba,0xb2,0xb4,0xb1,0xb3,
0x58,0x57,0x5e,0x55,0x5f,0x5d,0x50,0x56,0x5b,0x5c,0x59,0x5a,0x52,0x54,0x51,0x53,
0xf8,0xf7,0xfe,0xf5,0xff,0xfd,0xf0,0xf6,0xfb,0xfc,0xf9,0xfa,0xf2,0xf4,0xf1,0xf3,
0x08,0x07,0x0e,0x05,0x0f,0x0d,0x00,0x06,0x0b,0x0c,0x09,0x0a,0x02,0x04,0x01,0x03,
0x78,0x77,0x7e,0x75,0x7f,0x7d,0x70,0x76,0x7b,0x7c,0x79,0x7a,0x72,0x74,0x71,0x73,
0x28,0x27,0x2e,0x25,0x2f,0x2d,0x20,0x26,0x2b,0x2c,0x29,0x2a,0x22,0x24,0x21,0x23,
0x98,0x97,0x9e,0x95,0x9f,0x9d,0x90,0x96,0x9b,0x9c,0x99,0x9a,0x92,0x94,0x91,0x93,
0xd8,0xd7,0xde,0xd5,0xdf,0xdd,0xd0,0xd6,0xdb,0xdc,0xd9,0xda,0xd2,0xd4,0xd1,0xd3,
0x48,0x47,0x4e,0x45,0x4f,0x4d,0x40,0x46,0x4b,0x4c,0x49,0x4a,0x42,0x44,0x41,0x43,
0x88,0x87,0x8e,0x85,0x8f,0x8d,0x80,0x86,0x8b,0x8c,0x89,0x8a,0x82,0x84,0x81,0x83,
0x18,0x17,0x1e,0x15,0x1f,0x1d,0x10,0x16,0x1b,0x1c,0x19,0x1a,0x12,0x14,0x11,0x13,
0xc8,0xc7,0xce,0xc5,0xcf,0xcd,0xc0,0xc6,0xcb,0xcc,0xc9,0xca,0xc2,0xc4,0xc1,0xc3,
0xe8,0xe7,0xee,0xe5,0xef,0xed,0xe0,0xe6,0xeb,0xec,0xe9,0xea,0xe2,0xe4,0xe1,0xe3,
0xa8,0xa7,0xae,0xa5,0xaf,0xad,0xa0,0xa6,0xab,0xac,0xa9,0xaa,0xa2,0xa4,0xa1,0xa3,
0x38,0x37,0x3e,0x35,0x3f,0x3d,0x30,0x36,0x3b,0x3c,0x39,0x3a,0x32,0x34,0x31,0x33,
0x68,0x67,0x6e,0x65,0x6f,0x6d,0x60,0x66,0x6b,0x6c,0x69,0x6a,0x62,0x64,0x61,0x63
};

#endif