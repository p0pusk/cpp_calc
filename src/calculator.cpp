#include "calculator.h"

Calculator::Calculator() {
  operations_.emplace(
      "+", Operation("+", 1, [](double x, double y) { return x + y; }));
  operations_.emplace(
      "-", Operation("-", 1, [](double x, double y) { return x - y; }));
  operations_.emplace(
      "*", Operation("*", 2, [](double x, double y) { return x * y; }));
  operations_.emplace(
      "/", Operation("/", 2, [](double x, double y) { return x / y; }));
  operations_.emplace(
      "^", Operation("^", 3, [](double x, double y) { return pow(x, y); }));

  pm_ = pm_->Instance();
}

Calculator::~Calculator() { pm_->CloseLibs(); }

double Calculator::Calculate(std::string& input) {
  Parser p(operations_);
  std::vector<Token> expr = p.infixToPostfix(input);
  double result = EvalPostfix(expr);
  return result;
}

double Calculator::EvalPostfix(std::vector<Token>& expr) {
  std::stack<double> operands;

  for (auto& token : expr) {
    if (token.type_ == Token::Type::Operand) {
      operands.emplace(stod(token.name_));
    } else if (token.type_ == Token::Type::Operation) {
      double x = operands.top();
      operands.pop();
      double y = operands.top();
      operands.pop();

      auto op = operations_.find(token.name_)->second;
      operands.emplace(op.function(y, x));
    } else if (token.type_ == Token::Type::BinaryFunction) {
      if (pm_->ContainsBinary(token.name_)) {
        auto func = pm_->GetBinaryFunction(token.name_);
        double x = operands.top();
        operands.pop();
        double y = operands.top();
        operands.pop();
        operands.emplace(func(x, y));
      }
    } else if (token.type_ == Token::Type::UnaryFunction) {
      if (pm_->ContainsUnary(token.name_)) {
        auto func = pm_->GetUnaryFunction(token.name_);
        double x = operands.top();
        operands.pop();
        operands.emplace(func(x));
      }
    }
  }

  if (operands.size() != 1) {
    throw std::runtime_error("Invalid input expression");
  }

  return operands.top();
}