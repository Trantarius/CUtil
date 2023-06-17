#include <cinttypes>

inline uint64_t bitroll(uint64_t num,int8_t by){
  return (num<<(by%64))|(num>>((64-by)%64));
}

inline int64_t bitroll(int64_t num,int8_t by){
  return (int64_t)bitroll((uint64_t)num,by);
}

inline uint32_t bitroll(uint32_t num,int8_t by){
  return (num<<(by%32))|(num>>((32-by)%32));
}

inline int32_t bitroll(int32_t num,int8_t by){
  return (int32_t)bitroll((uint32_t)num,by);
}

inline uint16_t bitroll(uint16_t num,int8_t by){
  return (num<<(by%16))|(num>>((16-by)%16));
}

inline int16_t bitroll(int16_t num,int8_t by){
  return (int16_t)bitroll((uint16_t)num,by);
}

inline uint8_t bitroll(uint8_t num,int8_t by){
  return (num<<(by%8))|(num>>((8-by)%8));
}

inline int8_t bitroll(int8_t num,int8_t by){
  return (int8_t)bitroll((uint8_t)num,by);
}
