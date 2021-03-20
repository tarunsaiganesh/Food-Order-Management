#include "only_db.hpp"
#include <ctime>
using namespace std;
#define SIZE 10
#define ONLY_DB 1

int main(){
    
    cache_blk new_blk;
    Cache* cache_obj = new Cache(); 
    clock_t time_start, time_end;
    int req_type, blk_type;
    std::string food; 
    string x;
    cache_blk test[SIZE];
    int reqs[SIZE];
    time(&time_start);

    //test case  1
  /*  for (int i = 0; i < SIZE; i++)
    {
      reqs[i] = 0;
      test[i].food_id = 1000+i;
      test[i].food_name = "item"+to_string(1000+i);
      test[i].username = "user"+to_string(i);
      test[i].passwd = "pass"+to_string(i);
      test[i].ord_id =  200 + (i);
      test[i].food_qty = 15+i;
      test[i].food_pr = (i*33);
      test[i].type = i%3;
    }*/

    //test case 2
/*    reqs[0] = 0;
    test[0].food_id = 1000;
    test[0].food_name = "item1000";
    test[0].username = "user0";
    test[0].passwd = "pass0";
    test[0].ord_id = 1;
    test[0].food_qty = 10;
    test[0].food_pr = 116;
    test[0].type = 0; 

    reqs[1] = 0;
    test[1].food_id = 1001;
    test[1].food_name = "item1001";
    test[1].username = "user1";
    test[1].passwd = "pass1";
    test[1].ord_id = 2;
    test[1].food_qty = 15;
    test[1].food_pr = 216;
    test[1].type = 1;

    reqs[2] = 0;
    test[2].food_id = 1000;
    test[2].food_name = "item1000";
    test[2].username = "user2";
    test[2].passwd = "pass2";
    test[2].ord_id = 1;
    test[2].food_qty = 10;
    test[2].food_pr = 116;
    test[2].type = 1; 

    reqs[3] = 0;
    test[3].food_id = 1000;
    test[3].food_name = "item1002";
    test[3].username = "user1";
    test[3].passwd = "pass1";
    test[3].ord_id = 2;
    test[3].food_qty = 10;
    test[3].food_pr = 116;
    test[3].type = 2;

    reqs[4] = 0;
    test[4].food_id = 1001;
    test[4].food_name = "item1001";
    test[4].username = "user1";
    test[4].passwd = "pass1";
    test[4].ord_id = 3;
    test[4].food_qty = 10;
    test[4].food_pr = 116;
    test[4].type = 2;

    reqs[5] = 1;
    test[5].food_id = 1002;
    test[5].food_name = "item1002";
    test[5].username = "user1";
    test[5].passwd = "pass1";
    test[5].ord_id = 2;
    test[5].food_qty = 10;
    test[5].food_pr = 333;
    test[5].type = 2;

    reqs[6] = 1;
    test[6].food_id = 1003;
    test[6].food_name = "item1003";
    test[6].username = "user1";
    test[6].passwd = "pass1";
    test[6].ord_id = 2;
    test[6].food_qty = 10;
    test[6].food_pr = 444;
    test[6].type = 2;

    reqs[7] = 1;
    test[7].food_id = 1004;
    test[7].food_name = "item1004";
    test[7].username = "user1";
    test[7].passwd = "pass1";
    test[7].ord_id = 3;
    test[7].food_qty = 10;
    test[7].food_pr = 555;
    test[7].type = 2;

    reqs[8] = 1;
    test[8].food_id = 1000;
    test[8].food_name = "item1000";
    test[8].username = "user1";
    test[8].passwd = "pass1";
    test[8].ord_id = 3;
    test[8].food_qty = 33;
    test[8].food_pr = 666;
    test[8].type = 2;

    reqs[9] = 1;
    test[9].food_id = 1004;
    test[9].food_name = "item1004";
    test[9].username = "user1";
    test[9].passwd = "pass1";
    test[9].ord_id = 3;
    test[9].food_qty = 33;
    test[9].food_pr = 777;
    test[9].type = 2;
*/
    //test case 3
/*    reqs[0] = 0;
    test[0].food_id = 1000;
    test[0].food_name = "item1000";
    test[0].username = "user0";
    test[0].passwd = "pass0";
    test[0].ord_id = 1;
    test[0].food_qty = 10;
    test[0].food_pr = 116;
    test[0].type = 0; 

    reqs[1] = 0;
    test[1].food_id = 1001;
    test[1].food_name = "item1001";
    test[1].username = "user1";
    test[1].passwd = "pass1";
    test[1].ord_id = 2;
    test[1].food_qty = 15;
    test[1].food_pr = 216;
    test[1].type = 1;

    reqs[2] = 0;
    test[2].food_id = 1000;
    test[2].food_name = "item1000";
    test[2].username = "user2";
    test[2].passwd = "pass2";
    test[2].ord_id = 1;
    test[2].food_qty = 10;
    test[2].food_pr = 116;
    test[2].type = 1; 

    reqs[3] = 0;
    test[3].food_id = 1010;
    test[3].food_name = "item1010";
    test[3].username = "user1";
    test[3].passwd = "pass1";
    test[3].ord_id = 2;
    test[3].food_qty = 10;
    test[3].food_pr = 116;
    test[3].type = 0;

    reqs[4] = 0;
    test[4].food_id = 1001;
    test[4].food_name = "item1001";
    test[4].username = "user1";
    test[4].passwd = "pass1";
    test[4].ord_id = 4;
    test[4].food_qty = 24;
    test[4].food_pr = 116;
    test[4].type = 2;

    reqs[5] = 0;
    test[5].food_id = 1002;
    test[5].food_name = "item1002";
    test[5].username = "user2";
    test[5].passwd = "pass2";
    test[5].ord_id = 6;
    test[5].food_qty = 10;
    test[5].food_pr = 333;
    test[5].type = 2;

    reqs[6] = 1;
    test[6].food_id = 1010;
    test[6].food_name = "item1010";
    test[6].username = "user1";
    test[6].passwd = "pass1";
    test[6].ord_id = 2;
    test[6].food_qty = 66;
    test[6].food_pr = 444;
    test[6].type = 0;

    reqs[7] = 1;
    test[7].food_id = 1004;
    test[7].food_name = "item1004";
    test[7].username = "user2";
    test[7].passwd = "pass22";
    test[7].ord_id = 3;
    test[7].food_qty = 10;
    test[7].food_pr = 555;
    test[7].type = 1;

    reqs[8] = 2;
    test[8].food_id = 1000;
    test[8].food_name = "item1000";
    test[8].username = "user1";
    test[8].passwd = "pass1";
    test[8].ord_id = 3;
    test[8].food_qty = 33;
    test[8].food_pr = 666;
    test[8].type = 0;

    reqs[9] = 2;
    test[9].food_id = 1004;
    test[9].food_name = "item1004";
    test[9].username = "user1";
    test[9].passwd = "pass1";
    test[9].ord_id = 4;
    test[9].food_qty = 33;
    test[9].food_pr = 777;
    test[9].type = 2;
*/
    //test case 4
/*    reqs[0] = 0;
    test[0].food_id = 1000;
    test[0].food_name = "item1000";
    test[0].username = "user0";
    test[0].passwd = "pass0";
    test[0].ord_id = 1;
    test[0].food_qty = 10;
    test[0].food_pr = 116;
    test[0].type = 0; 

    reqs[1] = 0;
    test[1].food_id = 1001;
    test[1].food_name = "item1001";
    test[1].username = "user1";
    test[1].passwd = "pass1";
    test[1].ord_id = 2;
    test[1].food_qty = 15;
    test[1].food_pr = 216;
    test[1].type = 1;

    reqs[2] = 0;
    test[2].food_id = 1000;
    test[2].food_name = "item1000";
    test[2].username = "user2";
    test[2].passwd = "pass2";
    test[2].ord_id = 1;
    test[2].food_qty = 10;
    test[2].food_pr = 116;
    test[2].type = 1; 

    reqs[3] = 0;
    test[3].food_id = 1010;
    test[3].food_name = "item1010";
    test[3].username = "user3";
    test[3].passwd = "pass3";
    test[3].ord_id = 2;
    test[3].food_qty = 10;
    test[3].food_pr = 116;
    test[3].type = 1;

    reqs[4] = 0;
    test[4].food_id = 1010;
    test[4].food_name = "item1010";
    test[4].username = "user1";
    test[4].passwd = "pass1";
    test[4].ord_id = 4;
    test[4].food_qty = 24;
    test[4].food_pr = 116;
    test[4].type = 0;

    reqs[5] = 2;
    test[5].food_id = 1002;
    test[5].food_name = "item1002";
    test[5].username = "user1";
    test[5].passwd = "pass1";
    test[5].ord_id = 6;
    test[5].food_qty = 10;
    test[5].food_pr = 333;
    test[5].type = 1;

    reqs[6] = 2;
    test[6].food_id = 1000;
    test[6].food_name = "item1000";
    test[6].username = "user1";
    test[6].passwd = "pass1";
    test[6].ord_id = 2;
    test[6].food_qty = 66;
    test[6].food_pr = 444;
    test[6].type = 0;

    reqs[7] = 2;
    test[7].food_id = 1004;
    test[7].food_name = "item1004";
    test[7].username = "user3";
    test[7].passwd = "pass3";
    test[7].ord_id = 3;
    test[7].food_qty = 10;
    test[7].food_pr = 555;
    test[7].type = 1;

    reqs[8] = 2;
    test[8].food_id = 1010;
    test[8].food_name = "item1010";
    test[8].username = "user1";
    test[8].passwd = "pass1";
    test[8].ord_id = 3;
    test[8].food_qty = 33;
    test[8].food_pr = 666;
    test[8].type = 0;

    reqs[9] = 2;
    test[9].food_id = 1004;
    test[9].food_name = "item1004";
    test[9].username = "user2";
    test[9].passwd = "pass2";
    test[9].ord_id = 4;
    test[9].food_qty = 33;
    test[9].food_pr = 777;
    test[9].type = 1;
*/
    //test case 5
/*    reqs[0] = 0;
    test[0].food_id = 1000;
    test[0].food_name = "item1000";
    test[0].username = "user0";
    test[0].passwd = "pass0";
    test[0].ord_id = 1;
    test[0].food_qty = 10;
    test[0].food_pr = 116;
    test[0].type = 0; 

    reqs[1] = 0;
    test[1].food_id = 1001;
    test[1].food_name = "item1001";
    test[1].username = "user1";
    test[1].passwd = "pass1";
    test[1].ord_id = 2;
    test[1].food_qty = 15;
    test[1].food_pr = 216;
    test[1].type = 1;

    reqs[2] = 0;
    test[2].food_id = 1000;
    test[2].food_name = "item1000";
    test[2].username = "user1";
    test[2].passwd = "pass1";
    test[2].ord_id = 3;
    test[2].food_qty = 10;
    test[2].food_pr = 116;
    test[2].type = 2; 

    reqs[3] = 1;
    test[3].food_id = 1010;
    test[3].food_name = "item1010";
    test[3].username = "user1";
    test[3].passwd = "pass11";
    test[3].ord_id = 2;
    test[3].food_qty = 10;
    test[3].food_pr = 116;
    test[3].type = 1;

    reqs[4] = 1;
    test[4].food_id = 1000;
    test[4].food_name = "item1000";
    test[4].username = "user1";
    test[4].passwd = "pass1";
    test[4].ord_id = 4;
    test[4].food_qty = 24;
    test[4].food_pr = 116;
    test[4].type = 0;

    reqs[5] = 1;
    test[5].food_id = 1002;
    test[5].food_name = "item1002";
    test[5].username = "user1";
    test[5].passwd = "pass11";
    test[5].ord_id = 3;
    test[5].food_qty = 44;
    test[5].food_pr = 333;
    test[5].type = 2;

    reqs[6] = 1;
    test[6].food_id = 1000;
    test[6].food_name = "item1000";
    test[6].username = "user1";
    test[6].passwd = "pass111";
    test[6].ord_id = 2;
    test[6].food_qty = 66;
    test[6].food_pr = 444;
    test[6].type = 1;

    reqs[7] = 1;
    test[7].food_id = 1004;
    test[7].food_name = "item1004";
    test[7].username = "user1";
    test[7].passwd = "pass111";
    test[7].ord_id = 3;
    test[7].food_qty = 345;
    test[7].food_pr = 555;
    test[7].type = 2;

    reqs[8] = 3;
    test[8].food_id = 1000;
    test[8].food_name = "item1000";
    test[8].username = "user1";
    test[8].passwd = "pass1";
    test[8].ord_id = 3;
    test[8].food_qty = 33;
    test[8].food_pr = 666;
    test[8].type = 0;

    reqs[9] = 3;
    test[9].food_id = 1004;
    test[9].food_name = "item1004";
    test[9].username = "user1";
    test[9].passwd = "pass111";
    test[9].ord_id = 3;
    test[9].food_qty = 33;
    test[9].food_pr = 777;
    test[9].type = 2;
*/    

    //test case 6
    reqs[0] = 0;
    test[0].food_id = 1000;
    test[0].food_name = "item1000";
    test[0].username = "user0";
    test[0].passwd = "pass0";
    test[0].ord_id = 1;
    test[0].food_qty = 10;
    test[0].food_pr = 116;
    test[0].type = 0; 

    reqs[1] = 0;
    test[1].food_id = 1001;
    test[1].food_name = "item1001";
    test[1].username = "user1";
    test[1].passwd = "pass1";
    test[1].ord_id = 2;
    test[1].food_qty = 15;
    test[1].food_pr = 216;
    test[1].type = 1;

    reqs[2] = 0;
    test[2].food_id = 1000;
    test[2].food_name = "item1000";
    test[2].username = "user2";
    test[2].passwd = "pass2";
    test[2].ord_id = 3;
    test[2].food_qty = 10;
    test[2].food_pr = 116;
    test[2].type = 1; 

    reqs[3] = 0;
    test[3].food_id = 1010;
    test[3].food_name = "item1010";
    test[3].username = "user3";
    test[3].passwd = "pass3";
    test[3].ord_id = 2;
    test[3].food_qty = 10;
    test[3].food_pr = 116;
    test[3].type = 1;

    reqs[4] = 0;
    test[4].food_id = 1010;
    test[4].food_name = "item1010";
    test[4].username = "user1";
    test[4].passwd = "pass1";
    test[4].ord_id = 4;
    test[4].food_qty = 24;
    test[4].food_pr = 116;
    test[4].type = 0;

    reqs[5] = 0;
    test[5].food_id = 1002;
    test[5].food_name = "item1002";
    test[5].username = "user1";
    test[5].passwd = "pass1";
    test[5].ord_id = 5;
    test[5].food_qty = 10;
    test[5].food_pr = 333;
    test[5].type = 2;

    reqs[6] = 3;
    test[6].food_id = 1010;
    test[6].food_name = "item1010";
    test[6].username = "user1";
    test[6].passwd = "pass111";
    test[6].ord_id = 2;
    test[6].food_qty = 66;
    test[6].food_pr = 444;
    test[6].type = 0;

    reqs[7] = 1;
    test[7].food_id = 1004;
    test[7].food_name = "item1004";
    test[7].username = "user1";
    test[7].passwd = "pass11";
    test[7].ord_id = 3;
    test[7].food_qty = 345;
    test[7].food_pr = 555;
    test[7].type = 1;

    reqs[8] = 2;
    test[8].food_id = 1000;
    test[8].food_name = "item1000";
    test[8].username = "user3";
    test[8].passwd = "pass3";
    test[8].ord_id = 3;
    test[8].food_qty = 33;
    test[8].food_pr = 666;
    test[8].type = 1;

    reqs[9] = 1;
    test[9].food_id = 1000;
    test[9].food_name = "item1000";
    test[9].username = "user1";
    test[9].passwd = "pass111";
    test[9].ord_id = 5;
    test[9].food_qty = 123;
    test[9].food_pr = 777;
    test[9].type = 2;

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
           new_blk.food_pr = test[i].food_pr;
           new_blk.food_qty = test[i].food_qty;
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
           //printf("qt %ld\n", new_blk.food_qty);
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
        else if(req_type == 3){
            continue;
        }
        else{
           printf("Invalid request type");
           return 0;
        }
        
        //cache_obj->print_cache_blk();
        // printf("cache accesses: %ld\n", cache_obj->cache_acc);
    }
    time(&time_end);
    double time_in_sec = ((double)(time_end-time_start));
    time_in_sec = time_in_sec; //- cache_obj->sleep_cnt*2;
    printf("Time in sec %lf\n", time_in_sec);
    //printf("miss rate: %f\n", (float)(cache_obj->cache_acc-cache_obj->cache_hits)/(float)cache_obj->cache_acc);
    return 0;
}
