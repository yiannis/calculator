CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( Calculator )

SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )

SET( PARSER_SOURCES
  "Expr.cc"
  "Function.cc"
  "Lexer.cc"
  "Parser.cc")

##################TARGETS##################

#calculator##################
ADD_EXECUTABLE( calculator "calculator.cc" ${PARSER_SOURCES} )
#TARGET_LINK_LIBRARIES( mytasks ${QT_LIBRARIES} )
