
#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <sstream>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>

class Socket {
protected:
	int Csocket;
	std::string port;
	std::string host;
	int result;
	int timeout;
	unsigned int bufferSize;
	unsigned int buffer;
	unsigned int type;
	unsigned int family;
	unsigned int protocol;
public:
	Connect();
	Connect(std::string h, std::string p);
	~Connect();
	bool isOpen();
	void finishedSending();
	void sendMSG(std::string msg);
	void setBufferSize(unsigned int n);
	void setTimeout(int tm);
	void setPort(std::string p);
	void setHost(std::string h);
	void setTCP();
	void setUDP();
	void closeConnection();
	void allocateBuff();
	void deleteBuff();
	int getResult();
	std::string getPort();
	std::string getHost();
};

#endif

class Socket{
    int socket, port;
public:
    Socket();
    Socket(int p);
    ~Socket();
}

Socket::Socket(){

}

Socket::Socket(int p){

}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     socket = socket(AF_INET, SOCK_STREAM, 0);
     if (socket < 0) 
        throw std::runtime_error("Failed To Open Socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     
     if (bind(socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        throw std::runtime_error("Failed Server Binding");
     
     listen(socket, 5);
     
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     
     if (n < 0) error("ERROR writing to socket");
     
     close(newsockfd);
     close(sockfd);
     
     return 0; 
}