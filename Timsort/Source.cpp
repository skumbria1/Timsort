#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
using namespace std;

struct runCoord {
	int left, right;
};

int runLen(runCoord run) {
	return run.right - run.left + 1;
}

void insertionSort(vector<int> &list, int left, int right) { //сортировка вставками для ранов
	for (int i = 1; i < right - left + 1; i++) {
		int j = left + i;
		while (j > left and list[j - 1] > list[j]) {
			int c = list[j - 1];
			list[j - 1] = list[j];
			list[j] = c;
			j--;
		}
	}
}

void merge(vector<int> &list, runCoord leftarr, runCoord rightarr) {
	vector<int> buffer(runLen(leftarr));

	for (int i = 0; i < runLen(leftarr); i++) {
		buffer[i] = list[leftarr.left + i];
	}
	int indexBig = rightarr.left, indexTemp = 0, z = leftarr.left;
	while (indexTemp <= buffer.size() - 1 and indexBig <= rightarr.right) {
		if (buffer[indexTemp] <= list[indexBig]) {
			list[z] = buffer[indexTemp];
			indexTemp++;
			z++;
		}
		else {
			list[z] = list[indexBig];
			indexBig++;
			z++;
		}
	}
	if (indexTemp == buffer.size()) {
		while (indexBig <= rightarr.right) {
			list[z] = list[indexBig];
			indexBig++;
			z++;
		}
	}
	else if (indexBig == rightarr.right + 1) {
		while (indexTemp <= buffer.size() - 1) {
			list[z] = buffer[indexTemp];
			indexTemp++;
			z++;
		}
	}

}

int getMinrun(int n)
{
	int r = 0;
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

void timsort(vector<int> &list, int minrun, int listn) {
	int temp = 0;
	runCoord run1;
	vector<runCoord> stack;

	while (temp < listn - 2) {
		int left, right;

		left = temp;
		temp++;
		if (list[temp - 1] > list[temp]) {
			while (temp + 1 < listn) { //проверка значений  a0 > a1 > a2 ...
				if (list[temp] > list[temp + 1]) {
					temp++;
				}
				else { break; }
			}
			right = temp;
			for (int i = left; i < (right - left + 1) / 2; i++) { //сортировка по возрастанию
				int c = list[i];
				list[i] = list[right - i];
				list[right - i] = c;
			}
		}
		else {
			while (temp + 1 < listn) { //проверка значений a0 <= a1 <= a2 ...
				if (list[temp] <= list[temp + 1]) {
					temp++;
				}
				else { break; }
			}
			right = temp;
		}
		if ((right - left + 1) < minrun) { //если меньше минран, то дополнение до минрана 
			int z = (right - left + 1);
			for (int i = 0; i < minrun - z; i++) {
				if (temp >= listn - 1) { break; }
				temp++; 
			}
		}
		right = temp;
		insertionSort(list, left, right);
		temp++;
		run1.left = left;
		run1.right = right;
		stack.push_back(run1);
		bool first = 0, second = 0, truth = 0;
		while (stack.size() > 1 and truth == 0) {
			int n = stack.size() - 2; //n - 1 = Z, n = Y, n + 1 = X
			if (stack.size() > 1 and runLen(stack[n]) <= runLen(stack[n + 1])) {
				merge(list, stack[n], stack[n + 1]);
				stack[n].right = stack[n + 1].right;
				stack.erase(stack.begin() + n + 1);
				n--;
			}
			else if (stack.size() > 2 and runLen(stack[n - 1]) <= runLen(stack[n]) + runLen(stack[n + 1])) {
				if (runLen(stack[n - 1]) <= runLen(stack[n + 1])) {
					merge(list, stack[n - 1], stack[n]);
					stack[n - 1].right = stack[n].right;
					stack.erase(stack.begin() + n);
					n--;
				}
				else {
					merge(list, stack[n], stack[n + 1]);
					stack[n].right = stack[n + 1].right;
					stack.erase(stack.begin() + 2);
					n--;
				}
			}
			else if (temp == listn) {
				if (runLen(stack[n]) >= runLen(stack[n + 1])) {
					merge(list, stack[n], stack[n + 1]);
					stack[n].right = stack[n + 1].right;
					stack.erase(stack.begin() + n + 1);
				}
			}
			if (stack.size() > 1 and (temp != listn) and runLen(stack[n]) > runLen(stack[n + 1])) {
				first = 1;
				if (stack.size() < 3) {
					second = 1;
				}
				else if (stack.size() > 2 and runLen(stack[n - 1]) > runLen(stack[n]) + runLen(stack[n + 1])) {
					second = 1;
				}
				if (first and second) truth = 1;
			}
			
			
		}
	}

}

int main() {
	srand(time(0));
	int minrun, n, runNumber = 0;
	cout << "Number of elements: ";
	while (true) {
		cin >> n;
		if (n < 2) {
			continue;
		}
		else { break; }
	}
	vector<int> list(n);

	for (int i = 0; i < n; i++) {
		//list[i] = i;
		list[i] = (rand() % 100);
	}
	minrun = getMinrun(n);
	//cout << "\n" << minrun << "\n";
	auto begin = chrono::high_resolution_clock::now();
	//insertionSort(list, 0, n - 1);
	timsort(list, minrun, n);
	auto end = chrono::high_resolution_clock::now();
	cout << (chrono::duration_cast<chrono::nanoseconds>(end - begin).count()) / 1000000 << "ms" << endl;
	system("pause");
}