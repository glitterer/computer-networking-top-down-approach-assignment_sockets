#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h>	//sockaddr_in 등
#include <arpa/inet.h> //htnol, sockaddr_in, write 등
#include <sys/socket.h>
void error_handling(char* msg);
int main(int argc, char* argv[])
{
	int serv_sock; // server socket
	int clnt_sock; // client socket

	struct sockaddr_in serv_addr; // server info.
	struct sockaddr_in clnt_addr; // client info. : accept함수에서 사용
	socklen_t clnt_addr_size;

	char message[] = "Hello. 20150678!"; // Server가 Client한태 보내는 "학번" 메시지

	/*TCP protocol을 사용한 ipv4 domain을 위한 소켓 생성*/
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓을 생성 _ PF_INET=IPv4 protocol, SOCK_STREAM=TCP protocol
	if (serv_sock == -1) {	// 소켓 생성 실패 == 오류 출력
		error_handling("socket() error");
	}

	/*주소 초기화 후 IP주소와 포트 지정*/
	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr 메모리 초기화
	serv_addr.sin_family = AF_INET; // 주소 체계 IPv4로 설정
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 현재 실행중인 서버 ip를 넣는다.
	serv_addr.sin_port = htons(atoi(argv[1])); // port 주소는 1번째 argument로 한다.

	/*bind함수로 serv_sock에 IP주소와 port번호를 할당한다.*/
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}

	/*listen함수로 serv_sock에 client요청을 기다린다(대기한다). 대기열은 4개 생성*/
	if (listen(serv_sock, 4) == -1) // serv_sock에 Client 요청을 4개까지 대기한다.
	{
		error_handling("listen() error");
	}

	/*cllient로부터 요청이 올 경우 수락한다.*/
	clnt_addr_size = sizeof(clnt_addr); // client의 주소 크기 만큼 할당 받는다.
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // bind()의 정보를 토대로 accept가 이루어진다.
	if (clnt_sock == -1) // accept 되지 않을 경우 오류 처리
	{
		error_handling("accept() error");
	}
	else // 연결된 경우 Client가 접속 했음을 알린다. 
	{
		printf("Client %s login.\n", inet_ntoa(clnt_addr.sin_addr)); // clnt_addr.sin_addr에 있는 주소 정보를 ipv4 주소 체계로 변경하여 출력한다.
	}

	write(clnt_sock, message, sizeof(message)); // Client 소켓쪽에 메시지를 쓴다.
	
	close(clnt_sock); // Client 소켓을 닫는다.
	close(serv_sock); // Server 소켓을 닫는다.

	return 0;
}

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
