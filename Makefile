

PYTHON3 = /usr/bin/python3
CXX = g++

test: test_py test_cc

test_py:
	$(PYTHON3) ./tests.py

test_cc:
	$(CXX) -Wall -Wextra -Werror -std=c++20 -pedantic -o tests pretty.cc tests.cc
	./tests
