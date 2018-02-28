# @Author: YangGuang <sunshine>
# @Date:   2018-02-28T11:48:06+08:00
# @Email:  guang334419520@126.com
# @Filename: Makefile
# @Last modified by:   sunshine
# @Last modified time: 2018-02-28T12:40:57+08:00

RESULT = a.out
CC = g++
OBJS = copy.cc
FLAGS = -Wall -std=c++11 -g -I../../../

$(RESULT): $(OBJS)
	$(CC) $(FLAGS) -o $(RESULT) $(OBJS)

.PHONY: clean
clean:
	rm -rf $(RESULT)
