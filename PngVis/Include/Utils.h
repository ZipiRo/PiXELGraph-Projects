const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

float NormalizeAngle(float angle) 
{
    angle = angle - floor(angle / (2.0f * PI)) * (2.0f * PI);
    
    if (angle < 0)
        angle += 2.0f * PI;
    
    return angle;
}

void heapify(int arr[], int n, int i) 
{
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // Left child index
    int right = 2 * i + 2; // Right child index

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest]) 
    {
        largest = left;
    }

    // If right child is larger than the largest so far
    if (right < n && arr[right] > arr[largest]) 
    {
        largest = right;
    }

    // If largest is not root, swap with the largest child and heapify the affected subtree
    if (largest != i) 
    {
        int aux = arr[i];
        arr[i] = arr[largest];
        arr[largest] = aux;
        heapify(arr, n, largest); // Recursively heapify the affected subtree
    }
}

void HeapSort(int arr[], int n) 
{
    // Build max-heap (rearrange the array)
    for (int i = n / 2 - 1; i >= 0; i--) 
    {
        heapify(arr, n, i);
    }

    // One by one extract elements from heap
    for (int i = n - 1; i > 0; i--) 
    {
        // Move current root to the end
        int aux = arr[0];
        arr[0] = arr[i];
        arr[i] = aux;

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}