#pragma once
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "operations.h"
#include "plugin_manager.h"

class Token {
 public:
  enum Type { Function, Operation, Operand, Bracket };

  const Token::Type type_;
  const std::string name_;

  Token(Token&& token);
  Token(Token const& token);
  Token(Token::Type&& type, std::string&& name);
  Token(Token::Type const& type, std::string const& name);
  bool operator==(Token const& token);
  bool operator==(std::string const& str);
};

class Parser {
 private:
  std::map<std::string, Operation>& operations_;
  Token GetOperand(std::string const& str, int& index);
  std::optional<Token> GetFunction(std::string const& str, int& index);
  int GetPriority(Token const& token);

 public:
  Parser(std::map<std::string, Operation>& operations);
  std::vector<Token> InfixToPostfix(std::string& infix);
};
