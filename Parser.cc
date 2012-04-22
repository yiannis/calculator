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

ExprBase *Parser::parseFunction()
{ B_
  if (!isFunction())
    error( "Expected a function" );

  Function::ID funcID = m_token.m_data.id;

  ExprFunction *func = new ExprFunction( m_token );
  next(); // Get '('
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

ExprBase *Parser::parseUnaryOp()
{ // FIXME This is for prefix operators only
 B_ 
  if (!isUnaryOp())
    error( "Expected a unary operator" );

  ExprUnaryOp *op = new ExprUnaryOp( m_token );

  next(); // Consume operator
  ExprBase *expr = parseExpression(false);
  next();
  if (isBinaryOp()) {
    if (Precedence[m_token.m_type] > Precedence[op->type()])
      op->setArg( parseBinaryOp( expr ) );
    else {
      op->setArg( expr );
      return parseBinaryOp( op );
    }
  } else
      op->setArg( expr );
E_
  return op;
}

ExprBase *Parser::parseBinaryOp( ExprBase *left )
{ B_
  if (!isBinaryOp())
    error( "Expected a binary operator" );

  ExprBinOp *op = new ExprBinOp( m_token );

  next(); // Consume operator
  ExprBase *right = parseExpression(false);
  next(); // Get next token

  // 3+5*2
  // 3*5+2
  if (isBinaryOp()) {
    if (Precedence[m_token.m_type] > Precedence[op->type()])
      op->setArgs( left, parseBinaryOp( right ) );
    else {
      op->setArgs( left, right );
      return parseBinaryOp( op );
    }
  } else
    op->setArgs( left, right );
E_
  return op;
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

ExprBase *Parser::parseExpression( bool greedy )
{ B_
  ExprBase *expr = NULL;

  if (isLParen()) {
    next(); // Consume '('
    expr = parseExpression();
    if (isRParen())
      next(); // Consume ')'
    else
      error( "Expected ')'" );

//    next(); // Get next token
  } else if (isNumber()) {
    expr = parseNumber();
  } else if (isFunction()) {
    expr = parseFunction();
  } else if (isUnaryOp()) {
    expr = parseUnaryOp();
  }

  if (greedy) {
    if (isBinaryOp()) {
      return parseBinaryOp( expr );
    } else if (!(isRParen() || isComma() || isEnd()))
      error( "Expected ')' or ',' or EOF" );
  }
E_
  return expr;
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
