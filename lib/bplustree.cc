#include "bplustree.h"

static int _block_size = 128;
static int _max_entries = 5;
static int _max_order = 5;

#define offset_ptr(node) ((char*)(node) + sizeof(*node))
#define key(node) ((key_t *)offset_ptr(node))
#define data(node) ((long *)(offset_ptr(node) + _max_entries * sizeof(key_t)))
#define sub(node) ((off_t *)(offset_ptr(node) + (_max_entries - 1) * sizeof(key_t)))

static bool is_leaf(struct bplus_node * node)
{
    if(node->type == BPLUS_TREE_LEAF)
    {
        return true;
    }else{
        return false;
    }
}
//二分查找插入位置
static int key_insert_location(struct bplus_node *node,key_t key)
{
    //printf("key_insert_location!\n");
    key_t *arr = key(node);
    int low = -1;
    int len = is_leaf(node) ? node->children : node->children - 1;
    int high = len;

    // printf("high = %d\n",high);
    // printf("key = %d\n",key);
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    while(low + 1 < high)
    {
        int mid = (low + high) / 2;
        if(key > arr[mid])
        {
            low = mid;
        }else{
            high = mid;
        }
    }

    if(high >= len || arr[high] != key)
    {
        //                                                                                                                           printf("-high-1 = %d\n",-high-1);
        return -high-1;
    }else{
        return high;
    }
}
static int index_in_parent(struct bplus_node *node,key_t key)
{
    int index = key_insert_location(node,key);
    //printf("index = %d\n",index);
    return index >=  0 ? index : -index - 2;
}
static void leaf_simple_insert(struct bplus_node * node,key_t key,long data,int insert)
{
    // printf("leaf_simple_insert!\n");
    // printf("node->self = %ld\n",node->self);
    //memmove(void* dest,const void *src,size_t count)
    memmove(&(key(node)[insert + 1]),&key(node)[insert],
           (node->children - insert) * sizeof(key_t));
    memmove(&data(node)[insert + 1],&data(node)[insert],
           (node->children - insert) * sizeof(long));
    key(node)[insert] = key;
    data(node)[insert] = data;
    node->children++;
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
}
static void leaf_simple_delete(struct bplus_node *node,key_t key,long data,int index)
{
    // printf("leaf_simple_delete!\n");
    // printf("index = %d\n",index);

    memmove(&key(node)[index],&key(node)[index + 1],
            (_max_entries - index - 1) * sizeof(key_t ));
    memmove(&data(node)[index],&data(node)[index + 1],
            (_max_entries - index  -1) * sizeof(long));

    node->children--;

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d]  =%d\n",i,key(node)[i]);
    // }
}
static void non_leaf_simple_insert(struct bplus_node* node,
                                    struct bplus_node* l_ch,
                                    struct bplus_node* r_ch,
                                    key_t key,
                                    int insert)
{
    // printf("non_leaf_simple_insert!\n");
    // printf("node->self = %ld,l_ch = %ld,r_ch = %ld\n",
    //      node->self,l_ch->self,r_ch->self);
    // printf("insert = %d\n",insert);
    // printf("node->children = %d\n",node->children);

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    memmove(&key(node)[insert + 1],&key(node)[insert],
           (node->children - insert -1) * sizeof(key_t));

    memmove(&sub(node)[insert + 2],&sub(node)[insert + 1],
           (node->children - insert -1) * sizeof(off_t));

    key(node)[insert] = key;
    node->children++;
    sub(node)[insert] = l_ch->self;
    sub(node)[insert + 1] = r_ch -> self;
    l_ch -> parent = node ->self;
    r_ch -> parent = node ->self;
    r_ch -> prev = l_ch ->self;
    l_ch -> next = r_ch ->self;

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
}
key_t bptree::non_leaf_split_right1(struct bplus_node *node,
                                    struct bplus_node *right,
                                    struct bplus_node *l_ch,
                                    struct bplus_node *r_ch,
                                    key_t key,
                                    int insert)
{
    // printf("non_leaf_split_right1!\n");
    // printf("node->self = %ld,l_ch->self = %ld,r_ch->self = %ld\n",
    //      node->self,l_ch->self,r_ch->self);
    // printf("key = %d\n",key);

    add_right_node(node,right);


    // printf("right->self = %ld\n",right->self);
    // if(right->next != INVALID_OFFSET)
    //     printf("right->next = %ld\n",right->next);
    // printf("l_ch->next = %ld\n",l_ch->next);

    // for(int i = 0; i < MIN_CACHE_NUM; i++)
    // {
    //     printf("tree->used[%d] = %d\n",i,tree->used[i]);
    // }

    //调整分裂后两个节点的sub
    int split = (_max_entries + 1) / 2;
    key_t split_key = key;

    node->children = split + 1;
    right->children = _max_entries - split;
    // printf("node->children = %d,right->children = %d\n",
    //        node->children,right->children);
    memmove(&key(right)[0],&key(node)[split],
           (_max_entries - split) * sizeof(key_t));

    sub(right)[0] = r_ch->self;
    r_ch->parent = right->self;
    memmove(&sub(right)[1],&sub(node)[split + 1],
            (_max_entries - split) * sizeof(off_t));

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < right->children - 1; i++)
    // {
    //     printf("key(right)[%d] = %d\n",i,key(right)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
    // for(int i = 0; i < right->children; i++)
    // {
    //     printf("sub(right)[%d] = %ld\n",i,sub(right)[i]);
    // }
    node_flush(l_ch);
    node_flush(r_ch);

    for(int i = 1; i < right->children; i++)
    {
        struct bplus_node *temp = node_fetch(sub(node)[split + i]);
        temp->parent = right->self;
        //printf("temp->self = %ld\n",temp->self);
        node_flush(temp);
    }

    return split_key;
}
key_t bptree::non_leaf_split_right(struct bplus_node *node,
                                   struct bplus_node *right,
                                   struct bplus_node *l_ch,
                                   struct bplus_node *r_ch,
                                   key_t key,
                                   int insert)
{
    //printf("non_leaf_split_right!\n");
    add_right_node(node,right);

    // printf("node->self = %ld,right->self = %ld, l_ch->self = %ld,r_ch->self = %ld\n",
    //        node->self,right->self,l_ch->self,r_ch->self);

    //调整分裂后两个节点的sub
    int split = (_max_entries + 1) / 2;
    int privot  = insert - split - 1;
    key_t split_key = key(node)[split];

    node ->children = split + 1;
    right->children = _max_entries - split;

    memmove(&key(right)[0],&key(node)[split + 1],
           privot * sizeof(key_t));
    memmove(&sub(right)[0],&sub(node)[split + 1],
            privot * sizeof(off_t));

    key(right)[privot] = key;

    memmove(&key(right)[privot + 1],&key(node)[insert],
           (_max_entries - insert) * sizeof(key_t));
    memmove(&sub(right)[privot + 2],&sub(node)[insert + 1],
           (_max_entries - insert) * sizeof(off_t));

    l_ch->parent = right->self;
    r_ch->parent = right->self;
    sub(right)[privot] = l_ch->self;
    sub(right)[privot + 1] = r_ch->self;

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
    // for(int i = 0; i < right->children; i++)
    // {
    //     printf("sub(right)[%d] = %ld\n",i,sub(right)[i]);
    // }
    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < right->children - 1; i++)
    // {
    //     printf("key(right)[%d] = %d\n",i,key(right)[i]);
    // }
    node_flush(l_ch);
    node_flush(r_ch);

    for(int i = 0; i < right->children; i++)
    {
        struct bplus_node *node = node_fetch(sub(right)[i]);
        //printf("node->self = %ld\n",node->self);
        node->parent = right->self;
        node_flush(node);
    }
    return split_key;
}
key_t bptree::non_leaf_split_right2(struct bplus_node *node,
                                  struct bplus_node *right,
                                  struct bplus_node *l_ch,
                                  struct bplus_node *r_ch,
                                  key_t key,
                                  int insert)
{
    //printf("non_leaf_split_right2!\n");
    add_right_node(node,right);

    // printf("node->self = %ld,right->self = %ld,l_ch->self = %ld,r_ch->self = %ld\n",
    //        node->self,right->self,l_ch->self,r_ch->self);
    // printf("key = %d\n",key);
    // printf("insert = %d\n",insert);

    int split = (_max_entries + 1) / 2;
    key_t split_key = key(node)[split - 1];

    node->children = split + 1;
    right->children = _max_entries - split ;


    memmove(&key(right)[0],&key(node)[split],
            (_max_entries - split -1) * sizeof(key_t));
    memmove(&sub(right)[0],&sub(node)[split],
            (_max_entries - split) * sizeof(off_t));

    memmove(&key(node)[insert + 1],&key(node)[insert],
            (split - insert -1) * sizeof(key_t));
    memmove(&sub(node)[insert + 2],&sub(node)[insert + 1],
             (split - insert -1 ) * sizeof(off_t));

    key(node)[insert] = key;
    sub(node)[insert + 1] = r_ch ->self;
    r_ch->parent = node->self;


    // for(int i = 0; i < node->children -1 ; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < right->children - 1; i++)
    // {
    //     printf("key(right)[%d] = %d\n",i,key(right)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
    // for(int i = 0; i < right->children; i++)
    // {
    //     printf("sub(right)[%d] = %ld\n",i,sub(right)[i]);
    // }

    node_flush(l_ch);
    node_flush(r_ch);
    node_flush(node);

    for(int i = 0; i < right->children; i++)
    {
        struct bplus_node *node = node_fetch(sub(right)[i]);
        node->parent = right->self;
        node_flush(node);
    }

    return split_key;
}

bptree::bptree(const char *file_name,int blocksize)
    : tree(NULL)
{
    bzero(filename,sizeof(filename));
    strcpy(filename,file_name);
    _block_size = blocksize;
}
bptree::~bptree()
{

}
int bptree::bpopen()
{
    return ::open(filename ,O_CREAT | O_RDWR,0644);
}
void bptree::bpclose()
{
    ::close(fd);
}
int bptree::init()
{
    tree = (struct bplus_tree *)malloc(1 * sizeof(*tree));
    if(tree == NULL)
    {
        printf("malloc fail!\n");
        return -1;
    }
    tree->caches = (char *)malloc(_block_size * MIN_CACHE_NUM);
    if(tree->caches == NULL)
    {
        printf("malloc fail!\n");
        return -1;
    }
    tree->used[MIN_CACHE_NUM] = {0};
    tree->root = INVALID_OFFSET;
    tree->file_size = 0;
    fd = bpopen();
    if(fd < 0)
    {
        printf("open file fail!\n");
        return -1;
    }
    return 0;
}
void bptree::free_cache(struct bplus_node * node)
{
    char *buf = (char *)node;
    int i = (buf - tree->caches) / _block_size;
    tree->used[i] = 0;
}
void bptree::node_flush(struct bplus_node * node)
{
    //ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
    //带偏移量地写数据到文件中
    if(node != NULL)
    {
        int len = pwrite(fd,node,_block_size,node->self);
        assert(len == _block_size);
        free_cache(node);
    }
}
struct bplus_node *bptree::node_seek(off_t offset)
{
    if(offset == INVALID_OFFSET)
    {
        return NULL;
    }

    int i;
    for(i = 0; i < MIN_CACHE_NUM; i++)
    {
        if(!tree->used[i])
        {
            char *buf = tree->caches + _block_size * i;
            int len = pread(fd, buf, _block_size, offset);
            assert(len == _block_size);
            return  (struct bplus_node *)buf;
        }
    }
    assert(0);
}
struct bplus_node *bptree::get_cache()
{
    int i;
    for(i = 0; i < MIN_CACHE_NUM; i++)
    {
        if(!tree->used[i])
        {
            tree->used[i] = 1;
            char *buf = tree->caches + _block_size * i;
            return (struct bplus_node *)buf;
         }
    }
    assert(0);
}
struct bplus_node * bptree::node_fetch(off_t offset)
{
    if(offset == INVALID_OFFSET)
    {
        return NULL;
    }
    //size_t pread(int fd, void* buf, size_t count, off_t offset)
    //带偏移量地原子的从文件中读取数据
    struct bplus_node *node = get_cache();
    int len = pread(fd,node,_block_size,offset);
    assert(len == _block_size);
    return node;
}

struct bplus_node *bptree::node_new()
{
    struct bplus_node *node = get_cache();
    node->self = INVALID_OFFSET;
    node->parent = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    node->prev = INVALID_OFFSET;
    node->children = 0;
}
struct bplus_node *bptree::non_leaf_new()
{
    struct bplus_node *node = node_new();
    node->type = BPLUS_TREE_NON_LEAF;
    return node;
}

struct bplus_node *bptree::leaf_new()
{
    struct bplus_node *node = node_new();
    node->type = BPLUS_TREE_LEAF;
    return node;
}
off_t bptree::new_node_add(struct bplus_node *node)
{
    node->self = tree->file_size;
    tree->file_size += _block_size;


    return node->self;
}

void bptree::add_left_node(struct bplus_node *node,struct bplus_node *left)
{
    new_node_add(left);

    //printf("node->self = %ld,left->self = %ld\n",node->self,left->self);
    struct bplus_node *prev = node_fetch(node->prev);
    if(prev != NULL)
    {
        //printf("prev != NULL!\n");
        //printf("prev->self = %ld\n",prev->self);
        prev->next = left->self;
        left->prev = prev->self;
        node_flush(prev);

    }else{
        //printf("prev == NULL!\n");
        left->prev = INVALID_OFFSET;
    }
    node->prev = left->self;
    left->next = node->self;
}
//
void bptree::add_right_node(struct bplus_node *node,struct bplus_node *right)
{
    new_node_add(right);

    //printf("node->self = %ld,right = %ld\n",node->self,right->self);
    struct bplus_node *next = node_fetch(node->next);
    if(next != NULL)
    {
        //printf("next->self = %ld\n",next->self);
        next->prev = right->self;
        right->next = next->self;
        node_flush(next);
    }else{
        right->next = INVALID_OFFSET;
    }

    node->next = right->self;
    right->prev = node->self;
}
key_t bptree::leaf_split_left(struct bplus_node *node,struct bplus_node *left,
                             key_t key,long data,int insert)
{
    //printf("leaf_split_left!\n");
    int split = (node->children + 1) / 2;
    add_left_node(node,left);

    // printf("node->self = %ld,node->prev = %ld,left->next = %ld\n",
    //       node->self,node->prev,left->next);
    // printf("left->self  = %ld\n",left->self);
    // printf("insert = %d\n",insert);
    int privot = insert;
    left->children = split;
    node->children = _max_entries - split + 1;

    memmove(&key(left)[0],&key(node)[0],privot * sizeof(key_t));
    memmove(&data(left)[0],&data(node)[0],privot * sizeof(long));

    key(left)[privot] = key;
    data(left)[privot] = data;

    memmove(&key(left)[privot + 1], &key(node)[privot],
                 (split - privot - 1) * sizeof(key_t));
    memmove(&data(left)[privot + 1],&data(node)[privot],
                 (split - privot -1 )* sizeof(long));

    memmove(&key(node)[0],&key(node)[split - 1],
            node->children * sizeof(key_t));
    memmove(&data(node)[0],&data(node)[split - 1],
            node->children * sizeof(long));

    // for(int i = 0; i < left->children; i++)
    // {
    //     printf("key(left)[%d] = %d\n",i,key(left)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }

    return key(node)[0];

}
key_t bptree::leaf_split_right(struct bplus_node *node,struct bplus_node *right,
                               key_t key,long data,int insert)
{
    //printf("leaf_split_right!\n");
    int split = (node->children + 1) / 2;
    add_right_node(node,right);
    //printf("node->self = %ld,right->self = %ld\n",node->self,right->self);


    int privot = insert - split;
    node->children = split;
    right->children = _max_entries - split + 1;
    // printf("node->children = %d,right->children = %d\n",
    //     node->children,right->children);
    memmove(&key(right)[0],&key(node)[split],
            privot * sizeof(key_t));
    memmove(&data(right)[0],&data(node)[split],
            privot * sizeof(long));

    key(right)[privot] = key;
    data(right)[privot] = data;

    memmove(&key(right)[privot + 1], &key(node)[insert],
            (_max_entries - insert) * sizeof(key_t));
    memmove(&data(right)[privot + 1],&data(node)[insert],
            (_max_entries - insert) * sizeof(long));

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < right->children; i++)
    // {
    //     printf("key(right)[%d] = %d\n",i,key(right)[i]);
    // }
    return key(right)[0];
}

int bptree::creat_parent_node(struct bplus_node *l_ch,struct bplus_node *r_ch,key_t key)
{
    //printf("creat_parent_node!\n");
    if(l_ch->parent == INVALID_OFFSET && r_ch->parent == INVALID_OFFSET)
    {
        //printf("both NULL!\n");
        struct bplus_node *parent = non_leaf_new();
        key(parent)[0] = key;
        sub(parent)[0] = l_ch->self;
        sub(parent)[1] = r_ch->self;
        parent->children = 2;

        tree->root = new_node_add(parent);
        l_ch->parent = parent->self;
        r_ch->parent = parent->self;
        tree->level++;

        // printf("l_ch->self = %ld,r_ch->self = %ld,parent->self = %ld\n",
        //         l_ch->self,r_ch->self,parent->self);
        // for(int i = 0; i < parent->children; i++)
        // {
        //     printf("sub(parent)[%d] = %ld\n",i,sub(parent)[i]);
        // }
        // for(int i = 0; i < l_ch->children; i++)
        // {
        //     printf("key(l_ch)[%d] = %d\n",i,key(l_ch)[i]);
        // }
        // for(int i = 0; i < r_ch->children; i++)
        // {
        //     printf("key(r_ch)[%d] = %d\n",i, key(r_ch)[i]);
        // }
        node_flush(l_ch);
        node_flush(r_ch);
        node_flush(parent);
        return 0;

    }else if(r_ch->parent == INVALID_OFFSET){
        //printf("l_ch->self = %ld\n",l_ch->self);
        return non_leaf_insert(node_fetch(l_ch->parent),l_ch,r_ch,key);
    }else{
        //printf("r_ch->self = %ld\n",r_ch->self);
        //printf("key = %d\n",key);
        return non_leaf_insert(node_fetch(r_ch->parent),l_ch,r_ch,key);
    }
}

int bptree::non_leaf_insert(struct bplus_node *node,
                            struct bplus_node *l_ch,
                            struct bplus_node* r_ch,
                            key_t key)
{
    // printf("non_leaf_insert!\n");
    // printf("node->self = %ld,l_ch->self = %ld,r_ch->self = %ld\n",
    //        node->self,l_ch->self,r_ch->self);
    // printf("node->children = %d,l_ch->children = %d,r_ch->children = %d\n",
    //         node->children,l_ch->children,r_ch->children);
    // printf("key = %d\n",key);

    int insert = key_insert_location(node,key);
    assert(insert < 0);
    insert = -insert - 1;

    // printf("insert = %d\n",insert);
    // printf("node->children = %d\n",node->children);
    // printf("key = %d\n",key);
    if(node->children  == _max_entries)
    {
        key_t split_key;
        int split = (_max_entries + 1) / 2;
        struct bplus_node *sibling = non_leaf_new();

        if(insert < split)
        {
            //
            split_key = non_leaf_split_right2(node,sibling,l_ch,r_ch,key,insert);
        }else if(insert == split){
            //printf("insert == split!\n");
            split_key = non_leaf_split_right1(node,sibling,l_ch,r_ch,key,insert);
        }else{
            split_key = non_leaf_split_right(node,sibling,l_ch,r_ch,key,insert);

        }

        //printf("sibling->self = %ld\n",sibling->self);
        //printf("node->self  = %ld,sibling = %ld\n",node->self,sibling->self);
        if(insert < split)
        {
            return creat_parent_node(node,sibling,split_key);
        }else{
            return creat_parent_node(node,sibling,split_key);
        }
    }else{
        non_leaf_simple_insert(node,l_ch,r_ch,key,insert);
    //     for(int i = 0; i < node->children; i++)
    //     {
    //         printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    //     }
    //     printf("node->self = %ld\n",node->self);
    //     for(int i = 0; i < MIN_CACHE_NUM; i++)
    //    {
    //      printf("tree->used[%d] = %d\n",i,tree->used[i]);
    //    }
        node_flush(node);
        node_flush(l_ch);
        node_flush(r_ch);
    //     for(int i = 0; i < MIN_CACHE_NUM; i++)
    //     {
    //         printf("tree->used[%d] = %d\n",i,tree->used[i]);
    //    }
    }
}
int bptree::select_slib(struct bplus_node *parent,struct bplus_node *l_slib,
                        struct bplus_node *r_slib,int i)
{
    //i代表待删除key所在节点的第一个key在父节点中的位置
    //i分三种情况（1）在父节点为第一个  ---->   只能选择RIGTH_SLIBING
    //          （2）在父节点为最后一个 --->   只能选择LEFT_SLIBING
    //          （3）在中间           --->   选择children多的节点

    //printf("select_slib!\n");
    if(i == -1)
    {
        return RIGHT_SLIBING;
    }else if(i == parent->children - 2)
    {
        return LEFT_SLIBING;
    }else{
        return l_slib ->children >= r_slib->children ? LEFT_SLIBING : RIGHT_SLIBING;
    }
}
void bptree::merge_with_left_node(struct bplus_node *parent,
                                  struct bplus_node *l_slib,
                                  struct bplus_node *node,
                                  int index)
{
    printf("merge_with_left_node!\n");
    printf("parent->self = %ld,l_slib->self = %ld,node->self = %ld,index = %d\n",
           parent->self,l_slib->self,node->self,index);

    int insert = l_slib->children;
    memmove(&key(l_slib)[insert],&key(node)[0],
             node->children * sizeof(key_t));
    memmove(&data(l_slib)[insert],&data(node)[0],
             node->children * sizeof(long));

    l_slib->children += node->children;

    for(int i = 0; i < l_slib->children; i++)
    {
        printf("key(l_slib)[%d] = %d\n",i,key(l_slib)[i]);
    }
    printf("\n");
}
int bptree::delete_empty_node(struct bplus_node *node,
                              struct bplus_node *l_slib,
                              struct bplus_node *r_slib)
{
    //printf("delete_empty_node!!\n");
    //printf("node->self  = %ld,r_slib->self = %ld\n",node->self,r_slib->self);
    if(l_slib != NULL)
    {
        //printf("l_slib->self = %ld\n",l_slib->self);
        if(r_slib != NULL)
        {
            //printf("r_slib->self = %ld\n",r_slib->self);
            r_slib->prev = l_slib->self;
            l_slib->next = r_slib->self;
            node_flush(r_slib);
        }else{
            //printf("r_slib->self == NULL!\n");
            l_slib->next = INVALID_OFFSET;
        }
        node_flush(l_slib);
    }else{
        //printf("l_slib == NULL!\n");
        if(r_slib != NULL)
        {
            //printf("r_slib->self = %ld\n",r_slib->self);
            r_slib->prev = INVALID_OFFSET;
            node_flush(r_slib);
        }
    }

    //TODO:
    //把node的空间插入空闲链表

    //printf("node->self = %ld\n",node->self);
    free_cache(node);
}
int bptree::non_leaf_simple_delete(struct bplus_node *node,int i)
{
    // printf("non_leaf_simple_delete!\n");
    // printf("node->self = %ld,i = %d\n",node->self,i);

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i  < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }

    memmove(&key(node)[i],&key(node)[i + 1],(node->children - i - 2) * sizeof(key_t));
    memmove(&sub(node)[i + 1],&sub(node)[i + 2],(node->children - i - 2) * sizeof(off_t));

    node->children -- ;

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
}
int bptree::non_leaf_borrow_from_left(struct bplus_node *parent,
                                      struct bplus_node *l_slib,
                                      struct bplus_node *node,
                                      int i)
{
    //TODO:
    printf("non_leaf_borrow_from_left!\n");
    printf("parent->self = %ld,l_slib->self = %ld,node->self = %ld,i = %d\n",
           parent->self,l_slib->self,node->self,i);
    for(int i = 0; i < node->children; i++)
    {
        printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    }
    for(int i = 0; i < node->children - 1; i++)
    {
        printf("key(node)[%d] = %d\n",i,key(node)[i]);
    }

    for(int i = 0; i < l_slib->children; i++)
    {
        printf("sub(l_slib)[%d] = %ld\n",i,sub(l_slib)[i]);
    }
    for(int i = 0; i < l_slib->children - 1; i++)
    {
        printf("key(l_slib)[%d] = %d\n",i,key(l_slib)[i]);
    }

    memmove(&key(node)[1],&key(node)[0],(node->children - 1) * sizeof(key_t));
    memmove(&sub(node)[1],&sub(node)[0],(node->children) * sizeof(off_t));

    key(node)[0] = key(l_slib)[l_slib->children - 2];
    sub(node)[0] = sub(l_slib)[l_slib->children - 1];
    node->children ++;
    l_slib->children --;
    struct bplus_node *temp = node_fetch(sub(node)[0]);
    temp->parent = node->self;
    node_flush(temp);

    for(int i = 0; i < node->children; i++)
    {
        printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    }
    for(int i = 0; i < node->children - 1; i++)
    {
        printf("key(node)[%d] = %d\n",i,key(node)[i]);
    }

}
int bptree::non_leaf_borrow_from_right(struct bplus_node *parent,
                                       struct bplus_node *r_slib,
                                       struct bplus_node *node,
                                       int i)
{
    //TODO:
}
int bptree::non_leaf_merge_with_left(struct bplus_node *parent,
                                     struct bplus_node *node,
                                     struct bplus_node *l_slib,
                                     int i)
{
    // printf("non_leaf_merge_with_left!\n");
    // printf("parent->self = %ld,node->self = %ld,l_slib->self = %ld,i = %d\n",
    //         parent->self,node->self,l_slib->self,i);
    // printf("l_slib->children = %d\n",l_slib->children);
    // printf("node->children = %d\n",node->children);

    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }

    // for(int i = 0; i < l_slib->children - 1; i++)
    // {
    //     printf("key(l_slib)[%d] = %d\n",i,key(l_slib)[i]);
    // }
    // for(int i = 0; i < l_slib->children; i++)
    // {
    //     printf("sub(l_slib)[%d] = %ld\n",i,sub(l_slib)[i]);
    // }
    memmove(&key(node)[node->children + 1],&key(l_slib)[0],(l_slib->children - 1)* sizeof(key_t));
    key(node)[node->children - 1] = key(parent)[i];

    memmove(&sub(node)[node->children],&sub(l_slib)[0],l_slib->children * sizeof(off_t));

    node->children += l_slib->children;


    for(int i = 0; i < l_slib->children; i++)
    {
        struct bplus_node *temp = node_fetch(sub(l_slib)[i]);
        //printf("temp->self = %ld,temp->parent = %ld\n",temp->self,temp->parent);
        temp->parent = node->self;
        node_flush(temp);
    }

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }
    // for(int i = 0; i < node->children -1 ; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
}
int bptree::non_leaf_merge_with_right(struct bplus_node *parent,
                                      struct bplus_node *node,
                                      struct bplus_node *r_slib,
                                      int i)
{
    // printf("non_leaf_merge_with_right!\n");
    // printf("parent->self = %ld,node->self = %ld,r_slib->self = %ld,i = %d\n",
    //         parent->self,node->self,r_slib->self,i);
    // printf("r_slib->children = %d\n",r_slib->children);

    key(node)[node->children - 1] = key(parent)[i];
    node->children ++;

    memmove(&key(node)[node->children - 1],&key(r_slib)[0],
             (r_slib->children - 1) * sizeof(key_t));
    memmove(&sub(node)[node->children - 1],&sub(r_slib)[0],
            r_slib->children * sizeof(off_t));

    node->children += (r_slib->children - 1);

    for(int i = 0; i < r_slib->children; i++)
    {
        struct bplus_node *temp = node_fetch(sub(r_slib)[i]);
        temp->parent = node->self;
        node_flush(temp);
    }


    // for(int i = 0; i < node->children - 1; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    // }

}
int bptree::non_leaf_delete(struct bplus_node *node,int index)
{
    printf("non_leaf_delete!\n");
    printf("node->self = %ld,node->children = %d,index = %d\n",node->self,node->children,index);
    for(int i = 0; i < node->children - 1; i++)
    {
        printf("key(node)[%d] = %d\n",i,key(node)[i]);
    }
    for(int i = 0; i < node->children; i++)
    {
        printf("sub(node)[%d] = %ld\n",i,sub(node)[i]);
    }
    printf("node->parent = %ld\n",node->parent);

    if(node->parent == INVALID_OFFSET)
    {
        //printf("node->parent == INVALID_OFFSET!\n");
        if(node->children == 2)
        {
            struct bplus_node *root = node_fetch(sub(node)[index]);
            //printf("root->self = %ld\n",root->self);
            tree->root = root->self;
            tree->level--;
            root->parent = INVALID_OFFSET;
            delete_empty_node(node,NULL,NULL);
            node_flush(root);
        }else{
            non_leaf_simple_delete(node,index);
            node_flush(node);
        }

    }else if(node->children <= (_max_entries + 1) / 2){

        //printf("node -> children <= (_max_entries + 1) / 2");

        struct bplus_node *parent = node_fetch(node->parent);
        struct bplus_node *l_slib = node_fetch(node->prev);
        struct bplus_node *r_slib = node_fetch(node->next);

        // printf("parent->self = %ld,l_slib->self = %ld,r_slib->self = %ld\n",
        //        parent->self,l_slib->self,r_slib->self);
        //printf("parent->self = %ld,key(node)[0] = %d\n",parent->self,key(node)[0]);
        int i = index_in_parent(parent,key(node)[0]);
        //printf("i = %d\n",i);

        if(select_slib(parent,l_slib,r_slib,i) == LEFT_SLIBING)
        {
            //printf("select LEFT_SLIBING!\n");
            non_leaf_simple_delete(node,index);
            if(l_slib->children <= (_max_entries + 1) / 2)
            {

                non_leaf_merge_with_left(parent,l_slib,node,i);

                struct bplus_node *rr_slib = node_fetch(node->next);
                delete_empty_node(node,l_slib,rr_slib);
                node_flush(node);

                non_leaf_delete(parent,i);
            }else{
                non_leaf_borrow_from_left(parent,l_slib,node,index);
                node_flush(node);
                node_flush(parent);
                node_flush(r_slib);
                node_flush(l_slib);
            }
        }else{
            //printf("select RIGTH_SLIBING!\n");
            non_leaf_simple_delete(node,index);

            if(r_slib->children <= (_max_entries + 1) / 2)
            {

                non_leaf_merge_with_right(parent,node,r_slib,i + 1);


                struct bplus_node *rr_slib = node_fetch(r_slib->next);
                // printf("r_slib->self = %ld,node->self = %ld\n",
                //        r_slib->self,node->self);
                delete_empty_node(r_slib,node,rr_slib);
                node_flush(l_slib);

                non_leaf_delete(parent,i + 1);
            }else{
                non_leaf_borrow_from_right(parent,r_slib,node,index);
                node_flush(node);
                node_flush(parent);
                node_flush(r_slib);
                node_flush(l_slib);
            }
        }
    }else{
        non_leaf_simple_delete(node,index);
        node_flush(node);
    }
}
int bptree::borrow_from_right_node(struct bplus_node *parent,
                                   struct bplus_node *r_slib,
                                   struct bplus_node *node,
                                   int index)
{
    // printf("borrow_from_right_node!\n");
    // printf("parent->self = %ld,r_slib->self = %ld,node->self = %ld,index = %d\n",
    //        parent->self,r_slib->self,node->self,index);
    // printf("parent->children = %d\n",parent->children);

    key_t key = key(r_slib)[0];
    long data = data(r_slib)[0];

    memmove(&key(r_slib)[0],&key(r_slib)[1],(r_slib->children - 1) * sizeof(key_t));
    memmove(&data(r_slib)[0],&data(r_slib)[1],(r_slib->children - 1) * sizeof(long));
    r_slib->children--;
    node->children++;

    key(node)[node->children - 1] = key;
    data(node)[node->children - 1] = data;

    key(parent)[index] = key(r_slib)[0];

    //printf("parent->children = %d\n",parent->children);
    // for(int i = 0; i < parent->children - 1; i++)
    // {
    //     printf("key(parent)[%d] = %d\n",i,key(parent)[i]);
    // }
    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < r_slib->children; i++)
    // {
    //     printf("key(r_slib)[%d] = %d\n",i,key(r_slib)[i]);
    // }
}
int bptree::borrow_from_left_node(struct bplus_node *parent,
                                  struct bplus_node *l_slib,
                                  struct bplus_node *node,
                                  int index)
{
    // printf("borrow_from_left_node!\n");
    // printf("l_slib ->self = %ld,node->self = %ld\n",l_slib->self,node->self);
    // printf("parent->self = %ld,index = %d\n",parent->self,index);

    key_t key = key(l_slib)[l_slib->children - 1];
    long data = data(l_slib)[l_slib->children - 1];

    memmove(&key(node)[1],&key(node)[0],node->children * sizeof(key_t));
    memmove(&data(node)[1],&data(node)[0],node->children * sizeof(long));

    key(node)[0] = key;
    data(node)[0] = data;

    key(parent)[index] = key;
    l_slib->children -- ;
    node->children ++;

    // for(int i = 0; i < parent->children - 1; i++)
    // {
    //     printf("key(parent)[%d] = %d\n",i,key(parent)[i]);
    // }

    // for(int i = 0; i < node->children; i++)
    // {
    //     printf("key(node)[%d] = %d\n",i,key(node)[i]);
    // }
    // for(int i = 0; i < l_slib->children; i++)
    // {
    //     printf("key(l_slib)[%d] = %d\n",i,key(l_slib)[i]);
    // }
}
int bptree::leaf_delete(struct bplus_node *node,key_t key,long data)
{
    printf("leaf_delete!\n");
    printf("node->self = %ld\n",node->self);
    printf("node->children = %d\n",node->children);

    int index = key_insert_location(node,key);
    // printf("index = %d\n",index);
    // printf("key = %d\n",key);
    if(index < 0)
    {
        //not exit!
        return -1;
    }

    int i = ((char *)node - tree->caches) / _block_size;
    tree->used[i] = 1;
    if(node->parent == INVALID_OFFSET)
    {
        leaf_simple_delete(node,key,data,index);
        if(node->children == 0)
        {
            delete_empty_node(node,NULL,NULL);
            // for(int i = 0; i < MIN_CACHE_NUM; i++)
            // {
            //     printf("%5d",tree->used[i]);
            // }
            tree->root = INVALID_OFFSET;
        }
        node_flush(node);
    }else if(node->children <= (_max_entries + 1) / 2)
    {
        // printf("node->children <= (_max_entries + 1)\n");
        // printf("node->self = %ld,node->prev = %ld,node->next = %ld,node->parent = %ld\n",
        //         node->self,node->prev,node->next,node->parent);

        // for(int i = 0; i < MIN_CACHE_NUM; i++)
        // {
        //     printf("tree->used[%d] = %d\n",i,tree->used[i]);
        // }
        struct bplus_node *parent = node_fetch(node->parent);
        struct bplus_node *r_slib = node_fetch(node->next);
        struct bplus_node *l_slib = node_fetch(node->prev);
        // for(int i = 0; i < MIN_CACHE_NUM; i++)
        // {
        //     printf("tree->used[%d] = %d\n",i,tree->used[i]);
        // }
        int i = index_in_parent(parent,key(node)[0]);
        printf("parent->self = %ld,r_slib->self = %ld,l_slib->self = %ld\n",
                parent->self,r_slib->self,l_slib->self);
        printf("key(node)[0] = %d,i = %d\n",key(node)[0],i);

        if(select_slib(parent,l_slib,r_slib,i) == LEFT_SLIBING)
        {
            //printf("chose LEFT_SLIBING!\n");
            //先删除节点
            leaf_simple_delete(node,key,data,index);
            if(l_slib->children > (_max_entries + 1) / 2)
            {
                borrow_from_left_node(parent,l_slib,node,i);
                node_flush(node);
                node_flush(r_slib);
                node_flush(l_slib);
                node_flush(parent);
            }else{
                //printf("l_slib->children < (_max_entries + 1) / 2!\n");
                merge_with_left_node(parent,l_slib,node,index);

                //printf("r_slib->self = %ld\n",r_slib->self);
                delete_empty_node(node,l_slib,r_slib);

                non_leaf_delete(parent,i);
            }
        }else{
            printf("chose RIGHT_SLIBING!\n");
            //先删除节点
            leaf_simple_delete(node,key,data,index);
            if(r_slib->children > (_max_entries + 1) / 2)
            {
                borrow_from_right_node(parent,r_slib,node,i +  1);
                node_flush(node);
                node_flush(r_slib);
                node_flush(l_slib);
                node_flush(parent);
            }else{
                merge_with_left_node(parent,node,r_slib,index);

                struct bplus_node *rr_slib = node_fetch(r_slib->next);
                delete_empty_node(r_slib,node,rr_slib);
                node_flush(l_slib);

                non_leaf_delete(parent,i + 1);
            }
        }
    }else{
        leaf_simple_delete(node,key,data,index);
        node_flush(node);
    }
    return 0;
}
int bptree::leaf_insert(struct bplus_node *node,key_t key,long data)
{
    // printf("leaf_insert!\n");
    // printf("node->self = %ld,node->children = %d\n",node->self,node->children);
    // printf("node->parent = %ld,node->next = %ld",node->self,node->next);

    //查找key插入的位置（二分查）
    int insert = key_insert_location(node,key);
    if(insert >= 0)
    {
        //already exists
        return -1;
    }
    insert = -insert - 1;

    //将caches中的空间标记为1，表示在使用
    int i = ((char *)node - tree->caches) / _block_size;
    tree->used[i] = 1;
    // for(int i = 0; i < MIN_CACHE_NUM; i++)
    // {
    //      printf("tree->used[%d] = %d\n",i,tree->used[i]);
    // }

    //关键字个数达到上限，需要分裂
    if(node->children == _max_entries)
    {
        key_t split_key;
        int split = (_max_entries + 1) / 2;
        //printf("insert = %d,split = %d\n",insert,split);
        //printf("key = %d\n",key);

        //在caches中申请一个新的空间，填充分裂后的新节点
        struct bplus_node *sibling = leaf_new();
        // for(int i = 0; i < MIN_CACHE_NUM; i++)
        // {
        //      printf("tree->used[%d] = %d\n",i,tree->used[i]);
        // }
        //分裂节点，填充数据
        if(insert < split)
        {
            //printf("insert < split!\n");
            split_key = leaf_split_left(node,sibling,key,data,insert);
        }else{
            //printf("insert >= spplit!\n");
            split_key = leaf_split_right(node,sibling,key,data,insert);
        }

        //创建parent节点
        if(insert < split)
        {
            return creat_parent_node(sibling,node,split_key);
        }else
        {
            return creat_parent_node(node,sibling,split_key);
        }
    }else{
        //关键字不满时，直接插入，并把节点写入文件
        leaf_simple_insert(node,key,data,insert);
        //printf("node->parent = %ld\n",node->parent);
        node_flush(node);
    }
}
//插入操作
int bptree::insert(key_t key,long data)
{
    //把根从文件读到caches中
    struct bplus_node *node = node_seek(tree->root);

    //
    while(node != NULL)
    {
        if(is_leaf(node))
        {
            return leaf_insert(node,key,data);
        }else{
            int i = key_insert_location(node,key);
            //printf("i = %d\n",i);
            if(i >= 0)
            {
                node = node_seek(sub(node)[i + 1]);
            }else{
                //printf("i = %d\n",i);
                i = -i-1;
                //printf("sub(node)[i] = %ld\n",sub(node)[i]);
                node = node_seek(sub(node)[i]);
            }
        }
    }

    struct bplus_node *root = leaf_new();
    key(root)[0] = key;
    data(root)[0] = data;
    root->children = 1;
    tree->level = 1;
    tree->root = new_node_add(root);
    node_flush(root);

    return 0;
}

//删除操作
int bptree::remove(key_t key,long data)
{
    //把根文件读到cache中
    struct bplus_node *node = node_seek(tree->root);
    if(node == NULL)
    {
        printf("bplus tree is NULL!\n");
        return -1;
    }

    //printf("root ->self = %ld\n",node->self);
    while(node != NULL)
    {
        if(is_leaf(node))
        {
            return leaf_delete(node,key,data);
        }else{
            int i = key_insert_location(node,key);
            //printf("i = %d\n",i);
            if(i >= 0)
            {
                node  = node_seek(sub(node)[i + 1]);
            }else{
                //printf("i = %d\n",i);
                i = -i - 1;
                //printf("sub(node)[i] = %ld\n",sub(node)[i]);
                node = node_seek(sub(node)[i]);
            }

        }
    }
}
int bptree::get_level()
{
    return tree->level;
}
void bptree::print()
{
    printf("print!\n");
    if(tree == NULL || tree->root == INVALID_OFFSET)
    {
        printf("tree is NULL !\n");
        return ;
    }
    struct bplus_node *p = node_seek(tree->root);
    struct bplus_node *node = p;
    struct bplus_node *q  = NULL;
    int k = tree->level - 1;
    int count;


     //test
     struct bplus_node *temp1 = node_seek(tree->root);
     if(temp1 != NULL && !is_leaf(temp1))
     {
         for(int i = 0; i < temp1->children; i++)
         {
             printf("sub(temp1)[%d] = %ld\n", i,sub(temp1)[i]);
         }
     }



    //TODO: 只有一层时，段错误，有bug(k值的原因)
    for(int i = 1; i <= k; i++)
    {
        node = p;
        //printf("\n-------%d level-------\n",i);
        while(node != NULL)
        {
            for(int j = 0; j < node->children - 1; j++)
            {
                printf("%5d",key(node)[j]);
            }
            printf("        ");
             printf("\n node->self = %ld node->parent = %ld\n",node->self,node->parent);
            node = node_seek(node->next);
        }
        printf("\n");

        if(q != NULL)
        {
            int index = ((char *)q - tree->caches) / _block_size;
            tree->used[index] = 0;
        }
        q = p;
        p = node_fetch(sub(p)[0]);
    }

    printf("\n---------leaf node--------\n");
    while(p)
    {
        for(int i = 0; i < p->children; i++)
        {
            printf("%5d",key(p)[i]);
        }
        printf("\n p->self = %ld p->parent = %ld\n",p->self,p->parent);
        p = node_seek(p->next);
        printf("      ");
    }
    printf("\n");

    int index = ((char *)q - tree->caches) / _block_size;
    tree->used[index] = 0;

    printf("---------tree->used[i]----------\n");
    for(int i = 0; i < MIN_CACHE_NUM; i++)
    {
        printf("%5d",tree->used[i]);
    }
    printf("\n");
}