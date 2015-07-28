#include <cstdint>
#include <iostream>
typedef uint32_t valueint_t;

class Value {
public:
  static const valueint_t MAX = 59049;
  static const size_t WIDTH = 10;
  Value();
  Value(valueint_t value);
  Value(const Value& other);
  Value& operator=(valueint_t value);
  Value& operator=(const Value& other);
  Value operator++(int);
  Value rotate() const;
  valueint_t unwrap() const;
  size_t ptr() const;

private:
  valueint_t value;
};

Value crazy(Value v1, Value v2);
std::ostream& operator<<(std::ostream& stream, const Value& value);
