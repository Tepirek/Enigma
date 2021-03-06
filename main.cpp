#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#pragma warning(disable:4996)

struct Rotor {
	int position;
	int amountOfNotches;
	int* notch;
	int* code;
	int* operations;
};

int* encodeRotor(int numberOfLetters, int* code);
int* encodeRotorReverse(int numberOfLetters, int* code);
void printRotors(int numberOfLetters, int numberOfRotors, Rotor* rotors);
void enigmaMachine(Rotor* setup, int amountOfElements, int numberOfLetters, int amountOfRotors, int inputLength, int* input);
int modulo(int value, int max);
int modulo2(int value, int max);
void updateRotors(Rotor* setup, int amountOfRotors, int numberOfLetters);
bool contains(int value, int size, int* elements);
int findOperation(int goal, int position, int size, int* operations);

int main(void) {
	// Defintions of parts of the machine
	int numberOfLetters;
	scanf("%d", &numberOfLetters);
	int numberOfRotors;
	scanf("%d", &numberOfRotors);
	Rotor* rotors = new Rotor[numberOfRotors * sizeof(Rotor)];
	for (int i = 0; i < numberOfRotors; i++) {
		int* code = new int[numberOfLetters];
		for (int j = 0; j < numberOfLetters; j++) {
			scanf("%d", &code[j]);
		}
		rotors[i].position = 0;
		rotors[i].operations = encodeRotor(numberOfLetters, code);
		int notchAmount;
		scanf("%d", &notchAmount);
		int* notch = new int[notchAmount];
		for (int j = 0; j < notchAmount; j++) {
			scanf("%d", &notch[j]);
		}
		rotors[i].amountOfNotches = notchAmount;
		rotors[i].notch = notch;
		delete[] code;
		delete[] notch;
	}
	int numberOfReflectors;
	scanf("%d", &numberOfReflectors);
	Rotor* reflectors = new Rotor[numberOfReflectors];
	for (int i = 0; i < numberOfReflectors; i++) {
		int* code = new int[numberOfLetters];
		for (int j = 0; j < numberOfLetters; j++) {
			scanf("%d", &code[j]);
		}
		reflectors[i].position = 0;
		reflectors[i].operations = encodeRotor(numberOfLetters, code);
		delete[] code;
	}
	// A set of p tasks to perform
	int numberOfTasks;
	scanf("%d", &numberOfTasks);
	for (int i = 0; i < numberOfTasks; i++) {
		int amountOfRotors;
		scanf("%d", &amountOfRotors);
		int amountOfElements = (amountOfRotors * 2) + 1;
		Rotor* setup = new Rotor[amountOfElements * sizeof(Rotor)];
		for (int j = 0; j < amountOfRotors; j++) {
			int rotorIndex;
			scanf("%d", &rotorIndex);
			memcpy(&setup[j], &rotors[rotorIndex], sizeof Rotor);
			memcpy(&setup[amountOfRotors * 2 - j], &rotors[rotorIndex], sizeof Rotor);
			int rotorPosition;
			scanf("%d", &rotorPosition);
			setup[j].position = rotorPosition - 1;
			setup[amountOfRotors * 2 - j].position = rotorPosition - 1;
		}
		int reflectorIndex;
		scanf("%d", &reflectorIndex);
		memcpy(&setup[amountOfRotors], &reflectors[reflectorIndex], sizeof Rotor);
		// printRotors(numberOfLetters, amountOfElements, setup);
		int size = 16;
		int* input = new int[size];
		char c;
		while ((c = getchar()) != '\n');
		int inputLength = 0;
		for (int j = 0; ; j++) {
			inputLength = j;
			scanf("%d", &input[j]);
			if (input[j] == 0) break;
			if (j == size - 1) {
				size *= 2;
				int* tmp = input;
				input = new int[size];
				memcpy(input, tmp, size * sizeof(int));
				delete[] tmp;
			}
		}
		enigmaMachine(setup, amountOfElements, numberOfLetters, amountOfRotors, inputLength, input);
	}
	delete[] rotors;
	delete[] reflectors;
	return 0;
}

int* encodeRotor(int numberOfLetters, int* code) {
	int* operations = new int[numberOfLetters];
	for (int i = 0; i < numberOfLetters; i++) {
		operations[i] = code[i] - (i + 1);
	}
	return operations;
 }

int* encodeRotorReverse(int numberOfLetters, int* code) {
	int* operationsReverse = new int[numberOfLetters];
	for (int i = 0; i < numberOfLetters; i++) {
		operationsReverse[i] = (i + 1) - code[i];
	}
	return operationsReverse;
}

void printRotors(int numberOfLetters, int numberOfRotors, Rotor* rotors) {
	for (int i = 0; i < numberOfRotors; i++) {
		printf("Rotor %d -> ", i+1);
		for (int j = 0; j < numberOfLetters; j++) {
			printf("%d ", rotors[i].operations[j]);
		}
		printf(" Position: %d\n", rotors[i].position);
	}
}

void enigmaMachine(Rotor* setup, int amountOfElements, int numberOfLetters, int amountOfRotors, int inputLength, int* input) {
	// machine setup
	// encryption
	for (int i = 0; i < inputLength; i++) {
		updateRotors(setup, amountOfRotors, numberOfLetters);
		int value = input[i];
		int rotorPosition = 0;
		int rotorOperation = 0;
		int offset = modulo(value - 1, numberOfLetters);
		int position = modulo(offset + rotorPosition, numberOfLetters);
		for (int j = 0; j < amountOfElements; j++) {
			rotorPosition = setup[j].position;
			position = modulo(offset + rotorPosition, numberOfLetters);
			if (j <= amountOfRotors) {
				rotorOperation = setup[j].operations[position];
			}
			else {
				rotorOperation = findOperation(offset, setup[j].position, numberOfLetters, setup[j].operations);
			}
			value = modulo2(value + rotorOperation, numberOfLetters);
			offset = modulo(offset + rotorOperation, numberOfLetters);
			// printf("Rotor %d => value = %d, offset = %d, rP = %d, rO = %d\n", j + 1, value, offset, rotorPosition, rotorOperation);
		}
		printf("%d ", modulo2(value, numberOfLetters));
	}
	printf("\n");
	delete[] setup;
}

int modulo(int value, int max) {
	int r = value % max;
	return r < 0 ? r + max : r;
}

int modulo2(int value, int max) {
	int r = value % max;
	return r <= 0 ? r + max : r;
}

void updateRotors(Rotor* setup, int amountOfRotors, int numberOfLetters) {
		int position = setup[0].position;
		position = modulo(position + 1, numberOfLetters);
		setup[0].position = position;
		setup[amountOfRotors * 2 - 0].position = position;
		bool notch = setup[0].amountOfNotches;
		if (amountOfRotors > 1 && contains(position, setup[0].amountOfNotches, setup[0].notch)) {
			int position = setup[1].position;
			position = modulo(position + 1, numberOfLetters);
			setup[1].position = position;
			setup[amountOfRotors * 2 - 1].position = position;
		}
}

bool contains(int value, int size, int* elements) {
	for (int i = 0; i < size; i++) {
		if (elements[i] == value) return true;
	}
	return false;
}

int findOperation(int goal, int position, int size, int* operations) {
	for (int i = 0; i < size; i++) {
		int x = modulo(i + position, size);
		if (modulo(i + operations[x], size) == goal) return -operations[x];
	}
	return 0;
}
