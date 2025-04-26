// 자구단 #01 선형 탐색 linear search
int linear_search(int x)
{
    for (int i = 0; i < count; i++) // noqa
        if (arr[i] == x)
            return i;

    return -1; // NULL
}

// 자구단 #02 이진 탐색 binary search (재귀 호출) O(log n)
int binary_search(int s, int e, int key) // T(n)
{
    // O(1) 개수와 상관없이 일정
    if (s == e) // 원소가 1개인 case
        return (arr[s] == x) ? s : -1;

    int mid = (s + e) / 2;
    if (key == arr[mid])
        return mid;
    else if (arr[mid] > key)
        return binary_search(s, mid - 1, key); // 절반만 수행 T(n/2)
    else
        return binary_search(mid + 1, e, key);
}

// 자구단 #02 이진 탐색 binary search (반복 구조)
int binary_search_r(int key)
{
    int s = 0;
    int e = count - 1;
    int mid;

    while (s < e)
    {
        mid = (s + e) / 2;
        if (key = arr[mid])
            return mid;
        else if (arr[mid] > key)
            e = mid - 1;
        else
            s = mid + 1;
    }
    return (arr[s] == key) ? s : -1;
}

// 자구단 #03 배열 삽입 array insert
int insert_by_value(int arr[], int x)
{
    // 0. 예외 처리

    // 1. x를 삽입할 위치를 결정 O(k)
    for (i = 0; i < count; i++)
    {
        if (arr[i] > x) // 나보다 큰 녀석
            break;
    }

    // 2. 삽입할 위치의 원소를 옮겨서 공간을 확보(->) O(k-1)
    for (j = count - 1; j >= i; j--)
        arr[j + 1] = arr[j];

    // 3. 원소를 삽입 O(1)
    arr[i] = x;

    // 4.배열의 크기 (count)를 증가 O(1)
    count++;
    return arr;
}

// 자구단 #04 배열 제거 array delete O(n)
int delete(int arr[], int x)
{
    // 0. 예외적인 경우

    // 1. 삭제할 원소(x)를 배열 (arr)에서 찾음 O(k)
    for (i = 0; i < count; i++)
        if (arr[i] == x)
            break;
    if (i == count)
        return arr;

    // 2. 삭제할 원소 뒤에 있는 원소들을 앞으로 이동 (<-) O(k-1)
    for (j = i; j < count - 1; j++)
        arr[j] = arr[j + 1];

    // 3. 배열의 크기 (count)를 감소
    count--; // O(1)

    return arr;
}

// 자구단 #05 연결 리스트 선형 탐색
node *node::search(data_type item)
{
    // 1. Find a proper position
    node *curr = this; // node를 방문할 포인터 curr
    while (curr != NULL)
    {
        if (curr->item == item)
            return curr; // Found
        curr = curr->link;
    }

    return NULL; // Not found
}

// 자구단 #06 단일 연결 리스트 삽입 O(n)
void main()
{
    first.insert("GAT");
}

void hnode::insert(data_type item)
{
    this->link->insert(item);
}

void node::insert(data_type item) // GATD
{
    // 1. 적절한 위치를 결정
    node *curr = this;
    while (curr->link != NULL)
    {
        if (curr->link->item > item) // 다음 원소 item이 현재 내가 삽입하려는 것보다 클 때
            break;                   // 이 링크가 FAT의 링크. curr의 link를 따짐 따라서 curr는 FAT curr->link는 HAT
        curr = curr->link;
    }

    // 2. 새로운 노드를 생성
    node *hnode = new node; // node크기만큼의 기억공간 할당
    nnode->item = item;     // nnode에 GAT 할당

    // 3. 링크를 갱신
    nnode->link = curr->link; // nnode->link가 HAT여야 됨. HAT는 지금 curr->link임임 (오른쪽 화살표)
    curr->link = nnode;       // 왼쪽 화살표
}

// 자구단 #07 단일 연결 리스트 삭제 O(n)
void main()
{
    first.delete("FAT");
}

void hnode::delete(data_type item)
{
    this->link->delete(item);
}

void node::delete(data_type item)
{
    // 1. 삭제할 원소를 찾음 하나 앞
    node *curr = this;
    while (curr->link != NULL)
    {
        if (curr->link->item == item)
            break;
        curr = curr->link;
    }
    if (curr == NULL)
        return;
    // 2. 링크를 갱신
    node *dnode = curr->link; // dnode: FAT dnode->link: HAT
    curr->link = dnode->link; // curr->link: FAT 에서 dnode->link: HAT 갱신하기
    // 3. 삭제된 node를 free
    free(dnode);
}

// 자구단 #08 이준 연결 리스트 삽입 O(n)
void node::insert(data_type item)
{
    // 1. 적절한 위치를 결정
    node *curr = this;
    while (curr->rlink != NULL)
    {
        if (curr->rlink->item > item)
            break;
        curr = curr->rlink;
    }
    // 2. 새로운 노드를 생성
    node *nnode = new node;
    nnode->item = item;
    // 3. 링크를 갱신
    // 3.1 rlink 방향 (rlink: ->)
    nnode->rlink = curr->rlink;
    curr->rlink = nnode;
    // 3.2 llink 방향 (link: <-)
    nnode->llink = curr;
    nnode->rlink->llink = nnode;
}

// 자구단 #09 이중 연결 리스트 삭제 O(n)
void node::delete(data_type item)
{
    // 1. 삭제할 원소를 찾음
    node *curr = first;
    while (curr->rlink != NULL)
    {
        if (curr->rlink->item == item)
            break;
        curr = curr->rlink;
    }
    // 2. 링크를 갱신
    node *dnode = curr->rlink;
    curr->rlink = dnode->rlink;
    curr->rlink->llink = dnode->llink; // 아니면 curr
    // 3. 삭제된 노드를 free
    free(dnode);
}

// 자구단 #10 스택 push
void Stack::push(Datatype DataItem)
{
    if (is_Full())
        printf("Pushing in Full stack\n");

    Items[TOP] = DataItem;
    TOP++;
}

// 자구단 #11 스택 pop
Datatype Stack::pop()
{
    if (is_Empty())
        printf("Poping from Empty Stack\n");

    TOP--;
    retrun Items[TOP];
}

// 자구단 #12 큐 push
void queue::push(element item)
{
    if (isFull())
        printf("cannot add an element to a full queue");
    
    Items[++rear] = item;
}

// 자구단 #13 큐 pop
void queue::pop(element item)
{
    if(isEmpty())
        printf("You cannot delete from an empty queue");
    
    return Items[++front];
}