# @Author: YangGuang <sunshine>
# @Date:   2018-02-28T11:48:06+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2018-03-22T12:33:55+08:00

RESULT = a.out
CC = g++
OBJS = test.cc
FLAGS = -Wall -std=c++11 -g -I../../../

$(RESULT): $(OBJS)
	$(CC) $(FLAGS) -o $(RESULT) $(OBJS)

.PHONY: clean
clean:
	rm -rf $(RESULT)
