#include <iostream>

#include "Parser.h"

using namespace std;

bool Parser::itemIsOp()
{
  if (m_item->type() >= NEGATIVE &&
      m_item->type() <= FUNCTION)
    return true;
  else
    return false;
}

bool Parser::itemIsBinOp()
{
  if (m_item->type() >= PLUS &&
      m_item->type() <= POWER)
    return true;
  else
    return false;
}

bool Parser::itemIsUnaryOp()
{
  if (m_item->type() == NEGATIVE)
    return true;
  else
    return false;
}

bool Parser::itemIsFunc()
{
  if (m_item->type() == FUNCTION)
    return true;
  else
    return false;
}

void Parser::nextOpItem()
{
  do {
    m_item++;
    if (m_item == m_tokens->end()) break;
  } while(!itemIsOp());
}

int Parser::tokenPos()
{
  int i=0;
  for (list<Token>::const_iterator it = m_tokens->begin(); it != m_item; it++, i++);
  return i;
}

void Parser::eraseEnclosingParen()
{
  while (true) {
    list<Token>::iterator before  = m_item,
                          before2 = m_item,
                          after   = m_item;
    before2--; before2--; before--; after++;

    if (before->type() == LPAREN && after->type() == RPAREN && before2->type() != FUNCTION) {
      m_tokens->erase(before);
      m_tokens->erase(after);
    } else {
      break;
    }
  }
}

void Parser::parseTokens()
{
  bool listChanged = false;

  while (m_tokens->size() > 1) {
    Lexer::printTokens(m_tokens); //dbg
    clog << "m_item : " << tokenPos() << ": " << string(listChanged?"changed":"unchanged") << endl; //dbg
    if (itemIsFunc())
      listChanged = parseFunc()?true:listChanged;
    else if (itemIsBinOp())
      listChanged = parseBinOp()?true:listChanged;
    else if (itemIsUnaryOp())
      listChanged = parseUnaryOp()?true:listChanged;

    nextOpItem();
    if (m_item == m_tokens->end()) {
      if (listChanged) {
        m_item = m_tokens->begin();
        listChanged = false;
      } else {
        cerr << __func__ << "(): " << __LINE__ << ": Unable to parse input. Quiting :-(" << endl;
        m_parseOK = false;
        break;
      }
    }
  }
}

bool Parser::parseFunc()
{
  list<Token>::iterator lparen = m_item;
  lparen++; // goto '('

  // after1 :: one after '('
  list<Token>::iterator after1 = lparen,
                        after2 = lparen,
                        after3 = lparen,
                        after4 = lparen,
                        after5 = lparen;
  after1++;
  after2++; after2++;
  after3++; after3++; after3++;
  after4++; after4++; after4++; after4++;
  after5++; after5++; after5++; after5++; after5++;

  if (lparen->type() == LPAREN) {
    if (after1->type() == RPAREN) {
      cerr << __func__ << "(): " << __LINE__ << ": Function with no arguments, unsupported" << endl;
      //throw;
    }
    if (after1->type() >= VARX &&
        after1->type() <= EXPR &&
        after2->type() == RPAREN) {
      // Function with one argument
      ExprBase *arg = after1->expr();
      static_cast<ExprFunction*>(m_item->expr())->setArgs(arg);
      m_tokens->erase(lparen, after3); // erase '(', arg, ')'
      m_item->useAsExpr();
      eraseEnclosingParen();

      return true;
    }
    if (after1->type() >= VARX &&
        after1->type() <= EXPR &&
        after2->type() == COMMA &&
        after3->type() >= VARX &&
        after3->type() <= EXPR &&
        after4->type() == RPAREN) {
      // Function with two arguments
      ExprBase *arg1 = after1->expr(), *arg2 = after3->expr();
      static_cast<ExprFunction*>(m_item->expr())->setArgs(arg1, arg2);
      m_tokens->erase(lparen, after5); // erase '(', arg1, ',', arg2, ')'
      m_item->useAsExpr();
      eraseEnclosingParen();

      return true;
    }
    if (after1->type() >= VARX &&
        after1->type() <= EXPR &&
        after2->type() == COMMA &&
        after3->type() >= VARX &&
        after3->type() <= EXPR &&
        after4->type() == COMMA) {
      // Function with three or more arguments
      cerr << __func__ << "(): " << __LINE__ << ": Function with >3 arguments, unsupported" << endl;
      //throw;
    }
  } else {
    cerr << __func__ << "(): " << __LINE__ << ": No '(' after function name." << endl;
    //throw;
  }
  return false;
}

bool Parser::parseUnaryOp()
{ //FIXME Assumes UnaryOp == NEGATIVE
  list<Token>::iterator before  = m_item,
                        before2 = m_item,
                        after   = m_item,
                        after2  = m_item;
  before2--; before2--; before--; after++; after2++; after2++;

  if (after == m_tokens->end()) {
    cerr << __func__ << "(): " << __LINE__ << ": No argument for unary operator '-'" << endl;
    //throw;
  }

  if (after->type() >= VARX &&
      after->type() <= EXPR) {
    if (after2 != m_tokens->end() &&
        after2->type() >= PLUS &&
        after2->type() <= POWER &&
        Precedence[m_item->type()] < Precedence[after2->type()]) {
      clog << "Op " << tokenToString(m_item->type())
           << " has lower precedence than " << tokenToString(after2->type()) << endl;
      return false; // If the next operator has higher precedence, leave
    }

    ExprBase *right = after->expr();
    static_cast<ExprUnaryOp*>(m_item->expr())->setArgs(right);
    m_tokens->erase(after);
    m_item->useAsExpr();
    eraseEnclosingParen();
    return true;
  }

  return false;
}

bool Parser::parseBinOp()
{
  list<Token>::iterator before  = m_item,
                        before2 = m_item,
                        after   = m_item,
                        after2  = m_item;
  before2--; before2--; before--; after++; after2++; after2++;

  // x * y - 2
  //   ^---::m_item
  if (after == m_tokens->end()) {
    cerr << __func__ << "(): " << __LINE__ << ": No second argument for binary operator" << endl;
    //throw;
  } else if (m_item == m_tokens->begin()) { // 'before' exists
    cerr << __func__ << "(): " << __LINE__ << ": No first argument for binary operator" << endl;
    //throw;
  }

  if (before->type() >= VARX &&
      before->type() <= EXPR &&
      after->type() >= VARX &&
      after->type() <= EXPR) {
    clog << "Binop " << tokenToString(m_item->type()) << " is simple" << endl;
    if (after2 != m_tokens->end() &&
        after2->type() >= PLUS &&
        after2->type() <= POWER &&
        Precedence[m_item->type()] < Precedence[after2->type()]) {
      clog << "Binop " << tokenToString(m_item->type())
           << " has lower precedence than " << tokenToString(after2->type()) << endl;
      return false; // If the next operator has higher precedence, leave
    }
    if (before != m_tokens->begin() && // 'before2' exists
        before2->type() >= PLUS &&
        before2->type() <= POWER &&
        Precedence[m_item->type()] < Precedence[before2->type()]) {
      clog << "Binop " << tokenToString(m_item->type())
           << " has lower precedence than " << tokenToString(before2->type()) << endl;
      return false; // If the previous operator has higher precedence, leave
    }

    ExprBase *left = before->expr(), *right = after->expr();
    static_cast<ExprBinOp*>(m_item->expr())->setArgs(left, right);
    m_tokens->erase(before);
    m_tokens->erase(after);
    m_item->useAsExpr();
    eraseEnclosingParen();
    return true;
  }
  return false;
}

map<TokenType,int> initPrecedenceMap()
{
  map<TokenType,int> pm;

  pm[PLUS]     = 10;
  pm[MINUS]    = 10;
  pm[NEGATIVE] = 10;
  pm[MULT]     = 20;
  pm[DIV]      = 20;
  pm[POWER]    = 30;

  return pm;
}

map<TokenType,int> Parser::Precedence(initPrecedenceMap());
