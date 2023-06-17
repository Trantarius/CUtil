#include <cinttypes>

inline uint64_t bitroll(uint64_t num,int8_t by){
  return (num<<by)|(num>>(64-by));
}

inline int64_t bitroll(int64_t num,int8_t by){
  return (int64_t)bitroll((uint64_t)num,by);
}

inline uint32_t bitroll(uint32_t num,int8_t by){
  return (num<<by)|(num>>(32-by));
}

inline int32_t bitroll(int32_t num,int8_t by){
  return (int32_t)bitroll((uint32_t)num,by);
}

inline uint16_t bitroll(uint16_t num,int8_t by){
  return (num<<by)|(num>>(16-by));
}

inline int16_t bitroll(int16_t num,int8_t by){
  return (int16_t)bitroll((uint16_t)num,by);
}

inline uint8_t bitroll(uint8_t num,int8_t by){
  return (num<<by)|(num>>(8-by));
}

inline int8_t bitroll(int8_t num,int8_t by){
  return (int8_t)bitroll((uint8_t)num,by);
}
