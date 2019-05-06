#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>


void serviceClient(int);

int main(int argc, char *argv[]){  
  int socket_descriptor, client, portNumber, status;
  struct sockaddr_in server_Add;      // Server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }
  if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }
  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  server_Add.sin_family = AF_INET;
  server_Add.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  server_Add.sin_port = htons((uint16_t)portNumber);
  
  bind(socket_descriptor, (struct sockaddr *) &server_Add, sizeof(server_Add));
  listen(socket_descriptor, 5);

  while(1){
    
   printf("Waiting for the player to play game..");    
    client = accept(socket_descriptor, NULL, NULL);
    printf("\n Got a Player to play, Game is started..\n");
    
    if(!fork())
      serviceClient(client);

    close(client);
    waitpid(0, &status, WNOHANG);
  }
}

void serviceClient(int socket_descriptor){
	    char score[255];
	    int n;
        int total_score_S=0,total_score_C=0;
		int Server_Score,Client_Score;
		int dice=6;
		long int ss=0;

       write(socket_descriptor, "Game is Starting soon : ", 24);

	  /* sending messages to the client  */
	   while(1)
	   {
	    if(total_score_S>=100 || total_score_C>=100)
		{
			if(total_score_S>=100)
			{
				write(socket_descriptor, "Game over: you lost the game ", 40);
				fprintf(stderr,"Game over: you won the game");
			}
			else{
				write(socket_descriptor, "Game over: you won the game", 40);
			    fprintf(stderr,"Game over: you lost the game");
			}
			close(socket_descriptor);
			exit(0);
						
		}
		else
	   { 
		sleep(1);
	    
	      write(socket_descriptor, "Game on: you can now play your dice", 48);
	      
	    }
		  sleep(1);
		  Server_Score=(int) time(&ss)%dice + 1;
		  sprintf(score,"%d", Server_Score);
		  fprintf(stderr,"My Score :       %d\n", Server_Score);
		  total_score_S+=Server_Score;
		  fprintf(stderr,"My Total Score : %d\n\n", total_score_S);
		  if(total_score_S<100)
			write(socket_descriptor,score,255);
		  else{
				write(socket_descriptor,score,255);
				fprintf(stderr,"Game over: you won the game");
				exit(0);
			}
		  sleep(1);
		  
	   if(n=read(socket_descriptor, score, 255))
	   {
         
		  Client_Score=atoi(score);
		  fprintf(stderr,"Client Score :       %d\n", Client_Score);
		  total_score_C+=Client_Score;
		  fprintf(stderr,"Client Total Score : %d\n\n", total_score_C);
          if(!strcasecmp(score, "Bye\n")){
	  
             exit(0);
           
		  }
	   }
	   }
}
