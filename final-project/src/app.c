#include "app.h"
#include <pthread.h>


int main(int argc, char *argv[]){

	printf("%d\n", argc);
	if(argc < 3){
		printf("Not enough arguments given");
		exit(-1);
	}

	// populate IPs and messageList
	initialize_vars();
	initialize_addresses(argv[1]);
	initialize_messages(argv[2]);
	
	printf("MAIN:\tSetting up sig handlers...\n");
	signal(SIGALRM, produceMsg);
	signal(SIGTERM, catch_int_term);
	signal(SIGINT, catch_int_term);
	srand(time(NULL));

	printf("MAIN:\tIniatilizing buffer...\n");
	produceMsg(0);

	pthread_t clientThread;	
	pthread_create(&clientThread, NULL, client, NULL);
	
	printf("MAIN:\tStarting server...\n");
	server();
	
	return 0;
}

void catch_int_term(int signal){
	printf("INT signal ...\n");
	logger("", 1);
	for(int i = 0; i < message_count; i++){
		free(messageList[i]);
	}
	free(messageList);

	for(int i = 0; i < ip_count; i++){
		free(IPs[i]);
		// free(IPsLastMsgSentIndex[i]);
	}
	free(IPs);
	free(IPsToAEMs);
	free(IPsLastMsgSent);
	free(IPsLastMsgSentIndex);
	exit(1);
}


void initialize_vars(){
	count=0;
	fullBuffer=0;
	curr_log_count = 0;
	pthread_mutex_init(&buffer_mutex, NULL);
	pthread_mutex_init(&fd_mutex, NULL);
}

void initialize_addresses(char *file){
	FILE *fp;
	int i;

	fp = fopen(file, "r");
	
	fscanf(fp, "%d\n", &ip_count);
	IPsLastMsgSentIndex = (int *)malloc(ip_count * sizeof(int));
	IPsToAEMs =(int *)malloc(ip_count * sizeof(int));  

	IPs = (char **)malloc(ip_count * sizeof(char *)); 
	IPsLastMsgSent = (char **)malloc(ip_count * sizeof(char *));

    for (i = 0; i < ip_count; i++){
		IPsLastMsgSentIndex[i] = -1;
        IPsLastMsgSent[i] = (char *)malloc(MAX_MSG_LENGTH * sizeof(char));
        strcpy(IPsLastMsgSent[i], "null");
		IPs[i] = (char *)malloc(50 * sizeof(char));
		fscanf(fp, "%s %d\n", IPs[i], &IPsToAEMs[i]);
		IPs[i][strcspn(IPs[i], "\n")] = '\0';

	} 
	fclose(fp);
}

void initialize_messages(char *file){
	FILE *fp;
	int i;

	fp = fopen(file, "r");
	
	fscanf(fp, "%d\n", &message_count);
	messageList = (char **)malloc(message_count * sizeof(char *)); 

    for (i = 0; i < message_count; i++){
        messageList[i] = (char *)malloc(50 * sizeof(char));
		fgets(messageList[i], 50, fp);
		messageList[i][strcspn(messageList[i], "\n")] = '\0';

	} 
	fclose(fp);
	for(int i = 0 ; i < message_count ; i++){
		printf("%s\n", messageList[i]);
	}
}

void logger(char *receiveMsg, int flush_remaining){

	FILE *fp;
	int i = 0, end = 0;
	strcpy(log_buffer[curr_log_count++], receiveMsg);
	// determine if logging to file will take place
	if (curr_log_count == LOG_BATCH_SIZE || flush_remaining){
		printf("LOGGER:\tLogging into file...\n");
		end = curr_log_count;
		curr_log_count = 0;
		// write to file
		fp = fopen("stats.log", "a+");
		for (int i = 0; i < end; i++){
			fprintf(fp, "%s\n", log_buffer[i]);
		}
		fclose(fp);
	}
	return;
}