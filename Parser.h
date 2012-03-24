#include <list>
#include <map>

#include "Lexer.h"
#include "Expr.h"

class Parser {
  private:
    std::list<Token>* m_tokens;
    std::list<Token>::iterator m_item;
    bool m_parseOK;

  public:
    Parser(std::list<Token>* tokens) : m_tokens(tokens), m_parseOK(true)
    {
      m_item = m_tokens->begin();
      parseTokens();
    }

    ExprBase* getAST() { return m_tokens->front().expr(); }

    bool OK() const { return m_parseOK; }

    static std::map<TokenType,int> Precedence;

  private:
    bool itemIsOp();
    bool itemIsBinOp();
    bool itemIsUnaryOp();
    bool itemIsFunc();

    bool parseFunc();
    bool parseUnaryOp();
    bool parseBinOp();

    void eraseEnclosingParen();
    void parseTokens();
    void nextOpItem();
    int tokenPos();
};
