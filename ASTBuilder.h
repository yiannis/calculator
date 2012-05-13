#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include <iostream>
#include <cassert>

#include "Visitor.h"
#include "Parser.h"
#include "Lexer.h"

class ASTBuilder {
  public:
    ASTBuilder(std::istream *input) :
      m_lexer(new Lexer(input)), m_parser(NULL)
    {}

    ~ASTBuilder()
    {
      delete m_parser;
      delete m_lexer;
    }

    /// Set a user defined constant
    ASTBuilder& set( const std::string& name, float value )
    {
      m_constants[name] = value;
      m_lexer->pushConstant( name, &m_constants[name] );

      return *this;
    }
    /// Set a user defined constant (overloaded method)
    ASTBuilder& set( const char *name, float value )
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
    /// Run the visitor on the AST
    ASTdata visitorResult( ASTVisitor *visitor )
    {
      if (!m_parser)
        m_parser = new Parser(m_lexer);
      assert( m_parser != NULL );

      return m_parser->AST()->accept(visitor);
    }
    /// Show constants
    const std::map<std::string,float>& getConstantsTable() const { return m_constants; }
    /// Print for debugging
    void printConstantsTable() const
    {
      for (IC i=m_constants.begin(); i!=m_constants.end(); i++)
        std::cout << i->first << ": " << i->second << "@" << &i->second << std::endl;
      m_lexer->print();
    }

  protected:
    typedef std::map<std::string,float>::const_iterator IC;

  private:

    Lexer *m_lexer;
    Parser *m_parser;
    std::map<std::string,float> m_constants;
};

#endif // AST_BUILDER_H 
