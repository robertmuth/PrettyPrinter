

PYTHON3 = /usr/bin/python3
CXX = g++

test: test_py test_cc

test_py:
	$(PYTHON3) ./tests.py

test_cc:
	$(CXX) -Wall -Wextra -Werror -std=c++17 -o tests tests.cc pretty.cc
	./tests