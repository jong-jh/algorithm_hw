/*************************************************/
/*이름 : 정종현  학번: 2018038051*/
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

	initialize(&head);	//트리 헤드 초기화


	if (pFile == NULL)
	{
		printf("File does not exist");
		return 0;
	}
	// 파일 읽기 성공
	while (1) {
		fscanf(pFile, "%d", &intTemp);
		if (intTemp < 0) break;
		
		insert(head,intTemp);	//트리에 삽입
	}
	//int end1 = clock();	//tree 삽입 실행시간 확인
	cout<<"트리 생성 완료\n";
	cout<<"검색할 숫자 입력 : ";
	InputValue(key);
	
	search(head,key);
	/*search 가 끝난 후 실행시간 확인*/
	//int end2 = clock();

	//inorderTraversal(head->left);	//정렬이 되었는지 확인하기 위해 중위순회
	
	//파일 형태로 반환

	fclose(pFile);          //close file
	free(head);
	return 0;
}
/*중위순회 함수*/
void inorderTraversal(Node* ptr)
{
	if(ptr){
		inorderTraversal(ptr->left);		//왼쪽 서브트리로 이동
		printf("[%d] ", ptr->key);			//왼쪽 서브트리를 나오면 visit
		inorderTraversal(ptr->right);		//오른쪽 서브트리로 이동 후 앞선 내용 반복
	}
}
/*트리를 초기화할 때 사용한다.*/
int initialize(Node** h) {

	if(*h != NULL)
		free(*h);

	*h = new Node;
	(*h)->left = NULL;
	(*h)->right = *h;
	(*h)->key = -9999;
	return 1;
}
/*동적으로 할당된 트리를 해제해주는함수*/
int free(Node* head)
{
	if(head){
		free(head->left); 
		if(head->key==-9999) {/*printf("free head!");*/ delete head; return 0;}	//tree의 모든 방문을 끝낸 뒤 head로 돌아왔을 때, head->right가 recursive되면
		//무한하게 함수가 반복되기 때문에, head->left 바로 다음에 head를 메모리 해제 해야한다.
		
		free(head->right);
		delete head;	/*printf("free! memory\n")*/;	//메모리 해제
	}
}
/* tree 에 새로운 노드를 추가해주는 함수이다. 헤더노드의 주소와 입력받을 key 값을 받아온다. 
포인터 구조체변수 p를 선언하여 tree에 접근할 때 사용하고, Node* temp를 동적으로 생성하여 새로운 노드를 받을 메모리를 할당한다. 만약 tree가 비어있다면 head->left에 temp를 대입하여 작업을 끝낸다. 
그렇지않은 경우는 반복문을 통해 key가 현재 위치하고 있는 노드의 key 보다 작으면 왼쪽 서브트리로 이동, 크다면 오른쪽 서브트리로 이동해주는데, 이동할 자리가 비어있다면 temp를 tree에 삽입한다. 
빈 자리가 나타날 때 까지, 반복문을 통해 삽입할 위치를 찾는다.*/
int insert(Node* head, int key)
{
	Node *p = head->left;
	Node *temp = new Node; //새로 삽입할 노드 생성
	temp->key = key; temp->left=temp->right=NULL;
	
	if(head->left==NULL){//tree가 빈 트리라면 헤더노드의 left에 temp 삽입
		head->left=temp;
		return 0;
	}

	while (1)
	{
		if (key < p->key)	//key 가 원소의 값보다 작으면
		{
			if(p->left==NULL)	{p->left=temp; break;}	//왼쪽 서브트리로 이동 , 만약 비어있다면 tree에 삽입
			else 	p=p->left;
		}
		else if (key > p->key)	//key가 원소의 값보다 크면 오른쪽 서브트리로 이동 후 비어있다면 tree에 삽입
		{
			if(p->right==NULL)	{p->right=temp; break;}
			else 	p = p->right;
		}
	}
	return 0;
}
/*key를 입력받아 tree에 key값을 가진 노드가 존재한다면 해당 노드의 주소를 반환해주고, 존재하지 않는다면 NULL값을 반환해주는 함수이다. 
위의 searchRecursive와 다르게 Iterative한 방법으로 동작한다. head->left 주소를 받는 p를 선언하고, p가 NULL이 될 때 까지 반복문을 실행한다. 
p가 NULL이면 값을 찾지 못 하고 tree의 끝에 닿았다는 뜻이다. 만약 key값을 찾으면 해당 노드의 주소를 반환하고 찾지 못 하였다면 앞선 recursive 방법과 동일하게 tree를 이동한다. 
반복문을 빠져나왔다면 주소를 반환하지 못 했다는 의미이므로 NULL을 반환해준다.*/
void search(Node* head, int key)
{
	Node* p=head->left;
	while(p){//p가 NULL이 될 때 까지 반복 -> 원소를 끝까지 추적하였을 때
		if(key==p->key) {cout<<"값이 존재합니다\n"; return;}	//key와 일치하는 원소를 찾으면 해당되는 주소를 바환
		if(key<p->key)	//key 값이 원소의 값보다 작으면
			p=p->left;	//왼쪽 서브트리로 이동
		else
			p=p->right;	//key가 원소의 값보다 크면 오른쪽 서브트리로 이동
	}
	cout<<"값이 존재하지 않습니다\n";
	return; //반복문 안에서 해당되는 값을 찾지 못 했으므로 NULL 반환
}

/*숫자가 아닌 문자열이 들어오면 예외처리를 해주는 매서드*/
void InputValue(int& i) {
    cin >> i; cin.ignore();
    if (cin.fail()) {
        cout << "다시 입력해주세요 ! (키 오류) : "; //경고문 출력
        cin.clear();  //내부 플래그 초기화
        cin.ignore(10, '\n');
        return InputValue(i);
    }
}