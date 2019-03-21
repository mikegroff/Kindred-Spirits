/*
    Michael Groff
    COP 3502 Fall 2016
    Kindred Spirits
*/

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

#define EMPTY_LIST_ERR

//This is the professor's Linked List code edited slightly to fit this assignment
typedef struct list
{
	int data;
	struct list *next;
} list;

typedef struct point
{
	node *stacknode;
	struct point *next;
} point;

typedef struct LinkedList
{
	list *head;
	list *tail;
} LinkedList;

typedef struct LinkedListpoint
{
	point *head;
	point *tail;
} LinkedListpoint;

int Listcompare(LinkedList *a,LinkedList *b);

LinkedList *createList(void);

LinkedList *destroyLinkedList(LinkedList *lists);

void tailInsert(LinkedList *lists, int data);

int offWithItsHead(LinkedList *lists);

//now im doing the same but for the linkedlist of node pointers that im going to use as my stack
//ill admit there are better ways to do this but i was pressed for time since I originally coded up this assignment using arrays

LinkedListpoint *createListpoint(void);

LinkedListpoint *destroyLinkedListpoint(LinkedListpoint *point);

void tailInsertpoint(LinkedListpoint *points, node *nodehold);

void headInsertpoint(LinkedListpoint *points, node *nodehold);

node *offWithItsHeadpoint(LinkedListpoint *points);

list *createNode(int data)
{
	list *n;

	if ((n = malloc(sizeof(list))) == NULL)
		return NULL;

	n->data = data;
	n->next = NULL;

	return n;
}

LinkedList *createList(void)
{
	LinkedList *ptr = calloc(1, sizeof(LinkedList));
	return ptr;
}

list *recursiveDestroyList(list *head)
{
	if (head == NULL)
		return NULL;

	recursiveDestroyList(head->next);
	free(head);

	return NULL;
}

LinkedList *destroyLinkedList(LinkedList *lists)
{
	if (lists == NULL)
		return NULL;

	recursiveDestroyList(lists->head);
	free(lists);

	return NULL;
}

void tailInsert(LinkedList *lists, int data)
{
	if (lists == NULL)
	{
		return;
	}
	if (lists->tail == NULL)
	{
		lists->head = lists->tail = createNode(data);
	}
	else
	{
		lists->tail->next = createNode(data);
		lists->tail = lists->tail->next;
	}
}

void obliterateNode(list *ptr)
{
	free(ptr);
}

int offWithItsHead(LinkedList *lists)
{
	int retval;
	list *temp;

	if (lists == NULL || lists->head == NULL)
		return EMPTY_LIST_ERR;

	retval = lists->head->data;

	temp = lists->head;
	lists->head = lists->head->next;
	obliterateNode(temp);

	return retval;
}

point *createNodepoint(node *nodehold)
{
	point *n;

	if ((n = malloc(sizeof(point))) == NULL)
		return NULL;

	n->stacknode = nodehold;
	n->next = NULL;

	return n;
}

LinkedListpoint *createListpoint(void)
{
	LinkedListpoint *ptr = calloc(1, sizeof(LinkedListpoint));
	return ptr;
}

point *recursiveDestroyListpoint(point *head)
{
	if (head == NULL)
		return NULL;

	recursiveDestroyListpoint(head->next);
	free(head);

	return NULL;
}

LinkedListpoint *destroyLinkedListpoint(LinkedListpoint *points)
{
	if (points == NULL)
		return NULL;


	recursiveDestroyListpoint(points->head);
	free(points);

	return NULL;
}

void headInsertpoint(LinkedListpoint *points, node *nodehold)
{
    point *n;

	if (points->head == NULL)
	{
		points->head = points->tail = createNodepoint(nodehold);
	}
	else
	{
	    n = createNodepoint(nodehold);
        n->next = points->head;
		points->head = n;
	}
}

void tailInsertpoint(LinkedListpoint *points, node *nodehold)
{
	if (points == NULL)
	{
		return;
	}
	if (points->tail == NULL)
	{
		points->head = points->tail = createNodepoint(nodehold);
	}
	else
	{
		points->tail->next = createNodepoint(nodehold);
		points->tail = points->tail->next;
	}
}

void obliterateNodepoint(point *ptr)
{
	free(ptr);
}

node *offWithItsHeadpoint(LinkedListpoint *points)
{
	node *retval;
	point *temp;

	if (points == NULL || points->head == NULL)
		return EMPTY_LIST_ERR;

	retval = points->head->stacknode;

	temp = points->head;
	points->head = points->head->next;
	obliterateNodepoint(temp);

	return retval;
}


//this function is very straightforward
//if goes down the branches of he trees and checks the value in the second tree where the node would be
//if it is a reflection
int isReflection(node *a, node *b)
{
    if(a == NULL && b == NULL)
        return 1;

    if(a == NULL || b == NULL)//safe because the case where both are null has already been examined
        return 0;

    if(a->data == b->data)
    {
        if(a->left != NULL || b->right != NULL)
            isReflection(a->left,b->right);
        if(b->left != NULL || a->right != NULL)
            isReflection(b->left,a->right);
    }
    else
        return 0;

    return 1;
}

//very similar to previous function
//difference is that it makes a tree and creates nodes that corresponds to the reflection of the nodes
//from the input tree
node *makeReflection(node *root)
{
    if(root == NULL)
        return NULL;

    node *newroot = malloc(sizeof(node));

    newroot->data = root->data;

    if(root->right != NULL)
        newroot->left = makeReflection(root->right);
    else
        newroot->left = NULL;

    if(root->left != NULL)
        newroot->right = makeReflection(root->left);
    else
        newroot->right = NULL;

    return newroot;
}
//once i switched to use lists to compare the traversals i needed a way to check the trees
//this is an order n function since it goes through each node and checks the order
int Listcompare(LinkedList *a,LinkedList *b)
{
    list *treeone = a->head;
    list *treetwo = b->head;

    while(treeone !=NULL || treetwo != NULL)
    {
        if(treeone->data != treetwo->data)
            return 0;
        else
        {
            treeone = treeone->next;
            treetwo = treetwo->next;
        }
    }
    return 1;
}
//used linked lists to hold the traversals
// also used a linked list as a stack to hold the pointers that could not be placed into the traversal yet
// order n function because it visits each node and the either places it in the traversal or holds it
//technically it visits some nodes more than once but only when its popping them off the stack
//it then decides what node to check next
int kindredSpirits(node *a, node *b)
{
    if(a == NULL && b == NULL)
        return 1;

    if(a == NULL || b == NULL)//safe because the case where both are null has already been examined
        return 0;

    LinkedList *apre = createList();
    LinkedList *bpre = createList();
    LinkedList *apost = createList();
    LinkedList *bpost = createList();

    LinkedListpoint *traverse = createListpoint();

    node *temp, *previous;

    temp = a;

    while(temp != NULL)
    {
        tailInsert(apre,temp->data);

        if(temp->right != NULL && temp->left == NULL)
            temp = temp->right;

        else if(temp->right == NULL && temp->left != NULL)
            temp = temp->left;

        else if(temp->right != NULL && temp->left != NULL)
        {
            headInsertpoint(traverse, temp->right);//pushing onto a linked list as if it were a stack
            temp = temp->left;
        }
        else
            temp = offWithItsHeadpoint(traverse);//popping the head of the list
    }

    temp = b;

    while(temp != NULL)
    {
        if(temp->right != NULL && temp->left == NULL)
        {
            if(temp->right == previous)
            {
                tailInsert(bpost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                temp = temp->right;
            }
        }

        else if(temp->right == NULL && temp->left != NULL)
        {
            if(temp->left == previous)
            {
                tailInsert(bpost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                temp = temp->left;
            }
        }

        else if(temp->right != NULL && temp->left != NULL)
        {
            if(temp->right == previous)
            {
                tailInsert(bpost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                headInsertpoint(traverse, temp->right);
                temp = temp->left;
            }

        }
        else
        {
            tailInsert(bpost, temp->data);
            previous = temp;
            temp = offWithItsHeadpoint(traverse);
        }
    }

    temp = b;

    while(temp != NULL)
    {
        tailInsert(bpre,temp->data);

        if(temp->right != NULL && temp->left == NULL)
            temp = temp->right;

        else if(temp->right == NULL && temp->left != NULL)
            temp = temp->left;

        else if(temp->right != NULL && temp->left != NULL)
        {
            headInsertpoint(traverse, temp->right);
            temp = temp->left;
        }
        else
            temp = offWithItsHeadpoint(traverse);
    }

    temp = a;

    while(temp != NULL)
    {
        if(temp->right != NULL && temp->left == NULL)
        {
            if(temp->right == previous)
            {
                tailInsert(apost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                temp = temp->right;
            }
        }

        else if(temp->right == NULL && temp->left != NULL)
        {
            if(temp->left == previous)
            {
                tailInsert(apost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                temp = temp->left;
            }
        }

        else if(temp->right != NULL && temp->left != NULL)
        {
            if(temp->right == previous)
            {
                tailInsert(apost, temp->data);
                previous = temp;
                temp = offWithItsHeadpoint(traverse);
            }
            else
            {
                headInsertpoint(traverse, temp);
                headInsertpoint(traverse, temp->right);
                temp = temp->left;
            }

        }
        else
        {
            tailInsert(apost, temp->data);
            previous = temp;
            temp = offWithItsHeadpoint(traverse);
        }
    }

    if(Listcompare(apre, bpost) + Listcompare(bpre, apost))
        return 1;
    else
        return 0;
/*
    left over code from previous attempt to solve problem using arrays
    after seeing the professor state that his was a bad idea i scrapped it but retained the logic
    this time using linked lists instead of arrays since it is necessary to compare large trees

    list *temprint = bpost->head;

    while(temprint != NULL)
    {
        printf("%d ", temprint->data);
        temprint = temprint->next;
    }

    int *bpost = malloc(sizeof(int)*1000); //no longer useful because it needs to be able to compare large trees
    int *apre = malloc(sizeof(int)*1000);
    int *apost = malloc(sizeof(int)*1000);
    int *bpre = malloc(sizeof(int)*1000);
    int l=0,m=0,n=0,o=0,p=0,i=0;

    node *temp, *previous, **traverse;

    traverse = malloc(sizeof(node*)*1000);
    temp = a;

    while(p >= 0)
    {
        apre[l++]=temp->data;

        if(temp->right != NULL && temp->left == NULL)
            temp = temp->right;

        else if(temp->right == NULL && temp->left != NULL)
            temp = temp->left;

        else if(temp->right != NULL && temp->left != NULL)
        {
            traverse[p++] = temp->right;
            //printf("%d", traverse[p-1]->data); checking that the proper nodes are pushed
            temp = temp->left;
        }
        else
            temp = traverse[--p];
    }

    p=0;// not going to free traverse until the end so that i can just overwrite the values
    temp = b;//now we do the same for b to find its post order traversal

    while(p >= 0)
    {
        bpre[m++]=temp->data;

        if(temp->right != NULL && temp->left == NULL)
            temp = temp->right;

        else if(temp->right == NULL && temp->left != NULL)
            temp = temp->left;

        else if(temp->right != NULL && temp->left != NULL)
        {
            traverse[p++] = temp->right;
            temp = temp->left;
        }
        else
            temp = traverse[--p];
    }

    p=0;
    temp = b;//now we will go through the tree rooted at b using postorder traversal

    while(p >= 0)
    {
        if(temp->right != NULL && temp->left == NULL)
        {
            if(temp->right == previous)
            {
                bpost[n++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                traverse[p++] = temp;
                temp = temp->right;
            }
        }

        else if(temp->right == NULL && temp->left != NULL)
        {
            if(temp->left == previous)
            {
                bpost[n++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                traverse[p++] = temp;
                temp = temp->left;
            }
        }

        else if(temp->right != NULL && temp->left != NULL)
        {
            if(temp->right == previous)
            {
                bpost[n++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                if(temp != traverse[p-1])
                    traverse[p++] = temp;
                traverse[p++] = temp->right;
                temp = temp->left;
            }

        }
        else
        {
            bpost[n++] = temp->data;
            previous = temp;
            temp = traverse[--p];
        }
    }

    p=0;
    temp = a;//now we will go through the tree rooted at a using postorder traversal

    while(p >= 0)
    {
        if(temp->right != NULL && temp->left == NULL)
        {
            if(temp->right == previous)
            {
                apost[o++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                traverse[p++] = temp;
                temp = temp->right;
            }
        }

        else if(temp->right == NULL && temp->left != NULL)
        {
            if(temp->left == previous)
            {
                apost[o++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                traverse[p++] = temp;
                temp = temp->left;
            }
        }

        else if(temp->right != NULL && temp->left != NULL)
        {
            if(temp->right == previous)
            {
                apost[o++] = temp->data;
                previous = temp;
                temp = traverse[--p];
            }
            else
            {
                if(temp != traverse[p-1])
                    traverse[p++] = temp;
                traverse[p++] = temp->right;
                temp = temp->left;
            }

        }
        else
        {
            apost[o++] = temp->data;
            previous = temp;
            temp = traverse[--p];
        }
    }

    apre[l] = '\0';
    bpre[m] = '\0';
    bpost[n] = '\0';
    apost[o] = '\0';

    if(strcmp(apre,bpost) == 0 || strcmp(bpre, apost) == 0)
        return 1;
    else
        return 0;
        */
}
double difficultyRating(void)
{
    return 2.5; // the logic for the traversals took about two hours to come up with. most time was spent
                // coding up the linkedlist format to hold the traversals
}

double hoursSpent(void)
{
    return 6.0; //cut out a decent amount of time by using the profs linked list code
}


