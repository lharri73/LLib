#include <cstring>
#include <exception>
#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>
#include <tgmath.h>


class Val{
    public:
        Val(const std::string &input);

        double evaluate();

    private:
        std::vector<double> values;
        std::vector<char> operators;
};


int main(int argc, char **argv){
    std::string input_str;
    std::stringstream stream_in;
    stream_in.str();
    if(argc == 1){
        while(std::getline(std::cin, input_str)){
            stream_in << input_str;
        }
        input_str = stream_in.str();
    }else{
        for(int i=1; i < argc; i++){
            stream_in << argv[i];
        }
        input_str = stream_in.str();
    }
    Val result(input_str);

    double num_result = result.evaluate();
    std::cout << num_result << std::endl;

    return 0;
}

bool is_operator(char test){
    switch(test){
        case '+':
        case '-':
        case '/':
        case '*':
        case 'x':
        case '^':
            return true;
            break;
        default:
            return false;
    }
}

int find_close_paren(const std::string &input){
    if(input[0] != '(' && input[0] != '['){
        fprintf(stderr, "Expression '%s' does not begin with parens\n", input.c_str());
        exit(1);
    }

    size_t num_parens=1;
    char search = input[0];
    char close_search = search == '(' ? ')' : ']';
    for(size_t i = 1; i < input.size(); i++){
        if(input[i] == search) num_parens++;
        if(input[i] == (close_search)) num_parens--;

        if(num_parens == 0)
            return i;
    }
    return -search;

}

double get_inside_parens(const std::string &input, size_t i){
    char tmp[input.size() - i + 1];
    std::strncpy(tmp, input.c_str() +i, input.size() - i+1);
    std::string tmp_str(tmp);

    int close_paren = find_close_paren(tmp_str);
    if(close_paren < 0){
        fprintf(stderr, "Unmatched '%c'\n", -close_paren);
        exit(1);
    }
    
    memset(tmp, '\0', input.size() - i + 1);
    strncpy(tmp, tmp_str.c_str()+1, close_paren-1);
    tmp_str = tmp;
    Val left(tmp_str);

    return left.evaluate();
}

double parse_val(std::stringstream &buf){
    double read;
    buf >> read;
    buf.str("");
    buf.clear();
    return read;
}

Val::Val(const std::string &input){
    std::stringstream buf;
    buf.str("");
    size_t num_read = 0;
    for(size_t i=0; i < input.size(); i++){
        if(input[i] == ' ') continue;
        if(is_operator(input[i])){
            operators.push_back(input[i]);
            if(num_read != 0){
                values.push_back(parse_val(buf));
                num_read = 0;
            }
            continue;
        }
        if(input[i] == '(' || input[i] == '['){
            double next = get_inside_parens(input, i);
            values.push_back(next);
            //std::string tmp = input.c_str() + i;
            i += find_close_paren(input.c_str()+i);
            if(num_read != 0){
                values.push_back(parse_val(buf));
                num_read = 0;
            }
            continue;
        }else{
            buf << input[i];
            num_read++;
        }
    }

    if(num_read != 0){
        double prev_read;
        buf >> prev_read;
        num_read = 0;
        values.push_back(prev_read);
    }

}

double Val::evaluate(){
    if(values.size() != operators.size()+1){
        fprintf(stderr, "operators require left and right operands\n");
        exit(1);
    }
    for(size_t i = 0; i < operators.size(); i++){
        if(operators[i] == '^'){
            values[i] = pow(values[i], values[i+1]);
            values.erase(values.begin()+i+1);
            operators.erase(operators.begin()+i);
            i--;
        }
    }
    for(size_t i = 0; i < operators.size(); i++){
        if(operators[i] == '*' || operators[i] == 'x'){
            values[i] = values[i] * values[i+1];
            values.erase(values.begin()+i+1);
            operators.erase(operators.begin()+i);
            i--;
            continue;
        }
        if(operators[i] == '/'){
            values[i] = values[i] / values[i+1];
            values.erase(values.begin()+i+1);
            operators.erase(operators.begin()+i);
            i--;
        }
    }
    for(size_t i = 0; i < operators.size(); i++){
        if(operators[i] == '+'){
            values[i] = values[i] + values[i+1];
            values.erase(values.begin()+i+1);
            operators.erase(operators.begin()+i);
            i--;
            continue;
        }
        if(operators[i] == '-'){
            values[i] = values[i] - values[i+1];
            values.erase(values.begin()+i+1);
            operators.erase(operators.begin()+i);
            i--;
        }
    }
    return values[0];
}
