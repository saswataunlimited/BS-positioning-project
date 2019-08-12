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
    }
    max_heapify(a,largest,heapsize);  //possible flaw ***
}

void Build_max_heap(lst1 *a,int heapsize)
{
    int i;
    for(i=floor(heapsize/2);i>=0;--i)   //possible flaw ***
        max_heapify(a,i,heapsize);
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
    /*char *vertices = (char *)malloc(n*sizeof(char));
    if(vertices == NULL)
        exit(0);
    for(i=0;i<n;++i)
        vertices[i] = l1[i].v;*/
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
