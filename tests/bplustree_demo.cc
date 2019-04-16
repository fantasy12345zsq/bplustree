////
// @file bplustree_demo.cc
// @brief
// 实现测试
//
// @author zhangsiqi
// @email 1575033031@qq.com
//
#include "bplustree.h"

//测试
int main(int argc,char ** argv)
{
    const char* filename = "/tmp/data_test.index";
    int blocksize = 128;
    bptree b1;
    bool again = false;
    while(!again)
    {
        again = b1.init(filename,blocksize);
    }

    b1.handle_command();
    
    b1.deinit();


    //test
    // b1.insert(12,12);
    // b1.insert(23,23);
    // b1.insert(34,34);
    // b1.insert(45,45);
    // b1.insert(56,56);
    // b1.insert(67,67);
    // b1.insert(78,78);
    // b1.insert(89,89);
    // b1.insert(21,21);
    // b1.insert(32,32);
    // b1.insert(43,43);
    // b1.insert(54,54);
    // b1.insert(65,65);
    // b1.insert(87,87);
    // b1.insert(98,98);
    // b1.insert(100,100);
    // b1.insert(24,24);
    // b1.insert(35,35);
    // b1.insert(36,36);
    // b1.insert(40,40);
    // b1.insert(150,150);
    // b1.insert(90,90);
    // b1.insert(200,200);
    // b1.insert(160,160);
    // b1.insert(50,50);
    // b1.insert(110,110);
    // b1.insert(120,120);
    // b1.insert(130,130);
    // b1.insert(140,140);
    // b1.insert(91,91);
    // b1.insert(92,92);
    // b1.insert(93,93);
    // b1.insert(88,88);

    // b1.print();
 
    return 0;
}
