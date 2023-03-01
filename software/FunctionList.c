#include "stm32f10x.h"
#include "FunctionList.h"

typedef struct
{
    void *Func;   //º¯ÊýÖ¸Õë
    uint8_t Code; //º¯Êý±àÂë

} _FunList_TypeDef;

_FunList_TypeDef FunList_0xh[] =
    {
        (void *)FUNC_0X00,
        0x00,
        (void *)FUNC_0X01,
        0x01,
        (void *)FUNC_0X02,
        0x02,
        (void *)FUNC_0X03,
        0x03,
        (void *)FUNC_0X04,
        0x04,
        (void *)FUNC_0X05,
        0x05,
        (void *)FUNC_0X06,
        0x06,
        (void *)FUNC_0X07,
        0x07,
        (void *)FUNC_0X08,
        0x08,
        (void *)FUNC_0X09,
        0x09,
};

_FunList_TypeDef FunList_1xh[] =
    {
        (void *)NULL,
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
        (void *)NULL,
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
        (void *)NULL,
        0x65,
        (void *)NULL,
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
        (void *)NULL,
        0x91,
        (void *)NULL,
        0x92,
        (void *)NULL,
        0,
        (void *)NULL,
        0x94,
        (void *)NULL,
        0x95,
        (void *)NULL,
        0x96,
        (void *)NULL,
        0x97,
        (void *)NULL,
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

void FunList_Call(uint8_t code)
{
#define FUNC_PTR_TYPE void (*)(void)
#define FUNC_PTR FunMenu[(code&0xf0)>>4][code&0x0f].Func
    ((FUNC_PTR_TYPE)(FUNC_PTR))();
}
