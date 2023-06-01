
#include "stdAfx.h"
#include "HCRegister.h"
#include <Shlwapi.h>

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
HCRegister::HCRegister(void)
{

}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
HCRegister::~HCRegister(void)
{
}
//--------------------------------------------------------------
//|
//|������Ʈ��  �˻�
//|
//---------------------------------------------------------------
#define  MAX_REGDATA 4096
#define  MAX_REGDATABIG 65536
BOOL HCRegister::FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue)
{
	TCHAR    lpValue[MAX_REGDATA];
	TCHAR*    lpOutput=NULL;
	TCHAR    lpTmpData[10];
	DWORD    i, k;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;

	DWORD    Type;
	BYTE*     Data=NULL;
	TCHAR pBackGegName[MAX_REGDATA];
	ZeroMemory(pBackGegName,MAX_REGDATA);
	_tcscpy_s(pBackGegName, MAX_REGDATA, pRegName);
	if (RegOpenKeyEx(hRKey,pBackGegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		// ���߿��� ã�����ϴ� ���� ��ġ�ϴ� ���� ����Ʈ�� �߰��մϴ�.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{
			Size=MAX_REGDATA;
			Result=RegEnumValue(key,i,lpValue,&Size,NULL,NULL,NULL,NULL);
			if (_tcsicmp(pFindKeyValue, lpValue)==0)
			{
				if (Result==ERROR_SUCCESS)
				{
					Size=MAX_REGDATABIG;
					Data = new BYTE[Size+1];
					ZeroMemory(Data, (Size+1)*sizeof(BYTE));
					RegQueryValueEx(key,lpValue,NULL,&Type,Data,&Size);
					lpOutput = new TCHAR[(Size*3)+1];
					ZeroMemory(lpOutput, ((Size*3)+1)*sizeof(TCHAR));
					switch (Type) 
					{
					case REG_DWORD:
						wsprintf(lpOutput,TEXT("%d"),*(LPDWORD)Data);
						break;
					case REG_SZ:
						wsprintf(lpOutput,TEXT("%s"),Data);
						break;
					case REG_BINARY:
						lpOutput[0]=0;
						for(k=0; k < Size; k++)
						{
							if (k > 0) _tcscat_s(lpOutput,((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
							wsprintf(lpTmpData,TEXT("%002X"),Data[k]);
							_tcscat_s(lpOutput,((Size*3)+1)*sizeof(TCHAR), lpTmpData);
						}
						break;
					default:
						wsprintf(lpOutput,TEXT("[%d]"),Type);
						lpOutput[0]=0;
						for(k=0; k < Size; k++)
						{
							if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
							wsprintf(lpTmpData,TEXT("%02X"),Data[k]);
							_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
						}
						break;
					}
					if(Data)
					{
						delete Data; Data = NULL;
					}
					if(lpOutput)
					{
						delete lpOutput; lpOutput=NULL;
					}
				}
			} 
		} 
		RegCloseKey(key);
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|������Ʈ�� Key�� ������ ������ �´�
//|
//---------------------------------------------------------------
size_t HCRegister::GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString)
{
	TCHAR    lpValue[MAX_REGDATA];
	TCHAR*    lpOutput=NULL;
	TCHAR    lpTmpData[10];
	DWORD    i, k;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;

	DWORD    Type;
	BYTE*     Data=NULL;
	TCHAR pBackGegName[MAX_REGDATA];
	ZeroMemory(pBackGegName,MAX_REGDATA);
	_tcscpy_s(pBackGegName, MAX_REGDATA, pRegName);
	if (RegOpenKeyEx(hRKey,pBackGegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		// ���߿��� ã�����ϴ� ���� ��ġ�ϴ� ���� ����Ʈ�� �߰��մϴ�.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{
			Size=MAX_REGDATA;
			Result=RegEnumValue(key,i,lpValue,&Size,NULL,NULL,NULL,NULL);
			if (Result==ERROR_SUCCESS)
			{
				Size=MAX_REGDATABIG;
				Data = new BYTE[Size+1];
				ZeroMemory(Data, (Size+1)*sizeof(BYTE));
				RegQueryValueEx(key,lpValue,NULL,&Type,Data,&Size);
				lpOutput = new TCHAR[(Size*3)+1];
				ZeroMemory(lpOutput, ((Size*3)+1)*sizeof(TCHAR));
				switch (Type) 
				{
				case REG_DWORD:
					wsprintf(lpOutput,TEXT("%d"),*(LPDWORD)Data);
					break;
				case REG_EXPAND_SZ:
				case REG_SZ:
					wsprintf(lpOutput,TEXT("%s"),Data);
					break;
				case REG_BINARY:
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%002X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				default:
					wsprintf(lpOutput,TEXT("[%d]"),Type);
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%02X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				}
				ValueString[lpValue]=lpOutput;
				if(Data)
				{
					delete Data; Data = NULL;
				}
				if(lpOutput)
				{
					delete lpOutput; lpOutput=NULL;
				}
			}
		} 
		RegCloseKey(key);
	}
	return ValueString.GetCount();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
size_t HCRegister::GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString)
{
	TCHAR    lpValue[MAX_REGDATA];
	TCHAR*    lpOutput=NULL;
	TCHAR    lpTmpData[10];
	DWORD    i, k;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;

	DWORD    Type;
	BYTE*     Data=NULL;
	TCHAR pBackGegName[MAX_REGDATA];
	ZeroMemory(pBackGegName,MAX_REGDATA);
	_tcscpy_s(pBackGegName, MAX_REGDATA, pRegName);
	if (RegOpenKeyEx(hRKey,pBackGegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		// ���߿��� ã�����ϴ� ���� ��ġ�ϴ� ���� ����Ʈ�� �߰��մϴ�.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{
			Size=MAX_REGDATA;
			Result=RegEnumValue(key,i,lpValue,&Size,NULL,NULL,NULL,NULL);
			if (Result==ERROR_SUCCESS)
			{
				Size=MAX_REGDATABIG;
				Data = new BYTE[Size+1];
				ZeroMemory(Data, (Size+1)*sizeof(BYTE));
				RegQueryValueEx(key,lpValue,NULL,&Type,Data,&Size);
				lpOutput = new TCHAR[(Size*3)+1];
				ZeroMemory(lpOutput, ((Size*3)+1)*sizeof(TCHAR));
				switch (Type) 
				{
				case REG_DWORD:
					wsprintf(lpOutput,TEXT("%d"),*(LPDWORD)Data);
					break;
				case REG_EXPAND_SZ:
				case REG_SZ:
					wsprintf(lpOutput,TEXT("%s"),Data);
					break;
				case REG_BINARY:
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%002X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				default:
					wsprintf(lpOutput,TEXT("[%d]"),Type);
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%02X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				}
				valueString.push_back(lpOutput);
				if(Data)
				{
					delete Data; Data = NULL;
				}
				if(lpOutput)
				{
					delete lpOutput; lpOutput=NULL;
				}
			}
		} 
		RegCloseKey(key);
	}
	return valueString.size();
}
//--------------------------------------------------------------
//|
//| ����Ű Ž�� �Լ�
//|
//---------------------------------------------------------------
BOOL HCRegister::FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey)
{
	BOOL bFind=FALSE;
	TCHAR    lpSubKey[MAX_PATH];
	TCHAR    lpBuffer[MAX_REGDATA];
	DWORD    i;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;
	FILETIME FileTime;
	TCHAR pBackGegName[MAX_REGDATA];
	ZeroMemory(pBackGegName,MAX_REGDATA);
	_tcscpy_s(pBackGegName, MAX_REGDATA, pRegName);
	if (RegOpenKeyEx(hRKey,pBackGegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		// ���� Ű�� ã�Ƽ� FindRegKeyEnum�Լ��� ���ȣ���մϴ�.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{	
			Size=MAX_PATH;
			Result=RegEnumKeyEx(key,i,lpSubKey,&Size,NULL,NULL,NULL,&FileTime);

			if(_tcscmp(pFindSubKey,lpSubKey)==0)
			{
				bFind = TRUE;
			}
			if(Result==ERROR_SUCCESS&&bFind==FALSE) 
			{
				wsprintf(lpBuffer,TEXT("%s\\%s"),pBackGegName,lpSubKey);
				bFind = FindSubKeyEnum(hRKey, lpBuffer, pFindSubKey);
			}
		}
		RegCloseKey(key);
	}
	return bFind;
}
//--------------------------------------------------------------
//|
//| ����Ű String���� ������ �´�
//|
//---------------------------------------------------------------
size_t HCRegister::GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString)
{
	TCHAR    lpSubKey[MAX_PATH];
	DWORD    i;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;
	FILETIME FileTime;
	if (RegOpenKeyEx(hRKey,pRegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{	
			Size=MAX_PATH;
			Result=RegEnumKeyEx(key,i,lpSubKey,&Size,NULL,NULL,NULL,&FileTime);
			if(Result ==ERROR_SUCCESS)
				(*KeyString).push_back(lpSubKey);
		}
		RegCloseKey(key);
	}
	return (*KeyString).size();
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HKEY HCRegister::CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose, HKEY KeyOp, LPCTSTR subkey,BYTE opt)
{
	HKEY createkey=Key;
	DWORD dwDisp;
	DWORD sam;

	if(KeyOp)
	{
		RegCloseKey(createkey); createkey=NULL;;
		switch(opt)
		{
		case 0: sam=REG_OPTION_RESERVED;	break;
		case 1:sam=REG_OPTION_NON_VOLATILE;		break;
		case 2:sam=REG_OPTION_VOLATILE;		break;
		case 3:sam=REG_OPTION_CREATE_LINK;		break;
		case 4: sam=REG_OPTION_BACKUP_RESTORE;	break;
		case 5:sam=REG_OPTION_OPEN_LINK;		break;
		case 6:sam=REG_LEGAL_OPTION;		break;
		}
		RegCreateKeyEx( KeyOp, subkey, 0, NULL,	sam, KEY_WRITE, NULL, &createkey, &dwDisp );
	}
	if(createkey)
	{
		RegSetValueEx(createkey, valuename, 0, REG_SZ, (LPBYTE)lpData, (DWORD)lstrlen(valuename)+1);
	}
	if(keyclose){	RegCloseKey(createkey); createkey=NULL;}
	Key=createkey;
	return createkey;
}
//--------------------------------------------------------------
//|
//|������Ʈ���� ��ϵ� ��Ʈ������ ������ �´�
//|��
//---------------------------------------------------------------
CString HCRegister::GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename)
{
	TCHAR keydata[256];
	CString returnvalue;
	DWORD dwType = REG_SZ;
	DWORD dwSize=sizeof(keydata);
	HKEY hKey;
	LONG lResult = RegOpenKeyEx(LKey,subkey, 0 , KEY_READ , &hKey);
	if (lResult == ERROR_SUCCESS )
		RegQueryValueEx(hKey , valuename, NULL , &dwType, (LPBYTE)keydata, &dwSize);

	returnvalue.Format(_T("%s"), keydata);
	RegCloseKey(hKey);
	return returnvalue;
}