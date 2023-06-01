
#include "stdAfx.h"
#include <aclapi.h>
#include <Shlwapi.h>

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CRegistHelp::CRegistHelp(void)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CRegistHelp::~CRegistHelp(void)
{
}
//--------------------------------------------------------------
//|
//|레지스트리  ALL 엑세스 권한 주기
//|
//---------------------------------------------------------------
int CRegistHelp::RegAccessChange(HKEY hRKey,LPCTSTR pReg)   
{   
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;   
	PSID pInteractiveSid = NULL;   
	PSID pAdministratorsSid = NULL;   
	SECURITY_DESCRIPTOR sd;   
	PACL pDacl = NULL;   
	DWORD dwAclSize;   
	HKEY hKey;   
	LONG lRetCode;   
	BOOL bSuccess = FALSE; // assume this function fails   

	//    
	// open the performance key for WRITE_DAC access   
	//    
	lRetCode = RegOpenKeyEx(   
		HKEY_LOCAL_MACHINE,   
		pReg, //TEXT("SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_0471&Pid_040f\\SMX_JCOP41V221"),   
		0,   
		WRITE_DAC,   
		&hKey );   

	if (lRetCode != ERROR_SUCCESS) {   
		fprintf(stderr, "RegOpenKeyEx error! (rc=%lu)\n", lRetCode);   
		return 1;   
	}   

	//    
	// prepare a Sid representing any Interactively logged-on user   
	//    
	if (!AllocateAndInitializeSid(   
		&sia,   
		1,   
		SECURITY_INTERACTIVE_RID,   
		0, 0, 0, 0, 0, 0, 0,   
		&pInteractiveSid ))   
		goto cleanup;   

	//    
	// prepare a Sid representing the well-known admin group   
	//    
	if (!AllocateAndInitializeSid(   
		&sia,   
		2,   
		SECURITY_BUILTIN_DOMAIN_RID,   
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,   
		&pAdministratorsSid ))   
		goto cleanup;   

	//    
	// compute size of new acl   
	//    
	dwAclSize = sizeof(ACL) + 2 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD))   
		+ GetLengthSid(pInteractiveSid) + GetLengthSid(pAdministratorsSid);   

	//    
	// allocate storage for Acl   
	//    
	pDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);   

	if (pDacl == NULL)   
		goto cleanup;   

	if (!InitializeAcl(pDacl, dwAclSize, ACL_REVISION))   
		goto cleanup;   

	//    
	// grant the Interactive Sid KEY_READ access to the perf key   
	//    
	if (!AddAccessAllowedAce(   
		pDacl,   
		ACL_REVISION,   
		KEY_ALL_ACCESS,   
		pInteractiveSid ))   
		goto cleanup;   

	//    
	// grant the Administrators Sid GENERIC_ALL access to the perf key   
	//    
	if (!AddAccessAllowedAce(   
		pDacl,   
		ACL_REVISION,   
		KEY_ALL_ACCESS,   
		pAdministratorsSid ))   
		goto cleanup;   

	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))   
		goto cleanup;   

	if (!SetSecurityDescriptorDacl(&sd, TRUE, pDacl, FALSE))   
		goto cleanup;   

	//    
	// apply the security descriptor to the registry key   
	//    
	lRetCode = RegSetKeySecurity(   
		hKey,   
		(SECURITY_INFORMATION) DACL_SECURITY_INFORMATION,   
		&sd );   

	if (lRetCode != ERROR_SUCCESS)   
		goto cleanup;   

	bSuccess = TRUE; // indicate success   

cleanup:   

	RegCloseKey(hKey);   
	RegCloseKey(HKEY_LOCAL_MACHINE);   

	//    
	// free allocated resources   
	//    
	if (pDacl != NULL)   
		HeapFree(GetProcessHeap(), 0, pDacl);   

	if (pInteractiveSid != NULL)   
		FreeSid(pInteractiveSid);   

	if (pAdministratorsSid != NULL)   
		FreeSid(pAdministratorsSid);   

	if (!bSuccess)   
		return 1;   
	return 0;   
}  
//--------------------------------------------------------------
//|
//|레지스트리  검색
//|
//---------------------------------------------------------------
#define  MAX_REGDATA 4096
#define  MAX_REGDATABIG 65536
BOOL CRegistHelp::FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue)
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
		// 값중에서 찾고자하는 값과 일치하는 것을 리스트에 추가합니다.
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
//|레지스트리 Key의 값들을 가지고 온다
//|
//---------------------------------------------------------------
size_t CRegistHelp::GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString)
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
		// 값중에서 찾고자하는 값과 일치하는 것을 리스트에 추가합니다.
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
size_t CRegistHelp::GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString)
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
		// 값중에서 찾고자하는 값과 일치하는 것을 리스트에 추가합니다.
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
//| 서브키 탐색 함수
//|
//---------------------------------------------------------------
BOOL CRegistHelp::FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey)
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
		// 서브 키를 찾아서 FindRegKeyEnum함수를 재귀호출합니다.
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
//| 서브키 String값을 가지고 온다
//|
//---------------------------------------------------------------
size_t CRegistHelp::GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString)
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
//| 삭제 함수
//|
//---------------------------------------------------------------
void CRegistHelp::DeleteRegKey(HKEY hRKey,TCHAR *pRegName)
{
	char svSubKeyBuf[MAX_PATH+1];
	RegDeleteKeyRecurse(hRKey, pRegName, svSubKeyBuf);
}

int CRegistHelp::RegDeleteKeyRecurse(HKEY hRKey, LPCTSTR pRegName, char *svKeyBuf)
{

	char svSubKeyBuf[MAX_PATH+1];
	TCHAR    lpSubKey[MAX_PATH];
	TCHAR    lpBuffer[MAX_REGDATA];
	DWORD    i;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;
	FILETIME FileTime;
	RegAccessChange(hRKey,pRegName);
	if (RegOpenKeyEx(hRKey,pRegName,0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) 
	{
		// 서브 키를 찾아서 FindRegKeyEnum함수를 재귀호출합니다.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{	
			Size=MAX_PATH;
			Result=RegEnumKeyEx(key,i,lpSubKey,&Size,NULL,NULL,NULL,&FileTime);
			if(Result==ERROR_SUCCESS) 
			{
				wsprintf(lpBuffer,TEXT("%s\\%s"),pRegName,lpSubKey);
				RegDeleteKeyRecurse(hRKey, lpBuffer, svSubKeyBuf);
			}
		}
		RegCloseKey(key);
		SHDeleteKey(hRKey,pRegName);
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|프로그램 실행시 자동 실행할수 있도록 레지스트리 등록한다
//|
//---------------------------------------------------------------
void CRegistHelp::CreateRegRun(LPCTSTR regValueName, const char* pBuffer)
{
	HKEY hkey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,&hkey);
	RegSetValueEx(hkey, regValueName, 0, REG_SZ, (const unsigned char*)pBuffer, (DWORD)strlen(pBuffer));
	RegCloseKey(hkey);
}

//--------------------------------------------------------------
//|
//|레지스트리 저장소를 만들거나 연다
//|
//---------------------------------------------------------------
HKEY CRegistHelp::CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt)
{

	HKEY key;
	DWORD dwDisp;
	DWORD sam;
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
	if(KeyOp)
		RegCreateKeyEx( KeyOp, subkey, 0, NULL,	sam, KEY_WRITE, NULL,	&key, &dwDisp );
	return key;
}
//--------------------------------------------------------------
//|
//|열린 리지스트리에 쓴다
//|
//---------------------------------------------------------------
void CRegistHelp::SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose)
{
	if(Key)
	{
		RegSetValueEx(Key, valuename, 0, REG_SZ, (LPBYTE)lpData, (DWORD)strlen(valuename)+1);
	}
	if(keyclose) RegCloseKey(Key);
	return;
}
//--------------------------------------------------------------
//|
//|위 두함수를 합친함수 이다 사용방법은 잘...
//|
//---------------------------------------------------------------
HKEY CRegistHelp::CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose, HKEY KeyOp, LPCTSTR subkey,BYTE opt)
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
		RegSetValueEx(createkey, valuename, 0, REG_SZ, (LPBYTE)lpData, (DWORD)strlen(valuename)+1);
	}
	if(keyclose){	RegCloseKey(createkey); createkey=NULL;}
	Key=createkey;
	return createkey;
}
//--------------------------------------------------------------
//|
//|레지스트리에 등록된 스트링값을 가지고 온다
//|끄
//---------------------------------------------------------------
CString CRegistHelp::GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename)
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
void CRegistHelp::SetRegMultiSmartCard()
{
	HKEY hKey = NULL;
	LONG Result;
	LONG Result2;
	BYTE  key_value[]={0x01};
	Result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\UsbFlags", 0, KEY_ALL_ACCESS,&hKey);     
	if (Result == ERROR_SUCCESS)
		Result2 = RegSetValueEx(hKey, "IgnoreHWSerNum0471040f",0,REG_BINARY, (BYTE*)&key_value, sizeof(key_value));  
	RegCloseKey(hKey);
}