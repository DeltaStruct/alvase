#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>

struct arguments {

  std::multiset<std::string> AM; std::vector<std::string> AV;

  void set_arg(int argc, char* argv[]) noexcept {
    for (int i(1);i < argc;++i) AM.emplace(argv[i]),AV.emplace_back(argv[i]); // 実行ファイルの指定は除く
    AM.emplace("--system_arguments_end"),AV.emplace_back("--system_arguments_end");
  }

  bool contains(const std::string& arg) noexcept {
    return AM.find(arg) != AM.end();
  }

} args;

enum class char_type {
  number,   // 数字
  alphabet, // アルファベット
  symbol,   // 記号
  unknown   // ???
};

std::string symbol_list = "!\"#%&'()*+,-./:;<=>?@[\\]^_{|}~";

enum class token_type {
  number,    // 数値
  char_,      // 文字
  string,    // 文字列
  operator_, // 演算子
  variables, // 変数
  unknown    // ???
};

int exit_code = 0; std::string filename; std::vector<std::string> alvs_code;

std::vector<std::pair<token_type,std::string>> lexical_analysis_result;

void lexical_analysis(){
  std::sort(symbol_list.begin(),symbol_list.end());
  for (const std::string& line:alvs_code){
    lexical_analysis_result.emplace_back(token_type::unknown,"");
    for (char c:line){
      char_type ct = char_type::unknown;
      if ('a'<=c&&c<='z'){
        ct = char_type::alphabet;
      } else if ('A'<=c&&c<='Z'){
        ct = char_type::alphabet;
      } else if ('0'<=c&&c<='9'){
        ct = char_type::number;
      } else if (std::binary_search(symbol_list.begin(),symbol_list.end(),c)){
        ct = char_type::symbol;
      } else {
        ct = char_type::unknown;
      }
      decltype(lexical_analysis_result)::value_type& last = lexical_analysis_result.back();
      if (ct==char_type::unknown){
        lexical_analysis_result.emplace_back(token_type::unknown,"");
      } else if (last.first==token_type::string){
        last.second.push_back(c);
      } else if (ct==char_type::symbol&&c=='"'){
        if (last.first==token_type::string) lexical_analysis_result.emplace_back(token_type::unknown,"");
        else lexical_analysis_result.emplace_back(token_type::string,"");
      } else if (ct==char_type::symbol&&c=='\''){
        if (last.first==token_type::char_) lexical_analysis_result.emplace_back(token_type::unknown,"");
        else lexical_analysis_result.emplace_back(token_type::char_,"");
      } else if (last.first==token_type::variables){
      }
    }
  }
}

int main(int argc, char* argv[]){
  args.set_arg(argc,argv);
  if (args.contains("--args_test")){
    std::cout << "Input args test [because --args_test is setting]" << std::endl;
    for (auto& a:args.AV) std::cout << a << std::endl;
  }
  filename = args.AV[0];
  std::ifstream ifs(filename);
  std::string tmp_input;
  while(std::getline(ifs,tmp_input)) alvs_code.emplace_back(tmp_input);
  if (args.contains("--codeinput_test")){
    std::cout << "Input args test [because --codeinput_test is setting]" << std::endl;
    for (auto& a:alvs_code) std::cout << a << std::endl;
  }
  lexical_analysis();
  return exit_code;
}