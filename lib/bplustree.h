#ifndef _BPLUS_TREE_H
#define _BPLUS_TREE_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>


#define  MIN_CACHE_NUM 5

typedef int key_t;
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
typedef struct bplus_node{
    off_t self;
    off_t parent;
    off_t prev;
    off_t next;
    int type;
    int children;
}bplus_node;

struct bplus_tree{
    char* caches;
    int used[MIN_CACHE_NUM];
    int level;
    off_t root;
    off_t file_size;
};

class bptree{
    public:
      bptree(const char *filename,int blocksize);
      ~bptree();
      int init();
      int bpopen();
      void bpclose();
      int insert(key_t key,long data);
      int remove(key_t key,long data);
      void print();
      int get_level();
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

    private:
       struct bplus_tree *tree;
       int fd;
       char filename[1024];
};
#endif /* _BPLUS_TREE_H  */