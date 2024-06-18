#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>

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

int exit_code = 0; std::string filename; std::vector<std::string> alvs_code;

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
  return exit_code;
}
