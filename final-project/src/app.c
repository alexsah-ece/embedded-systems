#include "app.h"
#include <pthread.h>


int main(int argc, char *argv[]){

	printf("%d\n", argc);
	if(argc < 3){
		printf("Not enough arguments given");
		exit(-1);
	}

	// populate ip_addresses and message_list
	initialize_vars();
	initialize_addresses(argv[1]);
	initialize_messages(argv[2]);
	
	printf("MAIN:\tSetting up sig handlers...\n");
	signal(SIGALRM, produce_msg);
	signal(SIGTERM, int_term_handler);
	signal(SIGINT, int_term_handler);
	srand(time(NULL));

	printf("MAIN:\tIniatilizing buffer...\n");
	produce_msg(0);

	pthread_t clientThread;	
	pthread_create(&clientThread, NULL, client, NULL);
	
	printf("MAIN:\tStarting server...\n");
	server();
	
	return 0;
}

void int_term_handler(int signal){
	printf("INT signal ...\n");
	logger("", 1);
	for(int i = 0; i < message_count; i++){
		free(message_list[i]);
	}
	free(message_list);

	for(int i = 0; i < ip_count; i++){
		free(ip_addresses[i]);
		// free(last_msg_sent_index[i]);
	}
	free(ip_addresses);
	free(ip_to_aem);
	free(last_msg_sent);
	free(last_msg_sent_index);
	exit(1);
}


void initialize_vars(){
	count=0;
	is_buffer_full=0;
	curr_log_count = 0;
	pthread_mutex_init(&buffer_mutex, NULL);
	pthread_mutex_init(&fd_mutex, NULL);
}

void initialize_addresses(char *file){
	FILE *fp;
	int i;

	fp = fopen(file, "r");
	
	fscanf(fp, "%d\n", &ip_count);
	last_msg_sent_index = (int *)malloc(ip_count * sizeof(int));
	ip_to_aem =(int *)malloc(ip_count * sizeof(int));  

	ip_addresses = (char **)malloc(ip_count * sizeof(char *)); 
	last_msg_sent = (char **)malloc(ip_count * sizeof(char *));

    for (i = 0; i < ip_count; i++){
		last_msg_sent_index[i] = -1;
        last_msg_sent[i] = (char *)malloc(MAX_MSG_LENGTH * sizeof(char));
        strcpy(last_msg_sent[i], "null");
		ip_addresses[i] = (char *)malloc(50 * sizeof(char));
		fscanf(fp, "%s %d\n", ip_addresses[i], &ip_to_aem[i]);
		ip_addresses[i][strcspn(ip_addresses[i], "\n")] = '\0';

	} 
	fclose(fp);
}

void initialize_messages(char *file){
	FILE *fp;
	int i;

	fp = fopen(file, "r");
	
	fscanf(fp, "%d\n", &message_count);
	message_list = (char **)malloc(message_count * sizeof(char *)); 

    for (i = 0; i < message_count; i++){
        message_list[i] = (char *)malloc(50 * sizeof(char));
		fgets(message_list[i], 50, fp);
		message_list[i][strcspn(message_list[i], "\n")] = '\0';

	} 
	fclose(fp);
	for(int i = 0 ; i < message_count ; i++){
		printf("%s\n", message_list[i]);
	}
}

void logger(char *received_msg, int flush_remaining){

	FILE *fp;
	int i = 0, end = 0;
	strcpy(log_buffer[curr_log_count++], received_msg);
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