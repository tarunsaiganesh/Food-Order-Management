#include "cache.hpp"
using namespace std;

int main(){
    
    cache_blk new_blk;
    Cache* cache_obj = new Cache(); 
    
    int req_type, blk_type;
    string* food = new string; 
    while(1){
        printf("Enter type of request and cache block:\n");
        cin >> req_type;
        //printf("%d\n", req_type);
        cin >> blk_type;
        if(blk_type == 0){
           printf("Food ID: \n");
           cin >> new_blk.food_id;
           printf("Food name: \n");
           //cin >> new_blk.food_name;
           cin >> *food; 
           printf("123");
           cout << *food;
        }
        else if(blk_type == 1){
           printf("Username: \n");
           cin >> new_blk.username;
           printf("Password: \n");
           cin >> new_blk.passwd;
        }
        else if(blk_type == 2){
           printf("Username: \n");
           cin >> new_blk.username;
           printf("Order ID: \n");
           cin >> new_blk.ord_id;
        }
        else{
           printf("Invalid type. Choose from 0, 1 or 2");
           return 0;
        }
       
        if(req_type == 0)
           cache_obj->insert_cache_blk(blk_type, &new_blk);
        else if(req_type == 1)
           cache_obj->update_cache_blk(blk_type, &new_blk);
        else if(req_type == 2)
           cache_obj->delete_cache_blk(blk_type, &new_blk);
        else{
           printf("Invalid request type");
           return 0;
        }
        
        cache_obj->print_cache_blk();
    }
    return 0;
}
