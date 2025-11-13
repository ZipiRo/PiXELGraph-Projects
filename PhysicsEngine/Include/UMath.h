#pragma once

#include <cmath>

const double PI = 3.1415926535897932384;

const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

float NormalizeAngle(float &angle);

void heapify(int arr[], int n, int i);
void HeapSort(int arr[], int n);