/*
 * Copyright t
 */
#include "value.hxx"
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

const size_t MEMORY_SIZE = 59049;
valueint_t ENCRYPT_MAP[94] = {
  57, 109, 60, 46, 84, // 0-4
  86, 97, 99, 96, 117, // 5-9
  89, 42, 77, 75, 39, // 10-14
  88, 126, 120, 68, 108, // 15-19
  125, 82, 69, 111, 107, // 20-24
  78, 58, 35, 63, 71, // 25-29
  34, 105, 64, 53, 122, // 30-34
  93, 38, 103, 113, 116, // 35-39
  121, 102, 114, 36, 40, // 40-44
  119, 101, 52, 123, 87, // 45-49
  80, 41, 72, 45, 90, // 50-54
  110, 44, 91, 37, 92,// 55-59
  51, 100, 76, 43, 81,// 60-64
  59, 62, 85, 33, 112,// 65-69
  74, 83, 55, 50, 70, // 70-74
  104, 79, 65, 49, 67,// 75-79
  66, 54, 118, 94, 61, // 80-84
  73, 95, 48, 47, 56, // 85-89
  124, 106, 115, 98 // 90-93
};

enum ReadStatus {
  READ_OKAY,
  FILE_NOT_FOUND,
  PROGRAM_TOO_LARGE,
  SYNTAX_ERROR
};

ReadStatus read_source(std::vector<Value>&, const char *fileName);
void fill_memory(std::vector<Value>&);
int eval(std::vector<Value>);
void encrypt(std::vector<Value>& memory, Value c);
void dump_memory(std::vector<Value>&, const char* fileName);

int main(int argc, char* argv[]) {
  std::vector<Value> memory;
  ReadStatus error;
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FILE>" << std::endl;
    return 1;
  }
  if((error = read_source(memory, argv[1])) != READ_OKAY) {
    switch(error) {
    case FILE_NOT_FOUND:
      std::cerr << "File not found or read unsuccessful" << std::endl;
      break;
    case PROGRAM_TOO_LARGE:
      std::cerr << "Program overflowed memory with size" << std::endl;
      break;
    case SYNTAX_ERROR:
      std::cerr << "Syntax error" << std::endl;
      break;
    }
    return 1;
  }

  fill_memory(memory);

  return eval(memory);
}

void dump_memory(std::vector<Value>& memory, const char* fileName) {
  std::ofstream file(fileName);
  if(!file) {
    return;
  }
  for(Value v : memory) {
    file << v << std::endl;
  }
}

ReadStatus read_source(std::vector<Value>& memory, const char* fileName) {
  std::ifstream file(fileName, std::ios::in | std::ios::binary);
  if(!file) {
    return FILE_NOT_FOUND;
  }
  int byte;
  while((byte = file.get()) != EOF) {
    memory.push_back(Value((valueint_t)byte));
    continue;
    switch(byte) {
    case 4:
    case 5:
    case 23:
    case 39:
    case 40:
    case 62:
    case 68:
    case 81:
      // valid
      memory.push_back(Value((valueint_t)byte));
      if(memory.size() > MEMORY_SIZE) {
        return PROGRAM_TOO_LARGE;
      }
      break;
    default:
      if(!isspace(byte)) {
        // invalid
        return SYNTAX_ERROR;
      } else {
        memory.push_back(Value((valueint_t)byte));
      }
    }
  }
  return READ_OKAY;
}

void fill_memory(std::vector<Value>& memory) {
  while(memory.size() < MEMORY_SIZE) {
    Value value = crazy(memory[memory.size() - 1], memory[memory.size() - 2]);
    memory.push_back(value);
  }
}

int eval(std::vector<Value> memory) {
  Value a, c, d;
  Value result;
  while(true) {
    valueint_t instruction = memory[c.ptr()].unwrap();

    switch((memory[c.ptr()].unwrap() + c.unwrap()) % 94) {
    case 4:
      // jmp [d] + 1
      // The +1 takes place after the switch{}
      c = memory[d.ptr()];
      break;
    case 5:
      // out a
      std::cout << (char)a.unwrap();
      std::cout.flush();
      break;
    case 23:
      // in a
      a = std::cin.get();
      break;
    case 39:
      // rotr [d]
      // mov a, [d]
      result = memory[d.ptr()].rotate();
      memory[d.ptr()] = result;
      a = result;
      break;
    case 40:
      // mov d, [d]
      d = memory[d.ptr()];
      break;
    case 62:
      // crz [d], a
      // mov a, [d]
      result = crazy(memory[d.ptr()], a);
      memory[d.ptr()] = result;
      a = result;
      break;
    case 68:
      // nop
      break;
    case 81:
      // exit
      return 0;
    default:
      // nop
      break;
    }
    encrypt(memory, c);
    c++;
    d++;
  }
}

void encrypt(std::vector<Value>& memory, Value c) {
  memory[c.ptr()] = ENCRYPT_MAP[memory[c.ptr()].ptr() % 94];
}
