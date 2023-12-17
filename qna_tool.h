#include <iostream>
#include <fstream>
#include "Node.h"

#include<vector> // remove this
#include<string>
using namespace std;

static vector<int>mergeVector(vector<int>&v1, vector<int>&v2){

        vector<int>ans;
        int i=0;
        int j=0;
        while(i<v1.size() && j<v2.size()){

            if(v1[i]<v2[j]){
                ans.push_back(v1[i]);
                i++;
            }

            else if(v1[i]== v2[j]){
                 ans.push_back(v1[i]);
                i++;
                j++;
            }

            else{
                 ans.push_back(v2[j]);
                j++;
            }

        }

        while(i<v1.size()){
            ans.push_back(v1[i]);
            i++;
        }
    
        while(j<v2.size()){
            ans.push_back(v2[j]);
            j++;
        }
        return ans;
    };


class Data{

    public:
    
    int page_no, book_code, para_no;
    float score;
    bool ispresent;
    vector<int> wordvector;

    Data(int bookcode, int page, int paragraph){
        book_code = bookcode;
        page_no = page;
        para_no = paragraph;
        ispresent = false;
        score =0;
    }

    void pirntData(){
        cout << "[" << book_code << "," << page_no <<"," <<para_no << ","<<score  << "]"<< endl;
    }

    vector<int> getWordvector(){
        return wordvector;
    }

    ~Data(){};
};

class Element{

    public:
    string word;
    vector<Data*>vec;

    Element(){
        word = "";
    };

    Element(string w){
        word= w;
    }

    ~Element(){
        for(int i=0;i<vec.size();i++){
            delete vec[i];
        }
        vec.clear();
    };
};


class HubNode{

    public:
    float hubscore;
    vector<Data*>datalist;
    vector<int>hubwordvector;

    HubNode(){
        hubscore =0;
    }

    void insertHubdata(Data*d){
        hubscore += d->score;
        datalist.push_back(d);
        hubwordvector =mergeVector(hubwordvector,d->wordvector);
    }

    
    ~HubNode(){
    }
};

class Algorithms{

    public:

        static bool Seperate(char ch, string&seperators){
        for(int i=0;i<seperators.size();i++){
            if(ch==seperators[i]){
                return true;
            }
        }
        return false;

        }

       static vector<string>generateWordList(string sentence){

        sentence+=' ';
        vector<string>WordList;
        string word = "";
        bool flag= false;

        for(int i =0;i<sentence.size();i++){
            char ch = sentence[i];
            string seperators = ". , - : ! \' \" ( ) ?—[]“”‘’˙;@";
            bool isPresent = Seperate(ch,seperators);
            ch = tolower(ch);
            if(isPresent == false){
                word+=ch;
                flag = true;
            }
    
            else{
                if(flag == true){
                    WordList.push_back(word);
                }
                    word="";
                    flag= false;
            }
        }
    return WordList;
    }

    static int getHashcode(string word){

        int code =0;
        for(int i=0;i<word.size();i++){
            code+=word[i];
        }
        return code;

    }

    static int triplehash(int a, int b, int c, int&m){
        return (a*4+ b*2+c)%m;
    }



    static long long int scoreofword(string mainword){
       ifstream file;
       long long int result =0;
       file.open("unigram_freq.csv");
       string line;
       getline(file, line);
       while (getline(file, line)){

           string word ="";
           
           int i=0;
           while(line[i]!=','){
               word+=line[i];
               i++;
           }
           i++;

           if(word == mainword && int(line[i])>=48 && int(line[i])<=57){
           while(i<line.size()){
              long long int number = line[i]-'0';
              result = result*10 + number;
               i++;
           }
             file.close();
             return result;
           }
       }
       return 0;
    }

    // merge for datas
    static void merge(std::vector<Data*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temporary subarrays
        std::vector<Data*> leftSubarray;
        std::vector<Data*> rightSubarray;

        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }

        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (leftSubarray[i]->score >= rightSubarray[j]->score) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }

    static void mergeSort(std::vector<Data*>& arr, int left, int right){
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    //merge sort for hubs :
    static void mergeHUB(vector<HubNode*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temporary subarrays
        vector<HubNode*> leftSubarray;
        vector<HubNode*> rightSubarray;

        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }

        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (leftSubarray[i]->hubscore >= rightSubarray[j]->hubscore) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }


    static bool compare(Data* d1, Data* d2){

        if(d1->book_code == d2->book_code){
            if(d1->page_no <= d2->page_no){
                return true;
            }
            return false;
        }

        else if(d1->book_code <= d2->book_code){
            return true;
        }

        return false;
    }


    static void mergeHUBSort(std::vector<HubNode*>& arr, int left, int right){
        if (left < right){
            int mid = left + (right - left) / 2;
            mergeHUBSort(arr, left, mid);
            mergeHUBSort(arr, mid + 1, right);
            mergeHUB(arr, left, mid, right);
        }
    }

    // mrege sort function for data but in different way:
    static void mergeDifferent(vector<Data*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<Data*> leftSubarray;
        vector<Data*> rightSubarray;

        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }

        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (!compare(leftSubarray[i],rightSubarray[j])) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }

    static void mergeSortdifferent(std::vector<Data*>& arr, int left, int right){
        if (left < right){
            int mid = left + (right - left) / 2;
            mergeSortdifferent(arr, left, mid);
            mergeSortdifferent(arr, mid + 1, right);
            mergeDifferent(arr, left, mid, right);
        }
    }


    static bool isPresent(string word, vector<string>junkwords){
        for(int i=0;i<junkwords.size();i++){
            if(word == junkwords[i]){
                return true;
            }
        }
        return false;
    }


    static vector<string> modifyQuery(string question ){

        vector<string>junkword = {"the","of","and","to","a", "in","for","is","on","that","by","this","with","i","you","it","not","or","be","are","from","at","as","your","all","have","an","was","will","us"};

        vector<string>emp;
        vector<string> wordlist = generateWordList(question);
        for(int i=0;i<wordlist.size();i++){
            if(!isPresent(wordlist[i],junkword)){
                emp.push_back(wordlist[i]);
            }
        }
    return emp;
    }

    

    static int differnce(int x, int y){
        if(x>y){
            return x-y;
        }
        return y-x;
    }


    static vector<HubNode*> hubMaker(vector<Data*>datavector){

        vector<HubNode*>hubresults;
        HubNode* hub1 = new HubNode();
        hub1->insertHubdata(datavector[0]);
        hubresults.push_back(hub1);
        

        for(int i=1;i<datavector.size();i++){

        
            if(datavector[i]->book_code == datavector[i-1]->book_code && differnce(datavector[i-1]->page_no, datavector[i]->page_no) < 2){

               hubresults.back()->insertHubdata(datavector[i]);

            }

            else{
                HubNode* newhubnode = new HubNode();
                newhubnode->insertHubdata(datavector[i]);
                hubresults.push_back(newhubnode);
            }
        }
        return hubresults;
    }


    static void updateHub(vector<HubNode*>&hubvector, int&n){

        for(int i=0;i<hubvector.size();i++){

            if(hubvector[i]->hubwordvector.size() == n){
                hubvector[i]->hubscore *= 2;
            }

        }
    }
};









class Chaining{

    private:
    int max_size;

    public:
    vector<Element*>*word_storage;
    vector<Data*>*data_storage;

    Chaining(){
        max_size = 1000000;
        word_storage = new vector<Element*>[max_size];
        data_storage = new vector<Data*>[max_size];
    }

    Data* insertData(int book_code, int page_no, int para_no){
        int hashcode = Algorithms::triplehash(book_code,page_no,para_no,max_size);
        Data* newData = new Data(book_code,page_no,para_no);
        data_storage[hashcode].push_back(newData);
        return newData;
    }

    Data* searchData(int book_code, int page_no, int para_no){
        int hashcode = Algorithms::triplehash(book_code,page_no,para_no,max_size);
        vector<Data*> subvector = data_storage[hashcode];
        for(int i=0;i<subvector.size();i++){
            if(subvector[i]->book_code == book_code && subvector[i]->page_no == page_no && subvector[i]->para_no == para_no){
                return subvector[i];
            }
        }
        return NULL;
    }

    void insertElement(string word, int book_code, int page_no, int para_no){
       int hashcode = Algorithms :: getHashcode(word);
       for(int i=0;i<word_storage[hashcode].size();i++){
        if(word_storage[hashcode][i]->word == word){
            if (searchData(book_code,page_no,para_no)==NULL){
                word_storage[hashcode][i]->vec.push_back(insertData(book_code,page_no,para_no));
            }
            else{
                word_storage[hashcode][i]->vec.push_back(searchData(book_code,page_no,para_no));
            }
            return;
        }
       }
        Element *e = new Element(word);
        if (searchData(book_code,page_no,para_no)==NULL){
            e->vec.push_back(insertData(book_code,page_no,para_no));
        }
        else{
            e->vec.push_back(searchData(book_code,page_no,para_no));
        }
        word_storage[hashcode].push_back(e);
    }
    
    vector<Data*> searchElements(string word){
        vector<Data*>emp;
        int hashcode = Algorithms :: getHashcode(word);
        vector<Element*>subvector = word_storage[hashcode];
        for(int i=0;i<subvector.size();i++){
            if(subvector[i]->word == word){
               return subvector[i]->vec;
            }
        }
        return emp;
    }
};



class QNA_tool {

private:
    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY);
    // You can add attributes/helper functions here
public:
    Chaining*document;
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};


