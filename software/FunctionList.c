#include "stm32f10x.h"
#include "FunctionList.h"

typedef void (*FunList_Type)(void);

static FunList_Type FunList_0xh[] =
    {
        FUNC_0X00,
        FUNC_0X01,
        FUNC_0X02,
        FUNC_0X03,
        FUNC_0X04,
        FUNC_0X05,
        FUNC_0X06,
        FUNC_0X07,
        FUNC_0X08,
        FUNC_0X09,
};

static FunList_Type FunList_1xh[] =
    {
        FUNC_0X10,
        FUNC_0X11,
        FUNC_0X12,
        FUNC_0X13,
        FUNC_0X14,
        FUNC_0X15,
        FUNC_0X16,
        FUNC_0X17,
        FUNC_0X18,
        FUNC_0X19,
};

static FunList_Type FunList_2xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_3xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_4xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_5xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_6xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_7xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_8xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type FunList_9xh[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
};

static FunList_Type *FunMenu[] =
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
#define FUNC_PTR FunMenu[(code & 0xf0) >> 4][code & 0x0f]
    (FUNC_PTR)();
}
