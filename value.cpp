#include <cassert>
#include "value.hxx"

valueint_t CRAZY_TABLE[3][3] = {
  {1, 0, 0},
  {1, 0, 2},
  {2, 2, 1}
};

Value::Value(const Value& other) {
  *this = other;
}

Value::Value(valueint_t value) {
  assert(value <= Value::MAX);
  *this = value;
}

Value::Value() : value(0) { }

Value& Value::operator=(const Value& other) {
  this->value = other.unwrap();
}

Value& Value::operator=(valueint_t value) {
  assert(value <= Value::MAX);
  this->value = value;
}

Value Value::operator++(int _) {
  return Value(this->value++);
}

Value Value::rotate() const {
  return Value((value % 3) * (MAX / 3) + value / 3);
}

valueint_t Value::unwrap() const {
  return value;
}

size_t Value::ptr() const {
  return (size_t)value;
}


Value crazy(Value v1, Value v2) {
  auto int1 = v1.unwrap();
  auto int2 = v2.unwrap();
  valueint_t significance = 1;
  valueint_t result = 0;
  for(size_t i=0; i < Value::WIDTH; i++) {
    auto digit1 = int1 % 3;
    auto digit2 = int2 % 3;
    int1 /= 3;
    int2 /= 3;
    result += CRAZY_TABLE[digit1][digit2] * significance;
    significance *= 3;
  }
  return Value(result % Value::MAX);
}

std::ostream& operator<<(std::ostream& stream, const Value& value) {
  valueint_t buffer[Value::WIDTH];
  valueint_t inner = value.unwrap();
  for(size_t i=0; i < Value::WIDTH; i++) {
    buffer[Value::WIDTH - i - 1] = inner % 3;
    inner /= 3;
  }
  for(size_t i=0; i < Value::WIDTH; i++) {
    assert(buffer[i] < 3);
    stream << buffer[i];
  }
  return stream;
}
