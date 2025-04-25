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