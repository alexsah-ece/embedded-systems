#include "client.h"

void *client(void *param){
    int sock = 0;
    struct sockaddr_in serv_addr; 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
	int j=0;
	while (j<ip_count){
		printf("CLIENT:\tTrying new connection..\n");
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{ 
			printf("CLIENT:\tSocket creation error \n"); 
			pthread_exit(NULL); 
		} 
		// Convert IPv4 and IPv6 addresses from text to binary form 
		if(inet_pton(AF_INET, IPs[j], &serv_addr.sin_addr)<=0)  
		{ 
			printf("CLIENT:\tInvalid address/ Address not supported \n"); 
		} 
		
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			printf("CLIENT:\tConnection Failed with IP %s\n", IPs[j]);
		} 
		else {
			sendMsgs(sock, j);			

		}
		close(sock);
		j++;
		if (j == ip_count){
			j = 0;
			sleep(10);
		}
	}
	
	return NULL;
}

void sendMsgs(int sock, int receiver){
	int last_msg_sent_index = IPsLastMsgSentIndex[receiver];
	char *last_msg_sent = IPsLastMsgSent[receiver];

	int endIndex = count;
	if(endIndex >= BUFFLENGTH){
		endIndex = 0;
	}
	char ack[3];

	if((last_msg_sent_index != endIndex) || (strcmp(last_msg_sent, buff[endIndex]) != 0)){
		do {
			last_msg_sent_index++;
			if(last_msg_sent_index >= BUFFLENGTH){
				last_msg_sent_index = 0;
			}

			if (send(sock, buff[last_msg_sent_index], strlen(buff[last_msg_sent_index])+1, 0) == -1){
				break;
			};
			recv(sock, ack,sizeof(ack),0);

			last_msg_sent = buff[last_msg_sent_index];
			
		} while (last_msg_sent_index != endIndex);
		send(sock, "Exit", strlen("Exit") + 1, 0);
		IPsLastMsgSentIndex[receiver] = last_msg_sent_index;
		strcpy(IPsLastMsgSent[receiver], last_msg_sent);
	} else {
		printf("CLIENT:\tNo need of new messages\n");
	}
}

void produceMsg(int sig){
	
	// avoid deadlock on alarm
	if (pthread_mutex_trylock(&buffer_mutex) != 0){
		alarm(10);
		return;
	}
	
	char message[278];
	unsigned int sender = SENDER;
		
	sprintf(
		message, 
		"%d_%d_%d_%s",
		sender, IPsToAEMs[rand() % ip_count], (unsigned)time(NULL), messageList[rand() % message_count]
	);

	if (count >= BUFFLENGTH){
		count = 0;
		fullBuffer = 1;
	}

	strcpy(buff[count++], message);
	logger(message, 0);
		
	// alarm(rand() % (5*60 + 1 - 60) + 60);
	alarm(rand() % 10 + 1);
	
	pthread_mutex_unlock(&buffer_mutex);	
}