#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define __UTILS_IMPL__
#include "utils.h"


// TODO: IMPLEMENT ALL THE NETWORK INFO FILE READINGS 
typedef struct LightString_s {
	uint32_t StringStride = 0;
	char* StringPointer = nullptr;
}LightString_t;

typedef struct ExternalNetworkPC_s {
	uint8_t IndexPC;
	LightString_t NamePC;
	LightString_t NameOS;
	LightString_t PCAddressMAC;
	LightString_t PCAddressIP;
}ExternalNetworkPC_t;

typedef struct LocalNetworkProtocol_s {
	uint8_t IndexProtocol;
	LightString_t ProtocolName;
	LightString_t ProtocolIP;
}LocalNetworkProtocol_t;

enum NetWorkConfiStatus
{
	NetWorkStatusOK = 0,
	NetWorkStatusFileNotOpened = 1,
	NetWorkStatusNoExtNetWork = 2,
	NetWorkStatusNoLocalNetWork = 3,
	NetWorkStatusNoModdelsNames = 4,
};

class NetWorkInfo {
public:
	NetWorkInfo(const char* NetWorkConfigFilePath);
	uint8_t GetObjectStatus(void);
	void CheckFileContent(void);
	~NetWorkInfo();
private:
	char* NetWorkInfoFilePath = nullptr;
	std::vector<std::string> NetWorkInfoFileContnent;

	uint8_t NrExternalPCS = 0;
	uint8_t NrLocalProtocols = 0;
	uint8_t NetWorkConfigStatus = NetWorkStatusOK;

	void ReadNetWorkInfoFile(void);
	void EraseComenst(void);

		
};

#ifdef __NetWorkInfo_IMPL__
NetWorkInfo::NetWorkInfo(const char * NetWorkConfigFilePath) {

	//save used config path
	this->NetWorkInfoFilePath = (char*)NetWorkConfigFilePath;

	// read file
	this->ReadNetWorkInfoFile();

	// edit file content 
	this->EraseComenst();
}

NetWorkInfo::~NetWorkInfo() {
	return;
}

uint8_t NetWorkInfo::GetObjectStatus(void) {
	return this->NetWorkConfigStatus;
}

void NetWorkInfo::CheckFileContent(void) {
	for (uint32_t LineIndex = 0; LineIndex < this->NetWorkInfoFileContnent.size(); LineIndex++)
	{
		std::cout << this->NetWorkInfoFileContnent[LineIndex] << std::endl;
	}
	return;
}

void NetWorkInfo::ReadNetWorkInfoFile(void) {
	// Read file line by line and saving into the local string vector
	// skip if the line is a coment line
	std::string TmpString;
	std::fstream FileHandler(this->NetWorkInfoFilePath);
	if (FileHandler.is_open() != true)
	{
		this->NetWorkConfigStatus = NetWorkStatusFileNotOpened;
		return;
	}

	while (getline(FileHandler, TmpString))
	{
		if (TmpString[0] == '#')
			continue;
		this->NetWorkInfoFileContnent.push_back(
			TmpString
		);
	}
	FileHandler.close();

	return;
}

void NetWorkInfo::EraseComenst(void) {
	for (uint32_t LineIndex = 0; LineIndex < this->NetWorkInfoFileContnent.size(); LineIndex++)
	{
		std::vector<std::string> TmpLine = StringSplit(this->NetWorkInfoFileContnent[LineIndex], "#");
		if (TmpLine.size() > 1)
		{
			this->NetWorkInfoFileContnent[LineIndex] = TrimEndString(TmpLine[0]);
		}
	}
	return;
}
#endif