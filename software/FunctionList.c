#include "stm32f10x.h"
#include "FunctionList.h"
uint8_t mode8, size8, num8;
u16 sx16, sy16, ex16, ey16, color16, addr16, num16;
u32 addr32, num32;

typedef struct
{
    void *Func;   //函数指针
    uint8_t Code; //函数编码

} _FunList_TypeDef;

_FunList_TypeDef FunList_0xh[] =
    {
        (void *)Fun_HandShake_00h,
        0x00,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
};

_FunList_TypeDef FunList_1xh[] =
    {
        (void *)Fun_DisplayConfig_10h,
        0x10,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_2xh[] =
    {
        (void *)Fun_ShowFont_20h,
        0x20,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_3xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_4xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_5xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_6xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)Fun_LCDClear_65h,
        0x65,
        (void *)Fun_FillRectangle_66h,
        0x66,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_7xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
};

_FunList_TypeDef FunList_8xh[] =
    {
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,
        (void *)NULL,
        0,

};

_FunList_TypeDef FunList_9xh[] =
    {
        (void *)NULL,
        0,
        (void *)Fun_DisplayPic_91h,
        0x91,
        (void *)Fun_SetDisplayDir_92h,
        0x92,
        (void *)NULL,
        0,
        (void *)Fun_WR_EE_94h,
        0x94,
        (void *)Fun_RD_EE_95h,
        0x95,
        (void *)Fun_WR_FLASH_96h,
        0x96,
        (void *)Fun_RD_FLASH_97h,
        0x97,
        (void *)Fun_FileDownLoad_98h,
        0x98,
        (void *)NULL,
        0,

};

_FunList_TypeDef *FunMenu[] =
    {
        FunList_0xh,
        FunList_1xh,
        FunList_2xh,
        FunList_3xh,
        FunList_4xh,
        FunList_5xh,
        FunList_6xh,
        FunList_7xh,
        FunList_8xh,
        FunList_9xh,
};

/*回调函数*/
void Fun_Ack_CallBack(uint8_t FunCode)
{
    UART1_SendByte(0xAA); //返回 AA FunCode 00
    UART1_SendByte(FunCode);
    UART1_SendByte(0x00);
}
/********************接口函数**********************/
void Fun_HandShake_00h(uint8_t FunCode)
{
    Fun_Ack_CallBack(FunCode);
}
#if 0
/*调用对应函数*/
(*(u32 (*)())((FunMenu[temp >> 4][temp & 0x0F]).Func))((FunMenu[temp >> 4][temp & 0x0F]).Code);
#endif
