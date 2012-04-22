#include "Parser.h"

#include <cstdio>
//#define DBG std::cerr << __FILE__ ": " << __LINE__ << ": " << __func__ << "()" << std::endl;
//#define DBG std::cerr << __LINE__ << ": " << __func__ << "()" << std::endl;
//#define BEGIN std::cerr << __func__ << "()" << " {" << std::endl;
//#define END std::cerr << __func__ << "()" << " }" << std::endl;
#define B_ printf( "%s(%d:'%s') {\n", __func__, m_token.m_pos, m_token.toString().c_str() ); fflush(NULL);
#define E_ printf( "%s(%d:'%s') }\n", __func__, m_token.m_pos, m_token.toString().c_str() ); fflush(NULL);

void Parser::error( const std::string& message ) const
{
  throw ParseError( m_token, message );
}

void Parser::next()
{
  m_token = m_lexer->nextToken();
}

ExprBase *Parser::parseBasicExpression()
{
  ExprBase *result = NULL;

  if (isNumber()) {
    result = parseNumber();
  } else if (isLParen()) {
    next(); // Consume '('
    result = parseExpression();
    if (!isRParen()) error( "Expected ')'" );
  } else if (isFunction()) {
    result = parseFunction();
  } else if (isUnaryOp()) {
    ExprUnaryOp *op = new ExprUnaryOp( m_token );
    next(); // Consume '-'
    op->setArg( parseBasicExpression() );
    result = op;
  }

  return result;
}

ExprBase *Parser::parseExpression()
{ // 3 + 2 * 5 + 1
  ExprBase *result = parseBasicExpression();

  while (isBinaryOp()) {
    TokenType opLeft = m_token.m_type;
    ExprBase *leftExpr = result;
    ExprBinOp *leftBinOp = new ExprBinOp( m_token );

    next(); // Consume opLeft
    ExprBase *rightExpr = parseBasicExpression();

    if (isBinaryOp()) {
      TokenType opRight = m_token.m_type;
      if (Precedence[opRight] > Precedence[opLeft]) {
        ExprBinOp *rightBinOp = new ExprBinOp( m_token );
        next(); // Consume opRight
        rightBinOp->setArgs( rightExpr, parseBasicExpression() );
        leftBinOp->setArgs( leftExpr, rightBinOp );
      }
    } else
      leftBinOp->setArgs( leftExpr, rightExpr );

    result = leftBinOp;
    }

  return result;
}

ExprBase *Parser::parseFunction()
{ B_
  if (!isFunction())
    error( "Expected a function" );

  Function::ID funcID = m_token.m_data.id;

  ExprFunction *func = new ExprFunction( m_token );
  next(); // Consume function name
  if (!isLParen())
    error( "Expected '('" );
  next(); // Consume '('

  int argc = Function::numArguments[funcID];
  ExprBase *argv[MAX_FUNCTION_ARGUMENTS] = {NULL};
  for (int arg=0; arg<argc; arg++) {
    argv[arg] = parseExpression();

    switch (m_token.m_type) {
      case COMMA:
        next(); // Consume ','
        break;
      case RPAREN:
        if (arg < argc-1)
          error( "Too few arguments to function" );
        break;
      default:
        error( "Expected ',' or ')'" );
        break;
    }
  }

  if (isRParen()) {
    next(); // Consume ')'
    for (int arg=0; arg<argc; arg++)
      func->pushArg( argv[arg] );
  } else {
    error( "Expected ')'" );
  }
E_
  return func;
}

ExprBase *Parser::parseNumber()
{ B_
  ExprBase *num = NULL;
  switch (m_token.m_type) {
    case CONSTANT:
      num = new ExprConstant(m_token);
      next(); // Consume constant
      break;
    case FLOAT:
      num = new ExprLiteral(m_token);
      next(); // Consume literal
      break;
    default:
      error( "Expected a constant or literal" );
      break;
  }
E_
  return num;
}

bool Parser::isEnd()
{
  return m_token.m_type == END;
}

bool Parser::isComma()
{
  return m_token.m_type == COMMA;
}

bool Parser::isLParen()
{
  return m_token.m_type == LPAREN;
}

bool Parser::isRParen()
{
  return m_token.m_type == RPAREN;
}

bool Parser::isUnaryOp()
{
  if (m_token.m_type >= NEGATIVE &&
      m_token.m_type <= FACTORIAL)
    return true;
  else
    return false;
}

bool Parser::isBinaryOp()
{
  if (m_token.m_type >= PLUS &&
      m_token.m_type <= POWER)
    return true;
  else
    return false;
}

bool Parser::isNumber()
{
  if (m_token.m_type >= CONSTANT &&
      m_token.m_type <= FLOAT)
    return true;
  else
    return false;
}

bool Parser::isFunction()
{
  return m_token.m_type == FUNCTION;
}

std::map<TokenType,int> initPrecedenceMap()
{
  std::map<TokenType,int> pm;

  pm[PLUS]     = 10;
  pm[MINUS]    = 20;
  pm[MULT]     = 30;
  pm[DIV]      = 30;
  pm[NEGATIVE] = 40;
  pm[POWER]    = 50;

  return pm;
}

std::map<TokenType,int> Parser::Precedence(initPrecedenceMap());
