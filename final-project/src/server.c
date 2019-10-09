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
    address.sin_port = htons(SERVER_LISTEN_PORT); 
	
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
		pthread_t received_msg_thread;
		pthread_create(&received_msg_thread, NULL, received_msg, &newfd);

	}
	
}

void *received_msg(void *newfd){
	int *temp = (int *) newfd;
	int sock = *temp;

	printf("SERVER:\tWaiting for message\n");
	pthread_mutex_unlock(&fd_mutex);

	char msg[MAX_MSG_LENGTH];

	recv(sock, msg, sizeof(msg), 0);
	printf("SERVER:\tProcessing message:%s\n", msg);

	int i,doubleMsg,endIndex;
	while (strcmp(msg,"Exit")!=0){
		doubleMsg=0;
		if (count>=BUFFLENGTH){
			count=0;
			is_buffer_full=1;
		}
		
		if (is_buffer_full==0) {
			endIndex=count;
		}
		else {
			endIndex=BUFFLENGTH;
		}
		
		for (i=0; i<endIndex; i++){
			if (strcmp(msg, buff[i])==0){
				doubleMsg=1;
				break;
			}
		}
		
		if (doubleMsg==0){
			pthread_mutex_lock(&buffer_mutex);
			strcpy(buff[count], msg);
			logger(msg, 0);
			count++;
			pthread_mutex_unlock(&buffer_mutex);
		}
		send(sock,"OK",strlen("OK")+1,0);
		recv(sock, msg, sizeof(msg), 0);
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