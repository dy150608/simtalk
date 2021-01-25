Home=/home/lzx
Project=simple-chat/
Main=$(Home)+$(Project)
Target=schat

Account=$(wildcard account/*.cpp) #匹配指定目录下的文件
Database=$(wildcard database/*.cpp)
Message=$(wildcard message/*.cpp)
Network=$(wildcard network/*.cpp)
main=$(wildcard *.cpp)

SRC:=$(Account) $(Database) $(Message) $(Network) $(main)

MYSQL=/usr/include/mysql

$(Target):$(OBJ)
	g++ -D _DEBUG_ $^ -o $@ -std=c++11 -I $(MYSQL) `mysql_config --libs`

%.o:%.cpp
	g++ -c $< -std=c++11 -o $@ #模式匹配

clean:
	rm $(Target)
