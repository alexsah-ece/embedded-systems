#include "client.h"

void *client(void *param){
    int sock = 0;
    struct sockaddr_in serv_addr; 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(CLIENT_SENT_PORT); 
    
	int j=0;
	while (j<ip_count){
		printf("CLIENT:\tTrying new connection..\n");
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{ 
			printf("CLIENT:\tSocket creation error \n"); 
			pthread_exit(NULL); 
		} 
		// Convert IPv4 and IPv6 addresses from text to binary form 
		if(inet_pton(AF_INET, ip_addresses[j], &serv_addr.sin_addr)<=0)  
		{ 
			printf("CLIENT:\tInvalid address/ Address not supported \n"); 
		} 
		
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			printf("CLIENT:\tConnection Failed with IP %s\n", ip_addresses[j]);
		} 
		else {
			send_messages(sock, j);			

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

void send_messages(int sock, int receiver){
	int last_msg_index = last_msg_sent_index[receiver];
	char *last_msg = last_msg_sent[receiver];

	int endIndex = count;
	if(endIndex >= BUFFLENGTH){
		endIndex = 0;
	}
	char ack[3];

	if((last_msg_index != endIndex) || (strcmp(last_msg, buff[endIndex]) != 0)){
		do {
			last_msg_index++;
			if(last_msg_index >= BUFFLENGTH){
				last_msg_index = 0;
			}

			if (send(sock, buff[last_msg_index], strlen(buff[last_msg_index])+1, 0) == -1){
				break;
			};
			recv(sock, ack,sizeof(ack),0);

			last_msg = buff[last_msg_index];
			
		} while (last_msg_index != endIndex);
		send(sock, "Exit", strlen("Exit") + 1, 0);
		last_msg_sent_index[receiver] = last_msg_index;
		strcpy(last_msg_sent[receiver], last_msg);
	} else {
		printf("CLIENT:\tNo need of new messages\n");
	}
}

void produce_msg(int sig){
	
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
		sender, ip_to_aem[rand() % ip_count], (unsigned)time(NULL), message_list[rand() % message_count]
	);

	if (count >= BUFFLENGTH){
		count = 0;
		is_buffer_full = 1;
	}

	strcpy(buff[count++], message);
	logger(message, 0);
		
	// alarm(rand() % (5*60 + 1 - 60) + 60);
	alarm(rand() % 10 + 1);
	
	pthread_mutex_unlock(&buffer_mutex);	
}