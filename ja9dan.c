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