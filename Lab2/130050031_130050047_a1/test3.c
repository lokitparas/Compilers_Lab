int main(){
    int m,n,beg,end,midr,midc,e,found;
    int i, j;
    int a[200][200];

    found = 0;
    for(i=0;i<m;i++)
     for(j=0;j<n;j++)
       x =scanf("%d", a[j]);
    x =scanf("%d", e);
    beg=0;
    end=m-1;
    while(beg<=end){
     midr=(beg+end)/2;
     if(e<a[midr])
       end=midr-1;
     else if(e>=a[midr][e] && e<=a[midr][f])
     {
       found=1;
     }
     else
       beg=midr+1;
    }
    if(found==0)
     x =printf("Not found");
    else{
     beg=0;end=n-1;found=0;
     while(beg<=end){
       midc=(beg+end)/2;
       if(e<a[midr])
         end=midc-1;
       else if(e==a[midr])
       {
         found=1;
         
       }
       else
         beg=midc+1;
     }
     if(found==0)
       x =printf("Element not found");
     else {;}
    }
}