/*************************************************/
/*�̸� : ������  �й�: 2018038051*/
/*binary-search tree*/
/************************************************/
#include<iostream>
#include<ctime>
using namespace std;

typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

int initialize(Node** );
int free(Node* ); 
int insert(Node* , int );
void inorderTraversal(Node* );	
void search(Node* , int ); 
inline void InputValue(int&);

int main(void)
{
	int start = clock();
	ios_base::sync_with_stdio;
	cin.tie(0);
	FILE* pFile = fopen("C:\\test.txt", "r"); //read mode 
	int intTemp = -1;

	Node* head = NULL;
	Node* ptr = NULL;	/* temp */
	int key=0;

	initialize(&head);	//Ʈ�� ��� �ʱ�ȭ


	if (pFile == NULL)
	{
		printf("File does not exist");
		return 0;
	}
	// ���� �б� ����
	while (1) {
		fscanf(pFile, "%d", &intTemp);
		if (intTemp < 0) break;
		
		insert(head,intTemp);	//Ʈ���� ����
	}
	//int end1 = clock();	//tree ���� ����ð� Ȯ��
	cout<<"Ʈ�� ���� �Ϸ�\n";
	cout<<"�˻��� ���� �Է� : ";
	InputValue(key);
	
	search(head,key);
	/*search �� ���� �� ����ð� Ȯ��*/
	//int end2 = clock();

	//inorderTraversal(head->left);	//������ �Ǿ����� Ȯ���ϱ� ���� ������ȸ
	
	//���� ���·� ��ȯ

	fclose(pFile);          //close file
	free(head);
	return 0;
}
/*������ȸ �Լ�*/
void inorderTraversal(Node* ptr)
{
	if(ptr){
		inorderTraversal(ptr->left);		//���� ����Ʈ���� �̵�
		printf("[%d] ", ptr->key);			//���� ����Ʈ���� ������ visit
		inorderTraversal(ptr->right);		//������ ����Ʈ���� �̵� �� �ռ� ���� �ݺ�
	}
}
/*Ʈ���� �ʱ�ȭ�� �� ����Ѵ�.*/
int initialize(Node** h) {

	if(*h != NULL)
		free(*h);

	*h = new Node;
	(*h)->left = NULL;
	(*h)->right = *h;
	(*h)->key = -9999;
	return 1;
}
/*�������� �Ҵ�� Ʈ���� �������ִ��Լ�*/
int free(Node* head)
{
	if(head){
		free(head->left); 
		if(head->key==-9999) {/*printf("free head!");*/ delete head; return 0;}	//tree�� ��� �湮�� ���� �� head�� ���ƿ��� ��, head->right�� recursive�Ǹ�
		//�����ϰ� �Լ��� �ݺ��Ǳ� ������, head->left �ٷ� ������ head�� �޸� ���� �ؾ��Ѵ�.
		
		free(head->right);
		delete head;	/*printf("free! memory\n")*/;	//�޸� ����
	}
}
/* tree �� ���ο� ��带 �߰����ִ� �Լ��̴�. �������� �ּҿ� �Է¹��� key ���� �޾ƿ´�. 
������ ����ü���� p�� �����Ͽ� tree�� ������ �� ����ϰ�, Node* temp�� �������� �����Ͽ� ���ο� ��带 ���� �޸𸮸� �Ҵ��Ѵ�. ���� tree�� ����ִٸ� head->left�� temp�� �����Ͽ� �۾��� ������. 
�׷������� ���� �ݺ����� ���� key�� ���� ��ġ�ϰ� �ִ� ����� key ���� ������ ���� ����Ʈ���� �̵�, ũ�ٸ� ������ ����Ʈ���� �̵����ִµ�, �̵��� �ڸ��� ����ִٸ� temp�� tree�� �����Ѵ�. 
�� �ڸ��� ��Ÿ�� �� ����, �ݺ����� ���� ������ ��ġ�� ã�´�.*/
int insert(Node* head, int key)
{
	Node *p = head->left;
	Node *temp = new Node; //���� ������ ��� ����
	temp->key = key; temp->left=temp->right=NULL;
	
	if(head->left==NULL){//tree�� �� Ʈ����� �������� left�� temp ����
		head->left=temp;
		return 0;
	}

	while (1)
	{
		if (key < p->key)	//key �� ������ ������ ������
		{
			if(p->left==NULL)	{p->left=temp; break;}	//���� ����Ʈ���� �̵� , ���� ����ִٸ� tree�� ����
			else 	p=p->left;
		}
		else if (key > p->key)	//key�� ������ ������ ũ�� ������ ����Ʈ���� �̵� �� ����ִٸ� tree�� ����
		{
			if(p->right==NULL)	{p->right=temp; break;}
			else 	p = p->right;
		}
	}
	return 0;
}
/*key�� �Է¹޾� tree�� key���� ���� ��尡 �����Ѵٸ� �ش� ����� �ּҸ� ��ȯ���ְ�, �������� �ʴ´ٸ� NULL���� ��ȯ���ִ� �Լ��̴�. 
���� searchRecursive�� �ٸ��� Iterative�� ������� �����Ѵ�. head->left �ּҸ� �޴� p�� �����ϰ�, p�� NULL�� �� �� ���� �ݺ����� �����Ѵ�. 
p�� NULL�̸� ���� ã�� �� �ϰ� tree�� ���� ��Ҵٴ� ���̴�. ���� key���� ã���� �ش� ����� �ּҸ� ��ȯ�ϰ� ã�� �� �Ͽ��ٸ� �ռ� recursive ����� �����ϰ� tree�� �̵��Ѵ�. 
�ݺ����� �������Դٸ� �ּҸ� ��ȯ���� �� �ߴٴ� �ǹ��̹Ƿ� NULL�� ��ȯ���ش�.*/
void search(Node* head, int key)
{
	Node* p=head->left;
	while(p){//p�� NULL�� �� �� ���� �ݺ� -> ���Ҹ� ������ �����Ͽ��� ��
		if(key==p->key) {cout<<"���� �����մϴ�\n"; return;}	//key�� ��ġ�ϴ� ���Ҹ� ã���� �ش�Ǵ� �ּҸ� ��ȯ
		if(key<p->key)	//key ���� ������ ������ ������
			p=p->left;	//���� ����Ʈ���� �̵�
		else
			p=p->right;	//key�� ������ ������ ũ�� ������ ����Ʈ���� �̵�
	}
	cout<<"���� �������� �ʽ��ϴ�\n";
	return; //�ݺ��� �ȿ��� �ش�Ǵ� ���� ã�� �� �����Ƿ� NULL ��ȯ
}

/*���ڰ� �ƴ� ���ڿ��� ������ ����ó���� ���ִ� �ż���*/
void InputValue(int& i) {
    cin >> i; cin.ignore();
    if (cin.fail()) {
        cout << "�ٽ� �Է����ּ��� ! (Ű ����) : "; //��� ���
        cin.clear();  //���� �÷��� �ʱ�ȭ
        cin.ignore(10, '\n');
        return InputValue(i);
    }
}