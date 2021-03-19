#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <iterator>

#define FLAG 0
#define cache_sz 5

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
    int64_t cache_filled;
    int64_t num_users = 0;
    int64_t cache_hits;
    int64_t cache_acc;
    int64_t num_food;
    int64_t time = 0;
    vector<cache_way_blk> cache_way;

    Cache(){
        cache_way_blk* new_way = new cache_way_blk;
        new_way->username = "fooditems";
        new_way->lru_cnt = 0;
        new_way->lfu_cnt = 0;
        new_way->num_blks = 0;
        new_way->cache_set;
        cache_way.push_back(*new_way);
    
    }	
    
    int64_t insert_cache_blk(int64_t type, cache_blk* blk){
        printf("c1\n");
        if(cache_filled >= cache_sz){
          printf("c2\n");
	        evict(FLAG);
        } 
        
        cache_blk* new_blk = new cache_blk;
        if(type == 0){
            insert_food_item(blk, new_blk);
        printf("c3\n");
	    new_blk->dirty = false;
            new_blk->lfu_cnt = 1;	
            cache_way[0].cache_set.push_back(*new_blk);
	    cache_way[0].lru_cnt = (cache_way[0].lru_cnt == 0) ? 1 : cache_way[0].lru_cnt;
            cache_way[0].lfu_cnt = 1;
            cache_way[0].num_blks++; 
            num_food++;
            cache_filled++;
            free(new_blk); 
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
            free(new_blk); 
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
            free(new_blk); 
        }
        printf("c6\n");
        cache_acc++;
        return 0;
    }

    int64_t update_cache_blk(int64_t type, cache_blk* blk){
        cache_blk* new_blk = new cache_blk;
        if(type == 0){
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

            if(itr != itr_end){
                new_blk->lfu_cnt = cache_way[0].cache_set[index].lfu_cnt++;
                cache_way[0].cache_set[index] = *new_blk;
                cache_hits++; 
            }
            else{
                //retrieve from database
                // increase num_blks
                // take care of itr
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
            free(new_blk); 
        }
        else if(type == 1){
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
                  new_blk->lfu_cnt = itr->cache_set[index].lfu_cnt++;
                  itr->cache_set[index] = *new_blk;
                  cache_hits++; 
               }
               else{
                   //retrieve from database
                   //itr->lfu_cnt = 1;
                   //itr->num_blks++; 
                   //cache_filled++;
                   //return 0;
               }
               //itr->cache_set.push_back(*new_blk);
               int min_lru_cnt = itr_start->lru_cnt;
               int min_lfu_cnt = itr_end->lfu_cnt;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->lru_cnt < min_lru_cnt) 
                    min_lru_cnt = itrs->lru_cnt;
                  if(itrs->lfu_cnt < min_lfu_cnt)
                    min_lfu_cnt = itrs->lfu_cnt;
               }
               itr->lru_cnt = min_lru_cnt;
               itr->lfu_cnt = min_lfu_cnt;
               free(new_blk);
	       
               
            }
            else{
               //access database
               //insert
               //update
               //return 0;
	       /*cache_way_blk* new_way = new cache_way_blk;
               new_way->username = new_blk->username;
               new_way->lru_cnt = new_blk->lru_cnt;
               new_way->lfu_cnt = 1;
               new_way->num_blks = 1;
               new_way->cache_set.push_back(*new_blk);
               cache_way.push_back(*new_way);
               num_users++;
               cache_filled++;*/
            }
            free(new_blk);
         
        }
        else if(type == 2){
            insert_order_item(blk, new_blk);
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
               for(itrs = itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == new_blk->ord_id)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  new_blk->lfu_cnt = itr->cache_set[index].lfu_cnt++;
                  itr->cache_set[index] = *new_blk;
                  cache_hits++; 
               }
               else{
                   //retrieve from database
                   //itr->lfu_cnt = 1;
                   //itr->num_blks++; 
                   //cache_filled++;
                   //return 0;
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
               free(new_blk);
	       
               
            }
            else{
               //access database
               //insert
               //update
               //return 0;
	       /*cache_way_blk* new_way = new cache_way_blk;
               new_way->username = new_blk->username;
               new_way->lru_cnt = new_blk->lru_cnt;
               new_way->lfu_cnt = 1;
               new_way->num_blks = 1;
               new_way->cache_set.push_back(*new_blk);
               cache_way.push_back(*new_way);
               num_users++;
               cache_filled++;*/
            }
            free(new_blk);

        }

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
            // delete from database
           
            
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
             if(itr->lfu_cnt < min_cnt && itr->lru_cnt != 0){
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

       int min_cnts = (flag) ? itr_starts->lfu_cnt : itr_starts->lru_cnt; 
       
       for(itrs = itr_starts; itrs < itr_ends; itrs++){
	   if(flag == 0){	
              printf("e5\n");
             if(itrs->lru_cnt == min_cnts){
                min_itr = itrs; 
                break; 
             }
           }
           else{
             if(itrs->lfu_cnt < min_cnts){
                min_itr = itrs;  
                break;
             }
           }
       
       }

       if(min_itr->dirty){
          //update database
       }
       min_itrw->cache_set.erase(min_itr);
       min_itrw->num_blks--;
       cache_filled--; 
       printf("e6\n");
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
       printf("e7\n");  
       return 0;
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
                  printf("Food Item: %s %ld %ld %ld %d\n", itrs->food_name.c_str(), itrs->food_id, itrs->lru_cnt, itrs->lfu_cnt, itrs->dirty); 
              }
              else if(itrs->type == 1){
                  printf("User Item: %s %ld %ld %d\n", itrs->username.c_str(), itrs->lru_cnt, itrs->lfu_cnt, itrs->dirty);
              }
              else if(itrs->type == 2){
                  printf("Order Item: %ld %ld %ld %d\n", itrs->ord_id, itrs->lru_cnt, itrs->lfu_cnt, itrs->dirty);
              }
              set_indx++;
          }
          printf("\n");
	   way_indx++;
       }

    }          

  private:
    int64_t insert_db_ent(int64_t type, cache_blk* blk){

    }

    int64_t update_db_ent(int64_t type, cache_blk* blk){

    }

    int64_t delete_db_ent(int64_t type, cache_blk* blk){

    }
    
    void insert_food_item(cache_blk* blk, cache_blk* new_blk){
        
        new_blk->food_id = blk->food_id;
        new_blk->food_name = blk->food_name;
        new_blk->food_qty = blk->food_qty;
        new_blk->food_pr = blk->food_pr;
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



























 

