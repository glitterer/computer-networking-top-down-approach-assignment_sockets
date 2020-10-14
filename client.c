#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* msg);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr; // 서버 정보를 담는 구조체
	char message[50];

	/*TCP protocol을 사용한 ipv4 domain을 위한 소켓 생성*/
	sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓을 생성 _ PF_INET=IPv4 protocol, SOCK_STREAM=TCP protocol
	if (sock == -1) {	// 소켓 생성 실패 == 오류 출력
		error_handling("socket() error");
	}

	/*주소 초기화 후 IP주소와 포트 지정*/
	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr 메모리 초기화
	serv_addr.sin_family = AF_INET; // 주소 체계 IPv4로 설정
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 32비트 ip 주소 값 저장
	serv_addr.sin_port = htons(atoi(argv[2])); // port 주소를 2번째 arg에 저장

	/*Client socket에 서버 연결*/
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) // 연결을 시도한다. 성공시 0, 실패시 -1
	{
		error_handling("connect() error!");
	}

	/*연결이 성공적일 경우 데이터 받기*/
	int addr_len = read(sock, message, sizeof(message) - 1);
	if (addr_len == -1) {
		error_handling("read() error!");
	}
	printf("Message from server: %s\n", message); // 메시지 출력

	close(sock); // 소켓을 닫는다.
	
	return 0;
}

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
