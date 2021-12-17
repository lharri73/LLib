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

class Val;
class Val{
    public:
        Val(const std::string &input);
        Val(float im);
        ~Val();

        bool immediate;
        float ival;
        Val* left;
        Val* right;
        char op;

        std::string copy;

        float evaluate();
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

    float num_result = result.evaluate();
    if(int(num_result) == num_result){
        printf("%d\n", (int)num_result);
    }else{
        printf("%f\n", result.evaluate());
    }

    return 0;
}

bool is_operator(char test){
    switch(test){
        case '+':
        case '-':
        case '/':
        case '*':
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

Val::Val(const std::string &input_orig){
    copy = input_orig.c_str();
    std::string input = input_orig;

    while(true){
        if((input[0] == '[' || input[0] == '(') && 
                find_close_paren(input) == input.size()-1){
            // if we have `(expr)` or `[expr]`, then remove the parens
            printf("started and ended with parens: %s\n", input.c_str());
            char tmp[input.size()];
            strncpy(tmp, input.c_str()+1, input.size()-2);
            tmp[input.size()-2] = '\0';
            input = tmp;
        }else{
            break;
        }
    }

    left = nullptr;
    right = nullptr;
    op = 0;

    std::stringstream strBuf;
    size_t num_read = 0;
    strBuf.str();

    for(size_t i = 0; i < input.size(); i++){
        if(input[i] == ' ' || input[i] == '\n') continue;        
        if(is_operator(input[i])){
            if(op != -1 && op != 0){
                fprintf(stderr, "two consecutive operators not allowed\n");
                exit(1);
            }
            if(num_read == 0){
                fprintf(stderr, "operator '%c' expects left operand.\n", input[i]);
                exit(1);
            }
            op = input[i];
            immediate = false;
            continue;
        }
        if(op != 0 && op != -1){
            char tmp[input.size() - i + 1];
            memcpy(tmp, input.c_str() + i, input.size() -i+1);
            std::string tmp_str(tmp);
            right = new Val(tmp_str);
            float tmp_ival;
            strBuf >> tmp_ival;
            left = new Val(tmp_ival);
            break;
        }
        if(input[i] == '(' || input[i] == '['){
            char tmp[input.size() - i + 1];
            std::strncpy(tmp, input.c_str() +i, input.size() - i+1);
            std::string tmp_str(tmp);

            if(left != nullptr){
                right = new Val(tmp_str);
                break;
            }

            int close_paren = find_close_paren(tmp_str);
            if(close_paren < 0){
                fprintf(stderr, "Unmatched '%c'\n", -close_paren);
                exit(1);
            }
            
            memset(tmp, '\0', input.size() - i + 1);
            strncpy(tmp, input.c_str()+1, close_paren-1);
            tmp_str = tmp;
            left = new Val(tmp_str);

            i += close_paren+1; // skip over close paren

            op = -1;
        }
        if(op != 0 && left != nullptr){
            char tmp[input.size() - i + 1];
            std::strncpy(tmp, input.c_str() +i, input.size() - i+1);
            std::string tmp_str(tmp);

            if(left != nullptr){
                right = new Val(tmp_str);
                break;
            }
        }
        strBuf << input[i];
        num_read++;
    }
    if(op == 0){
        immediate = true;
        strBuf >> ival;
    }
}

Val::~Val(){
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

Val::Val(float im){
    copy = "immediate";
    immediate = true;
    ival = im;
}

float Val::evaluate(){
    float retVal;
    if(immediate){
        retVal = ival;
    }else{
        switch(op){
            case '+':
                retVal = left->evaluate() + right->evaluate();
                break;
            case '-': 
                retVal = left->evaluate() - right->evaluate();
                break;
            case '*': 
            case -1:
                retVal = left->evaluate() * right->evaluate();
                break;
            case '/': 
                retVal = left->evaluate() / right->evaluate();
                break;
            case '^': 
                retVal = pow(left->evaluate(), right->evaluate());
                break;
            default:
                fprintf(stderr, "operation %c(%x) not permitted\n", op, op);
                exit(-1);
        }
    }
    printf("retVal: (%s) %f\n", copy.c_str(), retVal);
    return retVal;
}
