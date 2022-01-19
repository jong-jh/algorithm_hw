/* 이름: 정종현, 학번: 2018038051 */
/* 문제정의: huffman encoding 을 구현*/
/*          Frequency table 을 minimum heap 으로 구현된 priority queue 로 관리 */
/*          huffman tree 를 생성 후 재귀적으로 binarycode table 생성 */
/*          binarycode table을 비교하며 encoding, decoding */

#include<iostream>
#include<string>
using namespace std;
#define QueueMax 52

typedef struct alphaNode{
    string data; //알파벳이 들어가고
    string bc;  //binary code
    int fre=0;  //빈도수
    struct alphaNode* left=NULL;
    struct alphaNode* right=NULL;
}Node;

 typedef struct priorityQueue{
     Node* Heap[QueueMax];
     int top=0;
 }queue;

/*code table*/
 typedef struct Codewordtable{
     string key;
     string code;
 }Code;

queue q;
Code ct[QueueMax];
int ctop=0;

int initialize(Node** h);                   //헤더 노드 초기화
void setfrequency(string str,Node* head);   //문자열의 빈도수를 체크
void swap(Node **,Node **);                 //swap 함수
void push(Node* );                          //Min heap push
Node* pop();                                //Min heap pop
void setfreqtable(Node* head);              //frequency table 생성
Node* sethuffmantree();                     //huffmantree 생성
void setbinarycode(Node *,string);          //binarycode 생성 및 binarytable 생성
string encoding(string);             //encoding
void decoding(string);                      //decoding

int main(){
    string original;
    string str;
    Node* head=NULL;
    Node* huffman=NULL;
    initialize(&head);
    
    cout<<"Put the String to encode: ";
    cin>>original;

    setfrequency(original,head);
    /*문자가 하나일 경우, code table 을 따로 생성해, 인코딩, 디코딩 후 종료*/
        if(!head->right->right){
            string result=encoding(original);
            decoding(result);
            return 0;
        }

    setfreqtable(head);
/************************우선순위 큐 안에 노드들이 정리되어있다.************************/
    huffman=sethuffmantree();
    
    setbinarycode(huffman,str);
    
    string encodedstr=encoding(original);
    decoding(encodedstr);

    return 0;
}

/*헤더 노드 초기화*/
int initialize(Node** h) {

	if(*h != NULL)
		free(*h);

	*h = new Node;
	(*h)->left = NULL;
	(*h)->right = NULL;
	(*h)->data = '1';
	return 1;
}

/*origianl string 의 alphabet's frequency 구하기*/
void setfrequency(string str,Node* head){
    Node* p=head; int cnt=0; int check=0;

    int alpha[52];  //각 알파벳의 수를 담을 배열
    for(int i=0;i<52;i++){
        alpha[i]=0;  //frequency of each alphabet
    }

    int size=str.size();
    for(int i=0;i<size;i++){        //original string 의 알파벳을 차례로 받아와
        alpha[int(str[i])-65]++;    //알파벳에 해당하는 index에 값을 늘린다
    }
    
    /*헤더노드에 알파벳의 data와 frequency값을 저장한 노드를 연결시켜준다*/
    for(int i=0;i<52;i++){
        if(alpha[i]>0){
            Node* temp= new Node;   //노드 생성 후 value 저장
            temp->data=i+65;
            temp->fre=alpha[i];
            
            temp->right=head->right;    //헤더노드와 각 노드의 연결
            head->right=temp;
        }
    }
    /*문자가 하나 일 경우 하프만 트리가 만들어지지 않기 때문에, 따로 code table 생성*/
    if(!head->right->right){
        ct[ctop].key=head->right->data;
        ct[ctop].code='1';
        ctop=1;
    }
}

void swap(Node** a,Node** b){
    Node *temp=*a;
    *a=*b;
    *b=temp; 
}
/*Min Heap 노드 push*/
void push(Node* a){
    Node* temp=new Node; 
    temp=a;
    q.Heap[q.top]=temp;    //queue 에 노드 저장

    if(q.top==0){q.top++; return;}  //queue가 비어있으면 삽입하고 종료

    int current =q.top;         //현재의 위치
    int parent=(q.top-1)/2;     //부모 노드의 위치
    
    while(current>0&&(q.Heap[current]->fre) < (q.Heap[parent]->fre)){   //부모노드가 존재하고, 현재의 값이 부모의 값보다 작을 때
        // cout<<"before swap "<<endl;
        // cout<<"q.Heap[current] = "<<q.Heap[current]->data<<endl;
        // cout<<"q.Heap[parent] = "<<q.Heap[parent]->data<<endl;
        swap(&q.Heap[current],&q.Heap[parent]);   //둘의 위치를 바꾼다

        // cout<<"swap compplete"<<endl;
        // cout<<"q.Heap[current] = "<<q.Heap[current]->data<<endl;
        // cout<<"q.Heap[parent] = "<<q.Heap[parent]->data<<endl;
        current=parent;         //삽입한 노드와 부모노드 위치 조정
        parent=(parent-1)/2;
    }

    q.top++;    //queue 의 size 증가

    return;
}
/*Min Heap 노드 pop*/
Node* pop(){
    if(q.top==0) return NULL;   //queue 가 비어있으면 종료
    int current =0;
    int left=current*2+1;
    int right=current*2+2;
    int min=current;

    Node* ret = q.Heap[0];  //Heap 의 루트노드 미리 저장
    q.top=q.top-1;

    q.Heap[0]=q.Heap[q.top];    //트리의 마지막 노드를 루트노드에 삽입

    while(left<q.top){  //자식노드가 존재한다면 반복
        
        if(q.Heap[min]->fre > q.Heap[left]->fre){   //왼쪽 자식노드의 값이 더 작으면
            min=left;
        }
        if(q.Heap[min]->fre > q.Heap[right]->fre){   //오른쪽 자식노드의 값이 더 작으면
            min=right;
        }

        if(min==current){       //양 쪽의 자식노드와 비교했을 때 아무변화 없다면 고정
            break;
        }

        swap(q.Heap[current],q.Heap[min]);  //자식노드들 중 더 작은 값을 현재 노드와 swap
        current=min;    //변화된 위치로 이동
        left=current*2+1;   //이동한 위치의 자식 노드
        right=current*2+2;

    }

    return ret;

}

/*frequency table 생성*/
void setfreqtable(Node* head){
    Node* p =head->right;
    Node* t =p; //노드들의 연결을 끊어줄 노드 포인터
    while(1){
        
        push(p);
        //cout<<"\""<<p->data<<":  fre = "<<p->fre<<"\" push complete"<<endl;
        p=p->right;
        t->right=NULL;  //연결되어있는 노드를 끊는다
        t=p;

        if(!p) break;
    }

}
/*huffmantree 생성*/
Node* sethuffmantree(){
    
    while(1){
    
    Node* i = pop();
    Node* j = pop();

    if(!j){   //queue 에 더이상 노드가 없으면 huffman tree 가 생성이 완료된다.
        //cout<<"huffman tree 생성완료"<<endl;
        return i;
    }

    Node* temp= new Node;
    temp->data=i->data+j->data;
    temp->fre=(i->fre)+(j->fre);  //새로운 노드의 frequency
    
    
    if(i->fre < j->fre){ //frequency 값이 적은 노드가 왼쪽으로 연결된다.
        temp->left = i;
        temp->right = j;
    }
    else{
        //cout<<i->data<<"  "<<j->data<<"  "<<i->fre<<"  "<<j->fre<<endl;
        temp->left=j;
        temp->right=i;
    }
    
    push(temp); //tree 를 push
    }
    
}
/*binarycode 생성
  huffman tree를 recursive 하게 leftchild & rightchilde 를 방문하여
  binarycode 값을 저장한다.
*/
void setbinarycode(Node * tree,string str){
    if(tree){
        string code=str;
        tree->bc=code;
        /*말단노드인 경우*/
        if(tree->data.size()==1){
            
            ct[ctop].key=tree->data;    //말단 노드의 binarycode 를 codetable 에 저장한다.
            ct[ctop].code=tree->bc;
            //cout<<ct[ctop].key<<"  "<<ct[ctop].code<<endl;
            ctop++;
        }
        /*left child 가 존재할 경우*/
        if(tree->left){
            code=str+'0';   //code 에 0을 추가하고
            setbinarycode(tree->left,code); //left child 방문
        }
        /*right child 가 존재할 경우*/
        if(tree->right){
            code=str+'1';   //code 에 1을 추가하고
            setbinarycode(tree->right,code);    //right child 방문
        }
        
    }
}

/*encoding 함수
 생서된 binary code table를 이용해 입력된 문자열과 비교하여 일치하는 문자에 대한 binarycode를 반환한다. 
*/
string encoding(string str){
    string ret;
    cout<<"encoding...\n\nEncoded result: ";
    
    for(int i=0;i<str.size();i++){
        for(int j=0;j<ctop;j++){
            if(str[i]==ct[j].key[0]){   //i번째 문자와, binarytable 의 값들을 비교
                ret+=ct[j].code;    //일치하면 binarycode 추가
            }
        }
    }
    cout<<ret;  //encoding 값 출력
    cout<<endl;
    return ret;
}
/*decoding 함수*/
void decoding(string str){
    cout<<"decoding...\n"<<endl;
    int index=0;
    string alpha;
    int i=1;

    while(index<str.size()){    //index의 값이 binarycode 의 길이를 넘을경우 종료
        
        for(int j=0;j<ctop;j++){
            if(str.substr(index,i)==ct[j].code){    //문자를 index 부터 i까지 indexing 후 binary talbe 의 code 와 비교
                alpha+=ct[j].key;   //일치하면 binary code 에 해당하는 문자 추가
                index+=i;   //index 값 수정, 비교가 완료된 문자열의 다음 index 부터 비교
                i=0;    //i값 초기화
            }
        }
        i++;    
    }
    cout<<"Decoded result: "<<alpha<<endl;  //decoded 값 반환

}