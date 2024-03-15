.PHONY:Json
Json:
	g++ -o TestJson json_example.cpp -ljsoncpp -std=c++11
	./TestJson
	mv TestJson ./test-exe

.PHONY:Bundle-Zip
Bundle-Zip:
	g++ -o TestBundleZip bundle_example_zip.cpp ../bundle/bundle.cpp  -std=c++11 -lpthread
	mkdir test-bundle-zip
	./TestBundleZip TestBundleZip ./test-bundle-zip/TestBundleZip.lz
	mv TestBundleZip ./test-exe

.PHONY:Bundle-UnZip
Bundle-UnZip:
	g++ -o TestBundleUnZip bundle_example_unzip.cpp ../bundle/bundle.cpp -std=c++11 -lpthread
	mkdir test-bundle-unzip
	./TestBundleUnZip ./test-bundle-zip/TestBundleZip.lz ./test-bundle-unzip/TestBundleZip
	mv TestBundleUnZip ./test-exe

.PHONY:Http
Http:
	rm -rf test-http-server
	mkdir test-http-server
	g++ -o HttpServer http_sever_example.cpp -lpthread -std=c++11
	mv HttpServer ./test-http-server/
	g++ -o HttpClient http_client_example.cpp -lpthread -std=c++11
	mv HttpClient ./test-http-server/

.PHONY:clean
clean:
	rm -rf test-bundle-zip/ test-bundle-unzip/ ./test-exe/* test-http-server/