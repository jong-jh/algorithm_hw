/* �̸�: ������, �й�: 2018038051 */
/* ��������: huffman encoding �� ����*/
/*          Frequency table �� minimum heap ���� ������ priority queue �� ���� */
/*          huffman tree �� ���� �� ��������� binarycode table ���� */
/*          binarycode table�� ���ϸ� encoding, decoding */

#include<iostream>
#include<string>
using namespace std;
#define QueueMax 52

typedef struct alphaNode{
    string data; //���ĺ��� ����
    string bc;  //binary code
    int fre=0;  //�󵵼�
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

int initialize(Node** h);                   //��� ��� �ʱ�ȭ
void setfrequency(string str,Node* head);   //���ڿ��� �󵵼��� üũ
void swap(Node **,Node **);                 //swap �Լ�
void push(Node* );                          //Min heap push
Node* pop();                                //Min heap pop
void setfreqtable(Node* head);              //frequency table ����
Node* sethuffmantree();                     //huffmantree ����
void setbinarycode(Node *,string);          //binarycode ���� �� binarytable ����
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
    /*���ڰ� �ϳ��� ���, code table �� ���� ������, ���ڵ�, ���ڵ� �� ����*/
        if(!head->right->right){
            string result=encoding(original);
            decoding(result);
            return 0;
        }

    setfreqtable(head);
/************************�켱���� ť �ȿ� ������ �����Ǿ��ִ�.************************/
    huffman=sethuffmantree();
    
    setbinarycode(huffman,str);
    
    string encodedstr=encoding(original);
    decoding(encodedstr);

    return 0;
}

/*��� ��� �ʱ�ȭ*/
int initialize(Node** h) {

	if(*h != NULL)
		free(*h);

	*h = new Node;
	(*h)->left = NULL;
	(*h)->right = NULL;
	(*h)->data = '1';
	return 1;
}

/*origianl string �� alphabet's frequency ���ϱ�*/
void setfrequency(string str,Node* head){
    Node* p=head; int cnt=0; int check=0;

    int alpha[52];  //�� ���ĺ��� ���� ���� �迭
    for(int i=0;i<52;i++){
        alpha[i]=0;  //frequency of each alphabet
    }

    int size=str.size();
    for(int i=0;i<size;i++){        //original string �� ���ĺ��� ���ʷ� �޾ƿ�
        alpha[int(str[i])-65]++;    //���ĺ��� �ش��ϴ� index�� ���� �ø���
    }
    
    /*�����忡 ���ĺ��� data�� frequency���� ������ ��带 ��������ش�*/
    for(int i=0;i<52;i++){
        if(alpha[i]>0){
            Node* temp= new Node;   //��� ���� �� value ����
            temp->data=i+65;
            temp->fre=alpha[i];
            
            temp->right=head->right;    //������� �� ����� ����
            head->right=temp;
        }
    }
    /*���ڰ� �ϳ� �� ��� ������ Ʈ���� ��������� �ʱ� ������, ���� code table ����*/
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
/*Min Heap ��� push*/
void push(Node* a){
    Node* temp=new Node; 
    temp=a;
    q.Heap[q.top]=temp;    //queue �� ��� ����

    if(q.top==0){q.top++; return;}  //queue�� ��������� �����ϰ� ����

    int current =q.top;         //������ ��ġ
    int parent=(q.top-1)/2;     //�θ� ����� ��ġ
    
    while(current>0&&(q.Heap[current]->fre) < (q.Heap[parent]->fre)){   //�θ��尡 �����ϰ�, ������ ���� �θ��� ������ ���� ��
        // cout<<"before swap "<<endl;
        // cout<<"q.Heap[current] = "<<q.Heap[current]->data<<endl;
        // cout<<"q.Heap[parent] = "<<q.Heap[parent]->data<<endl;
        swap(&q.Heap[current],&q.Heap[parent]);   //���� ��ġ�� �ٲ۴�

        // cout<<"swap compplete"<<endl;
        // cout<<"q.Heap[current] = "<<q.Heap[current]->data<<endl;
        // cout<<"q.Heap[parent] = "<<q.Heap[parent]->data<<endl;
        current=parent;         //������ ���� �θ��� ��ġ ����
        parent=(parent-1)/2;
    }

    q.top++;    //queue �� size ����

    return;
}
/*Min Heap ��� pop*/
Node* pop(){
    if(q.top==0) return NULL;   //queue �� ��������� ����
    int current =0;
    int left=current*2+1;
    int right=current*2+2;
    int min=current;

    Node* ret = q.Heap[0];  //Heap �� ��Ʈ��� �̸� ����
    q.top=q.top-1;

    q.Heap[0]=q.Heap[q.top];    //Ʈ���� ������ ��带 ��Ʈ��忡 ����

    while(left<q.top){  //�ڽĳ�尡 �����Ѵٸ� �ݺ�
        
        if(q.Heap[min]->fre > q.Heap[left]->fre){   //���� �ڽĳ���� ���� �� ������
            min=left;
        }
        if(q.Heap[min]->fre > q.Heap[right]->fre){   //������ �ڽĳ���� ���� �� ������
            min=right;
        }

        if(min==current){       //�� ���� �ڽĳ��� ������ �� �ƹ���ȭ ���ٸ� ����
            break;
        }

        swap(q.Heap[current],q.Heap[min]);  //�ڽĳ��� �� �� ���� ���� ���� ���� swap
        current=min;    //��ȭ�� ��ġ�� �̵�
        left=current*2+1;   //�̵��� ��ġ�� �ڽ� ���
        right=current*2+2;

    }

    return ret;

}

/*frequency table ����*/
void setfreqtable(Node* head){
    Node* p =head->right;
    Node* t =p; //������ ������ ������ ��� ������
    while(1){
        
        push(p);
        //cout<<"\""<<p->data<<":  fre = "<<p->fre<<"\" push complete"<<endl;
        p=p->right;
        t->right=NULL;  //����Ǿ��ִ� ��带 ���´�
        t=p;

        if(!p) break;
    }

}
/*huffmantree ����*/
Node* sethuffmantree(){
    
    while(1){
    
    Node* i = pop();
    Node* j = pop();

    if(!j){   //queue �� ���̻� ��尡 ������ huffman tree �� ������ �Ϸ�ȴ�.
        //cout<<"huffman tree �����Ϸ�"<<endl;
        return i;
    }

    Node* temp= new Node;
    temp->data=i->data+j->data;
    temp->fre=(i->fre)+(j->fre);  //���ο� ����� frequency
    
    
    if(i->fre < j->fre){ //frequency ���� ���� ��尡 �������� ����ȴ�.
        temp->left = i;
        temp->right = j;
    }
    else{
        //cout<<i->data<<"  "<<j->data<<"  "<<i->fre<<"  "<<j->fre<<endl;
        temp->left=j;
        temp->right=i;
    }
    
    push(temp); //tree �� push
    }
    
}
/*binarycode ����
  huffman tree�� recursive �ϰ� leftchild & rightchilde �� �湮�Ͽ�
  binarycode ���� �����Ѵ�.
*/
void setbinarycode(Node * tree,string str){
    if(tree){
        string code=str;
        tree->bc=code;
        /*���ܳ���� ���*/
        if(tree->data.size()==1){
            
            ct[ctop].key=tree->data;    //���� ����� binarycode �� codetable �� �����Ѵ�.
            ct[ctop].code=tree->bc;
            //cout<<ct[ctop].key<<"  "<<ct[ctop].code<<endl;
            ctop++;
        }
        /*left child �� ������ ���*/
        if(tree->left){
            code=str+'0';   //code �� 0�� �߰��ϰ�
            setbinarycode(tree->left,code); //left child �湮
        }
        /*right child �� ������ ���*/
        if(tree->right){
            code=str+'1';   //code �� 1�� �߰��ϰ�
            setbinarycode(tree->right,code);    //right child �湮
        }
        
    }
}

/*encoding �Լ�
 ������ binary code table�� �̿��� �Էµ� ���ڿ��� ���Ͽ� ��ġ�ϴ� ���ڿ� ���� binarycode�� ��ȯ�Ѵ�. 
*/
string encoding(string str){
    string ret;
    cout<<"encoding...\n\nEncoded result: ";
    
    for(int i=0;i<str.size();i++){
        for(int j=0;j<ctop;j++){
            if(str[i]==ct[j].key[0]){   //i��° ���ڿ�, binarytable �� ������ ��
                ret+=ct[j].code;    //��ġ�ϸ� binarycode �߰�
            }
        }
    }
    cout<<ret;  //encoding �� ���
    cout<<endl;
    return ret;
}
/*decoding �Լ�*/
void decoding(string str){
    cout<<"decoding...\n"<<endl;
    int index=0;
    string alpha;
    int i=1;

    while(index<str.size()){    //index�� ���� binarycode �� ���̸� ������� ����
        
        for(int j=0;j<ctop;j++){
            if(str.substr(index,i)==ct[j].code){    //���ڸ� index ���� i���� indexing �� binary talbe �� code �� ��
                alpha+=ct[j].key;   //��ġ�ϸ� binary code �� �ش��ϴ� ���� �߰�
                index+=i;   //index �� ����, �񱳰� �Ϸ�� ���ڿ��� ���� index ���� ��
                i=0;    //i�� �ʱ�ȭ
            }
        }
        i++;    
    }
    cout<<"Decoded result: "<<alpha<<endl;  //decoded �� ��ȯ

}