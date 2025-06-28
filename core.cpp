#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>

class file_c;
enum opentype_e;

int main(int argc,char**argv){
	file_c file;
	if(argc<2){
		file.set_mode(OM_WRITE);
		std::ostringstream path;
		for(int i=0;1;i++){
			path << "untitled-" << i << ".txt";
			std::ifstream test(path);
			if(!test){
				test.close();
				file.set_path(path);
			}
		}
	}else{
		for(int i=1;i<argc;i++){
			std::string arg=argv[i];
			if(arg=="-om:readonly"){
				file.set_mode(OM_READONLY);
			}if else(arg=="-om:write"){
				file.set_mode(OM_WRITE);
			}else{
				file.set_path(arg);
			}
		}
	}
	return 0;
}

class file_c{
	private:
		open_mode_e mode;
		std::string path;
	public:
		void set_mode(open_mode_e mode){
			this->mode=mode;
		}
		void set_path(std::string path){
			this->path=path;
		}
};

enum open_mode_e{
	OM_READONLY,
	OM_WRITE,
};
