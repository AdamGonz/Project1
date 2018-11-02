#include<string>
#include<iostream>
#include<fstream>
#include<ctype.h>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include<numeric>
#include<math.h>

using std::string;
using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::tolower;
using std::vector;
using std::iota;
using std::swap;


void readFromFile(string &S, string filename);
void convertToLower(string &S);
bool lessThan(const string &S, int first, int second);
int partition(const string &S, vector<int> &indices, int low, int high, int pivotIndex);
void quicksort(const string &S, vector <int> &A, int low, int high);
void insertion(const string &S, vector<int> &indices, int low, int high);
int selection(const string &S, vector <int> &indices, int start, int final, int k);


int main(int argc, char* argv[]){
    string file = argv[1];
    string S;
    readFromFile(S, file);
    convertToLower(S);

    if(argc < 2){
        cerr << "ERROR: Provide more arguments." << endl;
    }
    if(argc == 2){
        vector <int> part(S.length());
        iota(part.begin(), part.end(), 0);
        quicksort(S, part, 0, S.length() - 1);
        for(int i = 0; i < part.size(); i++){
            cout << part[i] << " ";
        }
        cout << endl;
    }
    else if(argc == 3){
        vector <int> select(S.length());
        iota(select.begin(), select.end(), 0);
        int k = atoi(argv[2]);
        int final = selection(S, select, 0, select.size() - 1,k);
        cout << final << endl;
        return 0;
    }
    else if(argc == 4){
        vector <int> inser(S.length());
        iota(inser.begin(), inser.end(), 0);

        int low = atoi(argv[2]), high = atoi(argv[3]);
        insertion(S, inser, low, high);

        for(int i = 0; i < inser.size(); i++){
            cout << inser[i] << " ";
        }
        cout << endl;
    }
    else{
        return -1;
    }
    return 0;
}
void readFromFile(string &S, string filename){
    ifstream in;
    in.open(filename.c_str(), ios::in);
    if(!in){
        cerr << "ERROR: could not open " << filename << endl;
        return;
    }
    string line;
    while(getline(in,line)){
        S = S + line;
    }
    in.close(); in.clear();
}

void convertToLower(string &S){
    char c;
    for(int i = 0; i < S.length(); i++){
        c = S[i];
        S[i] = tolower(c);
    }
    return;
}

bool lessThan(const string &S, int first, int second){
    while(true){
        if(first == S.length() || second == S.length())
            break;

        if(S[first] < S[second]){
            return true;
        }
        else if(S[first] > S[second]){
            return false;
        }
        else if(S[first] == S[second]){
            first++;
            second++;
        }
    }
    if(first > second){
        return true;
    }
    else{
        return false;
    }
}


int partition(const string &S, vector<int> &indices, int low, int high, int pivotIndex){
    int tempo = indices[pivotIndex];
    indices[pivotIndex] = indices[high];
    indices[high] = tempo;
    pivotIndex = high;
    int i = low, j = high - 1;
    while(i <= j){
        while(i < high && lessThan(S, indices[i], indices[pivotIndex])){
            i++;
        }
        while(j >= low && !lessThan(S, indices[j], indices[pivotIndex])){
            j--;
        }
        if(i < j){
            int temp = indices[i];
            indices[i] = indices[j];
            indices[j] = temp;
            i++;
            j--;
        }
    }
    int temp = indices[high];
    indices[high] = indices[i];
    indices[i] = temp;
    return i;
}

void quicksort(const string &S, vector <int> &A, int low, int high){
    if(low < high){
        int pivotIndex = partition(S, A, low, high, high);
        quicksort(S, A, low, pivotIndex - 1);
        quicksort(S, A, pivotIndex + 1, high);
    }
}

void insertion(const string &S, vector<int> &indices, int low, int high){
    int key, i;

    for(int j = low + 1; j <= high; j++){
        key = indices[j];
        i = j - 1;
        while(i >= low && lessThan(S, key, indices[i])){
            indices[i + 1] = indices[i];
            i = i - 1;
        }
        indices[i + 1] = key;
    }
}

int selection(const string &S, vector <int> &indices, int start, int final, int k){
    
    int indices_size = final - start + 1;

    if(start == final)
        return indices[start];
    else if(start > final)
        return -1;
    
    if(indices_size <= 5){
        insertion(S, indices, start, final);
        return indices[k];
    }
    
    int msize = ceil((double)indices_size/5);

    vector<int> medians(msize, 0);
    int med_ind = 0;
    for(int i = start; i <= final; i = i + 5){
        vector<int> temp(5, 0);
        int j = 0;
        for(; j < 5 && (i + j)<= final; j++){
            temp[j] = indices[i+j];
        }
        if(j < 5)
            temp.resize(j);
        insertion(S, temp, 0, temp.size() - 1);
        if(j == 5)
            medians[med_ind++] = temp[2];
        else{
            int jmid = j >> 1;
            if(j % 2 == 0)
                jmid--;
            medians[med_ind++] = temp[jmid];

            }
    }

    int mid = msize >> 1; 
    if((msize % 2) == 0)
        mid--;

    int x = selection(S, medians, 0, msize - 1, mid);
    if(x == -1)
    {
        cerr << "ERROR: returned value of median of medians is -1." << endl;
        exit(0);
    }
    int ind_of_x = -1;
    for(int i = start; i <= final; i++){
        if(indices[i] == x){
            ind_of_x = i;
            break;
        }
    }

    int pivotIndex = partition(S, indices, start, final, ind_of_x);

    if(k == pivotIndex)
        return indices[k];
    else if(pivotIndex < k)
        return selection(S, indices, pivotIndex + 1, final, k);
    else
        return selection(S, indices, start, pivotIndex - 1, k);

}


