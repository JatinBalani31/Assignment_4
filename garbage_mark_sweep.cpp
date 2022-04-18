#include<iostream>

using namespace std;
struct node
{
    int data;
    bool mark;
    struct node* adjacent1;
    struct node* adjacent2;
    struct node* adjacent3;
    node(int num)
    {
        data = num;
        adjacent1=NULL;
        adjacent2=NULL;
        adjacent3=NULL;
        mark=false;
    }
}; 																//3 connections at max

typedef struct root_node_tag
{
    node* pointer;
}root_node;

node* heap[8];

void initialize(root_node& root_node1,root_node& root_node2);
void print_node(const node* node);
void print_useful_heap(const root_node& value);
void mark_node(node* ptr);
void mark(const root_node& value);
void garbage_collector(const root_node& r1,const root_node& r2,node** hp);
void sweep(node** array);
void print_heap(node** array);

int main()
{
    root_node A,B;
    initialize(A,B);
    cout<<"||||||Simulator for reference counting garbage collector |||||||||\n";
   
    cout<<"Navigate the printed table: - indicates connection and () indicate all elements connected to the element\n";
    cout<<"Full heap: "<<endl;
    
    print_heap(heap);
    cout<<"____________________________________________"<<endl;
   
    cout<<"Heap connected to the root : "<<endl;
    print_useful_heap(A);
    print_useful_heap(B);
   
    cout<<"____________________________________________"<<endl;
    garbage_collector(A,B,heap);
   
    cout<<"Garbage collector called, check the full heap\n";
    print_heap(heap);
   
    cout<<"Check if it matches heap connected to root printed\n";
   
    print_useful_heap(A);
    print_useful_heap(B);
}

void initialize(root_node& root_node1,root_node& root_node2)
{                                                   
    node* temp = new node(5);
    heap[0]=temp;
    temp = new node(1);
    heap[1]=temp;
    temp = new node(2);
    heap[2]=temp;
    temp = new node(9);
    heap[3]=temp;
    temp = new node(10);
    heap[4]=temp;
    temp = new node(7);
    heap[5]=temp;
    temp = new node(8);
    heap[6]=temp;
    temp = new node(3);
    heap[7]=temp;
    temp = NULL;

    //All nodes are in the heap,to create connections now

    root_node1.pointer=heap[0];					//root_node1->5
    heap[0]->adjacent1=heap[1];					//5->1
    root_node2.pointer=heap[1];					//root_node2->1
    heap[1]->adjacent1=heap[2];					//1->2
    heap[1]->adjacent2=heap[3];					//1->9
    heap[1]->adjacent3=heap[4];					//1->10
    heap[5]->adjacent1=heap[1];					//7->1
    heap[5]->adjacent2=heap[6];					//7->8
    heap[6]->adjacent1=heap[4];					//8->10
    heap[7]->adjacent1=heap[6];					//3->8
    heap[7]->adjacent2=heap[4];					//3->10
    											//linking completed 
}
void print_node(const node* node)
{
    if(node==NULL)
    {
        return;
    }
    cout<<" "<<node->data;
    if (node->adjacent1!=NULL || node->adjacent2!=NULL || node->adjacent3!=NULL)
    {
        cout<<"-(";
        print_node(node->adjacent1);
        print_node(node->adjacent2);
        print_node(node->adjacent3);
        cout<<" )";
    }
}

void print_heap(node** array)
{
    for(int i=0;i<8;i++)
    {
        if(array[i]!=NULL)
        {
            print_node(array[i]);
            cout<<endl;
        }
    }
}

void print_useful_heap(const root_node& value)
{
    cout<<"root_node->";
    print_node(value.pointer);
    cout<<endl;
}

void mark_node(node* ptr)
{
    node* head=ptr;
    node* tail=NULL;
    node* middle=NULL;
  
    int flag=1;
    while(head !=NULL)
    {
        if(!head->mark) 												//If current node is unmarked, mark it
        {
            head->mark=true;
        }
        if(head->adjacent1!=NULL && !head->adjacent1->mark) 			//If adjacent node to this is unmarked, point to it and mark it  
        {
            tail=middle;
            middle=head;
            head=head->adjacent1;
        }
        else if(head->adjacent2!=NULL && !head->adjacent2->mark)		//Moving ONLY along one adjacent path every iteration
        {
            tail=middle;
            middle=head;
            head=head->adjacent2;   
        }
        else if(head->adjacent3!=NULL && !head->adjacent3->mark)
        {
            tail=middle;
            middle=head;
            head=head->adjacent3;   
        }
        else
        {
            head=middle;
            middle=tail;
            tail=NULL;
        }
    }
}

void garbage_collector(const root_node& r1,const root_node& r2,node** hp)
{
    cout<<"Mark process started :"<<endl;
    
	mark(r1);
    mark(r2);
    
    cout<<"Marking Completed"<<endl;
    cout<<"Sweep process started :"<<endl;
    sweep(hp);
}

void mark(const root_node& value)
{
    mark_node(value.pointer);
}
void sweep(node** array)
{
    for(int i=0;i<8;i++)
    {
        if(array[i]!=NULL)
        {
            if(!array[i]->mark)
            {
                array[i]->adjacent1=NULL;							//Disconnect the nodes
                array[i]->adjacent2=NULL;
                array[i]->adjacent3=NULL;
                delete array[i];
                array[i]=NULL;
            }
        }
    }
}

