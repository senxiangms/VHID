// VHidDll.cpp : 定义 DLL 应用程序的导出函数。

#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "VHidDll.h"
#include "vmulticlient.h"
BOOL Translate(char c, BYTE* shift, BYTE* code);
void InitMap();

bool SendHidRequests(pvmulti_client vmulti, const char *str, unsigned int interval)
{
	InitMap();
	bool is_success_input = true;
	BYTE keyCodes[KBD_KEY_CODES] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0; str[i] != '\0'; i++)
	{
		BYTE shift = 0;
		BYTE code = 0;
		if (Translate(str[i], &shift, &code))
		{
			keyCodes[0] = code;
			vmulti_update_keyboard(vmulti, shift, keyCodes);
			keyCodes[0] = 0x0;
			vmulti_update_keyboard(vmulti, 0x0, keyCodes);
			Sleep(interval);
		}
		else
		{
			is_success_input = false;
			break;
		}
	}
	keyCodes[0] = 0x0;
	vmulti_update_keyboard(vmulti, 0x0, keyCodes);
	return is_success_input;
}

int __stdcall InputString(const char *str, unsigned int interval)
{
	int ret_code = 0;
	if (strlen(str) == 0 || !str)
	{
		ret_code = 1;	//字符串为空
		return ret_code;
	}
	pvmulti_client vmulti = vmulti_alloc();
	if (!vmulti)
	{
		ret_code = 2;	//内存分配失败
		return ret_code;
	}
	if (!vmulti_connect(vmulti))
	{
		vmulti_free(vmulti);
		ret_code = 3;	//驱动问题
		return ret_code;
	}
	if (!SendHidRequests(vmulti, str, interval))
		ret_code = 4;	//出现不支持的字符

	vmulti_disconnect(vmulti);
	vmulti_free(vmulti);
	return ret_code;
}


typedef struct _ReportCode
{
	BYTE _shift;
	BYTE _code;
} ReportCode;

static  ReportCode s_Map[256];

void InitMap()
{
	char c;
	c = 'a';
	for (int i = 0; i < 256; i++)
	{
		s_Map[i]._shift = -1;
		s_Map[i]._code = -1;
	}
	while (c >= 'a' && c <= 'z')
	{
		s_Map[c]._shift = 0;
		s_Map[c]._code = 4 + c - 'a'; // A -- Z mapped to 4 -- 1D
		c++;
	}
	c = 'A';
	while (c >= 'A' && c <= 'Z')
	{
		s_Map[c]._shift = KBD_LSHIFT_BIT;
		s_Map[c]._code = 4 + c - 'A';
		c++;
	}

	s_Map['1']._shift = 0;					s_Map['1']._code = 0x1E;
	s_Map['!']._shift = KBD_LSHIFT_BIT;		s_Map['!']._code = 0x1E;
	s_Map['2']._shift = 0;					s_Map['2']._code = 0x1F;
	s_Map['@']._shift = KBD_LSHIFT_BIT;		s_Map['@']._code = 0x1F;
	s_Map['3']._shift = 0;					s_Map['3']._code = 0x20;
	s_Map['#']._shift = KBD_LSHIFT_BIT;		s_Map['#']._code = 0x20;
	s_Map['4']._shift = 0;					s_Map['4']._code = 0x21;
	s_Map['$']._shift = KBD_LSHIFT_BIT;		s_Map['$']._code = 0x21;
	s_Map['5']._shift = 0;					s_Map['5']._code = 0x22;
	s_Map['%']._shift = KBD_LSHIFT_BIT;		s_Map['%']._code = 0x22;
	s_Map['6']._shift = 0;					s_Map['6']._code = 0x23;
	s_Map['^']._shift = KBD_LSHIFT_BIT;		s_Map['^']._code = 0x23;
	s_Map['7']._shift = 0;					s_Map['7']._code = 0x24;
	s_Map['&']._shift = KBD_LSHIFT_BIT;		s_Map['&']._code = 0x24;
	s_Map['8']._shift = 0;					s_Map['8']._code = 0x25;
	s_Map['*']._shift = KBD_LSHIFT_BIT;		s_Map['*']._code = 0x25;
	s_Map['9']._shift = 0;					s_Map['9']._code = 0x26;
	s_Map['(']._shift = KBD_LSHIFT_BIT;		s_Map['(']._code = 0x26;
	s_Map['0']._shift = 0;					s_Map['0']._code = 0x27;
	s_Map[')']._shift = KBD_LSHIFT_BIT;		s_Map[')']._code = 0x27;
	s_Map['\n']._shift = 0;					s_Map['\n']._code = 0x28;
	//ESC -- 0x29
	//back space : 0x2a
	s_Map['\r']._shift = 0;					s_Map['\r']._code = 0x28;
	s_Map['\b']._shift = 0;					s_Map['\b']._code = 0x2a;
	s_Map['\t']._shift = 0;					s_Map['\t']._code = 0x2b;
	s_Map[' ']._shift = 0;					s_Map[' ']._code = 0x2c;
	s_Map['-']._shift = 0;					s_Map['-']._code = 0x2d;
	s_Map['_']._shift = KBD_LSHIFT_BIT;		s_Map['_']._code = 0x2d;
	s_Map['=']._shift = 0;					s_Map['=']._code = 0x2e;
	s_Map['+']._shift = KBD_LSHIFT_BIT;		s_Map['+']._code = 0x2e;
	s_Map['[']._shift = 0;					s_Map['[']._code = 0x2f;
	s_Map['{']._shift = KBD_LSHIFT_BIT;		s_Map['{']._code = 0x2f;
	s_Map[']']._shift = 0;					s_Map[']']._code = 0x30;
	s_Map['}']._shift = KBD_LSHIFT_BIT;		s_Map['}']._code = 0x30;
	s_Map['\\']._shift = 0;					s_Map['\\']._code = 0x31;
	s_Map['|']._shift = KBD_LSHIFT_BIT;		s_Map['|']._code = 0x31;
	//INT 2 : 0x32
	s_Map[';']._shift = 0;					s_Map[';']._code = 0x33;
	s_Map[':']._shift = KBD_LSHIFT_BIT;		s_Map[':']._code = 0x33;
	s_Map['\'']._shift = 0;					s_Map['\'']._code = 0x34;
	s_Map['"']._shift = KBD_LSHIFT_BIT;		s_Map['"']._code = 0x34;
	s_Map['`']._shift = 0;					s_Map['`']._code = 0x35;
	s_Map['~']._shift = KBD_LSHIFT_BIT;		s_Map['~']._code = 0x35;
	s_Map[',']._shift = 0;					s_Map[',']._code = 0x36;
	s_Map['<']._shift = KBD_LSHIFT_BIT;		s_Map['<']._code = 0x36;
	s_Map['.']._shift = 0;					s_Map['.']._code = 0x37;
	s_Map['>']._shift = KBD_LSHIFT_BIT;		s_Map['>']._code = 0x37;
	s_Map['/']._shift = 0;					s_Map['/']._code = 0x38;
	s_Map['?']._shift = KBD_LSHIFT_BIT;		s_Map['?']._code = 0x38;
	return;
}

BOOL Translate(char c, BYTE* shift, BYTE* code)
{
	if (s_Map[c]._code != (BYTE)-1)
	{
		*shift = s_Map[c]._shift;
		*code = s_Map[c]._code;
		return TRUE;
	}
	return FALSE;
}
