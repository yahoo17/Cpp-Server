
/*

	Socket�������˴���
	�����������˿�8888
	�������Ӻ󣬽���Client������ļ�·��
	���ļ�·�����ڣ����͸��ļ���Client�����������ر����ӡ�

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
	/*
	WORD sockVersion = MAKEWORD(2, 2);*/
	WSADATA wsaData;
	if (WSAStartup((2,2), &wsaData) != 0)
	{
		return 0;
	}

	/*
		�����������׽��֣�server_socket
		������TCP
		������Ƿ񴴽��ɹ�
	*/
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET) {
		//���������socket��Ч�����������
		perror("socket error !\n");
		return 0;
	}

	/*
	   ������ַ��server_addr�������ö˿ں�IP
	*/
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	//�˿ں� 8888
	server_addr.sin_port = htons(8888);
	//INADDR_ANY��ʾ��������IP��ַ
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;


	//��socket���ַserver_addr��
	if (bind(server_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		perror("bind error !\n");
		return 0;
	}

	//server_socket��ʼ����
	if (listen(server_socket, 20) == SOCKET_ERROR)
	{
		perror("listen error !\n");
		return 0;
	}
	
	while (1)
	{
		printf("�ȴ�����...\n");

		// ����ͻ��˵�socket��socket��ַ�ṹ
		SOCKET client_socket;
		sockaddr_in client_addr;
		int client_addr_length = sizeof(client_addr);

		// �����������󣬷���һ���µ�socket(������)�������socket����ͬ���ӵĿͻ���ͨ�� 
		// accept����������ӵ��Ŀͻ�����Ϣд��client_addr�� 
		client_socket = accept(server_socket, (SOCKADDR *)&client_addr, &client_addr_length);
		if (client_socket == INVALID_SOCKET)
		{
			perror("Socket���ӽ���ʧ��:\n");
			continue;
		}
		
		char IP_BUFFER[256];
		memset(IP_BUFFER, 0, 256);
		InetNtop(AF_INET, &client_addr.sin_addr, IP_BUFFER,256);
		printf("Socket���ӽ������ͻ���IPΪ��%s���˿�Ϊ��%d\n", IP_BUFFER, ntohs(client_addr.sin_port));


		//���տͻ�������ĵ��ļ�·��
		// recv�����������ݵ�������buffer�� 
		char buffer[BUFFER_SIZE];
		memset(buffer,0, BUFFER_SIZE);
		if (recv(client_socket, buffer, BUFFER_SIZE, 0) < 0)
		{
			perror("�����ļ���ʧ��:\n");
			break;
		}

		// Ȼ���buffer(������)������file_name�� 
		char file_name[FILE_NAME_MAX_SIZE + 1];
		memset(file_name, 0,FILE_NAME_MAX_SIZE + 1);
		strncpy_s(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
		

		// ���ļ�����ȡ�ļ����� 
		FILE *fp;
		errno_t F_ERR= fopen_s(&fp,file_name, "rb");
		if (F_ERR != 0)
		{
			printf("�ļ���ʧ��:%s\n", file_name);
		}
		else
		{
			printf("��ʼ�����ļ���%s\n", file_name);
			memset(buffer,0, BUFFER_SIZE);
			int length = 0;
			// ÿ��ȡһ�����ݣ��㽫�䷢�͸��ͻ��ˣ�ѭ��ֱ���ļ�����Ϊֹ 
			while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
			{
				if (send(client_socket, buffer, length, 0) < 0)
				{
					printf("�ļ�����ʧ��:%s/n", file_name);
					break;
				}
				memset(buffer,0, BUFFER_SIZE);
			}

			// �ر��ļ� 
			fclose(fp);
			printf("�ļ��������:%s!\n", file_name);
		}
		// �ر���ͻ��˵����� 
		closesocket(client_socket);
	}


	// �رռ����õ�socket 
	closesocket(server_socket);
	WSACleanup();
	return 0;

}