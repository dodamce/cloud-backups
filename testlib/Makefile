.PHONY:Json
Json:
	g++ -o TestJson json_example.cpp -ljsoncpp -std=c++11

.PHONY:Bundle
Bundle:
	g++ -o TestBundle bundle_example.cpp ../bundle/bundle.cpp  -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf TestJson TestBundle 