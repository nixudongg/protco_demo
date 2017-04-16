// protoc_demo.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include "monitor_message.pb.h"
#include "google\protobuf\repeated_field.h"

using namespace google::protobuf;
void testRepeatedMessage()
{
	printf("==================This is repeated message.================\n");
	SyncFileInfoList retrieveResp;
	CommandHead* head = retrieveResp.mutable_head();
	head->set_cmdid(0x13);
	head->set_cmdlength(0x21);

	SyncFileInfo* buddyInfo = retrieveResp.add_synclist();
	buddyInfo->set_filename("main.cpp");
	buddyInfo->set_targetpath("/SCADA/bin");
	
	buddyInfo = retrieveResp.add_synclist();
	buddyInfo->set_filename("test.cpp");
	buddyInfo->set_targetpath("/home");
	

	int length = retrieveResp.ByteSize();
	char* buf = new char[length + 4];
	std::string stdHead = "[01]";
	memcpy(buf, stdHead.c_str(), stdHead.length());
	retrieveResp.SerializeToArray(buf+4, length);

	stdHead = "";
	stdHead.append(buf, 4);

	SyncFileInfoList retrieveResp2;
	retrieveResp2.ParseFromArray(buf + 4, length);
	printf("head.cmdId = %d head.cmdLength = %d\n", retrieveResp2.head().cmdid()
	, retrieveResp2.head().cmdlength());
	printf("Repeated Size = %d\n", retrieveResp2.synclist_size());
	//这里仅提供了通过容器迭代器的方式遍历数组元素的测试代码。
	//事实上，通过buddiesinfo_size和buddiesinfo函数亦可循环遍历。
	RepeatedPtrField<SyncFileInfo>* buddiesInfo = retrieveResp2.mutable_synclist();
	RepeatedPtrField<SyncFileInfo>::iterator it = buddiesInfo->begin();
	for (; it != buddiesInfo->end(); ++it) {
		printf("filename = %s\n", it->filename().c_str());
		printf("path = %s\n", it->targetpath().c_str());
	}
	delete[] buf;
}

int main()
{
	testRepeatedMessage();
	getchar();
    return 0;
}

