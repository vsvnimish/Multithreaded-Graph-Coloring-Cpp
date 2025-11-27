#include<iostream>
#include<semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
#include <sys/time.h>
#include<fstream>
using namespace std;
long long row,col,threads;
int** graph; //this is base pointer for adjacency matrix
long long* color; //this is used to keep track of color of every vertex
long long* edges; //this to keep track of no of edges that a particular vertex is incident for suppose if vertex v is part of 5 edges then edges[v] will be 5
sem_t mutex; 
struct list{
	list* next;
	long long data;
};
list** graph_list; //this is base pointer for  adjacency list
list* add_at_start(list* head,long long value){ //this function will add an element to the front of the list
	list *temp = new list();
	temp->data=value;
	temp->next=head;
	return temp;
}
void input(){ //this function is for taking inputs and for making graphs
	cin>>threads>>row;
	col=row;
	long long i,j;
	graph = new int*[row];
	color =new long long [row];
	edges =new long long [row];
        graph_list=new list*[row];
        for (i = 0; i < row; i++){
          graph[i] = new int[col];
          graph_list[i]=NULL;
        }
        for(i=0;i<row;i++){
    	  for(j=0;j<col;j++){
    	    cin>>graph[i][j];
    	    if(graph[i][j]==1){
    	        edges[i]++;
                graph_list[i]=add_at_start(graph_list[i],j);
            }
            else if(i==j)
                graph_list[i]=add_at_start(graph_list[i],j);
          }
    	  color[i]=-1;   
	}    
}
void color_vertex(long long vertex){//this function is for coloring a vertex v using greedy method
	 bool available[row];//this array will tell whether a particular color is available or not
	 long long i;
     for(i=0;i<row;i++)
	   available[i]=true; //make all colors as available
     list* head=graph_list[vertex]; //get starting location of list of vertices for which vertex v is adjacent 
	 while(head!=NULL){
        if(head->data!=vertex){
	    	if(color[head->data]!=-1){
	    		available[color[head->data]]=false; //make all neighbour's color of vertex v as unavailable
			}
        }
        head=head->next;
	 }
	 for(i=0;i<row;i++){
	 	if(available[i]==true){
	 		 color[vertex]=i; //for vertex v we will assign a available color with a minimum value 
	 		 break;
		}
	 }	  
}
void color_external(long long vertex){
     sem_wait(&mutex); //this mutex is common lock ,we will keep a lock
     color_vertex(vertex); //color the vertex
     sem_post(&mutex); //unlock
}
void* test(void* tem){
 list* head=(list*)tem;
 list* store=head;
 list* tail=head;
 long long count=0;//this is to keep track of number of neighbours that a vertex is having in this partion
 while(head!=NULL){
 	while(tail!=NULL){
 		if(graph[head->data][tail->data]==1){
 			count++;
		}
		tail=tail->next;
	}
	tail=store;
	if(count==edges[head->data]){//if all neigbours of vertex are there in this partion itself then this is internal vertex
		color_vertex(head->data);
	}
	else{  //external vertex
	  color_external(head->data);
	}
	head=head->next;
	count=0;
 }
 return NULL;     
} 
int main(){
   srand(time(0));
   input();
   long long i,max=-1;
   struct timeval tp,tp1;
   sem_init(&mutex,0,1);//intialize semaphore to 1
   long long m;
   list* part[threads];//this to maintain a list of partions of every thread
   for(i=0;i<threads;i++){
        part[i]=NULL;
   }
   for(i=0;i<row;i++){
    	m=rand()%threads; //for every vertex we will select a random thread to partion
    	part[m]=add_at_start(part[m],i);//add that vertex to partion of thread m
   }      
   pthread_t t[threads];
   gettimeofday(&tp, NULL);
   double ms = tp.tv_sec * 1000 + tp.tv_usec / 1000; 
   for(i=0;i<threads;i++)
      pthread_create(&t[i],NULL,test,part[i]);//send the partion as argument for a thread
   for(i=0;i<threads;i++)
      pthread_join(t[i],NULL);//wait for all threads to finish their job
   gettimeofday(&tp1, NULL);
   double sm = tp1.tv_sec * 1000 + tp1.tv_usec / 1000;
   for(i=0;i<row;i++){
   	  if(color[i]>max)
   	     max=color[i]; //get maximum number of colors that were used
   }
   cout<<"Coarse Lock\n";
   cout<<"No. of colours used: "<<max+1<<"\n";
   cout<<"Time taken by the algorithm using: "<<sm-ms<<" Millisecond\n";
   cout<<"Colours : ";
   for(i=0;i<row-1;i++)
       cout<<"v"<<i+1<<" - "<<color[i]+1<<", ";
   cout<<"v"<<i+1<<" - "<<color[i]+1<<"\n";
    //cout<<sm-ms<<"\n";
    //cout<<max<<"\n";    
	return 0;
}
