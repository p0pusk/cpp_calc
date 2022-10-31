#include <parser.h>

Parser::Parser(std::map<std::string, Operation>& operations)
    : operations_(operations) {}

Token::Token(Token&& token)
    : type_(std::move(token.type_)), name_(std::move(token.name_)) {}

Token::Token(Token const& token) : type_(token.type_), name_(token.name_) {}

Token::Token(Token::Type&& type, std::string&& name)
    : type_(std::move(type)), name_(std::move(name)) {}

Token::Token(Token::Type const& type, std::string const& name)
    : type_(type), name_(name) {}

bool Token::operator==(Token const& token) { return token.name_ == this->name_; }

bool Token::operator==(std::string const& str) { return str == this->name_; }

std::vector<Token> Parser::InfixToPostfix(std::string& infix) {
  infix = '(' + infix + ')';
  int l = infix.size();
  std::stack<Token> st;
  std::vector<Token> output;

  for (int i = 0; i < l; i++) {
    while (isspace(infix[i])) i++;

    std::string sym = std::string(1, infix[i]);

    if (isdigit(infix[i])) {
      output.emplace_back(std::move(GetOperand(infix, i)));
    }

    else if (infix[i] == '(') {
      st.push(Token(Token::Bracket, "("));
    }

    else if (infix[i] == ')') {
      while (st.top() != "(") {
        output.push_back(st.top());
        st.pop();
      }
      st.pop();
    }

    else if (operations_.contains(sym)) {
      Token token(Token::Operation, sym);
      while (!st.empty()) {
        if (GetPriority(token) <= GetPriority(st.top())) {
          output.push_back(st.top());
          st.pop();
        } else
          break;
      }
      st.push(Token(Token::Operation, sym));
    }

    else if (isalpha(infix[i])) {
      auto token = GetFunction(infix, i);
      if (token.has_value()) {
        st.push(token.value());
      }
    }
  }

  while (!st.empty()) {
    output.push_back(st.top());
    st.pop();
  }
  return output;
}

Token Parser::GetOperand(std::string const& str, int& index) {
  std::string operand;
  while (isspace(str[index])) index++;

  while (isdigit(str[index])) operand += str[index++];

  index--;
  return Token(Token::Operand, operand);
}

std::optional<Token> Parser::GetFunction(std::string const& str, int& index) {
  std::shared_ptr<PluginManager> pm;
  pm = pm->Instance();

  std::string func_name;
  while (isalpha(str[index])) {
    func_name += str[index++];
  }
  index--;

  if (pm->ContainsFunction(func_name)) {
    return Token(Token::Function, func_name);
  }

  try {
    pm->LoadFunction(func_name);
    return Token(Token::Function, func_name);
  } catch (const std::exception& e) {
    std::cerr << '\n' << e.what() << '\n';
    return {};
  }
}

int Parser::GetPriority(Token const& token) {
  if (token.type_ == Token::Operation) {
    auto search = operations_.find(token.name_);
    if (search != operations_.end()) {
      return search->second.priority_;
    }
    return -1;
  }

  else if (token.type_ == Token::Function)
    return 4;
  else
    return -1;
}