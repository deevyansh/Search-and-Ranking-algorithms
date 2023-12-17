#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

void print1(vector<Element *>*e, int max_size = 1000000){
    for(int i=0;i<max_size;i++){
        for(int j=0;j<e[i].size();j++){
            cout << e[i][j]->word << " => ";
            for(int k=0;k<e[i][j]->vec.size();k++){
                cout << "[" << e[i][j]->vec[k]->book_code <<","<<e[i][j]->vec[k]->page_no <<"," <<e[i][j]->vec[k]->para_no <<"] ->";
            }
        }
    }

};


void print2(vector<Data*>*d, int max_size = 1000000){
    for(int i=0;i<max_size;i++){
        for(int j=0;j<d[i].size();j++){
            cout << "[" << d[i][j]->book_code <<","<<d[i][j]->page_no <<"," <<d[i][j]->para_no <<"]->";
        }
    }
};

void print3(vector<Data*> d){
    for (int i=0; i<d.size(); i++){
        cout<<"[" << d[i]->book_code <<","<<d[i]->page_no <<"," <<d[i]->para_no <<","<<d[i]->score <<","<<d[i]->ispresent <<"]->";
    }
}



void print4(vector<HubNode* >h){
    for (int i=0; i<h.size(); i++){
        cout<<endl;
        print3(h[i]->datalist);
        cout<<h[i]->hubscore;
    }
}

QNA_tool::QNA_tool(){
    // Implement your function here
    document = new Chaining();

}

QNA_tool::~QNA_tool(){
    // Implement your function here
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    vector<string>wordList = Algorithms::generateWordList(sentence);
    for(int i=0;i<wordList.size();i++){
       
      
        document->insertElement(wordList[i],book_code,page,paragraph);
    }

    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    vector<Data*>result;
    
    vector<string>wordlist = Algorithms::generateWordList(question);
    for(int i=0;i<wordlist.size();i++){
        vector<Data*> searchresult = document->searchElements(wordlist[i]);

        for(int j=0;j<searchresult.size();j++){
            searchresult[j]->wordvector.push_back(i);
            if(searchresult[j]->ispresent == false){
                result.push_back(searchresult[j]);
                searchresult[j]->ispresent = true;
            }
            searchresult[j]->score += float(searchresult.size()+1)/float(1+Algorithms::scoreofword(wordlist[i]));
        }
    }
    Algorithms::mergeSort(result,0,result.size()-1);
    

    Node* root = new Node();
    Node* n = root;
    for(int i=0;i<k;i++){
        result[i]->pirntData();
        cout << endl;
    }
    for (int i=0; i<result.size(); i++){
        result[i]->ispresent=false;
        result[i]->score=0;
    }
    for (int i = 0; i < k; i++) {
        n->right = new Node();
        n->book_code=result[i]->book_code;
        n->page=result[i]->page_no;
        n->paragraph=result[i]->para_no;
        n->right->left = n;
        n = n->right;
    }
    n->right=nullptr;
    Node* fn = root->right;
    fn->left=nullptr;
    return root;
}

void QNA_tool::query(string question, string filename){
    // Implement your function here
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;

    vector<Data*>result;
    //new query word list:
    vector<string>wordlist =  Algorithms :: modifyQuery(question);
    if (wordlist.size()==0){
        return;
    }
    cout<<wordlist.size();

     for(int i=0;i<wordlist.size();i++){
        vector<Data*> searchresult = document->searchElements(wordlist[i]);
        for(int j=0;j<searchresult.size();j++){
            searchresult[j]->wordvector.push_back(i);
            if(searchresult[j]->ispresent == false){
                result.push_back(searchresult[j]);
                searchresult[j]->ispresent = true;
            }
            searchresult[j]->score += float(searchresult.size()+1)/float(1+Algorithms::scoreofword(wordlist[i]));
        }
         print3(result);
    }
    Algorithms::mergeSortdifferent(result,0,result.size()-1);
//    cout<<endl;
//    print3(result);
//    cout<<"hello";

    vector<HubNode*>hubvector = Algorithms::hubMaker(result);
//    print4(hubvector);
//    cout<<"hello";
//
    Algorithms::mergeHUBSort(hubvector,0,hubvector.size()-1);
    print4(hubvector);
    cout<<"hello";
    for (int i=0; i<result.size(); i++){
        result[i]->ispresent=false;
        result[i]->score=0;
    }
    Node* root = new Node();
    Node* n = root;
    for (int i=0; i<2; i++){
        for (int j=0; j<hubvector[i]->datalist.size(); j++){
            n->right=new Node();
            n->book_code=hubvector[i]->datalist[j]->book_code;
            n->page=hubvector[i]->datalist[j]->page_no;
            n->paragraph=hubvector[i]->datalist[j]->para_no;
            hubvector[i]->datalist[j]->ispresent=false;
            hubvector[i]->datalist[j]->score=0;
            n->right->left=n;
            n=n->right;
        }
    }
    n->right=nullptr;
    Node* fn = root->right;
    fn->left=nullptr;
    //root is the thing
    return;
}

void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res){

    std::ifstream inputFile(FILENAME);
    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[4];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph) &&
            (metadata[3] == sentence_no)
        ){
            res = sentence;
            return;
        }

        res = "$I$N$V$A$L$I$D$";
        return;
    }

}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY){

    // first write the 3 paragraphs into different files
    Node* traverse = root;
    int num_paragraph = 0;
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        ofstream outfile(p_file);
        string paragraph;
        get_paragraph(traverse->book_code, traverse->page, traverse->paragraph, traverse->sentence_no, paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "Hey GPT, What is the purpose of life?";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // python3 <filename> API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += "paragraph_0.txt";
    command += " ";
    command += "paragraph_1.txt";
    command += " ";
    command += "paragraph_2.txt";
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}

#include <chrono>



 int main() {

     auto start = std::chrono::high_resolution_clock::now();
     QNA_tool gpt;
     cout <<"gpt intialzied" << endl;

     gpt.insert_sentence(4,1,4,1,"he is hard captian jack sparrow");
     gpt.insert_sentence(1,1,1,2,"pirates of the carribbean has main hero named jack sparrow who is a pirate");
     gpt.insert_sentence(1,1,1,3,"he was the hard this this this this this this this this captian of the black pearl before going to muntiny");

     gpt.insert_sentence(1,2,2,1,"commodore i really must protest pirate or not this man saved my hard life");
     gpt.insert_sentence(1,2,2,2,"i saved your life  you will save mine we are sqaure");
     gpt.insert_sentence(1,2,2,3, "This is the day you will always remember as the day you almost caught Captain Jack Sparrow");

     gpt.insert_sentence(1,3,3,1, "you knew my father");
     gpt.insert_sentence(1,3,3,2, "it was only after you learned my name you agreed to help hard");
     gpt.insert_sentence(1,3,3,3,"yes i knew your father probably among the ones who knew him as william everyone else just called him bootstrap or bootstrap bill");
     gpt.insert_sentence(3,1,3,4,"no my father was a respected sailor who obeyed the law hard");
     gpt.insert_sentence(3,1,3,5,"put it away son it is not worth beating you again");

     gpt.insert_sentence(3,1,3,6,"you ignored the rules of engagement in a fair fight i would have killed you");

     gpt.insert_sentence(3,1,3,7,"now as long as you just hang there pay attention to me hard");
     gpt.insert_sentence(3,1,3,8,"the only rules that really matters are these what a man can do and what a man cannot do");
     gpt.insert_sentence(3,1,3,8,"for instance you can accept that your father was a pirate and a good man or you cannot pirate is in your blood boy you will have to square with that some day so tell me will you sail under the command of a pirate or will you not");

     gpt.insert_sentence(4,1,4,6,"i must admit jack i thought i had you figured");
     gpt.insert_sentence(4,1,4,2,"but it turns out you are a hard man to predict");
     gpt.insert_sentence(4,1,4,3,"me i am dishonest and a this this  this this  dishonest men you can always trust to be dishonest");
     gpt.insert_sentence(4,1,4,4,"honestly it is the honest ones who wants to watch out for");
     gpt.insert_sentence(4,1,4,5,"cause you can never predict when they are going to do something terribly stupid");
//
//
//     cout<<"done rules";
//     gpt.query("done dishonest rules", "hello");
     
     int k  = 3;
     Node* head = gpt.get_top_k_para("this",k);
     Node*temp = head;

     for(int i=0;i<k;i++){
         cout << "("<< temp->book_code << "," << temp->page << "," << temp->paragraph << ")" << endl;
         temp = temp->right;
     }

     auto end = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
     std::cout << "Total Runtime: " << duration.count() << " microseconds" << std::endl;
     return 0;
 }
