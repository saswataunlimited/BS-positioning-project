#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct list2{
    char v1;
    int wt; //weight of an edge
};
typedef struct list2 lst2;

struct list1{
    char v;
    int distance;
    char previous;
    int marker;
    int deg;
    lst2 *l2;
};
typedef struct list1 lst1;

int parent(int i)
{
    if(i%2 == 0)
        return (i-1)/2;
    else
        return (i/2)-1;
}

int left(int i)
{
    return (2*i)+1;
}

int right(int i)
{
    return (2*i)+2;
}

void max_heapify(lst1 *a, int i,int heapsize)
{
    int l = left(i);
    int r = right(i);
    int largest;
    if(l<=heapsize && a[l].distance>a[i].distance)
        largest = l;
    else
        largest = i;
    if(r<=heapsize && a[r].distance>a[largest].distance)
        largest = r;
    if(largest != i)
    {
        lst1 temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        max_heapify(a,largest,heapsize);  //Changed here //Success
    }
    //max_heapify(a,largest,heapsize);  //possible flaw ***
}

void Build_max_heap(lst1 *a,int heapsize)
{
    int i;
    for(i=(floor((heapsize+1)/2))-1;i>=0;--i)   //possible flaw ***  //Changed  //Success
        max_heapify(a,i,heapsize);
}

lst1 *heap_extract_max(lst1 *a,int *heapsize_p)
{
    if(*heapsize_p<1)
    {
        printf("\nHeap Underflow!!\n");
        //exit(0);
        return NULL;
    }
    //lst1 *b = (lst1 *)malloc(sizeof(lst1));
    lst1 *b = &a[0];
    b.marker = 1;
    a[0] = a[*heapsize_p-1];
    *heapsize_p -= *heapsize_p;
    max_heapify(a,0,*heapsize_p);
    return b;
}

int _search(lst1 *a,char x,int n)
{
    int i;
    for(i=0;i<n;++i)
    {
        if( a[i].v == x )
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int n;
    printf("How many vertices:\n");
    scanf("%d",&n);
    lst1 *l1 = (lst1 *)malloc(n*sizeof(lst1));
    if (l1 == NULL)
        exit(0);
    printf("Enter your vertices one by one:\n");
    int i;
    for(i=0;i<n;++i)
    {
        char a;  //temporary
        getchar();
        scanf("%c",&a);
        l1[i].v = a;
    }
    printf("Just showing the results:\n");
    for(i=0;i<n;++i)
        printf("%c\n",l1[i].v);

    //Initializing each and every vertex
    for(i=0;i<n;++i)
    {
        l1[i].distance = 99999;
        l1[i].previous = '\0';
        l1[i].marker = 0;
    }
    printf("\nEnter child nodes for each one:\n");
    for(i=0;i<n;++i)
    {
        int k;
        printf("How many children for %c node:\n",l1[i].v);
        scanf("%d",&k);
        l1[i].deg = k;
        l1[i].l2 = (lst2 *)malloc(k*sizeof(lst2));
        if(l1[i].l2 == NULL)
            exit(0);
        int j;
        printf("\nNow input the child nodes and their weights one by one:\n");
        for(j=0;j<k;++j)
        {
            char ch;
            int a;
            getchar();
            scanf("%c%d",&ch,&a);
            (l1[i]).l2[j].v1= ch;
            (l1[i]).l2[j].wt = a;
        }
    }
    printf("\nInput taken complete. Let's check...\n");
    for(i=0;i<n;++i)
    {
        printf("%c-%d->->",l1[i].v,l1[i].deg);
        int j,k = l1[i].deg;
        for(j=0;j<k;++j)
        {
            printf("%c(%d)->",(l1[i]).l2[j].v1, (l1[i]).l2[j].wt );
        }
        printf("X\n");
    }

    //heap creation
    /* char *vertices = (char *)malloc(n*sizeof(char));
    if(vertices == NULL)
        exit(0);
    for(i=0;i<n;++i)
        vertices[i] = l1[i].v;*/

    printf("\nWhich one is your source:\n");
    char sv;  //sv stands for source vertex
    scanf("%c",&sv);
    for(i=0;i<n;++i)
    {
        if(l1[i].v == sv){
            l1[i].distance = 0;    //setting the source vertex
            break;
        }
    }
    Build_max_heap(l1,n);

    printf("\nHeap creation done. Let's check if any changes...\n");
    for(i=0;i<n;++i)
    {
        printf("%c-%d->->",l1[i].v,l1[i].deg);
        int j,k = l1[i].deg;
        for(j=0;j<k;++j)
        {
            printf("%c(%d)->",(l1[i]).l2[j].v1, (l1[i]).l2[j].wt );
        }
        printf("X\n");
    }
    int heapsize = n;
    int *heapsize_p = &heapsize;
    printf("\nLet's go Dijkstra:\n");
    for(i=0;i<n;++i)       //list is not empty:
    {
        if(l1[0].marker == 1)
            break;
        lst1 *l3 = heap_extract_max(l1,heapsize_p);
        if(l3 == NULL)
        {
            printf("\nEvery node is visited\nStill the iteration is running\nSOMETHING WRONG IN CODE>>CHECK\n");
            exit(0);
        }
        l3.marker = 1; //Marking the vertex as known, also marked it previously in the extract-max fn
        int j;
        for(j=0;j<l3.deg;++j)
        {
            int dist = l3.distance + l3.l2[j].wt;
            int k = _search(l1, l3.l2[j].v1, n );
            if(k == -1)
            {
                printf("\nSomething's wrong!!\n");
                exit(0);
            }
            lst1 *neighbour = l1[k];
            if(dist < neighbour.distance)
            {
                neighbour.distance = dist;
                neighbour.previous = l3.v;
            }
        }
    }

    //Upto here (strating from "Let's go Dijkstra") expecting that the distances are stored properly
    //Let's check
    for(i=0;i<n;++i)
    {
        printf("\nVertex:\'%c\' - dist from source:\'%d\' - Djk predecessor:\'%c\' \n",l1[i].v,l1[i].distance,l1[i].previous);
    }
    printf("\nAnd that\'s it...THE END\n");

    /*int j;
    for(i=0;i<n;++i)
    {
        for(j=0;j<(l1[i].deg);++j)
        {
            free((l1[i]).l2[j]);
        }
        free(l1[i]);
    }
    free(l1);*/
    return 0;
}
