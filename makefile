#Ben Borgstede and Anthony Van

classifier: classifier.cpp typeCheck.cpp
	c++11 classifier.cpp typeCheck.cpp -o classifier

typeCheck.o: typeCheck.cpp
	c++11 -c typeCheck.cpp -o typeCheck.o
