
#ifndef __WATERCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WATERCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaterCmdFormat
{
public:
	//생성자
	CWaterCmdFormat(void);
	//소멸자
	~CWaterCmdFormat(void);
	//통신을 보내는 쪽 Address
	static enum WateAddress{
		address_pc = 0x53,
		address_pcb = 0x5A,
	};
	//시리얼 통신 규약 값 정의
	static enum SeriaForamt{
		stx_format = 0x02,
		etx_format = 0x03,
		cr_format = 0x0D
	};
	//PC에서 보내는 Command 값
	static enum WaterCmd{
		watercmd_sellon = 0x4F,
		watercmd_selloff = 0x46,
		watercmd_watercheck = 0x50,
		watercmd_watererrorcheck = 0x80,
		watercmd_corkcheck = 0x90
	};
	// 50에 대한 PCB 응답값
	static enum WaterCheck{
		water_ready = 0x51,
		water_readycomplete = 0x52,
		water_run = 0x53,
		water_error = 0x54,
		water_none=0xFF
	};
	//80에 대한 PCB 응답값
	static enum WaterErrorCheck{
		sensererror_error = 0x81,
		watererror_low = 0x82,
		watererror_mid = 0x84,
		watererror_high = 0x88,
	};
	//Error 정의
	static enum WaterErrorCode{
		Any_Ok =  0x00,
		Any_Timeout = 0x01,
		Any_E_NotSerialPort = 0x02,
		Any_E_None=0xFF
	};
};

#endif //__WATERCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

