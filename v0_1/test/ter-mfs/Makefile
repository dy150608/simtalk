Home=/home/lzx
Project=simple-chat/
Main=$(Home)+$(Project)
Target=local_msg
$(Target):
	g++ -D _DEBUG_ ter-mfs.cpp /home/lzx/simple-chat/src/mfs/local_msg.cpp /home/lzx/simple-chat/tool/for_simple.cpp -o $(Target) -std=c++11 

clean:
	rm *.o a.out
