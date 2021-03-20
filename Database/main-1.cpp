#include "database.hpp"
using namespace std;
#define SIZE 10

int main(){
    
    db_blk new_blk;
    Database* db_obj = new Database(); 
    
    int req_type, blk_type;
    std::string food; 
    string x;
    db_blk test[SIZE];
    int reqs[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
      reqs[i] = 0;
      test[i].food_id = 1000+(i/3);
      test[i].food_name = "item"+to_string(i%3);
      test[i].username = "user"+to_string(i%3);
      test[i].passwd = "pass"+to_string(i);
      test[i].ord_id = 200 + (i%3);
      test[i].food_qty = 10;
      test[i].food_pr = (30+i);
      test[i].type = i%3;
    }
    for(int i=0; i< SIZE; i++){
        db_blk new_blk; 
        printf("1\n");
        blk_type = test[i].type;
        req_type = reqs[i];
        printf("2\n");
        if(blk_type == 0){
            printf("3\n");
           new_blk.food_id  = test[i].food_id;
           new_blk.food_name = test[i].food_name;
           new_blk.food_pr = test[i].food_pr;
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
           new_blk.food_qty = test[i].food_qty;
        }
        else{
            printf("6\n");
           printf("Invalid type. Choose from 0, 1 or 2");
           return 0;
        }
        printf("7\n");
        // printf("%s\n", new_blk.username.c_str());
        // printf("%ld\n", new_blk.ord_id);
        // printf("%d\n", blk_type);
        printf("%ld\n", new_blk.food_pr);
        if(req_type == 0){
           printf("8\n");
           db_obj->insert_db_blk(blk_type, &new_blk);
        }
        else if(req_type == 1){
           printf("9\n");
           db_obj->update_db_blk(blk_type, &new_blk);
        }
        else if(req_type == 2){
           printf("10\n");
           db_obj->delete_db_blk(blk_type, &new_blk);
        }
        else{
           printf("Invalid request type");
           return 0;
        }
        
        db_obj->print_db_blk();
    }
    return 0;
}
