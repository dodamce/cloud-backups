.PHONY:Json
Json:
	g++ -o TestJson json_example.cpp -ljsoncpp -std=c++11

.PHONY:Bundle-Zip
Bundle:
	g++ -o TestBundleZip bundle_example_zip.cpp ../bundle/bundle.cpp  -std=c++11 -lpthread
	mkdir test-bundle-zip
	./TestBundle TestBundle ./test-bundle-zip/TestBundle.lz

.PHONY:clean
clean:
	rm -rf TestJson TestBundleZip test-bundle-zip/