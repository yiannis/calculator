#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <cassert>
#include <exception>
#include <cassert>

#include "Function.h"
#include "Token.h"

#define MAX_FUNCTION_ARGUMENTS 4


class ExprBase {
  protected:
    Token m_token;

  public:
    ExprBase(Token t) : m_token(t) {}

    // Expose token
    TokenType type() const { return m_token.m_type; }
    int position() const { return m_token.m_pos; }
    std::string toString() const { return m_token.toString(); }

    virtual float result() const = 0;

    void error( const std::string& message ) const;
};

class ExprLiteral : public ExprBase {
  public:
    ExprLiteral(Token t) :
      ExprBase(t)
    {
      assert( m_token.m_type == FLOAT );
    }

    virtual float result() const { return m_token.m_data.value; }
};

class ExprConstant : public ExprBase {
  public:
    ExprConstant(Token t) :
      ExprBase(t)
    {
      assert( m_token.m_type == CONSTANT );
    }

    virtual float result() const { return *(m_token.m_data.pValue); }
};

class ExprUnaryOp : public ExprBase {
  private:
    ExprBase *m_expr;

  public:
    ExprUnaryOp(Token t, ExprBase* expr = NULL)
      : ExprBase(t), m_expr(expr)
    {
      assert( m_token.m_type == NEGATIVE );
    }

    void setArg(ExprBase* expr) {
      assert( expr != NULL );
      m_expr = expr;
    }

    virtual float result() const;
};

class ExprBinOp : public ExprBase {
  private:
    ExprBase *m_exprLeft, *m_exprRight;

  public:
    ExprBinOp(Token t, ExprBase* left = NULL, ExprBase* right = NULL)
      : ExprBase(t), m_exprLeft(left), m_exprRight(right)
    {
      assert( m_token.m_type >= PLUS && m_token.m_type <= POWER );
    }

    void setArgs(ExprBase* left, ExprBase* right)
    {
      assert( left != NULL );
      assert( right != NULL );

      m_exprLeft = left;
      m_exprRight = right;
    }

    virtual float result() const ;
};

class ExprFunction : public ExprBase {
  private:
    int m_numArgs;
    ExprBase* m_argv[MAX_FUNCTION_ARGUMENTS];

  public:
    ExprFunction( Token t ) :
      ExprBase(t), m_numArgs(0)
    {
      assert( m_token.m_type == FUNCTION );

      for (int i=0; i<MAX_FUNCTION_ARGUMENTS; i++)
        m_argv[i] = NULL;
    }

    void pushArg( ExprBase* arg )
    {
      assert( arg != NULL );

      if (m_numArgs < MAX_FUNCTION_ARGUMENTS) {
        m_argv[m_numArgs] = arg;
        m_numArgs++;
      } else
        error( "Asked to exceed maximum function arguments" );
    }

    virtual float result() const;
};

class ExprError : public std::exception
{
  public:
    ExprError( const ExprBase* expr, const std::string& message )
    {
      std::ostringstream print;
      print << "expr error: "
            << expr->position() << ": " << expr->toString()
            << ": " << message;
      m_message = print.str();
    }

    virtual const char* what() const throw()
    {
      return m_message.c_str();
    }
    virtual ~ExprError() throw() {}

  private:
    std::string m_message;
};


#endif //EXPR_H
