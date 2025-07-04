#include<cstdlib>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

class terminal_c;
class file_c;
enum opentype_e;

terminal_c terminal;

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
	file.read();

	while(true){
		int key=terminal.read();
		switch(key){
			case 'q':
				goto RETURN_TRUE;
			case '-':
				std::string cmd;
				std::getline(std::cout,cmd);
				if(cmd=="q"){
					goto RETURN_TRUE;
				}
				break;
		}
	}

RETURN_TRUE:
	file.read();
	return 0;
}

class terminal_c{
	private:
		unsigned int col;
		unsigned int row;
	public:
		terminal_c(){
			set_size();
		}
		void clear(); // nonportable
		char read(); // nonportable
		void set_size(); // nonportable
};

class file_c{
	private:
		open_mode_e mode;
		std::string path;
		std::vector<std::string> token;
	public:
		void read(){
			std::string line;
			std::ifstream ifs(path);
			if(!ifs){
				token.push_back("\n");
				return 0;
			}
			while(std::getline(ifs,line)){
				token.push_back("");
				for(char c:line){
					if(c==' '||c=='\t'){
						if(token[token.size()-1][0]!=' '||token[token.size()-1][0]!='\t'){
							token.push_back("");
						}
						token[token.size()-1]+=c;
					}else{
						if(token[token.size()-1][0]==' '||token[token.size()-1][0]=='\t'){
							token.push_back("");
						}
						token[token.size()-1]+=c;
					}
					token.push_back("\n");
				}
			}
		}
		void set_mode(open_mode_e mode){
			this->mode=mode
		}
		void set_path(std::string path){
			this->path=path;
		}
		void write(){
			if(mode!=OM_WRITE){
				return;
			}
			std::ofstream ofs(path);
			for(std::string t:token){
				ofs<<t;
			}
		}
};

enum open_mode_e{
	OM_READONLY,
	OM_WRITE,
};

 /* nonportable part */
#if defined(__linux__)
#	include<sys/ioctl.h>
#	include<termios.h>
#	include<unistd.h>
#elif defined(_WIN32)||defined(_WIN64)
#	include<windows.h>
#endif

void terminal_c::clear(){
#if defined(__linux__)
	std::system("clear");
#elif defined(_WIN32)||defined(_WIN64)
	std::system("cls");
#endif
}

void terminal_c::read(){
#if defined(__linux__)
	struct termios t;
	tcgetattr(STDIN_FILENO,&t);
	t.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&t);
#elif defined(_WIN32)||defined(_WIN64)
	DWORD d;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&m);
	m&=~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),m);
#endif
	int c=std::cin.getchar();
#if defined(__linux__)
	t.c_lflag|=(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&t);
#elif defined(_WIN32)||defined(_WIN64)
	m|=(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),m);
#endif
	return const_cast<char>(c);
}

void terminal_c::set_size(){
#if defined(__linux__)
	struct winsize ws;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&ws);
	col=ws.ws_col;
	row=ws.ws_row;
#elif defined(_WIN32)||defined(_WIN64)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	col=csbi.srWindow.Right-csbi.srWindow.Left+1;
	row=csbi.srWindow.Bottom-csbi.srWindow.Top+1;
#endif
}
