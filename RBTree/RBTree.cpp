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
 right�ڵ���������ͼʾ��
 node           right
 / \     ==>     / \
 a right       node y
   / \          / \
   b y          a b 
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_left(rb_node_t* node, rb_node_t* root)
{
    // ��Ҫ��ת�Ľڵ���right
    // 1��node���Ҷ��Ӹ���Ϊright��right�������
    rb_node_t* right = node->right;
    if (right->left != NULL)
    {
        node->right = right->left;
        right->left->parent = node;
    }

    // 2��right������Ӹ���Ϊnode������right�ĸ�ĸ�ڵ�
    right->left = node;
    if (node == root)
    {

        root = right;
    }
    else
    {
        // ����ԭnode�������丸ĸ������ӻ����Ҷ���
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

    // 3������node�ĸ�ĸ
    node->parent = right;
    return root;
}

/*-----------------------------------------------------------
 left�ڵ���������ͼʾ��
   node         left
   / \          / \
 left y ==>    a node
 / \             / \
 a b             b y 
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_right(rb_node_t* node, rb_node_t* root)
{
    // ��Ҫ��ת�Ľڵ���left
    // 1������node�������Ϊleft���Ҷ���
    rb_node_t* left = node->left;
    if (left->right != NULL)
    {
        node->left = left->right;
        left->right->parent = node;
    }

    // 2������left���Ҷ��Ӻ��丸ĸ�ڵ�
    left->right = node;
    if (node == root)
    {
        root = left;
    }
    else
    {
        // ����ԭnode�������丸ĸ������ӻ����Ҷ���
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

    // 3������node�ĸ�ĸ
    node->parent = left;
    return root;
}

/*****************************************************************
** Function name: rb_insert_rebalance
** Function Description: ͨ���ؼ���ƥ����ڵ��Ƿ��Ѿ����ڣ�������ʱ��
                         ����Ҫ����λ�õĸ�ĸ�ڵ�
** Input argv:
        key:ָ�������Ĺؼ���  
        root:������ĸ��ڵ�
** Output argv:
        save:ͨ���ؼ���ƥ�����ӽ��Ľڵ�,Ҫô��keyһ�µĽڵ㣬Ҫô��
             ��Ҫ����ڵ�ĸ�ĸ�ڵ�   
** Return:
        NULL:��ʾû���ҵ�
        ��NULL:��ʾ�ҵ���ͬ��key�Ľڵ�
*****************************************************************/
static rb_node_t* rb_insert_rebalance(rb_node_t *node, rb_node_t *root)
{
    rb_node_t *parent, *gparent, *uncle, *tmp;
    
    // ����Ľڵ�Ĭ����ɫ�Ǻ�ɫ
    // ����丸ĸ���ڲ����Ǻ�ɫ�ģ���Ӱ�������ԣ�����Ҫ����
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
** Function Description: ͨ���ؼ���ƥ����ڵ��Ƿ��Ѿ����ڣ�������ʱ��
                         ����Ҫ����λ�õĸ�ĸ�ڵ�
** Input argv:
        key:ָ�������Ĺؼ���  
        root:������ĸ��ڵ�
** Output argv:
        save:ͨ���ؼ���ƥ�����ӽ��Ľڵ�,Ҫô��keyһ�µĽڵ㣬Ҫô��
             ��Ҫ����ڵ�ĸ�ĸ�ڵ�   
** Return:
        NULL:��ʾû���ҵ�
        ��NULL:��ʾ�ҵ���ͬ��key�Ľڵ�
*****************************************************************/
static rb_node_t* rb_search_auxiliary(key_t key,rb_node_t* root, rb_node_t** save)
{
    rb_node_t *node = root, *parent = NULL;
    while (node != NULL)
    {
        parent = node;

        // �����ô�С�Ƚϣ���û��������Ƚ�0��Ϊ�˱������keyΪ�޷���
        // ��ʼ�շ�����ֵ�Ĵ���
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

    // ������keyֵ��һ������Ľڵ㣬����ȷ������λ��
    if (save)
    {
        *save= parent;
    }
    return NULL;
}

/*****************************************************************
** Function name: rb_insert
** Function Description: ����һ���µĽڵ㲢�ҵ��������ֺ������ƽ��
** Input argv:
        key:ָ�������Ĺؼ���  
        data:�ڵ������
        root:������ĸ��ڵ�  
** Return:
        �µ�root�ڵ�
*****************************************************************/
rb_node_t* rb_insert(key_t key, data_t data, rb_node_t* root)
{
    rb_node_t *parent = NULL, *node;

    // �ؼ��ֶ�Ӧ�Ľڵ��Ѵ��ڣ�ֱ�ӷ���
    if (rb_search_auxiliary(key, root, &parent) != NULL)
    {
        return root;
    }

    // ����ڵ㣬parent�����Ѿ��ҵ���
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