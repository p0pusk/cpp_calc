#pragma once
#include <string>
#include <stack>
#include <vector>
#include <map>
#include "operations.h"
#include "pluginManager.h"


class Token
{
  public:
    enum Type
    {
      UnaryFunction,
      BinaryFunction,
      Operation,
      Operand,
      Bracket
    };

    Token::Type type;
    std::string name;

    Token(Token &&token);
    Token(Token const &token);
    Token(Token::Type &&type, std::string &&name);
    Token(Token::Type const &type, std::string const &name);
    bool operator==(Token const &token);
    bool operator==(std::string const &str);
};

class Parser
{
  private:
    std::map<std::string, Operation> &operations;
    Token GetOperand(std::string const &str, int &index);
    std::optional<Token> GetFunction(std::string const &str, int &index);
    int GetPriority(Token const &token);

  public:
    Parser(std::map<std::string, Operation> &operations);
    std::vector<Token> infixToPostfix(std::string &infix);
};
