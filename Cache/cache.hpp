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

#define FLAG 1
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
        printf("c1\n");
        if(cache_filled >= cache_sz){
          printf("c2\n");
	        evict(FLAG);
        } 
        
        cache_blk* new_blk = new cache_blk;
        if(type == 0){
            //printf("q %ld\n", blk->food_qty);
            insert_food_item(blk, new_blk);
        //printf("%ld\n", new_blk->food_pr);
        printf("c3\n");
	    new_blk->dirty = false;
            new_blk->lfu_cnt = 1;	
            cache_way[0].cache_set.push_back(*new_blk);
	    cache_way[0].lru_cnt = (cache_way[0].lru_cnt == 0) ? new_blk->lru_cnt : cache_way[0].lru_cnt;
            cache_way[0].lfu_cnt = 1;
            cache_way[0].num_blks++; 
            num_food++;
            cache_filled++;
            //free(new_blk); 
        }
        else if(type == 1){
          printf("c4\n");
            insert_user_item(blk, new_blk);
            new_blk->dirty = false;
            new_blk->lfu_cnt = 1;
            vector<cache_way_blk>::iterator itr;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != cache_way.end()){
               itr->cache_set.push_back(*new_blk);
	       itr->lfu_cnt = 1;
               itr->num_blks++; 
               cache_filled++;
            }
            else{
	       cache_way_blk* new_way = new cache_way_blk;
               new_way->username = new_blk->username;
               new_way->lru_cnt = new_blk->lru_cnt;
               new_way->lfu_cnt = 1;
               new_way->num_blks = 1;
               new_way->cache_set.push_back(*new_blk);
               cache_way.push_back(*new_way);
               num_users++;
               cache_filled++;
            }
            //free(new_blk); 
        }
        else if(type == 2){
          printf("c5\n");
            insert_order_item(blk, new_blk);
            new_blk->dirty = false;
            new_blk->lfu_cnt = 1;
            vector<cache_way_blk>::iterator itr;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != cache_way.end()){
               itr->cache_set.push_back(*new_blk);
	       itr->lfu_cnt = 1;
               itr->num_blks++; 
               cache_filled++;
            }
            else{
	       cache_way_blk* new_way = new cache_way_blk;
               new_way->username = new_blk->username;
               new_way->lru_cnt = new_blk->lru_cnt;
               new_way->lfu_cnt = 1;
               new_way->num_blks = 1;
               new_way->cache_set.push_back(*new_blk);
               cache_way.push_back(*new_way);
               num_users++;
               cache_filled++;
            }
            //free(new_blk); 
        }
        //Insert to database (socket send)
	       char b[11] = "abcdefghij";
	       if(new_blk->type ==0){
            //printf("quant %ld\n", new_blk->food_qty);
            send_food_blk(0, new_blk);  
         }
          else if(new_blk->type ==1)
            send_user_blk(0, new_blk);
          else if(new_blk->type ==2)
            send_order_blk(0, new_blk);

        // Test program above
        printf("c6\n");
        cache_acc++;
        // free(new_blk);
        return 0;
    }

    int64_t update_cache_blk(int64_t type, cache_blk* blk){
        printf("u1\n");
        cache_blk* new_blk = new cache_blk;
        if(type == 0){
          printf("u2\n");
            insert_food_item(blk, new_blk);
            new_blk->dirty = true;
            vector<cache_blk>::iterator itr, itr_start, itr_end;
            itr_start = cache_way[0].cache_set.begin();
            itr_end = cache_way[0].cache_set.end();            

            int index = 0;
            for(itr =  itr_start; itr < itr_end; itr++){
                if(itr->food_id == new_blk->food_id)
                  break;
                index++;   
            }
            printf("u3\n");
            if(itr != itr_end){
                new_blk->lfu_cnt = cache_way[0].cache_set[index].lfu_cnt++;
                cache_way[0].cache_set[index] = *new_blk;
                cache_hits++; 
            }
            else{
              printf("u4\n");
                send_food_blk(3, new_blk);
                cache_blk* tmp_blk = new cache_blk;
                receive_food_blk(tmp_blk);
                if(tmp_blk->type != 3){
                  insert_cache_blk(0, tmp_blk);
                  update_cache_blk(0, tmp_blk);
                }
                return 0;
                //retrieve(search) from database (socket send & receive)
                // insert & update & then return from here
                
            }
             
            int min_lru_cnt = itr_start->lru_cnt;
            int min_lfu_cnt = itr_start->lfu_cnt;
            for(itr =  itr_start; itr < itr_end; itr++){
                if(itr->lru_cnt < min_lru_cnt) 
                   min_lru_cnt = itr->lru_cnt;
                if(itr->lfu_cnt < min_lfu_cnt)
                   min_lfu_cnt = itr->lfu_cnt;
            }
            cache_way[0].lru_cnt = min_lru_cnt;
            cache_way[0].lfu_cnt = min_lfu_cnt;
            // free(new_blk);
            printf("u5\n"); 
        }
        else if(type == 1){
          printf("u6\n");
            insert_user_item(blk, new_blk);
            new_blk->dirty = true;
            vector<cache_way_blk>::iterator itr;
            vector<cache_blk>::iterator itrs, itr_start, itr_end;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == new_blk->type)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  new_blk->lfu_cnt = ++itr->cache_set[index].lfu_cnt;
                  itr->cache_set[index] = *new_blk;
                  cache_hits++; 
               }
               else{
                   //retrieve(search) from database (socket send & receive)
                   // insert & update & then return from here
                  send_food_blk(3, new_blk);
                  cache_blk* tmp_blk = new cache_blk;
                  if(tmp_blk->type != 3){
                  insert_cache_blk(1, tmp_blk);
                  update_cache_blk(1, tmp_blk);
                  }
                  return 0;
                     
               }
               //itr->cache_set.push_back(*new_blk);
               int min_lru_cnt = itr_start->lru_cnt;
               int min_lfu_cnt = itr_start->lfu_cnt;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->lru_cnt < min_lru_cnt) 
                    min_lru_cnt = itrs->lru_cnt;
                  if(itrs->lfu_cnt < min_lfu_cnt)
                    min_lfu_cnt = itrs->lfu_cnt;
               }
               itr->lru_cnt = min_lru_cnt;
               itr->lfu_cnt = min_lfu_cnt;
               // free(new_blk);
	       
               
            }
            else{
               //retrieve(search) from database (socket send & receive)
               // insert & update & then return from here
                send_food_blk(3, new_blk);
                cache_blk* tmp_blk = new cache_blk;
                receive_food_blk(tmp_blk);
                if(tmp_blk->type != 3){
                  insert_cache_blk(1, tmp_blk);
                  update_cache_blk(1, tmp_blk);
                }
                return 0;
	       
            }
            // free(new_blk);
         
        }
        else if(type == 2){
          printf("u7\n");
            insert_order_item(blk, new_blk);
            new_blk->dirty = true;
            vector<cache_way_blk>::iterator itr;
            vector<cache_blk>::iterator itrs, itr_start, itr_end;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }
            printf("u8\n");
            if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs = itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == new_blk->ord_id)
                    break;
                  index++;   
               }
               printf("u9\n");
               if(itrs != itr_end){
                printf("u100\n");
                  new_blk->lfu_cnt = ++itr->cache_set[index].lfu_cnt;
                  itr->cache_set[index] = *new_blk;
                  cache_hits++; 
               }
               else{
                    printf("u10\n");
                   //retrieve(search) from database (socket send & receive)
                   // insert & update & then return from here
                    send_food_blk(3, new_blk);
                    cache_blk* tmp_blk = new cache_blk;
                    receive_food_blk(tmp_blk);
                    if(tmp_blk->type != 3){
                      insert_cache_blk(2, tmp_blk);
                      update_cache_blk(2, tmp_blk);
                    }
                    printf("u11\n");
                    return 0;
               }
               //itr->cache_set.push_back(*new_blk);
               int min_lru_cnt = itr_start->lru_cnt;
               int min_lfu_cnt = itr_start->lfu_cnt;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itr->lru_cnt < min_lru_cnt) 
                    min_lru_cnt = itrs->lru_cnt;
                  if(itr->lfu_cnt < min_lfu_cnt)
                    min_lfu_cnt = itrs->lfu_cnt;
               }
               itr->lru_cnt = min_lru_cnt;
               itr->lfu_cnt = min_lfu_cnt;
               // free(new_blk);
	       
               printf("u101\n");
            }
            else{
               //retrieve(search) from database (socket send & receive)
               // insert & update & then return from here
                    printf("u12\n");
                    send_food_blk(3, new_blk);
                    cache_blk* tmp_blk = new cache_blk;
                    receive_food_blk(tmp_blk);
                    if(tmp_blk->type != 3){
                        insert_cache_blk(2, tmp_blk);
                        update_cache_blk(2, tmp_blk);
                    }
                    printf("u13\n");
                    return 0;
            }
            //free(new_blk);

        }
        printf("u14\n");
        cache_acc++;
        return 0;

    }

    int64_t delete_cache_blk(int64_t type, cache_blk* blk){
        if(type == 0){
            vector<cache_blk>::iterator itr, itr_start, itr_end;
            itr_start = cache_way[0].cache_set.begin();
            itr_end = cache_way[0].cache_set.end();            

            int index = 0;
            for(itr =  itr_start; itr < itr_end; itr++){
                if(itr->food_id == blk->food_id)
                  break;
                index++;   
            }

            if(itr != itr_end){
                cache_way[0].cache_set.erase(itr);
                cache_hits++;
                cache_way[0].num_blks--;
                num_food--; 
                cache_filled--;
            }
            
           
            
            if(cache_way[0].num_blks > 0){ 
                int min_lru_cnt = itr_start->lru_cnt;
                int min_lfu_cnt = itr_start->lfu_cnt;
                for(itr =  itr_start; itr < itr_end; itr++){
                   if(itr->lru_cnt < min_lru_cnt) 
                     min_lru_cnt = itr->lru_cnt;
                   if(itr->lfu_cnt < min_lfu_cnt)
                     min_lfu_cnt = itr->lfu_cnt;
                }
                cache_way[0].lru_cnt = min_lru_cnt;
                cache_way[0].lfu_cnt = min_lfu_cnt;
           }
           else{
                cache_way[0].lru_cnt = 0;
                cache_way[0].lfu_cnt = 0;
           }
                 
        }
        else if(type == 1){
            vector<cache_way_blk>::iterator itr;
            vector<cache_blk>::iterator itrs, itr_start, itr_end;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == blk->type)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                printf("into erase\n");
                  itr->cache_set.erase(itrs);
                  cache_hits++; 
                  itr->num_blks--;
                  cache_filled--; 
               }
               
               //delete from database

               //itr->cache_set.push_back(*new_blk);
               if(itr->num_blks > 0){
                   int min_lru_cnt = itr_start->lru_cnt;
                   int min_lfu_cnt = itr_start->lfu_cnt;
                   for(itrs =  itr_start; itrs < itr_end; itrs++){
                     if(itrs->lru_cnt < min_lru_cnt) 
                       min_lru_cnt = itrs->lru_cnt;
                     if(itrs->lfu_cnt < min_lfu_cnt)
                       min_lfu_cnt = itrs->lfu_cnt;
                   }
                   itr->lru_cnt = min_lru_cnt;
                   itr->lfu_cnt = min_lfu_cnt;
               }
               else{
                   cache_way.erase(itr);
                   num_users--;
               }
	                   
            }
            else{
               //delete from database
            }
            
         
        }
        else if(type == 2){
            vector<cache_way_blk>::iterator itr;
            vector<cache_blk>::iterator itrs, itr_start, itr_end;
            for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == blk->ord_id)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  itr->cache_set.erase(itrs);
                  cache_hits++; 
                  itr->num_blks--;
                  cache_filled--; 
               }
               
               //delete from database
               //itr->cache_set.push_back(*new_blk);
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();
               if(itr->num_blks > 0){
                   int min_lru_cnt = itr_start->lru_cnt;
                   int min_lfu_cnt = itr_start->lfu_cnt;
                   for(itrs =  itr_start; itrs < itr_end; itrs++){
                     if(itrs->lru_cnt < min_lru_cnt) 
                       min_lru_cnt = itrs->lru_cnt;
                     if(itrs->lfu_cnt < min_lfu_cnt)
                       min_lfu_cnt = itrs->lfu_cnt;
                   }
                   itr->lru_cnt = min_lru_cnt;
                   itr->lfu_cnt = min_lfu_cnt;
               }
               else{
                   cache_way.erase(itr);
                   num_users--;
               }
	                   
            }
            else{
               //delete from database
            }
            
         
        }
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

    int64_t evict(int flag){
       printf("e1\n");
       vector<cache_way_blk>::iterator itr, min_itrw, itr_start, itr_end, itr_sec;
       itr_start = cache_way.begin();
       itr_end = cache_way.end();
       itr_sec = next(itr_start, 1);
       int min_cnt = (flag) ? itr_sec->lfu_cnt : itr_sec->lru_cnt; 
       min_itrw = itr_sec;
       printf("%d\n", min_cnt);
       for(itr = itr_start; itr < itr_end; itr++){
	     if(flag == 0){	
          printf("e2\n");
             if(itr->lru_cnt < min_cnt && itr->lru_cnt != 0){
                min_cnt = itr->lru_cnt;
                min_itrw = itr;  
                printf("%d\n", min_cnt);
             }
           }
           else{
             if(itr->lfu_cnt < min_cnt && itr->lfu_cnt != 0){
                min_cnt = itr->lfu_cnt;
                min_itrw = itr;  
             }
           }
       }
       printf("e3\n");
       vector<cache_blk>::iterator itrs, min_itr, itr_starts, itr_ends;
       itr_starts = min_itrw->cache_set.begin();
       printf("e4\n");
       itr_ends = min_itrw->cache_set.end();
       printf("e100\n");
       int min_cnts = (flag) ? itr_starts->lfu_cnt : itr_starts->lru_cnt; 
       min_itr = itr_starts;
       printf("e101\n");
       for(itrs = itr_starts; itrs < itr_ends; itrs++){
        printf("for\n");
	     if(flag == 0){	
              printf("e5\n");
             if(itrs->lru_cnt == min_cnts){
                min_itr = itrs; 
                break; 
             }
           }
           else{
            printf("e6\n");
             if(itrs->lfu_cnt < min_cnts){
                min_itr = itrs;  
                break;
             }
           }
       
       }

       if(min_itr->dirty){
          //update database(socket send)
          if(min_itr->type == 0)
            send_food_blk(1, &(*min_itr));
          else if(min_itr->type == 1)
            send_user_blk(1, &(*min_itr));
          else if(min_itr->type == 2)
            send_order_blk(1, &(*min_itr));
       }
       min_itrw->cache_set.erase(min_itr);
       min_itrw->num_blks--;
       cache_filled--; 
       printf("e8\n");
       itr_starts = min_itrw->cache_set.begin();
       itr_ends = min_itrw->cache_set.end();
       if(min_itrw->num_blks > 0){
           int min_lru_cnt = itr_starts->lru_cnt;
           int min_lfu_cnt = itr_starts->lfu_cnt;
           for(itrs =  itr_starts; itrs < itr_ends; itrs++){
              if(itrs->lru_cnt < min_lru_cnt) 
                 min_lru_cnt = itrs->lru_cnt;
              if(itrs->lfu_cnt < min_lfu_cnt)
                 min_lfu_cnt = itrs->lfu_cnt;
           }
           min_itrw->lru_cnt = min_lru_cnt;
           min_itrw->lfu_cnt = min_lfu_cnt;
       }
       else if(min_itrw->username != "fooditems"){
            cache_way.erase(min_itrw);
            num_users--;
       }
       printf("e9\n");  
       return 0;
    }

    

    //In search_cache_blk(), if not found in cache -> database search(socket send and receive)
    //insert in cache and return
    cache_blk* search_cache_blk(int64_t type, cache_blk* blk){
        cache_blk* new_blk = new cache_blk;
        if(type == 0){
           vector<cache_blk>::iterator itr, itrs, itr_start, itr_end;
           for(itr = cache_way[0].cache_set.begin(); itr < cache_way[0].cache_set.end() && cache_way[0].num_blks > 0; itr++){
               if(itr->food_id == blk->food_id)
                  break;
            }

           if(itr != cache_way[0].cache_set.end()){
               itr->lru_cnt = ++time;
               itr->lfu_cnt = itr->lfu_cnt + 1;
               
               itr_start = cache_way[0].cache_set.begin();
               itr_end = cache_way[0].cache_set.end(); 
               int min_lru_cnt = itr_start->lru_cnt;
               int min_lfu_cnt = itr_start->lfu_cnt;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->lru_cnt < min_lru_cnt) 
                    min_lru_cnt = itrs->lru_cnt;
                  if(itrs->lfu_cnt < min_lfu_cnt)
                    min_lfu_cnt = itrs->lfu_cnt;
               }
               cache_way[0].lru_cnt = min_lru_cnt;
               cache_way[0].lfu_cnt = min_lfu_cnt;
               insert_food_item((cache_blk*)&(*itr), new_blk);
               new_blk->dirty = 0;
               return new_blk;
           }
           else{
              printf("Invalid food item search block");
              //new_blk->dirty = 1;
              cache_blk* b = new cache_blk;
              send_food_blk(3, new_blk);
              receive_food_blk(b);
              if(b->type != 3)
                insert_cache_blk(0, b);
              return b;
              
           }
           
        }
        else if(type == 1){
           vector<cache_way_blk>::iterator itr;
           vector<cache_blk>::iterator itrs, itr_start, itr_end, itrst;
           for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
           }
           if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == blk->type)
                    break;
                  index++;   
               }
             
               if(itrs != itr_end){
                  itrs->lru_cnt = ++time;
                  itrs->lfu_cnt = itrs->lfu_cnt + 1;

                  int min_lru_cnt = itr_start->lru_cnt;
                  int min_lfu_cnt = itr_start->lfu_cnt;
                  for(itrst =  itr_start; itrst < itr_end; itrst++){
                    if(itrst->lru_cnt < min_lru_cnt) 
                      min_lru_cnt = itrst->lru_cnt;
                    if(itrst->lfu_cnt < min_lfu_cnt)
                      min_lfu_cnt = itrst->lfu_cnt;
                  }
                  itr->lru_cnt = min_lru_cnt;
                  itr->lfu_cnt = min_lfu_cnt;                 

                  insert_user_item((cache_blk*)&(*itrs), new_blk);
                  new_blk->dirty = 0;
                  return new_blk;
               }
               else{
                  printf("Invalid user item search block");
                  cache_blk* b = new cache_blk;
                  send_user_blk(3, new_blk);
                  receive_user_blk(b);
                  if(b->type != 3)
                    insert_cache_blk(1, b);
                  return b;
               }


           }
           else{
              printf("Invalid user item search block");
              cache_blk* b = new cache_blk;
                  send_user_blk(3, new_blk);
                  receive_user_blk(b);
                  if(b->type != 3)
                    insert_cache_blk(1, b);
                  return b;
           }
        }
        else if(type == 2){
           vector<cache_way_blk>::iterator itr;
           vector<cache_blk>::iterator itrs, itr_start, itr_end, itrst;
           for(itr = cache_way.begin(); itr < cache_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
           }
           if(itr != cache_way.end()){
               itr_start = itr->cache_set.begin();
               itr_end = itr->cache_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == blk->ord_id)
                    break;
                  index++;   
               }
             
               if(itrs != itr_end){
                  itrs->lru_cnt = ++time;
                  itrs->lfu_cnt = itrs->lfu_cnt + 1; 

                  int min_lru_cnt = itr_start->lru_cnt;
                  int min_lfu_cnt = itr_start->lfu_cnt;
                  for(itrst =  itr_start; itrst < itr_end; itrst++){
                    if(itrst->lru_cnt < min_lru_cnt) 
                      min_lru_cnt = itrst->lru_cnt;
                    if(itrst->lfu_cnt < min_lfu_cnt)
                      min_lfu_cnt = itrst->lfu_cnt;
                  }
                  itr->lru_cnt = min_lru_cnt;
                  itr->lfu_cnt = min_lfu_cnt;

                  insert_user_item((cache_blk*)&(*itrs), new_blk);
                  new_blk->dirty = 0;
                  return new_blk;
               }
               else{
                  cache_blk* b = new cache_blk;
                  send_order_blk(3, new_blk);
                  receive_order_blk(b);
                  if(b->type != 3)
                    insert_cache_blk(2, b);
                  return b;
               }


           }
           else{
              printf("Invalid order item search block");
              cache_blk* b = new cache_blk;
                  send_order_blk(3, new_blk);
                  receive_order_blk(b);
                  if(b->type != 3)
                    insert_cache_blk(2, b);
                  return b;
           }
        } 
  
    }


    void print_cache_blk(){

       vector<cache_way_blk>::iterator itr, itr_start, itr_end;
       vector<cache_blk>::iterator itrs, itr_starts, itr_ends;
       itr_start = cache_way.begin();
       itr_end = cache_way.end();
       
       int way_indx = 0, set_indx = 0;
       for(itr = itr_start; itr < itr_end; itr++){
          itr_starts = itr->cache_set.begin();
          itr_ends = itr->cache_set.end();
          printf("-------------------------------------------------------------------------\n");
          printf("Cache Way Index: %d\n", way_indx);
          printf("Username: %s\n", itr->username.c_str());
          printf("LRU count: %ld, LFU count: %ld, Num blocks: %ld\n", itr->lru_cnt, itr->lfu_cnt, itr->num_blks);
          printf("-------------------------------------------------------------------------\n");
          for(itrs = itr_starts; itrs < itr_ends; itrs++){
	      if(itrs->type == 0){
                  printf("Food Item: %s %ld %ld %ld %d\n", itrs->food_name.c_str(), itrs->food_pr, itrs->lru_cnt, itrs->lfu_cnt, itrs->dirty); 
              }
              else if(itrs->type == 1){
                  printf("User Item: %s %s %ld %ld %d\n", itrs->username.c_str(), itrs->passwd.c_str(), itrs->lru_cnt, itrs->lfu_cnt, itrs->dirty);
              }
              else if(itrs->type == 2){
                  printf("Order Item: %ld %ld %ld %ld %d\n", itrs->ord_id, itrs->lru_cnt, itrs->lfu_cnt, itrs->food_qty, itrs->dirty);
              }
              set_indx++;
          }
          printf("\n");
	   way_indx++;
       }

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



























 

