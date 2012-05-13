#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include "Lexer.h"

#include <iostream>
#define DBG std::cerr << __FILE__ ": " << __LINE__ << ": " << __func__ << "()" << std::endl;
using namespace std;


Token Lexer::scan()
{
  get();
  while (m_input->good()) {
    if (isalpha(m_in)) {
      unget();
      return scanString();
    } else if (isdigit(m_in)) {
      unget();
      return scanFloat();
    } else if (isspace(m_in)) {
      get();
      continue;
    } else {
      switch (m_in) {
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
          cerr << "Warning: Unknown charachter '" << (char)m_in
               << "' at position " << m_charPos << endl;
          get();
      }
    }
  }

  return Token(END, m_charPos);
}

Token Lexer::scanString()
{
  string name;
  while (m_input->good()) { //FIXME
    get();

    if (isalnum(m_in)) {
      name += (char)m_in;
    } else {
      unget();
      break;
    }
  }

  // Check if 'name' is a user constant
  if (m_constants.find( name ) != m_constants.end())
    return Token( m_constants[name], m_charPos, name );

  // Check if 'name' is a builtin constant
  if (name == "pi")
    return Token( &S_PI, m_charPos, "pi" );
  else if (name == "e")
    return Token( &S_E, m_charPos, "e" );
  else if (name == "ln2")
    return Token( &S_LN2, m_charPos, "ln2" );
  else if (name == "ln10")
    return Token( &S_LN10, m_charPos, "ln10" );
  else if (Function::isNameValid(name)) // Check for a function name
    return Token( Function::nameToID(name), m_charPos );
  else { // We don't support anything else right now...
    cerr << "Warning: Unknown string '" << name
         << "'.\nReplacing it with a constant of value 0.0F" << endl;
    return Token( &S_0, m_charPos, name );
  }
}

Token Lexer::scanFloat()
{ // X[X]*[.?X[X]*]
  string number;

  // Integral part
  get();
  int pos = m_charPos;
  do {
    if (isdigit(m_in))
      number += (char)m_in;
    else
      break;

    get();
  } while (m_input->good());

  if (m_input->good() && m_in == '.') {
    // separator
    number += '.';

    // Fractional part
    get();
    while (m_input->good()) {
      if (isdigit(m_in))
        number += (char)m_in;
      else
        break;

      get();
    }
  }

  if (m_input->good()) {
    unget();
  }

  return Token( atof( number.c_str() ), m_charPos );
}


float Lexer::S_PI    = M_PI;
float Lexer::S_E     = M_E;
float Lexer::S_LN2   = M_LN2;
float Lexer::S_LN10  = M_LN10;
float Lexer::S_0     = 0.0F;
