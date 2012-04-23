#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>

#include "Visitor.h"
#include "Parser.h"
#include "Lexer.h"

class Interpreter {
  public:
    Interpreter(std::istream *input) :
      m_ready(false), m_lexer(new Lexer(input)), m_parser(NULL)
    {}

    ~Interpreter()
    {
      delete m_parser;
      delete m_lexer;
    }

    /// Set a user defined constant
    Interpreter& set( const std::string& name, float value )
    {
      m_constants[name] = value;
      m_lexer->pushConstant( name, &m_constants[name] );

      return *this;
    }
    /// Set a user defined constant (overloaded method)
    Interpreter& set( const char *name, float value )
    {
      this->set( std::string(name), value );
      return *this;
    }
    /// Get/Set the value of a constant (overloaded method)
    float& operator[]( const std::string& name )
    {
      if (m_constants.find(name) == m_constants.end())
        this->set( name, 0.0F );

      return m_constants[name];
    }
    /// Parse the input and create the AST
    bool parse()
    {
      m_parser = new Parser(m_lexer);
      if (m_parser->AST() != NULL)
        m_ready = true;
      else
        m_ready = false;
      return m_ready;
    }
    /// Execute the code on the AST
    float result()
    {
      if (m_ready)
        return m_parser->AST()->accept(&m_executor).f;
      else
        return 0;
    }
    /// Show constants
    const std::map<std::string,float>& getConstantsTable() const { return m_constants; }
    /// Print for debugging
    void print() const
    {
      for (I i=m_constants.begin(); i!=m_constants.end(); i++)
        std::cout << i->first << ": " << i->second << "@" << &i->second << std::endl;
    }

  private:
    typedef std::map<std::string,float>::const_iterator I;

    bool m_ready;
    Lexer *m_lexer;
    Parser *m_parser;
    std::map<std::string,float> m_constants;
    ASTVisitorExecutor m_executor;
};

#endif // INTERPRETER_H 
