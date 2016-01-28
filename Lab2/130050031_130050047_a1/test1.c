int merge(int a[100], int b[100], int out[100], int n, int m) {
    int i, j, k;
    int res;
    i = j = k = 0;
    i = 0;
    res = 0;
    while(i < n || j < m) {
   	 if(i < n) {
   		 if(j < m) {
   			 if(a[i] < b[j]) {
   				 out[k] = a[i];
   				 /* i++;*/
   			 } else {
   				  out[k] = b[j];
   				 j = j + 1;
   				 res = res + (n - i);
   			 };
   		 } else {
   			 out[k] = a[i];
   			 /* i++; */
   		 }
   	 } else if(j < m) {
   		  out[k] = b[j];
   		 j = j + 1;
   	 }
     else{
      i=0;
     }
    }
    return res;
}

int inv_pairs (int a[100], int start, int end) {
    int mid;
    int temp[100];
    int i, left, right, cross;
    
    if(start == end) {
     return 0;
    }
    else{;}
    mid = (end + start) / 2;

    left = inv_pairs(a, start, mid);
    right = inv_pairs ( a, mid + 1, end);

    cross = merge(a + start, a + mid + 1, temp, mid - start + 1, end - mid);
    
    for(i = 0; i < (end - start + 1); i++) {
     a[i + start] = temp[i];
    }

    return cross + left+ right;
}

int main() {
    int ans, i, a[10];

    for ( i = 0; i < 10; i++) {
     a[i] = 10 - i;
    }
    
    ans = inv_pairs(a, 0, 10);

    return 0;
}
