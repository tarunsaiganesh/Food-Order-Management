#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define FLAG 0
#define cache_sz 5

//Socket parameters
#define PORT 1025
#define MAXDATASIZE 500
//#define SERVER_IP 192.168.1.1

using namespace std;

typedef struct cache_blk{
  string username;
  string email;
  string passwd;
  string first_name;
  string last_name;
  string home_addr;
  int64_t phone_num;
  int64_t food_id;
  string food_name;
  int64_t food_qty;
  int64_t food_pr;
  int64_t plrty;
  int64_t disct;
  string catgry;
  string origin;
  int64_t ord_id;
  int64_t type;
  bool dirty;
  int64_t lru_cnt;
  int64_t lfu_cnt;	  
  
}cache_blk;


typedef struct cache_way_blk{
  string username;
  int64_t lru_cnt;
  int64_t lfu_cnt;
  int64_t num_blks;
  vector<cache_blk> cache_set;
  
}cache_way_blk;


class Cache{
  public:
    // int64_t cache_sz;
    int sleep_cnt = 0;
    int64_t cache_filled;
    int64_t num_users = 0;
    int64_t cache_hits;
    int64_t cache_acc;
    int64_t num_food;
    int64_t time = 0;
    vector<cache_way_blk> cache_way;

    //Socket varaibles
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;

    char hostn[400];
    char ipadd[400];
    struct hostent *hostIP;

    Cache(){
        cache_way_blk* new_way = new cache_way_blk;
        new_way->username = "fooditems";
        new_way->lru_cnt = 0;
        new_way->lfu_cnt = 0;
        new_way->num_blks = 0;
        new_way->cache_set;
        cache_way.push_back(*new_way);

        //Socket creation
        
        if((gethostname(hostn, sizeof(hostn))) == 0){
            hostIP = gethostbyname(hostn); //the netdb.h function gethostbyname
        }
        else{
            printf("ERROR:FC4539 - IP Address not found."); //error if the hostname is not found
        }

        if((he=gethostbyname("128.195.27.28")) == NULL){ // get the server info
            perror("gethostbyname");
            exit(1);
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET; // host byte order
        their_addr.sin_port = htons(PORT); // short, network byte order
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(&(their_addr.sin_zero), 0, 8);

        if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
            perror("connect");
            exit(1);
        }
	 

    
    }	
    
    int64_t insert_cache_blk(int64_t type, cache_blk* blk){
        
        //Insert to database (socket send)
	       char b[11] = "abcdefghij";
	       if(blk->type ==0){
            //printf("quant %ld\n", new_blk->food_qty);
            send_food_blk(0, blk);  
         }
          else if(blk->type ==1)
            send_user_blk(0, blk);
          else if(blk->type ==2)
            send_order_blk(0, blk);

        // Test program above
        printf("c6\n");
        cache_acc++;
        // free(new_blk);
        return 0;
    }

    int64_t update_cache_blk(int64_t type, cache_blk* blk){
        
        if(type == 0){
       
                send_food_blk(1, blk);
        }
                //retrieve(search) from database (socket send & receive)
                // insert & update & then return from here
                
           
        else if(type == 1){
          
                send_user_blk(1, blk);    
               }

        else if(type == 2){
              send_order_blk(1, blk);

        }
        printf("u14\n");
        cache_acc++;
        return 0;

    }

    int64_t delete_cache_blk(int64_t type, cache_blk* blk){
        
        // delete from database (socket send) 
        if(type == 0)
          send_food_blk(2, blk);
        else if(type == 1)
          send_user_blk(2, blk);
        else if(type == 2)
          send_order_blk(2, blk);
        cache_acc++;
        return 0;
    }     

              

  private:
    void send_food_blk(int req_type, cache_blk* blk){
      printf("send food blk\n");
	char buf[MAXDATASIZE];
      int len = 0;
      strcpy(buf, to_string(0).c_str());
      len = to_string(0).length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
       sleep(2); sleep_cnt++;
       printf("send 2\n");
       strcpy(buf, to_string(req_type).c_str());
      len = to_string(req_type).length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
       strcpy(buf, to_string(blk->food_id).c_str());
      len = to_string(blk->food_id).length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
       strcpy(buf, blk->food_name.c_str());
      len = blk->food_name.length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
       strcpy(buf, to_string(blk->food_qty).c_str());
      len = to_string(blk->food_qty).length();
      //printf("qty: %ld\n", blk->food_qty);
     printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
       strcpy(buf, to_string(blk->food_pr).c_str());
      len = to_string(blk->food_pr).length();
     printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      printf("finish send\n");
    }
    void receive_food_blk(cache_blk* blk){
        //blk->type = 0;
        int bytes_r = 0;
        char buf[MAXDATASIZE];
        int cnt = -1;
        while(1){
          if (cnt == 4)
            break;  
          if ((bytes_r=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
            continue;
          }
          if (bytes_r > 0){
            printf("buf %s\n", buf);
             cnt++;
             buf[bytes_r] = 0;
             if(cnt == 0){
               blk->type = stoi(buf);
               if(blk->type == 3)
                return;
             }
             if(cnt == 1)
               blk->food_id = stoi(buf);
             if(cnt == 2)
               blk->food_name = buf;
             if(cnt == 3)
               blk->food_qty = stoi(buf);
             if(cnt == 4)
               blk->food_pr = stoi(buf);

          }
        }  
    }

    void send_user_blk(int req_type, cache_blk* blk){
     printf("send user blk\n");
	 char buf[MAXDATASIZE];
      int len = 0;
      strcpy(buf, to_string(1).c_str());
      len = to_string(1).length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(req_type).c_str());
      len = to_string(req_type).length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, blk->username.c_str());
      len = blk->username.length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, blk->passwd.c_str());
      len = blk->passwd.length();
      printf("buf = %s %d\n", buf,len);
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      printf("finish send\n");
    }
    void receive_user_blk(cache_blk* blk){
      //blk->type = 1;
      int bytes_r = 0;
      char buf[MAXDATASIZE];
      int cnt = -1;
      while(1){
      
        if (cnt == 2)
           break;  

        if ((bytes_r=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
             continue;
        }
        if (bytes_r > 0){
           cnt++;
           buf[bytes_r] = 0;
           if(cnt == 0){
               blk->type = stoi(buf);
               if(blk->type == 3)
                return;
             }
           if(cnt == 1)
             blk->username = buf;
           if(cnt == 2)
             blk->passwd = buf;
           
        }
      }
    }
    void send_order_blk(int req_type, cache_blk* blk){
     printf("send order blk\n");
	 char buf[MAXDATASIZE];
      int len = 0;
      strcpy(buf, to_string(2).c_str());
      len = to_string(2).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(req_type).c_str());
      len = to_string(req_type).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(blk->ord_id).c_str());
      len = to_string(blk->ord_id).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, blk->username.c_str());
      len = blk->username.length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(blk->food_qty).c_str());
      len = to_string(blk->food_qty).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(blk->food_id).c_str());
      len = to_string(blk->food_id).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      sleep(2); sleep_cnt++;
      strcpy(buf, to_string(blk->food_pr).c_str());
      len = to_string(blk->food_pr).length();
      if(send(sockfd, buf, len, 0 )==-1){
           perror("send");
           exit(1);
      }
      printf("finish send\n");
    }
    void receive_order_blk(cache_blk* blk){

      // blk->type = 2;
      int bytes_r = 0;
      char buf[MAXDATASIZE];
      int cnt = -1;
      while(1){
      
        if (cnt == 5)
           break;  

        if ((bytes_r=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
             continue;
        }
        if (bytes_r > 0){
           cnt++;
           buf[bytes_r] = 0;
           if(cnt == 0){
               blk->type = stoi(buf);
               if(blk->type == 3)
                return;
             }
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
    void insert_food_item(cache_blk* blk, cache_blk* new_blk){
        
        new_blk->food_id = blk->food_id;
        new_blk->food_name = blk->food_name;
        new_blk->food_qty = blk->food_qty;
        new_blk->food_pr = blk->food_pr;
        // printf("%ld %ld\n", new_blk->food_pr, blk->food_pr);
        new_blk->plrty = blk->plrty;
        new_blk->disct = blk->disct;
        new_blk->catgry = blk->catgry;
        new_blk->origin = blk->origin;
        new_blk->type = 0;
        //new_blk->dirty = false;
        new_blk->lru_cnt = ++time;
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

    void insert_user_item(cache_blk* blk, cache_blk* new_blk){
        
        new_blk->username = blk->username;
        new_blk->email = blk->email;
        new_blk->passwd = blk->passwd;
        new_blk->first_name = blk->first_name;
        new_blk->last_name = blk->last_name;
        new_blk->home_addr = blk->home_addr;
        new_blk->phone_num = blk->phone_num;
        new_blk->type = 1;
        //new_blk->dirty = false;
        new_blk->lru_cnt = ++time;
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

    void insert_order_item(cache_blk* blk, cache_blk* new_blk){
        
        new_blk->username = blk->username;
        new_blk->ord_id = blk->ord_id;
        new_blk->food_id = blk->food_id;
        new_blk->food_qty = blk->food_qty;
        new_blk->type = 2;
        //new_blk->dirty = false;
        new_blk->lru_cnt = ++time;
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

};



























 

