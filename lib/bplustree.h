////
// @file bplustree.h
// @brief
// 实现B+树头文件
//
// @author zhangsiqi
// @email 1575033031@qq.com
//
#ifndef _BPLUS_TREE_H
#define _BPLUS_TREE_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <list>


#define  MIN_CACHE_NUM 5
#define ADDR_STR_WIDTH 16

typedef int key_t; 
typedef long data_t;

enum{
    INVALID_OFFSET = 0xdeadbeef,
};

enum{
    BPLUS_TREE_LEAF,
    BPLUS_TREE_NON_LEAF = 1,
};
enum{
    LEFT_SLIBING,
    RIGHT_SLIBING = 1,
};
static int _max_entries;   //通过_block_size计算叶子节点的最大存放节点数
static int _max_order;

//对于叶子节点来说，存放key和data
//对于非叶子节点来说，存放key和sub，sub代表孩子节点的偏移
#define offset_ptr(node) ((char*)(node) + sizeof(*node))
#define key(node) ((key_t *)offset_ptr(node))
#define data(node) ((data_t *)(offset_ptr(node) + _max_entries * sizeof(key_t)))
#define sub(node) ((off_t *)(offset_ptr(node) + (_max_order - 1) * sizeof(key_t)))


//block定义
typedef struct bplus_node{
    off_t self;        //自身偏移
    off_t parent;      //父节点偏移 TODO: 建议不要parent
    off_t prev;        //前驱节点偏移
    off_t next;        //后继节点偏移
    int type;          //节点类型（叶子 or 非叶子）
    int children;      //节点孩子个数
}block;

struct bplus_tree{
    char* caches;               //从磁盘读出节点的缓存
    int used[MIN_CACHE_NUM];    //标记缓存的使用情况
    int level;                  //层数
    off_t root;                 //根节点偏移
    off_t file_size;            //文件大小
};

class bptree : public block
{
    private:
      struct bplus_tree *tree;
      int fd;
      char filename[1024];        
      int checksum;                //校验和
      int _block_size;             //块大小
      std::list<off_t> free_block; //空闲块链表
    
    public:
      //bptree(const char *filename,int blocksize);
      bptree();
      ~bptree();
      bool init(const char *filename,int blocksize);
      int bpopen(const char *filename);
      void bpclose();
      //插入
      int insert(key_t key,long data);
      //删除
      int remove(key_t key,long data);
      //打印
      void print();
      int get_level();
    public:
      void handle_command();
      void help();
      void deinit();
      void handle(char c);
      void handle_insert();
      void handle_remove();
    private:
      void free_cache(struct bplus_node *node);
      void node_flush(struct bplus_node *node);
      struct bplus_node *node_seek(off_t offset);
      struct bplus_node *get_cache();
      struct bplus_node *node_fetch(off_t offset);
      struct bplus_node *node_new();
      struct bplus_node *non_leaf_new();
      struct bplus_node *leaf_new();
      off_t new_node_add(struct bplus_node *node);
      void add_left_node(struct bplus_node *node,struct bplus_node *left);
      void add_right_node(struct bplus_node *node,struct bplus_node *right);
      key_t leaf_split_left(struct bplus_node *node,struct bplus_node* left,
                            key_t key,long data,int insert);
      key_t leaf_split_right(struct bplus_node *node,struct bplus_node *right,
                             key_t key,long data,int insert);
      key_t non_leaf_split_right2(struct bplus_node *node,
                                struct bplus_node *left,
                                struct bplus_node *l_ch,
                                struct bplus_node *r_ch,
                                key_t key,
                                int insert);
      key_t non_leaf_split_right(struct bplus_node *node,
                                 struct bplus_node *right,
                                 struct bplus_node *l_ch,
                                 struct bplus_node *r_ch,
                                 key_t key,
                                 int insert);
      key_t non_leaf_split_right1(struct bplus_node *node,
                                  struct bplus_node *right,
                                  struct bplus_node *l_ch,
                                  struct bplus_node *r_ch,
                                  key_t key,
                                  int insert);
      int creat_parent_node(struct bplus_node *l_ch,struct bplus_node *r_ch,key_t key);
      int non_leaf_insert(struct bplus_node *node,struct bplus_node *l_ch,
                          struct bplus_node* r_ch,key_t key);
      int leaf_insert(struct bplus_node *node,key_t key ,long data);
      int leaf_delete(struct bplus_node *node,key_t key,long data);
      int select_slib(struct bplus_node *parent,
                      struct bplus_node *l_slib,
                      struct bplus_node *r_slib,
                      int i);
      void merge_with_left_node(struct bplus_node *parent,
                                struct bplus_node *l_slib,
                                struct bplus_node *r_slib,
                                int index);
      int delete_empty_node(struct bplus_node *node,
                            struct bplus_node *l_slib,
                            struct bplus_node *r_slib);
      int non_leaf_delete(struct bplus_node *parent,int i);
      int borrow_from_left_node(struct bplus_node *parent,
                                struct bplus_node *l_slib,
                                struct bplus_node *node,
                                int index); 
      int borrow_from_right_node(struct bplus_node *parent,
                                 struct bplus_node *r_slib,
                                 struct bplus_node *node,
                                 int index);
      int non_leaf_borrow_from_left(struct bplus_node *parent,
                                    struct bplus_node *l_slib,
                                    struct bplus_node *node,
                                    int i);
      int non_leaf_borrow_from_right(struct bplus_node *parent,
                                     struct bplus_node *r_slib,
                                     struct bplus_node *node,
                                     int i); 
      int non_leaf_merge_with_right(struct bplus_node *parent,
                                    struct bplus_node *node,
                                    struct bplus_node *r_slib,
                                    int i);
      int non_leaf_merge_with_left(struct bplus_node *parent,
                                   struct bplus_node *node,
                                   struct bplus_node *l_slib,
                                   int i);
      int non_leaf_simple_delete(struct bplus_node *node,int index);

};
#endif /* _BPLUS_TREE_H  */