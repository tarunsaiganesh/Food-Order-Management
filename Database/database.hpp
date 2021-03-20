#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <iterator>

#define FLAG 0

using namespace std;

typedef struct db_blk{
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
  
}db_blk;


typedef struct db_way_blk{
  string username;
  int64_t lru_cnt;
  int64_t lfu_cnt;
  int64_t num_blks;
  vector<db_blk> db_set;
  
}db_way_blk;


class Database{
  public:
    int64_t db_filled;
    int64_t num_users = 0;
    int64_t db_acc;
    int64_t num_food;
    vector<db_way_blk> db_way;

    Database(){
        db_way_blk* new_way = new db_way_blk;
        new_way->username = "fooditems";
        new_way->num_blks = 0;
        new_way->db_set;
        db_way.push_back(*new_way);
    
    }	
    
    int64_t insert_db_blk(int64_t type, db_blk* blk){
        printf("c1\n");
                
        db_blk* new_blk = new db_blk;
        if(type == 0){
            insert_food_item(blk, new_blk);
            printf("c3\n");
            printf("nb: %d\n", new_blk->food_id);
            db_way[0].db_set.push_back(*new_blk);
	    printf("c22\n");
            db_way[0].num_blks++; 
            num_food++;
            db_filled++;
            //free(new_blk); 
            printf("c12\n");
        }
        else if(type == 1){
          printf("c4\n");
            insert_user_item(blk, new_blk);
            vector<db_way_blk>::iterator itr;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr->db_set.push_back(*new_blk);
	       itr->num_blks++; 
               db_filled++;
            }
            else{
	       db_way_blk* new_way = new db_way_blk;
               new_way->username = new_blk->username;
               new_way->num_blks = 1;
               new_way->db_set.push_back(*new_blk);
               db_way.push_back(*new_way);
               num_users++;
               db_filled++;
            }
            free(new_blk); 
        }
        else if(type == 2){
          printf("c5\n");
            insert_order_item(blk, new_blk);
            vector<db_way_blk>::iterator itr;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr->db_set.push_back(*new_blk);
	       itr->num_blks++; 
               db_filled++;
            }
            else{
	       db_way_blk* new_way = new db_way_blk;
               new_way->username = new_blk->username;
               new_way->num_blks = 1;
               new_way->db_set.push_back(*new_blk);
               db_way.push_back(*new_way);
               num_users++;
               db_filled++;
            }
            free(new_blk); 
        }
        printf("c6\n");
        db_acc++;
        return 0;
    }

    int64_t update_db_blk(int64_t type, db_blk* blk){
        db_blk* new_blk = new db_blk;
        if(type == 0){
            insert_food_item(blk, new_blk);
            vector<db_blk>::iterator itr, itr_start, itr_end;
            itr_start = db_way[0].db_set.begin();
            itr_end = db_way[0].db_set.end();            

            int index = 0;
            for(itr =  itr_start; itr < itr_end; itr++){
                if(itr->food_id == new_blk->food_id)
                  break;
                index++;   
            }

            if(itr != itr_end){
                new_blk->lfu_cnt = db_way[0].db_set[index].lfu_cnt++;
                db_way[0].db_set[index] = *new_blk;
                //db_hits++; 
            }
            else{
                printf("Invalid food item update block\n");
                return -1;
            }
             
            free(new_blk); 
        }
        else if(type == 1){
            insert_user_item(blk, new_blk);
            vector<db_way_blk>::iterator itr;
            vector<db_blk>::iterator itrs, itr_start, itr_end;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == new_blk->type)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  itr->db_set[index] = *new_blk;
                  //db_hits++; 
               }
               else{
                   printf("Invalid user item update block\n");
                   return -1;
               }
              
               free(new_blk);               
            }
            else{
               printf("Invalid user item update block\n");
               return -1;
            }
            free(new_blk);
         
        }
        else if(type == 2){
            insert_order_item(blk, new_blk);
            vector<db_way_blk>::iterator itr;
            vector<db_blk>::iterator itrs, itr_start, itr_end;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs = itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == new_blk->ord_id)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  itr->db_set[index] = *new_blk;
                  //db_hits++; 
               }
               else{
                   printf("Invalid order item update block\n");
                   return -1;
               }
               free(new_blk);
	                   
            }
            else{
               printf("Invalid order item update block\n");
               return -1;
            }
            free(new_blk);

        }

        db_acc++;
        return 0;

    }

    int64_t delete_db_blk(int64_t type, db_blk* blk){
        if(type == 0){
            vector<db_blk>::iterator itr, itr_start, itr_end;
            itr_start = db_way[0].db_set.begin();
            itr_end = db_way[0].db_set.end();            

            int index = 0;
            for(itr =  itr_start; itr < itr_end; itr++){
                if(itr->food_id == blk->food_id)
                  break;
                index++;   
            }

            if(itr != itr_end){
                db_way[0].db_set.erase(itr);
                //db_hits++;
                db_way[0].num_blks--;
                num_food--; 
                db_filled--;
            }
            else{
                printf("Invalid food item delete block\n");
                return -1;
            }
        }
        else if(type == 1){
            vector<db_way_blk>::iterator itr;
            vector<db_blk>::iterator itrs, itr_start, itr_end;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == blk->type)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  itr->db_set.erase(itrs);
                  //db_hits++; 
                  itr->num_blks--;
                  db_filled--; 
               }
               else{
                printf("Invalid user item delete block\n");
                return -1;
               }
               
               
               if(itr->num_blks <= 0){
                   db_way.erase(itr);
                   num_users--;
               }
                                  
            }
            else{
               printf("Invalid user item delete block\n");
               return -1;
            }
            
         
        }
        else if(type == 2){
            vector<db_way_blk>::iterator itr;
            vector<db_blk>::iterator itrs, itr_start, itr_end;
            for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
            }

            if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == blk->ord_id)
                    break;
                  index++;   
               }

               if(itrs != itr_end){
                  itr->db_set.erase(itrs);
                  //db_hits++; 
                  itr->num_blks--;
                  db_filled--; 
               }
               else{
                  printf("Invalid order item delete block\n");
                  return -1;
               }
               
               
               if(itr->num_blks <= 0){
                  db_way.erase(itr);
                  num_users--;
               }
               	                   
            }
            else{
               printf("Invalid order item delete block\n");
               return -1;
            }
            
         
        }

        db_acc++;
        return 0;
    }

    db_blk* search_db_blk(int64_t type, db_blk* blk){
        db_blk* new_blk = new db_blk;
        if(type == 0){
           vector<db_blk>::iterator itr;
           for(itr = db_way[0].db_set.begin(); itr < db_way[0].db_set.end() && db_way[0].num_blks > 0; itr++){
               if(itr->food_id == blk->food_id)
                  break;
            }

           if(itr != db_way[0].db_set.end()){
               insert_food_item((db_blk*)&(*itr), new_blk);
               new_blk->dirty = 0;
               return new_blk;
           }
           else{
              printf("Invalid food item search block");
              new_blk->dirty = 1;
              return new_blk;
              
           }
           
        }
        else if(type == 1){
           vector<db_way_blk>::iterator itr;
           vector<db_blk>::iterator itrs, itr_start, itr_end;
           for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
           }
           if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->type == blk->type)
                    break;
                  index++;   
               }
             
               if(itrs != itr_end){
                  insert_user_item((db_blk*)&(*itrs), new_blk);
                  new_blk->dirty = 0;
                  return new_blk;
               }
               else{
                  printf("Invalid user item search block");
                  new_blk->dirty = 1;
                  return new_blk;
               }


           }
           else{
              printf("Invalid user item search block");
              new_blk->dirty = 1;
              return new_blk;
           }
        }
        else if(type == 2){
           vector<db_way_blk>::iterator itr;
           vector<db_blk>::iterator itrs, itr_start, itr_end;
           for(itr = db_way.begin(); itr < db_way.end(); itr++){
               if(itr->username == blk->username)
                  break;
           }
           if(itr != db_way.end()){
               itr_start = itr->db_set.begin();
               itr_end = itr->db_set.end();            

               int index = 0;
               for(itrs =  itr_start; itrs < itr_end; itrs++){
                  if(itrs->ord_id == blk->ord_id)
                    break;
                  index++;   
               }
             
               if(itrs != itr_end){
                  insert_user_item((db_blk*)&(*itrs), new_blk);
                  new_blk->dirty = 0;
                  return new_blk;
               }
               else{
                  printf("Invalid order item search block");
                  new_blk->dirty = 1;
                  return new_blk;
               }


           }
           else{
              printf("Invalid order item search block");
              new_blk->dirty = 1;
              return new_blk;
           }
        } 
  
    }

    void print_db_blk(){

       vector<db_way_blk>::iterator itr, itr_start, itr_end;
       vector<db_blk>::iterator itrs, itr_starts, itr_ends;
       itr_start = db_way.begin();
       itr_end = db_way.end();
       
       int way_indx = 0, set_indx = 0;
       for(itr = itr_start; itr < itr_end; itr++){
          itr_starts = itr->db_set.begin();
          itr_ends = itr->db_set.end();
          printf("-------------------------------------------------------------------------\n");
          printf("Database Way Index: %d\n", way_indx);
          printf("Username: %s\n", itr->username.c_str());
          printf("Num blocks: %ld\n", itr->num_blks);
          printf("-------------------------------------------------------------------------\n");
          for(itrs = itr_starts; itrs < itr_ends; itrs++){
	      if(itrs->type == 0){
                  printf("Food Item: %s %ld\n", itrs->food_name.c_str(), itrs->food_id); 
              }
              else if(itrs->type == 1){
                  printf("User Item: %s\n", itrs->username.c_str());
              }
              else if(itrs->type == 2){
                  printf("Order Item: %ld\n", itrs->ord_id);
              }
              set_indx++;
          }
          printf("\n");
	  way_indx++;
       }

    }          

  private:
    void insert_food_item(db_blk* blk, db_blk* new_blk){
        
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
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

    void insert_user_item(db_blk* blk, db_blk* new_blk){
        
        new_blk->username = blk->username;
        new_blk->email = blk->email;
        new_blk->passwd = blk->passwd;
        new_blk->first_name = blk->first_name;
        new_blk->last_name = blk->last_name;
        new_blk->home_addr = blk->home_addr;
        new_blk->phone_num = blk->phone_num;
        new_blk->type = 1;
        //new_blk->dirty = false;
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

    void insert_order_item(db_blk* blk, db_blk* new_blk){
        
        new_blk->username = blk->username;
        new_blk->ord_id = blk->ord_id;
        new_blk->food_id = blk->food_id;
        new_blk->food_qty = blk->food_qty;
        new_blk->type = 2;
        //new_blk->dirty = false;
        //new_blk->lfu_cnt = 1;     
        //return new_blk;  
    }

};



























 

