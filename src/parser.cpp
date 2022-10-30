#include <iostream>
#include <parser.h>
#include <stack>
#include <vector>
#include <cmath>

Parser::Parser(std::map<std::string, Operation> &operations)
	:operations(operations) { }

Token::Token(Token &&token)
	:type(std::move(token.type)), name(std::move(token.name)) { }

Token::Token(Token const& token)
	:type(token.type), name(token.name) { }

Token::Token(Token::Type &&type, std::string &&name)
	: type(std::move(type)), name(std::move(name)) {  }

Token::Token(Token::Type const &type, std::string const& name)
	:type(type), name(name) {  }

bool Token::operator==(Token const &token)
{
	return token.name == this->name;
}

bool Token::operator==(std::string const& str)
{
	return str == this->name;
}

std::vector<Token> Parser::infixToPostfix(std::string infix)
{
	infix = '(' + infix + ')';
	int l = infix.size();
	std::stack<Token> st;
	std::vector<Token> output;

	for (int i = 0; i < l; i++) 
    {
		while(isspace(infix[i]))
			i++;

		std::string sym = std::string(1, infix[i]);

		if (isdigit(infix[i]))
			output.emplace_back(std::move(GetOperand(infix, i)));

		else if (infix[i] == '(')
		{
			st.push(Token(Token::Type::Bracket, "("));
			while (st.top() != "(")
			{
				output.push_back(st.top());
				st.pop();
			}
			st.pop();
		}

		else if (operations.contains(sym))
		{
			auto curOp = operations.find(sym)->second;
			while (!st.empty())
			{
				Operation topOp = operations.find(st.top().name)->second;
				if (curOp.priority == topOp.priority)
				{
					if (curOp.associativity == Associativity::Left)
					{
						output.push_back(st.top());
						st.pop();
					}
					else
					{
						st.push(Token(Token::Type::Operation, curOp.symbol));
					}
				}
				else if (curOp.priority < topOp.priority)
				{
					output.push_back(st.top());
					st.pop();
				}
				else break;
			}

			st.push(Token(Token::Type::Operation, curOp.symbol));
		}
		else if (isalpha(infix[i]))
		{
			auto token = GetFunction(infix, i);
			if (token.has_value())
			{
				st.push(token.value());
			}

			while (isspace(infix[i]))
			{
				i++;
			}
			if (infix[i] == '(')
			{
				Token operand = GetOperand(infix, i);

				while (isspace(infix[i]))
				{
					i++;
				}

				if (infix[i] == ')')
				{
					i++;
				}
			}
		}
	}

	while(!st.empty())
	{
		output.push_back(st.top());
		st.pop();
	}
	return output;
}


Token Parser::GetOperand(std::string const& str, int &index)
{
	std::string operand;
	while (isspace(str[index]))
		index++;

	while (isdigit(str[index]))
		operand += str[index++];

	index--;
	return Token(Token::Type::Operand, operand);
}

std::optional<Token> Parser::GetFunction(std::string const& str, int& index)
{
	std::shared_ptr<PluginManager> pluginManager;
	pluginManager = pluginManager->Instance();

	std::string func_name;
	while (isalpha(str[index]))
	{
		func_name += str[index];
		index++;
	}

	if (pluginManager->ContainsBinary(func_name))
	{
		return Token(Token::BinaryFunction, func_name);
	}
	else if (pluginManager->ContainsUnary(func_name))
	{
		return Token(Token::UnaryFunction, func_name);
	}

	try
	{
		pluginManager->LoadUnaryFunc(func_name);
		return Token(Token::Type::UnaryFunction, func_name);
	}
	catch(const std::exception& e)
	{
		try
		{
			pluginManager->LoadBinaryFunction(func_name);
			return Token(Token::Type::BinaryFunction, func_name);
		}
		catch(const std::exception& e)
		{
			std::cerr << '\n' << e.what() << '\n';
			return {  };
		}
	}
}