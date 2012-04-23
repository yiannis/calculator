#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <exception>
#include <sstream>

#include "Lexer.h"
#include "Expr.h"
#include "Token.h"


class Parser {
  private:
    Lexer *m_lexer;
    Token m_token;
    ExprBase *m_ast;

  public:
    Parser(Lexer *lexer) :
      m_lexer(lexer), m_token(END, -1), m_ast(NULL) 
    {
      next();
      m_ast = parseExpression();
    }
    ~Parser() { delete m_ast; }

    const ExprBase* AST() const { return m_ast; }

    static std::map<TokenType,int> Precedence;

  private:
    void next();

    ExprBase *parseExpression( ExprBase *expr = NULL );
    ExprBase *parseBasicExpression();
    ExprBase *parseParenthesis();
    ExprBase *parseNumber();
    ExprBase *parseFunction();

    bool isLParen();
    bool isRParen();
    bool isComma();
    bool isUnaryOp();
    bool isBinaryOp();
    bool isNumber();
    bool isFunction();
    bool isEnd();

    void error( const std::string& message ) const;
    void error( bool condition, const std::string& message ) const;
};

class ParseError : public std::exception
{
  public:
    ParseError( const Token& token, const std::string& message )
    {
      std::ostringstream print;
      print << "parse error: "
            << token.m_pos << ": "
            << token.toString()<< ": "
            << message;
      m_message = print.str();
    }

    virtual const char* what() const throw()
    {
      return m_message.c_str();
    }
    virtual ~ParseError() throw() {}

  private:
    std::string m_message;
};

#endif //PARSER_H
