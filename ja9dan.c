// �ڱ��� #01 ���� Ž�� linear search
int linear_search(int x)
{
    for (int i = 0; i < count; i++) // noqa
        if (arr[i] == x)
            return i;

    return -1; // NULL
}

// �ڱ��� #02 ���� Ž�� binary search (��� ȣ��) O(log n)
int binary_search(int s, int e, int key) // T(n)
{
    // O(1) ������ ������� ����
    if (s == e) // ���Ұ� 1���� case
        return (arr[s] == x) ? s : -1;

    int mid = (s + e) / 2;
    if (key == arr[mid])
        return mid;
    else if (arr[mid] > key)
        return binary_search(s, mid - 1, key); // ���ݸ� ���� T(n/2)
    else
        return binary_search(mid + 1, e, key);
}

// �ڱ��� #02 ���� Ž�� binary search (�ݺ� ����)
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

// �ڱ��� #03 �迭 ���� array insert
int insert_by_value(int arr[], int x)
{
    // 0. ���� ó��

    // 1. x�� ������ ��ġ�� ���� O(k)
    for (i = 0; i < count; i++)
    {
        if (arr[i] > x) // ������ ū �༮
            break;
    }

    // 2. ������ ��ġ�� ���Ҹ� �Űܼ� ������ Ȯ��(->) O(k-1)
    for (j = count - 1; j >= i; j--)
        arr[j + 1] = arr[j];

    // 3. ���Ҹ� ���� O(1)
    arr[i] = x;

    // 4.�迭�� ũ�� (count)�� ���� O(1)
    count++;
    return arr;
}

// �ڱ��� #04 �迭 ���� array delete O(n)
int delete(int arr[], int x)
{
    // 0. �������� ���

    // 1. ������ ����(x)�� �迭 (arr)���� ã�� O(k)
    for (i = 0; i < count; i++)
        if (arr[i] == x)
            break;
    if (i == count)
        return arr;

    // 2. ������ ���� �ڿ� �ִ� ���ҵ��� ������ �̵� (<-) O(k-1)
    for (j = i; j < count - 1; j++)
        arr[j] = arr[j + 1];

    // 3. �迭�� ũ�� (count)�� ����
    count--; // O(1)

    return arr;
}

// �ڱ��� #05 ���� ����Ʈ ���� Ž��
node *node::search(data_type item)
{
    // 1. Find a proper position
    node *curr = this; // node�� �湮�� ������ curr
    while (curr != NULL)
    {
        if (curr->item == item)
            return curr; // Found
        curr = curr->link;
    }

    return NULL; // Not found
}

// �ڱ��� #06 ���� ���� ����Ʈ ���� O(n)
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
    // 1. ������ ��ġ�� ����
    node *curr = this;
    while (curr->link != NULL)
    {
        if (curr->link->item > item) // ���� ���� item�� ���� ���� �����Ϸ��� �ͺ��� Ŭ ��
            break;                   // �� ��ũ�� FAT�� ��ũ. curr�� link�� ���� ���� curr�� FAT curr->link�� HAT
        curr = curr->link;
    }

    // 2. ���ο� ��带 ����
    node *hnode = new node; // nodeũ�⸸ŭ�� ������ �Ҵ�
    nnode->item = item;     // nnode�� GAT �Ҵ�

    // 3. ��ũ�� ����
    nnode->link = curr->link; // nnode->link�� HAT���� ��. HAT�� ���� curr->link���� (������ ȭ��ǥ)
    curr->link = nnode;       // ���� ȭ��ǥ
}

// �ڱ��� #07 ���� ���� ����Ʈ ���� O(n)
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
    // 1. ������ ���Ҹ� ã�� �ϳ� ��
    node *curr = this;
    while (curr->link != NULL)
    {
        if (curr->link->item == item)
            break;
        curr = curr->link;
    }
    if (curr == NULL)
        return;
    // 2. ��ũ�� ����
    node *dnode = curr->link; // dnode: FAT dnode->link: HAT
    curr->link = dnode->link; // curr->link: FAT ���� dnode->link: HAT �����ϱ�
    // 3. ������ node�� free
    free(dnode);
}

// �ڱ��� #08 ���� ���� ����Ʈ ���� O(n)
void node::insert(data_type item)
{
    // 1. ������ ��ġ�� ����
    node *curr = this;
    while (curr->rlink != NULL)
    {
        if (curr->rlink->item > item)
            break;
        curr = curr->rlink;
    }
    // 2. ���ο� ��带 ����
    node *nnode = new node;
    nnode->item = item;
    // 3. ��ũ�� ����
    // 3.1 rlink ���� (rlink: ->)
    nnode->rlink = curr->rlink;
    curr->rlink = nnode;
    // 3.2 llink ���� (link: <-)
    nnode->llink = curr;
    nnode->rlink->llink = nnode;
}

// �ڱ��� #09 ���� ���� ����Ʈ ���� O(n)
void node::delete(data_type item)
{
    // 1. ������ ���Ҹ� ã��
    node *curr = first;
    while (curr->rlink != NULL)
    {
        if (curr->rlink->item == item)
            break;
        curr = curr->rlink;
    }
    // 2. ��ũ�� ����
    node *dnode = curr->rlink;
    curr->rlink = dnode->rlink;
    curr->rlink->llink = dnode->llink; // �ƴϸ� curr
    // 3. ������ ��带 free
    free(dnode);
}

// �ڱ��� #10 ���� push
void Stack::push(Datatype DataItem)
{
    if (is_Full())
        printf("Pushing in Full stack\n");

    Items[TOP] = DataItem;
    TOP++;
}

// �ڱ��� #11 ���� pop
Datatype Stack::pop()
{
    if (is_Empty())
        printf("Poping from Empty Stack\n");

    TOP--;
    retrun Items[TOP];
}

// �ڱ��� #12 ť push
void queue::push(element item)
{
    if (isFull())
        printf("cannot add an element to a full queue");
    
    Items[++rear] = item;
}

// �ڱ��� #13 ť pop
void queue::pop(element item)
{
    if(isEmpty())
        printf("You cannot delete from an empty queue");
    
    return Items[++front];
}