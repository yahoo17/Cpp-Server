
/*
	Socket�ͻ��˴���
	������127.0.0.1ͨ�ţ��˿�8888
	�������Ӻ󣬷��͸�����������Ҫ������ļ�·��
	���ļ�·�����ڣ����շ��������͵��ļ��������������ر����ӡ�
*/

#include <tchar.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>  
#include <string.h>  
#include <Ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#define BUFFER_SIZE 2048
#define FILE_NAME_MAX_SIZE 512 

int main(int argc, char* argv[])
{
	/*
	��ʼ��WSA��ʹ�ó�����Ե���windows socket
	*/
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	/*
	�����׽��֣�server_socket
	������Ƿ񴴽��ɹ�
	*/
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); ;
	if (client_socket == INVALID_SOCKET) {
		//���������socket��Ч�����������
		perror("socket error !");
		return 0;
	}

	/*
	������ַ�ṹ��server_addr�������ö˿ں�IP
	*/
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//Ҫ���ӵķ������˿ں� 8888
	server_addr.sin_port = htons(8888);
	//ָ���������ĵ�ַ127.0.0.1
	InetPton(AF_INET, "127.0.0.1",&server_addr.sin_addr.s_addr);
	
	//���ַserver_addr��������
	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)))
	{
		perror("connect error !\n");
		return 0;
	}

	char REMOTE_file_name[FILE_NAME_MAX_SIZE + 1];
	memset(REMOTE_file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("������Ҫ��ȡ�ķ������ļ�·��:\n");
	scanf_s("%s", REMOTE_file_name, FILE_NAME_MAX_SIZE);

	char LOCAL_file_name[FILE_NAME_MAX_SIZE + 1];
	memset(LOCAL_file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("�����뱣���ļ��ı���·��:\n");
	scanf_s("%s", LOCAL_file_name, FILE_NAME_MAX_SIZE);

	char buffer[BUFFER_SIZE];
	memset(buffer,0 , BUFFER_SIZE);
	strncpy_s(buffer, REMOTE_file_name, strlen(REMOTE_file_name)>BUFFER_SIZE ? BUFFER_SIZE : strlen(REMOTE_file_name));

	// �����������buffer�е����� 
	if (send(client_socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		perror("�����ļ���ʧ��:");
		exit(1);
	}

	// ���ļ���׼��д�� 
	FILE *fp;
	errno_t F_ERR = fopen_s(&fp, LOCAL_file_name, "wb");
	if (F_ERR != 0)
	{
		printf("�ļ���ʧ�ܣ�%s\n", LOCAL_file_name);
		exit(1);
	}

	// �ӷ������������ݵ�buffer�� 
	// ÿ����һ�����ݣ��㽫��д���ļ��У�ѭ��ֱ���ļ������겢д��Ϊֹ 
	memset(buffer,0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		if (fwrite(buffer, sizeof(char), length, fp) < length)
		{
			printf("�ļ�д��ʧ�ܣ�%s\n", LOCAL_file_name);
			break;
		}
		memset(buffer, 0, BUFFER_SIZE);
	}

	printf("\n�ɹ��ӷ����������ļ�\n���뱾��Ŀ¼��%s\n", REMOTE_file_name, LOCAL_file_name);


	// ���ճɹ��󣬹ر��ļ����ر�socket��WSA 
	fclose(fp);
	closesocket(client_socket);
	WSACleanup();
	system("pause");
	return 0;

}