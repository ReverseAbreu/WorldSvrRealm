#pragma once

#pragma pack( push, 1 )
struct S2C_HEADER
{
    WORD wMagicCode;
    WORD wPayLoadLen;
    WORD wMainCmd;

    S2C_HEADER() :
        wMagicCode(MAGIC_CODE),
        wPayLoadLen(0),
        wMainCmd(0)
    {
    }

    S2C_HEADER(WORD len, WORD cmd) :
        wMagicCode(MAGIC_CODE),
        wPayLoadLen(len),
        wMainCmd(cmd)
    {
    }
};

struct S2C_HEADER_EX
{
    WORD wMagicCode;
    WORD wPayLoadLen;
    WORD wMainCmd;
    WORD wMainCmdEx;

    S2C_HEADER_EX() :
        wMagicCode(MAGIC_CODE),
        wPayLoadLen(0),
        wMainCmd(0)
    {
    }

    S2C_HEADER_EX(WORD len, WORD cmd, WORD exCmd) :
        wMagicCode(MAGIC_CODE),
        wPayLoadLen(len),
        wMainCmd(MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET),
        wMainCmdEx(exCmd)
    {
    }
};


struct exS2C_HEADER : public S2C_HEADER
{
    WORD exMainCmd;

    exS2C_HEADER(WORD len, WORD cmd, WORD excmd) :
        exMainCmd(excmd),
        S2C_HEADER::S2C_HEADER(len, cmd) {}
};

struct sNFS_LASTERRCODE : public S2C_HEADER
{
    WORD					wReqMainCmd;
    WORD					wReqMainCmdEx,
                            wLastErrCode,
                            wDeadType;
};

#pragma pack(pop)