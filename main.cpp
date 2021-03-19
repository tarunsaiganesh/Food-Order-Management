#include "cache.hpp"
using namespace std;
#define SIZE 10

int main(){
    
    cache_blk new_blk;
    Cache* cache_obj = new Cache(); 
    
    int req_type, blk_type;
    std::string food; 
    string x;
    cache_blk test[SIZE];
    int reqs[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
      reqs[i] = 0;
      test[i].food_id = 1000+i;
      test[i].food_name = "item"+to_string(i);
      test[i].username = "user"+to_string(i);
      test[i].passwd = "pass"+to_string(i);
      test[i].ord_id = 200 + (i);
      test[i].type = 2;
    }
    for(int i=0; i< SIZE; i++){
        cache_blk new_blk; 
        printf("1\n");
        blk_type = test[i].type;
        req_type = reqs[i];
        printf("2\n");
        if(blk_type == 0){
            printf("3\n");
           new_blk.food_id  = test[i].food_id;
           new_blk.food_name = test[i].food_name;
        }
        else if(blk_type == 1){
           printf("4\n");
           new_blk.username = test[i].username;
           new_blk.passwd = test[i].passwd;
        }
        else if(blk_type == 2){
           printf("5\n");
           new_blk.username = test[i].username;
           new_blk.ord_id = test[i].ord_id;
        }
        else{
            printf("6\n");
           printf("Invalid type. Choose from 0, 1 or 2");
           return 0;
        }
        printf("7\n");
        printf("%s\n", new_blk.username.c_str());
        printf("%ld\n", new_blk.ord_id);
        printf("%d\n", blk_type);
        if(req_type == 0){
           printf("8\n");
           cache_obj->insert_cache_blk(blk_type, &new_blk);
        }
        else if(req_type == 1){
           printf("9\n");
           cache_obj->update_cache_blk(blk_type, &new_blk);
        }
        else if(req_type == 2){
           printf("10\n");
           cache_obj->delete_cache_blk(blk_type, &new_blk);
        }
        else{
           printf("Invalid request type");
           return 0;
        }
        
        cache_obj->print_cache_blk();
    }
    return 0;
}
