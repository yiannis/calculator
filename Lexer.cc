#include <iostream>
#include <string>
#include <cctype>

#include "Lexer.h"

using namespace std;

void Lexer::printTokens(list<Token>* tokens)
{
  list<Token>::const_iterator i = tokens->begin();
  list<Token>::const_iterator end = tokens->end();

  while (i != end) {
    switch (i->type()) {
      case VARX:
        cout << " x " << flush;
        break;
      case VARY:
        cout << " y " << flush;
        break;
      case FLOAT:
        cout << " " << i->expr()->result() << " " << flush;
        break;
      case PLUS:
        cout << " + " << flush;
        break;
      case MINUS:
      case NEGATIVE:
        cout << " - " << flush;
        break;
      case MULT:
        cout << " * " << flush;
        break;
      case DIV:
        cout << " / " << flush;
        break;
      case POWER:
        cout << " ^ " << flush;
        break;
      case LPAREN:
        cout << " ( " << flush;
        break;
      case RPAREN:
        cout << " ) " << flush;
        break;
      case COMMA:
        cout << " , " << flush;
        break;
      case EXPR:
        cout << " <expr> " << flush;
        break;
      case FUNCTION:
        cout << static_cast<ExprFunction*>(i->expr())->name() << flush;
        break;
      default:
        cerr << __func__ << "(): " << __LINE__ << ": Unknown token '" << i->type() << "'" << endl;
    }
    i++;
  }
  cout << endl;
}

void Lexer::scan(istream& input)
{
  while (input.good()) {
    int in = input.get();

    if (isalpha(in)) {
      input.unget();
      scanString(input);
    } else if (isdigit(in)) {
      input.unget();
      scanFloat(input);
    } else if (isspace(in)) {
      continue;
    } else {
      switch (in) {
        case '+':
          m_tokens.push_back(Token(PLUS, new ExprBinOp(PLUS)));
          break;
        case '-':
          if (m_tokens.empty() ||
              m_tokens.back().type() == LPAREN ||
              m_tokens.back().type() == COMMA)
            m_tokens.push_back(Token(NEGATIVE, new ExprUnaryOp(NEGATIVE)));
          else
            m_tokens.push_back(Token(MINUS, new ExprBinOp(MINUS)));
          break;
        case '*':
          m_tokens.push_back(Token(MULT, new ExprBinOp(MULT)));
          break;
        case '/':
          m_tokens.push_back(Token(DIV, new ExprBinOp(DIV)));
          break;
        case '^':
          m_tokens.push_back(Token(POWER, new ExprBinOp(POWER)));
          break;
        case '(':
          m_tokens.push_back(Token(LPAREN));
          break;
        case ')':
          m_tokens.push_back(Token(RPAREN));
          break;
        case ',':
          m_tokens.push_back(Token(COMMA));
          break;
        default:
          cerr << __func__ << "(): " << __LINE__ << ": Unknown input character '" << (char)in << "'" << endl;
      }
    }
  }
}

void Lexer::scanString(istream& input)
{
  int in;
  string name;
  while (input.good()) {
    in = input.get();

    if (isalnum(in)) {
      name += (char)in;
    } else {
      input.unget();
      break;
    }
  }

  if (name == "x")
    m_tokens.push_back(Token(VARX, new ExprVariable(m_x)));
  else if (name == "y")
    m_tokens.push_back(Token(VARY, new ExprVariable(m_y)));
  else
    m_tokens.push_back(Token(FUNCTION, new ExprFunction(name)));
}

void Lexer::scanFloat(istream& input)
{
  float number;
  input >> number;
  m_tokens.push_back(Token(FLOAT, new ExprLiteral(number)));
}
