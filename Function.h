#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

class Function {
  public:
    enum ID {
      SIN = 0,
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
      NUM_FUNCTIONS,
    };

    static int *numArguments;

    static int *fillNumArguments();
    static int numFunctions();
    static ID nameToID(std::string name);
    static std::string IDtoName(ID id);
    static bool isNameValid( const std::string& name );
    static float call(ID id, float arg);
    static float call(ID id, float arg1, float arg2);
};

#endif //FUNCTION_H
