#include<algorithm>
#include<math.h>
#include<vector>
#include<iostream>
#include<string>
using namespace std;


//Initially, start = 0, fin = v.size()-1
//Returns the k-th element in v if the elements were
//sorted in ascending order
int select(vector<int> &v, int start, int fin, int k);

//initializes v with integers from standard input cin
void init(vector<int> &v);

//partitions elements of A around element A[index]
int partition(vector<int> &A, int low, int high, int index);

int main(){
    vector<int> v;   
    init(v);
    int vsize = v.size();
	for(int i = 0; i < vsize; i++){
		//Find the i-th element in the vector
		int KthItem = select(v, 0, v.size()-1, i);
		cout << KthItem << ", " << i << "-th element of v" << endl;
	}//for
}//main

//Returns the k-th element in increasing order (count starts with 0)
int select(vector<int> &v, int start, int fin, int k){

    int vsize = fin - start + 1;

    if(start == fin)
        return v[start];
    else if(start > fin)
        return -1;
	
	if(vsize <= 5){
        //Use "Insertion sort" instead of std::sort
		sort(v.begin() + start, v.begin() + fin + 1);
		return v[k];
	}
	
	int msize = ceil((double)vsize/5);

    vector<int> medians(msize, 0);
    int med_ind = 0;
    for(int i = start; i <= fin; i = i + 5){
        vector<int> temp(5, 0);
        int j = 0;
        for(; j < 5 && (i + j)<= fin; j++){
            temp[j] = v[i+j];
        }//for j
        if(j < 5)
            temp.resize(j);
        //use insertion sort instead of std::sort
        sort(temp.begin(), temp.end());
        if(j == 5)
            medians[med_ind++] = temp[2];
        else{
            int jmid = j >> 1;//divide by 2
            if(j % 2 == 0)
                jmid--;
            medians[med_ind++] = temp[jmid];

            }
    }//for i

    //so far we selected the medians of n/5 groups, each group of 5 elements
    int mid = msize >> 1; //divide by 2
    if((msize % 2) == 0)
        mid--;

	//find the median of the medians
    int x = select(medians, 0, msize - 1, mid);
	if(x == -1)
	{
		cerr << "ERROR: returned value of median of medians is -1." << endl;
		exit(0);
	}

    //partition elements of v around x
    //find the index of x in v
    int ind_of_x = -1;
	for(int i = start; i <= fin; i++){
		if(v[i] == x){
			ind_of_x = i;
			break;
		}//if
	}//for

    int pivotIndex = partition(v, start, fin, ind_of_x);

    if(k == pivotIndex)
        return v[k];
    else if(pivotIndex < k)
        return select(v, pivotIndex + 1, fin, k);
    else
        return select(v, start, pivotIndex - 1, k);
}//select

int partition(vector<int> &A, int low, int high, int index){
	//swap element at "index" and element at "high" (the last in the range)
	int temp = A[index];
	A[index] = A[high];
	A[high] = temp;

	int pivot = A[high];
	int i = low, j = high - 1;
	while(i <= j){
		while(i < high && A[i] <= pivot)
			i++;
		while(j >= low && A[j] > pivot)
			j--;
		if(i < j){//swap A[i] and A[j]
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++;
			j--;
		}//if
	}//while
	//swap A[i] and pivot
	A[high] = A[i];
	A[i] = pivot;
	return i;
}

void init(vector<int> &v){
    int x;
    while(cin >> x)
        v.push_back(x);

}

