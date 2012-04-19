#include <list>
#include <map>

#include "Lexer.h"
#include "Expr.h"

class Parser {
  private:
    Token *m_currToken, *m_prevToken, *m_ast;
    bool m_parseOK;

  public:
    Parser(const Lexer& lexer) : m_parseOK(true)
    {
      parseTokens( lexer );
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
