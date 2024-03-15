.PHONY:Json
Json:
	g++ -o TestJson json_example.cpp -ljsoncpp -std=c++11

.PHONY:Bundle-Zip
Bundle:
	g++ -o TestBundle bundle_example.cpp ../bundle/bundle.cpp  -std=c++11 -lpthread
	mkdir test-bundle-zip
	./TestBundle TestBundle ./test-bundle-zip/TestBundle.lz

.PHONY:clean
clean:
	rm -rf TestJson TestBundle test-bundle-zip/