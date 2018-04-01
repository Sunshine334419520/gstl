# @Author: YangGuang <sunshine>
# @Date:   2018-02-28T11:48:06+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2018-04-01T10:37:53+08:00

RESULT = a.out
CC = g++
OBJS =	test.o string.o
FLAGS = -Wall -std=c++11 -g -I../../String/	-I../../

$(RESULT): $(OBJS)
	$(CC) $(FLAGS) -o $(RESULT) $(OBJS)

test.o : test.cc
	$(CC) $(FLAGS) -c $<

string.o : ../../String/String.cc
	$(CC) $(FLAGS) -c $<
.PHONY: clean
clean:
	rm -rf $(RESULT) $(OBJS)
