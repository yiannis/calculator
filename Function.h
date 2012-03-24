#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

class Function {
  public:
    enum ID {
      SIN,
      COS,
      TAN,
      ACOS,
      ASIN,
      ATAN,
      EXP,
      LOG,
      LOG10,
      SQRT,
    
      POW,
      ATAN2,

      UNKNOWN,
    };

  static ID nameToID(std::string name);
  static std::string IDtoName(ID id);
  static float call(ID id, float arg);
  static float call(ID id, float arg1, float arg2);
};

#endif //FUNCTION_H
