#include<iostream>
#include<cstring>
#include<map>
#include<vector>
#include<cmath>
#include<algorithm>
#include<cctype>

using namespace std;

string table_name = "STORE";


vector<string> get_words (string expression){

    vector<string> words;

    string temp = "";
    bool is_alpha = false;
    for(int i =0;i<expression.size();i++){

        if(expression[i] != ' '){
            temp += expression[i];
            is_alpha = true;
        }
        else{
            if(is_alpha == true){
                words.push_back(temp);
                temp = "";
                is_alpha = false;
            }
            else{
                continue;
            }
        }
    }
    if(temp != ""){
        words.push_back(temp);
    }
    return words;

}


bool is_relation_operator(string expression){

    if((expression == "=") || (expression == "<") || (expression == "!=") || (expression == ">") || (expression == ">=")
    || (expression == "<=")){
        return true;
    }
    else{
        return false;
    }
}

bool is_logical_operator(string expression){

    if((expression == "AND") || (expression == "OR") || (expression == "NOT")){
        return true;
    }
    else{
        return false;
    }
}


bool is_alpha_string(string expression){

    transform(expression.begin(), expression.end(), expression.begin(), ::tolower);

    if((expression[0] == '"') && (expression[expression.size()-1] == '"')){
        if(expression.size() > 2){
            for(int i = 1;i<expression.size()-1;i++){
                char temp = expression[i];
                if((temp >='a') && (temp <= 'z')){
                    continue;
                }
                return false;
            }
            return true;
        }
        else{
            return false;
        }
    }
    else{
        for(int i = 0;i<expression.size();i++){
            char temp = expression[i];
            if((temp >='a') && (temp <= 'z')){
                continue;
            }
            return false;
        }
        return true;
    }

}

bool is_number (string expression){

    for(int i =0;i<expression.size();i++){
        int temp = expression[i];
        if((temp >= '0') && (temp <='9')){
            continue;
        }
        else{
            return false;
        }
    }
    return true;
}


bool parse_column(string expression, vector<string> initial_environment,string& parsed_string){

    vector<string> cols = get_words(expression);
    for(int i = 0;i<cols.size();i++){
        bool found = false;
        for(int j = 0; j<initial_environment.size();j++){
            if(cols[i] == initial_environment[j]){
                found = true;
                string curr = "";
                curr += ("( id " + (cols[i]) + " )");
                parsed_string += curr;
                break;
            }
        }
        if(found == false){
            return false;
        }
    }

    return true;

}



bool parse_expression(string expression, vector<string> initial_environment,
map<string,int> extended_environment, string& parsed_string, vector<int>& check_statements){

    vector<string> words = get_words(expression);
    int number_words = words.size();
    //cout<<"Number of Words : "<<number_words<<endl;
    cout<<endl;
    cout<<"Parsing . . . "<<expression<<" "<<endl<<endl;
    if(number_words == 1){
        if(is_alpha_string(expression) == true){
            string temp_10 = "";
            if(parse_column(expression,initial_environment,temp_10) == true){
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                string curr = "";
                curr += ("( id " + (expression) + " )");
                parsed_string += curr;
                check_statements.push_back(1);
                return true;
            }
            else if(( is_logical_operator(expression) == true ) && (extended_environment[expression] == -1)){
                string curr = "";
                curr += ("( logicalop " + (expression) + " )");
                parsed_string += curr;
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                check_statements.push_back(2);
                return true;
            }
            else if(extended_environment[expression] == -1){
                string curr = "";
                curr += ("( string " + (expression) + " )");
                parsed_string += curr;
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                check_statements.push_back(3);
                return true;
            }
            else{
                cout<<"Invalid Expression : Column not present"<<endl;
                return false;
            }
        }
        else{
            if(is_relation_operator (expression) == true){
                if(extended_environment[expression] == 0){
                     cout<<"Invalid Expression : Not a valid Column Name"<<endl;

                     return false;
                }
                else{
                    string curr = "";
                    curr += ("( relationalop " + (expression) + " )");
                    parsed_string += curr;
                    cout<<"Parsed successfully : "<<expression<<endl<<endl;
                    check_statements.push_back(0);
                    return true;
                }
            }
            else if (is_number(expression) == true){
                if(extended_environment[expression] == 0){
                     cout<<"Invalid Expression : Not a valid Column Name"<<endl;
                     return false;
                }
                else{
                    string curr = "";
                    curr += ("( num " + (expression) + " )");
                    parsed_string += curr;
                    cout<<"Parsed successfully : "<<expression<<endl<<endl;
                    check_statements.push_back(4);
                    return true;
                }
            }
            else{
                cout<<"Invalid Expression : Not a valid Column Name"<<endl;
                return false;
            }

        }
    }
    else{

        vector<string> temp_words;
        vector<string> parenthesis_stack;
            bool is_started = false;
            for(int i = 0;i<words.size();i++){

                if(is_logical_operator(words[i]) == true){
                    if(words[i-1] == ")" && (((i+1) < words.size()) && words[i+1] == "(")){
                        extended_environment[words[i]] = -1;
                        bool flag = parse_expression(words[i],initial_environment,extended_environment,parsed_string,check_statements);
                        if(flag == false){
                            return false;
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        cout<<"Invalid Expression";
                        false;
                    }
                }

                if(words[i] == "("){
                    parenthesis_stack.push_back("(");
                    parsed_string += "(";
                    is_started = true;
                }
                else if( (parenthesis_stack.size() > 0) && (words[i] != "(") && (words[i] != ")")){
                     temp_words.push_back(words[i]);
                }
                else if(words[i] == ")"){
                    if(parenthesis_stack.size() > 0){

                        for(int j = 0;j<temp_words.size();j++){

                            if(is_logical_operator(temp_words[j]) == true){
                                cout<<"Invalid Expression";
                                return false;
                            }

                            extended_environment[temp_words[j]] = -1;
                            bool flag = parse_expression(temp_words[j],initial_environment,extended_environment,parsed_string,check_statements);
                            if(flag == false){
                                return false;
                            }
                        }
                        parsed_string += ")";
                        parenthesis_stack.pop_back();
                        temp_words.clear();
                    }
                    else{
                        cout<<"Invalid Expression : Check parenthesis";
                        return false;
                    }
                }
            }

            if((parenthesis_stack.size() > 0) || (is_started == false)){
                cout<<"Invalid Expression : Check parenthesis";
                return false;
            }
            return true;
    }
}


string interpret_expression(string expression, vector<string> commands){

    string sql_statement = "";
    if(expression.size() == 0){
        return sql_statement;
    }
    else{
       sql_statement = commands[0] + " ";
       vector<string> words = get_words(expression);
       for(int i = 0;i<words.size()-1;i++){
            sql_statement += words[i] + ",";
       }
       sql_statement += words[words.size()-1] + " ";
       sql_statement += commands[1] + " " +table_name;
    }
    return sql_statement;
}

string interpret_condition(string expression, vector<string> commands){


    string sql_statement = "";

    if(expression.size() == 0){
        return sql_statement;
    }
    else{
        sql_statement = commands[2] + " " + expression;
    }
    return sql_statement;
}

bool check_valid_condition(vector<int> values){

    // 0 -> relationalop
    // 1 -> col
    // 2 -> logicalop
    // 3 -> string
    // 4 -> Num

    bool is_found = false;

    vector<int> start_values;
    vector<int> end_values;

    for(int i = 0;i<values.size();i++){
        if(((i==0) && (i == values.size()-1)) && ((values[i] == 0) || (values[i] == 2))){
            return false;
        }
        else{
            if(values[i] == 0){
                if((values[i-1] != 1) || (values[i+1] == 0) || (values[i+1] == 2) || (values[i+1] == 1)){
                    return false;
                }
                else{
                    is_found = true;
                    start_values.push_back(i-1);
                    end_values.push_back(i+1);
                    i++;
                }
            }
        }
    }

    for(int i = 0;i<values.size();i++){
        if(values[i] == 2){
            int start_key = i+1;
            int end_key = i-1;

            bool found = false;
            for(int j=0;j<start_values.size();j++){
                if(start_key == start_values[j]){
                    found = true;
                    break;
                }
            }

            if(found == false){
                return false;
            }
            else{
                found = false;
                for(int j = 0;j<end_values.size();j++){
                    if(end_key == end_values[j]){
                        found = true;
                        break;
                    }
                }
                if(found == false){
                    return false;
                }
                is_found = true;
            }
        }
    }

    if(is_found == false){
        return false;
    }
    else{
        return true;
    }

}



int main(){

    vector<string> commands;
    commands.push_back("SELECT");
    commands.push_back("FROM");
    commands.push_back("WHERE");

    vector<string> initial_environment;
    initial_environment.push_back("ItemName");
    initial_environment.push_back("ItemCost");
    initial_environment.push_back("ItemQuantity");
    initial_environment.push_back("ItemColor");

    map<string,int> extended_environment;

    cout<<"Welcome to DSL for SQL Statement-Select on the table : "<<table_name<<endl<<endl;
    cout<<"The columns present in the table are : ";

    for(int i =0; i<initial_environment.size();i++){
        cout<<initial_environment[i]<<" ";
    }
    cout<<endl<<endl;

    cout<<"Press q to quit"<<endl;

     while(1){
        cout<<endl;
        cout<<"Column:> ";
        string expression;
        getline(cin, expression);
        cout<<endl;
        if(expression == "q"){
            cout<<"Quitting . ."<<endl;
            break;
        }
        //cout<<expression<<endl;
        string parsed_string_expression = "";
        bool flag = parse_column(expression,initial_environment,parsed_string_expression);
        if(flag == true){
            cout<<"Parsed Successfully : "<<parsed_string_expression<<endl;
            vector<string> temp_conditions;
            bool is_and = false;
            bool is_or = false;
            cout<<"Commands : "<<endl;
            cout<<"D: Done"<<endl;
            cout<<endl;
            cout<<"Cond:> ";
            string condition_expression;

            getline(cin, condition_expression);

            if(condition_expression == "D"){
                string statement = interpret_expression(expression,commands);
                statement += ";";
                cout<<"Ouput:> "<<statement<<endl<<endl;
            }
            else{
                vector<int> check_statements;
                temp_conditions.push_back(condition_expression);
                string parsed_string ="";
                bool flag = parse_expression(condition_expression,initial_environment,extended_environment,parsed_string,check_statements);
                if(flag == true){
                    bool flag  = check_valid_condition (check_statements);
                    if(flag == true){
                        cout<<parsed_string<<endl<<endl;
                        string statement_conditon = interpret_condition(condition_expression,commands);
                        string statement = interpret_expression(expression,commands) + " " +statement_conditon + ";";
                        cout<<"Ouput:> "<<statement<<endl<<endl;
                    }
                    else{
                        cout<<"Error: Invalid Condition"<<endl;
                    }

                }
            }
            cout<<endl;
        }
        else{
            cout<<"Error: Invalid Column"<<endl;
        }




    }




return 0;
}

