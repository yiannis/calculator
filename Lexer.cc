#include <iostream>
#include <string>
#include <cctype>
#include <cmath>

#include "Lexer.h"

using namespace std;


Token Lexer::scan()
{
  while (m_input->good()) {
    int in = m_input->get();

    if (isalpha(in)) {
      m_input->unget();
      return scanString();
    } else if (isdigit(in)) {
      m_input->unget();
      return scanFloat();
    } else if (isspace(in)) {
      continue;
    } else {
      switch (in) {
        case '+':
          return Token(PLUS, m_charPos);
        case '-':
          if (m_charPos == 0 ||
              m_token.m_type == LPAREN ||
              m_token.m_type == COMMA)
            return Token(NEGATIVE, m_charPos);
          else
            return Token(MINUS, m_charPos);
        case '*':
          return Token(MULT, m_charPos);
        case '/':
          return Token(DIV, m_charPos);
        case '^':
          return Token(POWER, m_charPos);
        case '(':
          return Token(LPAREN, m_charPos);
        case ')':
          return Token(RPAREN, m_charPos);
        case ',':
          return Token(COMMA, m_charPos);
        default:
          cerr << "Warning: Unknown charachter '" << (char)in
               << "' at position " << m_charPos << endl;
      }
    }
  }

  return Token(END, m_charPos);
}

Token Lexer::scanString()
{
  int in;
  string name;
  while (m_input->good()) {
    in = m_input->get();

    if (isalnum(in)) {
      name += (char)in;
    } else {
      m_input->unget();
      break;
    }
  }

  // Check if 'name' is a user constant
  map<string,const float*>::const_iterator p_value;
  p_value = m_constants.find( name );
  if (p_value != m_constants.end())
    return Token( *(p_value->second), m_charPos );

  // Check if 'name' is a builtin constant
  if (name == "pi")
    return Token( &S_PI, m_charPos );
  else if (name == "e")
    return Token( &S_E, m_charPos );
  else if (name == "ln2")
    return Token( &S_LN2, m_charPos );
  else if (name == "ln10")
    return Token( &S_LN10, m_charPos );
  else if (Function::isNameValid(name)) // Check for a function name
    return Token( Function::nameToID(name), m_charPos );
  else // We don't support anything else right now...
    throw 100;
}

Token Lexer::scanFloat()
{
  float number;
  *m_input >> number;

  return Token( number, m_charPos );
}

float Lexer::S_PI    = M_PI;
float Lexer::S_E     = M_E;
float Lexer::S_LN2   = M_LN2;
float Lexer::S_LN10  = M_LN10;
