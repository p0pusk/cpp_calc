#include "calculator.h"


Calculator::Calculator()
{
	operations.emplace("+", Operation("+", 1, [](double x, double y) { return x + y; }));
	operations.emplace("-", Operation("-", 1, [](double x, double y) { return x - y; }));
	operations.emplace("*", Operation("*", 2, [](double x, double y) { return x * y; }));
	operations.emplace("/", Operation("/", 2, [](double x, double y) { return x / y; }));
	operations.emplace("^", Operation("^", 3, [](double x, double y) { return pow(x, y); }));

    pm = pm->Instance();
}

Calculator::~Calculator()
{
    pm->CloseLibs();
}

double Calculator::Calculate(std::string input)
{
    Parser p(operations);
    std::vector<Token> expr = p.infixToPostfix(input);
    double result = EvalPostfix(expr);
    return result;
}

double Calculator::EvalPostfix(std::vector<Token> expr)
{
    std::stack<double> operands;

    for (auto &token : expr)
    {
        if (token.type == Token::Type::Operand)
        {
            operands.push(stod(token.name));
        }
        else if (token.type == Token::Type::Operation)
        {
            double x = operands.top();
            operands.pop();
            double y = operands.top();
            operands.pop();

            auto op = operations.find(token.name)->second;
            operands.emplace(op.function(y, x));
        }
        else if (token.type == Token::Type::BinaryFunction)
        {
            if (pm->ContainsBinary(token.name))
            {
                auto func = pm->GetBinaryFunction(token.name);
                double x = operands.top();
                operands.pop();
                double y = operands.top();
                operands.pop();
                operands.push(func(x, y));
            }
            else
            {
                throw std::runtime_error("Unknown function name");
            }
        }
        else if (token.type == Token::Type::UnaryFunction)
        {
            if (pm->ContainsUnary(token.name))
            {
                auto func = pm->GetUnaryFunction(token.name);
                double x = operands.top();
                operands.pop();
                operands.push(func(x));
            }
            else
            {
                throw std::runtime_error("Unknown function name");
            }
        }
    }

    if (operands.size() != 1)
    {
        throw std::runtime_error("Invalid input expression");
    }

    return operands.top();
}