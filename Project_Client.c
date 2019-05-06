#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[]){
  char score[255];
  int server, port_Number, p_id, n;
  struct sockaddr_in server_Add;     // client socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  server_Add.sin_family = AF_INET;
  sscanf(argv[2], "%d", &port_Number);
  server_Add.sin_port = htons((uint16_t)port_Number);

  if(inet_pton(AF_INET, argv[1], &server_Add.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
}

 if(connect(server, (struct sockaddr *) &server_Add, sizeof(server_Add))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }

  read(server, score, 255);
  fprintf(stderr, " %s\n", score);
        int Server_total=0,Client_total=0;
		int Server_Score,Client_Score;
		long int m=0;
		int dice=6;
     while(1)
	 {		 
       /* reading server's messages */
       if(n=read(server, score, 255))
	   {
          score[n]='\0';
          fprintf(stderr,"%s\n\n", score);
	   }
	   
	   /* reading server's messages */
       if(n=read(server, score, 255))
	   {
         
		  Server_Score=atoi(score);
          fprintf(stderr,"Server Score :       %d\n", Server_Score);
		  Server_total+=Server_Score;
		  fprintf(stderr,"Server Total Score : %d\n\n", Server_total);
		  if(Server_total>=100){
			  fprintf(stderr,"Game Over: You lost the game \n");
			  exit(0);
			}          
	   }
	   
	   fprintf(stderr, "Hit enter to play the dice\n");
	   if(n=read(0, score, 255)){
         score[n]='\0';
      }
	   
            Client_Score=(int) time(&m)%6 + 1;
			sprintf(score,"%d",Client_Score);
			fprintf(stderr,"My score(client) :  %d\n", Client_Score);
			Client_total+=Client_Score;
			
			if(Client_total<100)
				write(server,score,255);
			else
			{
				write(server,score,255);
				sleep(1);
				fprintf(stderr,"My Total score(client) :  %d\n\n", Client_total);
				fprintf(stderr,"Game Over : You won the game ");
				close(server);
				exit(0);
				
			}
			fprintf(stderr,"My Total score(client) :  %d\n\n", Client_total);
	     }
		 
		 
		 n=read(0,score,255);
		 if(n=0){
			 close(server);
			 exit(0);
			 
		 }
}

