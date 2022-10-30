#include "calculator.h"

Calculator::Calculator()
{
	operations.emplace("+", Operation("+", 1, Associativity::Left, [](double x, double y)
								 { return x + y; }));
	operations.emplace("-", Operation("-", 1, Associativity::Left, [](double x, double y)
								 { return x - y; }));
	operations.emplace("*", Operation("*", 2, Associativity::Left, [](double x, double y)
								 { return x * y; }));
	operations.emplace("/", Operation("/", 2, Associativity::Left, [](double x, double y)
								 { return x / y; }));
	operations.emplace("^", Operation("^", 3, Associativity::Right, [](double x, double y)
								 { return pow(x, y); }));

    pm = pm->Instance();
}

double Calculator::Calculate(std::string input)
{
    Parser p(operations);
    std::vector<Token> expr = p.infixToPostfix(input);

    for (auto t : expr)
    {
        std::cout << t.name << ' ';
    }
    std::cout << std::endl;
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

    return operands.top();
}