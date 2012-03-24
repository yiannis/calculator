CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( Calculator )

SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )

SET( PARSER_SOURCES
  "Expr.cc"
  "Function.cc"
  "Token.cc"
  "Lexer.cc"
  "Parser.cc")

SET( TEST_H "Test.h" )

# For Qt-Test
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtTest REQUIRED)
INCLUDE(${QT_USE_FILE})
qt4_wrap_cpp( TEST_MOC_SOURCES ${TEST_H} )

##################TARGETS##################
#calculator#
ADD_EXECUTABLE( calculator "calculator.cc" ${PARSER_SOURCES} )

#test#
ADD_EXECUTABLE( test "Test.cc" ${PARSER_SOURCES} ${TEST_MOC_SOURCES} )
TARGET_LINK_LIBRARIES( test ${QT_LIBRARIES} )
