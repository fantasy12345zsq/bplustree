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
 
    return 0;
}
