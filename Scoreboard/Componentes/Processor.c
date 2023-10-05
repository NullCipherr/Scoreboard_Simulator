#include <bus.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_REGISTERS        10 // Changed to 10 registers
#define NUM_FUNCTIONAL_UNITS 3

// Structure to track the state of registers
struct Register {
  int  value;           // Register value
  bool writeInProgress; // Indicates if a write operation is in progress
  bool readInProgress;  // Indicates if a read operation is in progress
};

// Definition of the register bank with 10 registers
struct Register registers[NUM_REGISTERS];

// Functional units (FUs) defined as global variables
int aluResult;
int memResult;

// Function to perform an addition operation
void
add(int destination_register, int source_register1, int source_register2) {
  // Mark destination and source registers as in use
  registers[destination_register].writeInProgress = true;
  registers[source_register1].readInProgress      = true;
  registers[source_register2].readInProgress      = true;

  aluResult =
      registers[source_register1].value + registers[source_register2].value;
  registers[destination_register].value           = aluResult;
  registers[destination_register].writeInProgress = false;
}

// Function to perform a subtraction operation
void
subtract(int destination_register, int source_register1, int source_register2) {
  // Mark destination and source registers as in use
  registers[destination_register].writeInProgress = true;
  registers[source_register1].readInProgress      = true;
  registers[source_register2].readInProgress      = true;

  aluResult =
      registers[source_register1].value - registers[source_register2].value;
  registers[destination_register].value           = aluResult;
  registers[destination_register].writeInProgress = false;
}

// Function to read a value from memory
int
readMemory(int address) {
  // Simulate a memory read
  // In this example, we return a fixed value
  memResult = 42;
  return memResult;
}

// Function to write a value to memory
void
writeMemory(int address, int value) {
  // Simulate a memory write
  // We only update the memResult variable here
  memResult = value;
}

// Function to check if all registers are free
bool
allRegistersFree(struct Register registers[]) {
  for (int i = 0; i < NUM_REGISTERS; i++) {
    if (registers[i].writeInProgress || registers[i].readInProgress) {
      return false;
    }
  }

  return true;
}

// Function to check if a functional unit is free
bool
isFunctionalUnitFree(bool functionalUnits[], int unit) {
  return !functionalUnits[unit];
}

// Function to mark a functional unit as occupied
void
occupyFunctionalUnit(bool functionalUnits[], int unit) {
  functionalUnits[unit] = true;
}

// Function to release a functional unit
void
releaseFunctionalUnit(bool functionalUnits[], int unit) {
  functionalUnits[unit] = false;
}
