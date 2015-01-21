#include <stdio.h>
#include <stdlib.h>
#include "RBTree.h"

static rb_node_t* rb_new_node(key_t key,data_t data)
{
    rb_node_t *node = (rb_node_t *)malloc(sizeof(struct rb_node_t));
    if (node == NULL)
    {
        printf("malloc error!\n");
        return NULL;
    }
    node->key = key,node->data = data;
    return node;
}

/*-----------------------------------------------------------
 right节点左旋，如图示例
 node           right
 / \     ==>     / \
 a right       node y
   / \          / \
   b y          a b 
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_left(rb_node_t* node, rb_node_t* root)
{
    // 需要旋转的节点是right
    // 1、node的右儿子更新为right的right的左儿子
    rb_node_t* right = node->right;
    if (right->left != NULL)
    {
        node->right = right->left;
        right->left->parent = node;
    }

    // 2、right的左儿子更新为node，更新right的父母节点
    right->left = node;
    if (node == root)
    {

        root = right;
    }
    else
    {
        // 区分原node本身是其父母的左儿子还是右儿子
        rb_node_t* parent = node->parent;
        if (parent->right == node)
        {

            parent->right = right;
        }
        else
        {
            parent->left = right;
        }
        
        right->parent = parent;
    }

    // 3、处理node的父母
    node->parent = right;
    return root;
}

/*-----------------------------------------------------------
 left节点右旋，如图示例
   node         left
   / \          / \
 left y ==>    a node
 / \             / \
 a b             b y 
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_right(rb_node_t* node, rb_node_t* root)
{
    // 需要旋转的节点是left
    // 1、更新node的左儿子为left的右儿子
    rb_node_t* left = node->left;
    if (left->right != NULL)
    {
        node->left = left->right;
        left->right->parent = node;
    }

    // 2、更新left的右儿子和其父母节点
    left->right = node;
    if (node == root)
    {
        root = left;
    }
    else
    {
        // 区分原node本身是其父母的左儿子还是右儿子
        rb_node_t* parent = node->parent;
        if (parent->right == node)
        {
            parent->right = left;
        }
        else
        {
            parent->left = left;
        
        }
        
        left->parent = parent;
    }

    // 3、处理node的父母
    node->parent = left;
    return root;
}

/*****************************************************************
** Function name: rb_insert_rebalance
** Function Description: 通过关键字匹配检测节点是否已经存在，不存在时找
                         到需要插入位置的父母节点
** Input argv:
        key:指定搜索的关键字  
        root:红黑树的根节点
** Output argv:
        save:通过关键字匹配的最接近的节点,要么是key一致的节点，要么是
             需要插入节点的父母节点   
** Return:
        NULL:表示没有找到
        非NULL:表示找到有同样key的节点
*****************************************************************/
static rb_node_t* rb_insert_rebalance(rb_node_t *node, rb_node_t *root)
{
    rb_node_t *parent, *gparent, *uncle, *tmp;
    
    // 插入的节点默认颜色是红色
    // 如果其父母存在并且是黑色的，不影响红黑特性，不需要调整
    while ((parent = node->parent) && parent->color == RED)
    {
        gparent = parent->parent;
        if (parent == gparent->left)
        {
            uncle = gparent->right;
            if (uncle && uncle->color == RED)
            {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
            }
            else
            {
                if (parent->right == node)
                {
                    root = rb_rotate_left(parent, root);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                parent->color = BLACK;
                gparent->color = RED;
                root = rb_rotate_right(gparent, root);
            }
        }
        else
        {
            uncle = gparent->left;
            if (uncle && uncle->color == RED)
            {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
            }
            else
            {
                if (parent->left == node)
                {
                    root = rb_rotate_right(parent, root);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                parent->color = BLACK;
                gparent->color = RED;
                root = rb_rotate_left(gparent, root);
            }
        }
    }
    root->color = BLACK;
    return root;
}

static rb_node_t* rb_erase_rebalance(rb_node_t *node, rb_node_t *parent, rb_node_t *root)
{
    rb_node_t *other,*o_left, *o_right;
    while ((!node|| node->color == BLACK) && node!= root)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (other->color == RED)
            {
                other->color = BLACK;
                parent->color = RED;
                root = rb_rotate_left(parent, root);
                other = parent->right;
            }
            if ((!other->left || other->left->color == BLACK) &&
                    (!other->right || other->right->color == BLACK))
            {
                other->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!other->right || other->right->color == BLACK)
                {
                    if ((o_left = other->left))
                    {
                        o_left->color = BLACK;
                    }
                    other->color = RED;
                    root = rb_rotate_right(other, root);
                    other = parent->right;
                }
                other->color = parent->color;
                parent->color = BLACK;
                if (other->right)
                {
                    other->right->color = BLACK;
                }
                root = rb_rotate_left(parent, root);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (other->color == RED)
            {
                other->color = BLACK;
                parent->color = RED;
                root = rb_rotate_right(parent, root);
                other = parent->left;
            }
            if ((!other->left || other->left->color == BLACK) &&
                    (!other->right || other->right->color == BLACK))
            {
                other->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!other->left || other->left->color == BLACK)
                {
                    if ((o_right = other->right))
                    {
                        o_right->color = BLACK;
                    }
                    other->color = RED;
                    root = rb_rotate_left(other,root);
                    other = parent->left;
                }
                other->color = parent->color;
                parent->color = BLACK;
                if (other->left)
                {
                    other->left->color = BLACK;
                }
                root = rb_rotate_right(parent, root);
                node = root;
                break;
            }
        }
    }
    if (node)
    {
        node->color = BLACK;
    }
    return root;
}

/*****************************************************************
** Function name: rb_search_auxiliary
** Function Description: 通过关键字匹配检测节点是否已经存在，不存在时找
                         到需要插入位置的父母节点
** Input argv:
        key:指定搜索的关键字  
        root:红黑树的根节点
** Output argv:
        save:通过关键字匹配的最接近的节点,要么是key一致的节点，要么是
             需要插入节点的父母节点   
** Return:
        NULL:表示没有找到
        非NULL:表示找到有同样key的节点
*****************************************************************/
static rb_node_t* rb_search_auxiliary(key_t key,rb_node_t* root, rb_node_t** save)
{
    rb_node_t *node = root, *parent = NULL;
    while (node != NULL)
    {
        parent = node;

        // 这里用大小比较，而没有用相减比较0，为了避免出现key为无符号
        // 型始终返回正值的错误
        if (node->key > key)
        {
            node = node->left;
        }
        else if (node->key < key)
        {
            node = node->right;
        }
        else
        {
            return node;
        }
    }

    // 保存下key值最一致相近的节点，便于确定插入位置
    if (save)
    {
        *save= parent;
    }
    return NULL;
}

/*****************************************************************
** Function name: rb_insert
** Function Description: 插入一个新的节点并且调整树保持红黑特性平衡
** Input argv:
        key:指定搜索的关键字  
        data:节点的数据
        root:红黑树的根节点  
** Return:
        新的root节点
*****************************************************************/
rb_node_t* rb_insert(key_t key, data_t data, rb_node_t* root)
{
    rb_node_t *parent = NULL, *node;

    // 关键字对应的节点已存在，直接返回
    if (rb_search_auxiliary(key, root, &parent) != NULL)
    {
        return root;
    }

    // 插入节点，parent上面已经找到了
    node = rb_new_node(key,data);
    if (node == NULL)
    {
        return root;
    }
    
    node->parent = parent;
    node->left = node->right = NULL;
    node->color = RED;
    if (parent != NULL)
    {
        if (parent->key > key)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }
    }
    else
    {
        root = node;
    }
    
    return rb_insert_rebalance(node, root);
}

rb_node_t* rb_search(key_t key,rb_node_t* root)
{
    return rb_search_auxiliary(key,root, NULL);
}

rb_node_t* rb_erase(key_t key,rb_node_t *root)
{
    rb_node_t *child, *parent, *old, *left, *node;
    color_t color;
    if (!(node = rb_search_auxiliary(key,root, NULL)))
    {
        printf("key %d is not exist!\n");
        return root;
    }
    old = node;
    if (node->left && node->right)
    {
        node = node->right;
        while ((left = node->left) != NULL)
        {
            node = left;
        }
        child = node->right;
        parent = node->parent;
        color = node->color;

        if (child)
        {
            child->parent = parent;
        }
        if (parent)
        {
            if (parent->left == node)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }
        else
        {
            root = child;
        }
        if (node->parent == old)
        {
            parent = node;
        }
        node->parent = old->parent;
        node->color = old->color;
        node->right = old->right;
        node->left = old->left;
        if (old->parent)
        {
            if (old->parent->left == old)
            {
                old->parent->left = node;
            }
            else
            {
                old->parent->right = node;
            }
        }
        else
        {
            root = node;
        }
        old->left->parent = node;
        if (old->right)
        {
            old->right->parent = node;
        }
    }
    else
    {
        if (!node->left)
        {
            child = node->right;
        }
        else if (!node->right)
        {
            child = node->left;
        }
        parent = node->parent;
        color = node->color;
        if (child)
        {
            child->parent = parent;
        }
        if (parent)
        {
            if (parent->left == node)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }
        else
        {
            root = child;
        }
    }
    free(old);
    if (color == BLACK)
    {
        root = rb_erase_rebalance(child, parent, root);
    }
    return root;
}