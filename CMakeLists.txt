CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( Calculator )

SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )

SET( PARSER_SOURCES
  "Expr.cc"
  "Function.cc"
  "Token.cc"
  "Lexer.cc"
  "Parser.cc"
  "Visitor.cc")

SET( TEST_H "Test.h" )

# For Qt-Test
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtTest)
INCLUDE(${QT_USE_FILE})
qt4_wrap_cpp( TEST_MOC_SOURCES ${TEST_H} )

##################TARGETS##################
#calculator#
ADD_LIBRARY( calc SHARED ${PARSER_SOURCES} )
ADD_EXECUTABLE( calculator "calculator.cc" )
TARGET_LINK_LIBRARIES( calculator "calc" )

#test#
IF(QT4_FOUND)
  ADD_EXECUTABLE( test "Test.cc" ${TEST_MOC_SOURCES} )
  TARGET_LINK_LIBRARIES( test "calc" ${QT_LIBRARIES} )
ENDIF(QT4_FOUND)
