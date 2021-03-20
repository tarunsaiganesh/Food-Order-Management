#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include <sys/wait.h>
#include <signal.h>
#include "database.hpp"

#define MYPORT 1025 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 500 //max buf size
void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}

void receive_food_blk(db_blk* blk, int socket){
   blk->type = 0;
   int bytes_r = 0;
   char buf1[MAXDATASIZE];
   int cnt = 0;

   while(1){
  
    if (cnt == 4)
       break;  
    
    for(int i = 0; i < MAXDATASIZE; i++)
      buf1[i] = '\0';  

    if ((bytes_r=recv(socket, buf1, MAXDATASIZE-1, 0)) == -1){
         continue;
    }
    printf("receive0\n");
    if (bytes_r > 0){
       cnt++;
       buf1[bytes_r] = 0;
       //printf("buffer: %s %d\n", buf1, bytes_r);
       //printf("%c%c%c%c%c\n", buf1[0],buf1[1],buf1[2],buf1[3],buf1[4]);
       if(cnt == 1)
         blk->food_id = stoi(buf1);
       //printf("first: %d\n", blk->food_id);
       //printf("receive1\n");
       if(cnt == 2)
         blk->food_name = buf1;
       //printf("second: %s\n", blk->food_name.c_str());
       if(cnt == 3)
         blk->food_qty = stoi(buf1);
       //printf("third: %d\n", blk->food_qty);
       if(cnt == 4)
         blk->food_pr = stoi(buf1);
       //printf("fourth: %d\n", blk->food_pr);

    }
   }
   
   
}

void receive_user_blk(db_blk* blk, int socket){
   blk->type = 1;
   int bytes_r = 0;
   char buf[MAXDATASIZE];
   int cnt = 0;
   while(1){
  
    if (cnt == 2)
       break;  

    for(int i = 0; i < MAXDATASIZE; i++) 
      buf[i] = '\0';
  
    if ((bytes_r=recv(socket, buf, MAXDATASIZE-1, 0)) == -1){
         continue;
    }
    if (bytes_r > 0){
       cnt++;
       buf[bytes_r] = 0;
       if(cnt == 1)
         blk->username = buf;
       if(cnt == 2)
         blk->passwd = buf;
       
    }
   }

}

void receive_order_blk(db_blk* blk, int socket){

   blk->type = 2;
   int bytes_r = 0;
   char buf[MAXDATASIZE];
   int cnt = 0;
   while(1){
  
    if (cnt == 5)
       break;  

    for(int i = 0; i < MAXDATASIZE; i++)
       buf[i] = '\0';

    if ((bytes_r=recv(socket, buf, MAXDATASIZE-1, 0)) == -1){
         continue;
    }
    if (bytes_r > 0){
       cnt++;
       buf[bytes_r] = 0;
       if(cnt == 1)
         blk->ord_id = stoi(buf);
       if(cnt == 2)
         blk->username = buf;
       if(cnt == 3)
         blk->food_qty = stoi(buf);
       if(cnt == 4)
         blk->food_id = stoi(buf);
       if(cnt == 5)
         blk->food_pr = stoi(buf);

    }
   }    

}

void send_food_blk(db_blk* blk, int socket){
   if (send(socket, (const char*)'0', 1, 0) == -1)
       perror("send");
   sleep(2);
   int bytes_r = 0;
   char buf[MAXDATASIZE];
   int cnt = 0;
   int len = 0;
   printf("send 0:\n");
   while(1){
  
    if (cnt == 4)
       break;
    
    cnt++;
    if(cnt == 1){
         strcpy(buf, to_string(blk->food_id).c_str());
         len = to_string(blk->food_id).length();
    }
    if(cnt == 2){
         strcpy(buf, blk->food_name.c_str());
         len = blk->food_name.length();
    }
    if(cnt == 3){
         strcpy(buf, to_string(blk->food_qty).c_str());
         len = to_string(blk->food_qty).length();
    }
    if(cnt == 4){
         strcpy(buf, to_string(blk->food_pr).c_str());
         len = to_string(blk->food_pr).length();
    }
    
    if (send(socket, buf, len, 0) == -1){
        continue;
    }
    sleep(2);
    
  }

}

void send_user_blk(db_blk* blk, int socket){
  if (send(socket, (const char*)'1', 1, 0) == -1)
       perror("send");
   sleep(2);
   int bytes_r = 0;
   char buf[MAXDATASIZE];
   int cnt = 0;
   int len = 0;
   printf("send 1:\n");
   while(1){
  
    if (cnt == 2)
       break;
    
    cnt++;
    if(cnt == 1){
         strcpy(buf, blk->username.c_str());
         len = blk->username.length();
    }
    if(cnt == 2){
         strcpy(buf, blk->passwd.c_str());
         len = blk->passwd.length();
    }
    
    
    if (send(socket, buf, len, 0) == -1){
        continue;
    }
    sleep(2);
    
  }

}

void send_order_blk(db_blk* blk, int socket){
  if (send(socket, (const char*)'2', 1, 0) == -1)
       perror("send");
   sleep(2);
   int bytes_r = 0;
   char buf[MAXDATASIZE];
   int cnt = 0;
   int len = 0;
   printf("send 2:\n");
   while(1){
  
    if (cnt == 5)
       break;
    
    cnt++;
    if(cnt == 1){
         strcpy(buf, to_string(blk->ord_id).c_str());
         len = to_string(blk->ord_id).length();
    }
    if(cnt == 2){
         strcpy(buf, blk->username.c_str());
         len = blk->username.length();
    }
    if(cnt == 3){
         strcpy(buf, to_string(blk->food_qty).c_str());
         len = to_string(blk->food_qty).length();
    }
    if(cnt == 4){
         strcpy(buf, to_string(blk->food_id).c_str());
         len = to_string(blk->food_id).length();
    }
    if(cnt == 5){
         strcpy(buf, to_string(blk->food_pr).c_str());
         len = to_string(blk->food_pr).length();
    }
    
    if (send(socket, buf, len, 0) == -1){
        continue;
    }
    sleep(2);
  }

}


int main(void)
{
	time_t timer;
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector.s address information
	int sin_size;
	struct sigaction sa;
	int yes=1;
        char buf[MAXDATASIZE];
        int numbytes = 0;
        int ct = 0;
        int type = -1, req = -1;
     
        Database* db_obj = new Database();    
    
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	 {
		perror("socket");
		exit(1);
	}
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_port = htons(MYPORT); // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(my_addr.sin_zero), 0, 8); // zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
	{
		perror("sigaction");
		exit(1);
	}
	while(1)  // main accept() loop
	{
                printf("100");
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,(socklen_t*)&sin_size)) == -1)
		{
			//perror("accept");
			continue;
		}
		printf("Received request from Client: %s:%d\n",
		inet_ntoa(their_addr.sin_addr),MYPORT);
		if (!fork()) 
		{ // this is the child process
                        printf("fork");
			close(sockfd); // child doesn.t need the listener
                        while(1){
                            //printf("203");
                            if ((numbytes=recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) 
	                   {
		                continue;
	                   }
                           if(numbytes > 0){
                                ct++;
                                buf[numbytes] = 0;
                                if(ct == 1)
                                    type = stoi(buf);
                                if(ct == 2){
                                    printf("213");
                                    req = stoi(buf);
                                    db_blk* new_blk = new db_blk;
                                    if(type == 0){ printf("inside\n");
                                       receive_food_blk(new_blk, new_fd);}
                                    else if(type == 1)
                                       receive_user_blk(new_blk, new_fd);
                                    else if(type == 2)
                                       receive_order_blk(new_blk, new_fd);
                                    
                                    printf("223");
                                    if(req == 0)
                                       db_obj->insert_db_blk(type, new_blk);
                                    else if(req == 1)
                                       db_obj->update_db_blk(type, new_blk);
                                    else if(req == 2)
                                       db_obj->delete_db_blk(type, new_blk);
                                    else if(req == 3){
                                       db_blk* out_blk = db_obj->search_db_blk(type, new_blk);
                                       if(!out_blk->dirty){
                                         if(out_blk->type == 0)
                                           send_food_blk(out_blk, new_fd);
                                         else if(out_blk->type == 1)
                                           send_user_blk(out_blk, new_fd);
                                         else if(out_blk->type == 2)
                                           send_order_blk(out_blk, new_fd);
                                       }
                                       else{
                                         if (send(new_fd, (const char*)'3', 1, 0) == -1)
                                           perror("send");
                                       }
                                    }
                                    printf("233");             
                                    ct = 0;
                                    db_obj->print_db_blk();
                                }
                                
                                    
                                //printf("Received request from Client: %s:%d\n",
                                //inet_ntoa(their_addr.sin_addr),MYPORT);
                                //timer = time(NULL);
		            	//if (send(new_fd, ctime(&timer), 30, 0) == -1)
			          //perror("send");
                           }
                        }
			
			//close(new_fd);
			//exit(0);
		}
	close(new_fd); // parent doesn.t need this
	}
	return 0;
}



