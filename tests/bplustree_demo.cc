#include "bplustree.h"

#if  0
//测试
int main(int argc,char ** argv)
{
    const char* filename = "/tmp/data.index";
    int blocksize = 128;
    bptree b1(filename,blocksize);
    b1.init();

    b1.insert(12,12);
    b1.insert(3,3);
    b1.insert(45,45);
    b1.insert(13,13);
    b1.insert(43,43);
    b1.insert(56,56);
    b1.insert(15,15);
    b1.insert(42,42);
    b1.insert(78,78);
    // //printf("insert 96!\n");
    b1.insert(96,96);
    // //printf("insert 120 120!\n");
    b1.insert(120,120);
    // //printf("insert 115!\n");
    b1.insert(115,115);
    b1.insert(100,100);
    b1.insert(110,110);
    b1.insert(108,108);
    // //printf("\n\n\n\n\n\n\n\n\ninsert 112!\n");
    b1.insert(112,112);
    // //printf("\n\n\n\n\n\ninsert 130!\n");
    b1.insert(130,130);
    b1.insert(200,200);
    b1.insert(127,127);
    b1.insert(125,125);
    b1.insert(126,126);
    b1.insert(128,128);
    b1.insert(55,55);
    b1.insert(60,60);
    b1.insert(80,80);
    b1.insert(135,135);
    b1.insert(250,250);
    b1.insert(300,300);
    b1.insert(270,270);
    b1.insert(260,260);
    b1.insert(265,265);
    

    b1.print();
    return 0;
}
#endif

#if 1
//测试
int main(int argc,char ** argv)
{
    const char* filename = "/tmp/data_test.index";
    int blocksize = 128;
    bptree b1(filename,blocksize);
    b1.init();

    b1.insert(12,12);
    b1.insert(23,23);
    b1.insert(45,45);
    b1.insert(56,56);
    b1.insert(78,78);
    b1.insert(89,89);
    b1.insert(36,36);
    b1.insert(58,58);
    b1.insert(49,49);
    b1.insert(5,5);
    b1.insert(1,1);
    b1.insert(4,4);
    b1.insert(30,30);
    b1.insert(7,7);
    b1.insert(10,10);
    b1.insert(6,6);
    b1.insert(15,15);
    b1.insert(14,14);
    b1.insert(18,18);
    b1.insert(17,17);
    b1.insert(90,90);
    b1.insert(100,100);
    b1.insert(20,20);
    b1.insert(25,25);
    b1.insert(40,40);
    b1.insert(28,28);
    b1.insert(35,35);
    b1.insert(31,31);
    b1.insert(32,32);
    b1.insert(29,29);
    b1.insert(11,11);
    b1.insert(120,120);
    b1.insert(200,200);
    b1.insert(8,8);
    b1.insert(9,9);
    b1.insert(13,13);
    b1.insert(33,33);
    b1.insert(19,19);
    b1.insert(27,27);
    b1.insert(26,26);

    b1.print();

    printf("-------------------remove---------------------\n");
    b1.remove(10,10);
    b1.remove(29,29);
    b1.remove(33,33);
    // b1.remove(25,25);



    // printf("\n\n\n\n\n\n\n\nremove 30!\n");
    // b1.remove(30,30);
    // b1.remove(18,18);
    // b1.remove(17,17);
    // b1.remove(6,6);
    // b1.remove(12,12);
    // printf("\n\n\n\n\n\n\n\nremove 10!\n");
    // b1.remove(10,10);
    // b1.remove(100,100);
    // b1.remove(90,90);
    // b1.remove(9,9);
    // b1.remove(89,89);
    // b1.remove(14,14);
    // b1.remove(78,78);
    // b1.remove(5,5);
    // printf("\n\n\n\n\n\n\nremove 4!\n");
    // b1.remove(4,4);
    // b1.remove(32,32);
    // b1.remove(28,28);
    // b1.remove(30,30);
    // b1.remove(27,27);
    // b1.remove(8,8);
    // b1.remove(58,58);
    // b1.remove(20,20);
    // b1.remove(31,31);
    // b1.remove(25,25);
    // b1.remove(19,19);
    // b1.remove(45,45);
    // b1.remove(120,120);
    // b1.remove(56,56);
    // b1.remove(36,36);
    // b1.remove(33,33);
    // b1.remove(200,200);
    // b1.remove(1,1);
    // b1.remove(35,35);
    // b1.remove(49,49);
    // b1.remove(4,4);

    b1.print();
    return 0;
}
#endif 