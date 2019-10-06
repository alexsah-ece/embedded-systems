#include "server.h"

void server(void){
	int sockfd,newfd;
	int opt = 1;
	int i;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("SERVER:\tsocket failed\n"); 
        exit(EXIT_FAILURE); 
    } 
	
	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
	
	// reuse address and port options
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
         &opt, sizeof(opt))) 
        { 
            perror("SERVER:\tsetsockopt\n"); 
            exit(1); 
        } 

    if (bind(sockfd, (struct sockaddr *)&address,  sizeof(address))<0) {
        perror("SERVER:\tbind failed\n"); 
        exit(EXIT_FAILURE); 
    } 
	
	if (listen(sockfd, 3) < 0) {
        perror("SERVER:\tlisten\n"); 
        exit(EXIT_FAILURE); 
    } 

	for (;;){
		printf("SERVER:\tAccepting new connections...\n");
		pthread_mutex_lock(&fd_mutex);
		if ((newfd = accept(sockfd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) {
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		printf("SERVER:\tConnection accepted...\n");
		//printf("qawdqawd\n");
		pthread_t receiveThread;
		pthread_create(&receiveThread, NULL, receiveMsg, &newfd);
		
		i++;
		if (i>=ip_count) i=0;
		
	}
	
}

void *receiveMsg(void *newfd){
	int *temp = (int *) newfd;
	int sock = *temp;

	printf("SERVER:\tWaiting for message\n");
	pthread_mutex_unlock(&fd_mutex);

	char receivedMsg[278];

	recv(sock, receivedMsg, sizeof(receivedMsg), 0);
	printf("SERVER:\tProcessing message:%s\n", receivedMsg);

	int i,doubleMsg,endIndex;
	while (strcmp(receivedMsg,"Exit")!=0){
		doubleMsg=0;
		if (count>=BUFFLENGTH){
			count=0;
			fullBuffer=1;
		}
		
		if (fullBuffer==0) {
			endIndex=count;
		}
		else {
			endIndex=BUFFLENGTH;
		}
		
		for (i=0; i<endIndex; i++){
			if (strcmp(receivedMsg, buff[i])==0){
				doubleMsg=1;
				break;
			}
		}
		
		if (doubleMsg==0){
			pthread_mutex_lock(&buffer_mutex);
			strcpy(buff[count], receivedMsg);
			logger(receivedMsg, 0);
			count++;
			pthread_mutex_unlock(&buffer_mutex);
		}
		send(sock,"OK",strlen("OK")+1,0);
		recv(sock, receivedMsg, sizeof(receivedMsg), 0);
	}
	for (int i=0; i<count;i++){
		printf("\t%s\n",buff[i]);
	}
	printf("Count:%d\n", count);
	printf("SERVER:\tEnding connection...\n");

	close(sock);
	pthread_detach(pthread_self());
	pthread_exit(NULL);
}