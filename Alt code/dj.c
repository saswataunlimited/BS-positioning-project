#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#define inf INT_MAX
#define max(a,b) (a>b?a:b)
#define PARENT(x) ((x-1)/2)
#define LEFT(x) (2*x+1)
#define RIGHT(x) (2*x+2)
typedef struct Pair{
    int key;
    int value;
}pair;
typedef struct Graph{
    int V;
    pair ***adj;
    int *deg;
    int *pos;
}graph;
typedef struct Priority_Queue{
    int heapsize;
    int *keypos;
    pair **queue;
}pqueue;
pair *create_pair(int k,int val){
    pair *p=(pair *)malloc(sizeof(pair));
    p->key=k;
    p->value=val;
    return p;
}
int read_vertices(char *s){
    int v=0,n1,n2,w;
    FILE *fp=fopen(s,"r");
    while(!feof(fp)){
        fscanf(fp,"%d %d %d\n",&n1,&n2,&w);
        v=max(v,max(n1,n2));
    }
    fclose(fp);
    return v;
}
graph *create_graph(int v){
    graph *g=(graph *)malloc(sizeof(graph));
    g->V=v;
    g->deg=(int *)calloc(v,sizeof(int));
    g->pos=(int *)calloc(v,sizeof(int));
    g->adj=(pair ***)malloc(v*sizeof(pair **));
    return g;
}
void update_deg(char *s,graph *g){
    int u,v,w;
    FILE *fp=fopen(s,"r");
    while(!feof(fp)){
        fscanf(fp,"%d %d %d\n",&u,&v,&w);
        g->deg[u-1]++;
        g->deg[v-1]++;
    }
    fclose(fp);
}
void update_list(char *s,graph *g){
    int u,v,w;
    for(v=0;v<g->V;v++)
        g->adj[v]=(pair **)malloc(g->deg[v]*sizeof(pair *));
    FILE *fp=fopen(s,"r");
    while(!feof(fp)){
        fscanf(fp,"%d %d %d\n",&u,&v,&w);
        g->adj[u-1][g->pos[u-1]++]=create_pair(v-1,w);
        g->adj[v-1][g->pos[v-1]++]=create_pair(u-1,w);
    }
    fclose(fp);
}
graph *preprocess_graph(char *s){
    int v=read_vertices(s);
    graph *g=create_graph(v);
    update_deg(s,g);
    update_list(s,g);
    return g;
}
pqueue *preprocess_pqueue(graph *g){
    pqueue *pq=(pqueue *)malloc(sizeof(pqueue));
    pq->heapsize=g->V;
    pq->keypos=(int *)malloc(g->V*sizeof(int));
    pq->queue=(pair **)malloc(g->V*sizeof(pair *));
    int v;
    pq->queue[0]=create_pair(0,0);
    pq->keypos[0]=0;
    for(v=1;v<g->V;v++){
        pq->queue[v]=create_pair(v,inf);
        pq->keypos[v]=v;
    }
    return pq;
}
void swap(pqueue *pq,int i,int j){
    pair *p1=pq->queue[i];
    pair *p2=pq->queue[j];
    pq->keypos[p1->key]=j;
    pq->keypos[p2->key]=i;
    pq->queue[i]=p2;
    pq->queue[j]=p1;
}
void min_heapify(pqueue *pq,int i){
    int smallest=i;
    if(LEFT(i) < pq->heapsize && pq->queue[LEFT(i)]->value < pq->queue[smallest]->value)
        smallest=LEFT(i);
    if(RIGHT(i) < pq->heapsize && pq->queue[RIGHT(i)]->value < pq->queue[smallest]->value)
        smallest=RIGHT(i);
    if(smallest != i){
        swap(pq,i,smallest);
        min_heapify(pq,smallest);
    }
}
void decrease_key(pqueue *pq,int v,int val){
    int i=pq->keypos[v];
    pq->queue[i]->value=val;
    while(i>0 && pq->queue[i]->value < pq->queue[PARENT(i)]->value){
        swap(pq,i,PARENT(i));
        i=PARENT(i);
    }
}
int empty(pqueue *pq){
    return pq->heapsize == 0;
}
pair *extract_min(pqueue *pq){
    if(empty(pq))
        return NULL;
    pair *p=pq->queue[0];
    swap(pq,0,pq->heapsize-1);
    pq->heapsize--;
    min_heapify(pq,0);
    return p;
}
int present_in_heap(pqueue *pq,int v){
    return pq->keypos[v] < pq->heapsize;
}
int getpos(graph *g,int v,int u){
    int i,flag=0;
    for(i=0;i<g->deg[u];i++)
        if(g->adj[u][i]->key==v)
            return i;
    assert(flag);
    return -1;
}
void write_to_file(char *s,graph *g,int *parent){
    FILE *fp=fopen(s,"w");
    int v,length=0;
    for(v=1;v<g->V;v++){
        int pos=getpos(g,v,parent[v]);
        fprintf(fp,"%c %c %d\n",(char)(v+1+64),(char)(parent[v]+1+64),g->adj[parent[v]][pos]->value);
        length+=g->adj[parent[v]][pos]->value;
    }
    fprintf(fp,"Length of MST = %d\n",length);
    fclose(fp);
}
void dijkstra(graph *g,pqueue *pq,int s){
    int i,v,w,pos;
    pair *p;
    while((p=extract_min(pq)))
        for(i=0;i<g->deg[p->key];i++){
            v=g->adj[p->key][i]->key;
            w=g->adj[p->key][i]->value;
            pos=pq->keypos[v];
            if(present_in_heap(pq,v) && w + pq->queue[pq->keypos[p->key]]->value < pq->queue[pos]->value)
                decrease_key(pq,v,w+pq->queue[pq->keypos[p->key]]->value);
        }
    FILE *fp=fopen("Dijkstra.txt","w");
    for(v=0;v<g->V;v++)
        fprintf(fp,"Distance of Node %d to Node %d = %d\n",v+1,s+1,pq->queue[pq->keypos[v]]->value);
    fclose(fp);
}
int main()
{
    graph *g=preprocess_graph("Data.txt");
    pqueue *pq=preprocess_pqueue(g);
    dijkstra(g,pq,0);
    return 0;
}
