#ifndef __TYPEDEF_H
#define __TYPEDEF_H



typedef unsigned char	u8;
typedef signed char		s8;
typedef unsigned short	u16;
typedef signed short    s16;
typedef unsigned long	u32;
typedef signed long		s32;
typedef u32				pType;

typedef enum
{
	FALSE = 0x00,
	TRUE  = 0x01
}BOOL;



typedef union _U8_VAL
{
	u8 Val;
    struct
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    } bits;
} U8_VAL;


typedef union _U16_VAL
{
    u16 Val;
    struct
    {
        u8 LSB;
        u8 MSB;
    } byte;
    u8 v[2];
} U16_VAL;

typedef union _U32_VAL
{
    u32 Val;
    struct
    {
        u8 LOLSB;
        u8 LOMSB;
        u8 HILSB;
        u8 HIMSB;
    } bytes;
    struct
    {
        u16 LSW;
        u16 MSW;
    } words;
    u8 v[4];
} U32_VAL;



#endif	//__TYPEDEF_H
